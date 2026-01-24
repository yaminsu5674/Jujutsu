// Copyright Epic Games, Inc. All Rights Reserved.



#include "Controllers/JujutsuAIController.h"
#include "Characters/JujutsuBaseCharacter.h"

#include "JujutsuDebugHelper.h"

AJujutsuAIController::AJujutsuAIController()
{
}

void AJujutsuAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (AJujutsuBaseCharacter* AICharacter = Cast<AJujutsuBaseCharacter>(InPawn))
	{
		Debug::Print(TEXT("Possessed by AI Controller"));
	}
}
