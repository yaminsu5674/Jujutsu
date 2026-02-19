// Copyright Epic Games, Inc. All Rights Reserved.



#include "JujutsuGameplayTags.h"

namespace JujutsuGameplayTags
{
	/** Input Tags */
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Move, "InputTag.Move");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Jump, "InputTag.Jump");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_SuperJump, "InputTag.SuperJump");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Dash, "InputTag.Dash");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_A, "InputTag.A");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_S, "InputTag.S");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_D, "InputTag.D");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_E, "InputTag.E");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_R, "InputTag.R");
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
	UE_DEFINE_GAMEPLAY_TAG(Character_Ability_Move_SuperJump, "Character.Ability.Move.SuperJump");
	UE_DEFINE_GAMEPLAY_TAG(Character_Ability_Move_Dash, "Character.Ability.Move.Dash");

	/** Character Ability HitReact (부모. Character.Ability.HitReact) */
	UE_DEFINE_GAMEPLAY_TAG(Character_Ability_HitReact, "Character.Ability.HitReact");
	UE_DEFINE_GAMEPLAY_TAG(Character_Ability_HitReact_Stun, "Character.Ability.HitReact.Stun");
	UE_DEFINE_GAMEPLAY_TAG(Character_Ability_HitReact_Light, "Character.Ability.HitReact.Light");
	UE_DEFINE_GAMEPLAY_TAG(Character_Ability_HitReact_Middle, "Character.Ability.HitReact.Middle");
	UE_DEFINE_GAMEPLAY_TAG(Character_Ability_HitReact_Heavy, "Character.Ability.HitReact.Heavy");

	/** Character Event Hit (부모. Character.Event.Hit — 하위 전부 매칭) */
	UE_DEFINE_GAMEPLAY_TAG(Character_Event_Hit, "Character.Event.Hit");

	/** Character Event Land (착지 이벤트) */
	UE_DEFINE_GAMEPLAY_TAG(Character_Event_Land, "Character.Event.Land");

	/** Character Event Teleport */
	UE_DEFINE_GAMEPLAY_TAG(Character_Event_Teleport, "Character.Event.Teleport");
	UE_DEFINE_GAMEPLAY_TAG(Character_Event_Teleport_Back, "Character.Event.Teleport.Back");
	UE_DEFINE_GAMEPLAY_TAG(Character_Event_Teleport_Front, "Character.Event.Teleport.Front");
	UE_DEFINE_GAMEPLAY_TAG(Character_Event_Teleport_Right, "Character.Event.Teleport.Right");

	/** Character Event TeleportSuccess */
	UE_DEFINE_GAMEPLAY_TAG(Character_Event_TeleportSuccess, "Character.Event.TeleportSuccess");

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
	UE_DEFINE_GAMEPLAY_TAG(Character_Event_Death, "Character.Event.Death");
	UE_DEFINE_GAMEPLAY_TAG(Character_Status_Hit, "Character.Status.Hit");
	UE_DEFINE_GAMEPLAY_TAG(Character_Status_Dead, "Character.Status.Dead");

	/** Character SetByCaller tags */
	UE_DEFINE_GAMEPLAY_TAG(Character_SetByCaller_BaseDamage, "Character.SetByCaller.BaseDamage");
	UE_DEFINE_GAMEPLAY_TAG(Character_SetByCaller_UsedComboCount, "Character.SetByCaller.UsedComboCount");

	/** Cooldown tags */
	UE_DEFINE_GAMEPLAY_TAG(Cooldown_Dash, "Cooldown.Dash");

	/** GameplayCue.Sound (부모. A, E, R, ER → 각 하위 Gojo/Sukuna) */
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Sound, "GameplayCue.Sound");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Sound_A, "GameplayCue.Sound.A");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Sound_A_Gojo, "GameplayCue.Sound.A.Gojo");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Sound_A_Sukuna, "GameplayCue.Sound.A.Sukuna");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Sound_E, "GameplayCue.Sound.E");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Sound_E_Gojo, "GameplayCue.Sound.E.Gojo");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Sound_E_Sukuna, "GameplayCue.Sound.E.Sukuna");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Sound_R, "GameplayCue.Sound.R");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Sound_R_Gojo, "GameplayCue.Sound.R.Gojo");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Sound_R_Sukuna, "GameplayCue.Sound.R.Sukuna");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Sound_ER, "GameplayCue.Sound.ER");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Sound_ER_Gojo, "GameplayCue.Sound.ER.Gojo");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Sound_ER_Sukuna, "GameplayCue.Sound.ER.Sukuna");

	/** GameplayCue.Effect (부모. Light, Heavy, Special → 각 하위 Gojo/Sukuna) */
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Effect, "GameplayCue.Effect");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Effect_Light, "GameplayCue.Effect.Light");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Effect_Light_Gojo, "GameplayCue.Effect.Light.Gojo");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Effect_Light_Sukuna, "GameplayCue.Effect.Light.Sukuna");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Effect_Heavy, "GameplayCue.Effect.Heavy");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Effect_Heavy_Gojo, "GameplayCue.Effect.Heavy.Gojo");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Effect_Heavy_Sukuna, "GameplayCue.Effect.Heavy.Sukuna");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Effect_Special, "GameplayCue.Effect.Special");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Effect_Special_Gojo, "GameplayCue.Effect.Special.Gojo");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Effect_Special_Sukuna, "GameplayCue.Effect.Special.Sukuna");
}
