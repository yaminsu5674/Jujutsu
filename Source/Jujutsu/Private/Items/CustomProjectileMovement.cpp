// Copyright Epic Games, Inc. All Rights Reserved.

#include "Items/CustomProjectileMovement.h"
#include "Items/JujutsuProjectileBase.h"
#include "GameFramework/Actor.h"

UCustomProjectileMovement::UCustomProjectileMovement()
	: MoveType(EProjectileMoveType::Single)
	, bNeverStop(false)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCustomProjectileMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	switch (MoveType)
	{
	case EProjectileMoveType::Single:
	case EProjectileMoveType::Rush:
		Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
		break;
	}
}

void UCustomProjectileMovement::HandleImpact(const FHitResult& Hit, float TimeSlice, const FVector& MoveDelta)
{
	switch (MoveType)
	{
	case EProjectileMoveType::Single:
		Super::HandleImpact(Hit, TimeSlice, MoveDelta);
		break;

	case EProjectileMoveType::Rush:
		// Rush: 충돌해도 멈추지 않음
		break;
	}
}

UProjectileMovementComponent::EHandleBlockingHitResult UCustomProjectileMovement::HandleBlockingHit(const FHitResult& Hit, float TimeTick, const FVector& MoveDelta, float& SubTickTimeRemaining)
{
	if (Hit.bBlockingHit)
	{
		if (AJujutsuProjectileBase* Projectile = Cast<AJujutsuProjectileBase>(GetOwner()))
		{
			Projectile->OnProjectileHit(
				Hit.GetComponent(),
				Hit.GetActor(),
				Hit.ImpactPoint,
				Hit.ImpactNormal,
				Hit
			);
		}
	}

	switch (MoveType)
	{
	case EProjectileMoveType::Single:
		return Super::HandleBlockingHit(Hit, TimeTick, MoveDelta, SubTickTimeRemaining);

	case EProjectileMoveType::Rush:
		// Rush: 속도 유지하며 진행
		if (GetOwner())
		{
			const float CurrentSpeed = Velocity.Size();
			Velocity = GetOwner()->GetActorForwardVector() * CurrentSpeed;

			if (Hit.Normal.Z > 0.7f)
			{
				Velocity.Z = 0.0f;
			}

			ProjectileGravityScale = 0.0f;
		}
		return EHandleBlockingHitResult::AdvanceNextSubstep;
	}

	return Super::HandleBlockingHit(Hit, TimeTick, MoveDelta, SubTickTimeRemaining);
}

void UCustomProjectileMovement::SetMoveType(EProjectileMoveType NewMoveType)
{
	MoveType = NewMoveType;

	switch (MoveType)
	{
	case EProjectileMoveType::Single:
		bNeverStop = false;
		bShouldBounce = false;
		bIsHomingProjectile = false;
		break;

	case EProjectileMoveType::Rush:
		bNeverStop = true;
		bShouldBounce = false;
		bIsHomingProjectile = false;
		break;
	}
}

void UCustomProjectileMovement::SetDirection(AActor* TargetActor, float ProjectileSpeed)
{
	if (!GetOwner()) return;

	FVector Direction;

	if (TargetActor)
	{
		FVector TargetLocation = TargetActor->GetActorLocation();
		TargetLocation.Z += 30.0f;
		Direction = (TargetLocation - GetOwner()->GetActorLocation()).GetSafeNormal();
		GetOwner()->SetActorRotation(Direction.Rotation());
	}
	else
	{
		Direction = GetOwner()->GetActorForwardVector();
	}

	Velocity = Direction * ProjectileSpeed;
	InitialSpeed = ProjectileSpeed;
	MaxSpeed = ProjectileSpeed;
}

void UCustomProjectileMovement::ApplyBehaviorSettings(bool bShouldMove, bool bEnableLifespan, float LifespanDuration)
{
	if (bShouldMove)
	{
		// Velocity는 SetDirection에서 설정됨
	}
	else
	{
		Velocity = FVector::ZeroVector;
	}
}
