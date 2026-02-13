// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "Items/CustomProjectileMovement.h"
#include "JujutsuTypes/JujutsuEnumTypes.h"
#include "JujutsuProjectileBase.generated.h"

class AJujutsuBaseCharacter;
class UGameplayEffect;
class UNiagaraSystem;
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

	/** 발사체 수명 종료 시 호출. C++/블루프린트 오버라이드 가능. (Destroyed 직전 호출) */
	UFUNCTION(BlueprintNativeEvent, Category = "Projectile")
	void EndProjectile();
	virtual void EndProjectile_Implementation();

	/** BeginPlay 시점에 이미 Target과 오버랩 중이면 감지하여 OnProjectileOverlapBegin 호출 */
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void CheckOverlap();

	/** 발사: Target이 유효하면 타겟 방향, 아니면 본인 Forward 방향으로 이동·속도·수명 적용하여 발사 (Owner 미사용). 블루프린트에서 오버라이드 가능. */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Projectile")
	void LaunchProjectile(AJujutsuBaseCharacter* Target);
	virtual void LaunchProjectile_Implementation(AJujutsuBaseCharacter* Target);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

	/** 클라이언트에서 Caster 복제 수신 시 시전자 무시(콜리전/오버랩) 적용 */
	UFUNCTION()
	void OnRep_Caster();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	USphereComponent* ProjectileCollisionSphere;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	UNiagaraComponent* ProjectileNiagaraComponent;

	/** 재생할 나이아가라 이펙트 (지정 시 BeginPlay에서 컴포넌트에 설정 후 재생) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	TObjectPtr<UNiagaraSystem> ProjectileNiagaraSystem;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	UCustomProjectileMovement* ProjectileMovementComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	FProjectileMovementParams ProjectileMovementParams;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	float Damage = 0.f;

	/** 시전자. 블루프린트 Spawn Actor from Class 시 인풋 핀으로 설정. EndProjectile(소멸) 시 참조 해제. 발사 후에도 overlap 시 시전자 제외용으로 유지. (SimpleDamage 사용 시 데미지에는 미사용). 복제되어 클라이언트에서도 시전자 무시 가능. */
	UPROPERTY(BlueprintReadWrite, Category = "Projectile", ReplicatedUsing = OnRep_Caster, meta = (ExposeOnSpawn = "true"))
	TObjectPtr<AJujutsuBaseCharacter> Caster;

	/** 오버랩 중인 타겟에게 적용할 데미지 게임플레이 이펙트 (지정 시 BeginOverlap에서 적용, EndOverlap에서 제거) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(BlueprintReadOnly, Category = "Projectile", meta = (ExposeOnSpawn = "true"))
	FGameplayEffectSpecHandle ProjectileDamageEffectSpecHandle;

	/** 현재 Target과 오버랩 중인지 */
	UPROPERTY(BlueprintReadOnly, Category = "Projectile")
	bool bIsOverlapping = false;

	/** LaunchProjectile 호출 여부 */
	UPROPERTY(BlueprintReadOnly, Category = "Projectile")
	bool bDidLaunched = false;

	UFUNCTION(BlueprintNativeEvent, Category = "Projectile")
	void OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual void OnProjectileBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintNativeEvent, Category = "Projectile")
	void OnProjectileEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	virtual void OnProjectileEndOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/** 오버랩 시 데미지 적용 직전 호출 (이펙트 등) */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Projectile Overlap FX"))
	void BP_OnProjectileOverlapFX(const FVector& HitLocation);

protected:
	/** 현재 오버랩 중인 액터 목록. 자식 클래스에서 EndProjectile 등에서 활용 가능 */
	TArray<AActor*> OverlappedActors;

	/** 오버랩 중인 액터별 적용된 데미지 이펙트 핸들. EndOverlap 시 제거용 */
	TMap<TObjectPtr<AActor>, FActiveGameplayEffectHandle> ActiveDamageHandles;

private:
	// void HandleApplyProjectileDamage(APawn* InHitPawn, const FGameplayEventData& InPayload);
};
