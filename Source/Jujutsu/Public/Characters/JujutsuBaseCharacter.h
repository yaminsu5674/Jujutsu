// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "JujutsuBaseCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UJujutsuAbilitySystemComponent;
class UJujutsuAttributeSet;

UCLASS()
class JUJUTSU_API AJujutsuBaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AJujutsuBaseCharacter();

	//~ Begin IAbilitySystemInterface Interface.
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;
	//~ End IAbilitySystemInterface Interface

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//~ Begin APawn Interface.
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	UJujutsuAbilitySystemComponent* JujutsuAbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	UJujutsuAttributeSet* JujutsuAttributeSet;

public:
	FORCEINLINE UJujutsuAbilitySystemComponent* GetJujutsuAbilitySystemComponent() const {return JujutsuAbilitySystemComponent;}

	FORCEINLINE UJujutsuAttributeSet* GetJujutsuAttributeSet() const {return JujutsuAttributeSet;}

private:

#pragma region Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

#pragma endregion

};
