//
//  DistortionEffect.cpp
//  Automaption - App
//
//  Created by Cai Smaldon on 24/12/2018.
//

#include "EffectsModules.hpp"

void EffectsModule::process(const AudioSourceChannelInfo& buffer)
{
    //Get the buffer and send them to process sample for specific processing
    auto* leftBuffer  = buffer.buffer->getWritePointer (0, buffer.startSample);
    auto* rightBuffer = buffer.buffer->getWritePointer (1, buffer.startSample);
    
    for(auto sample = 0; sample < buffer.numSamples; ++sample)
    {
        float *inL = &leftBuffer[sample];
        float *inR = &rightBuffer[sample];
        
        /*
         Really inefficient, ideally would send buffers
         These two functions have been tagged with the keyword inline
         As an attempt to expand out the function and avoid the function
         call though I don't think it works
         
         Also it was done this way to abstract the necessity of updating parameters
         in every derived class at sample rate, though that probably would have been
         more efficient
         */
        processSample(*inL, *inR);
        updateParams();
    }
}

inline void DistortionModule::processSample(float &sampleL, float &sampleR)
{
    //Hard clipping to left and right channels
    sampleL *= currentGain;
    if(sampleL > 1.0f) sampleL = 1.0f;
    else if(sampleL < -1.0f) sampleL = -1.0f;
    
    sampleR *= currentGain;
    if(sampleR > 1.0f) sampleR = 1.0f;
    else if(sampleR < -1.0f) sampleR = -1.0f;
}
void DistortionModule::setParam(int paramNum, float value)
{
    /*
     Hard set the new parameter value
     This receives from the message thread so atomics are used
     to avoid conflicts
     */
    switch(paramNum)
    {
        case 0:
            //Scaled value
            gain.set((value*30.0f)+1.0f);
            break;
    }
}

inline void DistortionModule::updateParams()
{
    //Smooth the new parameter value with the current one
    float newGain = gain.get();
    gainFilter.update(currentGain, newGain, 0.001f);
}

void FilterModule::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    this->sampleRate = sampleRate;
    
    dsp::ProcessSpec processSpec;
    processSpec.sampleRate = sampleRate;
    processSpec.maximumBlockSize = samplesPerBlockExpected;
    processSpec.numChannels = 2;
    
    filter[0].prepare(processSpec);
    filter[1].prepare(processSpec);
    
    q = 1.0/MathConstants<float>::sqrt2;
    
    filter[0].parameters->type = dsp::StateVariableFilter::Parameters<float>::Type::lowPass;
    filter[0].parameters->setCutOffFrequency(sampleRate, 22000, q);
    
    filter[1].parameters->type = dsp::StateVariableFilter::Parameters<float>::Type::lowPass;
    filter[1].parameters->setCutOffFrequency(sampleRate, 22000, q);
}

inline void FilterModule::processSample(float &sampleL, float &sampleR)
{
    //Uses the juce::dsp filter module, making my own biquad filter is beyond me
    sampleL = filter[0].processSample(sampleL);
    sampleR = filter[1].processSample(sampleR);
}

void FilterModule::setParam(int paramNum, float value)
{
    switch(paramNum)
    {
        case 0:
            value = pow(value, 3) * 22000.0f;
            cutoff.set(value);
            break;
    }
}

inline void FilterModule::updateParams()
{
    float newCutoff = cutoff.get();
    if(newCutoff < 1) newCutoff =  1;
    paramFilter.update(currentCutoff, newCutoff, 0.001f);
    filter[0].parameters->setCutOffFrequency(sampleRate, currentCutoff, q);
    filter[1].parameters->setCutOffFrequency(sampleRate, currentCutoff, q);
}

void DelayModule::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    this->sampleRate = sampleRate;
    bufferL.resize((int)sampleRate * 2); //2s max delay
    bufferR.resize((int)sampleRate * 2);
    maxDelay = sampleRate * 2;
    
    for(int i = 0; i < bufferL.size(); i++)
    {
        bufferL[i] = 0;
        bufferR[i] = 0;
    }
}

void DelayModule::processSample(float &sampleL, float &sampleR)
{
    //Add samples to buffer
    bufferL[currentSample] = sampleL + (bufferL[currentSample] * currentFeedback);
    bufferR[currentSample] = sampleR + (bufferR[currentSample] * currentFeedback);
    
    //Add delayed signal to input (Interpolating between two samples)
    float indexDelay = currentSample - currentSampleDelay;
    if(indexDelay < 0) indexDelay += maxDelay;
    
    int i1 = (int)indexDelay;
    int i2 = (int)indexDelay + 1;
    if(i2 > maxDelay) i2 = 0;
    
    float ratio =  indexDelay - (int)indexDelay;
    
    float delaySampleL = (bufferL[i2] * ratio) + (bufferL[i1] * (1.0f-ratio));
    float delaySampleR = (bufferL[i2] * ratio) + (bufferR[i1] * (1.0f-ratio));
    
    sampleL = (sampleL * 1-currentDelayLevel) + (delaySampleL * currentDelayLevel);
    sampleR = (sampleR * 1-currentDelayLevel) + (delaySampleR * currentDelayLevel);
    
    //Tick buffer index
    currentSample++;
    if(currentSample > maxDelay) currentSample = 0;
}

void DelayModule::setParam(int paramNum, float value)
{
    //Route the new value
    switch(paramNum)
    {
        case 0:
            //value is a percentage of max delay time (0 to 1.0 like everything else)
            sampleDelay.set(value);
            break;
        case 1:
            delayLevel.set(value);
            break;
        case 2:
            feedback.set(value*0.95);
            break;
    }
}

inline void DelayModule::updateParams()
{
    //Delay time smoothed much more to attenuate the lasery sound
    sampleDelayFilter.update(currentSampleDelay, sampleDelay.get() * maxDelay, 0.00001f);
    delayLevelFilter.update(currentDelayLevel, delayLevel.get(), 0.0001f);
    feedbackFilter.update(currentFeedback, feedback.get(), 0.0001f);
}

