/*
 *   DWAB Technology Field LED Pi Hat handler for Falcon Player (FPP)
 *
 *   Copyright (C) 2013-2018 the Falcon Player Developers
 *      Initial development by:
 *      - David Pitts (dpitts)
 *      - Tony Mace (MyKroFt)
 *      - Mathew Mrosko (Materdaddy)
 *      - Chris Pinkham (CaptainMurdoch)
 *      For additional credits and developers, see credits.php.
 *
 *   The Falcon Player (FPP) is free software; you can redistribute it
 *   and/or modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation; either version 2 of
 *   the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _DWABHAT_H
#define _DWABHAT_H

#include "ChannelOutputBase.h"
#include "PixelString.h"

class DTPiHat : public ChannelOutputBase {
  public:
	DTPiHat(unsigned int startChannel, unsigned int channelCount);
	~DTPiHat();

    virtual int Init(Json::Value config) override;
    
	// Close the derived class.  This method must also call the
	// base class Close() method.
	int Close(void);

	// Main routine to send channel data out
	int SendData(unsigned char *channelData);

	// Dump the config variables for debugging.  This method must
	// also call the base class DumpConfig() method.
	void DumpConfig(void);
    
    virtual void GetRequiredChannelRange(int &min, int & max);

  private:
    int          m_i2c;

	int          m_pixels;

	std::vector<PixelString*> m_strings;

};

#endif
