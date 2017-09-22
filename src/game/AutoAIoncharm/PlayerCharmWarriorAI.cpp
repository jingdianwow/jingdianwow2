
#include "PlayerCharmWarriorAI.h"
#include "AutoAIoncharm.h"


PlayerCharmWarriorAI::PlayerCharmWarriorAI() : SpellSequence(0)
{
}

void PlayerCharmWarriorAI::SpellWarrior(Player* pPlayer)
{
	BATTLE_STANCE           = AIoncharm.initSpell(pPlayer, BATTLE_STANCE_1); //ARMS
    CHARGE                  = AIoncharm.initSpell(pPlayer, CHARGE_1); //ARMS
    OVERPOWER               = AIoncharm.initSpell(pPlayer, OVERPOWER_1); // ARMS
    HEROIC_STRIKE           = AIoncharm.initSpell(pPlayer, HEROIC_STRIKE_1); //ARMS
    REND                    = AIoncharm.initSpell(pPlayer, REND_1); //ARMS
    THUNDER_CLAP            = AIoncharm.initSpell(pPlayer, THUNDER_CLAP_1);  //ARMS
    HAMSTRING               = AIoncharm.initSpell(pPlayer, HAMSTRING_1);  //ARMS
    MOCKING_BLOW            = AIoncharm.initSpell(pPlayer, MOCKING_BLOW_1);  //ARMS
    RETALIATION             = AIoncharm.initSpell(pPlayer, RETALIATION_1);  //ARMS
    SWEEPING_STRIKES        = AIoncharm.initSpell(pPlayer, SWEEPING_STRIKES_1); //ARMS
    MORTAL_STRIKE           = AIoncharm.initSpell(pPlayer, MORTAL_STRIKE_1);  //ARMS
    BLADESTORM              = AIoncharm.initSpell(pPlayer, BLADESTORM_1);  //ARMS
    HEROIC_THROW            = AIoncharm.initSpell(pPlayer, HEROIC_THROW_1);  //ARMS
    SHATTERING_THROW        = AIoncharm.initSpell(pPlayer, SHATTERING_THROW_1);  //ARMS
    BLOODRAGE               = AIoncharm.initSpell(pPlayer, BLOODRAGE_1); //PROTECTION
    DEFENSIVE_STANCE        = AIoncharm.initSpell(pPlayer, DEFENSIVE_STANCE_1); //PROTECTION
    DEVASTATE               = AIoncharm.initSpell(pPlayer, DEVASTATE_1); //PROTECTION
    SUNDER_ARMOR            = AIoncharm.initSpell(pPlayer, SUNDER_ARMOR_1); //PROTECTION
    TAUNT                   = AIoncharm.initSpell(pPlayer, TAUNT_1); //PROTECTION
    SHIELD_BASH             = AIoncharm.initSpell(pPlayer, SHIELD_BASH_1); //PROTECTION
    REVENGE                 = AIoncharm.initSpell(pPlayer, REVENGE_1); //PROTECTION
    SHIELD_BLOCK            = AIoncharm.initSpell(pPlayer, SHIELD_BLOCK_1); //PROTECTION
    DISARM                  = AIoncharm.initSpell(pPlayer, DISARM_1); //PROTECTION
    SHIELD_WALL             = AIoncharm.initSpell(pPlayer, SHIELD_WALL_1); //PROTECTION
    SHIELD_SLAM             = AIoncharm.initSpell(pPlayer, SHIELD_SLAM_1); //PROTECTION
    VIGILANCE               = AIoncharm.initSpell(pPlayer, VIGILANCE_1); //PROTECTION
    DEVASTATE               = AIoncharm.initSpell(pPlayer, DEVASTATE_1); //PROTECTION
    SHOCKWAVE               = AIoncharm.initSpell(pPlayer, SHOCKWAVE_1); //PROTECTION
    CONCUSSION_BLOW         = AIoncharm.initSpell(pPlayer, CONCUSSION_BLOW_1); //PROTECTION
    SPELL_REFLECTION        = AIoncharm.initSpell(pPlayer, SPELL_REFLECTION_1); //PROTECTION
    LAST_STAND              = AIoncharm.initSpell(pPlayer, LAST_STAND_1); //PROTECTION
    BATTLE_SHOUT            = AIoncharm.initSpell(pPlayer, BATTLE_SHOUT_1); //FURY
    DEMORALIZING_SHOUT      = AIoncharm.initSpell(pPlayer, DEMORALIZING_SHOUT_1); //FURY
    CLEAVE                  = AIoncharm.initSpell(pPlayer, CLEAVE_1); //FURY
    INTIMIDATING_SHOUT      = AIoncharm.initSpell(pPlayer, INTIMIDATING_SHOUT_1); //FURY
    EXECUTE                 = AIoncharm.initSpell(pPlayer, EXECUTE_1); //FURY
    CHALLENGING_SHOUT       = AIoncharm.initSpell(pPlayer, CHALLENGING_SHOUT_1); //FURY
    SLAM                    = AIoncharm.initSpell(pPlayer, SLAM_1); //FURY
    BERSERKER_STANCE        = AIoncharm.initSpell(pPlayer, BERSERKER_STANCE_1); //FURY
    INTERCEPT               = AIoncharm.initSpell(pPlayer, INTERCEPT_1); //FURY
    DEATH_WISH              = AIoncharm.initSpell(pPlayer, DEATH_WISH_1); //FURY
    BERSERKER_RAGE          = AIoncharm.initSpell(pPlayer, BERSERKER_RAGE_1); //FURY
    WHIRLWIND               = AIoncharm.initSpell(pPlayer, WHIRLWIND_1); //FURY
    PUMMEL                  = AIoncharm.initSpell(pPlayer, PUMMEL_1); //FURY
    BLOODTHIRST             = AIoncharm.initSpell(pPlayer, BLOODTHIRST_1); //FURY
    RECKLESSNESS            = AIoncharm.initSpell(pPlayer, RECKLESSNESS_1); //FURY
    RAMPAGE                 = 0; // passive
    HEROIC_FURY             = AIoncharm.initSpell(pPlayer, HEROIC_FURY_1); //FURY
    COMMANDING_SHOUT        = AIoncharm.initSpell(pPlayer, COMMANDING_SHOUT_1); //FURY
    ENRAGED_REGENERATION    = AIoncharm.initSpell(pPlayer, ENRAGED_REGENERATION_1); //FURY
    PIERCING_HOWL           = AIoncharm.initSpell(pPlayer, PIERCING_HOWL_1); //FURY

    RECENTLY_BANDAGED       = 11196; // first aid check
}

void PlayerCharmWarriorAI::WarriorAI(Player* pPlayer)
{
	if (pPlayer->getVictim())
	{
		SpellWarrior(pPlayer);
		Unit* pTarget = pPlayer->getVictim();
		float dist = pPlayer->GetCombatDistance(pTarget, true);
		if (pTarget->IsNonMeleeSpellCasted(true))
			SpellSequence = WarriorSpellPreventing;
		else if (pPlayer->HasAura(BATTLE_STANCE, EFFECT_INDEX_0))
			SpellSequence = WarriorBattle;
		else if (pPlayer->HasAura(DEFENSIVE_STANCE, EFFECT_INDEX_0))
			SpellSequence = WarriorDefensive;
		else if (pPlayer->HasAura(BERSERKER_STANCE, EFFECT_INDEX_0))
			SpellSequence = WarriorBerserker;
		switch (SpellSequence)
    {
        case WarriorSpellPreventing:
			if (SHIELD_BASH > 0 && AIoncharm.GetRageAmount(pPlayer) >= 10)
				pPlayer->CastSpell(pTarget, SHIELD_BASH, false);
            else 
			if (PUMMEL > 0 && AIoncharm.GetRageAmount(pPlayer) >= 10)
				pPlayer->CastSpell(pTarget, PUMMEL, false);
            else 
			if (SPELL_REFLECTION > 0 && AIoncharm.GetRageAmount(pPlayer) >= 15 && !pPlayer->HasAura(SPELL_REFLECTION, EFFECT_INDEX_0))
				pPlayer->CastSpell(pPlayer, SPELL_REFLECTION, false);
            break;

        case WarriorBattle:
			if (EXECUTE > 0 && AIoncharm.GetRageAmount(pPlayer) >= 15 && pTarget->GetHealth() < pTarget->GetMaxHealth() * 0.2)
				pPlayer->CastSpell(pTarget, EXECUTE, false);
            else 
			if (LAST_STAND > 0 && !pPlayer->HasAura(LAST_STAND, EFFECT_INDEX_0) && pPlayer->GetHealth() < pPlayer->GetMaxHealth() * 0.5)
				pPlayer->CastSpell(pPlayer, LAST_STAND, false);
            else 
			if (BLOODRAGE > 0 && AIoncharm.GetRageAmount(pPlayer) < 50 && !pPlayer->HasAura(BLOODRAGE, EFFECT_INDEX_0))
				pPlayer->CastSpell(pPlayer, BLOODRAGE, false);
            else 
			if (DEATH_WISH > 0 && AIoncharm.GetRageAmount(pPlayer) >= 10 && !pPlayer->HasAura(DEATH_WISH, EFFECT_INDEX_0))
				pPlayer->CastSpell(pPlayer, DEATH_WISH, false);
            else 
			if (RETALIATION > 0 && pTarget == pPlayer && !pPlayer->HasAura(RETALIATION, EFFECT_INDEX_0))
				pPlayer->CastSpell(pPlayer, RETALIATION, false);
            else 
			if (DEMORALIZING_SHOUT > 0 && AIoncharm.GetRageAmount(pPlayer) >= 10 && !pTarget->HasAura(DEMORALIZING_SHOUT, EFFECT_INDEX_0))
				pPlayer->CastSpell(pTarget, DEMORALIZING_SHOUT, false);
            else 
			if (SWEEPING_STRIKES > 0 && AIoncharm.GetRageAmount(pPlayer) >= 30 && !pPlayer->HasAura(SWEEPING_STRIKES, EFFECT_INDEX_0))
				pPlayer->CastSpell(pPlayer, SWEEPING_STRIKES, false);
            else 
			if (BLADESTORM > 0 && AIoncharm.GetRageAmount(pPlayer) >= 25 && pTarget == pPlayer && !pPlayer->HasAura(BLADESTORM, EFFECT_INDEX_0))
				pPlayer->CastSpell(pTarget, BLADESTORM, false);
            else 
			if (MORTAL_STRIKE > 0 && AIoncharm.GetRageAmount(pPlayer) >= 30 && !pTarget->HasAura(MORTAL_STRIKE, EFFECT_INDEX_0))
				pPlayer->CastSpell(pTarget, MORTAL_STRIKE, false);
            else 
			if (INTIMIDATING_SHOUT > 0 && AIoncharm.GetRageAmount(pPlayer) >= 25)
				pPlayer->CastSpell(pTarget, INTIMIDATING_SHOUT, false);
            else 
			if (THUNDER_CLAP > 0 && AIoncharm.GetRageAmount(pPlayer) >= 20 && pTarget == pPlayer && !pTarget->HasAura(THUNDER_CLAP, EFFECT_INDEX_0))
				pPlayer->CastSpell(pTarget, THUNDER_CLAP, false);
            else
			if (ENRAGED_REGENERATION > 0 && AIoncharm.GetRageAmount(pPlayer) >= 15 && !pPlayer->HasAura(BERSERKER_RAGE, EFFECT_INDEX_0) && !pPlayer->HasAura(ENRAGED_REGENERATION, EFFECT_INDEX_0) && pPlayer->GetHealth() < pPlayer->GetMaxHealth() * 0.5)
				pPlayer->CastSpell(pPlayer, ENRAGED_REGENERATION, false);
            else 
			if (SHOCKWAVE > 0 && AIoncharm.GetRageAmount(pPlayer) >= 15 && pTarget == pPlayer && !pTarget->HasAura(WAR_STOMP, EFFECT_INDEX_0) && !pTarget->HasAura(PIERCING_HOWL, EFFECT_INDEX_0) && !pTarget->HasAura(SHOCKWAVE, EFFECT_INDEX_0) && !pTarget->HasAura(CONCUSSION_BLOW, EFFECT_INDEX_0))
				pPlayer->CastSpell(pTarget, SHOCKWAVE, false);
            else 
			if (REND > 0 && AIoncharm.GetRageAmount(pPlayer) >= 10 && !pTarget->HasAura(REND, EFFECT_INDEX_0))
				pPlayer->CastSpell(pTarget, REND, false);
            else 
			if (HAMSTRING > 0 && AIoncharm.GetRageAmount(pPlayer) >= 10 && !pTarget->HasAura(HAMSTRING, EFFECT_INDEX_0))
				pPlayer->CastSpell(pTarget, HAMSTRING, false);
            else 
			if (CHALLENGING_SHOUT > 0 && AIoncharm.GetRageAmount(pPlayer) >= 5 && pTarget != pPlayer && pPlayer->GetHealthPercent() > 25 && !pTarget->HasAura(MOCKING_BLOW, EFFECT_INDEX_0) && !pTarget->HasAura(CHALLENGING_SHOUT, EFFECT_INDEX_0))
				pPlayer->CastSpell(pTarget, CHALLENGING_SHOUT, false);
            else 
			if (BLOODTHIRST > 0 && AIoncharm.GetRageAmount(pPlayer) >= 20 && !pPlayer->HasAura(BLOODTHIRST, EFFECT_INDEX_0) && pPlayer->GetHealth() < pPlayer->GetMaxHealth() * 0.7)
				pPlayer->CastSpell(pTarget, BLOODTHIRST, false);
            else 
			if (CLEAVE > 0 && AIoncharm.GetRageAmount(pPlayer) >= 20)
				pPlayer->CastSpell(pTarget, CLEAVE, false);
            else 
			if (HEROIC_STRIKE > 0 && AIoncharm.GetRageAmount(pPlayer) >= 15) 
				pPlayer->CastSpell(pTarget, HEROIC_STRIKE, false);
            else 
			if (CONCUSSION_BLOW > 0 && AIoncharm.GetRageAmount(pPlayer) >= 15 && !pTarget->HasAura(WAR_STOMP, EFFECT_INDEX_0) && !pTarget->HasAura(PIERCING_HOWL, EFFECT_INDEX_0) && !pTarget->HasAura(SHOCKWAVE, EFFECT_INDEX_0) && !pTarget->HasAura(CONCUSSION_BLOW, EFFECT_INDEX_0))
				pPlayer->CastSpell(pTarget, CONCUSSION_BLOW, false);
            else 
			if (SLAM > 0 && AIoncharm.GetRageAmount(pPlayer) >= 15)
				pPlayer->CastSpell(pTarget, SLAM, false);
            else 
			if (PIERCING_HOWL > 0 && AIoncharm.GetRageAmount(pPlayer) >= 10 && !pTarget->HasAura(WAR_STOMP, EFFECT_INDEX_0) && !pTarget->HasAura(PIERCING_HOWL, EFFECT_INDEX_0) && !pTarget->HasAura(SHOCKWAVE, EFFECT_INDEX_0) && !pTarget->HasAura(CONCUSSION_BLOW, EFFECT_INDEX_0))
				pPlayer->CastSpell(pTarget, PIERCING_HOWL, false);
            else 
			if (MOCKING_BLOW > 0 && AIoncharm.GetRageAmount(pPlayer) >= 10 && pTarget != pPlayer && pPlayer->GetHealthPercent() > 25 && !pTarget->HasAura(MOCKING_BLOW, EFFECT_INDEX_0) && !pTarget->HasAura(CHALLENGING_SHOUT, EFFECT_INDEX_0))
				pPlayer->CastSpell(pTarget, MOCKING_BLOW, false);
            else 
			if (OVERPOWER > 0 && AIoncharm.GetRageAmount(pPlayer) >= 5)
				pPlayer->CastSpell(pTarget, OVERPOWER, false);
            else 
			if (SUNDER_ARMOR > 0)
				pPlayer->CastSpell(pTarget, SUNDER_ARMOR, false);
            else 
			if (SHATTERING_THROW > 0 && !pTarget->HasAura(SHATTERING_THROW, EFFECT_INDEX_0))
				pPlayer->CastSpell(pTarget, SHATTERING_THROW, false);
            else 
			if (HEROIC_THROW > 0)
				pPlayer->CastSpell(pTarget, HEROIC_THROW, false);
            else 
			if (pPlayer->getRace() == RACE_TAUREN && !pTarget->HasAura(WAR_STOMP, EFFECT_INDEX_0) && !pTarget->HasAura(PIERCING_HOWL, EFFECT_INDEX_0) && !pTarget->HasAura(SHOCKWAVE, EFFECT_INDEX_0) && !pTarget->HasAura(CONCUSSION_BLOW, EFFECT_INDEX_0))
				pPlayer->CastSpell(pTarget, WAR_STOMP, false);
            else 
			if (pPlayer->getRace() == RACE_HUMAN && pPlayer->hasUnitState(UNIT_STAT_STUNNED) || pPlayer->HasAuraType(SPELL_AURA_MOD_FEAR) || pPlayer->HasAuraType(SPELL_AURA_MOD_DECREASE_SPEED) || pPlayer->HasAuraType(SPELL_AURA_MOD_CHARM))
				pPlayer->CastSpell(pPlayer, EVERY_MAN_FOR_HIMSELF, false);
            else 
			if (pPlayer->getRace() == RACE_UNDEAD && pPlayer->HasAuraType(SPELL_AURA_MOD_FEAR) || pPlayer->HasAuraType(SPELL_AURA_MOD_CHARM))
				pPlayer->CastSpell(pPlayer, WILL_OF_THE_FORSAKEN, false);
            else 
			if (pPlayer->getRace() == RACE_GNOME && pPlayer->hasUnitState(UNIT_STAT_STUNNED) || pPlayer->HasAuraType(SPELL_AURA_MOD_DECREASE_SPEED))
				pPlayer->CastSpell(pPlayer, ESCAPE_ARTIST, false);
            else 
			if (pPlayer->getRace() == RACE_NIGHTELF && pTarget == pPlayer && pPlayer->GetHealthPercent() < 25 && !pPlayer->HasAura(SHADOWMELD, EFFECT_INDEX_0))
				pPlayer->CastSpell(pPlayer, SHADOWMELD, false);
            else 
			if (pPlayer->getRace() == RACE_ORC && !pPlayer->HasAura(BLOOD_FURY, EFFECT_INDEX_0))
				pPlayer->CastSpell(pPlayer, BLOOD_FURY, false);
            else 
			if (pPlayer->getRace() == RACE_TROLL && !pPlayer->HasAura(BERSERKING, EFFECT_INDEX_0))
				pPlayer->CastSpell(pPlayer, BERSERKING, false);
            break;

        case WarriorDefensive:
			if (DISARM > 0 && AIoncharm.GetRageAmount(pPlayer) >= 15 && !pTarget->HasAura(DISARM, EFFECT_INDEX_0))
				pPlayer->CastSpell(pTarget, DISARM, false);
			else if (SUNDER_ARMOR > 0 && AIoncharm.GetRageAmount(pPlayer) >= 15)
				pPlayer->CastSpell(pTarget, SUNDER_ARMOR, false);
            else if (REVENGE > 0 && AIoncharm.GetRageAmount(pPlayer) >= 5)
				pPlayer->CastSpell(pTarget, REVENGE, false);
			else if (SHIELD_BLOCK > 0 && !pPlayer->HasAura(SHIELD_BLOCK, EFFECT_INDEX_0))
				pPlayer->CastSpell(pPlayer, SHIELD_BLOCK, false);
			else if (SHIELD_WALL > 0 && !pPlayer->HasAura(SHIELD_WALL, EFFECT_INDEX_0))
				pPlayer->CastSpell(pPlayer, SHIELD_WALL, false);
            break;

        case WarriorBerserker:
			if (WHIRLWIND > 0 && AIoncharm.GetRageAmount(pPlayer) >= 25)
				pPlayer->CastSpell(pTarget, WHIRLWIND, false);
            break;
    }
	}
}