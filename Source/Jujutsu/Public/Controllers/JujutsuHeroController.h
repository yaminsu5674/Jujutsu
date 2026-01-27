// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbility.h"
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

	/** 캐릭터 ASC 초기화 완료 시 호출됨. 블루프린트에서 오버라이드하여 초기화 후 로직 추가 */
	UFUNCTION(BlueprintNativeEvent, Category = "Jujutsu|Controller")
	void OnCharacterASCInitComplete();

	/** PossessedBy 시 스타트업데이터의 ActivateOnGivenAbilities에 append될 어빌리티 목록 (베이스캐릭터에서 사용) */
	const TArray<TSubclassOf<UGameplayAbility>>& GetAbilitiesToGrantWithCharacterInit() const { return AbilitiesToGrantWithCharacterInit; }

protected:
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Jujutsu|Controller", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UGameplayAbility>> AbilitiesToGrantWithCharacterInit;

private:

#pragma region Inputs

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ControllerData", meta = (AllowPrivateAccess = "true"))
	UDataAsset_InputConfig* InputConfigDataAsset;

	void Input_Move(const FInputActionValue& InputActionValue);

	void Input_AbilityInputPressed(FGameplayTag InInputTag);
	void Input_AbilityInputReleased(FGameplayTag InInputTag);

#pragma endregion

};
