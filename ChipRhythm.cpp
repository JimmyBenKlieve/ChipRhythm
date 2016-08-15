#include "ChipRhythm.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "IKeyboardControl.h"
#include "resource.h"

const int kNumPrograms = 1;

enum EParams
{
  kWaveform = 0,
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

  IGraphics* pGraphics = MakeGraphics(this, kWidth, kHeight);
  pGraphics->AttachBackground(BG_ID, BG_FN);

  IBitmap whiteKeyImage = pGraphics->LoadIBitmap(WHITE_KEY_ID, WHITE_KEY_FN, 6);
  IBitmap blackKeyImage = pGraphics->LoadIBitmap(BLACK_KEY_ID, BLACK_KEY_FN);

  //                            C#     D#          F#      G#      A#
  int keyCoordinates[12] = { 0, 7, 12, 20, 24, 36, 43, 48, 56, 60, 69, 72 };
  mVirtualKeyboard = new IKeyboardControl(this, kKeyboardX, kKeyboardY, virtualKeyboardMinimumNoteNumber, 5, &whiteKeyImage, &blackKeyImage, keyCoordinates);
  pGraphics->AttachControl(mVirtualKeyboard);

  GetParam(kWaveform)->InitEnum("Waveform", OSCILLATOR_MODE_SINE, kNumOscillatorModes);
  GetParam(kWaveform)->SetDisplayText(0, "Sine");
  IBitmap waveformImage = pGraphics->LoadIBitmap(OSCILLATOR_MODE_ID, OSCILLATOR_MODE_FN, 4);
  pGraphics->AttachControl(new ISwitchControl(this, kWaveformX, kWaveformY, kWaveform, &waveformImage));

  AttachGraphics(pGraphics);

  CreatePresets();
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

void ChipRhythm::ProcessDoubleReplacing(double **inputs, double **outputs, int nFrames)
{
  // Mutex is already locked for us.

  double *leftOutput = outputs[0];
  double *rightOutput = outputs[1];

  ProcessVirtualKeyboard();

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
  mVirtualKeyboard->SetDirty();
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
  case kWaveform:
    mOscillator.setMode(static_cast<OscillatorMode>(GetParam(kWaveform)->Int()));

  default:
    break;
  }
}

void ChipRhythm::CreatePresets()
{
}