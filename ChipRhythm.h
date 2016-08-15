#ifndef __CHIPRHYTHM__
#define __CHIPRHYTHM__

#include "IPlug_include_in_plug_hdr.h"

#include "Oscillator.h"
#include "MIDIReceiver.h"

class ChipRhythm : public IPlug
{
public:
  ChipRhythm(IPlugInstanceInfo instanceInfo);
  ~ChipRhythm();

  void Reset();
  void OnParamChange(int paramIdx);
  void ProcessDoubleReplacing(double **inputs, double **outputs, int nFrames);
  void ProcessMidiMsg(IMidiMsg *pMsg);

private:
  void CreatePresets();

private:
  MIDIReceiver mMIDIReceiver;
  Oscillator mOscillator;
};

#endif
