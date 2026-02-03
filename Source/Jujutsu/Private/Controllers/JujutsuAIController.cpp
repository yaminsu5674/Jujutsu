// Copyright Epic Games, Inc. All Rights Reserved.

#include "Controllers/JujutsuAIController.h"
#include "Characters/JujutsuBaseCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/PlayerController.h"

AJujutsuAIController::AJujutsuAIController()
{
}

void AJujutsuAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void AJujutsuAIController::InitBlackboard()
{
	APlayerController* PC = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr;
	if (!PC || !PC->GetPawn()) return;

	AJujutsuBaseCharacter* PlayerCharacter = Cast<AJujutsuBaseCharacter>(PC->GetPawn());
	if (!PlayerCharacter) return;

	if (UBlackboardComponent* BlackboardComponent = GetBlackboardComponent())
	{
		BlackboardComponent->SetValueAsObject(FName("TargetActor"), PlayerCharacter);
	}
}
