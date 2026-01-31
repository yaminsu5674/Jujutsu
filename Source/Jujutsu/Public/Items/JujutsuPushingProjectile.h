// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Items/JujutsuProjectileBase.h"
#include "JujutsuPushingProjectile.generated.h"

/**
 * 밀기(Push) 발사체.
 * BeginOverlap 시 오버랩된 액터의 PushComponent에 AttachToSource 모드로 RequestPush.
 * 발사체 자신을 Source로 사용하여 맞은 캐릭터가 발사체와 함께 이동.
 */
UCLASS()
class JUJUTSU_API AJujutsuPushingProjectile : public AJujutsuProjectileBase
{
	GENERATED_BODY()

public:
	AJujutsuPushingProjectile();

protected:
	virtual void OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
