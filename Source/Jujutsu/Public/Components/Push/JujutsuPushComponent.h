// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "JujutsuTypes/JujutsuEnumTypes.h"
#include "JujutsuPushComponent.generated.h"

class AActor;
class ACharacter;
class UCharacterMovementComponent;

USTRUCT(BlueprintType)
struct FPushRequest
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	EPushMode Mode = EPushMode::Impulse;

	UPROPERTY(BlueprintReadWrite)
	FVector Direction = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite)
	float Strength = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float Duration = 0.f;

	UPROPERTY(BlueprintReadWrite)
	TWeakObjectPtr<AActor> Source;

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag PushType;
};

/**
 * 캐릭터 밀기(Push) 컴포넌트
 * 밀기/넉백 등 확장용 (AttachToSource / Impulse)
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class JUJUTSU_API UJujutsuPushComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UJujutsuPushComponent();

	UFUNCTION(BlueprintCallable, Category = "Jujutsu|Push")
	void RequestPush(const FPushRequest& Request);

	UFUNCTION(BlueprintCallable, Category = "Jujutsu|Push")
	void StopPush();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void TickAttachPush(float DeltaTime);
	void TickImpulsePush(float DeltaTime);
	void EndPush();

private:
	UPROPERTY()
	TWeakObjectPtr<ACharacter> OwnerCharacter;

	UPROPERTY()
	TWeakObjectPtr<UCharacterMovementComponent> MoveComp;

	bool bActive = false;
	float ElapsedTime = 0.f;

	FPushRequest ActivePush;

	/** AttachToSource 모드용: 이전 프레임 Source 위치 */
	FVector PrevSourceLocation = FVector::ZeroVector;
};
