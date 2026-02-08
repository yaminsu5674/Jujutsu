// Copyright Epic Games, Inc. All Rights Reserved.

#include "Controllers/JujutsuAIController.h"
#include "Characters/JujutsuBaseCharacter.h"
#include "AbilitySystem/JujutsuAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/JujutsuGameplayAbility.h"
#include "Abilities/GameplayAbility.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/PlayerController.h"

AJujutsuAIController::AJujutsuAIController()
{
}

FGameplayTag AJujutsuAIController::SelectRandomSkill(FGameplayTag AbilityTagToActivate)
{

	check(AbilityTagToActivate.IsValid());

	SelectedRandomSkillTag = FGameplayTag();

	AJujutsuBaseCharacter* BaseCharacter = Cast<AJujutsuBaseCharacter>(GetPawn());
	UJujutsuAbilitySystemComponent* ASC = BaseCharacter ? BaseCharacter->GetJujutsuAbilitySystemComponent() : nullptr;
	if (!ASC) return FGameplayTag();

	TArray<FGameplayAbilitySpec*> FoundAbilitySpecs;
	ASC->GetActivatableGameplayAbilitySpecsByAllMatchingTags(AbilityTagToActivate.GetSingleTagContainer(), FoundAbilitySpecs);

	UE_LOG(LogTemp, Warning, TEXT("SelectRandomSkill: Found %d abilities for tag %s"), FoundAbilitySpecs.Num(), *AbilityTagToActivate.ToString());

	if (!FoundAbilitySpecs.IsEmpty())
	{
		const int32 RandomAbilityIndex = FMath::RandRange(0, FoundAbilitySpecs.Num() - 1);
		FGameplayAbilitySpec* SpecToActivate = FoundAbilitySpecs[RandomAbilityIndex];

		check(SpecToActivate);
		UE_LOG(LogTemp, Warning, TEXT("SelectRandomSkill: Picked index %d of %d"), RandomAbilityIndex, FoundAbilitySpecs.Num());

		for (const FGameplayTag& Tag : SpecToActivate->DynamicAbilityTags)
		{
			if (Tag.MatchesTag(AbilityTagToActivate))
			{
				SelectedRandomSkillTag = Tag;
				UE_LOG(LogTemp, Warning, TEXT("SelectRandomSkill: Selected tag %s"), *Tag.ToString());
				return Tag;
			}
		}

		// DynamicAbilityTags가 비어 있는 경우 (GrantAbilities로 부여된 어빌리티) Ability 정의의 AssetTags 사용
		if (UGameplayAbility* Ability = SpecToActivate->Ability)
		{
			for (const FGameplayTag& Tag : Ability->GetAssetTags())
			{
				if (Tag.MatchesTag(AbilityTagToActivate))
				{
					SelectedRandomSkillTag = Tag;
					UE_LOG(LogTemp, Warning, TEXT("SelectRandomSkill: Selected tag from Ability def %s"), *Tag.ToString());
					return Tag;
				}
			}
		}

		SelectedRandomSkillTag = AbilityTagToActivate;
		UE_LOG(LogTemp, Warning, TEXT("SelectRandomSkill: Selected (fallback) %s"), *AbilityTagToActivate.ToString());
		return AbilityTagToActivate;
	}

	return FGameplayTag();
}

float AJujutsuAIController::GetDesiredRangeByTag(FGameplayTag AbilityTag)
{
	if (!AbilityTag.IsValid()) return 0.f;

	AJujutsuBaseCharacter* BaseCharacter = Cast<AJujutsuBaseCharacter>(GetPawn());
	UJujutsuAbilitySystemComponent* ASC = BaseCharacter ? BaseCharacter->GetJujutsuAbilitySystemComponent() : nullptr;
	if (!ASC) return 0.f;

	TArray<FGameplayAbilitySpec*> FoundAbilitySpecs;
	ASC->GetActivatableGameplayAbilitySpecsByAllMatchingTags(AbilityTag.GetSingleTagContainer(), FoundAbilitySpecs);

	if (!FoundAbilitySpecs.IsEmpty())
	{
		UGameplayAbility* Ability = FoundAbilitySpecs[0]->Ability;
		if (const UJujutsuGameplayAbility* JutsuAbility = Cast<UJujutsuGameplayAbility>(Ability))
		{
			return JutsuAbility->DesiredRange;
		}
	}

	return 0.f;
}

void AJujutsuAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void AJujutsuAIController::InitBlackboard()
{
	APlayerController* PC = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr;
	if (!PC || !PC->GetPawn()) return;

	AJujutsuBaseCharacter* PlayerCharacter = Cast<AJujutsuBaseCharacter>(PC->GetPawn());
	if (!PlayerCharacter) return;

	if (UBlackboardComponent* BlackboardComponent = GetBlackboardComponent())
	{
		BlackboardComponent->SetValueAsObject(FName("TargetActor"), PlayerCharacter);
	}
}
