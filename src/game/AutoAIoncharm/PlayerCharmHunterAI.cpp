
#include "PlayerCharmHunterAI.h"
#include "AutoAIoncharm.h"


PlayerCharmHunterAI::PlayerCharmHunterAI() : SpellSequence(0)
{
}

void PlayerCharmHunterAI::SpellHunter(Player* pPlayer)
{
	// PET CTRL
    PET_SUMMON                    = AIoncharm.initSpell(pPlayer, CALL_PET_1);
    PET_DISMISS                   = AIoncharm.initSpell(pPlayer, DISMISS_PET_1);
    PET_REVIVE                    = AIoncharm.initSpell(pPlayer, REVIVE_PET_1);
    PET_MEND                      = AIoncharm.initSpell(pPlayer, MEND_PET_1);
    PET_FEED                      = 1539;

    INTIMIDATION                  = AIoncharm.initSpell(pPlayer, INTIMIDATION_1); // (generic)

    // PET SKILLS must be initialized by pets
    SONIC_BLAST                   = 0; // bat
    DEMORALIZING_SCREECH          = 0;
    BAD_ATTITUDE                  = 0; // crocolisk
    NETHER_SHOCK                  = 0;

    // RANGED COMBAT
    AUTO_SHOT                     = AIoncharm.initSpell(pPlayer, AUTO_SHOT_1);
    HUNTERS_MARK                  = AIoncharm.initSpell(pPlayer, HUNTERS_MARK_1);
    ARCANE_SHOT                   = AIoncharm.initSpell(pPlayer, ARCANE_SHOT_1);
    CONCUSSIVE_SHOT               = AIoncharm.initSpell(pPlayer, CONCUSSIVE_SHOT_1);
    DISTRACTING_SHOT              = AIoncharm.initSpell(pPlayer, DISTRACTING_SHOT_1);
    MULTI_SHOT                    = AIoncharm.initSpell(pPlayer, MULTISHOT_1);
    EXPLOSIVE_SHOT                = AIoncharm.initSpell(pPlayer, EXPLOSIVE_SHOT_1);
    SERPENT_STING                 = AIoncharm.initSpell(pPlayer, SERPENT_STING_1);
    SCORPID_STING                 = AIoncharm.initSpell(pPlayer, SCORPID_STING_1);
    WYVERN_STING                  = AIoncharm.initSpell(pPlayer, WYVERN_STING_1);
    VIPER_STING                   = AIoncharm.initSpell(pPlayer, VIPER_STING_1);
    AIMED_SHOT                    = AIoncharm.initSpell(pPlayer, AIMED_SHOT_1);
    STEADY_SHOT                   = AIoncharm.initSpell(pPlayer, STEADY_SHOT_1);
    CHIMERA_SHOT                  = AIoncharm.initSpell(pPlayer, CHIMERA_SHOT_1);
    VOLLEY                        = AIoncharm.initSpell(pPlayer, VOLLEY_1);
    BLACK_ARROW                   = AIoncharm.initSpell(pPlayer, BLACK_ARROW_1);
    KILL_SHOT                     = AIoncharm.initSpell(pPlayer, KILL_SHOT_1);

    // MELEE
    RAPTOR_STRIKE                 = AIoncharm.initSpell(pPlayer, RAPTOR_STRIKE_1);
    WING_CLIP                     = AIoncharm.initSpell(pPlayer, WING_CLIP_1);
    MONGOOSE_BITE                 = AIoncharm.initSpell(pPlayer, MONGOOSE_BITE_1);
    DISENGAGE                     = AIoncharm.initSpell(pPlayer, DISENGAGE_1);
    MISDIRECTION                  = AIoncharm.initSpell(pPlayer, MISDIRECTION_1);
    DETERRENCE                    = AIoncharm.initSpell(pPlayer, DETERRENCE_1);

    // TRAPS
    BEAR_TRAP                     = 0; // non-player spell
    FREEZING_TRAP                 = AIoncharm.initSpell(pPlayer, FREEZING_TRAP_1);
    IMMOLATION_TRAP               = AIoncharm.initSpell(pPlayer, IMMOLATION_TRAP_1);
    FROST_TRAP                    = AIoncharm.initSpell(pPlayer, FROST_TRAP_1);
    EXPLOSIVE_TRAP                = AIoncharm.initSpell(pPlayer, EXPLOSIVE_TRAP_1);
    ARCANE_TRAP                   = 0; // non-player spell
    SNAKE_TRAP                    = AIoncharm.initSpell(pPlayer, SNAKE_TRAP_1);

    // BUFFS
    ASPECT_OF_THE_HAWK            = AIoncharm.initSpell(pPlayer, ASPECT_OF_THE_HAWK_1);
    ASPECT_OF_THE_MONKEY          = AIoncharm.initSpell(pPlayer, ASPECT_OF_THE_MONKEY_1);
    RAPID_FIRE                    = AIoncharm.initSpell(pPlayer, RAPID_FIRE_1);
    TRUESHOT_AURA                 = AIoncharm.initSpell(pPlayer, TRUESHOT_AURA_1);

    RECENTLY_BANDAGED             = 11196; // first aid check

	m_petSummonFailed = false;
	m_rangedCombat = true;
}

void PlayerCharmHunterAI::HunterAI(Player* pPlayer)
{
	if (pPlayer->getVictim())
	{
		SpellHunter(pPlayer);
		Unit* pTarget = pPlayer->getVictim();
		float dist = pPlayer->GetCombatDistance(pTarget, true);
		if (pPlayer->getRace() == RACE_ORC && !pPlayer->HasAura(BLOOD_FURY, EFFECT_INDEX_0))
			pPlayer->CastSpell(pPlayer, BLOOD_FURY, false);
		//ai->TellMaster( "Blood Fury." );
		else if (pPlayer->getRace() == RACE_TROLL && !pPlayer->HasAura(BERSERKING, EFFECT_INDEX_0))
			pPlayer->CastSpell(pPlayer, BERSERKING, false);

		if ((dist <= ATTACK_DISTANCE || !pPlayer->GetUInt32Value(PLAYER_AMMO_ID)) && m_rangedCombat)
		{
			m_rangedCombat = false;
			if (!pPlayer->GetUInt32Value(PLAYER_AMMO_ID) && ASPECT_OF_THE_MONKEY > 0 && !pPlayer->HasAura(ASPECT_OF_THE_MONKEY, EFFECT_INDEX_0))
				pPlayer->CastSpell(pPlayer, ASPECT_OF_THE_MONKEY, false);
		}
		else if (dist > ATTACK_DISTANCE && !m_rangedCombat)
		{
			// switch to ranged combat
			m_rangedCombat = true;
			if (ASPECT_OF_THE_HAWK > 0 && !pPlayer->HasAura(ASPECT_OF_THE_HAWK, EFFECT_INDEX_0)) 
				pPlayer->CastSpell(pPlayer, ASPECT_OF_THE_HAWK, false);
		}
		else if (m_rangedCombat && !pPlayer->HasAura(ASPECT_OF_THE_HAWK, EFFECT_INDEX_0) && ASPECT_OF_THE_HAWK > 0)
			    pPlayer->CastSpell(pPlayer, ASPECT_OF_THE_HAWK, false);
		else if (!m_rangedCombat && !pPlayer->HasAura(ASPECT_OF_THE_MONKEY, EFFECT_INDEX_0) && ASPECT_OF_THE_MONKEY > 0)
			    pPlayer->CastSpell(pPlayer, ASPECT_OF_THE_MONKEY, false);

		if (AUTO_SHOT > 0 && m_rangedCombat && !pPlayer->FindCurrentSpellBySpellId(AUTO_SHOT))
			pPlayer->CastSpell(pTarget, AUTO_SHOT, false);
		else if (AUTO_SHOT > 0 && pPlayer->FindCurrentSpellBySpellId(AUTO_SHOT))
			pPlayer->InterruptNonMeleeSpells(true, AUTO_SHOT);

		std::ostringstream out;
		if (m_rangedCombat)
		{
			if (HUNTERS_MARK > 0 && AIoncharm.GetManaPercent(pPlayer) >= 3 && !pTarget->HasAura(HUNTERS_MARK, EFFECT_INDEX_0))
				pPlayer->CastSpell(pTarget, HUNTERS_MARK, false);
			else if (RAPID_FIRE > 0 && AIoncharm.GetManaPercent(pPlayer) >= 3 && !pPlayer->HasAura(RAPID_FIRE, EFFECT_INDEX_0))
				pPlayer->CastSpell(pPlayer, RAPID_FIRE, false);
			else if (MULTI_SHOT > 0 && AIoncharm.GetManaPercent(pPlayer) >= 13)
				pPlayer->CastSpell(pTarget, MULTI_SHOT, false);
			else if (ARCANE_SHOT > 0 && AIoncharm.GetManaPercent(pPlayer) >= 7)
				pPlayer->CastSpell(pTarget, ARCANE_SHOT, false);
			else if (CONCUSSIVE_SHOT > 0 && AIoncharm.GetManaPercent(pPlayer) >= 6 && !pTarget->HasAura(CONCUSSIVE_SHOT, EFFECT_INDEX_0))
				pPlayer->CastSpell(pTarget, CONCUSSIVE_SHOT, false);
			else if (EXPLOSIVE_SHOT > 0 && AIoncharm.GetManaPercent(pPlayer) >= 10 && !pTarget->HasAura(EXPLOSIVE_SHOT, EFFECT_INDEX_0))
				pPlayer->CastSpell(pTarget, EXPLOSIVE_SHOT, false);
			else if (VIPER_STING > 0 && AIoncharm.GetManaPercent(pPlayer) >= 8 && pTarget->GetPower(POWER_MANA) > 0 && AIoncharm.GetManaPercent(pPlayer) < 70 && !pTarget->HasAura(VIPER_STING, EFFECT_INDEX_0))
				pPlayer->CastSpell(pTarget, VIPER_STING, false);
			else if (SERPENT_STING > 0 && AIoncharm.GetManaPercent(pPlayer) >= 13 && !pTarget->HasAura(SERPENT_STING, EFFECT_INDEX_0) && !pTarget->HasAura(SCORPID_STING, EFFECT_INDEX_0) && !pTarget->HasAura(VIPER_STING, EFFECT_INDEX_0))
				pPlayer->CastSpell(pTarget, SERPENT_STING, false);
			else if (SCORPID_STING > 0 && AIoncharm.GetManaPercent(pPlayer) >= 11 && !pTarget->HasAura(WYVERN_STING, EFFECT_INDEX_0) && !pTarget->HasAura(SCORPID_STING, EFFECT_INDEX_0) && !pTarget->HasAura(SERPENT_STING, EFFECT_INDEX_0) && !pTarget->HasAura(VIPER_STING, EFFECT_INDEX_0))
				pPlayer->CastSpell(pTarget, SCORPID_STING, false);
			else if (CHIMERA_SHOT > 0 && AIoncharm.GetManaPercent(pPlayer) >= 12)
				pPlayer->CastSpell(pTarget, CHIMERA_SHOT, false);
			else if (VOLLEY > 0 && AIoncharm.GetManaPercent(pPlayer) >= 24)
				pPlayer->CastSpell(pTarget, VOLLEY, false);
			else if (BLACK_ARROW > 0 && AIoncharm.GetManaPercent(pPlayer) >= 6 && !pTarget->HasAura(BLACK_ARROW, EFFECT_INDEX_0))
				pPlayer->CastSpell(pTarget, BLACK_ARROW, false);
			else if (AIMED_SHOT > 0 && AIoncharm.GetManaPercent(pPlayer) >= 12)
				pPlayer->CastSpell(pTarget, AIMED_SHOT, false);
			else if (STEADY_SHOT > 0 && AIoncharm.GetManaPercent(pPlayer) >= 5) 
				pPlayer->CastSpell(pTarget, STEADY_SHOT, false);
			else if (KILL_SHOT > 0 && AIoncharm.GetManaPercent(pPlayer) >= 7 && pTarget->GetHealth() < pTarget->GetMaxHealth() * 0.2)
				pPlayer->CastSpell(pTarget, KILL_SHOT, false);
		}
		else
		{
			if (RAPTOR_STRIKE > 0 && AIoncharm.GetManaPercent(pPlayer) >= 6)
				pPlayer->CastSpell(pPlayer, RAPTOR_STRIKE, false);
			else if (EXPLOSIVE_TRAP > 0 && AIoncharm.GetManaPercent(pPlayer) >= 27 && !pTarget->HasAura(EXPLOSIVE_TRAP, EFFECT_INDEX_0) && !pTarget->HasAura(ARCANE_TRAP, EFFECT_INDEX_0) && !pTarget->HasAura(IMMOLATION_TRAP, EFFECT_INDEX_0) && !pTarget->HasAura(FROST_TRAP, EFFECT_INDEX_0) && !pTarget->HasAura(BEAR_TRAP, EFFECT_INDEX_0))
				pPlayer->CastSpell(pTarget, EXPLOSIVE_TRAP, false);
			else if (WING_CLIP > 0 && AIoncharm.GetManaPercent(pPlayer) >= 6 && !pTarget->HasAura(WING_CLIP, EFFECT_INDEX_0))
				pPlayer->CastSpell(pTarget, WING_CLIP, false);
			else if (IMMOLATION_TRAP > 0 && AIoncharm.GetManaPercent(pPlayer) >= 13 && !pTarget->HasAura(IMMOLATION_TRAP, EFFECT_INDEX_0) && !pTarget->HasAura(ARCANE_TRAP, EFFECT_INDEX_0) && !pTarget->HasAura(EXPLOSIVE_TRAP, EFFECT_INDEX_0) && !pTarget->HasAura(FROST_TRAP, EFFECT_INDEX_0) && !pTarget->HasAura(BEAR_TRAP, EFFECT_INDEX_0))
				pPlayer->CastSpell(pTarget, IMMOLATION_TRAP, false);
			else if (MONGOOSE_BITE > 0 && AIoncharm.GetManaPercent(pPlayer) >= 4)
				pPlayer->CastSpell(pTarget, MONGOOSE_BITE, false);
			else if (FROST_TRAP > 0 && AIoncharm.GetManaPercent(pPlayer) >= 2 && !pTarget->HasAura(FROST_TRAP, EFFECT_INDEX_0) && !pTarget->HasAura(ARCANE_TRAP, EFFECT_INDEX_0) && !pTarget->HasAura(IMMOLATION_TRAP, EFFECT_INDEX_0) && !pTarget->HasAura(EXPLOSIVE_TRAP, EFFECT_INDEX_0) && !pTarget->HasAura(BEAR_TRAP, EFFECT_INDEX_0))
				pPlayer->CastSpell(pTarget, FROST_TRAP, false);
			else if (ARCANE_TRAP > 0 && !pTarget->HasAura(ARCANE_TRAP, EFFECT_INDEX_0) && !pTarget->HasAura(BEAR_TRAP, EFFECT_INDEX_0) && !pTarget->HasAura(EXPLOSIVE_TRAP, EFFECT_INDEX_0) && !pTarget->HasAura(IMMOLATION_TRAP, EFFECT_INDEX_0) && !pTarget->HasAura(FROST_TRAP, EFFECT_INDEX_0))
				pPlayer->CastSpell(pTarget, ARCANE_TRAP, false);
			else if (DETERRENCE > 0 && pTarget == pPlayer && pPlayer->GetHealth() < pPlayer->GetMaxHealth() * 0.5 && !pPlayer->HasAura(DETERRENCE, EFFECT_INDEX_0))
				pPlayer->CastSpell(pPlayer, DETERRENCE, false);
			else if (pPlayer->getRace() == RACE_TAUREN && !pTarget->HasAura(WAR_STOMP, EFFECT_INDEX_0))
				pPlayer->CastSpell(pTarget, WAR_STOMP, false);
			else if (pPlayer->getRace() == RACE_NIGHTELF && pTarget == pPlayer && pPlayer->GetHealthPercent() < 25 && !pPlayer->HasAura(SHADOWMELD, EFFECT_INDEX_0))
				pPlayer->CastSpell(pPlayer, SHADOWMELD, false);
		}
	}
}