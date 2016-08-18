#include "Filter.h"



Filter::Filter()
  : mFilterMode(FILTER_MODE_LOWPASS)
  , mFilterAttenuation(FILTER_ATTENUATION_12DB)
  , mCutoff(0.99)
  , mResonance(0.0)
  , mCutoffModulation(0.0)
{
  mBuffer[0] = 0.0;
  mBuffer[1] = 0.0;
  mBuffer[2] = 0.0;
  mBuffer[3] = 0.0;

  calculateFeedbackAmount();
}


Filter::~Filter()
{
}

double Filter::process(double inputValue)
{
  if (inputValue == 0.0) {
    return inputValue;
  }

  double calculatedCutoff = getCalculatedCutoff();

  mBuffer[0] += calculatedCutoff * (inputValue - mBuffer[0] + mFeedbackAmount * (mBuffer[0] - mBuffer[1]));
  mBuffer[1] += calculatedCutoff * (mBuffer[0] - mBuffer[1]);
  mBuffer[2] += calculatedCutoff * (mBuffer[1] - mBuffer[2]);
  mBuffer[3] += calculatedCutoff * (mBuffer[2] - mBuffer[3]);

  switch (mFilterMode) {
  case FILTER_MODE_LOWPASS:
    return (mFilterAttenuation == FILTER_ATTENUATION_12DB ? mBuffer[1] : mBuffer[3]);
    break;
  case FILTER_MODE_HIGHPASS:
    return inputValue - (mFilterAttenuation == FILTER_ATTENUATION_12DB ? mBuffer[0] : mBuffer[3]);
    break;
  case FILTER_MODE_BANDPASS:
    return mBuffer[0] - (mFilterAttenuation == FILTER_ATTENUATION_12DB ? mBuffer[1] : mBuffer[3]);
    break;
  default: 
    return 0.0;
  }
}
