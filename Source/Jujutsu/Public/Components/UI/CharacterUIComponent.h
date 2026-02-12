// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/CharacterExtensionComponentBase.h"
#include "CharacterUIComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPercentChangedDelegate, float, NewPercent);

/**
 * 캐릭터 UI 담당 컴포넌트 (체력바 등 위젯 바인딩용)
 */
UCLASS()
class JUJUTSU_API UCharacterUIComponent : public UCharacterExtensionComponentBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnPercentChangedDelegate OnCurrentHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnPercentChangedDelegate OnCurrentRageChanged;

	/** 현재 체력/분노 퍼센트를 한 번 브로드캐스트. 속성 복제만 되고 PostGE가 안 타는 클라이언트에서 위젯 초기값용. */
	void BroadcastCurrentHealthAndRage();
};
