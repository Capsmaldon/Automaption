//
//  CSynth.hpp
//  Automaption - App
//
//  Created by Cai Smaldon on 23/12/2018.
//

#ifndef CSynth_hpp
#define CSynth_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "EffectsModules.hpp"
#include "WavetablePlayer.hpp"

/**
 * A container for each wavetable and some meta data
 */
struct WavetableEntry
{
    int index;
    String name;
    AudioSampleBuffer wavetable;
};

struct VoiceSound : public SynthesiserSound
{
    VoiceSound() {}
    
    bool appliesToNote    (int) override        { return true; }
    bool appliesToChannel (int) override        { return true; }
};

/**
 * a Juce class required for the use of their synthesiser class
 * describes the timbre for the synthesiser
 */

class CVoice : public SynthesiserVoice
{
public:
    CVoice(WavetablePlayer* wavetable);
    
    bool canPlaySound (SynthesiserSound* sound) override;
    void startNote (int midiNoteNumber,
                    float velocity,
                    SynthesiserSound*,
                    int currentPitchWheelPosition) override;
    void stopNote (float velocity, bool allowTailOff) override;
    void pitchWheelMoved (int) override;
    void controllerMoved (int, int) override;
    void renderNextBlock (AudioSampleBuffer& outputBuffer,
                          int startSample,
                          int numSamples) override;
    
private:
    WavetablePlayer* wavetable;
    
    double currentAngle = 0.0;
    double angleDelta = 0.0;
    double level = 0.0;
    double tailOff = 0.0;
};

/**
 * A wrapper for the juce synthesiser - handles midi by
 * passing the data onto the juce synthesiser for playback
 */

class CSynth : public AudioSource
{
public:
    CSynth();
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void handleMidi(const MidiMessage &message);
    void setLevel(float value);
    void setWavetable(WavetableEntry *wavetable);
    DistortionModule* getDistortionModule() {return &distortionModule;}
    FilterModule* getFilterModule() {return &filterModule;}
    DelayModule* getDelayModule() {return &delayModule;}
private:
    Synthesiser synth;
    MidiMessageCollector collector;
    
    WavetablePlayer wavetablePlayer;
    
    float level = 1.0f;
    DistortionModule distortionModule;
    FilterModule filterModule;
    DelayModule delayModule;
};

#endif /* CSynth_hpp */
