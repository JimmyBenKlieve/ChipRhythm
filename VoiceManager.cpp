#include "VoiceManager.h"



VoiceManager::VoiceManager()
{
}


VoiceManager::~VoiceManager()
{
}

void VoiceManager::onNoteOn(int noteNumber, int velocity)
{
  Voice *voice = findFreeVoice();
  if (!voice) {
    return;
  }

  voice->reset();
  voice->setNoteNumber(noteNumber);
  voice->mVelocity = velocity;
  voice->mIsActive = true;
  voice->mVolumeEnvelope.enterStage(EnvelopeGenerator::ENVELOPE_STAGE_DELAY);
  voice->mFilterEnvelope.enterStage(EnvelopeGenerator::ENVELOPE_STAGE_ATTACK);
}

void VoiceManager::onNoteOff(int noteNumber, int velocity)
{
  for (int i = 0; i < NumberOfVoices; ++i) {
    Voice &voice = mVoices[i];
    if (voice.mIsActive && voice.mNoteNumber == noteNumber) {
      voice.mVolumeEnvelope.enterStage(EnvelopeGenerator::ENVELOPE_STAGE_RELEASE);
      voice.mFilterEnvelope.enterStage(EnvelopeGenerator::ENVELOPE_STAGE_RELEASE);
    }
  }
}

void VoiceManager::setSampleRate(double sampleRate)
{
  EnvelopeGenerator::setSampleRate(sampleRate);
  
  for (int i = 0; i < NumberOfVoices; ++i) {
    Voice &voice = mVoices[i];
    voice.mWaveformGenerator.setSampleRate(sampleRate);
  }

  mLfo.setSampleRate(sampleRate);
}

double VoiceManager::nextSample()
{
  double output = 0.0;
  double lfoValue = mLfo.nextSample();
  for (int i = 0; i < NumberOfVoices; ++i) {
    Voice &voice = mVoices[i];
    voice.setLfoValue(lfoValue);
    output += voice.nextSample();
  }
  return output;
}

Voice* VoiceManager::findFreeVoice()
{
  Voice *freeVoice = nullptr;
  for (int i = 0; i < NumberOfVoices; ++i) {
    if (!mVoices[i].mIsActive) {
      freeVoice = &(mVoices[i]);
      break;
    }
  }
  return freeVoice;
}
