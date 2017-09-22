
#include "PlayerCharmShamanAI.h"
#include "AutoAIoncharm.h"


PlayerCharmShamanAI::PlayerCharmShamanAI() : SpellSequence(0)
{
}

void PlayerCharmShamanAI::SpellShaman(Player* pPlayer)
{
	// restoration
    CHAIN_HEAL               = AIoncharm.initSpell(pPlayer, CHAIN_HEAL_1);
    HEALING_WAVE             = AIoncharm.initSpell(pPlayer, HEALING_WAVE_1);
    LESSER_HEALING_WAVE      = AIoncharm.initSpell(pPlayer, LESSER_HEALING_WAVE_1);
    RIPTIDE                  = AIoncharm.initSpell(pPlayer, RIPTIDE_1);
    ANCESTRAL_SPIRIT         = AIoncharm.initSpell(pPlayer, ANCESTRAL_SPIRIT_1);
    EARTH_SHIELD             = AIoncharm.initSpell(pPlayer, EARTH_SHIELD_1);
    WATER_SHIELD             = AIoncharm.initSpell(pPlayer, WATER_SHIELD_1);
    EARTHLIVING_WEAPON       = AIoncharm.initSpell(pPlayer, EARTHLIVING_WEAPON_1);
    TREMOR_TOTEM             = AIoncharm.initSpell(pPlayer, TREMOR_TOTEM_1); // totems
    HEALING_STREAM_TOTEM     = AIoncharm.initSpell(pPlayer, HEALING_STREAM_TOTEM_1);
    MANA_SPRING_TOTEM        = AIoncharm.initSpell(pPlayer, MANA_SPRING_TOTEM_1);
    MANA_TIDE_TOTEM          = AIoncharm.initSpell(pPlayer, MANA_TIDE_TOTEM_1);
    // enhancement
    FOCUSED                  = 0; // Focused what?
    STORMSTRIKE              = AIoncharm.initSpell(pPlayer, STORMSTRIKE_1);
    LAVA_LASH                = AIoncharm.initSpell(pPlayer, LAVA_LASH_1);
    SHAMANISTIC_RAGE         = AIoncharm.initSpell(pPlayer, SHAMANISTIC_RAGE_1);
    BLOODLUST                = AIoncharm.initSpell(pPlayer, BLOODLUST_1);
    HEROISM                  = AIoncharm.initSpell(pPlayer, HEROISM_1);
    FERAL_SPIRIT             = AIoncharm.initSpell(pPlayer, FERAL_SPIRIT_1);
    LIGHTNING_SHIELD         = AIoncharm.initSpell(pPlayer, LIGHTNING_SHIELD_1);
    ROCKBITER_WEAPON         = AIoncharm.initSpell(pPlayer, ROCKBITER_WEAPON_1);
    FLAMETONGUE_WEAPON       = AIoncharm.initSpell(pPlayer, FLAMETONGUE_WEAPON_1);
    FROSTBRAND_WEAPON        = AIoncharm.initSpell(pPlayer, FROSTBRAND_WEAPON_1);
    WINDFURY_WEAPON          = AIoncharm.initSpell(pPlayer, WINDFURY_WEAPON_1);
    STONESKIN_TOTEM          = AIoncharm.initSpell(pPlayer, STONESKIN_TOTEM_1); // totems
    STRENGTH_OF_EARTH_TOTEM  = AIoncharm.initSpell(pPlayer, STRENGTH_OF_EARTH_TOTEM_1);
    FROST_RESISTANCE_TOTEM   = AIoncharm.initSpell(pPlayer, FROST_RESISTANCE_TOTEM_1);
    FLAMETONGUE_TOTEM        = AIoncharm.initSpell(pPlayer, FLAMETONGUE_TOTEM_1);
    FIRE_RESISTANCE_TOTEM    = AIoncharm.initSpell(pPlayer, FIRE_RESISTANCE_TOTEM_1);
    GROUNDING_TOTEM          = AIoncharm.initSpell(pPlayer, GROUNDING_TOTEM_1);
    NATURE_RESISTANCE_TOTEM  = AIoncharm.initSpell(pPlayer, NATURE_RESISTANCE_TOTEM_1);
    WIND_FURY_TOTEM          = AIoncharm.initSpell(pPlayer, WINDFURY_TOTEM_1);
    STONESKIN_TOTEM          = AIoncharm.initSpell(pPlayer, STONESKIN_TOTEM_1);
    WRATH_OF_AIR_TOTEM       = AIoncharm.initSpell(pPlayer, WRATH_OF_AIR_TOTEM_1);
    EARTH_ELEMENTAL_TOTEM    = AIoncharm.initSpell(pPlayer, EARTH_ELEMENTAL_TOTEM_1);
    // elemental
    LIGHTNING_BOLT           = AIoncharm.initSpell(pPlayer, LIGHTNING_BOLT_1);
    EARTH_SHOCK              = AIoncharm.initSpell(pPlayer, EARTH_SHOCK_1);
    FLAME_SHOCK              = AIoncharm.initSpell(pPlayer, FLAME_SHOCK_1);
    PURGE                    = AIoncharm.initSpell(pPlayer, PURGE_1);
    WIND_SHOCK               = 0; //NPC spell
    FROST_SHOCK              = AIoncharm.initSpell(pPlayer, FROST_SHOCK_1);
    CHAIN_LIGHTNING          = AIoncharm.initSpell(pPlayer, CHAIN_LIGHTNING_1);
    LAVA_BURST               = AIoncharm.initSpell(pPlayer, LAVA_BURST_1);
    HEX                      = AIoncharm.initSpell(pPlayer, HEX_1);
    STONECLAW_TOTEM          = AIoncharm.initSpell(pPlayer, STONECLAW_TOTEM_1); // totems
    SEARING_TOTEM            = AIoncharm.initSpell(pPlayer, SEARING_TOTEM_1);
    FIRE_NOVA_TOTEM          = 0; // NPC only spell, check FIRE_NOVA_1
    MAGMA_TOTEM              = AIoncharm.initSpell(pPlayer, MAGMA_TOTEM_1);
    EARTHBIND_TOTEM          = AIoncharm.initSpell(pPlayer, EARTHBIND_TOTEM_1);
    TOTEM_OF_WRATH           = AIoncharm.initSpell(pPlayer, TOTEM_OF_WRATH_1);
    FIRE_ELEMENTAL_TOTEM     = AIoncharm.initSpell(pPlayer, FIRE_ELEMENTAL_TOTEM_1);

    RECENTLY_BANDAGED        = 11196; // first aid check
}

void PlayerCharmShamanAI::ShamanAI(Player* pPlayer)
{
	if (pPlayer->getVictim())
	{
		SpellShaman(pPlayer);
		Unit* pTarget = pPlayer->getVictim();
		float dist = pPlayer->GetCombatDistance(pTarget, true);
		switch (SpellSequence)
		{
		case SPELL_ENHANCEMENT:
			if (STRENGTH_OF_EARTH_TOTEM > 0 && LastSpellEnhancement == 1 && (!pPlayer->HasAura(STRENGTH_OF_EARTH_TOTEM, EFFECT_INDEX_0)) && AIoncharm.GetManaPercent(pPlayer) >= 13)
			{
				pPlayer->CastSpell(pPlayer, STRENGTH_OF_EARTH_TOTEM, false);
				SpellSequence = SPELL_RESTORATION;
				LastSpellEnhancement = LastSpellEnhancement + 1;
				break;
			}
			else if (STONESKIN_TOTEM > 0 && LastSpellEnhancement == 5 && (!pPlayer->HasAura(STONESKIN_TOTEM, EFFECT_INDEX_0)) && (!pPlayer->HasAura(STRENGTH_OF_EARTH_TOTEM, EFFECT_INDEX_0)) && AIoncharm.GetManaPercent(pPlayer) >= 13)
			{
				pPlayer->CastSpell(pPlayer, STONESKIN_TOTEM, false);
				SpellSequence = SPELL_RESTORATION;
				LastSpellEnhancement = LastSpellEnhancement + 1;
				break;
			}
			else if (FOCUSED > 0 && LastSpellEnhancement == 2)
			{
				pPlayer->CastSpell(pTarget, FOCUSED, false);
				SpellSequence = SPELL_RESTORATION;
				LastSpellEnhancement = LastSpellEnhancement + 1;
				break;
			}
			else if (FROST_RESISTANCE_TOTEM > 0 && LastSpellEnhancement == 10 && (!pPlayer->HasAura(FROST_RESISTANCE_TOTEM, EFFECT_INDEX_0)) && (!pPlayer->HasAura(TOTEM_OF_WRATH, EFFECT_INDEX_0)) && (!pPlayer->HasAura(FLAMETONGUE_TOTEM, EFFECT_INDEX_0)) && AIoncharm.GetManaPercent(pPlayer) >= 10)
			{
				pPlayer->CastSpell(pPlayer, FROST_RESISTANCE_TOTEM, false);
				SpellSequence = SPELL_RESTORATION;
				LastSpellEnhancement = LastSpellEnhancement + 1;
				break;
			}
			else if (FLAMETONGUE_TOTEM > 0 && LastSpellEnhancement == 15 && (!pPlayer->HasAura(FLAMETONGUE_TOTEM, EFFECT_INDEX_0)) && (!pPlayer->HasAura(TOTEM_OF_WRATH, EFFECT_INDEX_0)) && (!pPlayer->HasAura(FROST_RESISTANCE_TOTEM, EFFECT_INDEX_0)) && AIoncharm.GetManaPercent(pPlayer) >= 14)
			{
				pPlayer->CastSpell(pPlayer, FLAMETONGUE_TOTEM, false);
				SpellSequence = SPELL_RESTORATION;
				LastSpellEnhancement = LastSpellEnhancement + 1;
				break;
			}
			else if (FIRE_RESISTANCE_TOTEM > 0 && LastSpellEnhancement == 20 && (!pPlayer->HasAura(FIRE_RESISTANCE_TOTEM, EFFECT_INDEX_0)) && (!pPlayer->HasAura(HEALING_STREAM_TOTEM, EFFECT_INDEX_0)) && (!pPlayer->HasAura(MANA_SPRING_TOTEM, EFFECT_INDEX_0)) && AIoncharm.GetManaPercent(pPlayer) >= 10)
			{
				pPlayer->CastSpell(pPlayer, FIRE_RESISTANCE_TOTEM, false);
				SpellSequence = SPELL_RESTORATION;
				LastSpellEnhancement = LastSpellEnhancement + 1;
				break;
			}
			else if (GROUNDING_TOTEM > 0 && LastSpellEnhancement == 25 && (!pPlayer->HasAura(GROUNDING_TOTEM, EFFECT_INDEX_0)) && (!pPlayer->HasAura(WRATH_OF_AIR_TOTEM, EFFECT_INDEX_0)) && (!pPlayer->HasAura(WIND_FURY_TOTEM, EFFECT_INDEX_0)) && AIoncharm.GetManaPercent(pPlayer) >= 5)
			{
				pPlayer->CastSpell(pPlayer, GROUNDING_TOTEM, false);
				SpellSequence = SPELL_RESTORATION;
				LastSpellEnhancement = LastSpellEnhancement + 1;
				break;
			}
			else if (NATURE_RESISTANCE_TOTEM > 0 && LastSpellEnhancement == 30 && (!pPlayer->HasAura(NATURE_RESISTANCE_TOTEM, EFFECT_INDEX_0)) && (!pPlayer->HasAura(WRATH_OF_AIR_TOTEM, EFFECT_INDEX_0)) && (!pPlayer->HasAura(GROUNDING_TOTEM, EFFECT_INDEX_0)) && (!pPlayer->HasAura(WIND_FURY_TOTEM, EFFECT_INDEX_0)) && AIoncharm.GetManaPercent(pPlayer) >= 10)
			{
				pPlayer->CastSpell(pPlayer, NATURE_RESISTANCE_TOTEM, false);
				SpellSequence = SPELL_RESTORATION;
				LastSpellEnhancement = LastSpellEnhancement + 1;
				break;
			}
			else if (WIND_FURY_TOTEM > 0 && LastSpellEnhancement == 35 && (!pPlayer->HasAura(WIND_FURY_TOTEM, EFFECT_INDEX_0)) && (!pPlayer->HasAura(WRATH_OF_AIR_TOTEM, EFFECT_INDEX_0)) && (!pPlayer->HasAura(GROUNDING_TOTEM, EFFECT_INDEX_0)) && AIoncharm.GetManaPercent(pPlayer) >= 11)
			{
				pPlayer->CastSpell(pPlayer, WIND_FURY_TOTEM, false);
				SpellSequence = SPELL_RESTORATION;
				LastSpellEnhancement = LastSpellEnhancement + 1;
				break;
			}
			else if (STORMSTRIKE > 0 && LastSpellEnhancement == 4 && (!pTarget->HasAura(STORMSTRIKE, EFFECT_INDEX_0)) && AIoncharm.GetManaPercent(pPlayer) >= 8)
			{
				pPlayer->CastSpell(pTarget, STORMSTRIKE, false);
				SpellSequence = SPELL_RESTORATION;
				LastSpellEnhancement = LastSpellEnhancement + 1;
				break;
			}
			else if (LAVA_LASH > 0 && LastSpellEnhancement == 6 && AIoncharm.GetManaPercent(pPlayer) >= 4)
			{
				pPlayer->CastSpell(pTarget, LAVA_LASH, false);
				SpellSequence = SPELL_RESTORATION;
				LastSpellEnhancement = LastSpellEnhancement + 1;
				break;
			}
			else if (FERAL_SPIRIT > 0 && LastSpellEnhancement == 7 && AIoncharm.GetManaPercent(pPlayer) >= 12)
			{
				pPlayer->CastSpell(pPlayer, FERAL_SPIRIT, false);
				SpellSequence = SPELL_RESTORATION;
				LastSpellEnhancement = LastSpellEnhancement + 1;
				break;
			}
			else if (WRATH_OF_AIR_TOTEM > 0 && (!pPlayer->HasAura(WRATH_OF_AIR_TOTEM, EFFECT_INDEX_0)) && (!pPlayer->HasAura(GROUNDING_TOTEM, EFFECT_INDEX_0)) && LastSpellEnhancement == 40)
			{
				pPlayer->CastSpell(pPlayer, WRATH_OF_AIR_TOTEM, false);
				SpellSequence = SPELL_RESTORATION;
				LastSpellEnhancement = LastSpellEnhancement + 1;
				break;
			}
			else if (EARTH_ELEMENTAL_TOTEM > 0 && LastSpellEnhancement == 45 && AIoncharm.GetManaPercent(pPlayer) >= 24)
			{
				pPlayer->CastSpell(pPlayer, EARTH_ELEMENTAL_TOTEM, false);
				SpellSequence = SPELL_RESTORATION;
				LastSpellEnhancement = LastSpellEnhancement + 1;
				break;
			}
			else if (BLOODLUST > 0 && LastSpellEnhancement == 8 && (!pPlayer->HasAura(BLOODLUST, EFFECT_INDEX_0)) && AIoncharm.GetManaPercent(pPlayer) >= 26)
			{
				pPlayer->CastSpell(pPlayer, BLOODLUST, false);
				SpellSequence = SPELL_RESTORATION;
				LastSpellEnhancement = LastSpellEnhancement + 1;
				break;
			}
			else if (HEROISM > 0 && LastSpellEnhancement == 10 && (!pPlayer->HasAura(HEROISM, EFFECT_INDEX_0)) && AIoncharm.GetManaPercent(pPlayer) >= 26)
			{
				pPlayer->CastSpell(pPlayer, HEROISM, false);
				SpellSequence = SPELL_RESTORATION;
				LastSpellEnhancement = LastSpellEnhancement + 1;
				break;
			}
			else if (SHAMANISTIC_RAGE > 0 && (!pPlayer->HasAura(SHAMANISTIC_RAGE, EFFECT_INDEX_0)) && LastSpellEnhancement == 11)
			{
				pPlayer->CastSpell(pPlayer, SHAMANISTIC_RAGE, false);
				SpellSequence = SPELL_RESTORATION;
				LastSpellEnhancement = LastSpellEnhancement + 1;
				break;
			}
			else if (LastSpellEnhancement > 50)
			{
				LastSpellEnhancement = 1;
				SpellSequence = SPELL_RESTORATION;
				break;
			}
			LastSpellEnhancement = LastSpellEnhancement + 1;
			//SpellSequence = SPELL_RESTORATION;
			//break;

		case SPELL_RESTORATION:
			if (HEALING_STREAM_TOTEM > 0 && LastSpellRestoration < 3 && pPlayer->GetHealthPercent() < 50 && (!pPlayer->HasAura(HEALING_STREAM_TOTEM, EFFECT_INDEX_0)) && (!pPlayer->HasAura(MANA_SPRING_TOTEM, EFFECT_INDEX_0)) && AIoncharm.GetManaPercent(pPlayer) >= 4)
			{
				pPlayer->CastSpell(pPlayer, HEALING_STREAM_TOTEM, false);
				SpellSequence = SPELL_ELEMENTAL;
				LastSpellRestoration = LastSpellRestoration + 1;
				break;
			}
			else if (MANA_SPRING_TOTEM > 0 && LastSpellRestoration < 4 && (!pPlayer->HasAura(MANA_SPRING_TOTEM, EFFECT_INDEX_0)) && (!pPlayer->HasAura(HEALING_STREAM_TOTEM, EFFECT_INDEX_0)) && AIoncharm.GetManaPercent(pPlayer) >= 5)
			{
				pPlayer->CastSpell(pPlayer, MANA_SPRING_TOTEM, false);
				SpellSequence = SPELL_ELEMENTAL;
				LastSpellRestoration = LastSpellRestoration + 1;
				break;
			}
			else if (MANA_TIDE_TOTEM > 0 && LastSpellRestoration < 5 && AIoncharm.GetManaPercent(pPlayer) < 50 && AIoncharm.GetManaPercent(pPlayer) >= 3)
			{
				pPlayer->CastSpell(pPlayer, MANA_TIDE_TOTEM, false);
				SpellSequence = SPELL_ELEMENTAL;
				LastSpellRestoration = LastSpellRestoration + 1;
				break;
			}
			/*else if (TREMOR_TOTEM > 0 && LastSpellRestoration < 6 && (!pPlayer->HasAura(STRENGTH_OF_EARTH_TOTEM, EFFECT_INDEX_0)) && AIoncharm.GetManaPercent(pPlayer) >= 2)
			{
			pPlayer->CastSpell(TREMOR_TOTEM);
			SpellSequence = SPELL_ELEMENTAL;
			LastSpellRestoration = LastSpellRestoration +1;
			break;
			}*/
			else if (LastSpellRestoration > 6)
			{
				LastSpellRestoration = 0;
				SpellSequence = SPELL_ELEMENTAL;
				break;
			}
			LastSpellRestoration = LastSpellRestoration + 1;
			//SpellSequence = SPELL_ELEMENTAL;
			//break;

		case SPELL_ELEMENTAL:
			if (LIGHTNING_BOLT > 0 && LastSpellElemental == 1 && AIoncharm.GetManaPercent(pPlayer) >= 13)
			{
				pPlayer->CastSpell(pTarget, LIGHTNING_BOLT, false);
				SpellSequence = SPELL_ENHANCEMENT;
				LastSpellElemental = LastSpellElemental + 1;
				break;
			}
			else if (SEARING_TOTEM > 0 && LastSpellElemental == 2 && (!pTarget->HasAura(SEARING_TOTEM, EFFECT_INDEX_0)) && (!pPlayer->HasAura(TOTEM_OF_WRATH, EFFECT_INDEX_0)) && AIoncharm.GetManaPercent(pPlayer) >= 9)
			{
				pPlayer->CastSpell(pPlayer, SEARING_TOTEM, false);
				SpellSequence = SPELL_ENHANCEMENT;
				LastSpellElemental = LastSpellElemental + 1;
				break;
			}
			else if (STONECLAW_TOTEM > 0 && pPlayer->GetHealthPercent() < 51 && LastSpellElemental == 3 && (!pTarget->HasAura(STONECLAW_TOTEM, EFFECT_INDEX_0)) && (!pTarget->HasAura(EARTHBIND_TOTEM, EFFECT_INDEX_0)) && (!pPlayer->HasAura(STRENGTH_OF_EARTH_TOTEM, EFFECT_INDEX_0)) && AIoncharm.GetManaPercent(pPlayer) >= 8)
			{
				pPlayer->CastSpell(pPlayer, STONECLAW_TOTEM, false);
				SpellSequence = SPELL_ENHANCEMENT;
				LastSpellElemental = LastSpellElemental + 1;
				break;
			}
			else if (FLAME_SHOCK > 0 && LastSpellElemental == 4 && (!pTarget->HasAura(FLAME_SHOCK, EFFECT_INDEX_0)) && AIoncharm.GetManaPercent(pPlayer) >= 22)
			{
				pPlayer->CastSpell(pTarget, FLAME_SHOCK, false);
				SpellSequence = SPELL_ENHANCEMENT;
				LastSpellElemental = LastSpellElemental + 1;
				break;
			}
			else if (LAVA_BURST > 0 && LastSpellElemental == 5 && (pTarget->HasAura(FLAME_SHOCK, EFFECT_INDEX_0)) && AIoncharm.GetManaPercent(pPlayer) >= 10)
			{
				pPlayer->CastSpell(pTarget, LAVA_BURST, false);
				SpellSequence = SPELL_ENHANCEMENT;
				LastSpellElemental = LastSpellElemental + 1;
				break;
			}
			else if (MAGMA_TOTEM > 0 && LastSpellElemental == 6 && (!pPlayer->HasAura(TOTEM_OF_WRATH, EFFECT_INDEX_0)) && AIoncharm.GetManaPercent(pPlayer) >= 35)
			{
				pPlayer->CastSpell(pPlayer, MAGMA_TOTEM, false);
				SpellSequence = SPELL_ENHANCEMENT;
				LastSpellElemental = LastSpellElemental + 1;
				break;
			}
			else if (EARTHBIND_TOTEM > 0 && LastSpellElemental == 7 && (!pTarget->HasAura(EARTHBIND_TOTEM, EFFECT_INDEX_0)) && (!pPlayer->HasAura(STRENGTH_OF_EARTH_TOTEM, EFFECT_INDEX_0)) && AIoncharm.GetManaPercent(pPlayer) >= 5)
			{
				pPlayer->CastSpell(pPlayer, EARTHBIND_TOTEM, false);
				SpellSequence = SPELL_ENHANCEMENT;
				LastSpellElemental = LastSpellElemental + 1;
				break;
			}
			else if (EARTH_SHOCK > 0 && LastSpellElemental == 8 && AIoncharm.GetManaPercent(pPlayer) >= 23)
			{
				pPlayer->CastSpell(pTarget, EARTH_SHOCK, false);
				SpellSequence = SPELL_ENHANCEMENT;
				LastSpellElemental = LastSpellElemental + 1;
				break;
			}
			else if (PURGE > 0 && LastSpellElemental == 9 && AIoncharm.GetManaPercent(pPlayer) >= 8)
			{
				pPlayer->CastSpell(pTarget, PURGE, false);
				SpellSequence = SPELL_ENHANCEMENT;
				LastSpellElemental = LastSpellElemental + 1;
				break;
			}
			else if (WIND_SHOCK > 0 && LastSpellElemental == 10 && AIoncharm.GetManaPercent(pPlayer) >= 8)
			{
				pPlayer->CastSpell(pTarget, WIND_SHOCK, false);
				SpellSequence = SPELL_ENHANCEMENT;
				LastSpellElemental = LastSpellElemental + 1;
				break;
			}
			else if (FIRE_NOVA_TOTEM > 0 && LastSpellElemental == 11 && AIoncharm.GetManaPercent(pPlayer) >= 33)
			{
				pPlayer->CastSpell(pPlayer, FIRE_NOVA_TOTEM, false);
				SpellSequence = SPELL_ENHANCEMENT;
				LastSpellElemental = LastSpellElemental + 1;
				break;
			}
			else if (FROST_SHOCK > 0 && LastSpellElemental == 12 && (!pTarget->HasAura(FROST_SHOCK, EFFECT_INDEX_0)) && AIoncharm.GetManaPercent(pPlayer) >= 23)
			{
				pPlayer->CastSpell(pTarget, FROST_SHOCK, false);
				SpellSequence = SPELL_ENHANCEMENT;
				LastSpellElemental = LastSpellElemental + 1;
				break;
			}
			else if (CHAIN_LIGHTNING > 0 && LastSpellElemental == 13 && AIoncharm.GetManaPercent(pPlayer) >= 33)
			{
				pPlayer->CastSpell(pTarget, CHAIN_LIGHTNING, false);
				SpellSequence = SPELL_ENHANCEMENT;
				LastSpellElemental = LastSpellElemental + 1;
				break;
			}
			else if (TOTEM_OF_WRATH > 0 && LastSpellElemental == 14 && (!pPlayer->HasAura(TOTEM_OF_WRATH, EFFECT_INDEX_0)) && AIoncharm.GetManaPercent(pPlayer) >= 5)
			{
				pPlayer->CastSpell(pPlayer, TOTEM_OF_WRATH, false);
				SpellSequence = SPELL_ENHANCEMENT;
				LastSpellElemental = LastSpellElemental + 1;
				break;
			}
			else if (FIRE_ELEMENTAL_TOTEM > 0 && LastSpellElemental == 15 && AIoncharm.GetManaPercent(pPlayer) >= 23)
			{
				pPlayer->CastSpell(pPlayer, FIRE_ELEMENTAL_TOTEM, false);
				SpellSequence = SPELL_ENHANCEMENT;
				LastSpellElemental = LastSpellElemental + 1;
				break;
			}
			/*else if (HEX > 0 && LastSpellElemental == 16 && (!pTarget->HasAura(HEX, EFFECT_INDEX_0)) && AIoncharm.GetManaPercent(pPlayer) >= 3)
			{
			pPlayer->CastSpell(HEX);
			SpellSequence = SPELL_ENHANCEMENT;
			LastSpellElemental = LastSpellElemental + 1;
			break;
			}*/
			else if (LastSpellElemental > 16)
			{
				LastSpellElemental = 1;
				SpellSequence = SPELL_ENHANCEMENT;
				break;
			}
			else
			{
				LastSpellElemental = LastSpellElemental + 1;
				SpellSequence = SPELL_ENHANCEMENT;
			}
		}
	}
}