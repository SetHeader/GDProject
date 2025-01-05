// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryManagerComponent.h"

#include "InventoryStatics.h"
#include "Engine/ActorChannel.h"
#include "GDProject/GDProject.h"
#include "Inventory/InventoryItemInstance.h"
#include "Net/UnrealNetwork.h"
#include "GameplayTagContainer.h"

bool UInventoryManagerComponent::IsValidItemSlot(const FInventoryItemSlotHandle& Handle) const {
	auto Slot = GetItemSlotPtr(Handle);
	return Slot != nullptr;
}

FInventoryItemSlot UInventoryManagerComponent::GetItemSlot(const FInventoryItemSlotHandle& Handle) {
	auto SlotPtr = GetItemSlotPtr(Handle);
	return SlotPtr ? *SlotPtr : FInventoryItemSlot::EmptySlot;
}

int32 UInventoryManagerComponent::AddItem(UInventoryItemInstance* Item, int32 Count) {
	if (!HasAuthority()) {
		return 0;
	}
	
	if (!IsValid(Item) || Count <= 0)
		return 0;
	
	int32 CountPlaced = 0;
	for (auto& Slot : Slots) {
		CountPlaced += AddItemToSlot(Item, Slot, Count - CountPlaced);
		if (CountPlaced == Count)
			break;
	}

	return CountPlaced;
}

int32 UInventoryManagerComponent::AddItemToSlot(UInventoryItemInstance* Item,
	const FInventoryItemSlotHandle& SlotHandle,
	int32 Count) {
	if (!HasAuthority()) {
		return 0;
	}
	
	if (!IsValid(Item) || !IsValidItemSlot(SlotHandle)) 
		return 0;

	if (Count <= 0) {
		Logger.Warning(TEXT("AddItemToSlot，Count不能<=0"));
		return 0;
	}
	
	FInventoryItemSlot* Slot = GetItemSlotPtr(SlotHandle);
	if (!IsItemAcceptable(Item, *Slot)) {
		Logger.Warning(TEXT("不能放入物品，%s, %s"), *Slot->ToString(), *Item->ToString());
		return 0;
	}
	FItemSlotChangedData ChangedData;
	ChangedData.InventoryComponent = this;
	ChangedData.SlotHandle = SlotHandle;
	
	UInventoryItemInstance* PreviousItem = Slot->ItemInstance;
	
	ChangedData.PreviousItem = Slot->ItemInstance;
	ChangedData.PreviousStackCount = Slot->StackCount;
	
	int32 CountReplaced;
	
	Slot->ItemInstance = Item;
	if (PreviousItem == Slot->ItemInstance) {
		// 堆叠
		int32 RemainingStackCount = Slot->GetRemainingStackCount();
		CountReplaced = FMath::Min(RemainingStackCount, Count);
		Slot->StackCount += CountReplaced;
	} else {
		// 新增或替换
		CountReplaced = FMath::Min3(Item->GetMaxStackCount(), Slot->MaxStackCount, Count);
		Slot->StackCount = CountReplaced;
	}
	
	ChangedData.CurrentItem = Slot->ItemInstance;
	ChangedData.CurrentStackCount = Slot->StackCount;
	
	if (!ChangedData.PreviousItem || ChangedData.PreviousItem == ChangedData.CurrentItem)
		ChangedData.Operation = EItemSlotOperation::ADD;
	else
		ChangedData.Operation = EItemSlotOperation::REPLACE;
	
	OnItemSlotAddedOrRemovedDelegate.Broadcast(this);
	OnItemSlotUpdatedDelegate.Broadcast(ChangedData);
	OnItemSlotChanged.Broadcast();
	return CountReplaced;
}

void UInventoryManagerComponent::RemoveItemFromSlot(const FInventoryItemSlotHandle& SlotHandle, int32 Count, FInventoryRemovedItemInfo& OutRemovedItemInfo) {
	if (!HasAuthority()) {
		return;
	}
	
	if (!SlotHandle.IsValid() || Count <= 0) {
		Logger.Warning(TEXT("RemoveItemFromSlot，错误Count<=0"));
		return;
	}
	
	FInventoryItemSlot* Slot = GetItemSlotPtr(SlotHandle);
	if (!Slot->ItemInstance) {
		// 插槽中没有物品
		OutRemovedItemInfo.ItemInstance = nullptr;
		OutRemovedItemInfo.Count = 0;
		return;
	}

	if (Slot->StackCount < Count) {
		Logger.Warning(TEXT("RemoveItemFromSlot，物品数量不足"));
		return;
	}
	
	FItemSlotChangedData ChangedData;
	ChangedData.SlotHandle = SlotHandle;
	ChangedData.InventoryComponent = this;
	ChangedData.Operation = EItemSlotOperation::REMOVE;
	ChangedData.PreviousItem = Slot->ItemInstance;
	ChangedData.PreviousStackCount = Slot->StackCount;

	int32 RemovedCount = Slot->StackCount >= Count ? Count : Slot->StackCount;
	Slot->StackCount -= RemovedCount;
	if (Slot->StackCount == 0)
		Slot->ItemInstance = nullptr;
	
	ChangedData.CurrentItem = Slot->ItemInstance;
	ChangedData.CurrentStackCount = Slot->StackCount;

	OutRemovedItemInfo.ItemInstance = ChangedData.PreviousItem;
	OutRemovedItemInfo.Count = RemovedCount;
	
	OnItemSlotAddedOrRemovedDelegate.Broadcast(this);
	OnItemSlotUpdatedDelegate.Broadcast(ChangedData);
	OnItemSlotChanged.Broadcast();
}

void UInventoryManagerComponent::RemoveAllItemFromSlot(const FInventoryItemSlotHandle& SlotHandle, FInventoryRemovedItemInfo& OutRemovedItemInfo) {
	if (!HasAuthority()) {
		return;
	}
	if (FInventoryItemSlot* Slot = GetItemSlotPtr(SlotHandle))
		RemoveItemFromSlot(SlotHandle, Slot->StackCount, OutRemovedItemInfo);
}

bool UInventoryManagerComponent::IsItemAcceptable(const UInventoryItemInstance* Item, const FInventoryItemSlotHandle& SlotHandle) const {
	if (!IsValid(Item) || !SlotHandle.IsValid())
		return false;

	auto* Slot = GetItemSlotPtr(SlotHandle);
	return Slot && IsItemAcceptable(Item, *Slot);
}

bool UInventoryManagerComponent::IsItemAcceptable(const UInventoryItemInstance* Item,
	const FInventoryItemSlot& Slot) const {
	if (!IsValid(Item) || !Slot.IsValid())
		return false;
	
	if (!Slot.ItemInstance && Slot.GetMaxStackCount() > 0)
		return true;
	
	return Slot.ItemInstance == Item && Slot.StackCount < Slot.MaxStackCount && Slot.StackCount < Item->GetMaxStackCount();
}

void UInventoryManagerComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryManagerComponent, Slots);
}

bool UInventoryManagerComponent::ReplicateSubobjects(class UActorChannel* Channel,
	class FOutBunch* Bunch,
	FReplicationFlags* RepFlags) {
	bool Result = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (auto& Slot : Slots) {
		Result |= Channel->ReplicateSubobject(Slot.ItemInstance, *Bunch, *RepFlags);
	}
	return Result;
}

void UInventoryManagerComponent::OnRep_Slots() {
	OnItemSlotChanged.Broadcast();
}

TArray<FInventoryItemSlot> UInventoryManagerComponent::GetAllItemSlots() {
	return Slots;
}

TArray<FInventoryItemSlotHandle> UInventoryManagerComponent::GetAllItemSlotHandles() {
	TArray<FInventoryItemSlotHandle> Result;

	for (auto& Slot : Slots) {
		Result.Add(FInventoryItemSlotHandle(Slot));
	}

	return Result;
}

FInventoryItemSlotHandle UInventoryManagerComponent::CreateItemSlotHandle(const FInventoryItemSlot& ItemSlot) {
	if (!HasAuthority()) {
		return FInventoryItemSlotHandle::EmptyHandle;
	}
	FInventoryItemSlot* FoundSlot = Algo::FindByPredicate(Slots, [&ItemSlot](const FInventoryItemSlot& Item) {
		return ItemSlot.SlotId == Item.SlotId;
	});
	return FInventoryItemSlotHandle(*FoundSlot);
}

void UInventoryManagerComponent::ServerUseItem_Implementation(UAbilitySystemComponent* TargetASC,
	FInventoryItemSlotHandle SlotHandle,
	int32 AmountToUse) {
	UInventoryStatics::UseItem(TargetASC, SlotHandle, AmountToUse);
}

void UInventoryManagerComponent::ServerMoveItemWithSlot_Implementation(FInventoryItemSlotHandle FromSlotHandle,
	FInventoryItemSlotHandle ToSlotHandle,
	int32 AmountToMove) {
	UInventoryStatics::MoveItemWithSlot(FromSlotHandle, ToSlotHandle, AmountToMove);
}

void UInventoryManagerComponent::ServerSwapItem_Implementation(FInventoryItemSlotHandle FromSlotHandle,
	FInventoryItemSlotHandle ToSlotHandle) {
	UInventoryStatics::SwapItem(FromSlotHandle, ToSlotHandle);
}

void UInventoryManagerComponent::ServerMoveItem_Implementation(FInventoryItemSlotHandle FromSlotHandle, UInventoryManagerComponent* ToInventoryComponent, int32 AmountToMove) {
	UInventoryStatics::MoveItem(FromSlotHandle, ToInventoryComponent, AmountToMove);
}

TArray<FInventoryItemSlotHandle> UInventoryManagerComponent::GetItemSlotsByTag(const FGameplayTag& Tag) {
	return GetItemSlotsByTags(FGameplayTagContainer(Tag));
}

TArray<FInventoryItemSlotHandle> UInventoryManagerComponent::GetItemSlotsByTags(const FGameplayTagContainer& Tags) {
	TArray<FInventoryItemSlotHandle> Result;
	
	for (FInventoryItemSlot& Slot : Slots) {
		if (Tags.HasAny(Slot.SlotTags))
			Result.Emplace(Slot);
	}

	return Result;
}
FInventoryItemSlot* UInventoryManagerComponent::GetItemSlotPtr(const FInventoryItemSlotHandle& Handle) {
	return const_cast<FInventoryItemSlot*>(
		static_cast<const UInventoryManagerComponent*>(this)->GetItemSlotPtr(Handle)
	);
}

const FInventoryItemSlot* UInventoryManagerComponent::GetItemSlotPtr(const FInventoryItemSlotHandle& Handle) const {
	if (!Handle.IsValid() || Handle.GetOwner() != this)
		return nullptr;
	
	int32 SlotId = Handle.GetSlotId();
	for (auto& Slot : Slots) {
		if (Slot.SlotId == SlotId)
			return &Slot;
	}
	
	return nullptr;
}

int32 UInventoryManagerComponent::GenerateNextSlotId() const {
	return SlotIdGenerator.IncrementExchange();
}

bool UInventoryManagerComponent::HasAuthority() const {
	return GetOwner() && GetOwner()->HasAuthority();
}

UInventoryManagerComponent::UInventoryManagerComponent(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer) {
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);
}

void UInventoryManagerComponent::InitializeComponent() {
	Super::InitializeComponent();
	
	for (auto& Slot : Slots) {
		Slot.SlotId = GenerateNextSlotId();
		Slot.Owner = this;
	}
}

FInventoryItemSlotHandle UInventoryManagerComponent::CreateInventorySlot(const FGameplayTagContainer& SlotTags) {
	if (!HasAuthority()) {
		return FInventoryItemSlotHandle::EmptyHandle;
	}
	return CreateInventorySlotWithMaxStackCount(SlotTags, INT32_MAX);
}

FInventoryItemSlotHandle UInventoryManagerComponent::CreateInventorySlotWithMaxStackCount(const FGameplayTagContainer& SlotTags, int32 MaxStackCount) {
	if (!HasAuthority()) {
		return FInventoryItemSlotHandle::EmptyHandle;
	}
	FInventoryItemSlot Slot = Slots.Emplace_GetRef();
	Slot.SlotTags = SlotTags;
	Slot.SlotId = GenerateNextSlotId();
	Slot.StackCount = 0;
	Slot.Owner = this;
	Slot.MaxStackCount = MaxStackCount;

	OnItemSlotAddedOrRemovedDelegate.Broadcast(this);
	return FInventoryItemSlotHandle(Slot);
}

bool UInventoryManagerComponent::RemoveInventorySlot(const FInventoryItemSlotHandle& Handle) {
	if (!HasAuthority()) {
		return false;
	}
	if (!IsValidItemSlot(Handle)) 
		return false;
	
	Slots.Remove(GetItemSlot(Handle));
	OnItemSlotAddedOrRemovedDelegate.Broadcast(this);
	return true;
}