// Copyright Epic Games, Inc. All Rights Reserved.

#include "Widgets/JujutsuWidgetBase.h"
#include "Characters/JujutsuBaseCharacter.h"
#include "Components/UI/CharacterUIComponent.h"
#include "GameFramework/Actor.h"

void UJujutsuWidgetBase::InitCharacterCreatedWidget_Implementation(AActor* OwningCharacter)
{
	// WarriorRPG InitEnemyCreatedWidget 패턴: Actor에서 UI 컴포넌트를 꺼내서 동일한 BP 이벤트로 넘김
	// → 체력바 위젯은 "Event On Owning Character UI Component Initialized" 하나로 바인딩 가능
	if (AJujutsuBaseCharacter* BaseCharacter = Cast<AJujutsuBaseCharacter>(OwningCharacter))
	{
		if (UCharacterUIComponent* UIComp = BaseCharacter->GetCharacterUIComponent())
		{
			BP_OnOwningCharacterUIComponentInitialized(UIComp);
			UIComp->BroadcastCurrentHealthAndRage();
		}
	}
}

void UJujutsuWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	// 자동 바인딩 제거: GetOwningPlayer()->GetPawn()으로 스스로 주인을 찾으면, SetOwnerPlayer(LocalPlayer)된 적 체력바가 플레이어에 바인딩되어 이중 구독됨.
	// 모든 위젯은 외부에서 InitCharacterCreatedWidget 호출 시에만 바인딩. (캐릭터 머리 위 = BeginPlay, Hero 오버레이 = GA_DrawOverlayWidget에서 생성 직후)
}
