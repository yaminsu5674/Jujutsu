// Copyright Epic Games, Inc. All Rights Reserved.

#include "Items/JujutsuPushingProjectile.h"
#include "Components/Push/JujutsuPushComponent.h"
#include "Characters/JujutsuBaseCharacter.h"

AJujutsuPushingProjectile::AJujutsuPushingProjectile()
{
}

void AJujutsuPushingProjectile::OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnProjectileBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (!OtherActor) return;

	UJujutsuPushComponent* PushComp = nullptr;
	if (AJujutsuBaseCharacter* HitChar = Cast<AJujutsuBaseCharacter>(OtherActor))
	{
		PushComp = HitChar->GetPushComponent();
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("HitChar: %s"), *HitChar->GetName()));
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("OtherActor: %s"), *OtherActor->GetName()));
		}
		PushComp = OtherActor->FindComponentByClass<UJujutsuPushComponent>();
	}

	if (PushComp)
	{
		FPushRequest Request;
		Request.Mode = EPushMode::AttachToSource;
		Request.Source = this;
		Request.Duration = TNumericLimits<float>::Max(); // 발사체 수명 동안 유지 (Source 파괴 시 EndPush)
		PushComp->RequestPush(Request);
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("PushComp: %s"), *PushComp->GetName()));
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("PushComp is null")));
		}
	}
}
