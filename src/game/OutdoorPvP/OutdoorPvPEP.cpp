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

#include "OutdoorPvPEP.h"
#include "WorldPacket.h"
#include "World.h"
#include "ObjectMgr.h"
#include "Object.h"
#include "Creature.h"
#include "GameObject.h"
#include "Player.h"
#include "Chat.h"

OutdoorPvPEP::OutdoorPvPEP() : OutdoorPvP(),
	outdoorpvpid(1)
{
	// initially set graveyard owner to neither faction
    sObjectMgr.SetGraveYardLinkTeam(GRAVEYARD_ID_EASTERN_PLAGUE, GRAVEYARD_ZONE_EASTERN_PLAGUE, TEAM_INVALID);
	QueryResult* result = CharacterDatabase.PQuery("SELECT horde, aliance, ta1, ta2, ta3, ta4, worldstate1, worldstate2, worldstate3, worldstate4 FROM outdoorpvpta WHERE id = %u ", outdoorpvpid);
	if (!result)
	{
		m_towerWorldState[0] = WORLD_STATE_EP_NORTHPASS_NEUTRAL;
        m_towerWorldState[1] = WORLD_STATE_EP_CROWNGUARD_NEUTRAL;
        m_towerWorldState[2] = WORLD_STATE_EP_EASTWALL_NEUTRAL;
        m_towerWorldState[3] = WORLD_STATE_EP_PLAGUEWOOD_NEUTRAL;

        for (uint8 i = 0; i < MAX_EP_TOWERS; ++i)
             m_towerOwner[i] = TEAM_NONE;
	}
	else
	{
		Field *fields = result->Fetch();
		uint32 horde = fields[0].GetUInt32();
		uint32 aliance = fields[1].GetUInt32();
		uint32 ta1 = fields[2].GetUInt32();
		uint32 ta2 = fields[3].GetUInt32();
		uint32 ta3 = fields[4].GetUInt32();
		uint32 ta4 = fields[5].GetUInt32();
		uint32 worldstate1 = fields[6].GetUInt32();
		uint32 worldstate2 = fields[7].GetUInt32();
		uint32 worldstate3 = fields[8].GetUInt32();
		uint32 worldstate4 = fields[9].GetUInt32();
		m_towersAlliance = aliance;
	    m_towersHorde = horde;

		if (ta1 == HORDE)
		{
		    m_towerOwner[0] = HORDE;
			m_towerWorldState[0] = worldstate1;
		}
		else
		if (ta1 == ALLIANCE)
		{
			m_towerOwner[0] = ALLIANCE;
			m_towerWorldState[0] = worldstate1;
		}
		else
		{
			m_towerOwner[0] = TEAM_NONE;
			m_towerWorldState[0] = WORLD_STATE_EP_NORTHPASS_NEUTRAL;
		}
		
		if (ta2 == HORDE)
		{
		    m_towerOwner[1] = HORDE;
			m_towerWorldState[1] = worldstate2;
		}
		else
		if (ta2 == ALLIANCE)
		{
			m_towerOwner[1] = ALLIANCE;
			m_towerWorldState[1] = worldstate2;
		}
		else
		{
			m_towerOwner[1] = TEAM_NONE;
			m_towerWorldState[1] = WORLD_STATE_EP_CROWNGUARD_NEUTRAL;
		}

		if (ta3 == HORDE)
		{
		    m_towerOwner[2] = HORDE;
			m_towerWorldState[2] = worldstate3;
		}
		else
		if (ta3 == ALLIANCE)
		{
			m_towerOwner[2] = ALLIANCE;
			m_towerWorldState[2] = worldstate3;
		}
		else
		{
			m_towerOwner[2] = TEAM_NONE;
			m_towerWorldState[2] = WORLD_STATE_EP_EASTWALL_NEUTRAL;
		}

		if (ta4 == HORDE)
		{
		    m_towerOwner[3] = HORDE;
			m_towerWorldState[3] = worldstate4;
		}
		else
		if (ta4 == ALLIANCE)
		{
			m_towerOwner[3] = ALLIANCE;
			m_towerWorldState[3] = worldstate4;
		}
		else
		{
			m_towerOwner[3] = TEAM_NONE;
			m_towerWorldState[3] = WORLD_STATE_EP_PLAGUEWOOD_NEUTRAL;
		}
	}
}

void OutdoorPvPEP::FillInitialWorldStates(WorldPacket& data, uint32& count)
{
	 FillInitialWorldState(data, count, WORLD_STATE_EP_TOWER_COUNT_ALLIANCE, m_towersAlliance);
     FillInitialWorldState(data, count, WORLD_STATE_EP_TOWER_COUNT_HORDE, m_towersHorde);

     for (uint8 i = 0; i < MAX_EP_TOWERS; ++i)
          FillInitialWorldState(data, count, m_towerWorldState[i], WORLD_STATE_ADD);
}

void OutdoorPvPEP::SendRemoveWorldStates(Player* player)
{
    for (uint8 i = 0; i < MAX_EP_TOWERS; ++i)
        player->SendUpdateWorldState(m_towerWorldState[i], WORLD_STATE_REMOVE);
}

void OutdoorPvPEP::HandlePlayerEnterZone(Player* player, bool isMainZone)
{
    OutdoorPvP::HandlePlayerEnterZone(player, isMainZone);

    // remove the buff from the player first; Sometimes on relog players still have the aura
    for (uint8 i = 0; i < MAX_EP_TOWERS; ++i)
        player->RemoveAurasDueToSpell(player->GetTeam() == ALLIANCE ? plaguelandsTowerBuffs[i].spellIdAlliance : plaguelandsTowerBuffs[i].spellIdHorde);

	for (uint8 i = 0; i < 2; ++i)
	    player->RemoveAurasDueToSpell(player->GetTeam() == ALLIANCE ? SPELL_ECHOES_OF_LORDAERON_ALLIANCE_5 : SPELL_ECHOES_OF_LORDAERON_HORDE_5);

    // buff the player
    switch (player->GetTeam())
    {
        case ALLIANCE:
            if (m_towersAlliance > 0)
                player->CastSpell(player, plaguelandsTowerBuffs[m_towersAlliance - 1].spellIdAlliance, true);
			if (m_towersAlliance == 4)
                player->CastSpell(player, SPELL_ECHOES_OF_LORDAERON_ALLIANCE_5, true);
            break;
        case HORDE:
            if (m_towersHorde > 0)
                player->CastSpell(player, plaguelandsTowerBuffs[m_towersHorde - 1].spellIdHorde, true);
			if (m_towersHorde == 4)
                player->CastSpell(player, SPELL_ECHOES_OF_LORDAERON_HORDE_5, true);
            break;
        default:
            break;
    }
}

void OutdoorPvPEP::HandlePlayerLeaveZone(Player* player, bool isMainZone)
{
    // remove the buff from the player
    for (uint8 i = 0; i < MAX_EP_TOWERS; ++i)
        player->RemoveAurasDueToSpell(player->GetTeam() == ALLIANCE ? plaguelandsTowerBuffs[i].spellIdAlliance : plaguelandsTowerBuffs[i].spellIdHorde);

	for (uint8 i = 0; i < 2; ++i)
	    player->RemoveAurasDueToSpell(player->GetTeam() == ALLIANCE ? SPELL_ECHOES_OF_LORDAERON_ALLIANCE_5 : SPELL_ECHOES_OF_LORDAERON_HORDE_5);

    OutdoorPvP::HandlePlayerLeaveZone(player, isMainZone);
}

void OutdoorPvPEP::HandleCreatureCreate(Creature* creature)
{
    switch (creature->GetEntry())
    {
        /*case NPC_SPECTRAL_FLIGHT_MASTER:
            m_flightMaster = creature->GetObjectGuid();
            creature->setFaction(m_towerOwner[TOWER_ID_PLAGUEWOOD] == ALLIANCE ? FACTION_FLIGHT_MASTER_ALLIANCE : FACTION_FLIGHT_MASTER_HORDE);
            creature->CastSpell(creature, m_towerOwner[TOWER_ID_PLAGUEWOOD] == ALLIANCE ? SPELL_SPIRIT_PARTICLES_BLUE : SPELL_SPIRIT_PARTICLES_RED, true);
            break;*/
		case NPC_ZHIHUEIGUAN4:
		case NPC_SPECTRAL_FLIGHT_MASTER:
			creature->setFaction(m_towerOwner[TOWER_ID_PLAGUEWOOD] == ALLIANCE ? FACTION_ALLIANCE : FACTION_HORDE);
			creature->CastSpell(creature, m_towerOwner[TOWER_ID_PLAGUEWOOD] == ALLIANCE ? SPELL_SPIRIT_PARTICLES_BLUE : SPELL_SPIRIT_PARTICLES_RED, true);
            m_tuzhinpc4.push_back(creature->GetObjectGuid());
			break;
		case NPC_ZHIHUEIGUAN1:
			creature->setFaction(m_towerOwner[TOWER_ID_NORTHPASS] == ALLIANCE ? FACTION_ALLIANCE : FACTION_HORDE);
			creature->CastSpell(creature, m_towerOwner[TOWER_ID_NORTHPASS] == ALLIANCE ? SPELL_SPIRIT_PARTICLES_BLUE : SPELL_SPIRIT_PARTICLES_RED, true);
            m_tuzhinpc1.push_back(creature->GetObjectGuid());
			break;
		case NPC_ZHIHUEIGUAN3:
			creature->setFaction(m_towerOwner[TOWER_ID_EASTWALL] == ALLIANCE ? FACTION_ALLIANCE : FACTION_HORDE);
			creature->CastSpell(creature, m_towerOwner[TOWER_ID_EASTWALL] == ALLIANCE ? SPELL_SPIRIT_PARTICLES_BLUE : SPELL_SPIRIT_PARTICLES_RED, true);
            m_tuzhinpc3.push_back(creature->GetObjectGuid());
			break;
		case NPC_ZHIHUEIGUAN2:
			creature->setFaction(m_towerOwner[TOWER_ID_CROWNGUARD] == ALLIANCE ? FACTION_ALLIANCE : FACTION_HORDE);
			creature->CastSpell(creature, m_towerOwner[TOWER_ID_CROWNGUARD] == ALLIANCE ? SPELL_SPIRIT_PARTICLES_BLUE : SPELL_SPIRIT_PARTICLES_RED, true);
            m_tuzhinpc2.push_back(creature->GetObjectGuid());
			break;
        /*case NPC_LORDAERON_COMMANDER:
        case NPC_LORDAERON_SOLDIER:
        case NPC_LORDAERON_VETERAN:
        case NPC_LORDAERON_FIGHTER:
            m_soldiers.push_back(creature->GetObjectGuid());
            break;*/
    }
}

void OutdoorPvPEP::HandleGameObjectCreate(GameObject* go)
{
    OutdoorPvP::HandleGameObjectCreate(go);

    switch (go->GetEntry())
    {
        case GO_TOWER_BANNER_NORTHPASS:
            InitBanner(go, TOWER_ID_NORTHPASS);
            break;
        case GO_TOWER_BANNER_CROWNGUARD:
            InitBanner(go, TOWER_ID_CROWNGUARD);
            break;
        case GO_TOWER_BANNER_EASTWALL:
            InitBanner(go, TOWER_ID_EASTWALL);
            break;
        case GO_TOWER_BANNER_PLAGUEWOOD:
            InitBanner(go, TOWER_ID_PLAGUEWOOD);
            break;
        case GO_TOWER_BANNER:
            // sort banners
            if (go->IsWithinDist2d(plaguelandsTowerLocations[TOWER_ID_NORTHPASS][0], plaguelandsTowerLocations[TOWER_ID_NORTHPASS][1], 50.0f))
                InitBanner(go, TOWER_ID_NORTHPASS);
            else if (go->IsWithinDist2d(plaguelandsTowerLocations[TOWER_ID_CROWNGUARD][0], plaguelandsTowerLocations[TOWER_ID_CROWNGUARD][1], 50.0f))
                InitBanner(go, TOWER_ID_CROWNGUARD);
            else if (go->IsWithinDist2d(plaguelandsTowerLocations[TOWER_ID_EASTWALL][0], plaguelandsTowerLocations[TOWER_ID_EASTWALL][1], 50.0f))
                InitBanner(go, TOWER_ID_EASTWALL);
            else if (go->IsWithinDist2d(plaguelandsTowerLocations[TOWER_ID_PLAGUEWOOD][0], plaguelandsTowerLocations[TOWER_ID_PLAGUEWOOD][1], 50.0f))
                InitBanner(go, TOWER_ID_PLAGUEWOOD);
            break;
        /*case GO_LORDAERON_SHRINE_ALLIANCE:
            m_lordaeronShrineAlliance = go->GetObjectGuid();
            break;
        case GO_LORDAERON_SHRINE_HORDE:
            m_lordaeronShrineHorde = go->GetObjectGuid();
            break;*/
		case GO_LORDAERON_BEIDISHAOTA:
            m_beidishaota = go->GetObjectGuid();
			if (m_towerOwner[TOWER_ID_NORTHPASS] != TEAM_NONE)
			{
				for (uint8 i = 0; i < 1; ++i)
                     go->SummonCreature(SpawnLocsa[i].NpcEntry, SpawnLocsa[i].m_fX, SpawnLocsa[i].m_fY, SpawnLocsa[i].m_fZ, SpawnLocsa[i].m_fO, TEMPSUMMON_DEAD_DESPAWN, 0);
			}
            break;
		case GO_LORDAERON_HUANGGUANTA:
            m_huangguanta = go->GetObjectGuid();
			if (m_towerOwner[TOWER_ID_CROWNGUARD] != TEAM_NONE)
			{
				for (uint8 i = 0; i < 1; ++i)
                     go->SummonCreature(SpawnLocsb[i].NpcEntry, SpawnLocsb[i].m_fX, SpawnLocsb[i].m_fY, SpawnLocsb[i].m_fZ, SpawnLocsb[i].m_fO, TEMPSUMMON_DEAD_DESPAWN, 0);
			}
            break;
		case GO_LORDAERON_DONGQIANGTA:
            m_dongqiangta = go->GetObjectGuid();
			if (m_towerOwner[TOWER_ID_EASTWALL] != TEAM_NONE)
			{
				for (uint8 i = 0; i < 1; ++i)
                     go->SummonCreature(SpawnLocsc[i].NpcEntry, SpawnLocsc[i].m_fX, SpawnLocsc[i].m_fY, SpawnLocsc[i].m_fZ, SpawnLocsc[i].m_fO, TEMPSUMMON_DEAD_DESPAWN, 0);
			}
            break;
		case GO_LORDAERON_BINGMUSHATA:
            m_bingmulinta = go->GetObjectGuid();
			if (m_towerOwner[TOWER_ID_PLAGUEWOOD] != TEAM_NONE)
			{
				for (uint8 i = 0; i < 2; ++i)
                     go->SummonCreature(SpawnLocsd[i].NpcEntry, SpawnLocsd[i].m_fX, SpawnLocsd[i].m_fY, SpawnLocsd[i].m_fZ, SpawnLocsd[i].m_fO, TEMPSUMMON_DEAD_DESPAWN, 0);
			}
            break;
    }
}

void OutdoorPvPEP::HandleObjectiveComplete(uint32 eventId, const std::list<Player*>& players, Team team)
{
    uint32 credit = 0;

    switch (eventId)
    {
        case EVENT_CROWNGUARD_PROGRESS_ALLIANCE:
        case EVENT_CROWNGUARD_PROGRESS_HORDE:
            credit = NPC_CROWNGUARD_TOWER_QUEST_DOODAD;
            break;
        case EVENT_EASTWALL_PROGRESS_ALLIANCE:
        case EVENT_EASTWALL_PROGRESS_HORDE:
            credit = NPC_EASTWALL_TOWER_QUEST_DOODAD;
            break;
        case EVENT_NORTHPASS_PROGRESS_ALLIANCE:
        case EVENT_NORTHPASS_PROGRESS_HORDE:
            credit = NPC_NORTHPASS_TOWER_QUEST_DOODAD;
            break;
        case EVENT_PLAGUEWOOD_PROGRESS_ALLIANCE:
        case EVENT_PLAGUEWOOD_PROGRESS_HORDE:
            credit = NPC_PLAGUEWOOD_TOWER_QUEST_DOODAD;
            break;
        default:
            return;
    }

    for (std::list<Player*>::const_iterator itr = players.begin(); itr != players.end(); ++itr)
    {
        if ((*itr) && (*itr)->GetTeam() == team)
        {
            (*itr)->KilledMonsterCredit(credit);
            (*itr)->AddHonorCP(HONOR_REWARD_PLAGUELANDS, HONORABLE, 0, 0);

			/*switch (credit)
			{
			    case NPC_CROWNGUARD_TOWER_QUEST_DOODAD:
					AddJdItem((*itr), ITEM_WUPIN2, 1);
					break;
				case NPC_EASTWALL_TOWER_QUEST_DOODAD:
					AddJdItem((*itr), ITEM_WUPIN3, 1);
					break;
				case NPC_NORTHPASS_TOWER_QUEST_DOODAD:
					AddJdItem((*itr), ITEM_WUPIN1, 1);
					break;
				case NPC_PLAGUEWOOD_TOWER_QUEST_DOODAD:
					AddJdItem((*itr), ITEM_WUPIN4, 1);
					break;
			}*/
        }
    }
}

// process the capture events
bool OutdoorPvPEP::HandleEvent(uint32 eventId, GameObject* go)
{
    for (uint8 i = 0; i < MAX_EP_TOWERS; ++i)
    {
        if (plaguelandsBanners[i] == go->GetEntry())
        {
            for (uint8 j = 0; j < 4; ++j)
            {
                if (plaguelandsTowerEvents[i][j].eventEntry == eventId)
                {
                    // prevent processing if the owner did not change (happens if progress event is called after contest event)
                    if (plaguelandsTowerEvents[i][j].team != m_towerOwner[i])
                    {
                        if (plaguelandsTowerEvents[i][j].defenseMessage)
                            sWorld.SendDefenseMessage(ZONE_ID_EASTERN_PLAGUELANDS, plaguelandsTowerEvents[i][j].defenseMessage);

						    switch (i)
	                        {
		                           case 0:
			                            CharacterDatabase.PExecute("UPDATE outdoorpvpta SET ta1 = '%u', worldstate1 = '%u', go1 = '%u', eventEntry1 = '%u' WHERE id = '%u'", plaguelandsTowerEvents[i][j].team, plaguelandsTowerEvents[i][j].worldState, go->GetEntry(), plaguelandsTowerEvents[i][j].eventEntry, outdoorpvpid);
			                            break;
		                           case 1:
			                            CharacterDatabase.PExecute("UPDATE outdoorpvpta SET ta2 = '%u', worldstate2 = '%u', go2 = '%u', eventEntry2 = '%u' WHERE id = '%u'", plaguelandsTowerEvents[i][j].team, plaguelandsTowerEvents[i][j].worldState, go->GetEntry(), plaguelandsTowerEvents[i][j].eventEntry, outdoorpvpid);
			                            break;
		                           case 2:
			                            CharacterDatabase.PExecute("UPDATE outdoorpvpta SET ta3 = '%u', worldstate3 = '%u', go3 = '%u', eventEntry3 = '%u' WHERE id = '%u'", plaguelandsTowerEvents[i][j].team, plaguelandsTowerEvents[i][j].worldState, go->GetEntry(), plaguelandsTowerEvents[i][j].eventEntry, outdoorpvpid);
			                            break;
		                           case 3:
			                            CharacterDatabase.PExecute("UPDATE outdoorpvpta SET ta4 = '%u', worldstate4 = '%u', go4 = '%u', eventEntry4 = '%u' WHERE id = '%u'", plaguelandsTowerEvents[i][j].team, plaguelandsTowerEvents[i][j].worldState, go->GetEntry(), plaguelandsTowerEvents[i][j].eventEntry, outdoorpvpid);
			                            break;
	                        }

                        return ProcessCaptureEvent(go, i, plaguelandsTowerEvents[i][j].team, plaguelandsTowerEvents[i][j].worldState);
                    }
                    // no need to iterate other events or towers
                    return false;
                }
            }
            // no need to iterate other towers
            return false;
        }
    }

    return false;
}

bool OutdoorPvPEP::ProcessCaptureEvent(GameObject* go, uint32 towerId, Team team, uint32 newWorldState)
{
    if (team == ALLIANCE)
    {
        // update banner
        for (GuidList::const_iterator itr = m_towerBanners[towerId].begin(); itr != m_towerBanners[towerId].end(); ++itr)
            SetBannerVisual(go, (*itr), CAPTURE_ARTKIT_ALLIANCE, CAPTURE_ANIM_ALLIANCE);

        // update counter
        ++m_towersAlliance;
        SendUpdateWorldState(WORLD_STATE_EP_TOWER_COUNT_ALLIANCE, m_towersAlliance);

        // buff players
        BuffTeam(ALLIANCE, plaguelandsTowerBuffs[m_towersAlliance - 1].spellIdAlliance);

		if (m_towersAlliance == 4)
			BuffTeam(ALLIANCE, SPELL_ECHOES_OF_LORDAERON_ALLIANCE_5);
    }
    else if (team == HORDE)
    {
        // update banner
        for (GuidList::const_iterator itr = m_towerBanners[towerId].begin(); itr != m_towerBanners[towerId].end(); ++itr)
            SetBannerVisual(go, (*itr), CAPTURE_ARTKIT_HORDE, CAPTURE_ANIM_HORDE);

        // update counter
        ++m_towersHorde;
        SendUpdateWorldState(WORLD_STATE_EP_TOWER_COUNT_HORDE, m_towersHorde);

        // buff players
        BuffTeam(HORDE, plaguelandsTowerBuffs[m_towersHorde - 1].spellIdHorde);

		if (m_towersHorde == 4)
			BuffTeam(HORDE, SPELL_ECHOES_OF_LORDAERON_HORDE_5);
    }
    else
    {
        // update banner
        for (GuidList::const_iterator itr = m_towerBanners[towerId].begin(); itr != m_towerBanners[towerId].end(); ++itr)
            SetBannerVisual(go, (*itr), CAPTURE_ARTKIT_NEUTRAL, CAPTURE_ANIM_NEUTRAL);

        if (m_towerOwner[towerId] == ALLIANCE)
        {
            // update counter
            --m_towersAlliance;
            SendUpdateWorldState(WORLD_STATE_EP_TOWER_COUNT_ALLIANCE, m_towersAlliance);

            if (m_towersAlliance == 0)
                BuffTeam(ALLIANCE, plaguelandsTowerBuffs[0].spellIdAlliance, true);
			else
				BuffTeam(ALLIANCE, plaguelandsTowerBuffs[m_towersAlliance - 1].spellIdAlliance);

			if (m_towersAlliance == 3)
				BuffTeam(ALLIANCE, SPELL_ECHOES_OF_LORDAERON_ALLIANCE_5, true);
        }
        else
        {
            // update counter
            --m_towersHorde;
            SendUpdateWorldState(WORLD_STATE_EP_TOWER_COUNT_HORDE, m_towersHorde);

            if (m_towersHorde == 0)
                BuffTeam(HORDE, plaguelandsTowerBuffs[0].spellIdHorde, true);
			else
				BuffTeam(HORDE, plaguelandsTowerBuffs[m_towersHorde - 1].spellIdHorde);

			if (m_towersHorde == 3)
				BuffTeam(HORDE, SPELL_ECHOES_OF_LORDAERON_HORDE_5, true);
        }
    }

    bool eventHandled = true;

    if (team != TEAM_NONE)
    {
        // update capture point owner before rewards are applied
        m_towerOwner[towerId] = team;

        // apply rewards of changed tower
        switch (towerId)
        {
            case TOWER_ID_NORTHPASS:
                //RespawnGO(go, team == ALLIANCE ? m_lordaeronShrineAlliance : m_lordaeronShrineHorde, true);
				RespawnCreaturea(go, m_beidishaota);
                break;
            case TOWER_ID_CROWNGUARD:
                sObjectMgr.SetGraveYardLinkTeam(GRAVEYARD_ID_EASTERN_PLAGUE, GRAVEYARD_ZONE_EASTERN_PLAGUE, team);
				RespawnCreatureb(go, m_huangguanta);
                break;
            case TOWER_ID_EASTWALL:
                // Return false - allow the DB to handle summons
                if (m_towerOwner[TOWER_ID_NORTHPASS] != team)
                    eventHandled = false;
				RespawnCreaturec(go, m_dongqiangta);
                break;
            case TOWER_ID_PLAGUEWOOD:
                // Return false - allow the DB to handle summons
                eventHandled = false;
				RespawnCreatured(go, m_bingmulinta);
                break;
        }
    }
    else
    {
        // remove rewards of changed tower
        switch (towerId)
        {
            case TOWER_ID_NORTHPASS:
                //RespawnGO(go, m_towerOwner[TOWER_ID_NORTHPASS] == ALLIANCE ? m_lordaeronShrineAlliance : m_lordaeronShrineHorde, false);
				RespawnDespawn(go, m_tuzhinpc1);
                break;
            case TOWER_ID_CROWNGUARD:
                sObjectMgr.SetGraveYardLinkTeam(GRAVEYARD_ID_EASTERN_PLAGUE, GRAVEYARD_ZONE_EASTERN_PLAGUE, TEAM_INVALID);
				RespawnDespawn(go, m_tuzhinpc2);
                break;
            case TOWER_ID_EASTWALL:
                //UnsummonSoldiers(go);
				RespawnDespawn(go, m_tuzhinpc3);
                break;
            case TOWER_ID_PLAGUEWOOD:
                //UnsummonFlightMaster(go);
				RespawnDespawn(go, m_tuzhinpc4);
                break;
        }

        // update capture point owner after rewards have been removed
        m_towerOwner[towerId] = team;
    }

    // update tower state
    SendUpdateWorldState(m_towerWorldState[towerId], WORLD_STATE_REMOVE);
    m_towerWorldState[towerId] = newWorldState;
    SendUpdateWorldState(m_towerWorldState[towerId], WORLD_STATE_ADD);

	CharacterDatabase.PExecute("UPDATE outdoorpvpta SET horde = '%u', aliance = '%u' WHERE id = '%u'", m_towersHorde, m_towersAlliance, outdoorpvpid);
	CharacterDatabase.CommitTransaction();

	sWorld.SendWorldText(LANG_OPVP_SI_CAPTURE_1, m_towersAlliance, m_towersHorde);

    // there are some events which required further DB script
    return eventHandled;
}

bool OutdoorPvPEP::HandleGameObjectUse(Player* /*player*/, GameObject* go)
{
    // prevent despawning after go use
    if (go->GetEntry() == GO_LORDAERON_SHRINE_ALLIANCE || go->GetEntry() == GO_LORDAERON_SHRINE_HORDE)
        go->SetRespawnTime(0);

    return false;
}

void OutdoorPvPEP::InitBanner(GameObject* go, uint32 towerId)
{
    m_towerBanners[towerId].push_back(go->GetObjectGuid());
    go->SetGoArtKit(GetBannerArtKit(m_towerOwner[towerId]));
}

// Handle the unsummon of the spectral flight master when the Plaguewood tower is lost
/*void OutdoorPvPEP::UnsummonFlightMaster(const WorldObject* objRef)
{
    if (Creature* flightMaster = objRef->GetMap()->GetCreature(m_flightMaster))
        flightMaster->ForcedDespawn();
}*/

// Handle the unsummon of the soldiers when the Eastwall tower is lost
/*void OutdoorPvPEP::UnsummonSoldiers(const WorldObject* objRef)
{
    for (GuidList::const_iterator itr = m_soldiers.begin(); itr != m_soldiers.end(); ++itr)
    {
        if (Creature* soldier = objRef->GetMap()->GetCreature(*itr))
            soldier->ForcedDespawn();
    }

    m_soldiers.clear();
}*/
