// Copyright Epic Games, Inc. All Rights Reserved.



#include "Characters/JujutsuBaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Controllers/JujutsuHeroController.h"

// Sets default values
AJujutsuBaseCharacter::AJujutsuBaseCharacter()
{
 // Set this character to call Tick() every frame. You can turn this off to improve performance if you don't need it.
 PrimaryActorTick.bCanEverTick = false;
 PrimaryActorTick.bStartWithTickEnabled = false;

 GetMesh()->bReceivesDecals = false;
 GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	
 GetCapsuleComponent()->InitCapsuleSize(42.f,96.f);

 bUseControllerRotationPitch = false;
 bUseControllerRotationYaw = false;
 bUseControllerRotationRoll = false;

 CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
 CameraBoom->SetupAttachment(GetRootComponent());
 CameraBoom->TargetArmLength = 200.f;
 CameraBoom->SocketOffset = FVector(0.f,55.f,65.f);
 CameraBoom->bUsePawnControlRotation = true;

 FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
 FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
 FollowCamera->bUsePawnControlRotation = false;

 GetCharacterMovement()->bOrientRotationToMovement = true;
 GetCharacterMovement()->RotationRate = FRotator(0.f,500.f,0.f);
 GetCharacterMovement()->MaxWalkSpeed = 400.f;
 GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
}

void AJujutsuBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// PlayerController일 때만 Input 설정을 Controller에 위임
	if (AJujutsuHeroController* HeroController = Cast<AJujutsuHeroController>(GetController()))
	{
		HeroController->BindInputActions(PlayerInputComponent);
	}
}
