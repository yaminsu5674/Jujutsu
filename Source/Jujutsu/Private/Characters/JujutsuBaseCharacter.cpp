// Copyright Epic Games, Inc. All Rights Reserved.



#include "Characters/JujutsuBaseCharacter.h"

// Sets default values
AJujutsuBaseCharacter::AJujutsuBaseCharacter()
{
 // Set this character to call Tick() every frame. You can turn this off to improve performance if you don't need it.
 PrimaryActorTick.bCanEverTick = false;
 PrimaryActorTick.bStartWithTickEnabled = false;

 GetMesh()->bReceivesDecals = false;

}
