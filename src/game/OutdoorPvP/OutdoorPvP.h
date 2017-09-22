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

#ifndef OUTDOOR_PVP_H
#define OUTDOOR_PVP_H

#include "Common.h"
#include "ObjectGuid.h"
#include "SharedDefines.h"
#include "OutdoorPvPMgr.h"

// forward declaration
class WorldPacket;
class WorldObject;
class Player;
class GameObject;
class Unit;
class Creature;

enum
{
	SPELL_ECHOES_1  = 40001,
	SPELL_ECHOES_2  = 40002,
	SPELL_ECHOES_3  = 40003,
	SPELL_ECHOES_4  = 40004,
	SPELL_ECHOES_5  = 40005,

	XINRENITEM      = 26003,
	XINRENITEM1     = 26004,
	XINRENITEM2     = 26005,
	XINRENITEM3     = 26006,
	XINRENITEM_CONT = 1,

	MAP_ID1 = 389,
	MAP_ID2 = 36,
	MAP_ID3 = 43,
	MAP_ID4 = 33,
	MAP_ID5 = 34,
	MAP_ID6 = 48,
	MAP_ID7 = 47,
	MAP_ID8 = 90,
	MAP_ID9 = 189,
	MAP_ID10 = 129,
	MAP_ID11 = 70,
	MAP_ID12 = 209,
	MAP_ID13 = 349,
	MAP_ID14 = 109,
	MAP_ID15 = 230,
	MAP_ID16 = 329,
	MAP_ID17 = 289,
	MAP_ID18 = 429,

	MAX_SPELL = 5,
};

enum CapturePointArtKits
{
    CAPTURE_ARTKIT_ALLIANCE = 2,
    CAPTURE_ARTKIT_HORDE    = 1,
    CAPTURE_ARTKIT_NEUTRAL  = 21
};

enum CapturePointAnimations
{
    CAPTURE_ANIM_ALLIANCE   = 1,
    CAPTURE_ANIM_HORDE      = 0,
    CAPTURE_ANIM_NEUTRAL    = 2
};

struct PlayerBuff1
{
	uint32 spellId;
};

static const PlayerBuff1 PlayerBuffXinRen[MAX_SPELL] =
{
	{ SPELL_ECHOES_1 },
	{ SPELL_ECHOES_2 },
	{ SPELL_ECHOES_3 },
	{ SPELL_ECHOES_4 },
	{ SPELL_ECHOES_5 }
};

struct NpcShuaXin
{
    uint32  NpcEntry;
    float m_fX, m_fY, m_fZ, m_fO;
};

static const NpcShuaXin SpawnLocsa[1] =
{
	{20010, 3169.9421f, -4371.9824f, 139.6299f, 1.71433f}
};

static const NpcShuaXin SpawnLocsb[1] =
{
	{20012, 1854.6921f, -3719.1574f, 162.2227f, 1.20775f}
};

static const NpcShuaXin SpawnLocsc[1] =
{
	{20011, 2561.2031f, -4794.7143f, 110.2104f, 2.31729f}
};

static const NpcShuaXin SpawnLocsd[2] =
{
	{17209, 2987.5666f, -3049.2331f, 120.0391f, 5.78794f},
	{20009, 2975.1552f, -3037.4128f, 120.2959f, 5.83273f}
};

typedef std::map < ObjectGuid /*playerGuid*/, bool /*isMainZone*/ > GuidZoneMap;

class OutdoorPvP
{
        friend class OutdoorPvPMgr;

    public:
        OutdoorPvP() {}
        virtual ~OutdoorPvP() {}

        // called when the zone is initialized
        virtual void FillInitialWorldStates(WorldPacket& /*data*/, uint32& /*count*/) {}

        // Process Capture event
        virtual bool HandleEvent(uint32 /*eventId*/, GameObject* /*go*/) { return false; }

        // handle capture objective complete
        virtual void HandleObjectiveComplete(uint32 /*eventId*/, const std::list<Player*>& /*players*/, Team /*team*/) {}

        // Called when a creature is created
        virtual void HandleCreatureCreate(Creature* /*creature*/) {}
		virtual void HandleCreatureEnterCombat(Creature* /*creature*/) {}

        // Called when a gameobject is created or removed
        virtual void HandleGameObjectCreate(GameObject* /*go*/);
        virtual void HandleGameObjectRemove(GameObject* /*go*/);

        // Called on creature death
        virtual void HandleCreatureDeath(Creature* /*creature*/) {}

        // called when a player uses a gameobject related to outdoor pvp events
        virtual bool HandleGameObjectUse(Player* /*player*/, GameObject* /*go*/) { return false; }

        // called when a player triggers an areatrigger
        virtual bool HandleAreaTrigger(Player* /*player*/, uint32 /*triggerId*/) { return false; }

        // called when a player drops a flag
        virtual bool HandleDropFlag(Player* /*player*/, uint32 /*spellId*/) { return false; }

        // update - called by the OutdoorPvPMgr
        virtual void Update(uint32 /*diff*/) {}

        // Handle player kill
        void HandlePlayerKill(Player* killer, Player* victim);

    protected:

        // Player related stuff
        virtual void HandlePlayerEnterZone(Player* /*player*/, bool /*isMainZone*/);
        virtual void HandlePlayerLeaveZone(Player* /*player*/, bool /*isMainZone*/);

        // remove world states
        virtual void SendRemoveWorldStates(Player* /*player*/) {}

        // handle npc/player kill
        virtual void HandlePlayerKillInsideArea(Player* /*killer*/) {}

        // send world state update to all players present
        void SendUpdateWorldState(uint32 field, uint32 value);

        // applies buff to a team inside the specific zone
        void BuffTeam(Team team, uint32 spellId, bool remove = false);

        // get banner artkit based on controlling team
        uint32 GetBannerArtKit(Team team, uint32 artKitAlliance = CAPTURE_ARTKIT_ALLIANCE, uint32 artKitHorde = CAPTURE_ARTKIT_HORDE, uint32 artKitNeutral = CAPTURE_ARTKIT_NEUTRAL);

        // set banner visual
        void SetBannerVisual(const WorldObject* objRef, ObjectGuid goGuid, uint32 artKit, uint32 animId);
        void SetBannerVisual(GameObject* go, uint32 artKit, uint32 animId);

        // Handle gameobject spawn / despawn
        void RespawnGO(const WorldObject* objRef, ObjectGuid goGuid, bool respawn);
		void RespawnCreaturea(const WorldObject* objRef, ObjectGuid goGuid);
		void RespawnCreatureb(const WorldObject* objRef, ObjectGuid goGuid);
		void RespawnCreaturec(const WorldObject* objRef, ObjectGuid goGuid);
		void RespawnCreatured(const WorldObject* objRef, ObjectGuid goGuid);
		void RespawnDespawn(const WorldObject* objRef, GuidList npcEntry);
		void AddJdItem(Player* player, uint32 ItemId, uint32 cont);
		bool GetAreaTriggerLockStatus(Player* player);

        // store the players inside the area
        GuidZoneMap m_zonePlayers;
};

#endif
