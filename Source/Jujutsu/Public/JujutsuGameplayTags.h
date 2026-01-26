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

	/** Character Ability Tags */
	JUJUTSU_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Ability_Combo_A);
	JUJUTSU_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Ability_Combo_S);
	JUJUTSU_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Ability_Combo_D);

	/** Character Event tags */
	JUJUTSU_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Event_MeleeHit);

	/** Character SetByCaller tags */
	JUJUTSU_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_SetByCaller_BaseDamage);
	JUJUTSU_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_SetByCaller_AttackType_Light);
	JUJUTSU_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_SetByCaller_AttackType_Heavy);
}
