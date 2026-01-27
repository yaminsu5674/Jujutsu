// Copyright Epic Games, Inc. All Rights Reserved.

#include "Widgets/JujutsuWidgetBase.h"
#include "Characters/JujutsuBaseCharacter.h"
#include "Components/UI/CharacterUIComponent.h"
#include "GameFramework/Actor.h"

void UJujutsuWidgetBase::InitCharacterCreatedWidget_Implementation(AActor* OwningCharacter)
{
	// WarriorRPG InitEnemyCreatedWidget 패턴: Actor에서 UI 컴포넌트를 꺼내서 동일한 BP 이벤트로 넘김
	// → 체력바 위젯은 "Event On Owning Character UI Component Initialized" 하나로 바인딩 가능
	if (AJujutsuBaseCharacter* BaseCharacter = Cast<AJujutsuBaseCharacter>(OwningCharacter))
	{
		if (UCharacterUIComponent* UIComp = BaseCharacter->GetCharacterUIComponent())
		{
			BP_OnOwningCharacterUIComponentInitialized(UIComp);
		}
	}
}

void UJujutsuWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	APawn* OwningPawn = GetOwningPlayer() ? GetOwningPlayer()->GetPawn() : nullptr;
	if (AJujutsuBaseCharacter* BaseCharacter = Cast<AJujutsuBaseCharacter>(OwningPawn))
	{
		if (UCharacterUIComponent* UIComp = BaseCharacter->GetCharacterUIComponent())
		{
			BP_OnOwningCharacterUIComponentInitialized(UIComp);
		}
	}
}
