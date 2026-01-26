// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/JujutsuPawnExtensionComponentBase.h"
#include "JujutsuCharacterCombatComponent.generated.h"

class AActor;
class AJujutsuBaseCharacter;

/**
 * 캐릭터 전투 컴포넌트
 * 전투/히트 처리 등 확장용 (폰컴뱃 베이스 없이 단일 클래스)
 */
UCLASS()
class JUJUTSU_API UJujutsuCharacterCombatComponent : public UJujutsuPawnExtensionComponentBase
{
	GENERATED_BODY()

public:
	/** 양손·양발 콜리전 박스 4개를 한 번에 켜거나 끔 */
	UFUNCTION(BlueprintCallable, Category = "Jujutsu|Combat")
	void ToggleBodyCollision(bool bShouldEnable);

	/** 바디 콜리전 BeginOverlap 시 호출 (히트 시) */
	virtual void OnHitTargetActor(AActor* HitActor);
	/** 바디 콜리전 EndOverlap 시 호출 (타겟에서 벗어남) */
	virtual void OnPulledFromTargetActor(AActor* InteractedActor);

protected:
	virtual void BeginPlay() override;

	/** 현재 오버랩 중인 액터 (Hit 이벤트 중복 발동 방지·콜리전 끌 때 비움) */
	TArray<AActor*> OverlappedActors;
};
