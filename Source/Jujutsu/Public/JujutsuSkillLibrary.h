// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayEffectTypes.h"
#include "JujutsuSkillLibrary.generated.h"

class AActor;
class AJujutsuBaseCharacter;
class USceneComponent;
class UGameplayEffect;

/**
 * 스킬/전투 관련 블루프린트·네이티브 유틸
 */
UCLASS()
class JUJUTSU_API UJujutsuSkillLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** 베이스캐릭터의 CombatComponent에서 타겟을 찾고, 있으면 그 방향으로 캐릭터(및 컨트롤러) 회전 */
	UFUNCTION(BlueprintCallable, Category = "Jujutsu|SkillLibrary")
	static void SetActorRotationToTarget(AJujutsuBaseCharacter* InCharacter);

	/** Object를 Target을 바라보도록 회전 (Yaw만 적용, +90도 오프셋) */
	UFUNCTION(BlueprintCallable, Category = "Jujutsu|SkillLibrary", meta = (DisplayName = "Set Object Rotation To Target"))
	static void SetObjectRotationToTarget(USceneComponent* Object, AActor* Target);

	/**
	 * 액터를 기준으로 발사체 스폰 월드 위치를 반환.
	 * @param Actor 기준 액터 (바라보는 방향·위치 사용)
	 * @param Offset 액터 로컬 오프셋 (X=앞, Y=좌우, Z=위아래). 단위는 유닛(예: 앞 100, 오른쪽 50, 위 30)
	 * @return 스폰할 월드 위치. Actor가 null이면 (0,0,0) 반환
	 */
	UFUNCTION(BlueprintCallable, Category = "Jujutsu|SkillLibrary", meta = (DisplayName = "Get Spawn Location From Character"))
	static FVector GetSpawnLocationFromCharacter(AActor* Actor, FVector Offset);

	/** SourceActor의 ASC로 데미지용 GE 스펙 생성. BaseDamage·UsedComboCount는 SetByCaller로 전달. 어빌리티가 아닌 곳에서 데미지 적용 시 사용 */
	UFUNCTION(BlueprintCallable, Category = "Jujutsu|SkillLibrary", meta = (DisplayName = "Make Damage Effect Spec Handle"))
	static FGameplayEffectSpecHandle MakeDamageEffectSpecHandle(AActor* SourceActor, TSubclassOf<UGameplayEffect> EffectClass, float BaseDamage, int32 InUsedComboCount, int32 Level = 1);

	/** SourceActor의 ASC로 스펙을 TargetActor의 ASC에 적용. MakeDamageEffectSpecHandle으로 만든 스펙 적용 시 사용 */
	UFUNCTION(BlueprintCallable, Category = "Jujutsu|SkillLibrary", meta = (DisplayName = "Apply Effect Spec Handle To Target"))
	static FActiveGameplayEffectHandle ApplyEffectSpecHandleToTarget(AActor* SourceActor, AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle);

	/**
	 * Source(시전자) ASC 없이 타겟에게 데미지 GE 적용. SimpleDamage Exec용.
	 * TargetActor의 ASC가 스펙을 만들고 자기 자신에게 적용. InstigatorForContext는 컨텍스트용(예: 발사체), null 가능.
	 */
	UFUNCTION(BlueprintCallable, Category = "Jujutsu|SkillLibrary", meta = (DisplayName = "Apply Damage Effect To Target (No Source)"))
	static FActiveGameplayEffectHandle ApplyDamageEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> EffectClass, float BaseDamage, int32 InUsedComboCount, int32 Level = 1, AActor* InstigatorForContext = nullptr);
};
