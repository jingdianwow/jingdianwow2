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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "Common.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Opcodes.h"
#include "Log.h"
#include "Corpse.h"
#include "Player.h"
#include "MapManager.h"
#include "Transports.h"
#include "BattleGround/BattleGround.h"
#include "WaypointMovementGenerator.h"
#include "MapPersistentStateMgr.h"
#include "ObjectMgr.h"
#include "World.h"
#include "SpellMgr.h"
#include "Spell.h"

//#define __ANTI_DEBUG__

#ifdef __ANTI_DEBUG__
#include "Chat.h"
std::string FlagsToStr(const uint32 Flags)
{
    std::string Ret="";
    if(Flags==0)
    {
        Ret="None";
        return Ret;
    }
    
    if(Flags & MOVEMENTFLAG_FORWARD)
    {   Ret+="FW "; }
    if(Flags & MOVEMENTFLAG_BACKWARD)
    {   Ret+="BW "; }
    if(Flags & MOVEMENTFLAG_STRAFE_LEFT)
    {   Ret+="STL ";    }
    if(Flags & MOVEMENTFLAG_STRAFE_RIGHT)
    {   Ret+="STR ";    }
    if(Flags & MOVEMENTFLAG_LEFT)
    {   Ret+="LF "; }
    if(Flags & MOVEMENTFLAG_RIGHT)
    {   Ret+="RI "; }
    if(Flags & MOVEMENTFLAG_PITCH_UP)
    {   Ret+="PTUP ";   }
    if(Flags & MOVEMENTFLAG_PITCH_DOWN)
    {   Ret+="PTDW ";   }
    if(Flags & MOVEMENTFLAG_WALK_MODE)
    {   Ret+="WALK ";   }
    if(Flags & MOVEMENTFLAG_ONTRANSPORT)
    {   Ret+="TRANS ";  }
    if(Flags & MOVEMENTFLAG_LEVITATING)
    {   Ret+="LEVI ";   }
    if(Flags & MOVEMENTFLAG_FLY_UNK1)
    {   Ret+="FLYUNK1 ";    }
    if(Flags & MOVEMENTFLAG_JUMPING)
    {   Ret+="JUMP ";   }
    if(Flags & MOVEMENTFLAG_UNK4)
    {   Ret+="UNK4 ";   }
    if(Flags & MOVEMENTFLAG_FALLING)
    {   Ret+="FALL ";   }
    if(Flags & MOVEMENTFLAG_SWIMMING)
    {   Ret+="SWIM ";   }
    if(Flags & MOVEMENTFLAG_FLY_UP)
    {   Ret+="FLYUP ";  }
    if(Flags & MOVEMENTFLAG_CAN_FLY)
    {   Ret+="CFLY ";   }
    if(Flags & MOVEMENTFLAG_FLYING)
    {   Ret+="FLY ";    }
    if(Flags & MOVEMENTFLAG_FLYING2)
    {   Ret+="FLY2 ";   }
    if(Flags & MOVEMENTFLAG_WATERWALKING)
    {   Ret+="WTWALK "; }
    if(Flags & MOVEMENTFLAG_SAFE_FALL)
    {   Ret+="SAFE ";   }
   if(Flags & MOVEMENTFLAG_UNK3)
    {   Ret+="UNK3 ";   }
    if(Flags & MOVEMENTFLAG_SPLINE)
    {   Ret+="SPLINE ";     }
    if(Flags & MOVEMENTFLAG_SPLINE2)
    {   Ret+="SPLINE2 ";    }
    
    return Ret;
}
#endif // __ANTI_DEBUG__

bool WorldSession::Anti__ReportCheat(const char* Reason,float Speed,const char* Op,float Val1,uint32 Val2,MovementInfo* MvInfo)
{
    if(!Reason)
    {
        sLog.outError("Anti__ReportCheat: Missing Reason parameter!");
        return false;
    }
    const char* Player=GetPlayer()->GetName();
    uint32 Acc=GetPlayer()->GetSession()->GetAccountId();
    uint32 Map=GetPlayer()->GetMapId();
    if(!Player)
    {
        sLog.outError("Anti__ReportCheat: Player with no name?!?");
        return false;
    }

    QueryResult *Res=CharacterDatabase.PQuery("SELECT speed,Val1 FROM cheaters WHERE player='%s' AND reason LIKE '%s' AND Map='%u' AND last_date >= NOW()-300",Player,Reason,Map);
    if(Res)
    {
        Field* Fields = Res->Fetch();
        
        std::stringstream Query;
        Query << "UPDATE cheaters SET count=count+1,last_date=NOW()";
        Query.precision(5);
        if(Speed>0.0f && Speed > Fields[0].GetFloat())
        {
            Query << ",speed='";
            Query << std::fixed << Speed;
            Query << "'";
        }

       if(Val1>0.0f && Val1 > Fields[1].GetFloat())
        {
            Query << ",Val1='";
            Query << std::fixed << Val1;
            Query << "'";
        }
        
        Query << " WHERE player='" << Player << "' AND reason='" << Reason << "' AND Map='" << Map << "' AND last_date >= NOW()-300 ORDER BY entry DESC LIMIT 1";
        
        CharacterDatabase.Execute(Query.str().c_str());
        delete Res;
    }
    else
    {
        if(!Op)
        {   Op="";  }
        std::stringstream Pos;
        Pos << "OldPos: " << GetPlayer()->GetPositionX() << " " << GetPlayer()->GetPositionY() << " "
            << GetPlayer()->GetPositionZ();
        if(MvInfo)
        {
            Pos << "\nNew: " << MvInfo->GetTransportPos()->x << " " << MvInfo->GetTransportPos()->y << " " << MvInfo->GetTransportPos()->z << "\n"
                << "Flags: " << MvInfo->GetMovementFlags() << "\n"
                << "t_guid: " << MvInfo->GetTransportGuid() << " falltime: " << MvInfo->GetFallTime();
        }
        CharacterDatabase.PExecute("INSERT INTO cheaters (player,acctid,reason,speed,count,first_date,last_date,`Op`,Val1,Val2,Map,Pos,Level) "
                                   "VALUES ('%s','%u','%s','%f','1',NOW(),NOW(),'%s','%f','%u','%u','%s','%u')",
                                   Player,Acc,Reason,Speed,Op,Val1,Val2,Map,
                                   Pos.str().c_str(),GetPlayer()->getLevel());
    }

	if (Reason == "Tele hack")
	{
			float x_, y_, z_, o_;
			uint32 map;
			bool in_flight;
			GetPlayer()->LoadPositionFromDB(GetPlayer()->GetObjectGuid(), map, x_, y_, z_, o_, in_flight);
			float floor_z_ = GetPlayer()->GetMap()->GetHeight(x_, y_, z_);
			GetPlayer()->TeleportTo(map, x_, y_, floor_z_, o_);
	}
	else
	if (Reason == "Fly hack")
	{
		//float floor_z_ = GetPlayer()->GetMap()->GetHeight(GetPlayer()->GetPositionX(), GetPlayer()->GetPositionY(), GetPlayer()->GetPositionZ());
		//GetPlayer()->TeleportTo(GetPlayer()->GetMapId(), GetPlayer()->GetPositionX(), GetPlayer()->GetPositionY(), floor_z_, GetPlayer()->GetOrientation());
	}
	else
	{
		if (sWorld.GetMvAnticheatKill() && GetPlayer()->isAlive())
		{
			GetPlayer()->DealDamage(GetPlayer(), GetPlayer()->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
		}
		if (sWorld.GetMvAnticheatKick())
		{
			GetPlayer()->TeleportToHomebind();
		}
		if (sWorld.GetMvAnticheatBan() & 1)
		{
			sWorld.BanAccount(BAN_CHARACTER, Player, sWorld.GetMvAnticheatBanTime(), "Cheat", "Anticheat");
		}
		if (sWorld.GetMvAnticheatBan() & 2)
		{
			QueryResult *result = LoginDatabase.PQuery("SELECT last_ip FROM account WHERE id=%u", Acc);
			if (result)
			{

				Field *fields = result->Fetch();
				std::string LastIP = fields[0].GetCppString();
				if (!LastIP.empty())
				{
					sWorld.BanAccount(BAN_IP, LastIP, sWorld.GetMvAnticheatBanTime(), "Cheat", "Anticheat");
				}
				delete result;
			}
		}
	}
   return true;
}

bool WorldSession::Anti__CheatOccurred(uint32 CurTime,const char* Reason,float Speed,const char* Op,
                                float Val1,uint32 Val2,MovementInfo* MvInfo)
{
    if(!Reason)
    {
        sLog.outError("Anti__CheatOccurred: Missing Reason parameter!");
        return false;
    }
    
    GetPlayer()->m_anti_lastalarmtime = CurTime;
    GetPlayer()->m_anti_alarmcount = GetPlayer()->m_anti_alarmcount + 1;

	if (Reason == "Fly hack")
	{
		Anti__ReportCheat(Reason, Speed, Op, Val1, Val2, MvInfo);
		return true;
	}
	else
    if (GetPlayer()->m_anti_alarmcount > sWorld.GetMvAnticheatAlarmCount())
    {
        Anti__ReportCheat(Reason,Speed,Op,Val1,Val2,MvInfo);
        return true;
    }
    return false;
}

#define MOVEMENT_PACKET_TIME_DELAY 0

void WorldSession::HandleMoveWorldportAckOpcode(WorldPacket& /*recv_data*/)
{
    DEBUG_LOG("WORLD: got MSG_MOVE_WORLDPORT_ACK.");
    HandleMoveWorldportAckOpcode();
}

void WorldSession::HandleMoveWorldportAckOpcode()
{
    // ignore unexpected far teleports
    if (!GetPlayer()->IsBeingTeleportedFar())
        return;

    // get start teleport coordinates (will used later in fail case)
    WorldLocation old_loc;
    GetPlayer()->GetPosition(old_loc);

    // get the teleport destination
    WorldLocation& loc = GetPlayer()->GetTeleportDest();

    // possible errors in the coordinate validity check (only cheating case possible)
    if (!MapManager::IsValidMapCoord(loc.mapid, loc.coord_x, loc.coord_y, loc.coord_z, loc.orientation))
    {
        sLog.outError("WorldSession::HandleMoveWorldportAckOpcode: %s was teleported far to a not valid location "
                      "(map:%u, x:%f, y:%f, z:%f) We port him to his homebind instead..",
                      GetPlayer()->GetGuidStr().c_str(), loc.mapid, loc.coord_x, loc.coord_y, loc.coord_z);
        // stop teleportation else we would try this again and again in LogoutPlayer...
        GetPlayer()->SetSemaphoreTeleportFar(false);
        // and teleport the player to a valid place
        GetPlayer()->TeleportToHomebind();
        return;
    }

    // get the destination map entry, not the current one, this will fix homebind and reset greeting
    MapEntry const* mEntry = sMapStore.LookupEntry(loc.mapid);

    Map* map = NULL;

    // prevent crash at attempt landing to not existed battleground instance
    if (mEntry->IsBattleGround())
    {
        if (GetPlayer()->GetBattleGroundId())
            map = sMapMgr.FindMap(loc.mapid, GetPlayer()->GetBattleGroundId());

        if (!map)
        {
            DETAIL_LOG("WorldSession::HandleMoveWorldportAckOpcode: %s was teleported far to nonexisten battleground instance "
                       " (map:%u, x:%f, y:%f, z:%f) Trying to port him to his previous place..",
                       GetPlayer()->GetGuidStr().c_str(), loc.mapid, loc.coord_x, loc.coord_y, loc.coord_z);

            GetPlayer()->SetSemaphoreTeleportFar(false);

            // Teleport to previous place, if cannot be ported back TP to homebind place
            if (!GetPlayer()->TeleportTo(old_loc))
            {
                DETAIL_LOG("WorldSession::HandleMoveWorldportAckOpcode: %s cannot be ported to his previous place, teleporting him to his homebind place...",
                           GetPlayer()->GetGuidStr().c_str());
                GetPlayer()->TeleportToHomebind();
            }
            return;
        }
    }

    InstanceTemplate const* mInstance = ObjectMgr::GetInstanceTemplate(loc.mapid);

    // reset instance validity, except if going to an instance inside an instance
    if (GetPlayer()->m_InstanceValid == false && !mInstance)
        GetPlayer()->m_InstanceValid = true;

    GetPlayer()->SetSemaphoreTeleportFar(false);

    // relocate the player to the teleport destination
    if (!map)
        map = sMapMgr.CreateMap(loc.mapid, GetPlayer());

    GetPlayer()->SetMap(map);
    GetPlayer()->Relocate(loc.coord_x, loc.coord_y, loc.coord_z, loc.orientation);
	GetPlayer()->m_anti_TeleTime=time(NULL);

    GetPlayer()->SendInitialPacketsBeforeAddToMap();
    // the CanEnter checks are done in TeleporTo but conditions may change
    // while the player is in transit, for example the map may get full
    if (!GetPlayer()->GetMap()->Add(GetPlayer()))
    {
        // if player wasn't added to map, reset his map pointer!
        GetPlayer()->ResetMap();

        DETAIL_LOG("WorldSession::HandleMoveWorldportAckOpcode: %s was teleported far but couldn't be added to map "
                   " (map:%u, x:%f, y:%f, z:%f) Trying to port him to his previous place..",
                   GetPlayer()->GetGuidStr().c_str(), loc.mapid, loc.coord_x, loc.coord_y, loc.coord_z);

        // Teleport to previous place, if cannot be ported back TP to homebind place
        if (!GetPlayer()->TeleportTo(old_loc))
        {
            DETAIL_LOG("WorldSession::HandleMoveWorldportAckOpcode: %s cannot be ported to his previous place, teleporting him to his homebind place...",
                       GetPlayer()->GetGuidStr().c_str());
            GetPlayer()->TeleportToHomebind();
        }
        return;
    }

    // battleground state prepare (in case join to BG), at relogin/tele player not invited
    // only add to bg group and object, if the player was invited (else he entered through command)
    if (_player->InBattleGround())
    {
        // cleanup seting if outdated
        if (!mEntry->IsBattleGround())
        {
            // We're not in BG
            _player->SetBattleGroundId(0, BATTLEGROUND_TYPE_NONE);
            // reset destination bg team
            _player->SetBGTeam(TEAM_NONE);
        }
        // join to bg case
        else if (BattleGround* bg = _player->GetBattleGround())
        {
            if (_player->IsInvitedForBattleGroundInstance(_player->GetBattleGroundId()))
                bg->AddPlayer(_player);
        }
    }

    GetPlayer()->SendInitialPacketsAfterAddToMap();

    // flight fast teleport case
    if (GetPlayer()->GetMotionMaster()->GetCurrentMovementGeneratorType() == FLIGHT_MOTION_TYPE)
    {
        if (!_player->InBattleGround())
        {
            // short preparations to continue flight
            FlightPathMovementGenerator* flight = (FlightPathMovementGenerator*)(GetPlayer()->GetMotionMaster()->top());
            flight->Reset(*GetPlayer());
            return;
        }

        // battleground state prepare, stop flight
        GetPlayer()->GetMotionMaster()->MovementExpired();
        GetPlayer()->m_taxi.ClearTaxiDestinations();
    }

    if (mEntry->IsRaid() && mInstance)
    {
        if (time_t timeReset = sMapPersistentStateMgr.GetScheduler().GetResetTimeFor(mEntry->MapID))
        {
            uint32 timeleft = uint32(timeReset - time(NULL));
            GetPlayer()->SendInstanceResetWarning(mEntry->MapID, timeleft);
        }
    }

    // mount allow check
	if (!mEntry->IsMountAllowed() || GetPlayer()->HasAura(25953) || GetPlayer()->HasAura(26054) || GetPlayer()->HasAura(26055) || GetPlayer()->HasAura(26056))
        _player->RemoveSpellsCausingAura(SPELL_AURA_MOUNTED);

    // honorless target
    if (GetPlayer()->pvpInfo.inHostileArea)
        GetPlayer()->CastSpell(GetPlayer(), 2479, true);

    // resummon pet
    GetPlayer()->ResummonPetTemporaryUnSummonedIfAny();
	GetPlayer()->Anti__SetLastTeleTime(::time(NULL));
    GetPlayer()->m_anti_BeginFallZ=INVALID_HEIGHT;

    // lets process all delayed operations on successful teleport
    GetPlayer()->ProcessDelayedOperations();
}

void WorldSession::HandleMoveTeleportAckOpcode(WorldPacket& recv_data)
{
    DEBUG_LOG("MSG_MOVE_TELEPORT_ACK");

    ObjectGuid guid;

    recv_data >> guid;

    uint32 counter, time;
    recv_data >> counter >> time;
    DEBUG_LOG("Guid: %s", guid.GetString().c_str());
    DEBUG_LOG("Counter %u, time %u", counter, time / IN_MILLISECONDS);

    Unit* mover = _player->GetMover();
    Player* plMover = mover->GetTypeId() == TYPEID_PLAYER ? (Player*)mover : NULL;

    if (!plMover || !plMover->IsBeingTeleportedNear())
        return;

    if (guid != plMover->GetObjectGuid())
        return;

    plMover->SetSemaphoreTeleportNear(false);

    uint32 old_zone = plMover->GetZoneId();

    WorldLocation const& dest = plMover->GetTeleportDest();

    plMover->SetPosition(dest.coord_x, dest.coord_y, dest.coord_z, dest.orientation, true);

    uint32 newzone, newarea;
    plMover->GetZoneAndAreaId(newzone, newarea);
    plMover->UpdateZone(newzone, newarea);

    // new zone
    if (old_zone != newzone)
    {
        // honorless target
        if (plMover->pvpInfo.inHostileArea)
            plMover->CastSpell(plMover, 2479, true);
    }

    // resummon pet
    GetPlayer()->ResummonPetTemporaryUnSummonedIfAny();
	if(plMover)
    {
        plMover->Anti__SetLastTeleTime(::time(NULL));
        plMover->m_anti_BeginFallZ=INVALID_HEIGHT;
    }

    // lets process all delayed operations on successful teleport
    GetPlayer()->ProcessDelayedOperations();
}

void WorldSession::HandleMovementOpcodes(WorldPacket& recv_data)
{
    uint16 opcode = recv_data.GetOpcode();
    if (!sLog.HasLogFilter(LOG_FILTER_PLAYER_MOVES))
    {
        DEBUG_LOG("WORLD: Received opcode %s (%u, 0x%X)", LookupOpcodeName(opcode), opcode, opcode);
        recv_data.hexlike();
    }

    Unit* mover = _player->GetMover();
    Player* plMover = mover->GetTypeId() == TYPEID_PLAYER ? (Player*)mover : NULL;

    // ignore, waiting processing in WorldSession::HandleMoveWorldportAckOpcode and WorldSession::HandleMoveTeleportAck
    if (plMover && plMover->IsBeingTeleported())
    {
        recv_data.rpos(recv_data.wpos());                   // prevent warnings spam
        return;
    }

    /* extract packet */
    MovementInfo movementInfo;
    recv_data >> movementInfo;
    /*----------------*/

    if (!VerifyMovementInfo(movementInfo))
        return;

	if (plMover && plMover->m_movementInfo.HasMovementFlag(MOVEFLAG_FALLING) && !plMover->IsTaxiFlying())
	{
		float z_diff = (plMover->m_lastFallZ >= plMover->m_anti_BeginFallZ ? plMover->m_lastFallZ : plMover->m_anti_BeginFallZ) - movementInfo.GetPos()->z;

		if (z_diff >= 200.0f)
		{
			if (!plMover->isAlive())
			{
				plMover->ResurrectPlayer(0.5f);
				plMover->SpawnCorpseBones();
				plMover->TeleportToHomebind(TELE_TO_SPELL);
			}
			else
			{
				float x, y, z, o;
				uint32 map;
				bool in_flight;
				plMover->LoadPositionFromDB(plMover->GetObjectGuid(), map, x, y, z, o, in_flight);
				float floor_z = plMover->GetMap()->GetHeight(x, y, z);
				plMover->TeleportTo(map, x, y, floor_z, o);
			}
		}
	}

    // fall damage generation (ignore in flight case that can be triggered also at lags in moment teleportation to another map).
    if (opcode == MSG_MOVE_FALL_LAND && plMover && !plMover->IsTaxiFlying())
        plMover->HandleFall(movementInfo);

    /* process position-change */
	HandleMoverRelocation(movementInfo, opcode);

    if (plMover)
        plMover->UpdateFallInformationIfNeed(movementInfo, opcode);

    WorldPacket data(opcode, recv_data.size());
    data << mover->GetPackGUID();             // write guid
    movementInfo.Write(data);                               // write data
    mover->SendMessageToSetExcept(&data, _player);
}

void WorldSession::HandleForceSpeedChangeAckOpcodes(WorldPacket& recv_data)
{
    uint16 opcode = recv_data.GetOpcode();
    DEBUG_LOG("WORLD: Received %s (%u, 0x%X) opcode", recv_data.GetOpcodeName(), opcode, opcode);

    /* extract packet */
    ObjectGuid guid;
    MovementInfo movementInfo;
    float  newspeed;

    recv_data >> guid;
    recv_data >> Unused<uint32>();                          // counter or moveEvent
    recv_data >> movementInfo;
    recv_data >> newspeed;

    // now can skip not our packet
    if (_player->GetObjectGuid() != guid)
        return;
    /*----------------*/

    // client ACK send one packet for mounted/run case and need skip all except last from its
    // in other cases anti-cheat check can be fail in false case
    UnitMoveType move_type;
    UnitMoveType force_move_type;

    static char const* move_type_name[MAX_MOVE_TYPE] = {  "Walk", "Run", "RunBack", "Swim", "SwimBack", "TurnRate" };

    switch (opcode)
    {
        case CMSG_FORCE_WALK_SPEED_CHANGE_ACK:          move_type = MOVE_WALK;          force_move_type = MOVE_WALK;        break;
        case CMSG_FORCE_RUN_SPEED_CHANGE_ACK:           move_type = MOVE_RUN;           force_move_type = MOVE_RUN;         break;
        case CMSG_FORCE_RUN_BACK_SPEED_CHANGE_ACK:      move_type = MOVE_RUN_BACK;      force_move_type = MOVE_RUN_BACK;    break;
        case CMSG_FORCE_SWIM_SPEED_CHANGE_ACK:          move_type = MOVE_SWIM;          force_move_type = MOVE_SWIM;        break;
        case CMSG_FORCE_SWIM_BACK_SPEED_CHANGE_ACK:     move_type = MOVE_SWIM_BACK;     force_move_type = MOVE_SWIM_BACK;   break;
        case CMSG_FORCE_TURN_RATE_CHANGE_ACK:           move_type = MOVE_TURN_RATE;     force_move_type = MOVE_TURN_RATE;   break;
        default:
            sLog.outError("WorldSession::HandleForceSpeedChangeAck: Unknown move type opcode: %u", opcode);
            return;
    }

    // skip all forced speed changes except last and unexpected
    // in run/mounted case used one ACK and it must be skipped.m_forced_speed_changes[MOVE_RUN} store both.
    if (_player->m_forced_speed_changes[force_move_type] > 0)
    {
        --_player->m_forced_speed_changes[force_move_type];
        if (_player->m_forced_speed_changes[force_move_type] > 0)
            return;
    }

    if (!_player->GetTransport() && fabs(_player->GetSpeed(move_type) - newspeed) > 0.01f)
    {
        if (_player->GetSpeed(move_type) > newspeed)        // must be greater - just correct
        {
            sLog.outError("%sSpeedChange player %s is NOT correct (must be %f instead %f), force set to correct value",
                          move_type_name[move_type], _player->GetName(), _player->GetSpeed(move_type), newspeed);
            _player->SetSpeedRate(move_type, _player->GetSpeedRate(move_type), true);
        }
        else                                                // must be lesser - cheating
        {
            BASIC_LOG("Player %s from account id %u kicked for incorrect speed (must be %f instead %f)",
                      _player->GetName(), _player->GetSession()->GetAccountId(), _player->GetSpeed(move_type), newspeed);
            _player->GetSession()->KickPlayer();
        }
    }
}

void WorldSession::HandleSetActiveMoverOpcode(WorldPacket& recv_data)
{
    DEBUG_LOG("WORLD: Received opcode CMSG_SET_ACTIVE_MOVER");

    ObjectGuid guid;
    recv_data >> guid;

    if (_player->GetMover()->GetObjectGuid() != guid)
    {
        sLog.outError("HandleSetActiveMoverOpcode: incorrect mover guid: mover is %s and should be %s",
                      _player->GetMover()->GetGuidStr().c_str(), guid.GetString().c_str());
        return;
    }
}

void WorldSession::HandleMoveNotActiveMoverOpcode(WorldPacket& recv_data)
{
    DEBUG_LOG("WORLD: Received opcode CMSG_MOVE_NOT_ACTIVE_MOVER");
    recv_data.hexlike();

    ObjectGuid old_mover_guid;
    MovementInfo mi;

    recv_data >> old_mover_guid;
    recv_data >> mi;

    if (_player->GetMover()->GetObjectGuid() == old_mover_guid)
    {
        sLog.outError("HandleMoveNotActiveMover: incorrect mover guid: mover is %s and should be %s instead of %s",
                      _player->GetMover()->GetGuidStr().c_str(),
                      _player->GetGuidStr().c_str(),
                      old_mover_guid.GetString().c_str());
        recv_data.rpos(recv_data.wpos());                   // prevent warnings spam
        return;
    }

    _player->m_movementInfo = mi;
}

void WorldSession::HandleMountSpecialAnimOpcode(WorldPacket& /*recvdata*/)
{
    // DEBUG_LOG("WORLD: Received opcode CMSG_MOUNTSPECIAL_ANIM");

    WorldPacket data(SMSG_MOUNTSPECIAL_ANIM, 8);
    data << GetPlayer()->GetObjectGuid();

    GetPlayer()->SendMessageToSet(&data, false);
}

void WorldSession::HandleMoveKnockBackAck(WorldPacket& recv_data)
{
    DEBUG_LOG("CMSG_MOVE_KNOCK_BACK_ACK");

    Unit* mover = _player->GetMover();
    Player* plMover = mover->GetTypeId() == TYPEID_PLAYER ? (Player*)mover : NULL;

    // ignore, waiting processing in WorldSession::HandleMoveWorldportAckOpcode and WorldSession::HandleMoveTeleportAck
    if (plMover && plMover->IsBeingTeleported())
    {
        recv_data.rpos(recv_data.wpos());                   // prevent warnings spam
        return;
    }

    ObjectGuid guid;
    MovementInfo movementInfo;

    recv_data >> guid;
    recv_data >> Unused<uint32>();                          // knockback packets counter
    recv_data >> movementInfo;

    if (guid && !VerifyMovementInfo(movementInfo, guid))
        return;

	HandleMoverRelocation(movementInfo, recv_data.GetOpcode());

    WorldPacket data(MSG_MOVE_KNOCK_BACK, recv_data.size() + 15);
    data << mover->GetObjectGuid();
    data << movementInfo;
    data << movementInfo.GetJumpInfo().sinAngle;
    data << movementInfo.GetJumpInfo().cosAngle;
    data << movementInfo.GetJumpInfo().xyspeed;
    data << movementInfo.GetJumpInfo().velocity;
    mover->SendMessageToSetExcept(&data, _player);
}

void WorldSession::SendKnockBack(float angle, float horizontalSpeed, float verticalSpeed)
{
    float vsin = sin(angle);
    float vcos = cos(angle);

    WorldPacket data(SMSG_MOVE_KNOCK_BACK, 9 + 4 + 4 + 4 + 4 + 4);
    data << GetPlayer()->GetPackGUID();
    data << uint32(0);                                  // Sequence
    data << float(vcos);                                // x direction
    data << float(vsin);                                // y direction
    data << float(horizontalSpeed);                     // Horizontal speed
    data << float(-verticalSpeed);                      // Z Movement speed (vertical)
    SendPacket(&data);
}

void WorldSession::HandleMoveHoverAck(WorldPacket& recv_data)
{
    DEBUG_LOG("CMSG_MOVE_HOVER_ACK");

    MovementInfo movementInfo;

    recv_data >> Unused<uint64>();                          // guid
    recv_data >> Unused<uint32>();                          // unk
    recv_data >> movementInfo;
    recv_data >> Unused<uint32>();                          // unk2
}

void WorldSession::HandleMoveWaterWalkAck(WorldPacket& recv_data)
{
    DEBUG_LOG("CMSG_MOVE_WATER_WALK_ACK");

    MovementInfo movementInfo;

    recv_data.read_skip<uint64>();                          // guid
    recv_data.read_skip<uint32>();                          // unk
    recv_data >> movementInfo;
    recv_data >> Unused<uint32>();                          // unk2
}

void WorldSession::HandleSummonResponseOpcode(WorldPacket& recv_data)
{
    if (!_player->isAlive() || _player->isInCombat())
        return;

    ObjectGuid summonerGuid;
    recv_data >> summonerGuid;

    _player->SummonIfPossible(true);
}

bool WorldSession::VerifyMovementInfo(MovementInfo const& movementInfo, ObjectGuid const& guid) const
{
    // ignore wrong guid (player attempt cheating own session for not own guid possible...)
    if (guid != _player->GetMover()->GetObjectGuid())
        return false;

    return VerifyMovementInfo(movementInfo);
}

bool WorldSession::VerifyMovementInfo(MovementInfo const& movementInfo) const
{
    if (!MaNGOS::IsValidMapCoord(movementInfo.GetPos()->x, movementInfo.GetPos()->y, movementInfo.GetPos()->z, movementInfo.GetPos()->o))
        return false;

    if (movementInfo.HasMovementFlag(MOVEFLAG_ONTRANSPORT))
    {
        // transports size limited
        // (also received at zeppelin/lift leave by some reason with t_* as absolute in continent coordinates, can be safely skipped)
        if (movementInfo.GetTransportPos()->x > 50 || movementInfo.GetTransportPos()->y > 50 || movementInfo.GetTransportPos()->z > 100)
            return false;

        if (!MaNGOS::IsValidMapCoord(movementInfo.GetPos()->x + movementInfo.GetTransportPos()->x, movementInfo.GetPos()->y + movementInfo.GetTransportPos()->y,
                                     movementInfo.GetPos()->z + movementInfo.GetTransportPos()->z, movementInfo.GetPos()->o + movementInfo.GetTransportPos()->o))
        {
            return false;
        }
    }

    return true;
}

void WorldSession::HandleMoverRelocation(MovementInfo& movementInfo, uint16 opcode)
{
    if (m_clientTimeDelay == 0)
        m_clientTimeDelay = WorldTimer::getMSTime() - movementInfo.GetTime();
    movementInfo.UpdateTime(movementInfo.GetTime() + m_clientTimeDelay + MOVEMENT_PACKET_TIME_DELAY);

    Unit* mover = _player->GetMover();

    if (Player* plMover = mover->GetTypeId() == TYPEID_PLAYER ? (Player*)mover : NULL)
    {
        if (movementInfo.HasMovementFlag(MOVEFLAG_ONTRANSPORT))
        {
            if (!plMover->m_transport)
            {
				float trans_rad = movementInfo.GetTransportPos()->x*movementInfo.GetTransportPos()->x + movementInfo.GetTransportPos()->y*movementInfo.GetTransportPos()->y + movementInfo.GetTransportPos()->z*movementInfo.GetTransportPos()->z;
                if (trans_rad > 3600.0f) // transport radius = 60 yards //cheater with on_transport_flag
                {
	                return;
                }
                // elevators also cause the client to send MOVEFLAG_ONTRANSPORT - just unmount if the guid can be found in the transport list
                for (MapManager::TransportSet::const_iterator iter = sMapMgr.m_Transports.begin(); iter != sMapMgr.m_Transports.end(); ++iter)
                {
                    if ((*iter)->GetObjectGuid() == movementInfo.GetTransportGuid())
                    {
                        plMover->m_transport = (*iter);
                        (*iter)->AddPassenger(plMover);
                        break;
                    }
                }
            }
        }
        else if (plMover->m_transport)               // if we were on a transport, leave
        {
            plMover->m_transport->RemovePassenger(plMover);
            plMover->m_transport = NULL;
            movementInfo.ClearTransportData();
        }

        if (movementInfo.HasMovementFlag(MOVEFLAG_SWIMMING) != plMover->IsInWater())
        {
            // now client not include swimming flag in case jumping under water
            plMover->SetInWater( !plMover->IsInWater() || plMover->IsUnderWater() );
        if(plMover->IsUnderWater())
        {
            plMover->m_anti_BeginFallZ=INVALID_HEIGHT;
        }
	    }

    // ---- anti-cheat features -->>>
    uint32 Anti_TeleTimeDiff=plMover ? time(NULL) - plMover->Anti__GetLastTeleTime() : time(NULL);
    static const uint32 Anti_TeleTimeIgnoreDiff=sWorld.GetMvAnticheatIgnoreAfterTeleport();
    if (plMover && (plMover->m_transport == 0) && sWorld.GetMvAnticheatEnable() &&
        GetPlayer()->GetSession()->GetSecurity() <= sWorld.GetMvAnticheatGmLevel() &&
        GetPlayer()->GetMotionMaster()->GetCurrentMovementGeneratorType()!=FLIGHT_MOTION_TYPE &&
        Anti_TeleTimeDiff>Anti_TeleTimeIgnoreDiff)
    {
        const uint32 CurTime=WorldTimer::getMSTime();
        if(WorldTimer::getMSTimeDiff(GetPlayer()->m_anti_lastalarmtime,CurTime) > sWorld.GetMvAnticheatAlarmPeriod())
        {
            GetPlayer()->m_anti_alarmcount = 0;
        }
        /* I really don't care about movement-type yet (todo)
        UnitMoveType move_type;

        if (movementInfo.flags & MOVEMENTFLAG_FLYING) move_type = MOVE_FLY;
        else if (movementInfo.flags & MOVEMENTFLAG_SWIMMING) move_type = MOVE_SWIM;
        else if (movementInfo.flags & MOVEMENTFLAG_WALK_MODE) move_type = MOVE_WALK;
        else move_type = MOVE_RUN;*/

        float delta_x = GetPlayer()->GetPositionX() - movementInfo.GetPos()->x;
        float delta_y = GetPlayer()->GetPositionY() - movementInfo.GetPos()->y;
        float delta_z = GetPlayer()->GetPositionZ() - movementInfo.GetPos()->z;
        float delta = sqrt(delta_x * delta_x + delta_y * delta_y); // Len of movement-vector via Pythagoras (a^2+b^2=Len^2)
        float tg_z = 0.0f; //tangens
        float delta_t = WorldTimer::getMSTimeDiff(GetPlayer()->m_anti_lastmovetime,CurTime);
		if (delta > 1 && (GetPlayer()->getClass() == CLASS_WARLOCK || GetPlayer()->getClass() == CLASS_HUNTER || GetPlayer()->getClass() == CLASS_PRIEST))
		{
			uint32 time_now = time(NULL);
			uint32 time_limit = time_now - 2 * 60;
			QueryResult* result = LoginDatabase.PQuery("SELECT spell_id, start_time, x ,y, z, end_time FROM cheater_filter WHERE guid = %u and start_time >= %u ", GetPlayer()->GetGUIDLow(), time_limit);
			if (result)
			{
				do
				{
					Field *fields = result->Fetch();
					uint32 spell_id = fields[0].GetUInt32();
					uint32 start_time = fields[1].GetUInt32();
					float x = fields[2].GetFloat();
					float y = fields[3].GetFloat();
					float z = fields[4].GetFloat();
					uint32 end_time = fields[5].GetUInt32();
					if (end_time > 0)
					{
						//uint32 spell_time = GetSpellDuration(spellInfo) / 1000;
						if (time_now >= end_time && time_now <= end_time + 1)
						{
							float delta_x_2 = GetPlayer()->GetPositionX() - x;
							float delta_y_2 = GetPlayer()->GetPositionY() - y;
							float delta_2 = sqrt(delta_x_2 * delta_x_2 + delta_y_2 * delta_y_2);

							float delta_x_3 = movementInfo.GetPos()->x - x;
							float delta_y_3 = movementInfo.GetPos()->y - y;
							float delta_3 = sqrt(delta_x_3 * delta_x_3 + delta_y_3 * delta_y_3);
							delta = delta_2 > delta_3 ? delta_3 : delta_2;
							break;
						}
					}
				} while (result->NextRow());
				delete result;
			}
		}
        GetPlayer()->m_anti_lastmovetime = CurTime;
		GetPlayer()->m_anti_MovedLen += delta;

        if(delta_t > 15000.0f)
        {   delta_t = 15000.0f;   }

        // Tangens of walking angel
        /*if (!(movementInfo.flags & (MOVEMENTFLAG_FLYING | MOVEMENTFLAG_SWIMMING)))
        {
            //Mount hack detection currently disabled
            tg_z = ((delta !=0.0f) && (delta_z > 0.0f)) ? (atan((delta_z*delta_z) / delta) * 180.0f / M_PI) : 0.0f;
        }*/

        //antiOFF fall-damage, MOVEMENTFLAG_UNK4 seted by client if player try movement when falling and unset in this case the MOVEMENTFLAG_FALLING flag.
        
        /*if((GetPlayer()->m_anti_BeginFallZ == INVALID_HEIGHT) && (movementInfo.GetMovementFlags() & (MOVEFLAG_FALLING | MOVEFLAG_FALLINGFAR)) != 0)
        {
            GetPlayer()->m_anti_BeginFallZ=(float)(movementInfo.GetPos()->z);
        }*/

		if (!GetPlayer()->m_movementInfo.HasMovementFlag(MovementFlags(MOVEFLAG_FALLING | MOVEFLAG_FALLINGFAR)))
		{
        if (GetPlayer()->HasAuraType(SPELL_AURA_MOD_INCREASE_MOUNTED_SPEED))
		{
			int32 main_speed_mod  = GetPlayer()->GetMaxPositiveAuraModifier(SPELL_AURA_MOD_INCREASE_MOUNTED_SPEED);
			if (main_speed_mod <= 60)
			{
                if(GetPlayer()->m_anti_NextLenCheck <= CurTime)
                {
                   // Check every 500ms is a lot more advisable then 1000ms, because normal movment packet arrives every 500ms
                   uint32 OldNextLenCheck=GetPlayer()->m_anti_NextLenCheck;
                   float delta_xyt=GetPlayer()->m_anti_MovedLen/(float)(WorldTimer::getMSTimeDiff(OldNextLenCheck-500,CurTime));
                   GetPlayer()->m_anti_NextLenCheck = CurTime+500;
                   GetPlayer()->m_anti_MovedLen = 0.0f;
                   static const float MaxDeltaXYTA = sWorld.GetMvAnticheatMaxXPTZQ();

#ifdef __ANTI_DEBUG__
            SendAreaTriggerMessage("XYT: %f ; Flags: %s",delta_xyt,FlagsToStr(movementInfo.flags).c_str());
#endif //__ANTI_DEBUG__
            
                   if(delta_xyt > MaxDeltaXYTA && delta<=100.0f && GetPlayer()->GetZoneId() != 2257)
                   {
                      Anti__CheatOccurred(CurTime,"Speed hack",delta_xyt,NULL,
                      (float)(GetPlayer()->GetMotionMaster()->GetCurrentMovementGeneratorType()),
                      (float)(WorldTimer::getMSTimeDiff(OldNextLenCheck-500,CurTime)),&movementInfo);
                   }
			    }
			 }
			else
				if(GetPlayer()->m_anti_NextLenCheck <= CurTime)
                {
                   // Check every 500ms is a lot more advisable then 1000ms, because normal movment packet arrives every 500ms
                   uint32 OldNextLenCheck=GetPlayer()->m_anti_NextLenCheck;
                   float delta_xyt=GetPlayer()->m_anti_MovedLen/(float)(WorldTimer::getMSTimeDiff(OldNextLenCheck-500,CurTime));
                   GetPlayer()->m_anti_NextLenCheck = CurTime+500;
                   GetPlayer()->m_anti_MovedLen = 0.0f;
                   static const float MaxDeltaXYTB = sWorld.GetMvAnticheatMaxXGJZQ();

#ifdef __ANTI_DEBUG__
            SendAreaTriggerMessage("XYT: %f ; Flags: %s",delta_xyt,FlagsToStr(movementInfo.flags).c_str());
#endif //__ANTI_DEBUG__
            
                   if(delta_xyt > MaxDeltaXYTB && delta<=100.0f && GetPlayer()->GetZoneId() != 2257)
                   {
                      Anti__CheatOccurred(CurTime,"Speed hack",delta_xyt,NULL,
                      (float)(GetPlayer()->GetMotionMaster()->GetCurrentMovementGeneratorType()),
                      (float)(WorldTimer::getMSTimeDiff(OldNextLenCheck-500,CurTime)),&movementInfo);
                   }
			    }
         }
		else
			if (GetPlayer()->HasAuraType(SPELL_AURA_MOD_INCREASE_SPEED))
			{
				int32 main_speed_gh  = GetPlayer()->GetMaxPositiveAuraModifier(SPELL_AURA_MOD_INCREASE_SPEED);
				if (main_speed_gh >= 50)
				{
				    if(GetPlayer()->m_anti_NextLenCheck <= CurTime)
				    {
				       // Check every 500ms is a lot more advisable then 1000ms, because normal movment packet arrives every 500ms
                       uint32 OldNextLenCheck=GetPlayer()->m_anti_NextLenCheck;
                       float delta_xyt=GetPlayer()->m_anti_MovedLen/(float)(WorldTimer::getMSTimeDiff(OldNextLenCheck-500,CurTime));
                       GetPlayer()->m_anti_NextLenCheck = CurTime+500;
                       GetPlayer()->m_anti_MovedLen = 0.0f;
                       static const float MaxDeltaXYTC = sWorld.GetMvAnticheatMaxXGJGH();

#ifdef __ANTI_DEBUG__
            SendAreaTriggerMessage("XYT: %f ; Flags: %s",delta_xyt,FlagsToStr(movementInfo.flags).c_str());
#endif //__ANTI_DEBUG__
            
                       if(delta_xyt > MaxDeltaXYTC && delta<=100.0f && GetPlayer()->GetZoneId() != 2257)
                       {
                          Anti__CheatOccurred(CurTime,"Speed hack",delta_xyt,NULL,
                          (float)(GetPlayer()->GetMotionMaster()->GetCurrentMovementGeneratorType()),
                          (float)(WorldTimer::getMSTimeDiff(OldNextLenCheck-500,CurTime)),&movementInfo);
                       }
				    }
				}
				else
				if (GetPlayer()->HasAuraType(SPELL_AURA_MOD_SHAPESHIFT) && GetPlayer()->HasAuraType(SPELL_AURA_MOD_SPEED_ALWAYS))
				{
					if (GetPlayer()->m_anti_NextLenCheck <= CurTime)
					{
						// Check every 500ms is a lot more advisable then 1000ms, because normal movment packet arrives every 500ms
						uint32 OldNextLenCheck = GetPlayer()->m_anti_NextLenCheck;
						float delta_xyt = GetPlayer()->m_anti_MovedLen / (float)(WorldTimer::getMSTimeDiff(OldNextLenCheck - 500, CurTime));
						GetPlayer()->m_anti_NextLenCheck = CurTime + 500;
						GetPlayer()->m_anti_MovedLen = 0.0f;
						static const float MaxDeltaXDBX = sWorld.GetMvAnticheatMaxXDBX();

#ifdef __ANTI_DEBUG__
						SendAreaTriggerMessage("XYT: %f ; Flags: %s", delta_xyt, FlagsToStr(movementInfo.flags).c_str());
#endif //__ANTI_DEBUG__

						if (delta_xyt > MaxDeltaXDBX && delta <= 100.0f && GetPlayer()->GetZoneId() != 2257)
						{
							Anti__CheatOccurred(CurTime, "Speed hack", delta_xyt, NULL,
								(float)(GetPlayer()->GetMotionMaster()->GetCurrentMovementGeneratorType()),
								(float)(WorldTimer::getMSTimeDiff(OldNextLenCheck - 500, CurTime)), &movementInfo);
						}
					}
				}
				else
					if(GetPlayer()->m_anti_NextLenCheck <= CurTime)
				    {
				       // Check every 500ms is a lot more advisable then 1000ms, because normal movment packet arrives every 500ms
                       uint32 OldNextLenCheck=GetPlayer()->m_anti_NextLenCheck;
                       float delta_xyt=GetPlayer()->m_anti_MovedLen/(float)(WorldTimer::getMSTimeDiff(OldNextLenCheck-500,CurTime));
                       GetPlayer()->m_anti_NextLenCheck = CurTime+500;
                       GetPlayer()->m_anti_MovedLen = 0.0f;
                       static const float MaxDeltaXYTD = sWorld.GetMvAnticheatMaxXPTGH();

#ifdef __ANTI_DEBUG__
            SendAreaTriggerMessage("XYT: %f ; Flags: %s",delta_xyt,FlagsToStr(movementInfo.flags).c_str());
#endif //__ANTI_DEBUG__
            
                       if(delta_xyt > MaxDeltaXYTD && delta<=100.0f && GetPlayer()->GetZoneId() != 2257)
                       {
                          Anti__CheatOccurred(CurTime,"Speed hack",delta_xyt,NULL,
                          (float)(GetPlayer()->GetMotionMaster()->GetCurrentMovementGeneratorType()),
                          (float)(WorldTimer::getMSTimeDiff(OldNextLenCheck-500,CurTime)),&movementInfo);
                       }
				    }
			}
			else
				if (GetPlayer()->HasAuraType(SPELL_AURA_MOD_INCREASE_SWIM_SPEED))
			    {
				    int32 main_speed_swim  = GetPlayer()->GetMaxPositiveAuraModifier(SPELL_AURA_MOD_INCREASE_SWIM_SPEED);
				    if (main_speed_swim <= 100)
				    {
				        if(GetPlayer()->m_anti_NextLenCheck <= CurTime)
				        {
				           // Check every 500ms is a lot more advisable then 1000ms, because normal movment packet arrives every 500ms
                           uint32 OldNextLenCheck=GetPlayer()->m_anti_NextLenCheck;
                           float delta_xyt=GetPlayer()->m_anti_MovedLen/(float)(WorldTimer::getMSTimeDiff(OldNextLenCheck-500,CurTime));
                           GetPlayer()->m_anti_NextLenCheck = CurTime+500;
                           GetPlayer()->m_anti_MovedLen = 0.0f;
                           static const float MaxDeltaGJYY = sWorld.GetMvAnticheatMaxXGJYY();

#ifdef __ANTI_DEBUG__
            SendAreaTriggerMessage("XYT: %f ; Flags: %s",delta_xyt,FlagsToStr(movementInfo.flags).c_str());
#endif //__ANTI_DEBUG__
            
                           if(delta_xyt > MaxDeltaGJYY && delta<=100.0f && GetPlayer()->GetZoneId() != 2257)
                           {
                              Anti__CheatOccurred(CurTime,"Speed hack",delta_xyt,NULL,
                              (float)(GetPlayer()->GetMotionMaster()->GetCurrentMovementGeneratorType()),
                              (float)(WorldTimer::getMSTimeDiff(OldNextLenCheck-500,CurTime)),&movementInfo);
                           }
				        }
				    }
			    }
		   else
		   if (GetPlayer()->HasAuraType(SPELL_AURA_MOD_SHAPESHIFT) && GetPlayer()->HasAuraType(SPELL_AURA_MOD_SPEED_ALWAYS))
		   {
			   int32 main_speed_swim = GetPlayer()->GetMaxPositiveAuraModifier(SPELL_AURA_MOD_SPEED_ALWAYS);
			   if (main_speed_swim >= 15)
			   {
				   if (GetPlayer()->m_anti_NextLenCheck <= CurTime)
				   {
					   // Check every 500ms is a lot more advisable then 1000ms, because normal movment packet arrives every 500ms
					   uint32 OldNextLenCheck = GetPlayer()->m_anti_NextLenCheck;
					   float delta_xyt = GetPlayer()->m_anti_MovedLen / (float)(WorldTimer::getMSTimeDiff(OldNextLenCheck - 500, CurTime));
					   GetPlayer()->m_anti_NextLenCheck = CurTime + 500;
					   GetPlayer()->m_anti_MovedLen = 0.0f;
					   static const float MaxDeltaXDBX = sWorld.GetMvAnticheatMaxXDBX();

#ifdef __ANTI_DEBUG__
					   SendAreaTriggerMessage("XYT: %f ; Flags: %s", delta_xyt, FlagsToStr(movementInfo.flags).c_str());
#endif //__ANTI_DEBUG__

					   if (delta_xyt > MaxDeltaXDBX && delta <= 100.0f && GetPlayer()->GetZoneId() != 2257)
					   {
						   Anti__CheatOccurred(CurTime, "Speed hack", delta_xyt, NULL,
							   (float)(GetPlayer()->GetMotionMaster()->GetCurrentMovementGeneratorType()),
							   (float)(WorldTimer::getMSTimeDiff(OldNextLenCheck - 500, CurTime)), &movementInfo);
					   }
				   }
			   }
		   }
		   else
           if(GetPlayer()->m_anti_NextLenCheck <= CurTime)
           {
              // Check every 500ms is a lot more advisable then 1000ms, because normal movment packet arrives every 500ms
              uint32 OldNextLenCheck=GetPlayer()->m_anti_NextLenCheck;
              float delta_xyt=GetPlayer()->m_anti_MovedLen/(float)(WorldTimer::getMSTimeDiff(OldNextLenCheck-500,CurTime));
              GetPlayer()->m_anti_NextLenCheck = CurTime+500;
              GetPlayer()->m_anti_MovedLen = 0.0f;
              static const float MaxDeltaXYT = sWorld.GetMvAnticheatMaxXYT();

#ifdef __ANTI_DEBUG__
            SendAreaTriggerMessage("XYT: %f ; Flags: %s",delta_xyt,FlagsToStr(movementInfo.flags).c_str());
#endif //__ANTI_DEBUG__
            
              if(delta_xyt > MaxDeltaXYT && delta<=100.0f && GetPlayer()->GetZoneId() != 2257)
              {
				  Spell* curSpell = GetPlayer()->GetCurrentSpell(CURRENT_CHANNELED_SPELL);
				  if (curSpell && (curSpell->m_spellInfo->Id == 605 || curSpell->m_spellInfo->Id == 10911 || curSpell->m_spellInfo->Id == 10912))
				  {
					  sLog.outBug("speed hack player");
				  }
				  else
				  {
					  Anti__CheatOccurred(CurTime, "Speed hack", delta_xyt, NULL,
						  (float)(GetPlayer()->GetMotionMaster()->GetCurrentMovementGeneratorType()),
						  (float)(WorldTimer::getMSTimeDiff(OldNextLenCheck - 500, CurTime)), &movementInfo);
				  }
               }
			}
		}

        if(delta > 100.0f && GetPlayer()->GetZoneId() != 2257)
        {
            Anti__ReportCheat("Tele hack",delta,NULL);
        }
		float DIFF_OVERGROUND = 3.0f;
		float Anti__GroundZ = GetPlayer()->GetMap()->GetHeight(GetPlayer()->GetPositionX(), GetPlayer()->GetPositionY(), MAX_HEIGHT);
		float Anti__FloorZ = GetPlayer()->GetMap()->GetHeight(GetPlayer()->GetPositionX(), GetPlayer()->GetPositionY(), GetPlayer()->GetPositionZ());
		float Anti__MapZ = ((Anti__FloorZ <= (INVALID_HEIGHT + 5.0f)) ? Anti__GroundZ : Anti__FloorZ) + DIFF_OVERGROUND;
		if (!movementInfo.HasMovementFlag(MOVEFLAG_ONTRANSPORT) &&
			!movementInfo.HasMovementFlag(MOVEFLAG_FALLING) &&
			!GetPlayer()->IsTaxiFlying() &&
			!GetPlayer()->GetTerrain()->IsUnderWater(movementInfo.GetPos()->x, movementInfo.GetPos()->y, movementInfo.GetPos()->z - 7.0f) &&
			Anti__MapZ < GetPlayer()->GetPositionZ() && Anti__MapZ >(INVALID_HEIGHT + DIFF_OVERGROUND + 5.0f))
		{
			// Air-Jump-Detection definitively needs a better way to be detected...
			if (opcode == MSG_MOVE_JUMP) // Fly Hack
			{
				Anti__CheatOccurred(CurTime, "Fly hack", ((uint8)(GetPlayer()->HasAuraType(SPELL_AURA_MOD_INCREASE_SPEED)) * 2), NULL, GetPlayer()->GetPositionZ() - Anti__MapZ);
			}
		}
    } 
    // <<---- anti-cheat features

        plMover->SetPosition(movementInfo.GetPos()->x, movementInfo.GetPos()->y, movementInfo.GetPos()->z, movementInfo.GetPos()->o);
        plMover->m_movementInfo = movementInfo;

        if (movementInfo.GetPos()->z < -500.0f)
        {
            if (plMover->GetBattleGround()
                    && plMover->GetBattleGround()->HandlePlayerUnderMap(_player))
            {
                // do nothing, the handle already did if returned true
            }
            else
            {
                // NOTE: this is actually called many times while falling
                // even after the player has been teleported away
                // TODO: discard movement packets after the player is rooted
                if (plMover->isAlive())
                {
                    plMover->EnvironmentalDamage(DAMAGE_FALL_TO_VOID, plMover->GetMaxHealth());
                    // pl can be alive if GM/etc
                    if (!plMover->isAlive())
                    {
                        // change the death state to CORPSE to prevent the death timer from
                        // starting in the next player update
                        plMover->KillPlayer();
                        plMover->BuildPlayerRepop();
                    }
                }

                // cancel the death timer here if started
                plMover->RepopAtGraveyard();
            }
        }
    }
    else                                                    // creature charmed
    {
        if (mover->IsInWorld())
            mover->GetMap()->CreatureRelocation((Creature*)mover, movementInfo.GetPos()->x, movementInfo.GetPos()->y, movementInfo.GetPos()->z, movementInfo.GetPos()->o);
    }
}
