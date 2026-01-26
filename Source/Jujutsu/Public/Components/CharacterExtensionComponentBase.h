// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Character.h"
#include "CharacterExtensionComponentBase.generated.h"

/**
 * 캐릭터 확장 컴포넌트 베이스
 * 소유 캐릭터/컨트롤러 접근 헬퍼 제공
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class JUJUTSU_API UCharacterExtensionComponentBase : public UActorComponent
{
	GENERATED_BODY()

protected:
	template<typename T>
	T* GetOwningCharacter() const
	{
		static_assert(TIsDerivedFrom<T, ACharacter>::IsDerived, "'T' must be derived from ACharacter");
		return Cast<T>(GetOwner());
	}

	ACharacter* GetOwningCharacter() const { return GetOwningCharacter<ACharacter>(); }

	template<typename T>
	T* GetOwningController() const
	{
		static_assert(TIsDerivedFrom<T, AController>::IsDerived, "'T' must be derived from AController");
		ACharacter* C = GetOwningCharacter<ACharacter>();
		return C ? Cast<T>(C->GetController()) : nullptr;
	}
};
