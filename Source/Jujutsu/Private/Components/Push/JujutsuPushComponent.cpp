// Copyright Epic Games, Inc. All Rights Reserved.

#include "Components/Push/JujutsuPushComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Debug
#include "JujutsuDebugHelper.h"

UJujutsuPushComponent::UJujutsuPushComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UJujutsuPushComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (ACharacter* Char = OwnerCharacter.Get())
	{
		MoveComp = Char->GetCharacterMovement();
	}
}

void UJujutsuPushComponent::RequestPush(const FPushRequest& Request)
{
	if (!OwnerCharacter.IsValid() || !MoveComp.IsValid()) return;

	ActivePush = Request;
	ActivePush.Direction = ActivePush.Direction.GetSafeNormal();

	ElapsedTime = 0.f;
	bActive = true;
	SetComponentTickEnabled(true);

	// 밀림 중에는 중력 비활성화
	SavedGravityScale = MoveComp->GravityScale;
	MoveComp->GravityScale = 0.f;

	if (ActivePush.Mode == EPushMode::AttachToSource && ActivePush.Source.IsValid())
	{
		PrevSourceLocation = ActivePush.Source->GetActorLocation();
	}
}

void UJujutsuPushComponent::StopPush()
{
	// Debug::Print(FString::Printf(TEXT("StopPush: %s"), GetOwner() ? *GetOwner()->GetName() : TEXT("null")), FColor::Orange);
	EndPush();
}

void UJujutsuPushComponent::EndPush()
{
	bActive = false;
	ElapsedTime = 0.f;
	ActivePush = FPushRequest();
	SetComponentTickEnabled(false);

	// 중력 복원
	if (MoveComp.IsValid())
	{
		MoveComp->GravityScale = SavedGravityScale;
	}
}

void UJujutsuPushComponent::TickComponent(
	float DeltaTime,
	ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bActive) return;

	ElapsedTime += DeltaTime;
	if (ElapsedTime >= ActivePush.Duration)
	{
		EndPush();
		return;
	}

	if (ActivePush.Mode == EPushMode::AttachToSource)
	{
		TickAttachPush(DeltaTime);
	}
	else
	{
		TickImpulsePush(DeltaTime);
	}
}

void UJujutsuPushComponent::TickAttachPush(float DeltaTime)
{
	if (!ActivePush.Source.IsValid())
	{
		EndPush();
		return;
	}

	const FVector CurrentLoc = ActivePush.Source->GetActorLocation();
	const FVector Delta = (CurrentLoc - PrevSourceLocation) * ActivePush.FollowScale;

	ACharacter* Char = OwnerCharacter.Get();
	if (!Char) { EndPush(); return; }

	FHitResult Hit;
	Char->AddActorWorldOffset(Delta, true, &Hit);

	PrevSourceLocation = CurrentLoc;

	if (Hit.IsValidBlockingHit())
	{
		EndPush();
	}
}

void UJujutsuPushComponent::TickImpulsePush(float DeltaTime)
{
	const FVector Delta =
		ActivePush.Direction * ActivePush.Strength * DeltaTime;

	ACharacter* Char = OwnerCharacter.Get();
	if (!Char) { EndPush(); return; }

	FHitResult Hit;
	Char->AddActorWorldOffset(Delta, true, &Hit);

	if (Hit.IsValidBlockingHit())
	{
		EndPush();
	}
}
