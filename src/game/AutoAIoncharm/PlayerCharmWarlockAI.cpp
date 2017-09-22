
#include "PlayerCharmWarlockAI.h"
#include "AutoAIoncharm.h"

PlayerCharmWarlockAI::PlayerCharmWarlockAI() : SpellSequence(0), LastSpellCurse(0), LastSpellAffliction(0), LastSpellDestruction(0)
{
}

void PlayerCharmWarlockAI::SpellWarlock(Player* pPlayer)
{
    // DESTRUCTION
    SHADOW_BOLT           = AIoncharm.initSpell(pPlayer, SHADOW_BOLT_1);
    IMMOLATE              = AIoncharm.initSpell(pPlayer, IMMOLATE_1);
    INCINERATE            = AIoncharm.initSpell(pPlayer, INCINERATE_1);
    SEARING_PAIN          = AIoncharm.initSpell(pPlayer, SEARING_PAIN_1);
    CONFLAGRATE           = AIoncharm.initSpell(pPlayer, CONFLAGRATE_1);
    SHADOWFURY            = AIoncharm.initSpell(pPlayer, SHADOWFURY_1);
    CHAOS_BOLT            = AIoncharm.initSpell(pPlayer, CHAOS_BOLT_1);
    SHADOWFLAME           = AIoncharm.initSpell(pPlayer, SHADOWFLAME_1);
    HELLFIRE              = AIoncharm.initSpell(pPlayer, HELLFIRE_1);
    RAIN_OF_FIRE          = AIoncharm.initSpell(pPlayer, RAIN_OF_FIRE_1);
    SOUL_FIRE             = AIoncharm.initSpell(pPlayer, SOUL_FIRE_1); // soul shard spells
    SHADOWBURN            = AIoncharm.initSpell(pPlayer, SHADOWBURN_1);
    // CURSE
    CURSE_OF_WEAKNESS     = AIoncharm.initSpell(pPlayer, CURSE_OF_WEAKNESS_1);
    CURSE_OF_THE_ELEMENTS = AIoncharm.initSpell(pPlayer, CURSE_OF_THE_ELEMENTS_1);
    CURSE_OF_AGONY        = AIoncharm.initSpell(pPlayer, CURSE_OF_AGONY_1);
    CURSE_OF_EXHAUSTION   = AIoncharm.initSpell(pPlayer, CURSE_OF_EXHAUSTION_1);
    CURSE_OF_TONGUES      = AIoncharm.initSpell(pPlayer, CURSE_OF_TONGUES_1);
    CURSE_OF_DOOM         = AIoncharm.initSpell(pPlayer, CURSE_OF_DOOM_1);
    // AFFLICTION
    CORRUPTION            = AIoncharm.initSpell(pPlayer, CORRUPTION_1);
    DRAIN_SOUL            = AIoncharm.initSpell(pPlayer, DRAIN_SOUL_1);
    DRAIN_LIFE            = AIoncharm.initSpell(pPlayer, DRAIN_LIFE_1);
    DRAIN_MANA            = AIoncharm.initSpell(pPlayer, DRAIN_MANA_1);
    LIFE_TAP              = AIoncharm.initSpell(pPlayer, LIFE_TAP_1);
    UNSTABLE_AFFLICTION   = AIoncharm.initSpell(pPlayer, UNSTABLE_AFFLICTION_1);
    HAUNT                 = AIoncharm.initSpell(pPlayer, HAUNT_1);
    SEED_OF_CORRUPTION    = AIoncharm.initSpell(pPlayer, SEED_OF_CORRUPTION_1);
    DARK_PACT             = AIoncharm.initSpell(pPlayer, DARK_PACT_1);
    HOWL_OF_TERROR        = AIoncharm.initSpell(pPlayer, HOWL_OF_TERROR_1);
    FEAR                  = AIoncharm.initSpell(pPlayer, FEAR_1);
    // DEMONOLOGY
    DEMON_SKIN            = AIoncharm.initSpell(pPlayer, DEMON_SKIN_1);
    DEMON_ARMOR           = AIoncharm.initSpell(pPlayer, DEMON_ARMOR_1);
    DEMONIC_EMPOWERMENT   = AIoncharm.initSpell(pPlayer, DEMONIC_EMPOWERMENT_1);
    FEL_ARMOR             = AIoncharm.initSpell(pPlayer, FEL_ARMOR_1);
    SHADOW_WARD           = AIoncharm.initSpell(pPlayer, SHADOW_WARD_1);
    SOULSHATTER           = AIoncharm.initSpell(pPlayer, SOULSHATTER_1);
    SOUL_LINK             = AIoncharm.initSpell(pPlayer, SOUL_LINK_1);
    SOUL_LINK_AURA        = 25228; // dummy aura applied, after spell SOUL_LINK
    HEALTH_FUNNEL         = AIoncharm.initSpell(pPlayer, HEALTH_FUNNEL_1);
    DETECT_INVISIBILITY   = AIoncharm.initSpell(pPlayer, DETECT_INVISIBILITY_1);
    CREATE_FIRESTONE      = AIoncharm.initSpell(pPlayer, CREATE_FIRESTONE_1);
    CREATE_HEALTHSTONE    = AIoncharm.initSpell(pPlayer, CREATE_HEALTHSTONE_1);
    CREATE_SOULSTONE      = AIoncharm.initSpell(pPlayer, CREATE_SOULSTONE_1);
    // demon summon
    SUMMON_IMP            = AIoncharm.initSpell(pPlayer, SUMMON_IMP_1);
    SUMMON_VOIDWALKER     = AIoncharm.initSpell(pPlayer, SUMMON_VOIDWALKER_1);
    SUMMON_SUCCUBUS       = AIoncharm.initSpell(pPlayer, SUMMON_SUCCUBUS_1);
    SUMMON_FELHUNTER      = AIoncharm.initSpell(pPlayer, SUMMON_FELHUNTER_1);
    SUMMON_FELGUARD       = AIoncharm.initSpell(pPlayer, SUMMON_FELGUARD_1);
}

void PlayerCharmWarlockAI::WarlockAI(Player* pPlayer)
{
	if (pPlayer->getVictim())
	{
		SpellWarlock(pPlayer);
		Unit* pTarget = pPlayer->getVictim();
		Pet *pet = pPlayer->GetPet();
		float dist = pPlayer->GetCombatDistance(pTarget, true);
		// Damage Spells
		switch (SpellSequence)
		{
		case SPELL_CURSES:
			if (CURSE_OF_AGONY && !pTarget->HasAura(CURSE_OF_AGONY) && !pTarget->HasAura(SHADOWFLAME) && LastSpellCurse < 1)
			{
				pPlayer->CastSpell(pTarget, CURSE_OF_AGONY, false);
				SpellSequence = SPELL_AFFLICTION;
				++LastSpellCurse;
				break;
			}
			else if (CURSE_OF_THE_ELEMENTS && !pTarget->HasAura(CURSE_OF_THE_ELEMENTS) && !pTarget->HasAura(SHADOWFLAME) && !pTarget->HasAura(CURSE_OF_AGONY) && !pTarget->HasAura(CURSE_OF_WEAKNESS) && LastSpellCurse < 2)
			{
				pPlayer->CastSpell(pTarget, CURSE_OF_THE_ELEMENTS, false);
				SpellSequence = SPELL_AFFLICTION;
				++LastSpellCurse;
				break;
			}
			else if (CURSE_OF_WEAKNESS && !pTarget->HasAura(CURSE_OF_WEAKNESS) && !pTarget->HasAura(SHADOWFLAME) && !pTarget->HasAura(CURSE_OF_AGONY) && !pTarget->HasAura(CURSE_OF_THE_ELEMENTS) && LastSpellCurse < 3)
			{
				pPlayer->CastSpell(pTarget, CURSE_OF_WEAKNESS, false);
				SpellSequence = SPELL_AFFLICTION;
				++LastSpellCurse;
				break;
			}
			else if (CURSE_OF_TONGUES && !pTarget->HasAura(CURSE_OF_TONGUES) && !pTarget->HasAura(SHADOWFLAME) && !pTarget->HasAura(CURSE_OF_WEAKNESS) && !pTarget->HasAura(CURSE_OF_AGONY) && !pTarget->HasAura(CURSE_OF_THE_ELEMENTS) && LastSpellCurse < 4)
			{
				pPlayer->CastSpell(pTarget, CURSE_OF_TONGUES, false);
				SpellSequence = SPELL_AFFLICTION;
				++LastSpellCurse;
				break;
			}
			LastSpellCurse = 0;
			//SpellSequence = SPELL_AFFLICTION;
			//break;

		case SPELL_AFFLICTION:
			if (LIFE_TAP && LastSpellAffliction < 1 && AIoncharm.GetManaPercent(pPlayer) <= 50 && pPlayer->GetHealthPercent() > 50)
			{
				pPlayer->CastSpell(pPlayer, LIFE_TAP, false);
				SpellSequence = SPELL_DESTRUCTION;
				++LastSpellAffliction;
				break;
			}
			else if (CORRUPTION && !pTarget->HasAura(CORRUPTION) && !pTarget->HasAura(SHADOWFLAME) && !pTarget->HasAura(SEED_OF_CORRUPTION) && LastSpellAffliction < 2)
			{
				pPlayer->CastSpell(pTarget, CORRUPTION, false);
				SpellSequence = SPELL_DESTRUCTION;
				++LastSpellAffliction;
				break;
			}
			else if (DRAIN_SOUL && pTarget->GetHealth() < pTarget->GetMaxHealth() * 0.40 && !pTarget->HasAura(DRAIN_SOUL) && LastSpellAffliction < 3)
			{
				pPlayer->CastSpell(pTarget, DRAIN_SOUL, false);
				//ai->SetIgnoreUpdateTime(15);
				SpellSequence = SPELL_DESTRUCTION;
				++LastSpellAffliction;
				break;
			}
			else if (DRAIN_LIFE && LastSpellAffliction < 4 && !pTarget->HasAura(DRAIN_SOUL) && !pTarget->HasAura(SEED_OF_CORRUPTION) && !pTarget->HasAura(DRAIN_LIFE) && !pTarget->HasAura(DRAIN_MANA) && pPlayer->GetHealthPercent() <= 70)
			{
				pPlayer->CastSpell(pTarget, DRAIN_LIFE, false);
				//ai->SetIgnoreUpdateTime(5);
				SpellSequence = SPELL_DESTRUCTION;
				++LastSpellAffliction;
				break;
			}
			else if (UNSTABLE_AFFLICTION && LastSpellAffliction < 5 && !pTarget->HasAura(UNSTABLE_AFFLICTION) && !pTarget->HasAura(SHADOWFLAME))
			{
				pPlayer->CastSpell(pTarget, UNSTABLE_AFFLICTION, false);
				SpellSequence = SPELL_DESTRUCTION;
				++LastSpellAffliction;
				break;
			}
			else if (HAUNT && LastSpellAffliction < 6 && !pTarget->HasAura(HAUNT))
			{
				pPlayer->CastSpell(pTarget, HAUNT, false);
				SpellSequence = SPELL_DESTRUCTION;
				++LastSpellAffliction;
				break;
			}
			else if (SEED_OF_CORRUPTION && !pTarget->HasAura(SEED_OF_CORRUPTION) && LastSpellAffliction < 7)
			{
				pPlayer->CastSpell(pTarget, SEED_OF_CORRUPTION, false);
				SpellSequence = SPELL_DESTRUCTION;
				++LastSpellAffliction;
				break;
			}
			else if (HOWL_OF_TERROR && !pTarget->HasAura(HOWL_OF_TERROR) && LastSpellAffliction < 8)
			{
				pPlayer->CastSpell(pTarget, HOWL_OF_TERROR, false);
				SpellSequence = SPELL_DESTRUCTION;
				++LastSpellAffliction;
				break;
			}
			else if (FEAR && !pTarget->HasAura(FEAR) && pTarget == pPlayer && LastSpellAffliction < 9)
			{
				pPlayer->CastSpell(pTarget, FEAR, false);
				SpellSequence = SPELL_DESTRUCTION;
				++LastSpellAffliction;
				break;
			}
			else 
			if ((pet) && (DARK_PACT > 0 && AIoncharm.GetManaPercent(pPlayer) <= 50 && LastSpellAffliction < 10 && pet->GetPower(POWER_MANA) > 0))
			{
				pPlayer->CastSpell(pPlayer, DARK_PACT, false);
				SpellSequence = SPELL_DESTRUCTION;
				++LastSpellAffliction;
				break;
			}
			LastSpellAffliction = 0;
			//SpellSequence = SPELL_DESTRUCTION;
			//break;

		case SPELL_DESTRUCTION:
			if (SHADOWFURY && LastSpellDestruction < 1 && !pTarget->HasAura(SHADOWFURY))
			{
				pPlayer->CastSpell(pTarget, SHADOWFURY, false);
				SpellSequence = SPELL_CURSES;
				++LastSpellDestruction;
				break;
			}
			else if (SHADOW_BOLT && LastSpellDestruction < 2)
			{
				pPlayer->CastSpell(pTarget, SHADOW_BOLT, false);
				SpellSequence = SPELL_CURSES;
				++LastSpellDestruction;
				break;
			}
			else if (RAIN_OF_FIRE && LastSpellDestruction < 3)
			{
				pPlayer->CastSpell(pTarget, RAIN_OF_FIRE, false);
				SpellSequence = SPELL_CURSES;
				++LastSpellDestruction;
				break;
			}
			else if (SHADOWFLAME && !pTarget->HasAura(SHADOWFLAME) && LastSpellDestruction < 4)
			{
				pPlayer->CastSpell(pTarget, SHADOWFLAME, false);
				SpellSequence = SPELL_CURSES;
				++LastSpellDestruction;
				break;
			}
			else if (IMMOLATE && !pTarget->HasAura(IMMOLATE) && !pTarget->HasAura(SHADOWFLAME) && LastSpellDestruction < 5)
			{
				pPlayer->CastSpell(pTarget, IMMOLATE, false);
				SpellSequence = SPELL_CURSES;
				++LastSpellDestruction;
				break;
			}
			else if (CONFLAGRATE && LastSpellDestruction < 6)
			{
				pPlayer->CastSpell(pTarget, CONFLAGRATE, false);
				SpellSequence = SPELL_CURSES;
				++LastSpellDestruction;
				break;
			}
			else if (INCINERATE && LastSpellDestruction < 7)
			{
				pPlayer->CastSpell(pTarget, INCINERATE, false);
				SpellSequence = SPELL_CURSES;
				++LastSpellDestruction;
				break;
			}
			else if (SEARING_PAIN && LastSpellDestruction < 8)
			{
				pPlayer->CastSpell(pTarget, SEARING_PAIN, false);
				SpellSequence = SPELL_CURSES;
				++LastSpellDestruction;
				break;
			}
			else if (SOUL_FIRE && LastSpellDestruction < 9)
			{
				pPlayer->CastSpell(pTarget, SOUL_FIRE, false);
				SpellSequence = SPELL_CURSES;
				++LastSpellDestruction;
				break;
			}
			else if (CHAOS_BOLT && LastSpellDestruction < 10)
			{
				pPlayer->CastSpell(pTarget, CHAOS_BOLT, false);
				SpellSequence = SPELL_CURSES;
				++LastSpellDestruction;
				break;
			}
			else if (SHADOWBURN && LastSpellDestruction < 11 && pTarget->GetHealth() < pTarget->GetMaxHealth() * 0.20 && !pTarget->HasAura(SHADOWBURN))
			{
				pPlayer->CastSpell(pTarget, SHADOWBURN, false);
				SpellSequence = SPELL_CURSES;
				++LastSpellDestruction;
				break;
			}
			else if (HELLFIRE && LastSpellDestruction < 12 && !pPlayer->HasAura(HELLFIRE) && pPlayer->GetHealthPercent() >= 50)
			{
				pPlayer->CastSpell(pPlayer, HELLFIRE, false);
				SpellSequence = SPELL_CURSES;
				++LastSpellDestruction;
				break;
			}
			else
			{
				LastSpellDestruction = 0;
				SpellSequence = SPELL_CURSES;
			}
		}
	}
}
