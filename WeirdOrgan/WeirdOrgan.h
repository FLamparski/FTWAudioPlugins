#pragma once

#include "IPlug_include_in_plug_hdr.h"

#if IPLUG_DSP
#include "IPlugFaustGen.h"
#endif

#include "IControls.h"

#ifndef DSP_FILE
#define DSP_FILE ""
#endif

enum EControlTags
{
  kCtrlTagScope = 0,
  kNumCtrlTags
};

const int kNumParams = 56;

const int kParamPartial0Gain = 6;
const int kParamPartial0Attack = 1;
const int kParamPartial0Decay = 2;
const int kParamPartial0Sustain = 8;
const int kParamPartial0Release = 7;

// Parameter ids can be found after the log line
// END IPLUG CHANNEL IO PARSER --------------------------------------------------
const int kBandParamIds[10][5] = {
	{ 5, 1, 2, 8, 7 },      // partial 0
	{ 11, 9, 10, 13, 12 },  // partial 1
	{ 16, 14, 15, 18, 17 }, // partial 2
	{ 21, 19, 20, 23, 22 }, // partial 4
	{ 26, 24, 25, 28, 27 }, // partial 6
	{ 31, 29, 30, 33, 32 }, // partial 8
	{ 36, 34, 35, 38, 37 }, // partial 10
	{ 41, 39, 40, 43, 42 }, // partial 12

};
const int kBandParamIds_Gain = 0;
const int kBandParamIds_Attack = 1;
const int kBandParamIds_Decay = 2;
const int kBandParamIds_Sustain = 3;
const int kBandParamIds_Release = 4;

class BandParams final
{
public:
	BandParams(int paramIdGain, int paramIdAttack, int paramIdDecay, int paramIdSustain, int paramIdRelease)
		: gain(paramIdGain), attack(paramIdAttack), decay(paramIdDecay), sustain(paramIdSustain), release(paramIdRelease) {}
	int gain;
	int attack;
	int decay;
	int sustain;
	int release;
};

using namespace iplug;
using namespace igraphics;

class WeirdOrgan final : public Plugin
{
public:
  WeirdOrgan(const InstanceInfo& info);

private:
	std::vector<BandParams> mBandParams;
	void pushBandParams(int bandId);

#if IPLUG_DSP
public:
  void ProcessBlock(sample** inputs, sample** outputs, int nFrames) override;
  void OnReset() override;
  void ProcessMidiMsg(const IMidiMsg& msg) override;
  void OnParamChange(int paramIdx) override;
  void OnIdle() override;
private:
  FAUST_BLOCK(Faust1, mFaustProcessor, DSP_FILE, 1, 1);
  IBufferSender<2> mScopeSender;
#endif
};
