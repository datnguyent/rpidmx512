/**
 * @file ltcparamssave.h
 */
/* Copyright (C) 2019 by Arjan van Vught mailto:info@raspberrypi-dmx.nl
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdint.h>
#include <assert.h>

#include "ltcparams.h"

#include "propertiesbuilder.h"

#include "ltcparamsconst.h"

bool LtcParams::Save(uint8_t* pBuffer, uint32_t nLength, uint32_t& nSize) {
	assert(pBuffer != 0);

	if (m_pLTcParamsStore == 0) {
		nSize = 0;
		return false;
	}

	m_pLTcParamsStore->Copy(&m_tLtcParams);

	PropertiesBuilder builder(LtcParamsConst::FILE_NAME, pBuffer, nLength);

	bool isAdded = builder.Add(LtcParamsConst::SOURCE, GetSourceType((TLtcReaderSource) m_tLtcParams.tSource));

	isAdded &= builder.Add(LtcParamsConst::MAX7219_TYPE, m_tLtcParams.tMax7219Type == LTC_PARAMS_MAX7219_TYPE_7SEGMENT ? "7segment" : "matrix" , isMaskSet(LTC_PARAMS_MASK_MAX7219_TYPE));
	isAdded &= builder.Add(LtcParamsConst::MAX7219_INTENSITY, (uint32_t) m_tLtcParams.nMax7219Intensity, isMaskSet(LTC_PARAMS_MASK_MAX7219_INTENSITY));

	isAdded &= builder.Add(LtcParamsConst::DISABLE_DISPLAY, (uint32_t) isDisabledOutputMaskSet(LTC_PARAMS_DISABLE_DISPLAY), isDisabledOutputMaskSet(LTC_PARAMS_DISABLE_DISPLAY));
	isAdded &= builder.Add(LtcParamsConst::DISABLE_MAX7219, (uint32_t) isDisabledOutputMaskSet(LTC_PARAMS_DISABLE_MAX7219), isDisabledOutputMaskSet(LTC_PARAMS_DISABLE_MAX7219));
	isAdded &= builder.Add(LtcParamsConst::DISABLE_MIDI, (uint32_t) isDisabledOutputMaskSet(LTC_PARAMS_DISABLE_MIDI), isDisabledOutputMaskSet(LTC_PARAMS_DISABLE_MIDI));
	isAdded &= builder.Add(LtcParamsConst::DISABLE_ARTNET, (uint32_t) isDisabledOutputMaskSet(LTC_PARAMS_DISABLE_ARTNET), isDisabledOutputMaskSet(LTC_PARAMS_DISABLE_ARTNET));
	isAdded &= builder.Add(LtcParamsConst::DISABLE_TCNET, (uint32_t) isDisabledOutputMaskSet(LTC_PARAMS_DISABLE_TCNET), isDisabledOutputMaskSet(LTC_PARAMS_DISABLE_TCNET));
	isAdded &= builder.Add(LtcParamsConst::DISABLE_LTC, (uint32_t) isDisabledOutputMaskSet(LTC_PARAMS_DISABLE_LTC), isDisabledOutputMaskSet(LTC_PARAMS_DISABLE_LTC));

	isAdded &= builder.Add(LtcParamsConst::SHOW_SYSTIME, (uint32_t) m_tLtcParams.nShowSysTime, isMaskSet(LTC_PARAMS_MASK_SHOW_SYSTIME));
	isAdded &= builder.Add(LtcParamsConst::DISABLE_TIMESYNC, (uint32_t) m_tLtcParams.nDisableTimeSync, isMaskSet(LTC_PARAMS_MASK_DISABLE_TIMESYNC));

	nSize = builder.GetSize();

	return isAdded;
}