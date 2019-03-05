#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioEngine.hpp"
#include "MidiEngine.hpp"
#include "AutomationAtlas.hpp"
#include "MidiInSelector.hpp"
#include "MainControls.hpp"

//==============================================================================

/**
 * Handles allocation of space for each component in the application
 */

class MainComponent : public Component
{
public:
    //==============================================================================
    MainComponent(AudioEngine* audioEngine, MidiEngine* midiEngine);

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
    std::unique_ptr<AutomationAtlas> automationAtlas;
    std::unique_ptr<ParameterHandler> parameterHandler;
    std::unique_ptr<MidiInSelector> midiInSelector;
    std::unique_ptr<MainControls> mainControls;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
