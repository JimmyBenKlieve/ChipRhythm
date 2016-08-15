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
  if (mIsMuted) {
    return 0.0;
  }

  double newPhase = int(mPhase / mSegLength) * mSegLength;
  double value = 0.0;

  switch (mOscillatorMode) {
  case OSCILLATOR_MODE_SINE:
    value = sin(newPhase);
    break;
  case OSCILLATOR_MODE_TRIANGLE:
    if (mPhase < mPI) {
      value = 1 - fabs(1 - (2.0 * newPhase / mPI));
    }
    else {
      value = fabs(3 - (2.0 * newPhase / mPI)) - 1;
    }
    break;
  case OSCILLATOR_MODE_SAW:
    value = 1.0 - (2.0 * newPhase / twoPI);
    break;
  case OSCILLATOR_MODE_SQUARE:
    value = newPhase < mPI ? 1.0 : -1.0;
    break;
  }

  mPhase += mPhaseIncrement;
  while (mPhase > twoPI) {
    mPhase -= twoPI;
  }

  return value;
}