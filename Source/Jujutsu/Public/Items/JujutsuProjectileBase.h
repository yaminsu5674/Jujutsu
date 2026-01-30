// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "Items/CustomProjectileMovement.h"
#include "JujutsuTypes/JujutsuEnumTypes.h"
#include "JujutsuProjectileBase.generated.h"

class USphereComponent;
class USceneComponent;
class UNiagaraComponent;
struct FGameplayEventData;

/** 발사체 이동 관련 파라미터 (이동 타입, 속도) */
USTRUCT(BlueprintType)
struct FProjectileMovementParams
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	EProjectileMoveType MoveType = EProjectileMoveType::Single;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	float Speed = 700.f;
};

/**
 * 발사체 베이스 액터.
 * Sphere 콜리전 + CustomProjectileMovement 사용. 오버랩만 판단하여 데미지 적용.
 * Target이 설정되면 해당 액터와의 오버랩만 처리하며, Owner의 CombatComponent에서 Target을 가져올 수 있음.
 */
UCLASS()
class JUJUTSU_API AJujutsuProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	AJujutsuProjectileBase();

	virtual void Destroyed() override;

	/** BeginPlay 시점에 이미 Target과 오버랩 중이면 감지하여 OnProjectileOverlapBegin 호출 */
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void CheckOverlap();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	USphereComponent* ProjectileCollisionSphere;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	UNiagaraComponent* ProjectileNiagaraComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	UCustomProjectileMovement* ProjectileMovementComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	FProjectileMovementParams ProjectileMovementParams;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	float Damage = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Projectile", meta = (ExposeOnSpawn = "true"))
	FGameplayEffectSpecHandle ProjectileDamageEffectSpecHandle;

	/** 현재 Target과 오버랩 중인지 */
	UPROPERTY(BlueprintReadOnly, Category = "Projectile")
	bool bIsOverlapping = false;

	UFUNCTION()
	virtual void OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnProjectileEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/** 오버랩 시 데미지 적용 직전 호출 (이펙트 등) */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Projectile Overlap FX"))
	void BP_OnProjectileOverlapFX(const FVector& HitLocation);

private:
	// void HandleApplyProjectileDamage(APawn* InHitPawn, const FGameplayEventData& InPayload);

	TArray<AActor*> OverlappedActors;
};
