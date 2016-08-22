#include "EnvelopeGenerator.h"
#include <math.h>

double EnvelopeGenerator::mSampleRate = 44100.0;

EnvelopeGenerator::EnvelopeGenerator()
  : mMinimumLevel(0.0001)
  , mCurrentStage(ENVELOPE_STAGE_OFF)
  , mCurrentLevel(mMinimumLevel)
  , mMultiplier(1.0)
  , mCurrentSampleIndex(0)
  , mNextStageSampleIndex(0)
{
  mStageValue[ENVELOPE_STAGE_OFF] = 0.0; // level
  mStageValue[ENVELOPE_STAGE_DELAY] = 0.0; // time
  mStageValue[ENVELOPE_STAGE_ATTACK] = 0.01; // time
  mStageValue[ENVELOPE_STAGE_DECAY] = 0.5; // time
  mStageValue[ENVELOPE_STAGE_SUSTAIN] = 0.8; // level
  mStageValue[ENVELOPE_STAGE_RELEASE] = 1.0; // time
}


EnvelopeGenerator::~EnvelopeGenerator()
{
}

void EnvelopeGenerator::reset()
{
  mCurrentStage = ENVELOPE_STAGE_OFF;
  mCurrentLevel = mMinimumLevel;
  mMultiplier = 1.0;
  mCurrentSampleIndex = 0;
  mNextStageSampleIndex = 0;
}

void EnvelopeGenerator::enterStage(EnvelopeStage newStage)
{
  if (mCurrentStage == newStage) {
    return;
  }
  if (mCurrentStage == ENVELOPE_STAGE_OFF) {
    mBeganEnvelopeCycle.Emit();
  }
  if (newStage == ENVELOPE_STAGE_OFF) {
    mFinishedEnvelopeCycle.Emit();
  }

  mCurrentStage = newStage;
  mCurrentSampleIndex = 0;

  if (mCurrentStage == ENVELOPE_STAGE_OFF ||
    mCurrentStage == ENVELOPE_STAGE_SUSTAIN)
  {
    mNextStageSampleIndex = 0;
  } 
  else {
    mNextStageSampleIndex = mStageValue[mCurrentStage] * mSampleRate;
  }

  switch (newStage) {
  case ENVELOPE_STAGE_OFF:
    mCurrentLevel = 0.0;
    mMultiplier = 1.0;
    break;
  case ENVELOPE_STAGE_DELAY:
    break;
  case ENVELOPE_STAGE_ATTACK:
    mCurrentLevel = mMinimumLevel;
    calculateMultiplier(mCurrentLevel, 1.0, mNextStageSampleIndex);
    break;
  case ENVELOPE_STAGE_DECAY:
    mCurrentLevel = 1.0;
    calculateMultiplier(mCurrentLevel, fmax(mStageValue[ENVELOPE_STAGE_SUSTAIN], mMinimumLevel), mNextStageSampleIndex);
    break;
  case ENVELOPE_STAGE_SUSTAIN:
    mCurrentLevel = mStageValue[ENVELOPE_STAGE_SUSTAIN];
    mMultiplier = 1.0;
    break;
  case ENVELOPE_STAGE_RELEASE:
    calculateMultiplier(mCurrentLevel, mMinimumLevel, mNextStageSampleIndex);
    break;
  default:
    break;
  }
}

double EnvelopeGenerator::nextSample()
{
  if (mCurrentStage != ENVELOPE_STAGE_OFF &&
    mCurrentStage != ENVELOPE_STAGE_SUSTAIN)
  {
    if (mCurrentSampleIndex == mNextStageSampleIndex) {
      EnvelopeStage newStage = static_cast<EnvelopeStage>((mCurrentStage + 1) % kNumEnvelopeStages);
      enterStage(newStage);
    }

    mCurrentLevel *= mMultiplier;
    mCurrentSampleIndex++;
  }

  return mCurrentLevel;
}

void EnvelopeGenerator::setSampleRate(double newSampleRate)
{
  mSampleRate = newSampleRate;
}

void EnvelopeGenerator::setStageValue(EnvelopeStage stage, double value)
{
  mStageValue[stage] = value;

  if (stage == mCurrentStage) {
    // re-calculate the mMultiplier and mNextStageSampleIndex
    if (mCurrentStage == ENVELOPE_STAGE_DELAY ||
      mCurrentStage == ENVELOPE_STAGE_ATTACK ||
      mCurrentStage == ENVELOPE_STAGE_DECAY ||
      mCurrentStage == ENVELOPE_STAGE_RELEASE)
    {
      double nextLevelValue;
      switch (mCurrentStage) {
      case ENVELOPE_STAGE_DELAY:
        nextLevelValue = mMinimumLevel;
        break;
      case ENVELOPE_STAGE_ATTACK:
        nextLevelValue = 1.0;
        break;
      case ENVELOPE_STAGE_DECAY:
        nextLevelValue = fmax(mStageValue[ENVELOPE_STAGE_SUSTAIN], mMinimumLevel);
        break;
      case ENVELOPE_STAGE_RELEASE:
        nextLevelValue = mMinimumLevel;
        break;
      }

      // how far the generator is into the currnent stage
      double currentStageProcess = double(mCurrentSampleIndex) / double(mNextStageSampleIndex);
      double remainingStageProcess = 1.0 - currentStageProcess;

      unsigned long long sampleUntilNextStage = remainingStageProcess * value * mSampleRate;
      mNextStageSampleIndex = mCurrentSampleIndex + sampleUntilNextStage;

      calculateMultiplier(mCurrentLevel, nextLevelValue, sampleUntilNextStage);
    }
    else if (mCurrentStage == ENVELOPE_STAGE_SUSTAIN) {
      mCurrentLevel = value;
    }
  }
}

void EnvelopeGenerator::calculateMultiplier(double startLevel, double endLevel, unsigned long long lengthInSamples)
{
  mMultiplier = 1.0 + (log(endLevel) - log(startLevel)) / (lengthInSamples);
}
