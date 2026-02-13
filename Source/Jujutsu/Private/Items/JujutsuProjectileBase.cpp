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
#include "Net/UnrealNetwork.h"

// Debug
#include "JujutsuDebugHelper.h"

AJujutsuProjectileBase::AJujutsuProjectileBase()
	: bIsOverlapping(false)
{
	PrimaryActorTick.bCanEverTick = false;

	// 서버가 스폰한 발사체가 클라이언트(클라2)에 보이도록 복제. 리슨 서버(클라1)가 쓴 스킬 발사체가 클라2 화면에 나오게 함.
	bReplicates = true;
	SetReplicateMovement(true);

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

void AJujutsuProjectileBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AJujutsuProjectileBase, Caster);
}

void AJujutsuProjectileBase::OnRep_Caster()
{
	// 클라이언트: 복제된 Caster에 대해 시전자 무시 적용 (BeginPlay는 Caster 수신 전에 올 수 있음)
	if (ProjectileCollisionSphere && Caster)
	{
		ProjectileCollisionSphere->IgnoreActorWhenMoving(Caster, true);
		if (UJujutsuCharacterCombatComponent* Combat = Caster->GetCharacterCombatComponent())
		{
			if (AActor* TargetActor = Combat->Target.Get())
			{
				UJujutsuSkillLibrary::SetObjectRotationToTarget(GetRootComponent(), TargetActor);
			}
		}
	}
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

void AJujutsuProjectileBase::LaunchProjectile_Implementation(AJujutsuBaseCharacter* Target)
{
	if (!ProjectileMovementComp) return;

	bDidLaunched = true;

	// Target이 유효하면 타겟 방향, 아니면 본인(발사체) Forward 방향으로 발사
	ProjectileMovementComp->SetDirection(Target, ProjectileMovementParams.Speed);
	ProjectileMovementComp->ApplyBehaviorSettings(true, true, InitialLifeSpan);
}

void AJujutsuProjectileBase::OnProjectileBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor) return;

	// 방어벽 업그레이드: 시전자 본인 + 시전자가 주인인 무기/부착물(GetOwner/GetInstigator) 모두 차단
	const bool bIsCasterFamily = (OtherActor == Caster) ||
		(OtherActor->GetOwner() == Caster) ||
		(OtherActor->GetInstigator() == Caster);
	if (OtherActor == GetOwner() || bIsCasterFamily) return;
	if (OverlappedActors.Contains(OtherActor)) return;

	Debug::Print(FString::Printf(TEXT("적중한 타겟: %s"), *OtherActor->GetName()), FColor::Green);

	OverlappedActors.AddUnique(OtherActor);
	bIsOverlapping = true;

	// 연출(Visual) 영역: 서버·클라이언트 모두 실행 (클라2에서도 타격 이펙트/사운드 재생)
	if (OtherActor != GetOwner() && OtherActor != Caster && OtherActor != GetInstigator())
	{
		AfterOverlapBegin(OtherActor);
	}

	// 로직(Logic) 영역: 데미지는 서버만
	if (!HasAuthority()) return;

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
	if (!OtherActor) return;

	const bool bIsCasterFamily = (OtherActor == Caster) ||
		(OtherActor->GetOwner() == Caster) ||
		(OtherActor->GetInstigator() == Caster);
	if (OtherActor == GetOwner() || bIsCasterFamily) return;

	Debug::Print(FString::Printf(TEXT("적중한 타겟: %s"), *OtherActor->GetName()), FColor::Green);

	OverlappedActors.Remove(OtherActor);
	bIsOverlapping = (OverlappedActors.Num() > 0);

	// 연출: 서버·클라이언트 모두 실행
	if (OtherActor != GetOwner() && OtherActor != Caster && OtherActor != GetInstigator())
	{
		AfterOverlapEnd(OtherActor);
	}

	// 로직: 이펙트 제거는 서버만
	if (!HasAuthority()) return;

	if (FActiveGameplayEffectHandle* Handle = ActiveDamageHandles.Find(OtherActor))
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			TargetASC->RemoveActiveGameplayEffect(*Handle);
		}
		ActiveDamageHandles.Remove(OtherActor);
	}
}
