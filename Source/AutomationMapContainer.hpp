//
//  AutomationMapHandler.hpp
//  Automaption - App
//
//  Created by Cai Smaldon on 20/12/2018.
//

#ifndef AutomationMapHandler_hpp
#define AutomationMapHandler_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "AutomationMap.hpp"

/**
 * A wrapper for the automation map, this component holds the map
 * and two additional sliders to control parameters related to the
 * marker behaviour
 */

class AutomationMapContainer : public Component,
                             public Slider::Listener
{
public:
    AutomationMapContainer(int id, Vector2D startPoint);
    
    void resized() override;
    void paint (Graphics& g) override;
    
    void sliderValueChanged (Slider *slider) override;
    void tick(int &id, float &param1, float &param2);
    
private:
    int id;
    int borderSize = 2;
    
    std::unique_ptr<AutomationMap> automationMap;
    std::vector<std::unique_ptr<Slider>> sliders;
    std::vector<std::unique_ptr<Label>> labels;
};

#endif /* AutomationMapHandler_hpp */
