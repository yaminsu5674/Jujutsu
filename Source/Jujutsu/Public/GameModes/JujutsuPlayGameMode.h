// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameModes/JujutsuBaseGameMode.h"
#include "JujutsuPlayGameMode.generated.h"

class AJujutsuBaseCharacter;

/** 베이스 모드 상속. 플레이(대전) 단계용. */
UCLASS()
class JUJUTSU_API AJujutsuPlayGameMode : public AJujutsuBaseGameMode
{
	GENERATED_BODY()

public:
	/** 캐릭터 BeginPlay 완료 시 호출. MaxCharacterNumber만큼 쌓이면 OnAllCharactersReady 호출 */
	void NotifyCharacterReady(AJujutsuBaseCharacter* InCharacter);

protected:
	/** NotifyCharacterReady가 이 수만큼 호출되면 OnAllCharactersReady 호출 (멀티플레이 등에서 오버라이드) */
	UPROPERTY(EditDefaultsOnly, Category = "Ready")
	int32 MaxCharacterNumber = 2;

	/** MaxCharacterNumber 도달 시 호출. 싱글/멀티에서 오버라이드하여 "All ready" 출력 등 처리 */
	virtual void OnAllCharactersReady();

	int32 ReadyCharacterCount = 0;
};
