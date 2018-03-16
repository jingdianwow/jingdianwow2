/*
*
*    Made by: ???
*    Remaking and Improvements by: mthsena
*
*/

#include "AnticheatMgr.h"
#include "World.h"
#include "Config/Config.h"
#include "Chat.h"
#include "Timer.h"
#include "Player.h"
#include "Unit.h"

#define CLIMB_ANGLE 1.9f

AnticheatMgr::AnticheatMgr()
{
}

void AnticheatMgr::SpeedHackDetection(Player* player,MovementInfo movementInfo, uint32 opcode)
{
    uint32 key = player->GetGUIDLow();

	if (opcode == MSG_MOVE_FALL_LAND || (opcode == MSG_MOVE_STOP && movementInfo.GetMovementFlags() == MOVEFLAG_FALLING))
        return;

	if (movementInfo.HasMovementFlag(MOVEFLAG_FALLING) || m_Players[key].GetLastOpcode() == MSG_MOVE_FALL_LAND)
        return;

    if (player->IsMounted())
        return;
    // We also must check the map because the movementFlag can be modified by the client.
    // If we just check the flag, they could always add that flag and always skip the speed hacking detection.
    // 369 == DEEPRUN TRAM
    if (m_Players[key].GetLastMovementInfo().HasMovementFlag(MOVEFLAG_ONTRANSPORT) || player->GetMapId() == 369)
        return;

    float dx = movementInfo.pos.x - m_Players[key].GetLastMovementInfo().pos.x;
    float dy = movementInfo.pos.y - m_Players[key].GetLastMovementInfo().pos.y;
    float ExactDist2dSq = dx*dx + dy*dy;

    uint32 distance2D = std::sqrt(ExactDist2dSq);
    uint8 moveType = 0;

    // we need to know HOW is the player moving
    // TO-DO: Should we check the incoming movement flags?
    if (player->HasUnitMovementFlag(MOVEFLAG_SWIMMING))
        moveType = MOVE_SWIM;
    else if (player->HasUnitMovementFlag(MOVEFLAG_WALK_MODE))
        moveType = MOVE_WALK;
    else
        moveType = MOVE_RUN;

    // how many yards the player can do in one sec.
    uint32 speedRate = (uint32)(player->GetSpeed(UnitMoveType(moveType)) + movementInfo.jump.xyspeed);

    // how long the player took to move to here.
    uint32 timeDiff = WorldTimer::getMSTimeDiff(m_Players[key].GetLastMovementInfo().time,movementInfo.time);

    if (!timeDiff)
        timeDiff = 1;

    // this is the distance doable by the player in 1 sec, using the time done to move to this point.
    uint32 clientSpeedRate = distance2D * 1000 / timeDiff;

    // we did the (uint32) cast to accept a margin of tolerance
    if (clientSpeedRate > speedRate)
        Report(player, SPEED_HACK);
}

void AnticheatMgr::FlyHackDetection(Player* player, MovementInfo movementInfo)
{
    uint32 key = player->GetGUIDLow();

    if (!m_Players[key].GetLastMovementInfo().HasMovementFlag(MOVEFLAG_FLYING))
        return;

    Report(player, FLY_HACK);
}

void AnticheatMgr::WalkOnWaterHackDetection(Player* player, MovementInfo movementInfo, uint32 opcode)
{
    uint32 key = player->GetGUIDLow();

    if (opcode == MSG_MOVE_FALL_LAND)
        return;
    if (!m_Players[key].GetLastMovementInfo().HasMovementFlag(MOVEFLAG_WATERWALKING))
        return;

    // if we are a ghost we can walk on water
    if (!player->isAlive())
        return;

    if (player->HasAuraType(SPELL_AURA_FEATHER_FALL) || player->HasAuraType(SPELL_AURA_SAFE_FALL) || player->HasAuraType(SPELL_AURA_WATER_WALK))
        return;

    Report(player, WALKWATER_HACK);
}

void AnticheatMgr::ClimbHackDetection(Player *player, MovementInfo movementInfo, uint32 opcode)
{
    uint32 key = player->GetGUIDLow();

    if (opcode != MSG_MOVE_HEARTBEAT || m_Players[key].GetLastOpcode() != MSG_MOVE_HEARTBEAT)
        return;

    // in this case we don't care if they are "legal" flags, they are handled in another parts of the Anticheat Manager.
   // if (player->IsInWater() || player->IsFlying() || player->IsFalling())
     //   return;

    float x, y, z;
    player->GetPosition(x, y, z);

    float deltaZ = fabs(z - movementInfo.pos.z);

    float dx = movementInfo.pos.x - x;
    float dy = movementInfo.pos.y - y;
    float ExactDist2dSq = dx*dx + dy*dy;
    float deltaXY = std::sqrt(ExactDist2dSq);

    float angle = MapManager::NormalizeOrientation(tan(deltaZ/deltaXY));

    if (angle > CLIMB_ANGLE)
        Report(player, WALLCLIMB_HACK);
}

void AnticheatMgr::JumpHackDetection(Player* player, MovementInfo movementInfo,uint32 opcode)
{
    uint32 key = player->GetGUIDLow();

	if ((m_Players[key].GetLastOpcode() == MSG_MOVE_JUMP && opcode == MSG_MOVE_JUMP) || (m_Players[key].GetLastOpcode() == MSG_MOVE_HEARTBEAT && opcode == MSG_MOVE_JUMP && movementInfo.GetMovementFlags() == MOVEFLAG_FALLING))
        Report(player, JUMP_HACK);
}

void AnticheatMgr::TeleportPlaneHackDetection(Player* player, MovementInfo movementInfo, uint32 opcode)
{
    uint32 key = player->GetGUIDLow();

    if (opcode == MSG_MOVE_FALL_LAND || opcode == MSG_MOVE_HEARTBEAT || opcode == MSG_MOVE_STOP)
        return;

	if (movementInfo.HasMovementFlag(MOVEFLAG_FALLING))
        return;
    //if (opcode == MSG_MOVE_STOP && (movementInfo.GetMovementFlags() == MOVEFLAG_JUMPING || movementInfo.GetMovementFlags() == MOVEFLAG_NONE))
        //return;

    float distance = player->GetDistance(movementInfo.pos.x, movementInfo.pos.y, movementInfo.pos.z);
    float TeleportDistance = player->GetDistance(m_Players[key].GetLastMovementInfo().pos.x, m_Players[key].GetLastMovementInfo().pos.y, m_Players[key].GetLastMovementInfo().pos.z);
    if (TeleportDistance <= 40.0f)
        return;

    //if (m_Players[key].GetLastMovementInfo().pos.z == 0 || movementInfo.pos.z == 0)
        //return;
    if (movementInfo.HasMovementFlag(MOVEFLAG_ONTRANSPORT))
        return;

    if (movementInfo.HasMovementFlag(MOVEFLAG_FALLINGFAR))
        return;

    //DEAD_FALLING was deprecated
    if (player->getDeathState() == CORPSE_FALLING)
        return;

    // we are not really walking there
    Report(player, TELEPORTPLANE_HACK);
}

void AnticheatMgr::Alerts(Player* player, uint8 alertType, uint8 hackType)
{
    std::string hType = "";

    switch(hackType)
    {
    case SPEED_HACK:
        hType = "Speed";
        break;

    case FLY_HACK:
        hType = "Fly";
        break;

    case WALKWATER_HACK:
        hType = "WalkOnWater";
        break;

    case WALLCLIMB_HACK:
        hType = "WallClimb";
        break;

    case JUMP_HACK:
        hType = "Jump";
        break;

    case TELEPORTPLANE_HACK:
        hType = "TeleportPlane";
        break;

    default:
        hType = "Unknown";
    }

    // AlertType 0
    std::string alertType0 = "";
    alertType0 = "[Anti-Cheat][" + hType + "][" + std::string(player->GetName()) + "] Check it!";
    WorldPacket data(SMSG_NOTIFICATION, (alertType0.size()+1));
    data << alertType0;

    // AlertType 1
    std::string alertType1 = "|cFFFF0000You will be punished if continue using hacking.";

    // AlertType 2
    std::string alertType2;
	alertType2 += player->GetName();
    alertType2 += "|cFFFF0000 was punished by anticheat, reason: " + hType + " Hacking.";

    switch(alertType)
    {
    case 0:
        //sWorld.SendGlobalMessage(&data);
        break;
    case 1:
        ChatHandler(player->GetSession()).PSendSysMessage(alertType1.c_str());
        break;
    case 2:
        //sWorld.SendGlobalText(alertType2.c_str(), 0);
        break;
    default:
        return;
    }
}

void AnticheatMgr::Report(Player* player, uint8 hackType)
{
    uint32 _times;
    _times = player->GetGUID();

    player->m_Events.AddEvent(new ClearTimes(), player->m_Events.CalculateTime(7500));
    reportTimes[_times].Times++;
    //sLog.outError("AnticheatMgr::Report %u hackType %u", reportTimes[_times].Times, hackType);

    if (hackType == SPEED_HACK)
    {
        switch (reportTimes[_times].Times)
        {
        case 2:
            Alerts(player, 0, hackType);
            reportTimes[_times].Times = 2;
            break;
        case 3:
            Alerts(player, 1, hackType);
            reportTimes[_times].Times = 3;			
            break;
        case 5:
            reportTimes[_times].Times = 5;
            Alerts(player, 3, hackType);
			player->CastSpell(player, 24883, false);
            break;
        case 10:
            reportTimes.clear();
            Alerts(player, 4, hackType);
			player->CastSpell(player, 24883, false);
            break;
        }
        return;
    }

    if (hackType == TELEPORTPLANE_HACK)
    {
        switch (reportTimes[_times].Times)
        {
        case 1:
            Alerts(player, 0, hackType);
            reportTimes[_times].Times = 2;
            break;
        case 3:
            reportTimes[_times].Times = 5;
            Alerts(player, 1, hackType);
            break;
        }
        //如果使用了传送挂，那么就传送回去
        player->TeleportToHomebind();
        //或则使用了外挂，给外挂玩家一个限制移动的BUFF
        player->CastSpell(player, 24883, false);

        return;
    }

    if (hackType == JUMP_HACK)
    {
        switch (reportTimes[_times].Times)
        {
        case 2:
            Alerts(player, 0, hackType);
            reportTimes[_times].Times = 2;
            break;
        case 5:
            reportTimes[_times].Times = 5;
            Alerts(player, 1, hackType);
            break;
        case 6:
        case 7:
        case 8:
            reportTimes[_times].Times = 5;
            Alerts(player, 2, hackType);
			player->CastSpell(player, 24883, false);
            break;
        }
		player->CastSpell(player, 24883, false);
        return;
    }

    if (hackType == WALKWATER_HACK)
    {
        switch (reportTimes[_times].Times)
        {
        case 2:
            Alerts(player, 0, hackType);
            reportTimes[_times].Times = 2;
            break;
        case 3:
            reportTimes[_times].Times = 5;
            Alerts(player, 1, hackType);
            break;
        case 10:
            reportTimes[_times].Times = 5;
            Alerts(player, 2, hackType);
			player->CastSpell(player, 24883, false);
            break;
        }
        return;
    }

    switch (reportTimes[_times].Times)
    {
    case 2:
        Alerts(player, 0, hackType);
        reportTimes[_times].Times = 2;
        break;
    case 5:
        reportTimes[_times].Times = 5;
        Alerts(player, 1, hackType);
        break;
    case 10:
        reportTimes.clear();
        Alerts(player, 2, hackType);
		player->CastSpell(player, 24883, false);
        break;
    }
}

void AnticheatMgr::StartHackDetection(Player* player, MovementInfo movementInfo, uint32 opcode)
{
    uint32 key = player->GetGUIDLow();
    bool enable = sConfig.GetBoolDefault("Anticheat.Enable", true);

    if (!enable)
        return;

    //if (player->GetSession()->GetSecurity() > 0)
        //return;
    //sLog.outError("AnticheatMgr::StartHackDetection opcode %s MovementFlags() %u", LookupOpcodeName(opcode), movementInfo.GetMovementFlags());

    if (player->IsFlying() || player->GetTransport())
    {
        m_Players[key].SetLastMovementInfo(movementInfo);
        m_Players[key].SetLastOpcode(opcode);
        return;
    }

    SpeedHackDetection(player, movementInfo, opcode);//OK
    FlyHackDetection(player, movementInfo);
    WalkOnWaterHackDetection(player, movementInfo, opcode); //OK
    //ClimbHackDetection(player, movementInfo, opcode);//Not tested
    JumpHackDetection(player, movementInfo, opcode); //OK
    TeleportPlaneHackDetection(player, movementInfo, opcode); //OK

    m_Players[key].SetLastMovementInfo(movementInfo);
    m_Players[key].SetLastOpcode(opcode);
}