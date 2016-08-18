#pragma once
#include <math.h>

class Oscillator
{
public:
  enum OscillatorMode {
    OSCILLATOR_MODE_SINE = 0,
    OSCILLATOR_MODE_SAW,
    OSCILLATOR_MODE_SQUARE,
    OSCILLATOR_MODE_TRIANGLE,
    kNumOscillatorModes
  };

public:
  Oscillator();
  ~Oscillator();
  
  virtual inline void setMode(OscillatorMode mode) 
  {
    mOscillatorMode = mode;
  }
  
  virtual inline void setFrequency(double frequency) 
  {
    mFrequency = frequency;
    updateIncrement();
  }

  virtual inline void setSampleRate(double sampleRate) 
  {
    mSampleRate = sampleRate;
    updateIncrement();
  }

  virtual inline void setMuted(bool muted)
  {
    mIsMuted = muted;
  }

  virtual double nextSample();

protected:
  virtual inline void updateIncrement() 
  {
    mPhaseIncrement = mFrequency * 2 * mPI / mSampleRate;
  }

protected:
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

