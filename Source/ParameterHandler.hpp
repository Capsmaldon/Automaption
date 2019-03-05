//
//  ParameterHandler.hpp
//  Automaption - App
//
//  Created by Cai Smaldon on 25/12/2018.
//

#ifndef ParameterHandler_hpp
#define ParameterHandler_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "CSynth.hpp"

/**
 * Routes which modules accept from which map and co-ordinates
 */

class ParameterHandler : public Component
{
public:
    ParameterHandler(CSynth* synth);
    void processMapData(int &id, float &param1, float &param2);
    void resized() override;
    void paint (Graphics& g) override;
    
private:
    CSynth* synth;
    std::vector<EffectsModule*> modules;
    
    Label routingLabel[5];
    ComboBox routingMap[5];
    ComboBox routingAxis[5];
};

#endif /* ParameterHandler_hpp */
