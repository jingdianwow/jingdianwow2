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

#include "Player.h"
#include "Spell.h"
#include "AutoAIoncharm.h"
#include "SpellMgr.h"
#include "PlayerCharmMageAI.h"
#include "PlayerCharmPriestAI.h"
#include "PlayerCharmWarlockAI.h"
#include "PlayerCharmWarriorAI.h"
#include "PlayerCharmShamanAI.h"
#include "PlayerCharmPaladinAI.h"
#include "PlayerCharmRogueAI.h"
#include "PlayerCharmDruidAI.h"
#include "PlayerCharmHunterAI.h"

AutoAIoncharm::AutoAIoncharm() : m_PlayerUpdateTime(3000)
{
}

void AutoAIoncharm::UpdateAI(Player* pPlayer, Unit* owner, uint32 p_time)
{
	if (!owner || !pPlayer || owner->isDead() || pPlayer->isDead())
		return;

	if (pPlayer->HasAuraType(SPELL_AURA_MOD_CONFUSE) || pPlayer->HasAuraType(SPELL_AURA_MOD_FEAR) || pPlayer->HasAuraType(SPELL_AURA_MOD_STUN))
		return;

	if (m_PlayerUpdateTime < p_time)
	{
		switch (pPlayer->getClass())
		{
		    case CLASS_PRIEST:
				Priest.PriestAI(pPlayer);
			    break;
		    case CLASS_MAGE:
				Mage.MageAI(pPlayer);
			    break;
		    case CLASS_WARLOCK:
				Warlock.WarlockAI(pPlayer);
			    break;
		    case CLASS_WARRIOR:
				Warrior.WarriorAI(pPlayer);
			    break;
		    case CLASS_SHAMAN:
				Shaman.ShamanAI(pPlayer);
			    break;
		    case CLASS_PALADIN:
				Paladin.PaladinAI(pPlayer);
			    break;
		    case CLASS_ROGUE:
				Rogue.RogueAI(pPlayer);
			    break;
		    case CLASS_DRUID:
				Druid.DruidAI(pPlayer);
			    break;
		    case CLASS_HUNTER:
				Hunter.HunterAI(pPlayer);
			    break;
		}
		m_PlayerUpdateTime = 3000;
	}
	else
		m_PlayerUpdateTime -= p_time;
}

uint32 AutoAIoncharm::initSpell(Player* pPlayer, uint32 spellId)
{
	// Check if bot knows this spell
	if (!pPlayer->HasSpell(spellId))
		return 0;

	uint32 next = 0;
	SpellChainMapNext const& nextMap = sSpellMgr.GetSpellChainNext();
	for (SpellChainMapNext::const_iterator itr = nextMap.lower_bound(spellId); itr != nextMap.upper_bound(spellId); ++itr)
	{
		// Work around buggy chains
		if (sSpellStore.LookupEntry(spellId)->SpellIconID != sSpellStore.LookupEntry(itr->second)->SpellIconID)
			continue;

		SpellChainNode const* node = sSpellMgr.GetSpellChainNode(itr->second);
		// If next spell is a requirement for this one then skip it
		if (node->req == spellId)
			continue;
		if (node->prev == spellId)
		{
			next = initSpell(pPlayer, itr->second);
			break;
		}
	}
	if (next == 0)
	{
		const SpellEntry* const pSpellInfo = sSpellStore.LookupEntry(spellId);
		// Add spell to spellrange map
		Spell *spell = new Spell(pPlayer, pSpellInfo, false);
		SpellRangeEntry const* srange = sSpellRangeStore.LookupEntry(pSpellInfo->rangeIndex);
		float range = GetSpellMaxRange(srange);
		pPlayer->ApplySpellMod(spellId, SPELLMOD_RANGE, range, spell);
		m_spellRangeMap.insert(std::pair<uint32, float>(spellId, range));
		delete spell;
	}
	return (next == 0) ? spellId : next;
}