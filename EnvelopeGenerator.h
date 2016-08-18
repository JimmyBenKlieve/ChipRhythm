#pragma once
#include "GallantSignal.h"

using Gallant::Signal0;

class EnvelopeGenerator
{
public:
  enum EnvelopeStage {
    ENVELOPE_STAGE_OFF = 0,
    ENVELOPE_STAGE_DELAY,
    ENVELOPE_STAGE_ATTACK,
    ENVELOPE_STAGE_DECAY,
    ENVELOPE_STAGE_SUSTAIN,
    ENVELOPE_STAGE_RELEASE,
    kNumEnvelopeStages
  };

public:
  EnvelopeGenerator();
  ~EnvelopeGenerator();

  void enterStage(EnvelopeStage newStage);
  double nextSample();
  void setSampleRate(double newSampleRate);
  void setStageValue(EnvelopeStage stage, double value);

  inline EnvelopeStage getCurrentStage() const
  {
    return mCurrentStage;
  }

private:
  void calculateMultiplier(double startLevel, double endLevel, unsigned long long lengthInSamples);

public:
  Signal0<> mBeganEnvelopeCycle;
  Signal0<> mFinishedEnvelopeCycle;

  const double mMinimumLevel;

private:
  unsigned long long mCurrentSampleIndex;
  unsigned long long mNextStageSampleIndex;

  EnvelopeStage mCurrentStage;

  double mCurrentLevel;
  double mMultiplier;
  double mSampleRate;
  double mStageValue[kNumEnvelopeStages];
};

