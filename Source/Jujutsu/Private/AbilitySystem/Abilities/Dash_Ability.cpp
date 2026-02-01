// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystem/Abilities/Dash_Ability.h"
#include "AbilitySystem/JujutsuAbilitySystemComponent.h"
#include "Characters/JujutsuBaseCharacter.h"
#include "Components/JujutsuCharacterMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "JujutsuSkillLibrary.h"
#include "TimerManager.h"

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
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(DashGravityRestoreHandle);
		World->GetTimerManager().ClearTimer(DashFrictionRestoreHandle);
	}

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

	SavedGravityScale = MoveComp->GravityScale;
	MoveComp->GravityScale = 0.f;
	FVector DashDir = Character->GetActorForwardVector();
	DashDir.Z = 0.f;
	DashDir = DashDir.GetSafeNormal();
	Character->LaunchCharacter(DashDir * RunSpeed * AirDashSpeedMultiplier, true, true);

	bIsAirDashing = true;
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(DashGravityRestoreHandle, [this]()
		{
			RestoreMovementAfterAirDash();
		}, AirDashGravityRestoreDelay, false);
	}
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

	SavedBrakingFrictionFactor = MoveComp->BrakingFrictionFactor;
	SavedGroundFriction = MoveComp->GroundFriction;
	SavedBrakingDecelerationWalking = MoveComp->BrakingDecelerationWalking;
	MoveComp->BrakingFrictionFactor = 0.f;
	MoveComp->GroundFriction = 0.f;
	MoveComp->BrakingDecelerationWalking = 0.f;

	FVector DashDir = Character->GetActorForwardVector();
	DashDir.Z = 0.f;
	if (!DashDir.IsNearlyZero())
	{
		DashDir = DashDir.GetSafeNormal();
		FVector Vel = MoveComp->Velocity;
		Vel.X = Vel.Y = 0.f;
		MoveComp->Velocity = Vel;
		MoveComp->AddImpulse(DashDir * RunSpeed * GroundDashSpeedMultiplier, true);
	}

	bIsGroundDashing = true;
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(DashFrictionRestoreHandle, [this]()
		{
			RestoreMovementAfterGroundDash();
		}, GroundDashFrictionRestoreDelay, false);
	}
}

void UDash_Ability::RestoreMovementAfterAirDash()
{
	if (!bIsAirDashing) return;
	bIsAirDashing = false;
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(DashGravityRestoreHandle);
	}
	if (AJujutsuBaseCharacter* Character = GetCharacterFromActorInfo())
	{
		if (UCharacterMovementComponent* MoveComp = Character->GetCharacterMovement())
		{
			MoveComp->GravityScale = SavedGravityScale;
		}
	}
	bDashFinished = true;
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
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(DashFrictionRestoreHandle);
	}
	if (AJujutsuBaseCharacter* Character = GetCharacterFromActorInfo())
	{
		if (UCharacterMovementComponent* MoveComp = Character->GetCharacterMovement())
		{
			MoveComp->BrakingFrictionFactor = SavedBrakingFrictionFactor;
			MoveComp->GroundFriction = SavedGroundFriction;
			MoveComp->BrakingDecelerationWalking = SavedBrakingDecelerationWalking;
		}
	}
	bDashFinished = true;
	if (bEndAbilityRequested)
	{
		bEndAbilityRequested = false;
		EndAbility(CachedAbilityHandle, CachedActorInfo, CachedActivationInfo, true, false);
	}
}

void UDash_Ability::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// 대시가 끝나기 전(키 뗐을 때) 호출되면 무시. Restore 끝에서 bEndAbilityRequested 보고 그때 EndAbility 수행
	if (!bDashFinished)
	{
		bEndAbilityRequested = true;
		return;
	}

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(DashGravityRestoreHandle);
		World->GetTimerManager().ClearTimer(DashFrictionRestoreHandle);
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
