// Copyright Epic Games, Inc. All Rights Reserved.

#include "Items/JujutsuProjectileBase.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Items/CustomProjectileMovement.h"
#include "JujutsuFunctionLibrary.h"
#include "JujutsuGameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Characters/JujutsuBaseCharacter.h"
#include "Components/Combat/JujutsuCharacterCombatComponent.h"
#include "GameFramework/Pawn.h"
#include "GameplayEffect.h"
#include "JujutsuSkillLibrary.h"
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

	// 시전자(Caster 또는 Owner/Instigator)와는 오버랩·이동 시 충돌 무시
	if (AJujutsuBaseCharacter* CasterChar = Caster)
	{
		ProjectileCollisionSphere->IgnoreActorWhenMoving(CasterChar, true);
		if (UJujutsuCharacterCombatComponent* Combat = CasterChar->GetCharacterCombatComponent())
		{
			if (AActor* TargetActor = Combat->Target.Get())
			{
				UJujutsuSkillLibrary::SetObjectRotationToTarget(GetRootComponent(), TargetActor);
			}
		}
	}
	if (AActor* OwnerActor = GetOwner())
	{
		ProjectileCollisionSphere->IgnoreActorWhenMoving(OwnerActor, true);
	}
	if (APawn* InstigatorPawn = GetInstigator())
	{
		ProjectileCollisionSphere->IgnoreActorWhenMoving(InstigatorPawn, true);
	}

	// 나이아가라 이펙트 실행
	if (ProjectileNiagaraComponent)
	{
		if (ProjectileNiagaraSystem)
		{
			ProjectileNiagaraComponent->SetAsset(ProjectileNiagaraSystem);
		}
		ProjectileNiagaraComponent->Activate(true);
	}

	CheckOverlap();
}

void AJujutsuProjectileBase::Destroyed()
{
	EndProjectile();
	Super::Destroyed();
}

void AJujutsuProjectileBase::EndProjectile_Implementation()
{
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

	// SimpleDamage Exec 사용 시 데미지에 Caster 불필요. 참조 해제
	Caster = nullptr;
}

void AJujutsuProjectileBase::OnProjectileBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Debug::Print(FString::Printf(TEXT("OnProjectileBeginOverlap: %s"), OtherActor ? *OtherActor->GetName() : TEXT("null")), FColor::Green);

	if (!OtherActor || OtherActor == GetOwner() || OtherActor == Caster || OtherActor == GetInstigator()) return;
	if (OverlappedActors.Contains(OtherActor)) return;

	OverlappedActors.AddUnique(OtherActor);
	bIsOverlapping = true;

	// 데미지 이펙트 적용: SimpleDamage Exec용. Source(Caster) 불필요. 타겟 ASC가 스펙 만들어 자기에게 적용.
	if (DamageEffectClass)
	{
		FActiveGameplayEffectHandle Handle = UJujutsuSkillLibrary::ApplyDamageEffectToTarget(
			OtherActor,
			DamageEffectClass,
			Damage,
			0,   // UsedComboCount (발사체는 0)
			1,   // Level
			this // InstigatorForContext (히트 결과 등용, null 가능)
		);
		if (Handle.IsValid())
		{
			ActiveDamageHandles.Add(OtherActor, Handle);
		}
	}
}

void AJujutsuProjectileBase::OnProjectileEndOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Debug::Print(FString::Printf(TEXT("OnProjectileEndOverlap: %s"), OtherActor ? *OtherActor->GetName() : TEXT("null")), FColor::Yellow);

	if (OtherActor)
	{
		if (FActiveGameplayEffectHandle* Handle = ActiveDamageHandles.Find(OtherActor))
		{
			if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
			{
				TargetASC->RemoveActiveGameplayEffect(*Handle);
			}
			ActiveDamageHandles.Remove(OtherActor);
		}
	}

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
