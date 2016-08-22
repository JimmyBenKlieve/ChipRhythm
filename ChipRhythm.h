#ifndef __CHIPRHYTHM__
#define __CHIPRHYTHM__

#include "IPlug_include_in_plug_hdr.h"

#include "MIDIReceiver.h"
#include "VoiceManager.h"

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

public:
  int lastVirtualKeyboardNoteNumber;
  static const int virtualKeyboardMinimumNoteNumber = 36;

private:
  IControl *mVirtualKeyboard;

  MIDIReceiver mMIDIReceiver;
  VoiceManager mVoiceManager;
};

#endif
