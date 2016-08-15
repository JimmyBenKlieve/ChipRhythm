#pragma once
#include <math.h>

enum OscillatorMode {
  OSCILLATOR_MODE_SINE = 0,
  OSCILLATOR_MODE_SAW,
  OSCILLATOR_MODE_SQUARE,
  OSCILLATOR_MODE_TRIANGLE,
  kNumOscillatorModes
};

class Oscillator
{
public:
  Oscillator();
  ~Oscillator();

  inline void setLength(unsigned short length) 
  {
    mLength = length;
    mSegLength = twoPI / mLength;
  }
  
  inline void setMode(OscillatorMode mode) 
  {
    mOscillatorMode = mode;
  }
  
  inline void setFrequency(double frequency) 
  {
    mFrequency = frequency;
    updateIncrement();
  }

  inline void setSampleRate(double sampleRate) 
  {
    mSampleRate = sampleRate;
    updateIncrement();
  }

  inline void setMuted(bool muted)
  {
    mIsMuted = muted;
  }

  double nextSample();

private:
  inline void updateIncrement() 
  {
    mPhaseIncrement = mFrequency * 2 * mPI / mSampleRate;
  }

private:
  const double mPI;
  const double twoPI;
  
  OscillatorMode mOscillatorMode;

  bool mIsMuted;

  unsigned short mLength;
  double mSegLength;

  double mFrequency;
  double mPhase;
  double mSampleRate;
  double mPhaseIncrement;
};

