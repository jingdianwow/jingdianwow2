#include "Player.h"


enum
{
    SPELL_CURSES,
    SPELL_AFFLICTION,
    SPELL_DESTRUCTION,
    SPELL_DEMONOLOGY
};

enum WarlockSpells
{
    BANISH_1                        = 710,
    CHALLENGING_HOWL_1              = 59671,
    CHAOS_BOLT_1                    = 50796,
    CONFLAGRATE_1                   = 17962,
    CORRUPTION_1                    = 172,
    CREATE_FIRESTONE_1              = 6366,
    CREATE_HEALTHSTONE_1            = 6201,
    CREATE_SOULSTONE_1              = 693,
    CREATE_SPELLSTONE_1             = 2362,
    CURSE_OF_AGONY_1                = 980,
    CURSE_OF_DOOM_1                 = 603,
    CURSE_OF_EXHAUSTION_1           = 18223,
    CURSE_OF_THE_ELEMENTS_1         = 1490,
    CURSE_OF_TONGUES_1              = 1714,
    CURSE_OF_WEAKNESS_1             = 702,
    DARK_PACT_1                     = 18220,
    DEATH_COIL_WARLOCK_1            = 6789,
    DEMON_ARMOR_1                   = 706,
    DEMON_CHARGE_1                  = 54785,
    DEMON_SKIN_1                    = 687,
    DEMONIC_CIRCLE_SUMMON_1         = 48018,
    DEMONIC_CIRCLE_TELEPORT_1       = 48020,
    DEMONIC_EMPOWERMENT_1           = 47193,
    DEMONIC_IMMOLATE_1              = 75445,
    DETECT_INVISIBILITY_1           = 132,
    DRAIN_LIFE_1                    = 689,
    DRAIN_MANA_1                    = 5138,
    DRAIN_SOUL_1                    = 1120,
    ENSLAVE_DEMON_1                 = 1098,
    EYE_OF_KILROGG_1                = 126,
    FEAR_1                          = 5782,
    FEL_ARMOR_1                     = 28176,
    FEL_DOMINATION_1                = 18708,
    HAUNT_1                         = 48181,
    HEALTH_FUNNEL_1                 = 755,
    HELLFIRE_1                      = 1949,
    HOWL_OF_TERROR_1                = 5484,
    IMMOLATE_1                      = 348,
    IMMOLATION_AURA_1               = 50589,
    INCINERATE_1                    = 29722,
    INFERNO_1                       = 1122,
    LIFE_TAP_1                      = 1454,
    METAMORPHOSIS_1                 = 59672,
    RAIN_OF_FIRE_1                  = 5740,
    RITUAL_OF_DOOM_1                = 18540,
    RITUAL_OF_SOULS_1               = 29893,
    RITUAL_OF_SUMMONING_1           = 698,
    SEARING_PAIN_1                  = 5676,
    SEED_OF_CORRUPTION_1            = 27243,
    SENSE_DEMONS_1                  = 5500,
    SHADOW_BOLT_1                   = 686,
    SHADOW_CLEAVE_1                 = 50581,
    SHADOW_WARD_1                   = 6229,
    SHADOWBURN_1                    = 17877,
    SHADOWFLAME_1                   = 47897,
    SHADOWFURY_1                    = 30283,
    SOUL_FIRE_1                     = 6353,
    SOUL_LINK_1                     = 19028,
    SOULSHATTER_1                   = 29858,
    SUMMON_FELGUARD_1               = 30146,
    SUMMON_FELHUNTER_1              = 691,
    SUMMON_IMP_1                    = 688,
    SUMMON_SUCCUBUS_1               = 712,
    SUMMON_VOIDWALKER_1             = 697,
    UNENDING_BREATH_1               = 5697,
    UNSTABLE_AFFLICTION_1           = 30108
};

//class Player;
class MANGOS_DLL_SPEC PlayerCharmWarlockAI
{
    public:
		PlayerCharmWarlockAI();
		void WarlockAI(Player* pPlayer);
		void SpellWarlock(Player* pPlayer);

private:

    // CURSES
    uint32 CURSE_OF_WEAKNESS,
           CURSE_OF_AGONY,
           CURSE_OF_EXHAUSTION,
           CURSE_OF_TONGUES,
           CURSE_OF_THE_ELEMENTS,
           CURSE_OF_DOOM;

    // AFFLICTION
    uint32 CORRUPTION,
           DRAIN_SOUL,
           DRAIN_LIFE,
           DRAIN_MANA,
           LIFE_TAP,
           UNSTABLE_AFFLICTION,
           HAUNT,
           SEED_OF_CORRUPTION,
           DARK_PACT,
           HOWL_OF_TERROR,
           FEAR;

    // DESTRUCTION
    uint32 SHADOW_BOLT,
           IMMOLATE,
           INCINERATE,
           SEARING_PAIN,
           CONFLAGRATE,
           SOUL_FIRE,
           SHADOWFURY,
           CHAOS_BOLT,
           SHADOWFLAME,
           HELLFIRE,
           RAIN_OF_FIRE,
           SHADOWBURN;

    // DEMONOLOGY
    uint32 DEMON_SKIN,
           DEMON_ARMOR,
           DEMONIC_EMPOWERMENT,
           SHADOW_WARD,
           FEL_ARMOR,
           SOULSHATTER,
           SOUL_LINK,
           SOUL_LINK_AURA,
           HEALTH_FUNNEL,
           DETECT_INVISIBILITY,
           CREATE_FIRESTONE,
           CREATE_SOULSTONE,
           CREATE_HEALTHSTONE;

    // DEMON SUMMON
    uint32 SUMMON_IMP,
           SUMMON_VOIDWALKER,
           SUMMON_SUCCUBUS,
           SUMMON_FELHUNTER,
           SUMMON_FELGUARD;

    // DEMON SKILLS
    uint32 BLOOD_PACT,
           FIREBOLT,
           FIRE_SHIELD,
           ANGUISH,
           CLEAVE,
           INTERCEPT,
           DEVOUR_MAGIC,
           FEL_INTELLIGENCE,
           SHADOW_BITE,
           SPELL_LOCK,
           LASH_OF_PAIN,
           SEDUCTION,
           SOOTHING_KISS,
           CONSUME_SHADOWS,
           SACRIFICE,
           SUFFERING,
           TORMENT;

    // first aid
    uint32 RECENTLY_BANDAGED;

    // racial
    uint32 ARCANE_TORRENT,
           GIFT_OF_THE_NAARU,
           STONEFORM,
           ESCAPE_ARTIST,
           EVERY_MAN_FOR_HIMSELF,
           SHADOWMELD,
           BLOOD_FURY,
           WAR_STOMP,
           BERSERKING,
           WILL_OF_THE_FORSAKEN;

    uint32 SpellSequence,
           LastSpellCurse,
           LastSpellAffliction,
           LastSpellDestruction;
};

#define Warlock MaNGOS::Singleton<PlayerCharmWarlockAI>::Instance()
