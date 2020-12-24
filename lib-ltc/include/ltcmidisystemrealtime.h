/**
 * @file ltcmidisystemrealtime.h
 *
 */
/* Copyright (C) 2020 by Arjan van Vught mailto:info@orangepi-dmx.nl
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

#ifndef LTCMIDISYSTEMREALTIME_H_
#define LTCMIDISYSTEMREALTIME_H_

class LtcMidiSystemRealtime {
public:
	LtcMidiSystemRealtime(struct TLtcDisabledOutputs *ptLtcDisabledOutputs);
	~LtcMidiSystemRealtime() {}

	void Start();
	void Stop();
	void Run();

	void SendStart();
	void SendStop();
	void SendContinue();

	void SetBPM(uint32_t nBPM);

	static LtcMidiSystemRealtime *Get() {
		return s_pThis;
	}

private:
	void Send(uint8_t nByte);
	void ShowBPM(uint32_t nBPM);

private:
	int m_nHandle{-1};
	uint8_t m_Buffer[64];
	uint32_t m_nBPMPrevious{999};

	static LtcMidiSystemRealtime *s_pThis;
};

#endif /* LTCMIDISYSTEMREALTIME_H_ */