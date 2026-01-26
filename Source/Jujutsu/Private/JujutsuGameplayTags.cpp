// Copyright Epic Games, Inc. All Rights Reserved.



#include "JujutsuGameplayTags.h"

namespace JujutsuGameplayTags
{
	/** Input Tags */
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Move, "InputTag.Move");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_A, "InputTag.A");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_S, "InputTag.S");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_D, "InputTag.D");

	/** Character Ability Tags */
	UE_DEFINE_GAMEPLAY_TAG(Character_Ability_Combo_A, "Character.Ability.Combo.A");
	UE_DEFINE_GAMEPLAY_TAG(Character_Ability_Combo_S, "Character.Ability.Combo.S");
	UE_DEFINE_GAMEPLAY_TAG(Character_Ability_Combo_D, "Character.Ability.Combo.D");

	/** Character Ability HitReact (피격 타입) */
	UE_DEFINE_GAMEPLAY_TAG(Character_Ability_HitReact_Stun, "Character.Ability.HitReact.Stun");
	UE_DEFINE_GAMEPLAY_TAG(Character_Ability_HitReact_Light, "Character.Ability.HitReact.Light");
	UE_DEFINE_GAMEPLAY_TAG(Character_Ability_HitReact_Middle, "Character.Ability.HitReact.Middle");
	UE_DEFINE_GAMEPLAY_TAG(Character_Ability_HitReact_Heavy, "Character.Ability.HitReact.Heavy");

	/** Character Event Hit (부모. Character.Event.Hit — 하위 전부 매칭) */
	UE_DEFINE_GAMEPLAY_TAG(Character_Event_Hit, "Character.Event.Hit");

	/** Character Event Hit 하위 (피격 이벤트 타입) */
	UE_DEFINE_GAMEPLAY_TAG(Character_Event_Hit_Light, "Character.Event.Hit.Light");
	UE_DEFINE_GAMEPLAY_TAG(Character_Event_Hit_Stun, "Character.Event.Hit.Stun");
	UE_DEFINE_GAMEPLAY_TAG(Character_Event_Hit_Middle, "Character.Event.Hit.Middle");
	UE_DEFINE_GAMEPLAY_TAG(Character_Event_Hit_Heavy, "Character.Event.Hit.Heavy");

	/** Character SetByCaller tags */
	UE_DEFINE_GAMEPLAY_TAG(Character_SetByCaller_BaseDamage, "Character.SetByCaller.BaseDamage");
	UE_DEFINE_GAMEPLAY_TAG(Character_SetByCaller_UsedComboCount, "Character.SetByCaller.UsedComboCount");
}
