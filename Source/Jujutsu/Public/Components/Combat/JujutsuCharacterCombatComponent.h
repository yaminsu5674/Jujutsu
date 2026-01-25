// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/JujutsuPawnExtensionComponentBase.h"
#include "JujutsuCharacterCombatComponent.generated.h"

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
};
