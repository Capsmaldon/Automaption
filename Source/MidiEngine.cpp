//
//  MidiEngine.cpp
//  Automaption - App
//
//  Created by Cai Smaldon on 23/12/2018.
//

#include "MidiEngine.hpp"

MidiEngine::MidiEngine(AudioEngine* audioEngine)
{
    //Keep a reference of the Synth
    synth = audioEngine->getSynth();
    
    /*
     Enable receiving of midi messages from all sources -
     Could not figure out why adding all devices to the callback
     and altering which ones were enabled wouldn't work,
     so the reverse is done, all are enabled and then are
     hot swapped from the callback
     */
    StringArray devices = MidiInput::getDevices();
    
    for(auto device : devices)
    {
        adm.setMidiInputEnabled(device, true);
    }
    
    //Add the first device detected to the callback
    adm.addMidiInputCallback(devices[0], this);
    currentInputDevice = devices[0];
}

void MidiEngine::setInput(String deviceName)
{
    /*
     After receiving a message from the main controls object,
     remove the previous object from the callback and add the new one
     */
    adm.removeMidiInputCallback(currentInputDevice, this);
    adm.addMidiInputCallback(deviceName, this);
    currentInputDevice = deviceName;
}

void MidiEngine::handleIncomingMidiMessage (MidiInput *source, const MidiMessage &message)
{
    //When this midi listener receives a message, pass it on to the synthesiser
    synth->handleMidi(message);
}
