// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "JujutsuAIController.generated.h"

/**
 *
 */
UCLASS()
class JUJUTSU_API AJujutsuAIController : public AAIController
{
	GENERATED_BODY()

public:
	AJujutsuAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;

};
