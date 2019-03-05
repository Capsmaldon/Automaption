//
//  MidiInSelector.cpp
//  Automaption - App
//
//  Created by Cai Smaldon on 24/01/2019.
//

#include "MidiInSelector.hpp"

MidiInSelector::MidiInSelector(MidiEngine* midiEngine) : midiEngine(midiEngine)
{
    //Set up the combo box
    midiSelectionBox =  std::make_unique<ComboBox>();
    StringArray devices = MidiInput::getDevices();
    for(int i = 0; i < devices.size(); i++)
    {
        midiSelectionBox->addItem (devices[i], i+1);
    }
    midiSelectionBox->setSelectedId(1);
    midiSelectionBox->addListener(this);
    addAndMakeVisible(midiSelectionBox.get());
}

void MidiInSelector::resized()
{
    //Allocate space
    Rectangle<int> componentSize = getLocalBounds();
    componentSize.reduce(20, 20);
    midiSelectionBox->setBounds(componentSize);
}

void MidiInSelector::paint (Graphics& g)
{
    
}

void MidiInSelector::comboBoxChanged (ComboBox *midiInSelector)
{
    //Pass the string stored by the combo box onto the midi engine
    String midiSelected = midiInSelector->getText();
    midiEngine->setInput(midiSelected);
}
