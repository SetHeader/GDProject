// Copyright 2020 Dan Kestranek.


#include "AbilitySystem//AttributeSets/GDAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "GameplayEffect.h"
#include "GDGameplayTags.h"
#include "AbilitySystem/GDAbilitySystemLibrary.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "Player/GDPlayerController.h"


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
		const float LocalIncomingDamage = GetIncomingDamage();
		SetIncomingDamage(0.f);
		if (LocalIncomingDamage > 0.f)
		{
			const float NewHealth = GetHealth() - LocalIncomingDamage;
			SetHealth(FMath::Clamp(NewHealth, 0, GetMaxHealth()));
			const bool bFatal = NewHealth <= 0.f;
			// 触发角色死亡
			if (bFatal)
			{
				if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetAvatarActor))
				{
					CombatInterface->Die();
				} 
			} else
			{
				// 触发受击反应
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FGDGameplayTags::Get().Effects_HitReact);
				Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);
			}

			// 显示浮动伤害
			if (Props.SourceCharacter != Props.TargetCharacter)
			{
				AGDPlayerController* PC = Cast<AGDPlayerController>(Props.SourceController);
				if (PC)
				{
					const bool bBlockedHit = UGDAbilitySystemLibrary::IsBlockedHit(Props.EffectContextHandle);
					const bool bCriticalHit = UGDAbilitySystemLibrary::IsCriticalHit(Props.EffectContextHandle);
					PC->Client_ShowDamageNumber(LocalIncomingDamage, Props.TargetCharacter, bBlockedHit, bCriticalHit);
					return;
				}
				PC = Cast<AGDPlayerController>(Props.TargetController);
				if (PC)
				{
					const bool bBlockedHit = UGDAbilitySystemLibrary::IsBlockedHit(Props.EffectContextHandle);
					const bool bCriticalHit = UGDAbilitySystemLibrary::IsCriticalHit(Props.EffectContextHandle);
					PC->Client_ShowDamageNumber(LocalIncomingDamage, Props.TargetCharacter, bBlockedHit, bCriticalHit);
				}
			}
		}
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
