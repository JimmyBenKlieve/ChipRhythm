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

      // a key pressed later overrides any previously pressed key
      if (noteNumber != mLastNoteNumber) {
        mLastNoteNumber = noteNumber;
        mLastFrequency = noteNumberToFrequency(mLastNoteNumber);
        mLastVelocity = velocity;

        // emit a "note on" signal
        noteOn(noteNumber, velocity);
      }
    }
    else {
      if (mKeyStatus[noteNumber] == true) {
        mKeyStatus[noteNumber] = false;
        mNumKeys -= 1;
      }

      // last note released, nothing should play
      if (noteNumber == mLastNoteNumber) {
        mLastNoteNumber = -1;

        // emit a "note off" signal
        noteOff(noteNumber, mLastVelocity);
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