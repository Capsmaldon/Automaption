//
//  Wavetable.hpp
//  Automaption - App
//
//  Created by Cai Smaldon on 24/01/2019.
//

#ifndef Wavetable_hpp
#define Wavetable_hpp

#include "../JuceLibraryCode/JuceHeader.h"

const double TWO_PI = 2.0 * MathConstants<double>::pi;

/**
 * Holds the sample data for the wavetable,
 * a single instance of this is held by the CSynth
 * and all of the voices access it for sample data
 */

class WavetablePlayer
{
public:
    WavetablePlayer();
    
    float getSample(float angle);
    void changeSound(AudioSampleBuffer &newWavetable);
    
private:
    std::vector<float> wavetable;
};

#endif /* Wavetable_hpp */
