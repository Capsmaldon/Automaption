//
//  Wavetable.cpp
//  Automaption - App
//
//  Created by Cai Smaldon on 24/01/2019.
//

#include "WavetablePlayer.hpp"

WavetablePlayer::WavetablePlayer()
{
    //If the audio thread starts and tries to play a note, this would normally be empty and crash so pushing 0 to this is an easy and efficient fix
    wavetable.push_back(0);
}

float WavetablePlayer::getSample(float angle)
{
    /*It is inefficient to have a function call every sample but it's the only way I could think of with the amount of time I had left
      to link up all of the voices and make it easy to swap out the wavetable*/
    
    float currentIndex = (angle/TWO_PI) * wavetable.size();
    
    int i0, i1;
    
    //Works both ways (Positive / Negative frequencies)
    i0 = currentIndex >= wavetable.size() ? 0:currentIndex;
    i1 = currentIndex+1 >= wavetable.size() ? 0:currentIndex+1;
    
    float mix = currentIndex - i0;
    
    float value0 = wavetable[i0];
    float value1 = wavetable[i1];
    
    float currentSample = (value0 * mix) + (value1 * (1.0f-mix));
    
    return currentSample;
}

void WavetablePlayer::changeSound(AudioSampleBuffer &newWavetable)
{
    //Swap out wavetable
    int length = newWavetable.getNumSamples();
    wavetable.resize(length);
    
    float* sample = newWavetable.getWritePointer(0); // Mono or left channel
    
    for(int i = 0; i < length; i++)
    {
        wavetable[i] = *sample;
        sample++;
    }
}
