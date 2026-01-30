// Copyright Epic Games, Inc. All Rights Reserved.

#include "Items/JujutsuProjectileBase.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "Items/CustomProjectileMovement.h"
#include "JujutsuFunctionLibrary.h"
#include "JujutsuGameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Characters/JujutsuBaseCharacter.h"
#include "GameFramework/Pawn.h"
#include "JujutsuFunctionLibrary.h"
#include "JujutsuGameplayTags.h"

// Debug
#include "JujutsuDebugHelper.h"

AJujutsuProjectileBase::AJujutsuProjectileBase()
	: bIsOverlapping(false)
{
	PrimaryActorTick.bCanEverTick = false;

	InitialLifeSpan = 5.0f;

	ProjectileCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ProjectileCollisionSphere"));
	SetRootComponent(ProjectileCollisionSphere);
	ProjectileCollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ProjectileCollisionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	ProjectileCollisionSphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	ProjectileCollisionSphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	ProjectileCollisionSphere->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnProjectileBeginOverlap);
	ProjectileCollisionSphere->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnProjectileEndOverlap);

	ProjectileNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ProjectileNiagaraComponent"));
	ProjectileNiagaraComponent->SetupAttachment(GetRootComponent());

	ProjectileMovementComp = CreateDefaultSubobject<UCustomProjectileMovement>(TEXT("ProjectileMovementComp"));
	ProjectileMovementComp->SetUpdatedComponent(ProjectileCollisionSphere);
	ProjectileMovementComp->SetMoveType(ProjectileMovementParams.MoveType);
	ProjectileMovementComp->InitialSpeed = 0.0f;
	ProjectileMovementComp->MaxSpeed = ProjectileMovementParams.Speed;
	ProjectileMovementComp->Velocity = FVector::ZeroVector;
	ProjectileMovementComp->bRotationFollowsVelocity = true;
	ProjectileMovementComp->bShouldBounce = false;
	ProjectileMovementComp->ProjectileGravityScale = 0.f;
	ProjectileMovementComp->bAutoActivate = true;
	ProjectileMovementComp->bSweepCollision = true;
}

void AJujutsuProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMovementComp->SetMoveType(ProjectileMovementParams.MoveType);
	ProjectileMovementComp->SetUpdatedComponent(ProjectileCollisionSphere);
	ProjectileMovementComp->MaxSpeed = ProjectileMovementParams.Speed;

	// 시전자(Owner/Instigator)와는 오버랩·이동 시 충돌 무시 (어빌리티에서 스폰 시 Instigator 설정하는 경우 많음)
	if (AActor* OwnerActor = GetOwner())
	{
		ProjectileCollisionSphere->IgnoreActorWhenMoving(OwnerActor, true);
	}
	if (APawn* InstigatorPawn = GetInstigator())
	{
		ProjectileCollisionSphere->IgnoreActorWhenMoving(InstigatorPawn, true);
	}

	CheckOverlap();
}

void AJujutsuProjectileBase::Destroyed()
{
	Super::Destroyed();
}

void AJujutsuProjectileBase::CheckOverlap()
{
	// BeginPlay 시점에 이미 겹쳐 있는 액터 처리 (오버랩 이벤트로만 처리)
}

void AJujutsuProjectileBase::LaunchProjectile(AJujutsuBaseCharacter* Target)
{
	if (!ProjectileMovementComp) return;

	// Target이 유효하면 타겟 방향, 아니면 본인(발사체) Forward 방향으로 발사
	ProjectileMovementComp->SetDirection(Target, ProjectileMovementParams.Speed);
	ProjectileMovementComp->ApplyBehaviorSettings(true, true, InitialLifeSpan);
}

void AJujutsuProjectileBase::OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Debug::Print(FString::Printf(TEXT("OnProjectileBeginOverlap: %s"), OtherActor ? *OtherActor->GetName() : TEXT("null")), FColor::Green);

	// 시전자(Owner/Instigator)는 오버랩 무시
	if (OtherActor == GetOwner() || OtherActor == GetInstigator()) return;
	if (OverlappedActors.Contains(OtherActor)) return;

	OverlappedActors.AddUnique(OtherActor);
	bIsOverlapping = true;

	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		// FGameplayEventData Data;
		// Data.Instigator = GetInstigator();
		// Data.Target = HitPawn;
		// HandleApplyProjectileDamage(HitPawn, Data);
	}
}

void AJujutsuProjectileBase::OnProjectileEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Debug::Print(FString::Printf(TEXT("OnProjectileEndOverlap: %s"), OtherActor ? *OtherActor->GetName() : TEXT("null")), FColor::Yellow);

	OverlappedActors.Remove(OtherActor);
	bIsOverlapping = (OverlappedActors.Num() > 0);
}

// void AJujutsuProjectileBase::HandleApplyProjectileDamage(APawn* InHitPawn, const FGameplayEventData& InPayload)
// {
// 	checkf(ProjectileDamageEffectSpecHandle.IsValid(), TEXT("Forgot to assign a valid spec handle to the projectile: %s"), *GetActorNameOrLabel());
//
// 	BP_OnProjectileOverlapFX(InHitPawn ? InHitPawn->GetActorLocation() : GetActorLocation());
//
// 	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InHitPawn);
// 	if (!TargetASC) return;
//
// 	UAbilitySystemComponent* InstigatorASC = GetInstigator()
// 		? UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetInstigator())
// 		: nullptr;
// 	if (!InstigatorASC) return;
//
// 	FActiveGameplayEffectHandle ActiveHandle = InstigatorASC->ApplyGameplayEffectSpecToTarget(
// 		*ProjectileDamageEffectSpecHandle.Data,
// 		TargetASC
// 	);
//
// 	if (ActiveHandle.WasSuccessfullyApplied())
// 	{
// 		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
// 			InHitPawn,
// 			JujutsuGameplayTags::Character_Event_Hit,
// 			InPayload
// 		);
// 	}
// }
