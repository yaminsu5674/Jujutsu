// Copyright Epic Games, Inc. All Rights Reserved.



#include "Controllers/JujutsuHeroController.h"
#include "Characters/JujutsuBaseCharacter.h"
#include "Controllers/JujutsuPlayerState.h"
#include "JujutsuGameInstance.h"
#include "AbilitySystem/JujutsuAbilitySystemComponent.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "Components/Input/JujutsuInputComponent.h"
#include "JujutsuGameplayTags.h"

// Debug
#include "JujutsuDebugHelper.h"

AJujutsuHeroController::AJujutsuHeroController()
{
}

TSubclassOf<AJujutsuBaseCharacter> AJujutsuHeroController::GetHeroCharacterClass() const
{
	if (AJujutsuPlayerState* PS = GetPlayerState<AJujutsuPlayerState>())
	{
		return PS->GetHeroCharacterClass();
	}
	return nullptr;
}

void AJujutsuHeroController::SetHeroCharacterClass(TSubclassOf<AJujutsuBaseCharacter> InClass)
{
	if (AJujutsuPlayerState* PS = GetPlayerState<AJujutsuPlayerState>())
	{
		PS->SetHeroCharacterClass(InClass);
	}
	if (UJujutsuGameInstance* GI = GetGameInstance<UJujutsuGameInstance>())
	{
		GI->SetHeroCharacterClass(InClass);
	}
}

TSubclassOf<AJujutsuBaseCharacter> AJujutsuHeroController::GetEnemyCharacterClass() const
{
	if (AJujutsuPlayerState* PS = GetPlayerState<AJujutsuPlayerState>())
	{
		return PS->GetEnemyCharacterClass();
	}
	return nullptr;
}

void AJujutsuHeroController::SetEnemyCharacterClass(TSubclassOf<AJujutsuBaseCharacter> InClass)
{
	if (AJujutsuPlayerState* PS = GetPlayerState<AJujutsuPlayerState>())
	{
		PS->SetEnemyCharacterClass(InClass);
	}
	if (UJujutsuGameInstance* GI = GetGameInstance<UJujutsuGameInstance>())
	{
		GI->SetEnemyCharacterClass(InClass);
	}
}

void AJujutsuHeroController::TravelServerByLevel(const TSoftObjectPtr<UWorld>& Level)
{
	const FString LevelPath = Level.ToSoftObjectPath().GetLongPackageName();
	UE_LOG(LogTemp, Log, TEXT("[TravelServerByLevel] LevelPath=%s"), *LevelPath);
	TravelServer(LevelPath);
}

void AJujutsuHeroController::TravelServer_Implementation(const FString& LevelPath)
{
	UE_LOG(LogTemp, Log, TEXT("[TravelServer_Implementation] LevelPath=%s, HasAuthority=%d"), *LevelPath, HasAuthority());
	if (UWorld* World = GetWorld(); World && !LevelPath.IsEmpty())
	{
		World->SeamlessTravel(LevelPath, true);
		UE_LOG(LogTemp, Log, TEXT("[TravelServer_Implementation] SeamlessTravel called"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[TravelServer_Implementation] Skipped - World=%d, LevelPathEmpty=%d"), World != nullptr, LevelPath.IsEmpty());
	}
}

void AJujutsuHeroController::ServerSetPlayerSelection_Implementation(TSubclassOf<AJujutsuBaseCharacter> HeroClass, TSubclassOf<AJujutsuBaseCharacter> EnemyClass)
{
	if (AJujutsuPlayerState* PS = GetPlayerState<AJujutsuPlayerState>())
	{
		PS->SetHeroCharacterClass(HeroClass);
		PS->SetEnemyCharacterClass(EnemyClass);
		if (AGameModeBase* GM = GetWorld() ? GetWorld()->GetAuthGameMode() : nullptr)
		{
			GM->RestartPlayer(this);
		}
	}
}

void AJujutsuHeroController::BeginPlay()
{
	Super::BeginPlay();
	if (IsLocalController() && GetNetMode() == NM_Client)
	{
		if (UJujutsuGameInstance* GI = GetGameInstance<UJujutsuGameInstance>())
		{
			TSubclassOf<AJujutsuBaseCharacter> HeroClass = GI->GetHeroCharacterClass();
			TSubclassOf<AJujutsuBaseCharacter> EnemyClass = GI->GetEnemyCharacterClass();
			if (HeroClass || EnemyClass)
			{
				ServerSetPlayerSelection(HeroClass, EnemyClass);
			}
		}
	}
}

void AJujutsuHeroController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void AJujutsuHeroController::OnCharacterASCInitComplete_Implementation()
{
}

void AJujutsuHeroController::BindInputActions(UInputComponent* PlayerInputComponent)
{
	checkf(InputConfigDataAsset,TEXT("Forgot to assign a valid data asset as input config"));

	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	check(LocalPlayer);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	check(Subsystem);

	Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext,0);

	if (UJujutsuInputComponent* JujutsuInputComponent = Cast<UJujutsuInputComponent>(PlayerInputComponent))
	{
		JujutsuInputComponent->BindNativeInputAction(InputConfigDataAsset, JujutsuGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
		JujutsuInputComponent->BindAbilityInputAction(InputConfigDataAsset, this, &ThisClass::Input_AbilityInputPressed, &ThisClass::Input_AbilityInputReleased);
	}
}

void AJujutsuHeroController::Input_Move(const FInputActionValue& InputActionValue)
{
	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector2D MovementVector = InputActionValue.Get<FVector2D>();

		const FRotator MovementRotation(0.f,GetControlRotation().Yaw,0.f);

		if (MovementVector.Y != 0.f)
		{
			const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			ControlledPawn->AddMovementInput(ForwardDirection,MovementVector.Y);
		}

		if (MovementVector.X != 0.f)
		{
			const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);
			ControlledPawn->AddMovementInput(RightDirection, MovementVector.X);
		}
	}
}

void AJujutsuHeroController::Input_AbilityInputPressed(FGameplayTag InInputTag)
{
	if (AJujutsuBaseCharacter* BaseCharacter = Cast<AJujutsuBaseCharacter>(GetPawn()))
	{
		if (UJujutsuAbilitySystemComponent* ASC = BaseCharacter->GetJujutsuAbilitySystemComponent())
		{
			ASC->OnAbilityInputPressed(InInputTag);
		}
	}
}

void AJujutsuHeroController::Input_AbilityInputReleased(FGameplayTag InInputTag)
{
	if (AJujutsuBaseCharacter* BaseCharacter = Cast<AJujutsuBaseCharacter>(GetPawn()))
	{
		if (UJujutsuAbilitySystemComponent* ASC = BaseCharacter->GetJujutsuAbilitySystemComponent())
		{
			ASC->OnAbilityInputReleased(InInputTag);
		}
	}
}