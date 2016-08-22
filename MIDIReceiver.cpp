#include "MIDIReceiver.h"



MIDIReceiver::MIDIReceiver()
  : mNumKeys(0)
  , mOffset(0)
{
  for (int i = 0; i < keyCount; ++i) {
    mKeyStatus[i] = false;
  }
}


MIDIReceiver::~MIDIReceiver()
{
}

bool MIDIReceiver::getKeyStatus(int keyIndex) const
{
  return mKeyStatus[keyIndex];
}

int MIDIReceiver::getNumKeys() const
{
  return mNumKeys;
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
        mNoteOn(noteNumber, velocity);
      }
    }
    else {
      if (mKeyStatus[noteNumber] == true) {
        mKeyStatus[noteNumber] = false;
        mNumKeys -= 1;
        mNoteOff(noteNumber, velocity);
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