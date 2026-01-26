// Copyright Epic Games, Inc. All Rights Reserved.

#include "Widgets/JujutsuWidgetBase.h"
#include "Characters/JujutsuBaseCharacter.h"
#include "Components/UI/CharacterUIComponent.h"

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
