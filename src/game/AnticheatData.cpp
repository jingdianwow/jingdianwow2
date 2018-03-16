/*
*
*    Made by: ???
*    Remaking and Improvements by: mthsena
*
*/

#include "AnticheatData.h"

AnticheatData::AnticheatData()
{
}

void AnticheatData::SetLastOpcode(uint32 opcode)
{
    lastOpcode = opcode;
}

void AnticheatData::SetPosition(float x, float y, float z, float o)
{
    lastMovementInfo.pos.x = x;
    lastMovementInfo.pos.y = y;
    lastMovementInfo.pos.z = z;
    lastMovementInfo.pos.o = o;
}

uint32 AnticheatData::GetLastOpcode() const
{
    return lastOpcode;
}

const MovementInfo& AnticheatData::GetLastMovementInfo() const
{
    return lastMovementInfo;
}

void AnticheatData::SetLastMovementInfo(MovementInfo& moveInfo)
{
    lastMovementInfo = moveInfo;
}