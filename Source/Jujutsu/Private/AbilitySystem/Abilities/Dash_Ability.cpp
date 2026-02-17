// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystem/Abilities/Dash_Ability.h"
#include "AbilitySystem/JujutsuAbilitySystemComponent.h"
#include "Characters/JujutsuBaseCharacter.h"
#include "Components/JujutsuCharacterMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "JujutsuGameplayTags.h"
#include "JujutsuSkillLibrary.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h"

void UDash_Ability::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CachedAbilityHandle = Handle;
	CachedActorInfo = ActorInfo;
	CachedActivationInfo = ActivationInfo;
	bDashFinished = false;
	bEndAbilityRequested = false;
	bIsAirDashing = false;
	bIsGroundDashing = false;

	AJujutsuBaseCharacter* Character = GetCharacterFromActorInfo();
	if (!Character) return;

	UCharacterMovementComponent* MoveComp = Character->GetCharacterMovement();
	// 이동 키를 누르고 있는지 = 가속도(입력 반영)가 0이 아닌지로 판단
	const bool bIsMoving = MoveComp && MoveComp->GetCurrentAcceleration().SizeSquared2D() > 0.f;
	if (!bIsMoving)
	{
		UJujutsuSkillLibrary::SetActorRotationToTarget(Character);
		if (MoveComp)
		{
			MoveComp->StopMovementImmediately();
		}
	}

	const bool bIsFalling = Character->GetCharacterMovement() && Character->GetCharacterMovement()->IsFalling();
	UAnimInstance* AnimInst = Character->GetMesh() ? Character->GetMesh()->GetAnimInstance() : nullptr;

	if (bIsFalling)
	{
		ApplyMovementForAirDash(Character);
	}
	else
	{
		ApplyMovementForGroundDash(Character);
		if (UJujutsuCharacterMovementComponent* JutsuMove = Cast<UJujutsuCharacterMovementComponent>(MoveComp))
		{
			MoveComp->MaxWalkSpeed = JutsuMove->RunSpeed;
		}
	}


	if (DashMontage && AnimInst)
	{
		Character->PlayAnimMontage(DashMontage);
	}
}

void UDash_Ability::ApplyMovementForAirDash(AJujutsuBaseCharacter* Character)
{
	UCharacterMovementComponent* MoveComp = Character ? Character->GetCharacterMovement() : nullptr;
	if (!MoveComp) return;

	float RunSpeed = MoveComp->MaxWalkSpeed;
	if (const UJujutsuCharacterMovementComponent* JutsuMove = Cast<UJujutsuCharacterMovementComponent>(MoveComp))
	{
		RunSpeed = JutsuMove->RunSpeed;
	}

	FVector DashDir = Character->GetActorForwardVector();
	DashDir.Z = 0.f;
	if (DashDir.IsNearlyZero()) return;
	DashDir = DashDir.GetSafeNormal();

	const float Strength = RunSpeed * AirDashSpeedMultiplier;
	const FVector FinishVelocity = DashDir * RunSpeed;

	UAbilityTask_ApplyRootMotionConstantForce* DashTask = UAbilityTask_ApplyRootMotionConstantForce::ApplyRootMotionConstantForce(
		this,
		FName(TEXT("AirDashTask")),
		DashDir,
		Strength,
		AirDashGravityRestoreDelay,
		false,
		nullptr,
		ERootMotionFinishVelocityMode::SetVelocity,
		FinishVelocity,
		0.1f,
		false   // bEnableGravity: 공중 대시 중에는 중력 무시
	);

	if (DashTask)
	{
		DashTask->OnFinish.AddDynamic(this, &UDash_Ability::RestoreMovementAfterAirDash);
		DashTask->ReadyForActivation();
	}

	bIsAirDashing = true;
}

void UDash_Ability::ApplyMovementForGroundDash(AJujutsuBaseCharacter* Character)
{
	UCharacterMovementComponent* MoveComp = Character ? Character->GetCharacterMovement() : nullptr;
	if (!MoveComp) return;

	float RunSpeed = MoveComp->MaxWalkSpeed;
	if (const UJujutsuCharacterMovementComponent* JutsuMove = Cast<UJujutsuCharacterMovementComponent>(MoveComp))
	{
		RunSpeed = JutsuMove->RunSpeed;
	}

	FVector DashDir = Character->GetActorForwardVector();
	DashDir.Z = 0.f;
	if (DashDir.IsNearlyZero()) return;
	DashDir = DashDir.GetSafeNormal();

	const float Strength = RunSpeed * GroundDashSpeedMultiplier;
	const FVector FinishVelocity = DashDir * RunSpeed;

	UAbilityTask_ApplyRootMotionConstantForce* DashTask = UAbilityTask_ApplyRootMotionConstantForce::ApplyRootMotionConstantForce(
		this,
		FName(TEXT("GroundDashTask")),
		DashDir,
		Strength,
		GroundDashFrictionRestoreDelay,
		false,  // bIsAdditive
		nullptr,
		ERootMotionFinishVelocityMode::SetVelocity,
		FinishVelocity,
		0.1f,
		false   // bEnableGravity
	);

	if (DashTask)
	{
		DashTask->OnFinish.AddDynamic(this, &UDash_Ability::RestoreMovementAfterGroundDash);
		DashTask->ReadyForActivation();
	}

	bIsGroundDashing = true;
}

void UDash_Ability::RestoreMovementAfterAirDash()
{
	if (!bIsAirDashing) return;
	bIsAirDashing = false;

	bDashFinished = true;
	// 키를 이미 뗀 경우에만 여기서 EndAbility 호출. 아니면 어빌리티 유지(공중은 RunSpeed 미적용, 지상은 RunSpeed 유지)
	if (bEndAbilityRequested)
	{
		bEndAbilityRequested = false;
		EndAbility(CachedAbilityHandle, CachedActorInfo, CachedActivationInfo, true, false);
	}
}

void UDash_Ability::RestoreMovementAfterGroundDash()
{
	if (!bIsGroundDashing) return;
	bIsGroundDashing = false;

	bDashFinished = true;
	// 키를 이미 뗀 경우에만 여기서 EndAbility 호출. 아니면 어빌리티 유지 → MaxWalkSpeed = RunSpeed 유지
	if (bEndAbilityRequested)
	{
		bEndAbilityRequested = false;
		EndAbility(CachedAbilityHandle, CachedActorInfo, CachedActivationInfo, true, false);
	}
}

void UDash_Ability::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// Character_Status_Hit(피격) 상태일 때는 즉시 복구
	if (ActorInfo && ActorInfo->AbilitySystemComponent.IsValid() && ActorInfo->AbilitySystemComponent->HasMatchingGameplayTag(JujutsuGameplayTags::Character_Status_Hit))
	{
		if (bIsAirDashing)
		{
			RestoreMovementAfterAirDash();
		}
		if (bIsGroundDashing)
		{
			RestoreMovementAfterGroundDash();
		}
		bDashFinished = true;
	}

	// 대시가 끝나기 전(키 뗐을 때) 호출되면 무시. Restore 끝에서 bEndAbilityRequested 보고 그때 EndAbility 수행
	if (!bDashFinished)
	{
		bEndAbilityRequested = true;
		return;
	}

	if (bIsAirDashing)
	{
		RestoreMovementAfterAirDash();
	}
	if (bIsGroundDashing)
	{
		RestoreMovementAfterGroundDash();
	}

	if (AJujutsuBaseCharacter* Character = GetCharacterFromActorInfo())
	{
		if (UJujutsuCharacterMovementComponent* JutsuMove = Cast<UJujutsuCharacterMovementComponent>(Character->GetCharacterMovement()))
		{
			JutsuMove->MaxWalkSpeed = JutsuMove->WalkSpeed;
		}
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
