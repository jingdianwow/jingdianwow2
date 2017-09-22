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
#include "player.h"

typedef std::map<uint32, float> SpellRanges;
class MANGOS_DLL_SPEC AutoAIoncharm
{
    public:
		AutoAIoncharm();

		void UpdateAI(Player* pPlayer, Unit* owner, uint32 p_time);
		uint32 initSpell(Player* pPlayer, uint32 spellId);
		float GetManaPercent(Unit* pTarget){ return (static_cast<float> (pTarget->GetPower(POWER_MANA)) / pTarget->GetMaxPower(POWER_MANA)) * 100; }
		float GetRageAmount(Unit* pTarget){ return (static_cast<float> (pTarget->GetPower(POWER_RAGE))); }
		float GetEnergyAmount(Unit* pTarget){ return (static_cast<float> (pTarget->GetPower(POWER_ENERGY))); }

    private:
		SpellRanges m_spellRangeMap;
		uint32 m_PlayerUpdateTime;
};

#define AIoncharm MaNGOS::Singleton<AutoAIoncharm>::Instance()