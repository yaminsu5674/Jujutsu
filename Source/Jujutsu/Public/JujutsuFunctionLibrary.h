// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "JujutsuFunctionLibrary.generated.h"

class UJujutsuAbilitySystemComponent;
class UJujutsuCharacterCombatComponent;

UENUM()
enum class EJujutsuConfirmType : uint8
{
	Yes,
	No
};

UENUM()
enum class EJujutsuValidType : uint8
{
	Valid,
	Invalid
};

/**
 * Jujutsu 공용 블루프린트/네이티브 함수 라이브러리
 */
UCLASS()
class JUJUTSU_API UJujutsuFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static UJujutsuAbilitySystemComponent* NativeGetJujutsuASCFromActor(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "Jujutsu|FunctionLibrary")
	static void AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd);

	UFUNCTION(BlueprintCallable, Category = "Jujutsu|FunctionLibrary")
	static void RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove);

	static bool NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck);

	UFUNCTION(BlueprintCallable, Category = "Jujutsu|FunctionLibrary", meta = (DisplayName = "Does Actor Have Tag", ExpandEnumAsExecs = "OutConfirmType"))
	static void BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EJujutsuConfirmType& OutConfirmType);

	static UJujutsuCharacterCombatComponent* NativeGetCharacterCombatComponentFromActor(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "Jujutsu|FunctionLibrary", meta = (DisplayName = "Get Character Combat Component From Actor", ExpandEnumAsExecs = "OutValidType"))
	static UJujutsuCharacterCombatComponent* BP_GetCharacterCombatComponentFromActor(AActor* InActor, EJujutsuValidType& OutValidType);
};
