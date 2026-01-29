// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "NativeGameplayTags.h"

namespace JujutsuGameplayTags
{
	/** Input Tags */
	JUJUTSU_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
	JUJUTSU_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_A);
	JUJUTSU_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_S);
	JUJUTSU_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_D);

	/** Character Ability Attack (부모. Attack_Combo_* 등 하위 전부 매칭) */
	JUJUTSU_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Ability_Attack);

	/** Character Ability Attack Combo */
	JUJUTSU_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Ability_Attack_Combo_A);
	JUJUTSU_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Ability_Attack_Combo_S);
	JUJUTSU_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Ability_Attack_Combo_D);

	/** Character Ability HitReact (피격 타입) */
	JUJUTSU_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Ability_HitReact_Stun);
	JUJUTSU_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Ability_HitReact_Light);
	JUJUTSU_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Ability_HitReact_Middle);
	JUJUTSU_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Ability_HitReact_Heavy);

	/** Character Event Hit (피격 이벤트 부모. 하위 Light/Stun/Middle/Heavy 전부 Hit으로 적용하려면 MatchesTag(Character_Event_Hit) 사용) */
	JUJUTSU_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Event_Hit);

	/** 공격자에게만 전달. "때렸다 맞췄다" — 블루프린트에서 이 이벤트 대기 후 Data.Target에게 피격반응/데미지 */
	JUJUTSU_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Event_HitSuccess);

	/** Character Event Hit 하위 (피격 이벤트 타입) */
	JUJUTSU_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Event_Hit_Light);
	JUJUTSU_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Event_Hit_Stun);
	JUJUTSU_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Event_Hit_Middle);
	JUJUTSU_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Event_Hit_Heavy);

	/** Character Ability/Status (Enemy death 등) */
	JUJUTSU_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Ability_Death);
	JUJUTSU_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Status_Dead);

	/** Character SetByCaller tags */
	JUJUTSU_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_SetByCaller_BaseDamage);
	JUJUTSU_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_SetByCaller_UsedComboCount);
}
