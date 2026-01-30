// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "JujutsuTypes/JujutsuEnumTypes.h"
#include "CustomProjectileMovement.generated.h"

/**
 * Custom Projectile Movement Component
 * ProjectileMovementComponent를 상속받아 커스텀 동작 구현
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class JUJUTSU_API UCustomProjectileMovement : public UProjectileMovementComponent
{
	GENERATED_BODY()

public:
	UCustomProjectileMovement();

protected:
	/** 발사체 이동 타입 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom Projectile")
	EProjectileMoveType MoveType;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** 충돌 처리 함수 오버라이드 */
	virtual void HandleImpact(const FHitResult& Hit, float TimeSlice = 0.f, const FVector& MoveDelta = FVector::ZeroVector) override;

	/** 블로킹 충돌 처리 오버라이드 */
	virtual EHandleBlockingHitResult HandleBlockingHit(const FHitResult& Hit, float TimeTick, const FVector& MoveDelta, float& SubTickTimeRemaining) override;

public:
	/** 이동 타입 설정 */
	UFUNCTION(BlueprintCallable, Category = "Custom Projectile")
	void SetMoveType(EProjectileMoveType NewMoveType);

	/** 발사 방향 설정 (Target을 향해 발사) */
	UFUNCTION(BlueprintCallable, Category = "Custom Projectile")
	void SetDirection(AActor* TargetActor, float ProjectileSpeed);

	/** 행동 타입에 따른 설정 적용 */
	UFUNCTION(BlueprintCallable, Category = "Custom Projectile")
	void ApplyBehaviorSettings(bool bShouldMove, bool bEnableLifespan, float LifespanDuration);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Projectile")
	bool bNeverStop;
};
