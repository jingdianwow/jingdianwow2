
#include "PlayerCharmPriestAI.h"
#include "AutoAIoncharm.h"

PlayerCharmPriestAI::PlayerCharmPriestAI() : SpellSequence(0), LastSpellHoly(0), LastSpellShadowMagic(0), LastSpellDiscipline(0)
{
}

void PlayerCharmPriestAI::SpellPriest(Player* pPlayer)
{
    RENEW                         = AIoncharm.initSpell(pPlayer, RENEW_1);
    LESSER_HEAL                   = AIoncharm.initSpell(pPlayer, LESSER_HEAL_1);
    FLASH_HEAL                    = AIoncharm.initSpell(pPlayer, FLASH_HEAL_1);
    (FLASH_HEAL > 0) ? FLASH_HEAL : FLASH_HEAL = LESSER_HEAL;
    HEAL                          = AIoncharm.initSpell(pPlayer, HEAL_1);
    (HEAL > 0) ? HEAL : HEAL = FLASH_HEAL;
    GREATER_HEAL                  = AIoncharm.initSpell(pPlayer, GREATER_HEAL_1);
    (GREATER_HEAL > 0) ? GREATER_HEAL : GREATER_HEAL = HEAL;
    RESURRECTION                  = AIoncharm.initSpell(pPlayer, RESURRECTION_1);
    SMITE                         = AIoncharm.initSpell(pPlayer, SMITE_1);
    MANA_BURN                     = AIoncharm.initSpell(pPlayer, MANA_BURN_1);
    HOLY_NOVA                     = AIoncharm.initSpell(pPlayer, HOLY_NOVA_1);
    HOLY_FIRE                     = AIoncharm.initSpell(pPlayer, HOLY_FIRE_1);
    DESPERATE_PRAYER              = AIoncharm.initSpell(pPlayer, DESPERATE_PRAYER_1);
    PRAYER_OF_HEALING             = AIoncharm.initSpell(pPlayer, PRAYER_OF_HEALING_1);
    CIRCLE_OF_HEALING             = AIoncharm.initSpell(pPlayer, CIRCLE_OF_HEALING_1);
    BINDING_HEAL                  = AIoncharm.initSpell(pPlayer, BINDING_HEAL_1);
    PRAYER_OF_MENDING             = AIoncharm.initSpell(pPlayer, PRAYER_OF_MENDING_1);

    // SHADOW
    FADE                          = AIoncharm.initSpell(pPlayer, FADE_1);
    SHADOW_WORD_PAIN              = AIoncharm.initSpell(pPlayer, SHADOW_WORD_PAIN_1);
    MIND_BLAST                    = AIoncharm.initSpell(pPlayer, MIND_BLAST_1);
    SCREAM                        = AIoncharm.initSpell(pPlayer, PSYCHIC_SCREAM_1);
    MIND_FLAY                     = AIoncharm.initSpell(pPlayer, MIND_FLAY_1);
    DEVOURING_PLAGUE              = AIoncharm.initSpell(pPlayer, DEVOURING_PLAGUE_1);
    SHADOW_PROTECTION             = AIoncharm.initSpell(pPlayer, SHADOW_PROTECTION_1);
    VAMPIRIC_TOUCH                = AIoncharm.initSpell(pPlayer, VAMPIRIC_TOUCH_1);
    PRAYER_OF_SHADOW_PROTECTION   = AIoncharm.initSpell(pPlayer, PRAYER_OF_SHADOW_PROTECTION_1);
    SHADOWFIEND                   = AIoncharm.initSpell(pPlayer, SHADOWFIEND_1);
    MIND_SEAR                     = AIoncharm.initSpell(pPlayer, MIND_SEAR_1);

    // DISCIPLINE
    PENANCE                       = AIoncharm.initSpell(pPlayer, PENANCE_1);
    INNER_FIRE                    = AIoncharm.initSpell(pPlayer, INNER_FIRE_1);
    POWER_WORD_SHIELD             = AIoncharm.initSpell(pPlayer, POWER_WORD_SHIELD_1);
    POWER_WORD_FORTITUDE          = AIoncharm.initSpell(pPlayer, POWER_WORD_FORTITUDE_1);
    PRAYER_OF_FORTITUDE           = AIoncharm.initSpell(pPlayer, PRAYER_OF_FORTITUDE_1);
    FEAR_WARD                     = AIoncharm.initSpell(pPlayer, FEAR_WARD_1);
    DIVINE_SPIRIT                 = AIoncharm.initSpell(pPlayer, DIVINE_SPIRIT_1);
    PRAYER_OF_SPIRIT              = AIoncharm.initSpell(pPlayer, PRAYER_OF_SPIRIT_1);
    MASS_DISPEL                   = AIoncharm.initSpell(pPlayer, MASS_DISPEL_1);
    POWER_INFUSION                = AIoncharm.initSpell(pPlayer, POWER_INFUSION_1);
    INNER_FOCUS                   = AIoncharm.initSpell(pPlayer, INNER_FOCUS_1);
}

void PlayerCharmPriestAI::PriestAI(Player* pPlayer)
{
	if (pPlayer->getVictim())
	{
		SpellPriest(pPlayer);
		Unit* pTarget = pPlayer->getVictim();
		float dist = pPlayer->GetCombatDistance(pTarget, true);
		switch (SpellSequence)
		{
		case SPELL_HOLY:
			if (SMITE > 0 && LastSpellHoly < 1 && !pTarget->HasAura(SMITE, EFFECT_INDEX_0) && AIoncharm.GetManaPercent(pPlayer) >= 17)
			{
				pPlayer->CastSpell(pTarget, SMITE, false);
				SpellSequence = SPELL_SHADOWMAGIC;
				LastSpellHoly = LastSpellHoly + 1;
				break;
			}
			else if (MANA_BURN > 0 && LastSpellHoly < 2 && pTarget->GetPower(POWER_MANA) > 0 && AIoncharm.GetManaPercent(pPlayer) < 70 && AIoncharm.GetManaPercent(pPlayer) >= 14)
			{
				pPlayer->CastSpell(pTarget, MANA_BURN, false);
				SpellSequence = SPELL_SHADOWMAGIC;
				LastSpellHoly = LastSpellHoly + 1;
				break;
			}
			else if (HOLY_NOVA > 0 && LastSpellHoly < 3 && dist <= ATTACK_DISTANCE && AIoncharm.GetManaPercent(pPlayer) >= 22)
			{
				pPlayer->CastSpell(pPlayer, HOLY_NOVA, false);
				SpellSequence = SPELL_SHADOWMAGIC;
				LastSpellHoly = LastSpellHoly + 1;
			}
			else if (HOLY_FIRE > 0 && LastSpellHoly < 4 && !pTarget->HasAura(HOLY_FIRE, EFFECT_INDEX_0) && AIoncharm.GetManaPercent(pPlayer) >= 13)
			{			
				pPlayer->CastSpell(pTarget, HOLY_FIRE, false);
				SpellSequence = SPELL_SHADOWMAGIC;
				LastSpellHoly = LastSpellHoly + 1;
				break;
			}
			else if (PRAYER_OF_MENDING > 0 && LastSpellHoly < 5 && pTarget == pPlayer && pPlayer->GetHealth() <= pPlayer->GetMaxHealth() * 0.7 && !pPlayer->HasAura(PRAYER_OF_MENDING, EFFECT_INDEX_0) && AIoncharm.GetManaPercent(pPlayer) >= 15)
			{
				pPlayer->CastSpell(pPlayer, PRAYER_OF_MENDING, false);
				SpellSequence = SPELL_SHADOWMAGIC;
				LastSpellHoly = LastSpellHoly + 1;
				break;
			}

			LastSpellHoly = 0;

		case SPELL_SHADOWMAGIC:
			if (SHADOW_WORD_PAIN > 0 && LastSpellShadowMagic < 1 && !pTarget->HasAura(SHADOW_WORD_PAIN, EFFECT_INDEX_0) && AIoncharm.GetManaPercent(pPlayer) >= 25)
			{
				pPlayer->CastSpell(pTarget, SHADOW_WORD_PAIN, false);
				SpellSequence = SPELL_DISCIPLINE;
				LastSpellShadowMagic = LastSpellShadowMagic + 1;
				break;
			}
			else if (MIND_BLAST > 0 && LastSpellShadowMagic < 2 && AIoncharm.GetManaPercent(pPlayer) >= 19)
			{
				pPlayer->CastSpell(pTarget, MIND_BLAST, false);
				SpellSequence = SPELL_DISCIPLINE;
				LastSpellShadowMagic = LastSpellShadowMagic + 1;
				break;
			}
			else if (SCREAM > 0 && LastSpellShadowMagic < 3 && AIoncharm.GetManaPercent(pPlayer) >= 15)
			{
				pPlayer->CastSpell(pTarget, SCREAM, false);
				SpellSequence = SPELL_DISCIPLINE;
				LastSpellShadowMagic = LastSpellShadowMagic + 1;
				break;
			}
			else if (MIND_FLAY > 0 && LastSpellShadowMagic < 4 && !pTarget->HasAura(MIND_FLAY, EFFECT_INDEX_0) && AIoncharm.GetManaPercent(pPlayer) >= 10)
			{
				pPlayer->CastSpell(pTarget, MIND_FLAY, false);
				SpellSequence = SPELL_DISCIPLINE;
				LastSpellShadowMagic = LastSpellShadowMagic + 1;
				break;
			}
			else if (DEVOURING_PLAGUE > 0 && LastSpellShadowMagic < 5 && !pTarget->HasAura(DEVOURING_PLAGUE, EFFECT_INDEX_0) && AIoncharm.GetManaPercent(pPlayer) >= 28)
			{
				pPlayer->CastSpell(pTarget, DEVOURING_PLAGUE, false);
				SpellSequence = SPELL_DISCIPLINE;
				LastSpellShadowMagic = LastSpellShadowMagic + 1;
				break;
			}
			else if (SHADOW_PROTECTION > 0 && LastSpellShadowMagic < 6 && AIoncharm.GetManaPercent(pPlayer) >= 60)
			{
				pPlayer->CastSpell(pTarget, SHADOW_PROTECTION, false);
				SpellSequence = SPELL_DISCIPLINE;
				LastSpellShadowMagic = LastSpellShadowMagic + 1;
				break;
			}
			else if (VAMPIRIC_TOUCH > 0 && LastSpellShadowMagic < 7 && !pTarget->HasAura(VAMPIRIC_TOUCH, EFFECT_INDEX_0) && AIoncharm.GetManaPercent(pPlayer) >= 18)
			{
				pPlayer->CastSpell(pTarget, VAMPIRIC_TOUCH, false);
				SpellSequence = SPELL_DISCIPLINE;
				LastSpellShadowMagic = LastSpellShadowMagic + 1;
				break;
			}
			else if (SHADOWFIEND > 0 && LastSpellShadowMagic < 8)
			{
				pPlayer->CastSpell(pPlayer, SHADOWFIEND, false);
				SpellSequence = SPELL_DISCIPLINE;
				LastSpellShadowMagic = LastSpellShadowMagic + 1;
				break;
			}
			else if (MIND_SEAR > 0 && LastSpellShadowMagic < 9 && AIoncharm.GetManaPercent(pPlayer) >= 28)
			{
				pPlayer->CastSpell(pTarget, MIND_SEAR, false);
				SpellSequence = SPELL_DISCIPLINE;
				LastSpellShadowMagic = LastSpellShadowMagic + 1;
				break;
			}

			LastSpellShadowMagic = 0;


		case SPELL_DISCIPLINE:
			if (FEAR_WARD > 0 && LastSpellDiscipline < 1 && AIoncharm.GetManaPercent(pPlayer) >= 3)
			{
				pPlayer->CastSpell(pPlayer, FEAR_WARD, false);
				SpellSequence = SPELL_HOLY;
				LastSpellDiscipline = LastSpellDiscipline + 1;
				break;
			}
			else if (POWER_INFUSION > 0 && LastSpellDiscipline < 2 && AIoncharm.GetManaPercent(pPlayer) >= 16)
			{
				pPlayer->CastSpell(pPlayer, POWER_INFUSION, false);
				SpellSequence = SPELL_HOLY;
				LastSpellDiscipline = LastSpellDiscipline + 1;
				break;
			}
			else if (MASS_DISPEL > 0 && LastSpellDiscipline < 3 && AIoncharm.GetManaPercent(pPlayer) >= 33)
			{
				pPlayer->CastSpell(pPlayer, MASS_DISPEL, false);
				SpellSequence = SPELL_HOLY;
				LastSpellDiscipline = LastSpellDiscipline + 1;
				break;
			}
			else if (INNER_FOCUS > 0 && !pPlayer->HasAura(INNER_FOCUS, EFFECT_INDEX_0) && LastSpellDiscipline < 4)
			{
				pPlayer->CastSpell(pPlayer, INNER_FOCUS, false);
				SpellSequence = SPELL_HOLY;
				LastSpellDiscipline = LastSpellDiscipline + 1;
				break;
			}
			else if (PENANCE > 0 && LastSpellDiscipline < 5 && AIoncharm.GetManaPercent(pPlayer) >= 16)
			{
				pPlayer->CastSpell(pPlayer, PENANCE, false);
				SpellSequence = SPELL_HOLY;
				LastSpellDiscipline = LastSpellDiscipline + 1;
				break;
			}
			else
			{
				SpellSequence = SPELL_HOLY;
				LastSpellDiscipline = 0;
			}
		}
	}
}
