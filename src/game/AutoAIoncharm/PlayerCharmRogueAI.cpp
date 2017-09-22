
#include "PlayerCharmRogueAI.h"
#include "AutoAIoncharm.h"


PlayerCharmRogueAI::PlayerCharmRogueAI() : SpellSequence(0)
{
}

void PlayerCharmRogueAI::SpellRogue(Player* pPlayer)
{
	SINISTER_STRIKE          = AIoncharm.initSpell(pPlayer, SINISTER_STRIKE_1);
    BACKSTAB                 = AIoncharm.initSpell(pPlayer, BACKSTAB_1);
    KICK                     = AIoncharm.initSpell(pPlayer, KICK_1);
    FEINT                    = AIoncharm.initSpell(pPlayer, FEINT_1);
    FAN_OF_KNIVES            = AIoncharm.initSpell(pPlayer, FAN_OF_KNIVES_1);
    GOUGE                    = AIoncharm.initSpell(pPlayer, GOUGE_1);
    SPRINT                   = AIoncharm.initSpell(pPlayer, SPRINT_1);

    SHADOWSTEP               = AIoncharm.initSpell(pPlayer, SHADOWSTEP_1);
    STEALTH                  = AIoncharm.initSpell(pPlayer, STEALTH_1);
    VANISH                   = AIoncharm.initSpell(pPlayer, VANISH_1);
    EVASION                  = AIoncharm.initSpell(pPlayer, EVASION_1);
    CLOAK_OF_SHADOWS         = AIoncharm.initSpell(pPlayer, CLOAK_OF_SHADOWS_1);
    HEMORRHAGE               = AIoncharm.initSpell(pPlayer, HEMORRHAGE_1);
    GHOSTLY_STRIKE           = AIoncharm.initSpell(pPlayer, GHOSTLY_STRIKE_1);
    SHADOW_DANCE             = AIoncharm.initSpell(pPlayer, SHADOW_DANCE_1);
    BLIND                    = AIoncharm.initSpell(pPlayer, BLIND_1);
    DISTRACT                 = AIoncharm.initSpell(pPlayer, DISTRACT_1);
    PREPARATION              = AIoncharm.initSpell(pPlayer, PREPARATION_1);
    PREMEDITATION            = AIoncharm.initSpell(pPlayer, PREMEDITATION_1);
    PICK_POCKET              = AIoncharm.initSpell(pPlayer, PICK_POCKET_1);

    EVISCERATE               = AIoncharm.initSpell(pPlayer, EVISCERATE_1);
    KIDNEY_SHOT              = AIoncharm.initSpell(pPlayer, KIDNEY_SHOT_1);
    SLICE_DICE               = AIoncharm.initSpell(pPlayer, SLICE_AND_DICE_1);
    GARROTE                  = AIoncharm.initSpell(pPlayer, GARROTE_1);
    EXPOSE_ARMOR             = AIoncharm.initSpell(pPlayer, EXPOSE_ARMOR_1);
    RUPTURE                  = AIoncharm.initSpell(pPlayer, RUPTURE_1);
    DISMANTLE                = AIoncharm.initSpell(pPlayer, DISMANTLE_1);
    CHEAP_SHOT               = AIoncharm.initSpell(pPlayer, CHEAP_SHOT_1);
    AMBUSH                   = AIoncharm.initSpell(pPlayer, AMBUSH_1);
    MUTILATE                 = AIoncharm.initSpell(pPlayer, MUTILATE_1);

    RECENTLY_BANDAGED   = 11196; // first aid check
}

void PlayerCharmRogueAI::RogueAI(Player* pPlayer)
{
	if (pPlayer->getVictim())
	{
		SpellRogue(pPlayer);
		Unit* pTarget = pPlayer->getVictim();
		float dist = pPlayer->GetCombatDistance(pTarget, true);
		if (pPlayer->HasAura(STEALTH, EFFECT_INDEX_0))
			SpellSequence = RogueStealth;
		else if (pTarget->IsNonMeleeSpellCasted(true))
			SpellSequence = RogueSpellPreventing;
		else if (pTarget == pPlayer && pPlayer->GetHealthPercent() < 40)
			SpellSequence = RogueThreat;
		else
			SpellSequence = RogueCombat;

		// we fight in melee, target is not in range, skip the next part!
		if (dist > ATTACK_DISTANCE)
			return;

		switch (SpellSequence)
		{
		case RogueStealth:
			if (PICK_POCKET > 0)
				pPlayer->CastSpell(pTarget, PICK_POCKET, false);
			else if (PREMEDITATION > 0)
				pPlayer->CastSpell(pTarget, PREMEDITATION, false);
			else if (AMBUSH > 0 && AIoncharm.GetEnergyAmount(pPlayer) >= 60)
				pPlayer->CastSpell(pTarget, AMBUSH, false);
			else if (CHEAP_SHOT > 0 && !pTarget->HasAura(CHEAP_SHOT, EFFECT_INDEX_0) && AIoncharm.GetEnergyAmount(pPlayer) >= 60)
				pPlayer->CastSpell(pTarget, CHEAP_SHOT, false);
			else if (GARROTE > 0 && AIoncharm.GetEnergyAmount(pPlayer) >= 50)
				pPlayer->CastSpell(pTarget, GARROTE, false);
			else
				pPlayer->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);
			break;
		case RogueThreat:
			if (GOUGE > 0 && AIoncharm.GetEnergyAmount(pPlayer) >= 45 && !pTarget->HasAura(GOUGE, EFFECT_INDEX_0))
				pPlayer->CastSpell(pTarget, GOUGE, false);
			else if (EVASION > 0 && pPlayer->GetHealthPercent() <= 35 && !pPlayer->HasAura(EVASION, EFFECT_INDEX_0))
				pPlayer->CastSpell(pPlayer, EVASION, false);
			else if (BLIND > 0 && pPlayer->GetHealthPercent() <= 30 && !pTarget->HasAura(BLIND, EFFECT_INDEX_0) && AIoncharm.GetEnergyAmount(pPlayer) >= 30)
				pPlayer->CastSpell(pTarget, BLIND, false);
			else if (FEINT > 0 && pPlayer->GetHealthPercent() <= 25 && AIoncharm.GetEnergyAmount(pPlayer) >= 20)
				pPlayer->CastSpell(pPlayer, FEINT, false);
			else if (VANISH > 0 && pPlayer->GetHealthPercent() <= 20 && !pPlayer->HasAura(FEINT, EFFECT_INDEX_0))
				pPlayer->CastSpell(pPlayer, VANISH, false);
			else if (PREPARATION > 0)
				pPlayer->CastSpell(pPlayer, PREPARATION, false);
			else if (pPlayer->getRace() == RACE_NIGHTELF && pPlayer->GetHealthPercent() <= 15 && !pPlayer->HasAura(SHADOWMELD, EFFECT_INDEX_0))
				pPlayer->CastSpell(pPlayer, SHADOWMELD, false);
			break;
		case RogueSpellPreventing:
			if (KIDNEY_SHOT > 0 && AIoncharm.GetEnergyAmount(pPlayer) >= 25 && pPlayer->GetComboPoints() >= 2)
				pPlayer->CastSpell(pTarget, KIDNEY_SHOT, false);
			else if (KICK > 0 && AIoncharm.GetEnergyAmount(pPlayer) >= 25)
				pPlayer->CastSpell(pTarget, KICK, false);
			break;
		case RogueCombat:
		default:
			if (pPlayer->GetComboPoints() <= 4)
			{
				if (SHADOW_DANCE > 0 && !pPlayer->HasAura(SHADOW_DANCE, EFFECT_INDEX_0))
					pPlayer->CastSpell(pPlayer, SHADOW_DANCE, false);
				else if (CHEAP_SHOT > 0 && pPlayer->HasAura(SHADOW_DANCE, EFFECT_INDEX_0) && !pTarget->HasAura(CHEAP_SHOT, EFFECT_INDEX_0) && AIoncharm.GetEnergyAmount(pPlayer) >= 60)
					pPlayer->CastSpell(pTarget, CHEAP_SHOT, false);
				else if (AMBUSH > 0 && pPlayer->HasAura(SHADOW_DANCE, EFFECT_INDEX_0) && AIoncharm.GetEnergyAmount(pPlayer) >= 60)
					pPlayer->CastSpell(pTarget, AMBUSH, false);
				else if (GARROTE > 0 && pPlayer->HasAura(SHADOW_DANCE, EFFECT_INDEX_0) && AIoncharm.GetEnergyAmount(pPlayer) >= 50)
					pPlayer->CastSpell(pTarget, GARROTE, false);
				else if (BACKSTAB > 0 && pTarget->isInBackInMap(pPlayer, 1) && AIoncharm.GetEnergyAmount(pPlayer) >= 60)
					pPlayer->CastSpell(pTarget, BACKSTAB, false);
				else if (MUTILATE > 0 && AIoncharm.GetEnergyAmount(pPlayer) >= 60)
					pPlayer->CastSpell(pTarget, MUTILATE, false);
				else if (SINISTER_STRIKE > 0 && AIoncharm.GetEnergyAmount(pPlayer) >= 45)
					pPlayer->CastSpell(pTarget, SINISTER_STRIKE, false);
				else if (GHOSTLY_STRIKE > 0 && AIoncharm.GetEnergyAmount(pPlayer) >= 40)
					pPlayer->CastSpell(pTarget, GHOSTLY_STRIKE, false);
				else if (HEMORRHAGE > 0 && AIoncharm.GetEnergyAmount(pPlayer) >= 35)
					pPlayer->CastSpell(pTarget, HEMORRHAGE, false);
				else if (DISMANTLE > 0 && !pTarget->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISARMED) && AIoncharm.GetEnergyAmount(pPlayer) >= 25)
					pPlayer->CastSpell(pTarget, DISMANTLE, false);
				else if (SHADOWSTEP > 0 && AIoncharm.GetEnergyAmount(pPlayer) >= 10)
					pPlayer->CastSpell(pTarget, SHADOWSTEP, false);
				else if ((pPlayer->getRace() == RACE_HUMAN && pPlayer->hasUnitState(UNIT_STAT_STUNNED)) || pPlayer->HasAuraType(SPELL_AURA_MOD_FEAR) || pPlayer->HasAuraType(SPELL_AURA_MOD_DECREASE_SPEED) || (pPlayer->HasAuraType(SPELL_AURA_MOD_CHARM)))
					pPlayer->CastSpell(pPlayer, EVERY_MAN_FOR_HIMSELF, false);
				else if ((pPlayer->getRace() == RACE_UNDEAD && pPlayer->HasAuraType(SPELL_AURA_MOD_FEAR)) || (pPlayer->HasAuraType(SPELL_AURA_MOD_CHARM)))
					pPlayer->CastSpell(pPlayer, WILL_OF_THE_FORSAKEN, false);
				else if ((pPlayer->getRace() == RACE_GNOME && pPlayer->hasUnitState(UNIT_STAT_STUNNED)) || (pPlayer->HasAuraType(SPELL_AURA_MOD_DECREASE_SPEED)))
					pPlayer->CastSpell(pPlayer, ESCAPE_ARTIST, false);
				else if (pPlayer->getRace() == RACE_ORC && !pPlayer->HasAura(BLOOD_FURY, EFFECT_INDEX_0))
					pPlayer->CastSpell(pPlayer, BLOOD_FURY, false);
				else if (pPlayer->getRace() == RACE_TROLL && !pPlayer->HasAura(BERSERKING, EFFECT_INDEX_0))
					pPlayer->CastSpell(pPlayer, BERSERKING, false);
			}
			else
			{
				if (EVISCERATE > 0 && pTarget->getClass() == CLASS_ROGUE && AIoncharm.GetEnergyAmount(pPlayer) >= 35)
					pPlayer->CastSpell(pTarget, EVISCERATE, false);
				else if (EVISCERATE > 0 && pTarget->getClass() == CLASS_DRUID && AIoncharm.GetEnergyAmount(pPlayer) >= 35)
					pPlayer->CastSpell(pTarget, EVISCERATE, false);
				else if (KIDNEY_SHOT > 0 && pTarget->getClass() == CLASS_SHAMAN && AIoncharm.GetEnergyAmount(pPlayer) >= 25)
					pPlayer->CastSpell(pTarget, KIDNEY_SHOT, false);
				else if (SLICE_DICE > 0 && pTarget->getClass() == CLASS_WARLOCK && AIoncharm.GetEnergyAmount(pPlayer) >= 25)
					pPlayer->CastSpell(pTarget, SLICE_DICE, false);
				else if (SLICE_DICE > 0 && pTarget->getClass() == CLASS_HUNTER && AIoncharm.GetEnergyAmount(pPlayer) >= 25)
					pPlayer->CastSpell(pTarget, SLICE_DICE, false);
				else if (EXPOSE_ARMOR > 0 && pTarget->getClass() == CLASS_WARRIOR && !pTarget->HasAura(EXPOSE_ARMOR, EFFECT_INDEX_0) && AIoncharm.GetEnergyAmount(pPlayer) >= 25)
					pPlayer->CastSpell(pTarget, EXPOSE_ARMOR, false);
				else if (EXPOSE_ARMOR > 0 && pTarget->getClass() == CLASS_PALADIN && !pTarget->HasAura(EXPOSE_ARMOR, EFFECT_INDEX_0) && AIoncharm.GetEnergyAmount(pPlayer) >= 25)
					pPlayer->CastSpell(pTarget, EXPOSE_ARMOR, false);
				else if (RUPTURE > 0 && pTarget->getClass() == CLASS_MAGE && AIoncharm.GetEnergyAmount(pPlayer) >= 25)
					pPlayer->CastSpell(pTarget, RUPTURE, false);
				else if (RUPTURE > 0 && pTarget->getClass() == CLASS_PRIEST && AIoncharm.GetEnergyAmount(pPlayer) >= 25)
					pPlayer->CastSpell(pTarget, RUPTURE, false);
				else if (EVISCERATE > 0 && AIoncharm.GetEnergyAmount(pPlayer) >= 35)
					pPlayer->CastSpell(pTarget, EVISCERATE, false);
			}
			break;
		}
	}
}