// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbility.h"
#include "Engine/World.h"
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

	/** 서버를 지정 레벨로 이동. 블루프린트에서 레벨 에셋 선택 시 클라이언트에서 경로 변환 후 서버로 전달 */
	UFUNCTION(BlueprintCallable, Category = "Jujutsu|Controller", meta = (AllowedClasses = "/Script/Engine.World"))
	void TravelServerByLevel(const TSoftObjectPtr<UWorld>& Level);

	/** 서버 RPC. 경로 문자열로 ServerTravel 호출 (내부용, TravelServerByLevel에서 호출) */
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Jujutsu|Controller")
	void TravelServer(const FString& LevelPath);

	/** 조인 시 클라이언트의 GameInstance 선택을 서버 PlayerState로 전달. 클라이언트 전용 RPC. */
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Jujutsu|Controller")
	void ServerSetPlayerSelection(TSubclassOf<AJujutsuBaseCharacter> HeroClass, TSubclassOf<AJujutsuBaseCharacter> EnemyClass);

	/** 위젯에서 호출. 서버에 캐릭터 선택 전달 */
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Jujutsu|Controller")
	void Server_SetSelectedCharacter(TSubclassOf<AJujutsuBaseCharacter> InClass);

	/** 위젯에서 호출. 서버에 준비 완료 전달 */
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Jujutsu|Controller")
	void Server_SetReady(bool bReady);

	/** GameMode에서 호출. 클라이언트 화면에 카운트다운 표시 (DebugHelper) */
	UFUNCTION(Client, Reliable, Category = "Jujutsu|Controller")
	void Client_ShowCountdown(int32 SecondsRemaining);

	/** OnRep_PlayerState 등에서 호출. 로컬에서만 컨트롤러 부여 어빌리티(위젯 등) 수동 활성화. 어빌리티 스펙이 아직 없으면 false. */
	bool TryActivateControllerGrantedAbilities();

protected:
	virtual void BeginPlay() override;
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
