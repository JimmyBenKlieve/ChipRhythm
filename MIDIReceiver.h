#pragma once
#include "IPlug_include_in_plug_hdr.h"

#include "IMidiQueue.h"

class MIDIReceiver
{
public:
  MIDIReceiver();
  ~MIDIReceiver();

  inline bool getKeyStatus(int keyIndex) const
  {
    return mKeyStatus[keyIndex];
  }
  inline int getNumKeys() const
  {
    return mNumKeys;
  }
  inline int getLastNoteNumber() const
  {
    return mLastNoteNumber;
  }
  inline int getLastVelocity() const
  {
    return mLastVelocity;
  }
  inline double getLastFrequency() const
  {
    return mLastFrequency;
  }

  void advance();
  void onMessageReceived(IMidiMsg *midiMessage);

  inline void flush(int nFrames)
  {
    mMidiQueue.Flush(nFrames);
    mOffset = 0;
  }
  inline void resize(int blockSize)
  {
    mMidiQueue.Resize(blockSize);
  }

private:
  inline double noteNumberToFrequency(int noteNumber)
  {
    return 440.0 * pow(2.0, (noteNumber - 69.0) / 12.0);
  }

private:
  IMidiQueue mMidiQueue;

  static const int keyCount = 128;
  int mNumKeys;
  bool mKeyStatus[keyCount];

  int mLastNoteNumber;
  int mLastVelocity;
  double mLastFrequency;

  int mOffset;
};

