// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "JujutsuHeroController.generated.h"

class UDataAsset_InputConfig;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class JUJUTSU_API AJujutsuHeroController : public APlayerController
{
	GENERATED_BODY()

public:
	AJujutsuHeroController();

	void BindInputActions(class UInputComponent* PlayerInputComponent);

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:

#pragma region Inputs

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ControllerData", meta = (AllowPrivateAccess = "true"))
	UDataAsset_InputConfig* InputConfigDataAsset;

	void Input_Move(const FInputActionValue& InputActionValue);

#pragma endregion

};
