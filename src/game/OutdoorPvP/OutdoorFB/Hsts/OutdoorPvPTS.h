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

#ifndef WORLD_PVP_TS
#define WORLD_PVP_TS

#include "Common.h"
#include "../../OutdoorPvP.h"
#include "Language.h"

enum
{
	SPELL_ECHOES_TX_1 = 37021,
	SPELL_ECHOES_TX_2 = 37022,
	SPELL_ECHOES_TX_3 = 37023,
	SPELL_ECHOES_TX_4 = 37024,
	SPELL_ECHOES_TX_5 = 37025,
	SPELL_ECHOES_TX_6 = 37026,
	SPELL_ECHOES_TX_7 = 37027,
	SPELL_ECHOES_TX_8 = 37028,
	SPELL_ECHOES_TX_9 = 37029,
	SPELL_ECHOES_TX_10 = 37030,
	MAP_ID_TX = 229,
	MAX_TX_SPELL = 10,
};

struct PlayerBuffe
{
	uint32 spellId;
};

static const PlayerBuffe PlayerBuffTx[MAX_TX_SPELL] =
{
	{ SPELL_ECHOES_TX_1 },
	{ SPELL_ECHOES_TX_2 },
	{ SPELL_ECHOES_TX_3 },
	{ SPELL_ECHOES_TX_4 },
	{ SPELL_ECHOES_TX_5 },
	{ SPELL_ECHOES_TX_6 },
	{ SPELL_ECHOES_TX_7 },
	{ SPELL_ECHOES_TX_8 },
	{ SPELL_ECHOES_TX_9 },
	{ SPELL_ECHOES_TX_10 }
};

class OutdoorPvPTS : public OutdoorPvP
{
public:
	OutdoorPvPTS();

	void Update(uint32 diff) override;

private:
};

#endif