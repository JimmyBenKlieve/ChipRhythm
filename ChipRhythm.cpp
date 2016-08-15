#include "ChipRhythm.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"

const int kNumPrograms = 1;

enum EParams
{
  kNumParams
};

enum ELayout
{
  kWidth = GUI_WIDTH,
  kHeight = GUI_HEIGHT
};

ChipRhythm::ChipRhythm(IPlugInstanceInfo instanceInfo)
  :	IPLUG_CTOR(kNumParams, kNumPrograms, instanceInfo)
{
  TRACE;

  IGraphics* pGraphics = MakeGraphics(this, kWidth, kHeight);
  pGraphics->AttachPanelBackground(&COLOR_RED);

  AttachGraphics(pGraphics);

  CreatePresets();
}

ChipRhythm::~ChipRhythm() {}

void ChipRhythm::ProcessDoubleReplacing(double **inputs, double **outputs, int nFrames)
{
  // Mutex is already locked for us.

  double *leftOutput = outputs[0];
  double *rightOutput = outputs[1];

  for (int i = 0; i < nFrames; ++i) {
    mMIDIReceiver.advance();

    int velocity = mMIDIReceiver.getLastVelocity();
    if (velocity > 0) {
      mOscillator.setFrequency(mMIDIReceiver.getLastFrequency());
      mOscillator.setMuted(false);
    }
    else {
      mOscillator.setMuted(true);
    }

    leftOutput[i] = rightOutput[i] = mOscillator.nextSample() * velocity / 127.0;
  }

  mMIDIReceiver.flush(nFrames);
}

void ChipRhythm::ProcessMidiMsg(IMidiMsg * pMsg)
{
  mMIDIReceiver.onMessageReceived(pMsg);
}

void ChipRhythm::Reset()
{
  TRACE;
  IMutexLock lock(this);

  mOscillator.setSampleRate(GetSampleRate());
}

void ChipRhythm::OnParamChange(int paramIdx)
{
  IMutexLock lock(this);

  switch (paramIdx)
  {
  default:
    break;
  }
}

void ChipRhythm::CreatePresets() 
{
  MakeDefaultPreset((char *) "-", kNumParams);
}