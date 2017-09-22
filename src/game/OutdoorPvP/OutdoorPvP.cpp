/*
 * This file is part of the CMaNGOS Project. See AUTHORS file for Copyright information
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

#include "OutdoorPvP.h"
#include "Language.h"
#include "World.h"
#include "ObjectMgr.h"
#include "Object.h"
#include "GameObject.h"
#include "Player.h"

/**
   Function that adds a player to the players of the affected outdoor pvp zones

   @param   player to add
   @param   whether zone is main outdoor pvp zone or a affected zone
 */
void OutdoorPvP::HandlePlayerEnterZone(Player* player, bool isMainZone)
{
    m_zonePlayers[player->GetObjectGuid()] = isMainZone;
}

/**
   Function that removes a player from the players of the affected outdoor pvp zones

   @param   player to remove
   @param   whether zone is main outdoor pvp zone or a affected zone
 */
void OutdoorPvP::HandlePlayerLeaveZone(Player* player, bool isMainZone)
{
    if (m_zonePlayers.erase(player->GetObjectGuid()))
    {
        // remove the world state information from the player
        if (isMainZone && !player->GetSession()->PlayerLogout())
            SendRemoveWorldStates(player);

        sLog.outDebug("Player %s left an Outdoor PvP zone", player->GetName());
    }
}

/**
   Function that updates the world state for all the players of the outdoor pvp zone

   @param   world state to update
   @param   new world state value
 */
void OutdoorPvP::SendUpdateWorldState(uint32 field, uint32 value)
{
    for (GuidZoneMap::const_iterator itr = m_zonePlayers.begin(); itr != m_zonePlayers.end(); ++itr)
    {
        // only send world state update to main zone
        if (!itr->second)
            continue;

        if (Player* player = sObjectMgr.GetPlayer(itr->first))
            player->SendUpdateWorldState(field, value);
    }
}

void OutdoorPvP::HandleGameObjectCreate(GameObject* go)
{
	QueryResult* result = CharacterDatabase.PQuery("SELECT ta1, ta2, ta3, ta4, worldstate1, worldstate2, worldstate3, worldstate4, go1, go2, go3, go4, eventEntry1, eventEntry2, eventEntry3, eventEntry4 FROM outdoorpvpta WHERE id = %u ", 1);
	if (!result)
	{
    // set initial data and activate capture points
        if (go->GetGOInfo()->type == GAMEOBJECT_TYPE_CAPTURE_POINT)
        {
            CapturePointSliderMap const* capturePoints = sOutdoorPvPMgr.GetCapturePointSliderMap();
            CapturePointSliderMap::const_iterator itr = capturePoints->find(go->GetEntry());
            if (itr != capturePoints->end())
                go->SetCapturePointSlider(itr->second.Value, itr->second.IsLocked);
            else
                go->SetCapturePointSlider(CAPTURE_SLIDER_MIDDLE, false);
		}
	}
	else
	{
		Field *fields = result->Fetch();
		uint32 ta1 = fields[0].GetUInt32();
		uint32 ta2 = fields[1].GetUInt32();
		uint32 ta3 = fields[2].GetUInt32();
		uint32 ta4 = fields[3].GetUInt32();
		uint32 worldstate1 = fields[4].GetUInt32();
		uint32 worldstate2 = fields[5].GetUInt32();
		uint32 worldstate3 = fields[6].GetUInt32();
		uint32 worldstate4 = fields[7].GetUInt32();
		uint32 go1 = fields[8].GetUInt32();
		uint32 go2 = fields[9].GetUInt32();
		uint32 go3 = fields[10].GetUInt32();
		uint32 go4 = fields[11].GetUInt32();
		uint32 eventEntry1 = fields[12].GetUInt32();
		uint32 eventEntry2 = fields[13].GetUInt32();
		uint32 eventEntry3 = fields[14].GetUInt32();
		uint32 eventEntry4 = fields[15].GetUInt32();

		if (ta1 == HORDE && worldstate1 == 2373 && go->GetEntry() == go1 && eventEntry1 == 10698)
		{
		   if (go->GetGOInfo()->type == GAMEOBJECT_TYPE_CAPTURE_POINT)
		   {
			   CapturePointSliderMap const* capturePoints = sOutdoorPvPMgr.GetCapturePointSliderMap();
               CapturePointSliderMap::const_iterator itr = capturePoints->find(go1);
               if (itr != capturePoints->end())
                   go->SetCapturePointSlider(itr->second.Value, itr->second.IsLocked);
               else
                   go->SetCapturePointSlider(CAPTURE_SLIDER_HORDE, false);
		   }
		}
		else
		if (ta1 == ALLIANCE && worldstate1 == 2372 && go->GetEntry() == go1  && eventEntry1 == 10699)
		{
		   if (go->GetGOInfo()->type == GAMEOBJECT_TYPE_CAPTURE_POINT)
		   {
			   CapturePointSliderMap const* capturePoints = sOutdoorPvPMgr.GetCapturePointSliderMap();
               CapturePointSliderMap::const_iterator itr = capturePoints->find(go1);
               if (itr != capturePoints->end())
                   go->SetCapturePointSlider(itr->second.Value, itr->second.IsLocked);
               else
                   go->SetCapturePointSlider(CAPTURE_SLIDER_ALLIANCE, false);
		   }
		}
		else
		if (ta2 == HORDE && worldstate2 == 2379 && go->GetEntry() == go2 && eventEntry2 == 10704)
		{
		   if (go->GetGOInfo()->type == GAMEOBJECT_TYPE_CAPTURE_POINT)
		   {
			   CapturePointSliderMap const* capturePoints = sOutdoorPvPMgr.GetCapturePointSliderMap();
               CapturePointSliderMap::const_iterator itr = capturePoints->find(go2);
               if (itr != capturePoints->end())
                   go->SetCapturePointSlider(itr->second.Value, itr->second.IsLocked);
               else
                   go->SetCapturePointSlider(CAPTURE_SLIDER_HORDE, false);
		   }
		}
		else
		if (ta2 == ALLIANCE && worldstate2 == 2378 && go->GetEntry() == go2  && eventEntry2 == 10705)
		{
		   if (go->GetGOInfo()->type == GAMEOBJECT_TYPE_CAPTURE_POINT)
		   {
			   CapturePointSliderMap const* capturePoints = sOutdoorPvPMgr.GetCapturePointSliderMap();
               CapturePointSliderMap::const_iterator itr = capturePoints->find(go2);
               if (itr != capturePoints->end())
                   go->SetCapturePointSlider(itr->second.Value, itr->second.IsLocked);
               else
                   go->SetCapturePointSlider(CAPTURE_SLIDER_ALLIANCE, false);
		   }
		}
		else
		if (ta3 == HORDE && worldstate3 == 2356 && go->GetEntry() == go3 && eventEntry3 == 10692)
		{
		   if (go->GetGOInfo()->type == GAMEOBJECT_TYPE_CAPTURE_POINT)
		   {
			   CapturePointSliderMap const* capturePoints = sOutdoorPvPMgr.GetCapturePointSliderMap();
               CapturePointSliderMap::const_iterator itr = capturePoints->find(go3);
               if (itr != capturePoints->end())
                   go->SetCapturePointSlider(itr->second.Value, itr->second.IsLocked);
               else
                   go->SetCapturePointSlider(CAPTURE_SLIDER_HORDE, false);
		   }
		}
		else
		if (ta3 == ALLIANCE && worldstate3 == 2354 && go->GetEntry() == go3  && eventEntry3 == 10691)
		{
		   if (go->GetGOInfo()->type == GAMEOBJECT_TYPE_CAPTURE_POINT)
		   {
			   CapturePointSliderMap const* capturePoints = sOutdoorPvPMgr.GetCapturePointSliderMap();
               CapturePointSliderMap::const_iterator itr = capturePoints->find(go3);
               if (itr != capturePoints->end())
                   go->SetCapturePointSlider(itr->second.Value, itr->second.IsLocked);
               else
                   go->SetCapturePointSlider(CAPTURE_SLIDER_ALLIANCE, false);
		   }
		}
		else
		if (ta4 == HORDE && worldstate4 == 2371 && go->GetEntry() == go4 && eventEntry4 == 10700)
		{
		   if (go->GetGOInfo()->type == GAMEOBJECT_TYPE_CAPTURE_POINT)
		   {
			   CapturePointSliderMap const* capturePoints = sOutdoorPvPMgr.GetCapturePointSliderMap();
               CapturePointSliderMap::const_iterator itr = capturePoints->find(go4);
               if (itr != capturePoints->end())
                   go->SetCapturePointSlider(itr->second.Value, itr->second.IsLocked);
               else
                   go->SetCapturePointSlider(CAPTURE_SLIDER_HORDE, false);
		   }
		}
		else
		if (ta4 == ALLIANCE && worldstate4 == 2370 && go->GetEntry() == go4  && eventEntry4 == 10701)
		{
		   if (go->GetGOInfo()->type == GAMEOBJECT_TYPE_CAPTURE_POINT)
		   {
			   CapturePointSliderMap const* capturePoints = sOutdoorPvPMgr.GetCapturePointSliderMap();
               CapturePointSliderMap::const_iterator itr = capturePoints->find(go4);
               if (itr != capturePoints->end())
                   go->SetCapturePointSlider(itr->second.Value, itr->second.IsLocked);
               else
                   go->SetCapturePointSlider(CAPTURE_SLIDER_ALLIANCE, false);
		   }
		}
		else
		{
		   if (go->GetGOInfo()->type == GAMEOBJECT_TYPE_CAPTURE_POINT)
		   {
			   CapturePointSliderMap const* capturePoints = sOutdoorPvPMgr.GetCapturePointSliderMap();
               CapturePointSliderMap::const_iterator itr = capturePoints->find(go->GetEntry());
               if (itr != capturePoints->end())
                   go->SetCapturePointSlider(itr->second.Value, itr->second.IsLocked);
               else
                   go->SetCapturePointSlider(CAPTURE_SLIDER_MIDDLE, false);
		   }
		}
	}
}

void OutdoorPvP::HandleGameObjectRemove(GameObject* go)
{
    // save capture point slider value (negative value if locked)
    if (go->GetGOInfo()->type == GAMEOBJECT_TYPE_CAPTURE_POINT)
    {
        CapturePointSlider value(go->GetCapturePointSliderValue(), go->getLootState() != GO_ACTIVATED);
        sOutdoorPvPMgr.SetCapturePointSlider(go->GetEntry(), value);
    }
}

/**
   Function that handles player kills in the main outdoor pvp zones

   @param   player who killed another player
   @param   victim who was killed
 */
void OutdoorPvP::HandlePlayerKill(Player* killer, Player* victim)
{
    if (Group* group = killer->GetGroup())
    {
        for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
        {
            Player* groupMember = itr->getSource();

            if (!groupMember)
                continue;

            // skip if too far away
            if (!groupMember->IsAtGroupRewardDistance(victim))
                continue;

            // creature kills must be notified, even if not inside objective / not outdoor pvp active
            // player kills only count if active and inside objective
            if (groupMember->CanUseCapturePoint())
                HandlePlayerKillInsideArea(groupMember);
        }
    }
    else
    {
        // creature kills must be notified, even if not inside objective / not outdoor pvp active
        if (killer && killer->CanUseCapturePoint())
            HandlePlayerKillInsideArea(killer);
    }
}

// apply a team buff for the main and affected zones
void OutdoorPvP::BuffTeam(Team team, uint32 spellId, bool remove /*= false*/)
{
    for (GuidZoneMap::const_iterator itr = m_zonePlayers.begin(); itr != m_zonePlayers.end(); ++itr)
    {
        Player* player = sObjectMgr.GetPlayer(itr->first);
        if (player && player->GetTeam() == team)
        {
            if (remove)
                player->RemoveAurasDueToSpell(spellId);
            else
                player->CastSpell(player, spellId, true);
        }
    }
}

uint32 OutdoorPvP::GetBannerArtKit(Team team, uint32 artKitAlliance /*= CAPTURE_ARTKIT_ALLIANCE*/, uint32 artKitHorde /*= CAPTURE_ARTKIT_HORDE*/, uint32 artKitNeutral /*= CAPTURE_ARTKIT_NEUTRAL*/)
{
    switch (team)
    {
        case ALLIANCE:
            return artKitAlliance;
        case HORDE:
            return artKitHorde;
        default:
            return artKitNeutral;
    }
}

void OutdoorPvP::SetBannerVisual(const WorldObject* objRef, ObjectGuid goGuid, uint32 artKit, uint32 animId)
{
    if (GameObject* go = objRef->GetMap()->GetGameObject(goGuid))
        SetBannerVisual(go, artKit, animId);
}

void OutdoorPvP::SetBannerVisual(GameObject* go, uint32 artKit, uint32 animId)
{
    go->SendGameObjectCustomAnim(go->GetObjectGuid(), animId);
    go->SetGoArtKit(artKit);
    go->Refresh();
}

void OutdoorPvP::RespawnGO(const WorldObject* objRef, ObjectGuid goGuid, bool respawn)
{
    if (GameObject* go = objRef->GetMap()->GetGameObject(goGuid))
    {
        go->SetRespawnTime(7 * DAY);

        if (respawn)
            go->Refresh();
        else if (go->isSpawned())
            go->SetLootState(GO_JUST_DEACTIVATED);
    }
}

void OutdoorPvP::RespawnCreaturea(const WorldObject* objRef, ObjectGuid goGuid)
{
    if (GameObject* go = objRef->GetMap()->GetGameObject(goGuid))
    {
		for (uint8 i = 0; i < 1; ++i)
             go->SummonCreature(SpawnLocsa[i].NpcEntry, SpawnLocsa[i].m_fX, SpawnLocsa[i].m_fY, SpawnLocsa[i].m_fZ, SpawnLocsa[i].m_fO, TEMPSUMMON_DEAD_DESPAWN, 0);
    }
}

void OutdoorPvP::RespawnCreatureb(const WorldObject* objRef, ObjectGuid goGuid)
{
    if (GameObject* go = objRef->GetMap()->GetGameObject(goGuid))
    {
		for (uint8 i = 0; i < 1; ++i)
             go->SummonCreature(SpawnLocsb[i].NpcEntry, SpawnLocsb[i].m_fX, SpawnLocsb[i].m_fY, SpawnLocsb[i].m_fZ, SpawnLocsb[i].m_fO, TEMPSUMMON_DEAD_DESPAWN, 0);
    }
}

void OutdoorPvP::RespawnCreaturec(const WorldObject* objRef, ObjectGuid goGuid)
{
    if (GameObject* go = objRef->GetMap()->GetGameObject(goGuid))
    {
		for (uint8 i = 0; i < 1; ++i)
             go->SummonCreature(SpawnLocsc[i].NpcEntry, SpawnLocsc[i].m_fX, SpawnLocsc[i].m_fY, SpawnLocsc[i].m_fZ, SpawnLocsc[i].m_fO, TEMPSUMMON_DEAD_DESPAWN, 0);
    }
}

void OutdoorPvP::RespawnCreatured(const WorldObject* objRef, ObjectGuid goGuid)
{
    if (GameObject* go = objRef->GetMap()->GetGameObject(goGuid))
    {
		for (uint8 i = 0; i < 2; ++i)
             go->SummonCreature(SpawnLocsd[i].NpcEntry, SpawnLocsd[i].m_fX, SpawnLocsd[i].m_fY, SpawnLocsd[i].m_fZ, SpawnLocsd[i].m_fO, TEMPSUMMON_DEAD_DESPAWN, 0);
    }
}

void OutdoorPvP::RespawnDespawn(const WorldObject* objRef, GuidList npcEntry)
{

	for (GuidList::const_iterator itr = npcEntry.begin(); itr != npcEntry.end(); ++itr)
    {
         if (Creature* soldier = objRef->GetMap()->GetCreature(*itr))
             soldier->ForcedDespawn();
    }
    npcEntry.clear();
}

void OutdoorPvP::AddJdItem(Player* player, uint32 ItemId, uint32 cont)
{
	 uint32 noSpaceForCount = 0;
     ItemPosCountVec dest;
     uint8 msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, ItemId, cont, &noSpaceForCount);

     if (msg != EQUIP_ERR_OK)
         cont -= noSpaceForCount;

	 if (cont == 0 || dest.empty())
		 return;

	 Item* item = player->StoreNewItem(dest, ItemId, true, Item::GenerateItemRandomPropertyId(ItemId));
	 player->SendNewItem(item, cont, true, false);
}

bool OutdoorPvP::GetAreaTriggerLockStatus(Player* player)
{
	uint32 miscRequirement = 0;
    AreaLockStatus lockStatus = AREA_LOCKSTATUS_OK;
    if (AreaTrigger const* at = sObjectMgr.GetMapEntranceTrigger(player->GetMapId()))
	{
        lockStatus = player->GetAreaTriggerLockStatus(at, miscRequirement);
		if (lockStatus == AREA_LOCKSTATUS_OK || lockStatus == AREA_LOCKSTATUS_INSTANCE_IS_FULL)
			return false;
	}
	return true;
}
