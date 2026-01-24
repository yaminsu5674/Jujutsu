// Copyright Epic Games, Inc. All Rights Reserved.



#include "Controllers/JujutsuHeroController.h"
#include "Characters/JujutsuBaseCharacter.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"

#include "JujutsuDebugHelper.h"

AJujutsuHeroController::AJujutsuHeroController()
{
}

void AJujutsuHeroController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (AJujutsuBaseCharacter* HeroCharacter = Cast<AJujutsuBaseCharacter>(InPawn))
	{
		//Debug::Print(TEXT("Possessed by Hero Controller"));
	}
}
