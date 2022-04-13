#include "WeirdOrgan.h"
#include "IPlug_include_in_plug_src.h"

WeirdOrgan::WeirdOrgan(const InstanceInfo& info)
: Plugin(info, MakeConfig(kNumParams, 1)), mBandParams()
{
#if IPLUG_DSP
  mFaustProcessor.SetMaxChannelCount(MaxNChannels(ERoute::kInput), MaxNChannels(ERoute::kOutput));
  mFaustProcessor.Init();
  mFaustProcessor.CompileCPP();
  mFaustProcessor.SetAutoRecompile(true);
#ifndef FAUST_COMPILED
  mFaustProcessor.SetCompileFunc([&](){
    OnParamReset(EParamSource::kRecompile);
  });
#endif
#endif

  mFaustProcessor.CreateIPlugParameters(this, 0, mFaustProcessor.NParams());
  pushBandParams(0);
  pushBandParams(1);
  pushBandParams(2);
  pushBandParams(4);
  pushBandParams(6);
  pushBandParams(8);
  pushBandParams(10);
  pushBandParams(12);
  pushBandParams(14);
  pushBandParams(16);
  
#if IPLUG_EDITOR
  mMakeGraphicsFunc = [&]() {
    return MakeGraphics(*this, PLUG_WIDTH, PLUG_HEIGHT, PLUG_FPS, GetScaleForScreen(PLUG_WIDTH, PLUG_HEIGHT));
  };
  
  mLayoutFunc = [&](IGraphics* pGraphics) {
    IRECT b = pGraphics->GetBounds().GetPadded(-20);

    IRECT viz = b.GetReducedFromTop(400);
    IRECT keyb = viz.ReduceFromBottom(100);
    pGraphics->AttachCornerResizer(EUIResizerMode::Scale);
    pGraphics->LoadFont("Roboto-Regular", ROBOTO_FN);
    pGraphics->AttachPanelBackground(COLOR_GRAY);

    IRECT bandControls = b.GetFromTop(300);
    auto nBands = mBandParams.size();
    auto nColumns = nBands + 1;
    for (auto band = 0; band < nBands; band++) {
        auto &bandParamIds = mBandParams.at(band);
        IRECT bandStrip = b.GetGridCell(0, band, 1, nColumns);
        IRECT slider = bandStrip.ReduceFromTop(150);
        pGraphics->AttachControl(new IVSliderControl(slider, bandParamIds.gain, "Level"));
        IRECT envKnobs = bandStrip.ReduceFromTop(250);
        pGraphics->AttachControl(new IVKnobControl(envKnobs.GetGridCell(0, 0, 4, 1), bandParamIds.attack, "Attack"));
        pGraphics->AttachControl(new IVKnobControl(envKnobs.GetGridCell(1, 0, 4, 1), bandParamIds.decay, "Decay"));
        pGraphics->AttachControl(new IVKnobControl(envKnobs.GetGridCell(2, 0, 4, 1), bandParamIds.sustain, "Sustain"));
        pGraphics->AttachControl(new IVKnobControl(envKnobs.GetGridCell(3, 0, 4, 1), bandParamIds.release, "Release"));
    }

    IRECT outControls = b.GetGridCell(0, nBands, 1, nColumns);
    IRECT slider = outControls.ReduceFromTop(200);
    pGraphics->AttachControl(new IVSliderControl(slider, kParamVolume, "Volume"));
    IRECT qKnob = outControls.ReduceFromTop(100);
    pGraphics->AttachControl(new IVKnobControl(qKnob, kParamQ, "Q"));
    
    pGraphics->AttachControl(new IVScopeControl<2>(viz, "", DEFAULT_STYLE.WithColor(kBG, COLOR_BLACK).WithColor(kFG, COLOR_GREEN)), kCtrlTagScope);
    pGraphics->AttachControl(new IVKeyboardControl(keyb));
  };
#endif
}

void WeirdOrgan::pushBandParams(int bandId)
{
    std::string gainName = "partial gain " + std::to_string(bandId);
    std::string attackName = "attack " + std::to_string(bandId);
    std::string decayName = "decay " + std::to_string(bandId);
    std::string sustainName = "sustain " + std::to_string(bandId);
    std::string releaseName = "release " + std::to_string(bandId);

    int gainId = -1;
    int attackId = -1;
    int decayId = -1;
    int sustainId = -1;
    int releaseId = -1;
    
    for (int i = 0; i < NParams(); i++) {
        auto name = GetParam(i)->GetName();
        if (gainName.compare(name) == 0) {
            gainId = i;
            continue;
        }
        if (attackName.compare(name) == 0) {
            attackId = i;
            continue;
        }
        if (decayName.compare(name) == 0) {
            decayId = i;
            continue;
        }
        if (sustainName.compare(name) == 0) {
            sustainId = i;
            continue;
        }
        if (releaseName.compare(name) == 0) {
            releaseId = i;
            continue;
        }
    }

    mBandParams.emplace_back(gainId, attackId, decayId, sustainId, releaseId);
}

#if IPLUG_DSP

void WeirdOrgan::ProcessBlock(sample** inputs, sample** outputs, int nFrames)
{
  mFaustProcessor.ProcessBlock(inputs, outputs, nFrames);
  mScopeSender.ProcessBlock(outputs, nFrames, kCtrlTagScope);
}

void WeirdOrgan::OnReset()
{
  mFaustProcessor.SetSampleRate(GetSampleRate());
}

void WeirdOrgan::ProcessMidiMsg(const IMidiMsg& msg)
{
  mFaustProcessor.ProcessMidiMsg(msg);
}

void WeirdOrgan::OnParamChange(int paramIdx)
{
  mFaustProcessor.SetParameterValueNormalised(paramIdx, GetParam(paramIdx)->GetNormalized());
}

void WeirdOrgan::OnIdle()
{
  mScopeSender.TransmitData(*this);
}
#endif
