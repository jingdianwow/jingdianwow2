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

#ifndef WORLD_PVP_FX
#define WORLD_PVP_FX

#include "Common.h"
#include "../../OutdoorPvP.h"
#include "Language.h"

enum
{
  SPELL_ECHOES_FX_1                   = 37051,
  SPELL_ECHOES_FX_2                   = 37052,
  SPELL_ECHOES_FX_3                   = 37053,
  SPELL_ECHOES_FX_4                   = 37054,
  SPELL_ECHOES_FX_5                   = 37055,
  SPELL_ECHOES_FX_6                   = 37056,
  SPELL_ECHOES_FX_7                   = 37057,
  SPELL_ECHOES_FX_8                   = 37058,
  SPELL_ECHOES_FX_9                   = 37059,
  SPELL_ECHOES_FX_10                  = 37060,
  SPELL_ECHOES_FX_11                  = 37061,
  SPELL_ECHOES_FX_12                  = 37062,
  SPELL_ECHOES_FX_13                  = 37063,
  SPELL_ECHOES_FX_14                  = 37064,
  SPELL_ECHOES_FX_15                  = 37065,
  SPELL_ECHOES_FX_16                  = 37066,
  SPELL_ECHOES_FX_17                  = 37067,
  SPELL_ECHOES_FX_18                  = 37068,
  SPELL_ECHOES_FX_19                  = 37069,
  SPELL_ECHOES_FX_20                  = 37070,
  MAP_ID_FX                          = 509,
  MAX_FX_SPELL                        = 20,
};

struct PlayerBuffg
{
    uint32 spellId;
};

static const PlayerBuffg PlayerBuffFx[MAX_FX_SPELL] =
{
    {SPELL_ECHOES_FX_1},
    {SPELL_ECHOES_FX_2},
    {SPELL_ECHOES_FX_3},
	{SPELL_ECHOES_FX_4},
    {SPELL_ECHOES_FX_5},
	{SPELL_ECHOES_FX_6},
	{SPELL_ECHOES_FX_7},
	{SPELL_ECHOES_FX_8},
	{SPELL_ECHOES_FX_9},
	{SPELL_ECHOES_FX_10},
	{SPELL_ECHOES_FX_11},
	{SPELL_ECHOES_FX_12},
	{SPELL_ECHOES_FX_13},
	{SPELL_ECHOES_FX_14},
	{SPELL_ECHOES_FX_15},
	{SPELL_ECHOES_FX_16},
	{SPELL_ECHOES_FX_17},
	{SPELL_ECHOES_FX_18},
	{SPELL_ECHOES_FX_19},
	{SPELL_ECHOES_FX_20}
};

class OutdoorPvPFX : public OutdoorPvP
{
    public:
        OutdoorPvPFX();

		void Update(uint32 diff) override;

    private:
};

#endif