#include "Voice.h"



Voice::Voice()
  : mNoteNumber(-1)
  , mVelocity(0)
  , mFilterEnvelopeAmount(0.0)
  , mFilterLfoAmount(0.0)
  , mLfoValue(0.0)
  , mIsActive(false)
{
  mVolumeEnvelope.mFinishedEnvelopeCycle.Connect(this, &Voice::setFree);
}


Voice::~Voice()
{
}

double Voice::nextSample()
{
  if (!mIsActive) {
    return 0.0;
  }

  double waveformOutput = mWaveformGenerator.nextSample();

  double volumeEnvelopeValue = mVolumeEnvelope.nextSample();
  double filterEnvelopeValue = mFilterEnvelope.nextSample();

  mFilter.setCutoffMod(filterEnvelopeValue * mFilterEnvelopeAmount + mLfoValue * mFilterLfoAmount);

  return mFilter.process(waveformOutput * volumeEnvelopeValue * mVelocity / 127.0);
}

void Voice::setFree()
{
  mIsActive = false;
}

void Voice::reset()
{
  mNoteNumber = -1;
  mVelocity = 0;

  mWaveformGenerator.reset();
  mVolumeEnvelope.reset();
  mFilterEnvelope.reset();
  mFilter.reset();
}
