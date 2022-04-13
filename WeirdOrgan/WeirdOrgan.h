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

const int kParamQ = 34;
const int kParamVolume = 55;

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
