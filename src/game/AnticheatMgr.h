/*
*
*	Made by: ???
*	Remaking and Improvements by: mthsena
*
*/

#ifndef SC_ACMGR_H
#define SC_ACMGR_H

#include "Common.h"
#include "AnticheatData.h"
#include "World.h"
#include "Player.h"
#include "Config/Config.h"
#include "Chat.h"
#include "Object.h"
#include "MapManager.h"

class AnticheatData;
typedef std::map<uint32, AnticheatData> AnticheatPlayersDataMap;

enum hackTypes
{
	SPEED_HACK,
	FLY_HACK,
	WALKWATER_HACK,
	WALLCLIMB_HACK,
	JUMP_HACK,
	TELEPORTPLANE_HACK,
};

struct ReportsInfo
{
	uint32 Times;
};
static std::map<uint32, ReportsInfo> reportTimes;

class ClearTimes : public BasicEvent
{
public:
	ClearTimes() {}

	bool Execute(uint64 /*time*/, uint32 /*diff*/)
	{
		reportTimes.clear();
		return true;
	}
};

class AnticheatMgr
{
	friend class ACE_Singleton<AnticheatMgr, ACE_Null_Mutex>;
	AnticheatMgr();

private:
	void SpeedHackDetection(Player* player, MovementInfo movementInfo, uint32 opcode);
	void FlyHackDetection(Player* player, MovementInfo movementInfo);
	void WalkOnWaterHackDetection(Player* player, MovementInfo movementInfo, uint32 opcode);
	void ClimbHackDetection(Player* player, MovementInfo movementInfo, uint32 opcode);
	void JumpHackDetection(Player* player, MovementInfo movementInfo, uint32 opcode);
	void TeleportPlaneHackDetection(Player* player, MovementInfo, uint32 opcode);

	void Alerts(Player* player, uint8 alertType, uint8 hackType);
	void Report(Player* player, uint8 hackType);

	AnticheatPlayersDataMap m_Players;

public:
	void StartHackDetection(Player* player, MovementInfo movementInfo, uint32 opcode);

};
#define sAnticheatMgr ACE_Singleton<AnticheatMgr, ACE_Null_Mutex>::instance()

#endif