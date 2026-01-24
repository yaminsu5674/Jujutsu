// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "JujutsuGameplayAbility.generated.h"

UENUM(BlueprintType)
enum class EJujutsuAbilityActivationPolicy : uint8
{
	OnTriggered,
	OnGiven
};

/**
 *
 */
UCLASS()
class JUJUTSU_API UJujutsuGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	//~ Begin UGameplayAbility Interface.
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface

	UPROPERTY(EditDefaultsOnly, Category = "JujutsuAbility")
	EJujutsuAbilityActivationPolicy AbilityActivationPolicy = EJujutsuAbilityActivationPolicy::OnTriggered;

};
