// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "JujutsuBaseCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UBoxComponent;
class UCharacterUIComponent;
class UJujutsuAbilitySystemComponent;
class UJujutsuAttributeSet;
class UDataAsset_StartUpDataBase;
class UJujutsuCharacterCombatComponent;
class UJujutsuCharacterMovementComponent;
class UJujutsuPushComponent;
class UWidgetComponent;

DECLARE_DELEGATE_OneParam(FOnTargetInteractedDelegate, AActor*);

UCLASS()
class JUJUTSU_API AJujutsuBaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AJujutsuBaseCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~ Begin IAbilitySystemInterface Interface.
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ End IAbilitySystemInterface Interface

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//~ Begin AActor Interface.
	virtual void BeginPlay() override;
	//~ End AActor Interface.

	//~ Begin APawn Interface.
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface

#if WITH_EDITOR
	//~ Begin UObject Interface.
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	//~ End UObject Interface
#endif

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	UJujutsuAbilitySystemComponent* JujutsuAbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	UJujutsuAttributeSet* JujutsuAttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData")
	TSoftObjectPtr<UDataAsset_StartUpDataBase> CharacterStartUpData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UJujutsuCharacterCombatComponent* CharacterCombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Push", meta = (AllowPrivateAccess = "true"))
	UJujutsuPushComponent* PushComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UCharacterUIComponent* CharacterUIComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* CharacterHealthWidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	UBoxComponent* LeftHandCollisionBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	FName LeftHandCollisionBoxAttachBoneName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	UBoxComponent* RightHandCollisionBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	FName RightHandCollisionBoxAttachBoneName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	UBoxComponent* LeftFootCollisionBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	FName LeftFootCollisionBoxAttachBoneName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	UBoxComponent* RightFootCollisionBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	FName RightFootCollisionBoxAttachBoneName;

protected:
	UFUNCTION()
	void OnBodyCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBodyCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	/** 바디 콜리전이 다른 Pawn과 BeginOverlap 시 브로드캐스트 */
	FOnTargetInteractedDelegate OnBodyHitTarget;
	/** 바디 콜리전이 다른 Pawn과 EndOverlap 시 브로드캐스트 */
	FOnTargetInteractedDelegate OnBodyPulledFromTarget;

	FORCEINLINE UJujutsuAbilitySystemComponent* GetJujutsuAbilitySystemComponent() const { return JujutsuAbilitySystemComponent; }

	FORCEINLINE UJujutsuAttributeSet* GetJujutsuAttributeSet() const { return JujutsuAttributeSet; }

	FORCEINLINE UJujutsuCharacterCombatComponent* GetCharacterCombatComponent() const { return CharacterCombatComponent; }

	FORCEINLINE UJujutsuPushComponent* GetPushComponent() const { return PushComponent; }

	FORCEINLINE UCharacterUIComponent* GetCharacterUIComponent() const { return CharacterUIComponent; }

	FORCEINLINE UWidgetComponent* GetCharacterHealthWidgetComponent() const { return CharacterHealthWidgetComponent; }

	/** 체력 위젯 컴포넌트를 제거(파괴)한다. 블루프린트에서 호출 가능. */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void RemoveHealthWidgetComponent();

	UBoxComponent* GetLeftHandCollisionBox() const { return LeftHandCollisionBox; }
	UBoxComponent* GetRightHandCollisionBox() const { return RightHandCollisionBox; }
	UBoxComponent* GetLeftFootCollisionBox() const { return LeftFootCollisionBox; }
	UBoxComponent* GetRightFootCollisionBox() const { return RightFootCollisionBox; }

private:

#pragma region Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

#pragma endregion

};
