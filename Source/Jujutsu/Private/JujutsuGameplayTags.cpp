// Copyright Epic Games, Inc. All Rights Reserved.



#include "JujutsuGameplayTags.h"

namespace JujutsuGameplayTags
{
	/** Input Tags */
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Move, "InputTag.Move");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Jump, "InputTag.Jump");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_SuperJump, "InputTag.SuperJump");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_A, "InputTag.A");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_S, "InputTag.S");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_D, "InputTag.D");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_ER, "InputTag.ER");

	/** Character Ability Attack (부모) */
	UE_DEFINE_GAMEPLAY_TAG(Character_Ability_Attack, "Character.Ability.Attack");

	/** Character Ability Attack Combo (부모) */
	UE_DEFINE_GAMEPLAY_TAG(Character_Ability_Attack_Combo, "Character.Ability.Attack.Combo");
	UE_DEFINE_GAMEPLAY_TAG(Character_Ability_Attack_Jujutsu, "Character.Ability.Attack.Jujutsu");
	UE_DEFINE_GAMEPLAY_TAG(Character_Ability_Attack_Special, "Character.Ability.Attack.Special");
	UE_DEFINE_GAMEPLAY_TAG(Character_Ability_Attack_Final, "Character.Ability.Attack.Final");

	/** Character Ability Move (부모) */
	UE_DEFINE_GAMEPLAY_TAG(Character_Ability_Move, "Character.Ability.Move");
	UE_DEFINE_GAMEPLAY_TAG(Character_Ability_Move_Jump, "Character.Ability.Move.Jump");
	UE_DEFINE_GAMEPLAY_TAG(Character_Ability_Move_Dash, "Character.Ability.Move.Dash");

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

	/** Skill Event */
	UE_DEFINE_GAMEPLAY_TAG(Skill_Event_Notify, "Skill.Event.Notify");

	/** Character Ability/Status (Enemy death 등) */
	UE_DEFINE_GAMEPLAY_TAG(Character_Ability_Death, "Character.Ability.Death");
	UE_DEFINE_GAMEPLAY_TAG(Character_Status_Hit, "Character.Status.Hit");
	UE_DEFINE_GAMEPLAY_TAG(Character_Status_Dead, "Character.Status.Dead");

	/** Character SetByCaller tags */
	UE_DEFINE_GAMEPLAY_TAG(Character_SetByCaller_BaseDamage, "Character.SetByCaller.BaseDamage");
	UE_DEFINE_GAMEPLAY_TAG(Character_SetByCaller_UsedComboCount, "Character.SetByCaller.UsedComboCount");
}
