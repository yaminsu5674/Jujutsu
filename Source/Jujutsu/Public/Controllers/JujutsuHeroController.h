// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbility.h"
#include "JujutsuHeroController.generated.h"

class UDataAsset_InputConfig;
class AJujutsuBaseCharacter;
class AJujutsuPlayerState;
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

	/** 자신의 PlayerState에 저장된 HeroCharacterClass 가져오기 (PlayerState 없으면 null) */
	UFUNCTION(BlueprintCallable, Category = "Jujutsu|Controller")
	TSubclassOf<AJujutsuBaseCharacter> GetHeroCharacterClass() const;

	/** 자신의 PlayerState에 HeroCharacterClass 저장 */
	UFUNCTION(BlueprintCallable, Category = "Jujutsu|Controller")
	void SetHeroCharacterClass(TSubclassOf<AJujutsuBaseCharacter> InClass);

	/** 자신의 PlayerState에 저장된 EnemyCharacterClass 가져오기 (PlayerState 없으면 null) */
	UFUNCTION(BlueprintCallable, Category = "Jujutsu|Controller")
	TSubclassOf<AJujutsuBaseCharacter> GetEnemyCharacterClass() const;

	/** 자신의 PlayerState에 EnemyCharacterClass 저장 */
	UFUNCTION(BlueprintCallable, Category = "Jujutsu|Controller")
	void SetEnemyCharacterClass(TSubclassOf<AJujutsuBaseCharacter> InClass);

	/** 서버를 지정 레벨로 이동 (클라이언트 호출 시 서버 RPC로 전달됨). 예: /Game/Maps/MyMap */
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Jujutsu|Controller")
	void TravelServer(const FString& LevelPath);

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
