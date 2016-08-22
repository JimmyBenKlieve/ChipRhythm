#pragma once
#include "Voice.h"

class VoiceManager
{
public:
  static const int NumberOfVoices = 64;

public:
  VoiceManager();
  ~VoiceManager();

  void onNoteOn(int noteNumber, int velocity);
  void onNoteOff(int noteNumber, int velocity);

  void setSampleRate(double sampleRate);

  double nextSample();
private:
  Voice* findFreeVoice();

private:
  Voice mVoices[NumberOfVoices];

  Oscillator mLfo;
};

