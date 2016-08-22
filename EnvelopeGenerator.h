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

  inline EnvelopeStage getCurrentStage() const
  {
    return mCurrentStage;
  }

  static void setSampleRate(double newSampleRate);

  void reset();
  void enterStage(EnvelopeStage newStage);
  double nextSample();
  void setStageValue(EnvelopeStage stage, double value);

private:
  void calculateMultiplier(double startLevel, double endLevel, unsigned long long lengthInSamples);

public:
  Signal0<> mBeganEnvelopeCycle;
  Signal0<> mFinishedEnvelopeCycle;

  const double mMinimumLevel;

private:
  static double mSampleRate;

  unsigned long long mCurrentSampleIndex;
  unsigned long long mNextStageSampleIndex;

  EnvelopeStage mCurrentStage;

  double mCurrentLevel;
  double mMultiplier;
  double mStageValue[kNumEnvelopeStages];
};

