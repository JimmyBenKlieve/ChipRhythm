#include "EnvelopeGenerator.h"
#include <math.h>


EnvelopeGenerator::EnvelopeGenerator()
  : mMinimumLevel(0.0001)
  , mCurrentStage(ENVELOPE_STAGE_OFF)
  , mCurrentLevel(mMinimumLevel)
  , mMultiplier(1.0)
  , mSampleRate(44100.0)
  , mCurrentSampleIndex(0)
  , mNextStageSampleIndex(0)
{
  mStageValue[ENVELOPE_STAGE_OFF] = 0.0; // level
  mStageValue[ENVELOPE_STAGE_DELAY] = 1.0; // time
  mStageValue[ENVELOPE_STAGE_ATTACK] = 0.01; // time
  mStageValue[ENVELOPE_STAGE_DECAY] = 0.5; // time
  mStageValue[ENVELOPE_STAGE_SUSTAIN] = 0.1; // level
  mStageValue[ENVELOPE_STAGE_RELEASE] = 1.0; // time
}


EnvelopeGenerator::~EnvelopeGenerator()
{
}

void EnvelopeGenerator::enterStage(EnvelopeStage newStage)
{
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

void EnvelopeGenerator::SetSampleRate(double newSampleRate)
{
  mSampleRate = newSampleRate;
}

void EnvelopeGenerator::calculateMultiplier(double startLevel, double endLevel, unsigned long long lengthInSamples)
{
  mMultiplier = 1.0 + (log(endLevel) - log(startLevel)) / (lengthInSamples);
}
