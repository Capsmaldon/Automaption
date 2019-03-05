//
//  MidiInSelector.hpp
//  Automaption - App
//
//  Created by Cai Smaldon on 24/01/2019.
//

#ifndef MidiInSelector_hpp
#define MidiInSelector_hpp

#include "MidiEngine.hpp"

/**
 * Small component with a single combo box to tell the midi engine to change the midi input device
 */

class MidiInSelector : public Component,
                       public ComboBox::Listener
{
public:
    MidiInSelector(MidiEngine* midiEngine);
    
    void resized() override;
    void paint (Graphics& g) override;
    
    virtual void comboBoxChanged (ComboBox *midiInSelector) override;
private:
    MidiEngine* midiEngine;
    std::unique_ptr<ComboBox> midiSelectionBox;
};

#endif /* MidiInSelector_hpp */
