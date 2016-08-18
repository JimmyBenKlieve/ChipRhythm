#ifndef __CHIPRHYTHM__
#define __CHIPRHYTHM__

#include "IPlug_include_in_plug_hdr.h"

#include "Oscillator.h"
#include "WaveformGenerator.h"
#include "MIDIReceiver.h"
#include "EnvelopeGenerator.h"

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

  int lastVirtualKeyboardNoteNumber;
  
  static const int virtualKeyboardMinimumNoteNumber = 48;

private:
  void ProcessVirtualKeyboard();
  void CreatePresets();

  inline void onNoteOn(const int noteNumber, const int velocity) 
  {
    mEnvelopeGenerator.enterStage(EnvelopeGenerator::ENVELOPE_STAGE_DELAY);
  }

  inline void onNoteOff(const int noteNumber, const int velocity) 
  {
    mEnvelopeGenerator.enterStage(EnvelopeGenerator::ENVELOPE_STAGE_RELEASE);
  }

private:
  IControl *mVirtualKeyboard;

  MIDIReceiver mMIDIReceiver;
  WaveformGenerator mWaveformGenerator;
  EnvelopeGenerator mEnvelopeGenerator;
};

#endif
