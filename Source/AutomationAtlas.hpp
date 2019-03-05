//
//  AutomationAtlas.hpp
//  Automaption - App
//
//  Created by Cai Smaldon on 24/12/2018.
//

#ifndef AutomationAtlas_hpp
#define AutomationAtlas_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioEngine.hpp"
#include "AutomationMapContainer.hpp"
#include "EffectsModules.hpp"

enum
{
    MAX_MAPS = 4
};

/**
 * Container for MAX_MAPS number of automation map containers,
 * however the parameter handler is only set up to accept from 4 maps
 */

class AutomationAtlas : public Component,
                        public Button::Listener,
                        public Timer
{
public:
    AutomationAtlas(ParameterHandler* paramHandler);
    void resized() override;
    void buttonClicked (Button *button) override;
    void timerCallback() override;
    
private:
    ParameterHandler* paramHandler;
    
    TextButton buttons[MAX_MAPS];
    std::unique_ptr<AutomationMapContainer> automationMaps[MAX_MAPS];
};

#endif /* AutomationAtlas_hpp */
