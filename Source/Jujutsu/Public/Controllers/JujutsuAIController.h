// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "JujutsuAIController.generated.h"

class AJujutsuBaseCharacter;

/**
 *
 */
UCLASS()
class JUJUTSU_API AJujutsuAIController : public AAIController
{
	GENERATED_BODY()

public:
	AJujutsuAIController();

	/** 블랙보드 TargetActor에 플레이어 설정. 블루프린트에서 BT 실행 후 호출 시 유효. (BB에 TargetActor Object 키 필요) */
	UFUNCTION(BlueprintCallable, Category = "Jujutsu|AI")
	void InitBlackboard();

protected:
	virtual void OnPossess(APawn* InPawn) override;
};
