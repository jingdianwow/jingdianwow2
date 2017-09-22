/**
 * This code is part of MaNGOS. Contributor & Copyright details are in AUTHORS/THANKS.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "Mod.h"
#include "../Spell.h"
#include "../SpellMgr.h"
#include "../SpellAuras.h"


void ModClass::spellPrepare(Spell* spell, Unit* caster)
{
    // nothing to do for now
}

void ModClass::spellEffect(Spell* spell, uint8 eff, SpellEffectIndex i)
{
    // nothing to do for now
}

void ModClass::auraApplyModifier(Aura* aura, AuraType aType, bool apply, bool real)
{
    // nothing to do for now
}

void ModClass::applyDiminishingToDuration(Unit* unit, Unit* caster, int32& duration, DiminishingGroup group)
{
    // [MOD] Duration of crowd control abilities on pvp target is limited by 10 sec. (after patch 2.2.0)
    if (duration > 15 * IN_MILLISECONDS && IsDiminishingReturnsGroupDurationLimited(group))
    {
        // test pet/charm masters instead pets/charmeds
        Unit const* targetOwner = unit->GetCharmerOrOwner();
        Unit const* casterOwner = caster->GetCharmerOrOwner();

        Unit const* target = targetOwner ? targetOwner : unit;
        Unit const* source = casterOwner ? casterOwner : caster;

        if (target->GetTypeId() == TYPEID_PLAYER && source->GetTypeId() == TYPEID_PLAYER)
            duration = 15000;
    }
}

void ModClass::getSpellCastTime(const SpellEntry* spellInfo, const Spell* spell, int32& castTime)
{
    // [workaround] holy light need script effect, but 19968 spell for it have 2.5 cast time sec
    // it should be instant instead
    //if (spellInfo->Id == 19968 || spellInfo->Id == 39001 || spellInfo->Id == 39002 || spellInfo->Id == 39003 || spellInfo->Id == 39004 || spellInfo->Id == 39005 || spellInfo->Id == 39006 || spellInfo->Id == 39007 || spellInfo->Id == 39008)
        //castTime = 0;
	if (spellInfo->Id == 25863)
		castTime = 3000;

	if (spell)
	{
		if (spellInfo->Effect[EFFECT_INDEX_0] == SPELL_EFFECT_CREATE_ITEM)
		{
			if (Player* modOwner = spell->GetCaster()->GetSpellModOwner())
			{
				if (modOwner->GetPlayerCdSuoDuanLevel() == 1)
				    castTime = 1000;
			}
		}
	}
}
