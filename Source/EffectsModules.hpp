//
//  DistortionEffect.hpp
//  Automaption - App
//
//  Created by Cai Smaldon on 24/12/2018.
//

#ifndef DistortionEffect_hpp
#define DistortionEffect_hpp

#include "../JuceLibraryCode/JuceHeader.h"
/**
 * A simple filter for smoothing all the parameters gotten from the
 * data thread at ~20Hz (Metronome interval) and merging them with the audio thread at
 * 44100Hz (Most Likely)
 */
class FloatFilter
{
public:
    void update(float &currentValue, float destinationValue, float coefficient)
    {
        currentValue = (destinationValue*coefficient) + (oldValue*(1.0-coefficient));
        oldValue = currentValue;
    }
private:
    float oldValue = 0;
};

/**
 * An abstract class to retain consistency between the distortion, filter and delay modules
 */
class EffectsModule
{
public:
    virtual ~EffectsModule() {}
    void process(const AudioSourceChannelInfo& buffer);
    inline virtual void processSample(float &sampleL, float &sampleR) = 0;
    virtual void setParam(int paramNum, float value) = 0;
    virtual void prepareToPlay (int samplesPerBlockExpected, double sampleRate) {this->sampleRate = sampleRate;}
    inline virtual void updateParams() = 0;
    
protected:
    double sampleRate;
};

/**
 * Very simple hard clipping module
 */
class DistortionModule : public EffectsModule
{
public:
    inline void processSample(float &sampleL, float &sampleR) override;
    void setParam(int paramNum, float value) override;
    inline void updateParams() override;
    
private:
    Atomic<float> gain = 1.0f;
    float currentGain;
    FloatFilter gainFilter;
};

/**
 * Very simple filter module that uses the juce::dsp state variable filter
 */
class FilterModule : public EffectsModule
{
public:
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    inline void processSample(float &sampleL, float &sampleR) override;
    void setParam(int paramNum, float value) override;
    inline void updateParams() override;
    
private:
    dsp::StateVariableFilter::Filter<float> filter[2];
    
    Atomic<float> cutoff;
    float currentCutoff = 1;
    float q;
    FloatFilter paramFilter;
};

/**
 * Very simple delay module
 */

class DelayModule : public EffectsModule
{
public:
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    inline void processSample(float &sampleL, float &sampleR) override;
    void setParam(int paramNum, float value) override;
    inline void updateParams() override;
    
private:
    std::vector<float> bufferL;
    std::vector<float> bufferR;
    
    int maxDelay;
    
    FloatFilter sampleDelayFilter;
    Atomic<float> sampleDelay = 0.0f;
    float currentSampleDelay = 0;
    
    FloatFilter delayLevelFilter;
    Atomic<float> delayLevel = 0.0f;
    float currentDelayLevel = 0.0f;
    
    FloatFilter feedbackFilter;
    Atomic<float> feedback = 0.0f;
    float currentFeedback = 0.0f;
    
    int currentSample = 0;
};

#endif /* DistortionEffect_hpp */
