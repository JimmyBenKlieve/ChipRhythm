#include "WaveformGenerator.h"


WaveformGenerator::WaveformGenerator()
  : mResolution(32)
  , mSegLength(twoPI / double(mResolution))
  , mWaveform()
{
  mWaveform.fill(0.0);
  setMode(OSCILLATOR_MODE_SINE);
}


WaveformGenerator::~WaveformGenerator()
{
}

void WaveformGenerator::setMode(OscillatorMode mode)
{
  mOscillatorMode = mode;

  switch (mOscillatorMode) {
  case OSCILLATOR_MODE_SINE:
    for (unsigned int i = 0; i < mResolution; ++i) {
      mWaveform[i] = sin(double(i) * twoPI / double(mResolution));
    }
    break;
  case OSCILLATOR_MODE_TRIANGLE:
    for (unsigned int i = 0; i < mResolution; ++i) {
      if (i < mResolution / 2) {
        mWaveform[i] = 1.0 - fabs(-1.0 + double(i) / double(mResolution / 4.0));
      }
      else {
        mWaveform[i] = fabs(1.0 - double(i - mResolution / 2.0) / double(mResolution / 4.0)) - 1.0;
      }
    }
    break;
  case OSCILLATOR_MODE_SAW:
    for (unsigned int i = 0; i < mResolution; ++i) {
      mWaveform[i] = 1.0 - double(i) / double(mResolution);
    }
    break;
  case OSCILLATOR_MODE_SQUARE:
    for (unsigned int i = 0; i < mResolution; ++i) {
      mWaveform[i] = i < (mResolution / 2) ? 1.0 : -1.0;
    }
    break;
  }
}

double WaveformGenerator::nextSample()
{
  double value = 0.0;

  unsigned int index = unsigned int(mPhase / mSegLength);

  mPhase += mPhaseIncrement;
  while (mPhase > twoPI) {
    mPhase -= twoPI;
  }

  return mWaveform.at(index);
}


