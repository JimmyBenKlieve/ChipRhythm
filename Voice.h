#pragma once

#include "WaveformGenerator.h"
#include "EnvelopeGenerator.h"
#include "Filter.h"

class Voice
{
  friend class VoiceManager;

public:
  Voice();
  ~Voice();

  inline void setFilterEnvelopeAmount(double amount)
  {
    mFilterEnvelopeAmount = amount;
  }

  inline void setFilterLfoAmount(double amount)
  {
    mFilterLfoAmount = amount;
  }

  inline void setLfoValue(double value)
  {
    mLfoValue = value;
  }

  inline void setNoteNumber(int noteNumber)
  {
    mNoteNumber = noteNumber;

    double frequency = 440.0 * pow(2.0, (mNoteNumber - 69.0) / 12.0);
    mWaveformGenerator.setFrequency(frequency);
  }

  double nextSample();

  void setFree();
  void reset();

private:
  WaveformGenerator mWaveformGenerator;
  EnvelopeGenerator mVolumeEnvelope;
  EnvelopeGenerator mFilterEnvelope;
  Filter mFilter;

  int mNoteNumber;
  int mVelocity;

  double mFilterEnvelopeAmount;
  double mFilterLfoAmount;

  double mLfoValue;

  bool mIsActive;
};

