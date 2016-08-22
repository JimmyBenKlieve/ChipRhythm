#include "ChipRhythm.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "IKeyboardControl.h"
#include "resource.h"

const int kNumPrograms = 1;

enum EParams
{
  // oscillator
  kOscWaveform = 0,
  kOscPitchMod,
  // oscillator volume envelope
  kEnvDelay,
  kEnvAttack,
  kEnvDecay,
  kEnvSustain,
  kEnvRelease,
  // filter
  kFilterMode,
  kFilterCutoff,
  kFilterResonance,
  kFilterLfoAmount,
  kFilterEnvAmount,
  // filter envelope
  kFIlterEnvAttack,
  kFilterEnvDecay,
  kFilterEnvSustain,
  kFilterEnvRelease,
  // lfo
  kLfoWaveform,
  kLfoFrequency,
  // lfo envelope
  kLfoEnvDelay,
  kLfoEnvAttack,
  kNumParams
};

enum ELayout
{
  kWidth = GUI_WIDTH,
  kHeight = GUI_HEIGHT,

  kKeyboardX = 1,
  kKeyboardY = 0,

  kWaveformX = 10,
  kWaveformY = 96,
};

ChipRhythm::ChipRhythm(IPlugInstanceInfo instanceInfo)
  :	IPLUG_CTOR(kNumParams, kNumPrograms, instanceInfo)
  , lastVirtualKeyboardNoteNumber(virtualKeyboardMinimumNoteNumber - 1)

{
  TRACE;

  CreateParams();
  CreateGraphics();
  CreatePresets();

  mMIDIReceiver.mNoteOn.Connect(&mVoiceManager, &VoiceManager::onNoteOn);
  mMIDIReceiver.mNoteOff.Connect(&mVoiceManager, &VoiceManager::onNoteOff);
}

ChipRhythm::~ChipRhythm() {}

void ChipRhythm::ProcessVirtualKeyboard()
{
  IKeyboardControl *virtualKeyboard = (IKeyboardControl *)mVirtualKeyboard;
  int virtualKeyboardNoteNumber = virtualKeyboard->GetKey() + virtualKeyboardMinimumNoteNumber;

  if (lastVirtualKeyboardNoteNumber >= virtualKeyboardMinimumNoteNumber
    && lastVirtualKeyboardNoteNumber != virtualKeyboardNoteNumber)
  {
    IMidiMsg midiMessage;
    midiMessage.MakeNoteOffMsg(lastVirtualKeyboardNoteNumber, 0);
    mMIDIReceiver.onMessageReceived(&midiMessage);
  }

  if (virtualKeyboardNoteNumber >= virtualKeyboardMinimumNoteNumber
    && virtualKeyboardNoteNumber != lastVirtualKeyboardNoteNumber)
  {
    IMidiMsg midiMessage;
    midiMessage.MakeNoteOnMsg(virtualKeyboardNoteNumber, virtualKeyboard->GetVelocity(), 0);
    mMIDIReceiver.onMessageReceived(&midiMessage);
  }

  lastVirtualKeyboardNoteNumber = virtualKeyboardNoteNumber;
}

void ChipRhythm::CreateParams()
{
}

void ChipRhythm::CreateGraphics()
{
  IGraphics* pGraphics = MakeGraphics(this, kWidth, kHeight);
  pGraphics->AttachBackground(BG_ID, BG_FN);

  IBitmap whiteKeyImage = pGraphics->LoadIBitmap(WHITE_KEY_ID, WHITE_KEY_FN, 6);
  IBitmap blackKeyImage = pGraphics->LoadIBitmap(BLACK_KEY_ID, BLACK_KEY_FN);

  //                            C#     D#          F#      G#      A#
  int keyCoordinates[12] = { 0, 7, 12, 20, 24, 36, 43, 48, 56, 60, 69, 72 };
  mVirtualKeyboard = new IKeyboardControl(this, kKeyboardX, kKeyboardY, virtualKeyboardMinimumNoteNumber, 5, &whiteKeyImage, &blackKeyImage, keyCoordinates);
  pGraphics->AttachControl(mVirtualKeyboard);

  GetParam(kOscWaveform)->InitEnum("Waveform", WaveformGenerator::OSCILLATOR_MODE_SINE, WaveformGenerator::kNumOscillatorModes);
  GetParam(kOscWaveform)->SetDisplayText(0, "Sine");
  IBitmap waveformImage = pGraphics->LoadIBitmap(OSCILLATOR_MODE_ID, OSCILLATOR_MODE_FN, 4);
  pGraphics->AttachControl(new ISwitchControl(this, kWaveformX, kWaveformY, kOscWaveform, &waveformImage));

  AttachGraphics(pGraphics);
}

void ChipRhythm::CreatePresets()
{

}

void ChipRhythm::ProcessDoubleReplacing(double **inputs, double **outputs, int nFrames)
{
  // Mutex is already locked for us.

  double *leftOutput = outputs[0];
  double *rightOutput = outputs[1];

  ProcessVirtualKeyboard();

  for (int i = 0; i < nFrames; ++i) {
    mMIDIReceiver.advance();
    leftOutput[i] = rightOutput[i] = mVoiceManager.nextSample();
  }

  mMIDIReceiver.flush(nFrames);
}

void ChipRhythm::ProcessMidiMsg(IMidiMsg * pMsg)
{
  mMIDIReceiver.onMessageReceived(pMsg);
  mVirtualKeyboard->SetDirty();
}

void ChipRhythm::Reset()
{
  TRACE;
  IMutexLock lock(this);

  mVoiceManager.setSampleRate(GetSampleRate());
}

void ChipRhythm::OnParamChange(int paramIdx)
{
  IMutexLock lock(this);

  switch (paramIdx)
  {
  case kOscWaveform:

  default:
    break;
  }
}