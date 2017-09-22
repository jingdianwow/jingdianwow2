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

#ifndef WORLD_PVP_ZG
#define WORLD_PVP_ZG

#include "Common.h"
#include "../../OutdoorPvP.h"
#include "Language.h"

enum
{
  SPELL_ECHOES_ZG_1                   = 37031,
  SPELL_ECHOES_ZG_2                   = 37032,
  SPELL_ECHOES_ZG_3                   = 37033,
  SPELL_ECHOES_ZG_4                   = 37034,
  SPELL_ECHOES_ZG_5                   = 37035,
  SPELL_ECHOES_ZG_6                   = 37036,
  SPELL_ECHOES_ZG_7                   = 37037,
  SPELL_ECHOES_ZG_8                   = 37038,
  SPELL_ECHOES_ZG_9                   = 37039,
  SPELL_ECHOES_ZG_10                  = 37040,
  SPELL_ECHOES_ZG_11                  = 37041,
  SPELL_ECHOES_ZG_12                  = 37042,
  SPELL_ECHOES_ZG_13                  = 37043,
  SPELL_ECHOES_ZG_14                  = 37044,
  SPELL_ECHOES_ZG_15                  = 37045,
  SPELL_ECHOES_ZG_16                  = 37046,
  SPELL_ECHOES_ZG_17                  = 37047,
  SPELL_ECHOES_ZG_18                  = 37048,
  SPELL_ECHOES_ZG_19                  = 37049,
  SPELL_ECHOES_ZG_20                  = 37050,
  MAP_ID_ZG                          = 309,
  MAX_ZG_SPELL                        = 20,
};

struct PlayerBufff
{
    uint32 spellId;
};

static const PlayerBufff PlayerBuffZg[MAX_ZG_SPELL] =
{
    {SPELL_ECHOES_ZG_1},
    {SPELL_ECHOES_ZG_2},
    {SPELL_ECHOES_ZG_3},
	{SPELL_ECHOES_ZG_4},
    {SPELL_ECHOES_ZG_5},
	{SPELL_ECHOES_ZG_6},
	{SPELL_ECHOES_ZG_7},
	{SPELL_ECHOES_ZG_8},
	{SPELL_ECHOES_ZG_9},
	{SPELL_ECHOES_ZG_10},
	{SPELL_ECHOES_ZG_11},
	{SPELL_ECHOES_ZG_12},
	{SPELL_ECHOES_ZG_13},
	{SPELL_ECHOES_ZG_14},
	{SPELL_ECHOES_ZG_15},
	{SPELL_ECHOES_ZG_16},
	{SPELL_ECHOES_ZG_17},
	{SPELL_ECHOES_ZG_18},
	{SPELL_ECHOES_ZG_19},
	{SPELL_ECHOES_ZG_20}
};

class OutdoorPvPZG : public OutdoorPvP
{
    public:
        OutdoorPvPZG();

		void Update(uint32 diff) override;

    private:
};

#endif