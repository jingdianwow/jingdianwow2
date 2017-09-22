
#include "PlayerCharmPaladinAI.h"
#include "AutoAIoncharm.h"


PlayerCharmPaladinAI::PlayerCharmPaladinAI() : SpellSequence(0)
{
}

void PlayerCharmPaladinAI::SpellPaladin(Player* pPlayer)
{
	RETRIBUTION_AURA              = AIoncharm.initSpell(pPlayer, RETRIBUTION_AURA_1);
    CRUSADER_AURA                 = AIoncharm.initSpell(pPlayer, CRUSADER_AURA_1);
    CRUSADER_STRIKE               = AIoncharm.initSpell(pPlayer, CRUSADER_STRIKE_1);
    SEAL_OF_COMMAND               = AIoncharm.initSpell(pPlayer, SEAL_OF_COMMAND_1);
    SEAL_OF_RIGHTEOUSNESS         = AIoncharm.initSpell(pPlayer, SEAL_OF_RIGHTEOUSNESS_1);
    SEAL_OF_CORRUPTION            = AIoncharm.initSpell(pPlayer, SEAL_OF_CORRUPTION_1);
    SEAL_OF_JUSTICE               = AIoncharm.initSpell(pPlayer, SEAL_OF_JUSTICE_1);
    SEAL_OF_LIGHT                 = AIoncharm.initSpell(pPlayer, SEAL_OF_LIGHT_1);
    SEAL_OF_VENGEANCE             = AIoncharm.initSpell(pPlayer, SEAL_OF_VENGEANCE_1);
    SEAL_OF_WISDOM                = AIoncharm.initSpell(pPlayer, SEAL_OF_WISDOM_1);
    JUDGEMENT_OF_LIGHT            = AIoncharm.initSpell(pPlayer, JUDGEMENT_OF_LIGHT_1);
    JUDGEMENT_OF_WISDOM           = AIoncharm.initSpell(pPlayer, JUDGEMENT_OF_WISDOM_1);
    JUDGEMENT_OF_JUSTICE          = AIoncharm.initSpell(pPlayer, JUDGEMENT_OF_JUSTICE_1);
    DIVINE_STORM                  = AIoncharm.initSpell(pPlayer, DIVINE_STORM_1);
    BLESSING_OF_MIGHT             = AIoncharm.initSpell(pPlayer, BLESSING_OF_MIGHT_1);
    GREATER_BLESSING_OF_MIGHT     = AIoncharm.initSpell(pPlayer, GREATER_BLESSING_OF_MIGHT_1);
    HAMMER_OF_WRATH               = AIoncharm.initSpell(pPlayer, HAMMER_OF_WRATH_1);
    FLASH_OF_LIGHT                = AIoncharm.initSpell(pPlayer, FLASH_OF_LIGHT_1); // Holy
    HOLY_LIGHT                    = AIoncharm.initSpell(pPlayer, HOLY_LIGHT_1);
    HOLY_SHOCK                    = AIoncharm.initSpell(pPlayer, HOLY_SHOCK_1);
    HOLY_WRATH                    = AIoncharm.initSpell(pPlayer, HOLY_WRATH_1);
    DIVINE_FAVOR                  = AIoncharm.initSpell(pPlayer, DIVINE_FAVOR_1);
    CONCENTRATION_AURA            = AIoncharm.initSpell(pPlayer, CONCENTRATION_AURA_1);
    BLESSING_OF_WISDOM            = AIoncharm.initSpell(pPlayer, BLESSING_OF_WISDOM_1);
    GREATER_BLESSING_OF_WISDOM    = AIoncharm.initSpell(pPlayer, GREATER_BLESSING_OF_WISDOM_1);
    CONSECRATION                  = AIoncharm.initSpell(pPlayer, CONSECRATION_1);
    AVENGING_WRATH                = AIoncharm.initSpell(pPlayer, AVENGING_WRATH_1);
    LAY_ON_HANDS                  = AIoncharm.initSpell(pPlayer, LAY_ON_HANDS_1);
    EXORCISM                      = AIoncharm.initSpell(pPlayer, EXORCISM_1);
    SACRED_SHIELD                 = AIoncharm.initSpell(pPlayer, SACRED_SHIELD_1);
    DIVINE_PLEA                   = AIoncharm.initSpell(pPlayer, DIVINE_PLEA_1);
    BLESSING_OF_KINGS             = AIoncharm.initSpell(pPlayer, BLESSING_OF_KINGS_1);
    GREATER_BLESSING_OF_KINGS     = AIoncharm.initSpell(pPlayer, GREATER_BLESSING_OF_KINGS_1);
    BLESSING_OF_SANCTUARY         = AIoncharm.initSpell(pPlayer, BLESSING_OF_SANCTUARY_1);
    GREATER_BLESSING_OF_SANCTUARY = AIoncharm.initSpell(pPlayer, GREATER_BLESSING_OF_SANCTUARY_1);
    HAMMER_OF_JUSTICE             = AIoncharm.initSpell(pPlayer, HAMMER_OF_JUSTICE_1);
    RIGHTEOUS_FURY                = AIoncharm.initSpell(pPlayer, RIGHTEOUS_FURY_1);
    RIGHTEOUS_DEFENSE             = AIoncharm.initSpell(pPlayer, RIGHTEOUS_DEFENSE_1);
    SHADOW_RESISTANCE_AURA        = AIoncharm.initSpell(pPlayer, SHADOW_RESISTANCE_AURA_1);
    DEVOTION_AURA                 = AIoncharm.initSpell(pPlayer, DEVOTION_AURA_1);
    FIRE_RESISTANCE_AURA          = AIoncharm.initSpell(pPlayer, FIRE_RESISTANCE_AURA_1);
    FROST_RESISTANCE_AURA         = AIoncharm.initSpell(pPlayer, FROST_RESISTANCE_AURA_1);
    HAND_OF_PROTECTION            = AIoncharm.initSpell(pPlayer, HAND_OF_PROTECTION_1);
    DIVINE_PROTECTION             = AIoncharm.initSpell(pPlayer, DIVINE_PROTECTION_1);
    DIVINE_INTERVENTION           = AIoncharm.initSpell(pPlayer, DIVINE_INTERVENTION_1);
    DIVINE_SACRIFICE              = AIoncharm.initSpell(pPlayer, DIVINE_SACRIFICE_1);
    DIVINE_SHIELD                 = AIoncharm.initSpell(pPlayer, DIVINE_SHIELD_1);
    HOLY_SHIELD                   = AIoncharm.initSpell(pPlayer, HOLY_SHIELD_1);
    AVENGERS_SHIELD               = AIoncharm.initSpell(pPlayer, AVENGERS_SHIELD_1);
    HAND_OF_SACRIFICE             = AIoncharm.initSpell(pPlayer, HAND_OF_SACRIFICE_1);
    SHIELD_OF_RIGHTEOUSNESS       = AIoncharm.initSpell(pPlayer, SHIELD_OF_RIGHTEOUSNESS_1);
    REDEMPTION                    = AIoncharm.initSpell(pPlayer, REDEMPTION_1);

    // Warrior auras
    DEFENSIVE_STANCE              = 71;   //Def Stance
    BERSERKER_STANCE              = 2458; //Ber Stance
    BATTLE_STANCE                 = 2457; //Bat Stance

    FORBEARANCE                   = 25771; // cannot be protected

    RECENTLY_BANDAGED             = 11196; // first aid check
}

void PlayerCharmPaladinAI::PaladinAI(Player* pPlayer)
{
	if (pPlayer->getVictim())
	{
		SpellPaladin(pPlayer);
		Unit* pTarget = pPlayer->getVictim();
		float dist = pPlayer->GetCombatDistance(pTarget, true);
		switch (SpellSequence)
    {
        case Combat:
            if (JUDGEMENT_OF_LIGHT > 0 && !pTarget->HasAura(JUDGEMENT_OF_LIGHT, EFFECT_INDEX_0) && CombatCounter < 1 && AIoncharm.GetManaPercent(pPlayer) >= 5)
            {
				pPlayer->CastSpell(pTarget, JUDGEMENT_OF_LIGHT, false);
                CombatCounter++;
                break;
            }
            else if (SEAL_OF_COMMAND > 0 && !pPlayer->HasAura(SEAL_OF_COMMAND, EFFECT_INDEX_0) && CombatCounter < 2 && AIoncharm.GetManaPercent(pPlayer) >= 14)
            {
				pPlayer->CastSpell(pPlayer, SEAL_OF_COMMAND, false);
                CombatCounter++;
                break;
            }
            else if (HAMMER_OF_JUSTICE > 0 && !pTarget->HasAura(HAMMER_OF_JUSTICE, EFFECT_INDEX_0) && CombatCounter < 3 && AIoncharm.GetManaPercent(pPlayer) >= 3)
            {
				pPlayer->CastSpell(pTarget, HAMMER_OF_JUSTICE, false);
                CombatCounter++;
                break;
            }
            else if (CRUSADER_STRIKE > 0 && CombatCounter < 4 && AIoncharm.GetManaPercent(pPlayer) >= 5)
            {
				pPlayer->CastSpell(pTarget, CRUSADER_STRIKE, false);
                CombatCounter++;
                break;
            }
            else if (AVENGING_WRATH > 0 && CombatCounter < 5 && !pPlayer->HasAura(AVENGING_WRATH, EFFECT_INDEX_0) && AIoncharm.GetManaPercent(pPlayer) >= 8)
            {
				pPlayer->CastSpell(pPlayer, AVENGING_WRATH, false);
                CombatCounter++;
                break;
            }
			else if (SACRED_SHIELD > 0 && CombatCounter < 6 && pTarget == pPlayer && pPlayer->GetHealthPercent() < 70 && !pPlayer->HasAura(SACRED_SHIELD, EFFECT_INDEX_0) && AIoncharm.GetManaPercent(pPlayer) >= 12)
            {
				pPlayer->CastSpell(pPlayer, SACRED_SHIELD, false);
                CombatCounter++;
                break;
            }
            else if (DIVINE_STORM > 0 && CombatCounter < 7 && dist <= ATTACK_DISTANCE && AIoncharm.GetManaPercent(pPlayer) >= 12)
            {
				pPlayer->CastSpell(pTarget, DIVINE_STORM, false);
                CombatCounter++;
                break;
            }
            else if (HAMMER_OF_WRATH > 0 && CombatCounter < 8 && pTarget->GetHealth() < pTarget->GetMaxHealth() * 0.20 && AIoncharm.GetManaPercent(pPlayer) >= 14)
            {
				pPlayer->CastSpell(pTarget, HAMMER_OF_WRATH, false);
                CombatCounter++;
                break;
            }
			else if (HOLY_WRATH > 0 && CombatCounter < 9 && dist <= ATTACK_DISTANCE && AIoncharm.GetManaPercent(pPlayer) >= 24)
            {
				pPlayer->CastSpell(pTarget, HOLY_WRATH, false);
                CombatCounter++;
                break;
            }
			else if (HAND_OF_SACRIFICE > 0 && pTarget == pPlayer && !pPlayer->HasAura(HAND_OF_SACRIFICE, EFFECT_INDEX_0) && CombatCounter < 10 && AIoncharm.GetManaPercent(pPlayer) >= 6)
            {
				pPlayer->CastSpell(pPlayer, HAND_OF_SACRIFICE, false);
                CombatCounter++;
                break;
            }
			else if (DIVINE_PROTECTION > 0 && pTarget == pPlayer && !pPlayer->HasAura(FORBEARANCE, EFFECT_INDEX_0) && pPlayer->GetHealthPercent() < 30 && CombatCounter < 11 && AIoncharm.GetManaPercent(pPlayer) >= 3)
            {
				pPlayer->CastSpell(pPlayer, DIVINE_PROTECTION, false);
                CombatCounter++;
                break;
            }
			else if (RIGHTEOUS_DEFENSE > 0 && pTarget != pPlayer && pPlayer->GetHealthPercent() > 70 && CombatCounter < 12)
            {
				pPlayer->CastSpell(pTarget, RIGHTEOUS_DEFENSE, false);
                CombatCounter++;
                break;
            }
            else if (DIVINE_PLEA > 0 && !pPlayer->HasAura(DIVINE_PLEA, EFFECT_INDEX_0) && AIoncharm.GetManaPercent(pPlayer) < 50 && CombatCounter < 13)
            {
				pPlayer->CastSpell(pPlayer, DIVINE_PLEA, false);
                CombatCounter++;
                break;
            }
            else if (DIVINE_FAVOR > 0 && !pPlayer->HasAura(DIVINE_FAVOR, EFFECT_INDEX_0) && CombatCounter < 14)
            {
				pPlayer->CastSpell(pPlayer, DIVINE_FAVOR, false);
                CombatCounter++;
                break;
            }
            else if (CombatCounter > 15)
            {
                CombatCounter = 0;
                break;
            }
            else
            {
                CombatCounter = 0;
                break;
            }

    }

    if (AVENGING_WRATH > 0 && !pPlayer->HasAura(AVENGING_WRATH, EFFECT_INDEX_0) && AIoncharm.GetManaPercent(pPlayer) >= 8)
		pPlayer->CastSpell(pPlayer, AVENGING_WRATH, false);

	if (DIVINE_SHIELD > 0 && pPlayer->GetHealthPercent() < 30 && pTarget == pPlayer && !pPlayer->HasAura(FORBEARANCE, EFFECT_INDEX_0) && !pPlayer->HasAura(DIVINE_SHIELD, EFFECT_INDEX_0) && AIoncharm.GetManaPercent(pPlayer) >= 3)
		pPlayer->CastSpell(pPlayer, DIVINE_SHIELD, false);

	if (DIVINE_SACRIFICE > 0 && pPlayer->GetHealthPercent() > 50 && pTarget != pPlayer && !pPlayer->HasAura(DIVINE_SACRIFICE, EFFECT_INDEX_0))
		pPlayer->CastSpell(pPlayer, DIVINE_SACRIFICE, false);
	}
}