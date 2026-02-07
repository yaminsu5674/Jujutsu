// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Characters/JujutsuBaseCharacter.h"
#include "Net/UnrealNetwork.h"
#include "JujutsuPlayerState.generated.h"

/**
 * 플레이어가 고른 캐릭터/적 클래스 타입 및 준비 상태 저장
 * - Replicated: 서버에서 설정, 모든 클라이언트에 복제
 * - Seamless Travel 시 CopyProperties로 유지
 */
UCLASS()
class JUJUTSU_API AJujutsuPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Jujutsu|PlayerState")
	TSubclassOf<AJujutsuBaseCharacter> GetHeroCharacterClass() const { return HeroCharacterClass; }

	UFUNCTION(BlueprintCallable, Category = "Jujutsu|PlayerState")
	void SetHeroCharacterClass(TSubclassOf<AJujutsuBaseCharacter> InClass) { HeroCharacterClass = InClass; }

	UFUNCTION(BlueprintCallable, Category = "Jujutsu|PlayerState")
	TSubclassOf<AJujutsuBaseCharacter> GetEnemyCharacterClass() const { return EnemyCharacterClass; }

	UFUNCTION(BlueprintCallable, Category = "Jujutsu|PlayerState")
	void SetEnemyCharacterClass(TSubclassOf<AJujutsuBaseCharacter> InClass) { EnemyCharacterClass = InClass; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Jujutsu|PlayerState")
	bool GetIsReady() const { return bIsReady; }

	UFUNCTION(BlueprintCallable, Category = "Jujutsu|PlayerState")
	void SetIsReady(bool bReady) { bIsReady = bReady; }

	/** Seamless Travel 시 새 PlayerState로 Hero/EnemyCharacterClass, bIsReady 복사 */
	virtual void CopyProperties(APlayerState* NewPlayerState) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(Replicated)
	TSubclassOf<AJujutsuBaseCharacter> HeroCharacterClass;

	UPROPERTY(Replicated)
	TSubclassOf<AJujutsuBaseCharacter> EnemyCharacterClass;

	UPROPERTY(Replicated)
	bool bIsReady = false;
};
