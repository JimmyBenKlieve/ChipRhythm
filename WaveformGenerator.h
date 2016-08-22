#pragma once
#include "Oscillator.h"
#include <array>

using namespace std;

class WaveformGenerator :
  public Oscillator
{
private:
  typedef array<double, 64> Waveform;

public:
  WaveformGenerator();
  ~WaveformGenerator();

  virtual inline void setResolution(unsigned int resolution)
  {
    mResolution = resolution;
    mSegLength = twoPI / double(mResolution);
  }
  virtual void setMode(OscillatorMode mode);
  virtual double nextSample();

private:
  Waveform mWaveform;
  
  unsigned int mResolution;
  double mSegLength;
};

