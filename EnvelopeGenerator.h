#pragma once
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
  void SetSampleRate(double newSampleRate);
  
  inline EnvelopeStage getCurrentStage() const
  {
    return mCurrentStage;
  }

  const double mMinimumLevel;

private:
  void calculateMultiplier(double startLevel, double endLevel, unsigned long long lengthInSamples);

private:
  unsigned long long mCurrentSampleIndex;
  unsigned long long mNextStageSampleIndex;

  EnvelopeStage mCurrentStage;

  double mCurrentLevel;
  double mMultiplier;
  double mSampleRate;
  double mStageValue[kNumEnvelopeStages];
};

