#pragma once
#include <math.h>

class Filter
{
public:
  enum FilterMode {
    FILTER_MODE_LOWPASS = 0,
    FILTER_MODE_HIGHPASS,
    FILTER_MODE_BANDPASS,
    kNumFilterModes
  };

  enum FilterAttenuation {
    FILTER_ATTENUATION_12DB = 0,
    FILTER_ATTENUATION_24DB,
    kNumFilterAttenuations
  };

public:
  Filter();
  ~Filter();

  double process(double inputValue);

  inline void setFilterMode(FilterMode mode)
  {
    mFilterMode = mode;
  }

  inline void setAttenuation(FilterAttenuation attenuation)
  {
    mFilterAttenuation = attenuation;
  }

  inline void setCutoffMod(double cutoffModulation)
  {
    mCutoffModulation = cutoffModulation;
    calculateFeedbackAmount();
  }

  inline void setCutoff(double cutoff)
  {
    mCutoff = cutoff;
    calculateFeedbackAmount();
  }

  inline void setResonance(double resonance)
  {
    mResonance = resonance;
    calculateFeedbackAmount();
  }

private:
  inline void calculateFeedbackAmount() 
  {
    mFeedbackAmount = mResonance + mResonance / (1.0 - getCalculatedCutoff());
  }

  inline double getCalculatedCutoff() const 
  {
    return fmax(fmin(mCutoff + mCutoffModulation, 0.99), 0.01);
  }

private:
  FilterMode mFilterMode;
  FilterAttenuation mFilterAttenuation;

  double mCutoffModulation;

  double mCutoff;
  double mResonance;
  double mFeedbackAmount;

  double mBuffer[4];
};

