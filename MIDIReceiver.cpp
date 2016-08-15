#include "MIDIReceiver.h"



MIDIReceiver::MIDIReceiver()
  : mNumKeys(0)
  , mLastNoteNumber(-1)
  , mLastVelocity(0)
  , mLastFrequency(-1.0)
  , mOffset(0)
{
  for (int i = 0; i < keyCount; ++i) {
    mKeyStatus[i] = false;
  }
}


MIDIReceiver::~MIDIReceiver()
{
}

void MIDIReceiver::advance()
{
  while (!mMidiQueue.Empty()) {

    IMidiMsg *midiMessage = mMidiQueue.Peek();

    if (midiMessage->mOffset > mOffset) {
      break;
    }

    IMidiMsg::EStatusMsg status = midiMessage->StatusMsg();

    int noteNumber = midiMessage->NoteNumber();
    int velocity = midiMessage->Velocity();

    if (status == IMidiMsg::kNoteOn && velocity) {
      if (mKeyStatus[noteNumber] == false) {
        mKeyStatus[noteNumber] = true;
        mNumKeys += 1;
      }

      if (noteNumber != mLastNoteNumber) {
        mLastNoteNumber = noteNumber;
        mLastFrequency = noteNumberToFrequency(mLastNoteNumber);
        mLastVelocity = velocity;
      }
    }
    else {
      if (mKeyStatus[noteNumber] == true) {
        mKeyStatus[noteNumber] = false;
        mNumKeys -= 1;
      }

      if (noteNumber == mLastNoteNumber) {
        mLastNoteNumber = -1;
        mLastFrequency = -1.0;
        mLastVelocity = 0;
      }
    }
    mMidiQueue.Remove();
  
  }
  ++mOffset;

}

void MIDIReceiver::onMessageReceived(IMidiMsg * midiMessage)
{
  IMidiMsg::EStatusMsg status = midiMessage->StatusMsg();

  if (status == IMidiMsg::kNoteOn ||
      status == IMidiMsg::kNoteOff
    )
  {
    mMidiQueue.Add(midiMessage);
  }
}