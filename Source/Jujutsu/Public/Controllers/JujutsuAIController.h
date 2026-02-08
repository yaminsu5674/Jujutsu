// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameplayTagContainer.h"
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

	/** possessed pawn의 ASC에서 태그에 매칭되는 어빌리티 중 랜덤으로 하나의 게임플레이태그 반환 */
	UFUNCTION(BlueprintCallable, Category = "Jujutsu|AI")
	FGameplayTag SelectRandomSkill(FGameplayTag AbilityTagToActivate);

	/** 태그에 해당하는 어빌리티의 DesiredRange 반환. 없으면 0.f */
	UFUNCTION(BlueprintCallable, Category = "Jujutsu|AI")
	float GetDesiredRangeByTag(FGameplayTag AbilityTag);

	/** SelectRandomSkill 호출 시 설정되는 랜덤 선택된 스킬 태그 */
	UPROPERTY(BlueprintReadOnly, Category = "Jujutsu|AI")
	FGameplayTag SelectedRandomSkillTag;

protected:
	virtual void OnPossess(APawn* InPawn) override;
};
