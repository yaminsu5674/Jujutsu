// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/JujutsuPawnExtensionComponentBase.h"
#include "GameplayTagContainer.h"
#include "GameplayEffect.h"
#include "JujutsuTypes/JujutsuEnumTypes.h"
#include "JujutsuCharacterCombatComponent.generated.h"

class AActor;
class AJujutsuBaseCharacter;

/**
 * 캐릭터 전투 컴포넌트
 * 전투/히트 처리 등 확장용 (폰컴뱃 베이스 없이 단일 클래스)
 */
UCLASS()
class JUJUTSU_API UJujutsuCharacterCombatComponent : public UJujutsuPawnExtensionComponentBase
{
	GENERATED_BODY()

public:
	UJujutsuCharacterCombatComponent();
	/** 전부 켜기/끄기 */
	UFUNCTION(BlueprintCallable, Category = "Jujutsu|Combat")
	void ToggleBodyCollision(bool bShouldEnable);

	/** 지정한 부위(1~4개)의 콜리전만 켜거나 끔. Parts 비어있으면 전부 켜기/끄기 */
	UFUNCTION(BlueprintCallable, Category = "Jujutsu|Combat", meta = (AutoCreateRefTerm = "Parts"))
	void ToggleBodyCollisionParts(bool bShouldEnable, const TArray<EBodyCollisionPart>& Parts);

	/** 바디 콜리전 BeginOverlap 시 호출 (히트 시) */
	virtual void OnHitTargetActor(AActor* HitActor);
	/** 바디 콜리전 EndOverlap 시 호출 (타겟에서 벗어남) */
	virtual void OnPulledFromTargetActor(AActor* InteractedActor);

	/** 현재 타겟 (약한 참조). 타겟 락 등에서 여기서 관리 */
	UPROPERTY(BlueprintReadOnly, Category = "Jujutsu|Combat")
	TWeakObjectPtr<AJujutsuBaseCharacter> Target;

	/** 바디 콜리전 히트 시 피격자에게 전달할 이벤트 태그 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Jujutsu|Combat")
	FGameplayTag HitEventTag;

	/** 바디 콜리전 히트 시 피격자에게 적용할 데미지 게임플레이 이펙트 (지정 시 OnHitTargetActor에서 적용) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Jujutsu|Combat|Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	/** DamageEffectClass 사용 시 적용할 기본 데미지 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Jujutsu|Combat|Damage", meta = (ClampMin = "0"))
	float Damage = 100.f;

	/** HitEventTag 설정. 블루프린트에서 호출 가능 */
	UFUNCTION(BlueprintCallable, Category = "Jujutsu|Combat")
	void SetHitEventTag(FGameplayTag InTag);

	/** 베이스 데미지 설정. 블루프린트에서 호출 가능 */
	UFUNCTION(BlueprintCallable, Category = "Jujutsu|Combat|Damage", meta = (ClampMin = "0"))
	void SetBaseDamage(float InDamage);

protected:
	virtual void BeginPlay() override;

	/** 현재 오버랩 중인 액터 (Hit 이벤트 중복 발동 방지·콜리전 끌 때 비움) */
	TArray<AActor*> OverlappedActors;
};
