// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JujutsuWidgetBase.generated.h"

class UCharacterUIComponent;

/**
 * 오너 캐릭터의 CharacterUIComponent 구독용 위젯 베이스
 * 블루프린트에서 BP_OnOwningCharacterUIComponentInitialized 구현 후 델리게이트 바인딩
 */
UCLASS()
class JUJUTSU_API UJujutsuWidgetBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Owning Character UI Component Initialized"))
	void BP_OnOwningCharacterUIComponentInitialized(UCharacterUIComponent* OwningCharacterUIComponent);
};
