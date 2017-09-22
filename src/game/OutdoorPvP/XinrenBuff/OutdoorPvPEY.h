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
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#ifndef WORLD_PVP_EY
#define WORLD_PVP_EY

#include "Common.h"
#include "../OutdoorPvP.h"
#include "Language.h"

enum
{
	NPC_EYUNJINGJICHANG1 = 11498,
	NPC_EYUNJINGJICHANG2 = 11447,

	GO_JINGJICHANGMEN = 179469,
};

class OutdoorPvPEY : public OutdoorPvP
{
public:
	OutdoorPvPEY();
	void HandleCreatureEnterCombat(Creature* pCreature) override;
	void HandleCreatureDeath(Creature* pCreature) override;
	void HandleGameObjectCreate(GameObject* go) override;
private:
	ObjectGuid m_jingjichangmen;
};

#endif