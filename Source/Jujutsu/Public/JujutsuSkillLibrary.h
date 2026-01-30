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

	/**
	 * 액터를 기준으로 발사체 스폰 월드 위치를 반환.
	 * @param Actor 기준 액터 (바라보는 방향·위치 사용)
	 * @param Offset 액터 로컬 오프셋 (X=앞, Y=좌우, Z=위아래). 단위는 유닛(예: 앞 100, 오른쪽 50, 위 30)
	 * @return 스폰할 월드 위치. Actor가 null이면 (0,0,0) 반환
	 */
	UFUNCTION(BlueprintCallable, Category = "Jujutsu|SkillLibrary", meta = (DisplayName = "Get Spawn Location From Character"))
	static FVector GetSpawnLocationFromCharacter(AActor* Actor, FVector Offset);
};
