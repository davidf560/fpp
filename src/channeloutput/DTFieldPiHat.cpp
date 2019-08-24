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

#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#include "common.h"
#include "DTFieldPiHat.h"
#include "log.h"
#include "Sequence.h"

#define I2C_BUS "/dev/i2c-1"
#define I2C_ADDR 0x10
#define CMD_WRITE_CH1 0x21

extern "C"
{
	DTPiHat *createOutputDTFieldPiHat(unsigned int startChannel,
			unsigned int channelCount)
	{
		return new DTPiHat(startChannel, channelCount);
	}
}

DTPiHat::DTPiHat(unsigned int startChannel, unsigned int channelCount)
	: ChannelOutputBase(startChannel, channelCount)
{
	LogInfo(VB_CHANNELOUT, "DTPiHat::DTPiHat(%u, %u)\n", startChannel, channelCount);

	// Set any max channels limit if necessary
	m_maxChannels = DT_HAT_MAX_CHANNELS * DT_HAT_NUM_PORTS;
}

DTPiHat::~DTPiHat()
{
	LogDebug(VB_CHANNELOUT, "DTPiHat::~DTPiHat()\n");
}

int DTPiHat::Init(Json::Value config)
{
	LogDebug(VB_CHANNELOUT, "DTPiHat::Init(JSON)\n");

	// Parse config
	for (int i = 0; i < config["outputs"].size(); i++)
	{
		Json::Value s = config["outputs"][i];
		PixelString *newString = new PixelString();

		if (!newString->Init(s))
		{
			return 0;
		}

		m_pixels += newString->m_outputChannels / 3;

		m_strings.push_back(newString);
	}

	LogDebug(VB_CHANNELOUT, "   Found %d strings of pixels\n", m_strings.size());

	// Open I2C bus
	if((m_i2c = open(I2C_BUS, O_RDWR)) < 0)
	{
		LogErr(VB_CHANNELOUT, "failed opening I2C bus: %s\n", strerror(errno));
		return 0;
	}

	if(ioctl(m_i2c, I2C_SLAVE, I2C_ADDR) < 0)
	{
		LogErr(VB_CHANNELOUT, "failed setting I2C bus address: %s\n", strerror(errno));
		return 0;
	}

	// Call the base class' Init() method, do not remove this line.
	return ChannelOutputBase::Init(config);
}

int DTPiHat::Close(void)
{
	LogDebug(VB_CHANNELOUT, "DTPiHat::Close()\n");

	if(m_i2c > 0)
	{
		close(m_i2c);
	}
	return ChannelOutputBase::Close();
}

int DTPiHat::SendData(unsigned char *channelData)
{
	LogExcess(VB_CHANNELOUT, "DTPiHat::SendData(%p)\n", channelData);

	PixelString *ps = NULL;

	for (int s = 0; s < m_strings.size(); s++)
	{
		ps = m_strings[s];
		if (ps->m_outputChannels == 0)
		{
			// Skip writintg out to ports with no output channels
			continue;
		}

		// Create the buffer that will be written via I2C to the hat. write()
		// must only be called once so that it is written as one transaction
		// on the I2C bus.

		// The first byte of the buffer written to the hat contains the
		// command to set the correct output channel number
		m_buf[0] = CMD_WRITE_CH1 + ps->m_portNumber;

		// Remaining bytes are RGB data
		for (int p = 0; p < ps->m_outputChannels; p++)
		{
			m_buf[p+1] = ps->m_brightnessMaps[p][channelData[ps->m_outputMap[p]]];
		}

		int buflen = ps->m_outputChannels + 1;
		if(write(m_i2c, m_buf, buflen) != buflen)
		{
			LogErr(VB_CHANNELOUT, "failed to write to I2C: %s\n", strerror(errno));
		}
	}

	return m_channelCount;
}

void DTPiHat::DumpConfig(void)
{
	LogDebug(VB_CHANNELOUT, "DTPiHat::DumpConfig()\n");

	for (int i = 0; i < m_strings.size(); i++)
	{
		LogDebug(VB_CHANNELOUT, "    String #%d\n", i);
		m_strings[i]->DumpConfig();
	}

	// Call the base class' DumpConfig() method, do not remove this line.
	ChannelOutputBase::DumpConfig();
}

void DTPiHat::GetRequiredChannelRange(int &min, int & max) {
	min = FPPD_MAX_CHANNELS;
	max = 0;

	PixelString *ps = NULL;
	for (int s = 0; s < m_strings.size(); s++) {
		ps = m_strings[s];
		int inCh = 0;
		for (int p = 0; p < ps->m_outputChannels; p++) {
			int ch = ps->m_outputMap[inCh++];
			if (ch < FPPD_MAX_CHANNELS) {
				min = std::min(min, ch);
				max = std::max(max, ch);
			}
		}
	}
}
