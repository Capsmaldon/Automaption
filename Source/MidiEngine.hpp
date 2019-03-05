//
//  MidiEngine.hpp
//  Automaption - App
//
//  Created by Cai Smaldon on 23/12/2018.
//

#ifndef MidiEngine_hpp
#define MidiEngine_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioEngine.hpp"

/**
 * A midi listener that plugs into the CSynth, is
 * controlled in the main controls to change device input
 */

class MidiEngine : public MidiInputCallback
{
public:
    MidiEngine(AudioEngine* audioEngine);
    
    void setInput(String deviceName);
    
    void handleIncomingMidiMessage (MidiInput *source, const MidiMessage &message) override;
private:
    String currentInputDevice;
    AudioDeviceManager adm;
    CSynth* synth;
};

#endif /* MidiEngine_hpp */
