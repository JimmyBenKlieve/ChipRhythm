#ifndef __CHIPRHYTHM__
#define __CHIPRHYTHM__

#include "IPlug_include_in_plug_hdr.h"

#include "Oscillator.h"
#include "WaveformGenerator.h"
#include "MIDIReceiver.h"
#include "EnvelopeGenerator.h"
#include "Filter.h"

class ChipRhythm : public IPlug
{
public:
  ChipRhythm(IPlugInstanceInfo instanceInfo);
  ~ChipRhythm();

  void Reset();
  void OnParamChange(int paramIdx);
  void ProcessDoubleReplacing(double **inputs, double **outputs, int nFrames);
  void ProcessMidiMsg(IMidiMsg *pMsg);

  inline int GetNumKeys() const
  {
    return mMIDIReceiver.getNumKeys();
  }

  inline bool GetKeyStatus(int key) const
  {
    return mMIDIReceiver.getKeyStatus(key);
  }

private:
  void ProcessVirtualKeyboard();
  void CreateParams();
  void CreateGraphics();
  void CreatePresets();

  inline void onNoteOn(const int noteNumber, const int velocity)
  {
    mEnvelopeGenerator.enterStage(EnvelopeGenerator::ENVELOPE_STAGE_DELAY);
    mFilterEnvelopeGenerator.enterStage(EnvelopeGenerator::ENVELOPE_STAGE_ATTACK);
  }

  inline void onNoteOff(const int noteNumber, const int velocity) 
  {
    mEnvelopeGenerator.enterStage(EnvelopeGenerator::ENVELOPE_STAGE_RELEASE);
    mFilterEnvelopeGenerator.enterStage(EnvelopeGenerator::ENVELOPE_STAGE_RELEASE);
  }

  inline void onBeganEnvelopeCycle()
  {
    mWaveformGenerator.setMuted(false);
  }

  inline void onFinishedEnvelopeCycle()
  {
    mWaveformGenerator.setMuted(true);
  }

public:
  int lastVirtualKeyboardNoteNumber;
  static const int virtualKeyboardMinimumNoteNumber = 36;

private:
  IControl *mVirtualKeyboard;

  MIDIReceiver mMIDIReceiver;

  WaveformGenerator mWaveformGenerator;
  EnvelopeGenerator mEnvelopeGenerator;

  Filter mFilter;
  EnvelopeGenerator mFilterEnvelopeGenerator;
  double mFilterEnvelopeAmount;

  Oscillator mLfo;
  double mLfoFilterModAmount;
};

#endif
