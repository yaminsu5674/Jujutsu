// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystem/JujutsuAbilitySystemComponent.h"
#include "JujutsuAttributeSet.generated.h"

class UCharacterUIComponent;

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 *
 */
UCLASS()
class JUJUTSU_API UJujutsuAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UJujutsuAttributeSet();

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_CurrentHealth)
	FGameplayAttributeData CurrentHealth;
	ATTRIBUTE_ACCESSORS(UJujutsuAttributeSet, CurrentHealth)
	UFUNCTION()
	virtual void OnRep_CurrentHealth(const FGameplayAttributeData& OldCurrentHealth);

	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UJujutsuAttributeSet, MaxHealth)
	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UPROPERTY(BlueprintReadOnly, Category = "Rage", ReplicatedUsing = OnRep_CurrentRage)
	FGameplayAttributeData CurrentRage;
	ATTRIBUTE_ACCESSORS(UJujutsuAttributeSet, CurrentRage)
	UFUNCTION()
	virtual void OnRep_CurrentRage(const FGameplayAttributeData& OldCurrentRage);

	UPROPERTY(BlueprintReadOnly, Category = "Rage", ReplicatedUsing = OnRep_MaxRage)
	FGameplayAttributeData MaxRage;
	ATTRIBUTE_ACCESSORS(UJujutsuAttributeSet, MaxRage)
	UFUNCTION()
	virtual void OnRep_MaxRage(const FGameplayAttributeData& OldMaxRage);

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UJujutsuAttributeSet, AttackPower)

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData DefensePower;
	ATTRIBUTE_ACCESSORS(UJujutsuAttributeSet, DefensePower)

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData DamageTaken;
	ATTRIBUTE_ACCESSORS(UJujutsuAttributeSet, DamageTaken)

private:
	TWeakObjectPtr<UCharacterUIComponent> CachedCharacterUIComponent;
};
