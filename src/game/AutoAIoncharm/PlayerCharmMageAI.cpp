
#include "PlayerCharmMageAI.h"
#include "AutoAIoncharm.h"


PlayerCharmMageAI::PlayerCharmMageAI() : SpellSequence(0), LastSpellArcane(0), LastSpellFire(0), LastSpellFrost(0)
{
}

void PlayerCharmMageAI::SpellMage(Player* pPlayer)
{
	ARCANE_MISSILES         = AIoncharm.initSpell(pPlayer, ARCANE_MISSILES_1);
    ARCANE_EXPLOSION        = AIoncharm.initSpell(pPlayer, ARCANE_EXPLOSION_1);
    COUNTERSPELL            = AIoncharm.initSpell(pPlayer, COUNTERSPELL_1);
    SLOW                    = AIoncharm.initSpell(pPlayer, SLOW_1);
    ARCANE_BARRAGE          = AIoncharm.initSpell(pPlayer, ARCANE_BARRAGE_1);
    ARCANE_BLAST            = AIoncharm.initSpell(pPlayer, ARCANE_BLAST_1);
    ARCANE_POWER            = AIoncharm.initSpell(pPlayer, ARCANE_POWER_1);
    DAMPEN_MAGIC            = AIoncharm.initSpell(pPlayer, DAMPEN_MAGIC_1);
    AMPLIFY_MAGIC           = AIoncharm.initSpell(pPlayer, AMPLIFY_MAGIC_1);
    MAGE_ARMOR              = AIoncharm.initSpell(pPlayer, MAGE_ARMOR_1);
    MIRROR_IMAGE            = AIoncharm.initSpell(pPlayer, MIRROR_IMAGE_1);
    ARCANE_INTELLECT        = AIoncharm.initSpell(pPlayer, ARCANE_INTELLECT_1);
    ARCANE_BRILLIANCE       = AIoncharm.initSpell(pPlayer, ARCANE_BRILLIANCE_1);
    DALARAN_INTELLECT       = AIoncharm.initSpell(pPlayer, DALARAN_INTELLECT_1);
    DALARAN_BRILLIANCE      = AIoncharm.initSpell(pPlayer, DALARAN_BRILLIANCE_1);
    MANA_SHIELD             = AIoncharm.initSpell(pPlayer, MANA_SHIELD_1);
    CONJURE_WATER           = AIoncharm.initSpell(pPlayer, CONJURE_WATER_1);
    CONJURE_FOOD            = AIoncharm.initSpell(pPlayer, CONJURE_FOOD_1);
    FIREBALL                = AIoncharm.initSpell(pPlayer, FIREBALL_1);
    FIRE_BLAST              = AIoncharm.initSpell(pPlayer, FIRE_BLAST_1);
    FLAMESTRIKE             = AIoncharm.initSpell(pPlayer, FLAMESTRIKE_1);
    SCORCH                  = AIoncharm.initSpell(pPlayer, SCORCH_1);
    PYROBLAST               = AIoncharm.initSpell(pPlayer, PYROBLAST_1);
    BLAST_WAVE              = AIoncharm.initSpell(pPlayer, BLAST_WAVE_1);
    COMBUSTION              = AIoncharm.initSpell(pPlayer, COMBUSTION_1);
    DRAGONS_BREATH          = AIoncharm.initSpell(pPlayer, DRAGONS_BREATH_1);
    LIVING_BOMB             = AIoncharm.initSpell(pPlayer, LIVING_BOMB_1);
    FROSTFIRE_BOLT          = AIoncharm.initSpell(pPlayer, FROSTFIRE_BOLT_1);
    FIRE_WARD               = AIoncharm.initSpell(pPlayer, FIRE_WARD_1);
    MOLTEN_ARMOR            = AIoncharm.initSpell(pPlayer, MOLTEN_ARMOR_1);
    ICY_VEINS               = AIoncharm.initSpell(pPlayer, ICY_VEINS_1);
    DEEP_FREEZE             = AIoncharm.initSpell(pPlayer, DEEP_FREEZE_1);
    FROSTBOLT               = AIoncharm.initSpell(pPlayer, FROSTBOLT_1);
    FROST_NOVA              = AIoncharm.initSpell(pPlayer, FROST_NOVA_1);
    BLIZZARD                = AIoncharm.initSpell(pPlayer, BLIZZARD_1);
    CONE_OF_COLD            = AIoncharm.initSpell(pPlayer, CONE_OF_COLD_1);
    ICE_BARRIER             = AIoncharm.initSpell(pPlayer, ICE_BARRIER_1);
    SUMMON_WATER_ELEMENTAL  = AIoncharm.initSpell(pPlayer, SUMMON_WATER_ELEMENTAL_1);
    FROST_WARD              = AIoncharm.initSpell(pPlayer, FROST_WARD_1);
    ICE_LANCE               = AIoncharm.initSpell(pPlayer, ICE_LANCE_1);
    FROST_ARMOR             = AIoncharm.initSpell(pPlayer, FROST_ARMOR_1);
    ICE_ARMOR               = AIoncharm.initSpell(pPlayer, ICE_ARMOR_1);
    ICE_BLOCK               = AIoncharm.initSpell(pPlayer, ICE_BLOCK_1);
    COLD_SNAP               = AIoncharm.initSpell(pPlayer, COLD_SNAP_1);
}

void PlayerCharmMageAI::MageAI(Player* pPlayer)
{
	if (pPlayer->getVictim())
	{
		SpellMage(pPlayer);
		Unit* pTarget = pPlayer->getVictim();
		float dist = pPlayer->GetCombatDistance(pTarget, true);
		switch (SpellSequence)
		{
		case SPELL_FROST:
			if (ICY_VEINS > 0 && !pPlayer->HasAura(ICY_VEINS, EFFECT_INDEX_0) && LastSpellFrost < 1 && AIoncharm.GetManaPercent(pPlayer) >= 3)
			{
				pPlayer->CastSpell(pTarget, ICY_VEINS, false);
				SpellSequence = SPELL_FIRE;
				LastSpellFrost = LastSpellFrost + 1;
				break;
			}
			else if (FROSTBOLT > 0 && LastSpellFrost < 2 && !pTarget->HasAura(FROSTBOLT, EFFECT_INDEX_0) && AIoncharm.GetManaPercent(pPlayer) >= 16)
			{
				pPlayer->CastSpell(pTarget, FROSTBOLT, false);
				SpellSequence = SPELL_FIRE;
				LastSpellFrost = LastSpellFrost + 1;
				break;
			}
			else if (FROST_WARD > 0 && LastSpellFrost < 3 && !pPlayer->HasAura(FROST_WARD, EFFECT_INDEX_0) && AIoncharm.GetManaPercent(pPlayer) >= 19)
			{
				pPlayer->CastSpell(pPlayer, FROST_WARD, false);
				SpellSequence = SPELL_FIRE;
				LastSpellFrost = LastSpellFrost + 1;
				break;
			}
			else if (FROST_NOVA > 0 && LastSpellFrost < 4 && dist <= ATTACK_DISTANCE && !pTarget->HasAura(FROST_NOVA, EFFECT_INDEX_0) && AIoncharm.GetManaPercent(pPlayer) >= 10)
			{
				pPlayer->CastSpell(pTarget, FROST_NOVA, false);
				SpellSequence = SPELL_FIRE;
				LastSpellFrost = LastSpellFrost + 1;
				break;
			}
			else if (ICE_LANCE > 0 && LastSpellFrost < 5 && AIoncharm.GetManaPercent(pPlayer) >= 7)
			{
				pPlayer->CastSpell(pTarget, ICE_LANCE, false);
				SpellSequence = SPELL_FIRE;
				LastSpellFrost = LastSpellFrost + 1;
				break;
			}
			else if (BLIZZARD > 0 && LastSpellFrost < 6 && AIoncharm.GetManaPercent(pPlayer) >= 89)
			{
				pPlayer->CastSpell(pTarget, BLIZZARD, false);
				SpellSequence = SPELL_FIRE;
				LastSpellFrost = LastSpellFrost + 1;
				break;
			}
			else if (CONE_OF_COLD > 0 && LastSpellFrost < 7 && dist <= ATTACK_DISTANCE && !pTarget->HasAura(CONE_OF_COLD, EFFECT_INDEX_0) && AIoncharm.GetManaPercent(pPlayer) >= 35)
			{
				pPlayer->CastSpell(pTarget, CONE_OF_COLD, false);
				SpellSequence = SPELL_FIRE;
				LastSpellFrost = LastSpellFrost + 1;
				break;
			}
			else if (DEEP_FREEZE > 0 && LastSpellFrost < 8 && pTarget->HasAura(AURA_STATE_FROZEN, EFFECT_INDEX_0) && !pTarget->HasAura(DEEP_FREEZE, EFFECT_INDEX_0) && AIoncharm.GetManaPercent(pPlayer) >= 9)
			{
				pPlayer->CastSpell(pTarget, DEEP_FREEZE, false);
				SpellSequence = SPELL_FIRE;
				LastSpellFrost = LastSpellFrost + 1;
				break;
			}
			else if (ICE_BARRIER > 0 && LastSpellFrost < 9 && pTarget == pPlayer && !pPlayer->HasAura(ICE_BARRIER, EFFECT_INDEX_0) && pPlayer->GetHealthPercent() < 50 && AIoncharm.GetManaPercent(pPlayer) >= 30)
			{
				pPlayer->CastSpell(pPlayer, ICE_BARRIER, false);
				SpellSequence = SPELL_FIRE;
				LastSpellFrost = LastSpellFrost + 1;
				break;
			}
			else if (SUMMON_WATER_ELEMENTAL > 0 && LastSpellFrost < 10 && AIoncharm.GetManaPercent(pPlayer) >= 16)
			{
				pPlayer->CastSpell(pTarget, SUMMON_WATER_ELEMENTAL, false);
				SpellSequence = SPELL_FIRE;
				LastSpellFrost = LastSpellFrost + 1;
				break;
			}
			else if (ICE_BLOCK > 0 && LastSpellFrost < 11 && pTarget == pPlayer && !pPlayer->HasAura(ICE_BLOCK, EFFECT_INDEX_0) && pPlayer->GetHealthPercent() < 30)
			{
				pPlayer->CastSpell(pPlayer, ICE_BLOCK, false);
				SpellSequence = SPELL_FIRE;
				LastSpellFrost = LastSpellFrost + 1;
				break;
			}
			else if (COLD_SNAP > 0 && LastSpellFrost < 12)
			{
				pPlayer->CastSpell(pPlayer, COLD_SNAP, false);
				SpellSequence = SPELL_FIRE;
				LastSpellFrost = LastSpellFrost + 1;
				break;
			}
			LastSpellFrost = 0;
			//SpellSequence = SPELL_FIRE;
			//break;

		case SPELL_FIRE:
			if (FIRE_WARD > 0 && !pPlayer->HasAura(FIRE_WARD, EFFECT_INDEX_0) && LastSpellFire < 1 && AIoncharm.GetManaPercent(pPlayer) >= 3)
			{
				pPlayer->CastSpell(pPlayer, FIRE_WARD, false);
				SpellSequence = SPELL_ARCANE;
				LastSpellFire = LastSpellFire + 1;
				break;
			}
			else if (COMBUSTION > 0 && !pPlayer->HasAura(COMBUSTION, EFFECT_INDEX_0) && LastSpellFire < 2)
			{
				pPlayer->CastSpell(pPlayer, COMBUSTION, false);
				SpellSequence = SPELL_ARCANE;
				LastSpellFire = LastSpellFire + 1;
				break;
			}
			else if (FIREBALL > 0 && LastSpellFire < 3 && AIoncharm.GetManaPercent(pPlayer) >= 23)
			{
				pPlayer->CastSpell(pTarget, FIREBALL, false);
				SpellSequence = SPELL_ARCANE;
				LastSpellFire = LastSpellFire + 1;
				break;
			}
			else if (FIRE_BLAST > 0 && LastSpellFire < 4 && AIoncharm.GetManaPercent(pPlayer) >= 25)
			{
				pPlayer->CastSpell(pTarget, FIRE_BLAST, false);
				SpellSequence = SPELL_ARCANE;
				LastSpellFire = LastSpellFire + 1;
				break;
			}
			else if (FLAMESTRIKE > 0 && LastSpellFire < 5 && AIoncharm.GetManaPercent(pPlayer) >= 35)
			{
				pPlayer->CastSpell(pTarget, FLAMESTRIKE, false);
				SpellSequence = SPELL_ARCANE;
				LastSpellFire = LastSpellFire + 1;
				break;
			}
			else if (SCORCH > 0 && LastSpellFire < 6 && AIoncharm.GetManaPercent(pPlayer) >= 10)
			{
				pPlayer->CastSpell(pTarget, SCORCH, false);
				SpellSequence = SPELL_ARCANE;
				LastSpellFire = LastSpellFire + 1;
				break;
			}
			else if (PYROBLAST > 0 && LastSpellFire < 7 && !pTarget->HasAura(PYROBLAST, EFFECT_INDEX_0) && AIoncharm.GetManaPercent(pPlayer) >= 27)
			{
				pPlayer->CastSpell(pTarget, PYROBLAST, false);
				SpellSequence = SPELL_ARCANE;
				LastSpellFire = LastSpellFire + 1;
				break;
			}
			else if (BLAST_WAVE > 0 && LastSpellFire < 8 && dist <= ATTACK_DISTANCE && AIoncharm.GetManaPercent(pPlayer) >= 34)
			{
				pPlayer->CastSpell(pTarget, BLAST_WAVE, false);
				SpellSequence = SPELL_ARCANE;
				LastSpellFire = LastSpellFire + 1;
				break;
			}
			else if (DRAGONS_BREATH > 0 && LastSpellFire < 9 && dist <= ATTACK_DISTANCE && AIoncharm.GetManaPercent(pPlayer) >= 37)
			{
				pPlayer->CastSpell(pTarget, DRAGONS_BREATH, false);
				SpellSequence = SPELL_ARCANE;
				LastSpellFire = LastSpellFire + 1;
				break;
			}
			else if (LIVING_BOMB > 0 && LastSpellFire < 10 && !pTarget->HasAura(LIVING_BOMB, EFFECT_INDEX_0) && AIoncharm.GetManaPercent(pPlayer) >= 27)
			{
				pPlayer->CastSpell(pTarget, LIVING_BOMB, false);
				SpellSequence = SPELL_ARCANE;
				LastSpellFire = LastSpellFire + 1;
				break;
			}
			else if (FROSTFIRE_BOLT > 0 && LastSpellFire < 11 && !pTarget->HasAura(FROSTFIRE_BOLT, EFFECT_INDEX_0) && AIoncharm.GetManaPercent(pPlayer) >= 14)
			{
				pPlayer->CastSpell(pTarget, FROSTFIRE_BOLT, false);
				SpellSequence = SPELL_ARCANE;
				LastSpellFire = LastSpellFire + 1;
				break;
			}
			LastSpellFire = 0;
			//SpellSequence = SPELL_ARCANE;
			//break;

		case SPELL_ARCANE:
			if (ARCANE_POWER > 0 && LastSpellArcane < 1 && AIoncharm.GetManaPercent(pPlayer) >= 37)
			{
				pPlayer->CastSpell(pTarget, ARCANE_POWER, false);
				SpellSequence = SPELL_FROST;
				LastSpellArcane = LastSpellArcane + 1;
				break;
			}
			else if (ARCANE_MISSILES > 0 && LastSpellArcane < 2 && AIoncharm.GetManaPercent(pPlayer) >= 37)
			{
				pPlayer->CastSpell(pTarget, ARCANE_MISSILES, false);
				SpellSequence = SPELL_FROST;
				LastSpellArcane = LastSpellArcane + 1;
				break;
			}
			else if (ARCANE_EXPLOSION > 0 && LastSpellArcane < 3 && dist <= ATTACK_DISTANCE && AIoncharm.GetManaPercent(pPlayer) >= 27)
			{
				pPlayer->CastSpell(pTarget, ARCANE_EXPLOSION, false);
				SpellSequence = SPELL_FROST;
				LastSpellArcane = LastSpellArcane + 1;
				break;
			}
			else if (COUNTERSPELL > 0 && pTarget->IsNonMeleeSpellCasted(true) && LastSpellArcane < 4 && AIoncharm.GetManaPercent(pPlayer) >= 9)
			{
				pPlayer->CastSpell(pTarget, COUNTERSPELL, false);
				SpellSequence = SPELL_FROST;
				LastSpellArcane = LastSpellArcane + 1;
				break;
			}
			else if (SLOW > 0 && LastSpellArcane < 5 && !pTarget->HasAura(SLOW, EFFECT_INDEX_0) && AIoncharm.GetManaPercent(pPlayer) >= 12)
			{
				pPlayer->CastSpell(pTarget, SLOW, false);
				SpellSequence = SPELL_FROST;
				LastSpellArcane = LastSpellArcane + 1;
				break;
			}
			else if (ARCANE_BARRAGE > 0 && LastSpellArcane < 6 && AIoncharm.GetManaPercent(pPlayer) >= 27)
			{
				pPlayer->CastSpell(pTarget, ARCANE_BARRAGE, false);
				SpellSequence = SPELL_FROST;
				LastSpellArcane = LastSpellArcane + 1;
				break;
			}
			else if (ARCANE_BLAST > 0 && LastSpellArcane < 7 && AIoncharm.GetManaPercent(pPlayer) >= 8)
			{
				pPlayer->CastSpell(pTarget, ARCANE_BLAST, false);
				SpellSequence = SPELL_FROST;
				LastSpellArcane = LastSpellArcane + 1;
				break;
			}
			else if (MIRROR_IMAGE > 0 && LastSpellArcane < 8 && AIoncharm.GetManaPercent(pPlayer) >= 10)
			{
				pPlayer->CastSpell(pTarget, MIRROR_IMAGE, false);
				SpellSequence = SPELL_FROST;
				LastSpellArcane = LastSpellArcane + 1;
				break;
			}
			else if (MANA_SHIELD > 0 && LastSpellArcane < 9 && pPlayer->GetHealthPercent() < 70 && pTarget == pPlayer && !pPlayer->HasAura(MANA_SHIELD, EFFECT_INDEX_0) && AIoncharm.GetManaPercent(pPlayer) >= 8)
			{
				pPlayer->CastSpell(pPlayer, MANA_SHIELD, false);
				SpellSequence = SPELL_FROST;
				LastSpellArcane = LastSpellArcane + 1;
				break;
			}
			else
			{
				LastSpellArcane = 0;
				SpellSequence = SPELL_FROST;
			}
		}
	}
}