//
//  MainControls.hpp
//  Automaption - App
//
//  Created by Cai Smaldon on 25/01/2019.
//

#ifndef MainControls_hpp
#define MainControls_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioEngine.hpp"

/**
 * Component to control the wavetable selected and master volume
 */

class MainControls : public Component,
                     public ComboBox::Listener,
                     public Slider::Listener
{
public:
    MainControls(CSynth* cSynth);
    
    void resized() override;
    void paint (Graphics& g) override;
    
    void loadWavetables();
    
    virtual void comboBoxChanged (ComboBox *wavetableSelect) override;
    virtual void sliderValueChanged (Slider *slider) override;
private:
    CSynth* cSynth;
    std::unique_ptr<ComboBox> wavetableSelect;
    std::vector<std::unique_ptr<WavetableEntry>> wavetables;
    
    std::unique_ptr<Slider> masterVolume;
};

#endif /* MainControls_hpp */
