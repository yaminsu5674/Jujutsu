// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/JujutsuPawnExtensionComponentBase.h"
#include "JujutsuPushComponent.generated.h"

/**
 * 캐릭터 밀기(Push) 컴포넌트
 * 밀기/넉백 등 확장용
 */
UCLASS()
class JUJUTSU_API UJujutsuPushComponent : public UJujutsuPawnExtensionComponentBase
{
	GENERATED_BODY()

public:
	UJujutsuPushComponent();

protected:
	virtual void BeginPlay() override;
};
