// Copyright 2020 Dan Kestranek.


#include "AbilitySystem//AttributeSets/GDAttributeSetBase.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "GameplayEffect.h"
#include "GDGameplayTags.h"
#include "GameFramework/Character.h"


UGDAttributeSetBase::UGDAttributeSetBase()
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
}

void UGDAttributeSetBase::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
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

void UGDAttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
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
		const float LocalIncomingDamage = GetIncomingDamage();
		SetIncomingDamage(0.f);
		if (LocalIncomingDamage > 0.f)
		{
			const float NewHealth = GetHealth() - LocalIncomingDamage;
			SetHealth(FMath::Clamp(NewHealth, 0, GetMaxHealth()));
			const bool bFatal = NewHealth <= 0.f;
			// 触发受击反应
			if (!bFatal)
			{
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FGDGameplayTags::Get().Effects_HitReact);
				Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);
			}
		}
	}
}

void UGDAttributeSetBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UGDAttributeSetBase, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGDAttributeSetBase, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGDAttributeSetBase, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGDAttributeSetBase, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGDAttributeSetBase, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGDAttributeSetBase, MaxStamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGDAttributeSetBase, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGDAttributeSetBase, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGDAttributeSetBase, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGDAttributeSetBase, Vigor, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UGDAttributeSetBase, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGDAttributeSetBase, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGDAttributeSetBase, BlockChange, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGDAttributeSetBase, CriticalHitChange, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGDAttributeSetBase, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGDAttributeSetBase, CriticalHitResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGDAttributeSetBase, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGDAttributeSetBase, ManaRegeneration, COND_None, REPNOTIFY_Always);
}

void UGDAttributeSetBase::SetEffectProperties(const FGameplayEffectModCallbackData& InData, FEffectProperties& OutProps)
{
	OutProps.EffectContextHandle = InData.EffectSpec.GetContext();
	OutProps.SourceASC = InData.EffectSpec.GetContext().GetOriginalInstigatorAbilitySystemComponent();
	OutProps.SourceActor = OutProps.SourceASC->GetAvatarActor();

	if (ACharacter* Character = Cast<ACharacter>(OutProps.SourceActor)) {
		OutProps.SourceCharacter = Character;
		OutProps.SourceController = Character->GetController();
	}

	OutProps.TargetASC = &InData.Target;
	OutProps.TargetActor = InData.Target.GetAvatarActor();
	if (ACharacter* Character = Cast<ACharacter>(OutProps.TargetActor)) {
		OutProps.TargetCharacter = Character;
		OutProps.TargetController = Character->GetController();
	}
}

void UGDAttributeSetBase::OnRep_Strength(const FGameplayAttributeData& OldStrength)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGDAttributeSetBase, Strength, OldStrength);
}

void UGDAttributeSetBase::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGDAttributeSetBase, Intelligence, OldIntelligence);
}

void UGDAttributeSetBase::OnRep_Resilience(const FGameplayAttributeData& OldResilience)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGDAttributeSetBase, Resilience, OldResilience);
}

void UGDAttributeSetBase::OnRep_Vigor(const FGameplayAttributeData& OldVigor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGDAttributeSetBase, Vigor, OldVigor);
}

void UGDAttributeSetBase::OnRep_Armor(const FGameplayAttributeData& OldArmor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGDAttributeSetBase, Armor, OldArmor);
}

void UGDAttributeSetBase::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGDAttributeSetBase, ArmorPenetration, OldArmorPenetration);
}

void UGDAttributeSetBase::OnRep_BlockChange(const FGameplayAttributeData& OldBlockChange)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGDAttributeSetBase, BlockChange, OldBlockChange);
}

void UGDAttributeSetBase::OnRep_CriticalHitChange(const FGameplayAttributeData& OldCriticalHitChange)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGDAttributeSetBase, CriticalHitChange, OldCriticalHitChange);
}

void UGDAttributeSetBase::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGDAttributeSetBase, CriticalHitDamage, OldCriticalHitDamage);
}

void UGDAttributeSetBase::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGDAttributeSetBase, CriticalHitResistance, OldCriticalHitResistance);
}

void UGDAttributeSetBase::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGDAttributeSetBase, HealthRegeneration, OldHealthRegeneration);
}

void UGDAttributeSetBase::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGDAttributeSetBase, ManaRegeneration, OldManaRegeneration);
}

void UGDAttributeSetBase::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGDAttributeSetBase, Health, OldHealth);
}

void UGDAttributeSetBase::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGDAttributeSetBase, MaxHealth, OldMaxHealth);
}

void UGDAttributeSetBase::OnRep_Mana(const FGameplayAttributeData& OldMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGDAttributeSetBase, Mana, OldMana);
}

void UGDAttributeSetBase::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGDAttributeSetBase, MaxMana, OldMaxMana);
}

void UGDAttributeSetBase::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGDAttributeSetBase, Stamina, OldStamina);
}

void UGDAttributeSetBase::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGDAttributeSetBase, MaxStamina, OldMaxStamina);
}
