// Copyright 2020 Dan Kestranek.


#include "AbilitySystem//AttributeSets/GDAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "GameplayEffect.h"
#include "GDGameplayTags.h"
#include "AbilitySystem/GDAbilitySystemLibrary.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/PlayerInterface.h"
#include "Player/GDPlayerController.h"
#include "AbilitySystem/GDAbilityTypes.h"
#include "GameplayEffectComponents\TargetTagsGameplayEffectComponent.h"

struct FGDGameplayEffectContext;

UGDAttributeSet::UGDAttributeSet()
{
	Tag2AttributeFuncMap.Add(FGDGameplayTags::Get().Attribute_Primary_Strength, GetStrengthAttribute);
	Tag2AttributeFuncMap.Add(FGDGameplayTags::Get().Attribute_Primary_Intelligence, GetIntelligenceAttribute);
	Tag2AttributeFuncMap.Add(FGDGameplayTags::Get().Attribute_Primary_Resilience, GetResilienceAttribute);
	Tag2AttributeFuncMap.Add(FGDGameplayTags::Get().Attribute_Primary_Vigor, GetVigorAttribute);
	
	Tag2AttributeFuncMap.Add(FGDGameplayTags::Get().Attribute_Secondary_Armor, GetArmorAttribute);
	Tag2AttributeFuncMap.Add(FGDGameplayTags::Get().Attribute_Secondary_ArmorPenetration, GetArmorPenetrationAttribute);
	Tag2AttributeFuncMap.Add(FGDGameplayTags::Get().Attribute_Secondary_BlockChange, GetBlockChangeAttribute);
	Tag2AttributeFuncMap.Add(FGDGameplayTags::Get().Attribute_Secondary_CriticalHitChange, GetCriticalHitChangeAttribute);
	Tag2AttributeFuncMap.Add(FGDGameplayTags::Get().Attribute_Secondary_CriticalHitDamage, GetCriticalHitDamageAttribute);
	Tag2AttributeFuncMap.Add(FGDGameplayTags::Get().Attribute_Secondary_CriticalHitResistance, GetCriticalHitResistanceAttribute);
	Tag2AttributeFuncMap.Add(FGDGameplayTags::Get().Attribute_Secondary_HealthRegeneration, GetHealthRegenerationAttribute);
	Tag2AttributeFuncMap.Add(FGDGameplayTags::Get().Attribute_Secondary_ManaRegeneration, GetManaRegenerationAttribute);
	Tag2AttributeFuncMap.Add(FGDGameplayTags::Get().Attribute_Secondary_MaxHealth, GetMaxHealthAttribute);
	Tag2AttributeFuncMap.Add(FGDGameplayTags::Get().Attribute_Secondary_MaxMana, GetMaxManaAttribute);
	Tag2AttributeFuncMap.Add(FGDGameplayTags::Get().Attribute_Secondary_MaxStamina, GetMaxStaminaAttribute);

	Tag2AttributeFuncMap.Add(FGDGameplayTags::Get().Attribute_Vital_Health, GetHealthAttribute);
	Tag2AttributeFuncMap.Add(FGDGameplayTags::Get().Attribute_Vital_Mana, GetManaAttribute);
	Tag2AttributeFuncMap.Add(FGDGameplayTags::Get().Attribute_Vital_Stamina, GetStaminaAttribute);
	
	Tag2AttributeFuncMap.Add(FGDGameplayTags::Get().Attribute_Resistance_Fire, GetFireResistanceAttribute);
	Tag2AttributeFuncMap.Add(FGDGameplayTags::Get().Attribute_Resistance_Lighting, GetLightingResistanceAttribute);
	Tag2AttributeFuncMap.Add(FGDGameplayTags::Get().Attribute_Resistance_Arcane, GetArcaneResistanceAttribute);
	Tag2AttributeFuncMap.Add(FGDGameplayTags::Get().Attribute_Resistance_Physical, GetPhysicalResistanceAttribute);
}

void UGDAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	// 限制属性范围，注意，这只影响Get属性返回的值，实际值不会变化
	if (Attribute == GetHealthAttribute()) {
		NewValue = FMath::Clamp(NewValue, 0, GetMaxHealth());
	}
	else if (Attribute == GetManaAttribute()) {
		NewValue = FMath::Clamp(NewValue, 0, GetMaxMana());
	}
	else if (Attribute == GetStaminaAttribute()) {
		NewValue = FMath::Clamp(NewValue, 0, GetMaxStamina());
	}
}

void UGDAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	FEffectProperties Props;
	SetEffectProperties(Data, Props);

	// 限制属性范围，真正限制实际属性值的地方
	if (Data.EvaluatedData.Attribute == GetHealthAttribute()) {
		SetHealth(FMath::Clamp(GetHealth(), 0, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetManaAttribute()) {
		SetMana(FMath::Clamp(GetMana(), 0, GetMaxMana()));
	}
	else if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute()) {
		HandleIncomingDamage(Props);
	}
	else if (Data.EvaluatedData.Attribute == GetIncomingXPAttribute())
	{
		HandleIncomingXP(Props);
	}
}

void UGDAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	if (Attribute == GetMaxHealthAttribute() && bTopOffHealth)
	{
		bTopOffHealth = false;
		SetHealth(GetMaxHealth());
	}
	else if (Attribute == GetMaxManaAttribute() && bTopOffMana)
	{
		bTopOffMana = false;
		SetMana(GetMaxMana());
	}
}

void UGDAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UGDAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGDAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGDAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGDAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGDAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGDAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGDAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGDAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGDAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGDAttributeSet, Vigor, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UGDAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGDAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGDAttributeSet, BlockChange, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGDAttributeSet, CriticalHitChange, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGDAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGDAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGDAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGDAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UGDAttributeSet, FireResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGDAttributeSet, LightingResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGDAttributeSet, ArcaneResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGDAttributeSet, PhysicalResistance, COND_None, REPNOTIFY_Always)
}

void UGDAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& InData, FEffectProperties& OutProps)
{
	OutProps.EffectContextHandle = InData.EffectSpec.GetContext();
	OutProps.SourceASC = InData.EffectSpec.GetContext().GetOriginalInstigatorAbilitySystemComponent();
	OutProps.SourceAvatarActor = OutProps.SourceASC->GetAvatarActor();

	if (ACharacter* Character = Cast<ACharacter>(OutProps.SourceAvatarActor)) {
		OutProps.SourceCharacter = Character;
		OutProps.SourceController = Character->GetController();
	}

	OutProps.TargetASC = &InData.Target;
	OutProps.TargetAvatarActor = InData.Target.GetAvatarActor();
	if (ACharacter* Character = Cast<ACharacter>(OutProps.TargetAvatarActor)) {
		OutProps.TargetCharacter = Character;
		OutProps.TargetController = Character->GetController();
	}
}

void UGDAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGDAttributeSet, Strength, OldStrength);
}

void UGDAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGDAttributeSet, Intelligence, OldIntelligence);
}

void UGDAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGDAttributeSet, Resilience, OldResilience);
}

void UGDAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGDAttributeSet, Vigor, OldVigor);
}

void UGDAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGDAttributeSet, Armor, OldArmor);
}

void UGDAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGDAttributeSet, ArmorPenetration, OldArmorPenetration);
}

void UGDAttributeSet::OnRep_BlockChange(const FGameplayAttributeData& OldBlockChange)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGDAttributeSet, BlockChange, OldBlockChange);
}

void UGDAttributeSet::OnRep_CriticalHitChange(const FGameplayAttributeData& OldCriticalHitChange)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGDAttributeSet, CriticalHitChange, OldCriticalHitChange);
}

void UGDAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGDAttributeSet, CriticalHitDamage, OldCriticalHitDamage);
}

void UGDAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGDAttributeSet, CriticalHitResistance, OldCriticalHitResistance);
}

void UGDAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGDAttributeSet, HealthRegeneration, OldHealthRegeneration);
}

void UGDAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGDAttributeSet, ManaRegeneration, OldManaRegeneration);
}

void UGDAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGDAttributeSet, Health, OldHealth);
}

void UGDAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGDAttributeSet, MaxHealth, OldMaxHealth);
}

void UGDAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGDAttributeSet, Mana, OldMana);
}

void UGDAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGDAttributeSet, MaxMana, OldMaxMana);
}

void UGDAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGDAttributeSet, Stamina, OldStamina);
}

void UGDAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGDAttributeSet, MaxStamina, OldMaxStamina);
}

void UGDAttributeSet::OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGDAttributeSet, FireResistance, OldFireResistance);
}

void UGDAttributeSet::OnRep_LightingResistance(const FGameplayAttributeData& OldLightingResistance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGDAttributeSet, LightingResistance, OldLightingResistance);
}

void UGDAttributeSet::OnRep_ArcaneResistance(const FGameplayAttributeData& OldArcaneResistance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGDAttributeSet, ArcaneResistance, OldArcaneResistance);
}

void UGDAttributeSet::OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGDAttributeSet, PhysicalResistance, OldPhysicalResistance);
}

void UGDAttributeSet::SendXPEvent(const FEffectProperties& Props)
{
	if (Props.TargetCharacter->Implements<UCombatInterface>())
	{
		const int32 TargetLevel = ICombatInterface::Execute_GetPlayerLevel(Props.TargetCharacter);
		// GetCharacterClass是BlueprintNativeEvent方法，故需要调静态方法获才行
		const ECharacterClass CharacterClass = ICombatInterface::Execute_GetCharacterClass(Props.TargetCharacter);
		const int32 XPReward = UGDAbilitySystemLibrary::GetXPRewardForClassAndLevel(Props.TargetCharacter, CharacterClass, TargetLevel);

		FGameplayEventData EventData;
		EventData.EventTag = FGDGameplayTags::Get().Attribute_Meta_IncomingXP;
		EventData.EventMagnitude = XPReward;
		
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			Props.SourceCharacter,
			FGDGameplayTags::Get().Attribute_Meta_IncomingXP,
			EventData);
	}
}
void UGDAttributeSet::ShowFloatingText(const FEffectProperties& Props, float Damage, bool bBlockedHit, bool bCriticalHit) const
{
	if (Props.SourceCharacter != Props.TargetCharacter)
	{
		if(AGDPlayerController* PC = Cast<AGDPlayerController>(Props.SourceCharacter->Controller))
		{
			PC->Client_ShowDamageNumber(Damage, Props.TargetCharacter, bBlockedHit, bCriticalHit);
			return;
		}
		if(AGDPlayerController* PC = Cast<AGDPlayerController>(Props.TargetCharacter->Controller))
		{
			PC->Client_ShowDamageNumber(Damage, Props.TargetCharacter, bBlockedHit, bCriticalHit);
		}
	}
}

void UGDAttributeSet::HandleIncomingDamage(const FEffectProperties& Props)
{
	// 已经死亡就不处理伤害
	if (ICombatInterface::Execute_IsDead(Props.TargetAvatarActor))
	{
		return;
	}
	
	const float LocalIncomingDamage = GetIncomingDamage();
	SetIncomingDamage(0.f);
	if (LocalIncomingDamage > 0.f)
	{
		const float NewHealth = GetHealth() - LocalIncomingDamage;
		SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

		const bool bFatal = NewHealth <= 0.f;
		// 致命伤就触发角色死亡
		if (bFatal)
		{
			ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetAvatarActor);
			if (CombatInterface)
			{
				CombatInterface->Die();
			}
			if (Props.SourceCharacter->Implements<UPlayerInterface>()) {
				// 给攻击者增加经验
				SendXPEvent(Props);
			}
		}
		// 非致命伤就触发受击反应
		else
		{
			FGameplayTagContainer TagContainer;
			TagContainer.AddTag(FGDGameplayTags::Get().Effects_HitReact);
			Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);

			const FVector& KnockbackForce = UGDAbilitySystemLibrary::GetKnockbackForce(Props.EffectContextHandle);
			if (!KnockbackForce.IsNearlyZero(1.f))
			{
				Props.TargetCharacter->LaunchCharacter(KnockbackForce, true, true);
			}
		}
		
		const bool bBlock = UGDAbilitySystemLibrary::IsBlockedHit(Props.EffectContextHandle);
		const bool bCriticalHit = UGDAbilitySystemLibrary::IsCriticalHit(Props.EffectContextHandle);
		// 显示伤害文本
		ShowFloatingText(Props, LocalIncomingDamage, bBlock, bCriticalHit);
		// 应用Debuff
		if (UGDAbilitySystemLibrary::IsSuccessfulDebuff(Props.EffectContextHandle))
		{
			Debuff(Props);
		}
	}
}

void UGDAttributeSet::HandleIncomingXP(const FEffectProperties& Props)
{
	const float LocalIncomingXP = GetIncomingXP();
	SetIncomingXP(0.f);
	
	if (Props.SourceCharacter->Implements<UPlayerInterface>() && Props.SourceCharacter->Implements<UCombatInterface>())
	{
		const int32 CurrentLevel = ICombatInterface::Execute_GetPlayerLevel(Props.SourceCharacter);
		const int32 CurrentXP = IPlayerInterface::Execute_GetXP(Props.SourceCharacter);
		
		const int32 NewLevel = IPlayerInterface::Execute_FindLevelForXP(Props.SourceCharacter, CurrentXP + LocalIncomingXP);
		const int32 NumLevelUps = NewLevel - CurrentLevel;
		if (NumLevelUps > 0)
		{
			const int32 AttributePointsReward = IPlayerInterface::Execute_GetAttributePointsReward(Props.SourceCharacter, CurrentLevel);
			const int32 SpellPointsReward = IPlayerInterface::Execute_GetSpellPointsReward(Props.SourceCharacter, CurrentLevel);
				
			IPlayerInterface::Execute_AddToPlayerLevel(Props.SourceCharacter, NumLevelUps);
			IPlayerInterface::Execute_AddToAttributePoints(Props.SourceCharacter, AttributePointsReward);
			IPlayerInterface::Execute_AddToSpellPoints(Props.SourceCharacter, SpellPointsReward);
	
			bTopOffHealth = true;
			bTopOffMana = true;
			
			IPlayerInterface::Execute_LevelUp(Props.SourceCharacter);
		}
		
		IPlayerInterface::Execute_AddToXP(Props.SourceCharacter, LocalIncomingXP);
	}
}

void UGDAttributeSet::Debuff(const FEffectProperties& Props)
{
	const FGDGameplayTags& GameplayTags = FGDGameplayTags::Get();
	FGameplayEffectContextHandle EffectContext = Props.SourceASC->MakeEffectContext();
	EffectContext.AddSourceObject(Props.SourceAvatarActor);

	const FGameplayTag DamageType = UGDAbilitySystemLibrary::GetDamageType(Props.EffectContextHandle);
	const float DebuffDamage = UGDAbilitySystemLibrary::GetDebuffDamage(Props.EffectContextHandle);
	const float DebuffDuration = UGDAbilitySystemLibrary::GetDebuffDuration(Props.EffectContextHandle);
	const float DebuffFrequency = UGDAbilitySystemLibrary::GetDebuffFrequency(Props.EffectContextHandle);

	// 动态创建一个效果Effect
	FString DebuffName = FString::Printf(TEXT("DynamicDebuff_%s"), *DamageType.ToString());
	UGameplayEffect* Effect = NewObject<UGameplayEffect>();

	Effect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
	Effect->Period = DebuffFrequency;
	Effect->DurationMagnitude = FScalableFloat(DebuffDuration);
	Effect->StackingType = EGameplayEffectStackingType::AggregateBySource;
	Effect->StackLimitCount = 1;
	Effect->StackDurationRefreshPolicy = EGameplayEffectStackingDurationPolicy::RefreshOnSuccessfulApplication;
	Effect->StackPeriodResetPolicy = EGameplayEffectStackingPeriodPolicy::ResetOnSuccessfulApplication;
	Effect->StackExpirationPolicy = EGameplayEffectStackingExpirationPolicy::ClearEntireStack;

	FGameplayModifierInfo ModifierInfo;
	
	ModifierInfo.ModifierMagnitude = FScalableFloat(DebuffDamage);
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	ModifierInfo.Attribute = UGDAttributeSet::GetIncomingDamageAttribute();
	
	Effect->Modifiers.Add(ModifierInfo);
	
	FGameplayTag DebuffTag = GameplayTags.DamageTypesToDebuffs[DamageType];
	
	FInheritedTagContainer TagContainerMods;
	TagContainerMods.Added.AddTag(DebuffTag);
	
	UTargetTagsGameplayEffectComponent& Comp = Effect->AddComponent<UTargetTagsGameplayEffectComponent>();
	Comp.SetAndApplyTargetTagChanges(TagContainerMods);
	
	if (FGameplayEffectSpec* MutableSpec = new FGameplayEffectSpec(Effect, EffectContext, 1.f))
	{
		FGDGameplayEffectContext* AuraContext = static_cast<FGDGameplayEffectContext*>(MutableSpec->GetContext().Get());
		TSharedPtr<FGameplayTag> DebuffDamageType = MakeShareable(new FGameplayTag(DamageType));
		AuraContext->SetDamageType(DebuffDamageType);

		Props.TargetASC->ApplyGameplayEffectSpecToSelf(*MutableSpec);
	}
}
