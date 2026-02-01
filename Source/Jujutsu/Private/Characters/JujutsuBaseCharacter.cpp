// Copyright Epic Games, Inc. All Rights Reserved.



#include "Characters/JujutsuBaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/JujutsuCharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Controllers/JujutsuHeroController.h"
#include "AbilitySystem/JujutsuAbilitySystemComponent.h"
#include "AbilitySystem/JujutsuAttributeSet.h"
#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "Components/Combat/JujutsuCharacterCombatComponent.h"
#include "Components/Push/JujutsuPushComponent.h"
#include "Components/UI/CharacterUIComponent.h"
#include "Components/WidgetComponent.h"
#include "Widgets/JujutsuWidgetBase.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/GameModeBase.h"
#include "GameModes/JujutsuPlayGameMode.h"
// Debug
#include "JujutsuDebugHelper.h"

// Sets default values
AJujutsuBaseCharacter::AJujutsuBaseCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UJujutsuCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set this character to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetMesh()->bReceivesDecals = false;
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
		
	GetCapsuleComponent()->InitCapsuleSize(42.f,96.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 200.f;
	CameraBoom->SocketOffset = FVector(0.f,55.f,65.f);
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f,500.f,0.f);
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	JujutsuAbilitySystemComponent = CreateDefaultSubobject<UJujutsuAbilitySystemComponent>(TEXT("JujutsuAbilitySystemComponent"));

	JujutsuAttributeSet = CreateDefaultSubobject<UJujutsuAttributeSet>(TEXT("JujutsuAttributeSet"));

	CharacterCombatComponent = CreateDefaultSubobject<UJujutsuCharacterCombatComponent>(TEXT("CharacterCombatComponent"));

	PushComponent = CreateDefaultSubobject<UJujutsuPushComponent>(TEXT("PushComponent"));

	CharacterUIComponent = CreateDefaultSubobject<UCharacterUIComponent>(TEXT("CharacterUIComponent"));

	CharacterHealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("CharacterHealthWidgetComponent"));
	CharacterHealthWidgetComponent->SetupAttachment(GetMesh());

	LeftHandCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftHandCollisionBox"));
	LeftHandCollisionBox->SetupAttachment(GetMesh());
	LeftHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftHandCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnBodyCollisionBoxBeginOverlap);
	LeftHandCollisionBox->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnBodyCollisionBoxEndOverlap);

	RightHandCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RightHandCollisionBox"));
	RightHandCollisionBox->SetupAttachment(GetMesh());
	RightHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightHandCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnBodyCollisionBoxBeginOverlap);
	RightHandCollisionBox->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnBodyCollisionBoxEndOverlap);

	LeftFootCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftFootCollisionBox"));
	LeftFootCollisionBox->SetupAttachment(GetMesh());
	LeftFootCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftFootCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnBodyCollisionBoxBeginOverlap);
	LeftFootCollisionBox->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnBodyCollisionBoxEndOverlap);

	RightFootCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RightFootCollisionBox"));
	RightFootCollisionBox->SetupAttachment(GetMesh());
	RightFootCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightFootCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnBodyCollisionBoxBeginOverlap);
	RightFootCollisionBox->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnBodyCollisionBoxEndOverlap);
}

UAbilitySystemComponent* AJujutsuBaseCharacter::GetAbilitySystemComponent() const
{
	return GetJujutsuAbilitySystemComponent();
}

void AJujutsuBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 위젯은 보통 BeginPlay 시점쯤 생성됨. 여기서 바인딩해두면 대미지 시 체력바 갱신 가능.
	if (CharacterHealthWidgetComponent)
	{
		if (UJujutsuWidgetBase* HealthWidget = Cast<UJujutsuWidgetBase>(CharacterHealthWidgetComponent->GetWidget()))
		{
			HealthWidget->InitCharacterCreatedWidget(this);
		}
	}

	if (UWorld* World = GetWorld())
	{
		if (AGameModeBase* GM = World->GetAuthGameMode())
		{
			if (AJujutsuPlayGameMode* PlayGM = Cast<AJujutsuPlayGameMode>(GM))
			{
				PlayGM->NotifyCharacterReady(this);
			}
		}
	}
}

void AJujutsuBaseCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if (UJujutsuCharacterMovementComponent* JutsuMove = Cast<UJujutsuCharacterMovementComponent>(GetCharacterMovement()))
	{
		JutsuMove->JumpCount = 0;
	}
}

void AJujutsuBaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (JujutsuAbilitySystemComponent)
	{
		JujutsuAbilitySystemComponent->InitAbilityActorInfo(this, this);
		ensureMsgf(!CharacterStartUpData.IsNull(), TEXT("Forgot to assign start up data to %s"), *GetName());

		// GE 적용 시 OnCurrentHealthChanged.Broadcast 호출됨. 위젯이 그 시점에 이미 바인딩돼 있어야 초기 체력을 받음. PossessedBy가 BeginPlay보다 먼저 오면 위젯이 아직 null일 수 있으므로 여기서 한 번 더 시도.
		if (CharacterHealthWidgetComponent)
		{
			if (UJujutsuWidgetBase* HealthWidget = Cast<UJujutsuWidgetBase>(CharacterHealthWidgetComponent->GetWidget()))
			{
				HealthWidget->InitCharacterCreatedWidget(this);
			}
		}

		if (!CharacterStartUpData.IsNull())
		{
			if (UDataAsset_StartUpDataBase* LoadedData = CharacterStartUpData.LoadSynchronous())
			{
				LoadedData->GiveToAbilitySystemComponent(JujutsuAbilitySystemComponent);
			}
		}
	}
}

#if WITH_EDITOR
void AJujutsuBaseCharacter::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, LeftHandCollisionBoxAttachBoneName))
	{
		LeftHandCollisionBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, LeftHandCollisionBoxAttachBoneName);
	}

	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, RightHandCollisionBoxAttachBoneName))
	{
		RightHandCollisionBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, RightHandCollisionBoxAttachBoneName);
	}

	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, LeftFootCollisionBoxAttachBoneName))
	{
		LeftFootCollisionBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, LeftFootCollisionBoxAttachBoneName);
	}

	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, RightFootCollisionBoxAttachBoneName))
	{
		RightFootCollisionBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, RightFootCollisionBoxAttachBoneName);
	}
}
#endif

void AJujutsuBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// PlayerController일 때만 Input 설정을 Controller에 위임
	if (AJujutsuHeroController* HeroController = Cast<AJujutsuHeroController>(GetController()))
	{
		HeroController->BindInputActions(PlayerInputComponent);
	}
}

void AJujutsuBaseCharacter::OnBodyCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		if (HitPawn != this)
		{
			OnBodyHitTarget.ExecuteIfBound(OtherActor);
		}
	}
}

void AJujutsuBaseCharacter::OnBodyCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		if (HitPawn != this)
		{
			OnBodyPulledFromTarget.ExecuteIfBound(OtherActor);
		}
	}
}

void AJujutsuBaseCharacter::RemoveHealthWidgetComponent()
{
	if (CharacterHealthWidgetComponent)
	{
		CharacterHealthWidgetComponent->DestroyComponent();
		CharacterHealthWidgetComponent = nullptr;
	}
}
