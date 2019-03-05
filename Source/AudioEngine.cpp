//
//  AudioEngine.cpp
//  Automaption - App
//
//  Created by Cai Smaldon on 17/12/2018.
//

#include "AudioEngine.hpp"

AudioEngine::AudioEngine()
{
    // 2 Audio Outputs Opened
    setAudioChannels (0, 2);
}

AudioEngine::~AudioEngine()
{
    shutdownAudio();
}

void AudioEngine::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    synth.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void AudioEngine::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    //Wipe the buffer before populating it
    bufferToFill.buffer->clear();
    synth.getNextAudioBlock(bufferToFill);
}

void AudioEngine::releaseResources()
{
    synth.releaseResources();
}

