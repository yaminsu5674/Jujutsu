// Copyright Epic Games, Inc. All Rights Reserved.



#include "JujutsuGameplayTags.h"

namespace JujutsuGameplayTags
{
	/** Input Tags */
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Move, "InputTag.Move");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_A, "InputTag.A");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_S, "InputTag.S");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_D, "InputTag.D");

	/** Character Ability Attack (부모) */
	UE_DEFINE_GAMEPLAY_TAG(Character_Ability_Attack, "Character.Ability.Attack");

	/** Character Ability Attack Combo */
	UE_DEFINE_GAMEPLAY_TAG(Character_Ability_Attack_Combo_A, "Character.Ability.Attack.Combo.A");
	UE_DEFINE_GAMEPLAY_TAG(Character_Ability_Attack_Combo_S, "Character.Ability.Attack.Combo.S");
	UE_DEFINE_GAMEPLAY_TAG(Character_Ability_Attack_Combo_D, "Character.Ability.Attack.Combo.D");

	/** Character Ability HitReact (피격 타입) */
	UE_DEFINE_GAMEPLAY_TAG(Character_Ability_HitReact_Stun, "Character.Ability.HitReact.Stun");
	UE_DEFINE_GAMEPLAY_TAG(Character_Ability_HitReact_Light, "Character.Ability.HitReact.Light");
	UE_DEFINE_GAMEPLAY_TAG(Character_Ability_HitReact_Middle, "Character.Ability.HitReact.Middle");
	UE_DEFINE_GAMEPLAY_TAG(Character_Ability_HitReact_Heavy, "Character.Ability.HitReact.Heavy");

	/** Character Event Hit (부모. Character.Event.Hit — 하위 전부 매칭) */
	UE_DEFINE_GAMEPLAY_TAG(Character_Event_Hit, "Character.Event.Hit");

	/** 공격자 전용. Character.Event.HitSuccess */
	UE_DEFINE_GAMEPLAY_TAG(Character_Event_HitSuccess, "Character.Event.HitSuccess");

	/** Character Event Hit 하위 (피격 이벤트 타입) */
	UE_DEFINE_GAMEPLAY_TAG(Character_Event_Hit_Light, "Character.Event.Hit.Light");
	UE_DEFINE_GAMEPLAY_TAG(Character_Event_Hit_Stun, "Character.Event.Hit.Stun");
	UE_DEFINE_GAMEPLAY_TAG(Character_Event_Hit_Middle, "Character.Event.Hit.Middle");
	UE_DEFINE_GAMEPLAY_TAG(Character_Event_Hit_Heavy, "Character.Event.Hit.Heavy");

	/** Character Ability/Status (Enemy death 등) */
	UE_DEFINE_GAMEPLAY_TAG(Character_Ability_Death, "Character.Ability.Death");
	UE_DEFINE_GAMEPLAY_TAG(Character_Status_Dead, "Character.Status.Dead");

	/** Character SetByCaller tags */
	UE_DEFINE_GAMEPLAY_TAG(Character_SetByCaller_BaseDamage, "Character.SetByCaller.BaseDamage");
	UE_DEFINE_GAMEPLAY_TAG(Character_SetByCaller_UsedComboCount, "Character.SetByCaller.UsedComboCount");
}
