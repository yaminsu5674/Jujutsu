// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JujutsuPawnExtensionComponentBase.generated.h"

/**
 * 폰 확장 컴포넌트 베이스
 * 소유 폰/컨트롤러 접근 헬퍼 제공
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class JUJUTSU_API UJujutsuPawnExtensionComponentBase : public UActorComponent
{
	GENERATED_BODY()

protected:
	template<typename T>
	T* GetOwningPawn() const
	{
		static_assert(TIsDerivedFrom<T, APawn>::IsDerived, "'T' must be derived from APawn");
		return Cast<T>(GetOwner());
	}

	APawn* GetOwningPawn() const { return GetOwningPawn<APawn>(); }

	template<typename T>
	T* GetOwningController() const
	{
		static_assert(TIsDerivedFrom<T, AController>::IsDerived, "'T' must be derived from AController");
		APawn* P = GetOwningPawn<APawn>();
		return P ? Cast<T>(P->GetController()) : nullptr;
	}
};
