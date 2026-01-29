// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "JujutsuSkillLibrary.generated.h"

class AJujutsuBaseCharacter;

/**
 * 스킬/전투 관련 블루프린트·네이티브 유틸
 */
UCLASS()
class JUJUTSU_API UJujutsuSkillLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** 베이스캐릭터의 CombatComponent에서 타겟을 찾고, 있으면 그 방향으로 캐릭터(및 컨트롤러) 회전 */
	UFUNCTION(BlueprintCallable, Category = "Jujutsu|SkillLibrary")
	static void SetActorRotationToTarget(AJujutsuBaseCharacter* InCharacter);
};
