//
//  CSynth.cpp
//  Automaption - App
//
//  Created by Cai Smaldon on 23/12/2018.
//

#include "CSynth.hpp"

CVoice::CVoice(WavetablePlayer* wavetable) : wavetable(wavetable)
{
    
}

bool CVoice::canPlaySound (SynthesiserSound* sound)
{
    //Taken from JUCE tutorial on using the synthesiser
    //https://docs.juce.com/master/tutorial_synth_using_midi_input.html
    return dynamic_cast<VoiceSound*> (sound) != nullptr;
}

void CVoice::startNote (int midiNoteNumber, float velocity,
                SynthesiserSound*, int /*currentPitchWheelPosition*/)
{
    //Taken from JUCE tutorial on using the synthesiser
    //https://docs.juce.com/master/tutorial_synth_using_midi_input.html
    currentAngle = 0.0;
    level = velocity * 0.15; // This is where key velocity is calculated
    tailOff = 0.0;
    
    auto cyclesPerSecond = MidiMessage::getMidiNoteInHertz (midiNoteNumber);
    auto cyclesPerSample = cyclesPerSecond / getSampleRate();
    
    angleDelta = cyclesPerSample * 2.0 * MathConstants<double>::pi;
}

void CVoice::stopNote (float /*velocity*/, bool allowTailOff)
{
    //Taken from JUCE tutorial on using the synthesiser
    //https://docs.juce.com/master/tutorial_synth_using_midi_input.html
    if (allowTailOff) // Set to true
    {
        if (tailOff == 0.0)
            tailOff = 1.0;
    }
    else
    {
        //Deletes the note when it has finished fading out
        clearCurrentNote();
        angleDelta = 0.0;
    }
}

void CVoice::pitchWheelMoved (int){}
void CVoice::controllerMoved (int, int){}

void CVoice::renderNextBlock (AudioSampleBuffer& outputBuffer, int startSample, int numSamples)
{
    if (angleDelta != 0.0) // If a frequency has been calculated
    {
        if (tailOff > 0.0) // Note released - Fade out before clearing the note
        {
            while (--numSamples >= 0)
            {
                //This is where the wavetable samples are calculated
                auto currentSample = (float) (wavetable->getSample(currentAngle) * level * tailOff);
                
                for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
                    outputBuffer.addSample (i, startSample, currentSample);
                
                currentAngle += angleDelta;
                float twoPi = 2.0 * MathConstants<double>::pi;
                currentAngle = currentAngle > (twoPi) ? currentAngle - twoPi : currentAngle;
                ++startSample;
                
                tailOff *= 0.99;
                
                if (tailOff <= 0.005)
                {
                    clearCurrentNote();
                    angleDelta = 0.0;
                    break;
                }
            }
        }
        else //Note is being held - Play full volume (Does technically snap in if cycle doesn't start at 0 crossing)
        {
            while (--numSamples >= 0)
            {
                //Also where wavetable samples are created
                auto currentSample = (float) (wavetable->getSample(currentAngle) * level);
                
                for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
                    outputBuffer.addSample (i, startSample, currentSample);
                
                currentAngle += angleDelta;
                float twoPi = 2.0 * MathConstants<double>::pi;
                currentAngle = currentAngle > (twoPi) ? currentAngle - twoPi : currentAngle;
                ++startSample;
            }
        }
    }
}

//==============================================================

CSynth::CSynth()
{
    //Allow for up to 8 notes to be played simultaneously
    for (auto i = 0; i < 8; ++i)
        synth.addVoice (new CVoice(&wavetablePlayer));
    
    /*
     Configures some parameters for the voices
     Couldn't work out how to make the VoiceSound class actually contain
     the wavetable itself and still be easy to swap wavetables around
     */
    synth.addSound (new VoiceSound());
}
void CSynth::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    //Reset MIDI and Audio FX modules
    collector.reset(sampleRate);
    distortionModule.prepareToPlay(samplesPerBlockExpected, sampleRate);
    filterModule.prepareToPlay(samplesPerBlockExpected, sampleRate);
    delayModule.prepareToPlay(samplesPerBlockExpected, sampleRate);
    synth.setCurrentPlaybackSampleRate(sampleRate);
}
void CSynth::releaseResources() {}
void CSynth::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    //Get notes from Midi input
    bufferToFill.clearActiveBufferRegion();
    MidiBuffer midiBuffer;
    collector.removeNextBlockOfMessages(midiBuffer, bufferToFill.numSamples);
    
    //Send Midi messages to the synthesiser
    synth.renderNextBlock (*bufferToFill.buffer, midiBuffer,
                           bufferToFill.startSample, bufferToFill.numSamples);
    
    //Process the samples in this order
    distortionModule.process(bufferToFill);
    filterModule.process(bufferToFill);
    delayModule.process(bufferToFill);
    
    //Alter the master output level
    auto* leftBuffer  = bufferToFill.buffer->getWritePointer (0, bufferToFill.startSample);
    auto* rightBuffer = bufferToFill.buffer->getWritePointer (1, bufferToFill.startSample);
    
    for(auto sample = 0; sample < bufferToFill.numSamples; ++sample)
    {
        leftBuffer[sample]  *= level;
        rightBuffer[sample] *= level;
    }
}
void CSynth::handleMidi(const MidiMessage &message)
{
    //When the CSynth receives messages from the Midi Engine, collect them for playback at correct timings
    collector.addMessageToQueue(message);
}

void CSynth::setLevel(float value)
{
    level = value;
}

void CSynth::setWavetable(WavetableEntry *wavetable)
{
    wavetablePlayer.changeSound(wavetable->wavetable);
}
