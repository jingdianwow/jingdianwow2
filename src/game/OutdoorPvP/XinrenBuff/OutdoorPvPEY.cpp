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

#include "OutdoorPvPEY.h"
#include "Player.h"
#include "World.h"

OutdoorPvPEY::OutdoorPvPEY() : OutdoorPvP()
{
}

void OutdoorPvPEY::HandleCreatureEnterCombat(Creature* pCreature)
{
	switch (pCreature->GetEntry())
	{
	case NPC_EYUNJINGJICHANG1:
		sWorld.SendWorldText(LANG_GONGGAO_19, pCreature->GetName());
		if (GameObject* go = pCreature->GetMap()->GetGameObject(m_jingjichangmen))
		if (go->GetGoState() == 0)
			go->SetGoState(GO_STATE_READY);
		break;
	case NPC_EYUNJINGJICHANG2:
		sWorld.SendWorldText(LANG_GONGGAO_20, pCreature->GetName());
		if (GameObject* go = pCreature->GetMap()->GetGameObject(m_jingjichangmen))
		if (go->GetGoState() == 0)
			go->SetGoState(GO_STATE_READY);
		break;
	}
}

void OutdoorPvPEY::HandleCreatureDeath(Creature* pCreature)
{
	switch (pCreature->GetEntry())
	{
	case NPC_EYUNJINGJICHANG1:
		sWorld.SendWorldText(LANG_GONGGAO_19, pCreature->GetName());
		if (GameObject* go = pCreature->GetMap()->GetGameObject(m_jingjichangmen))
		if (go->GetGoState() == 1)
			go->SetGoState(GO_STATE_ACTIVE);
		break;
	case NPC_EYUNJINGJICHANG2:
		sWorld.SendWorldText(LANG_GONGGAO_20, pCreature->GetName());
		if (GameObject* go = pCreature->GetMap()->GetGameObject(m_jingjichangmen))
		if (go->GetGoState() == 1)
			go->SetGoState(GO_STATE_ACTIVE);
		break;
	}
}

void OutdoorPvPEY::HandleGameObjectCreate(GameObject* go)
{
	switch (go->GetEntry())
	{
	case GO_JINGJICHANGMEN:
		m_jingjichangmen = go->GetObjectGuid();
		break;
	}
}