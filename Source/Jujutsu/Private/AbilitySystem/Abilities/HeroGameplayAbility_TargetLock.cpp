// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystem/Abilities/HeroGameplayAbility_TargetLock.h"
#include "AbilitySystem/JujutsuAbilitySystemComponent.h"
#include "Characters/JujutsuBaseCharacter.h"
#include "Components/Combat/JujutsuCharacterCombatComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"

UHeroGameplayAbility_TargetLock::UHeroGameplayAbility_TargetLock()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	AbilityActivationPolicy = EJujutsuAbilityActivationPolicy::OnGiven;
}

void UHeroGameplayAbility_TargetLock::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UWorld* World = GetWorld();
	if (!World) return;

	// 매 프레임에 가깝게 틱 (타겟 방향 회전 갱신)
	const float TickRate = 0.016f;
	World->GetTimerManager().SetTimer(
		TargetLockTickTimerHandle,
		this,
		&UHeroGameplayAbility_TargetLock::OnTargetLockTick,
		TickRate,
		true
	);
}

void UHeroGameplayAbility_TargetLock::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(TargetLockTickTimerHandle);
	}
	TargetLockTickTimerHandle.Invalidate();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UHeroGameplayAbility_TargetLock::OnTargetLockTick()
{
	AJujutsuBaseCharacter* Hero = GetCharacterFromActorInfo();
	if (!Hero) return;

	UJujutsuCharacterCombatComponent* CombatComp = GetCharacterCombatComponentFromActorInfo();
	if (!CombatComp) return;

	AJujutsuBaseCharacter* TargetChar = CombatComp->Target.Get();
	if (!TargetChar) return;

	AController* Controller = GetCharacterControllerFromActorInfo();
	APlayerController* PC = Cast<APlayerController>(Controller);
	if (!PC) return;

	const FVector HeroLoc = Hero->GetActorLocation();
	const FVector TargetLoc = TargetChar->GetActorLocation();
	FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(HeroLoc, TargetLoc);
	LookAtRot.Pitch -= TargetLockCameraPitchOffset;
	LookAtRot.Yaw += TargetLockCameraYawOffset;

	const FRotator CurrentControlRot = PC->GetControlRotation();
	const FRotator TargetRot = FMath::RInterpTo(CurrentControlRot, LookAtRot, 0.016f, TargetLockRotationInterpSpeed);

	PC->SetControlRotation(FRotator(TargetRot.Pitch, TargetRot.Yaw, 0.f));
	//Hero->SetActorRotation(FRotator(0.f, TargetRot.Yaw, 0.f));
}
