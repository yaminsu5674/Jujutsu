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
		Request.Duration = TNumericLimits<float>::Max(); // 발사체 수명 동안 유지 (EndProjectile 시 StopPush)
		PushComp->RequestPush(Request);
	}
}

void AJujutsuPushingProjectile::OnProjectileEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		UJujutsuPushComponent* PushComp = nullptr;
		if (AJujutsuBaseCharacter* HitChar = Cast<AJujutsuBaseCharacter>(OtherActor))
		{
			PushComp = HitChar->GetPushComponent();
		}
		else
		{
			PushComp = OtherActor->FindComponentByClass<UJujutsuPushComponent>();
		}
		if (PushComp)
		{
			PushComp->StopPush();
		}
	}
	Super::OnProjectileEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AJujutsuPushingProjectile::EndProjectile_Implementation()
{
	Super::EndProjectile_Implementation();

	for (AActor* OverlappedActor : OverlappedActors)
	{
		if (!OverlappedActor) continue;

		UJujutsuPushComponent* PushComp = nullptr;
		if (AJujutsuBaseCharacter* HitChar = Cast<AJujutsuBaseCharacter>(OverlappedActor))
		{
			PushComp = HitChar->GetPushComponent();
		}
		else
		{
			PushComp = OverlappedActor->FindComponentByClass<UJujutsuPushComponent>();
		}
		if (PushComp)
		{
			PushComp->StopPush();
		}
	}
}
