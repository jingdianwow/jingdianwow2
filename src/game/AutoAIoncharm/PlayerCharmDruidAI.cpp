
#include "PlayerCharmDruidAI.h"
#include "AutoAIoncharm.h"


PlayerCharmDruidAI::PlayerCharmDruidAI() : SpellSequence(0)
{
}

void PlayerCharmDruidAI::SpellDruid(Player* pPlayer)
{
	MOONFIRE                      = AIoncharm.initSpell(pPlayer, MOONFIRE_1); // attacks
    STARFIRE                      = AIoncharm.initSpell(pPlayer, STARFIRE_1);
    STARFALL                      = AIoncharm.initSpell(pPlayer, STARFALL_1);
    WRATH                         = AIoncharm.initSpell(pPlayer, WRATH_1);
    ROOTS                         = AIoncharm.initSpell(pPlayer, ENTANGLING_ROOTS_1);
    INSECT_SWARM                  = AIoncharm.initSpell(pPlayer, INSECT_SWARM_1);
    FORCE_OF_NATURE               = AIoncharm.initSpell(pPlayer, FORCE_OF_NATURE_1);
    HURRICANE                     = AIoncharm.initSpell(pPlayer, HURRICANE_1);
    MARK_OF_THE_WILD              = AIoncharm.initSpell(pPlayer, MARK_OF_THE_WILD_1); // buffs
    GIFT_OF_THE_WILD              = AIoncharm.initSpell(pPlayer, GIFT_OF_THE_WILD_1);
    THORNS                        = AIoncharm.initSpell(pPlayer, THORNS_1);
    BARKSKIN                      = AIoncharm.initSpell(pPlayer, BARKSKIN_1);
    INNERVATE                     = AIoncharm.initSpell(pPlayer, INNERVATE_1);
    FAERIE_FIRE                   = AIoncharm.initSpell(pPlayer, FAERIE_FIRE_1); // debuffs
    REJUVENATION                  = AIoncharm.initSpell(pPlayer, REJUVENATION_1); // heals
    REGROWTH                      = AIoncharm.initSpell(pPlayer, REGROWTH_1);
    WILD_GROWTH                   = AIoncharm.initSpell(pPlayer, WILD_GROWTH_1);
    LIFEBLOOM                     = AIoncharm.initSpell(pPlayer, LIFEBLOOM_1);
    NOURISH                       = AIoncharm.initSpell(pPlayer, NOURISH_1);
    HEALING_TOUCH                 = AIoncharm.initSpell(pPlayer, HEALING_TOUCH_1);
    SWIFTMEND                     = AIoncharm.initSpell(pPlayer, SWIFTMEND_1);
    TRANQUILITY                   = AIoncharm.initSpell(pPlayer, TRANQUILITY_1);
    REVIVE                        = AIoncharm.initSpell(pPlayer, REVIVE_1);
    // Druid Forms
    MOONKIN_FORM                  = AIoncharm.initSpell(pPlayer, MOONKIN_FORM_1);
    DIRE_BEAR_FORM                = AIoncharm.initSpell(pPlayer, DIRE_BEAR_FORM_1);
    BEAR_FORM                     = AIoncharm.initSpell(pPlayer, BEAR_FORM_1);
    CAT_FORM                      = AIoncharm.initSpell(pPlayer, CAT_FORM_1);
    TREE_OF_LIFE                  = AIoncharm.initSpell(pPlayer, TREE_OF_LIFE_1);
    TRAVEL_FORM                   = AIoncharm.initSpell(pPlayer, TRAVEL_FORM_1);
    // Cat Attack type's
    RAKE                          = AIoncharm.initSpell(pPlayer, RAKE_1);
    CLAW                          = AIoncharm.initSpell(pPlayer, CLAW_1); // 45
    COWER                         = AIoncharm.initSpell(pPlayer, COWER_1); // 20
    MANGLE                        = AIoncharm.initSpell(pPlayer, MANGLE_1); // 45
    TIGERS_FURY                   = AIoncharm.initSpell(pPlayer, TIGERS_FURY_1);
    // Cat Finishing Move's
    RIP                           = AIoncharm.initSpell(pPlayer, RIP_1); // 30
    FEROCIOUS_BITE                = AIoncharm.initSpell(pPlayer, FEROCIOUS_BITE_1); // 35
    MAIM                          = AIoncharm.initSpell(pPlayer, MAIM_1); // 35
    // Bear/Dire Bear Attacks & Buffs
    BASH                          = AIoncharm.initSpell(pPlayer, BASH_1);
    MAUL                          = AIoncharm.initSpell(pPlayer, MAUL_1); // 15
    SWIPE                         = AIoncharm.initSpell(pPlayer, SWIPE_BEAR_1); // 20
    DEMORALIZING_ROAR             = AIoncharm.initSpell(pPlayer, DEMORALIZING_ROAR_1); // 10
    CHALLENGING_ROAR              = AIoncharm.initSpell(pPlayer, CHALLENGING_ROAR_1);
    ENRAGE                        = AIoncharm.initSpell(pPlayer, ENRAGE_1);
    GROWL                         = AIoncharm.initSpell(pPlayer, GROWL_1);

    RECENTLY_BANDAGED             = 11196; // first aid check
}

void PlayerCharmDruidAI::DruidAI(Player* pPlayer)
{
	if (pPlayer->getVictim())
	{
		SpellDruid(pPlayer);
		Unit* pTarget = pPlayer->getVictim();
		float dist = pPlayer->GetCombatDistance(pTarget, true);
		if (IsFeral())
			SpellSequence = DruidCombat;
		else
			SpellSequence = DruidSpell;

		switch (SpellSequence)
		{
			if (pPlayer->HasAura(CAT_FORM, EFFECT_INDEX_0))
			{
				pPlayer->RemoveAurasDueToSpell(768);
				break;
			}
			if (pPlayer->HasAura(BEAR_FORM, EFFECT_INDEX_0))
			{
				pPlayer->RemoveAurasDueToSpell(5487);
				break;
			}
			if (pPlayer->HasAura(DIRE_BEAR_FORM, EFFECT_INDEX_0))
			{
				pPlayer->RemoveAurasDueToSpell(9634);
				break;
			}
			if (pPlayer->HasAura(MOONKIN_FORM, EFFECT_INDEX_0))
			{
				pPlayer->RemoveAurasDueToSpell(24858);
				break;
			}
			if (FAERIE_FIRE > 0 && DruidSpellCombat < 1 && !pTarget->HasAura(FAERIE_FIRE, EFFECT_INDEX_0) && AIoncharm.GetManaPercent(pPlayer) >= 8)
			{
				pPlayer->CastSpell(pTarget, FAERIE_FIRE, false);
				DruidSpellCombat++;
				break;
			}
			else if (MOONFIRE > 0 && DruidSpellCombat < 2 && !pTarget->HasAura(MOONFIRE, EFFECT_INDEX_0) && AIoncharm.GetManaPercent(pPlayer) >= 24)
			{
				pPlayer->CastSpell(pTarget, MOONFIRE, false);
				DruidSpellCombat++;
				break;
			}
			else if (ROOTS > 0 && DruidSpellCombat < 3 && !pTarget->HasAura(ROOTS, EFFECT_INDEX_0) && AIoncharm.GetManaPercent(pPlayer) >= 8)
			{
				pPlayer->CastSpell(pTarget, ROOTS, false);
				DruidSpellCombat++;
				break;
			}
			else if (HURRICANE > 0 && DruidSpellCombat < 4 && AIoncharm.GetManaPercent(pPlayer) >= 91)
			{
				//ai->TellMaster("casting hurricane!");
				pPlayer->CastSpell(pTarget, HURRICANE, false);
				DruidSpellCombat++;
				break;
			}
			else if (WRATH > 0 && DruidSpellCombat < 5 && AIoncharm.GetManaPercent(pPlayer) >= 13)
			{
				pPlayer->CastSpell(pTarget, WRATH, false);
				DruidSpellCombat++;
				break;
			}
			else if (INSECT_SWARM > 0 && DruidSpellCombat < 6 && !pTarget->HasAura(INSECT_SWARM, EFFECT_INDEX_0) && AIoncharm.GetManaPercent(pPlayer) >= 9)
			{
				pPlayer->CastSpell(pTarget, INSECT_SWARM, false);
				DruidSpellCombat++;
				break;
			}
			else if (STARFIRE > 0 && DruidSpellCombat < 7 && AIoncharm.GetManaPercent(pPlayer) >= 18)
			{
				pPlayer->CastSpell(pTarget, STARFIRE, false);
				DruidSpellCombat++;
				break;
			}
			else if (FORCE_OF_NATURE > 0 && DruidSpellCombat < 8 && AIoncharm.GetManaPercent(pPlayer) >= 12)
			{
				//ai->TellMaster("summoning treants.");
				pPlayer->CastSpell(pPlayer, FORCE_OF_NATURE, false);
				DruidSpellCombat++;
				break;
			}
			else if (STARFALL > 0 && !pPlayer->HasAura(STARFALL, EFFECT_INDEX_0) && DruidSpellCombat < 9 && AIoncharm.GetManaPercent(pPlayer) >= 39)
			{
				pPlayer->CastSpell(pTarget, STARFALL, false);
				DruidSpellCombat++;
				break;
			}
			else if (BARKSKIN > 0 && pTarget == pPlayer && DruidSpellCombat < 10 && !pPlayer->HasAura(BARKSKIN, EFFECT_INDEX_0))
			{
				pPlayer->CastSpell(pPlayer, BARKSKIN, false);
				DruidSpellCombat++;
				break;
			}
			else if (INNERVATE > 0 && AIoncharm.GetManaPercent(pPlayer) < 50 && DruidSpellCombat < 11 && !pPlayer->HasAura(INNERVATE, EFFECT_INDEX_0))
			{
				pPlayer->CastSpell(pPlayer, INNERVATE, false);
				DruidSpellCombat++;
				break;
			}
			else if (DruidSpellCombat > 13)
			{
				DruidSpellCombat = 0;
				break;
			}
			else
			{
				DruidSpellCombat = 0;
				break;
			}
			break;
		case DruidCombat:
			if (pPlayer->HasAura(BEAR_FORM, EFFECT_INDEX_0))
			{
				pPlayer->RemoveAurasDueToSpell(5487);
				//ai->TellMaster("FormClearBear");
				break;
			}
			if (pPlayer->HasAura(DIRE_BEAR_FORM, EFFECT_INDEX_0))
			{
				pPlayer->RemoveAurasDueToSpell(9634);
				//ai->TellMaster("FormClearDireBear");
				break;
			}
			if (pPlayer->HasAura(MOONKIN_FORM, EFFECT_INDEX_0))
			{
				pPlayer->RemoveAurasDueToSpell(24858);
				//ai->TellMaster("FormClearMoonkin");
				break;
			}
			if (CAT_FORM > 0 && !pPlayer->HasAura(CAT_FORM, EFFECT_INDEX_0))
				pPlayer->CastSpell(pPlayer, CAT_FORM, false);
			/*
			if (COWER > 0 && m_bot->GetComboPoints() == 1 && AIoncharm.GetEnergyAmount(pPlayer) >= 20)
			{
			pPlayer->CastSpell(COWER);
			//ai->TellMaster("Cower");
			}*/
			if (MAIM > 0 && pPlayer->GetComboPoints() >= 1 && pTarget->IsNonMeleeSpellCasted(true))
			{
				pPlayer->CastSpell(pTarget, MAIM, false);
				//ai->TellMaster("SpellPreventing Maim");
				break;
			}

			if (RAKE > 0 && pPlayer->GetComboPoints() <= 1 && AIoncharm.GetEnergyAmount(pPlayer) >= 40)
			{
				pPlayer->CastSpell(pTarget, RAKE, false);
				//ai->TellMaster("Rake");
				break;
			}
			else if (CLAW > 0 && pPlayer->GetComboPoints() <= 2 && AIoncharm.GetEnergyAmount(pPlayer) >= 45)
			{
				pPlayer->CastSpell(pTarget, CLAW, false);
				//ai->TellMaster("Claw");
				break;
			}
			else if (MANGLE > 0 && pPlayer->GetComboPoints() <= 3 && AIoncharm.GetEnergyAmount(pPlayer) >= 45)
			{
				pPlayer->CastSpell(pTarget, MANGLE, false);
				//ai->TellMaster("Mangle");
				break;
			}
			else if (CLAW > 0 && pPlayer->GetComboPoints() <= 4 && AIoncharm.GetEnergyAmount(pPlayer) >= 45)
			{
				pPlayer->CastSpell(pTarget, CLAW, false);
				//ai->TellMaster("Claw2");
				break;
			}

			if (pPlayer->GetComboPoints() == 5)
			{
				if (RIP > 0 && pTarget->getClass() == CLASS_ROGUE && AIoncharm.GetEnergyAmount(pPlayer) >= 30)
					pPlayer->CastSpell(pTarget, RIP, false);
				else if (MAIM > 0 && pTarget->getClass() == CLASS_DRUID && AIoncharm.GetEnergyAmount(pPlayer) >= 35)
					pPlayer->CastSpell(pTarget, MAIM, false);
				else if (MAIM > 0 && pTarget->getClass() == CLASS_SHAMAN && AIoncharm.GetEnergyAmount(pPlayer) >= 35)
					pPlayer->CastSpell(pTarget, MAIM, false);
				else if (MAIM > 0 && pTarget->getClass() == CLASS_WARLOCK && AIoncharm.GetEnergyAmount(pPlayer) >= 35)
					pPlayer->CastSpell(pTarget, MAIM, false);
				else if (FEROCIOUS_BITE > 0 && pTarget->getClass() == CLASS_HUNTER && AIoncharm.GetEnergyAmount(pPlayer) >= 35)
					pPlayer->CastSpell(pTarget, FEROCIOUS_BITE, false);
				else if (FEROCIOUS_BITE > 0 && pTarget->getClass() == CLASS_WARRIOR && AIoncharm.GetEnergyAmount(pPlayer) >= 35)
					pPlayer->CastSpell(pTarget, FEROCIOUS_BITE, false);
				else if (FEROCIOUS_BITE > 0 && pTarget->getClass() == CLASS_PALADIN && AIoncharm.GetEnergyAmount(pPlayer) >= 35)
					pPlayer->CastSpell(pTarget, FEROCIOUS_BITE, false);
				else if (MAIM > 0 && pTarget->getClass() == CLASS_MAGE && AIoncharm.GetEnergyAmount(pPlayer) >= 35)
					pPlayer->CastSpell(pTarget, MAIM, false);
				else if (MAIM > 0 && pTarget->getClass() == CLASS_PRIEST && AIoncharm.GetEnergyAmount(pPlayer) >= 35)
					pPlayer->CastSpell(pTarget, MAIM, false);
				else if (MAIM > 0 && AIoncharm.GetEnergyAmount(pPlayer) >= 35)
					pPlayer->CastSpell(pTarget, MAIM, false);
				break;
			}
			else
			{
				DruidSpellCombat = 0;
				break;
			}
			break;
		}
	}
}

bool PlayerCharmDruidAI::IsFeral()
{
	if (MOONKIN_FORM > 0)
		return true;
	else if (DIRE_BEAR_FORM > 0)
		return true;
	else if (BEAR_FORM > 0)
		return true;
	else if (CAT_FORM > 0)
		return true;
	else
		return false;
}