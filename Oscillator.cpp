#include "Oscillator.h"



Oscillator::Oscillator()
  : mOscillatorMode(OSCILLATOR_MODE_SINE)
  , mPI(2 * acos(0.0))
  , twoPI(2 * mPI)
  , mFrequency(440.0)
  , mPhase(0.0)
  , mSampleRate(44100.0)
  , mLength(32)
  , mSegLength(twoPI / mLength)
  , mIsMuted(true)
{
  updateIncrement();
}

Oscillator::~Oscillator()
{
}

double Oscillator::nextSample() {
  double value = 0.0;

  if (mIsMuted) {
    return value;
  }

  switch (mOscillatorMode) {
  case OSCILLATOR_MODE_SINE:
    value = sin(mPhase);
    break;
  case OSCILLATOR_MODE_TRIANGLE:
    if (mPhase < mPI) {
      value = 1.0 - fabs(1.0 - (2.0 * mPhase / mPI));
    }
    else {
      value = fabs(3.0 - (2.0 * mPhase / mPI)) - 1.0;
    }
    break;
  case OSCILLATOR_MODE_SAW:
    value = 1.0 - (2.0 * mPhase / twoPI);
    break;
  case OSCILLATOR_MODE_SQUARE:
    value = mPhase < mPI ? 1.0 : -1.0;
    break;
  }

  mPhase += mPhaseIncrement;
  while (mPhase > twoPI) {
    mPhase -= twoPI;
  }

  return value;
}