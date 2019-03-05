//
//  MainControls.cpp
//  Automaption - App
//
//  Created by Cai Smaldon on 25/01/2019.
//

#include "MainControls.hpp"

MainControls::MainControls(CSynth* cSynth) : cSynth(cSynth)
{
    //Make a combo box and populate it with all the wavetables found in the wavetables folder
    wavetableSelect = std::make_unique<ComboBox>();
    wavetableSelect->addListener(this);
    addAndMakeVisible(wavetableSelect.get());
    
    //Adds a list of 'Wavetable entries' found in the wavetables folder
    loadWavetables();
    
    for (int i = 0; i < wavetables.size(); i++)
    {
        wavetableSelect->addItem(wavetables[i]->name, wavetables[i]->index+1);
    }
    wavetableSelect->setSelectedId(1);
    
    //Setup for the master volume dial
    masterVolume = std::make_unique<Slider>(Slider::SliderStyle::Rotary, Slider::TextBoxBelow);
    masterVolume->setRange(0, 1);
    masterVolume->setValue(0.8);
    masterVolume->addListener(this);
    addAndMakeVisible(masterVolume.get());
}

void MainControls::resized()
{
    //Allocate space for volume
    Rectangle<int> componentSize = getLocalBounds();
    masterVolume->setBounds(componentSize.removeFromRight(componentSize.getWidth()*0.3));
    
    //Allocate space for combo box
    componentSize.reduce(20, 20);
    wavetableSelect->setBounds(componentSize);
}

void MainControls::paint (Graphics& g)
{
    
}

void MainControls::loadWavetables()
{
    /*
     * This should work for both release and debug builds so long
     * as the wavetables folder is in the same place as the projucer.
     * If this gets hit The wavetables folder wasn't found so the
     * relative filepath isn't working
     * All prepackaged wavetables are taken from:
     * https://www.adventurekid.se/akrt/waveforms/adventure-kid-waveforms/
     */
    
    
    File filepath = File::getSpecialLocation(File::invokedExecutableFile);
    for(int i = 0; i < 8; i++){ filepath = filepath.getParentDirectory();}
    
    jassert(filepath.getChildFile("Wavetables").exists());
    DirectoryIterator iterator(File(filepath.getChildFile("Wavetables")), false, "*.wav");
    
    /*
      //If you could just comment out the above block, uncomment this block and drop in the absolute filepath between the speech marks it will work.
      //Any folder with .wav format wavetables will work
     DirectoryIterator iterator(File(""), false, "*.wav");
    */
    
    /*
     * Iterate through the folder adding all the found wavetables to memory
     * -While this could be done in a more memory efficient way, a file chooser
     * to pick a wavetable would freeze the metronome so the map would freeze
     * while a file was being chosen
     */
    while(iterator.next())
    {
        File file = iterator.getFile();
        AudioFormatManager formatManager;
        formatManager.registerBasicFormats();
        AudioFormatReader* reader = formatManager.createReaderFor(file);
        
        int index = (int)wavetables.size();
        wavetables.push_back(std::make_unique<WavetableEntry>());
        WavetableEntry* wavetableEntry = wavetables[wavetables.size()-1].get();
        wavetableEntry->index = index;
        wavetableEntry->name = file.getFileName();
        DBG(file.getFileName());
        wavetableEntry->wavetable.setSize(reader->numChannels,
                                          (int) reader->lengthInSamples);
        
        //This function requires an audio sample buffer, so the wavetable entry needs to be an audio buffer
        reader->read(&wavetableEntry->wavetable,
                     0,
                     (int) reader->lengthInSamples,
                     0,
                     true,
                     true);
        
        delete reader;
    }
}

void MainControls::comboBoxChanged (ComboBox *wavetableSelector)
{
    //Pass on the wavetable entry to the synth
    int id = wavetableSelector->getSelectedId()-1;
    cSynth->setWavetable(wavetables[id].get());
}

void MainControls::sliderValueChanged (Slider *slider)
{
    //Set the master volume of the synth, not currently smoothed so there is clicking
    cSynth->setLevel(slider->getValue());
}


