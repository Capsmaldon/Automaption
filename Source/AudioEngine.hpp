//
//  AudioEngine.hpp
//  Automaption - App
//
//  Created by Cai Smaldon on 17/12/2018.
//

#ifndef AudioEngine_hpp
#define AudioEngine_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "CSynth.hpp"
#include "ParameterHandler.hpp"

/**
 * Handler for aquiring the audio output buffer, all sounds are owned by this object.
 * Equivalent to a speaker - the synthesiser itself is owned by this
 */
class AudioEngine : public AudioAppComponent
{
public:
    AudioEngine();
    ~AudioEngine();
    
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    
    CSynth* getSynth(){return &synth;}
private:
    CSynth synth;
};


#endif /* AudioEngine_hpp */
