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

#include "OutdoorPvPTS.h"
#include "Player.h"
#include "World.h"

OutdoorPvPTS::OutdoorPvPTS() : OutdoorPvP()
{
}

void OutdoorPvPTS::Update(uint32 diff)
{
	for (GuidZoneMap::const_iterator itr = m_zonePlayers.begin(); itr != m_zonePlayers.end(); ++itr)
	{
		if (!itr->second)
			continue;

		if (Player* player = sObjectMgr.GetPlayer(itr->first))
		{
			if (Group* pGroup = player->GetGroup())
			{
				if (pGroup)
				{
					uint8 m_sy1 = 0;
					for (GroupReference* itr = pGroup->GetFirstMember(); itr != NULL; itr = itr->next())
					{
						Player* pPlayer = itr->getSource();
						if (pPlayer->IsInWorld())
						{
							if (pPlayer->GetMapId() == MAP_ID_TX)
							{
								++m_sy1;
								if (m_sy1 > 10)
									pPlayer->TeleportToHomebind();
							}
						}
					}
				}
			}
		}
	}
}