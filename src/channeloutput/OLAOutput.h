/*
 *   OLA Channel Output driver for Falcon Player (FPP)
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

#ifndef _OLAOUTPUT_H
#define _OLAOUTPUT_H

#include <vector>

#include "ThreadedChannelOutputBase.h"
#include "UniverseEntry.h"

#include <ola/DmxBuffer.h>
#include <ola/client/StreamingClient.h>

class OLAOutput : public ThreadedChannelOutputBase {
  public:
	OLAOutput(unsigned int startChannel, unsigned int channelCount);
	~OLAOutput();

	int Init(Json::Value config);
	int Close(void);

	int RawSendData(unsigned char *channelData);

	void DumpConfig(void);
    
    virtual void GetRequiredChannelRange(int &min, int & max);

  private:
	ola::DmxBuffer                  m_buffer;
	ola::client::StreamingClient   *m_client;

	std::vector<Universe>           m_universes;
};

#endif
