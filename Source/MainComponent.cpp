#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent(AudioEngine* audioEngine, MidiEngine* midiEngine)
{
    //Add all visible components and set the initial size of the window
    parameterHandler = std::make_unique<ParameterHandler>(audioEngine->getSynth());
    addAndMakeVisible(parameterHandler.get());
    
    automationAtlas = std::make_unique<AutomationAtlas>(parameterHandler.get());
    addAndMakeVisible(automationAtlas.get());
    
    midiInSelector = std::make_unique<MidiInSelector>(midiEngine);
    addAndMakeVisible(midiInSelector.get());
    
    mainControls = std::make_unique<MainControls>(audioEngine->getSynth());
    addAndMakeVisible(mainControls.get());
    
    setSize (600, 800);
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    //Paint background
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    //Allocate space for every component owned by this component
    Rectangle<int> windowSize = getBounds();
    int shrinkAmount = 5;
    
    Rectangle<int> automationAtlasRect = windowSize.removeFromTop(windowSize.getHeight()*0.8);
    automationAtlasRect.reduce(shrinkAmount, shrinkAmount);
    automationAtlas->setBounds(automationAtlasRect);
    
    Rectangle<int> parameterHandlerRect = windowSize.removeFromLeft(windowSize.getWidth()*0.4);
    parameterHandlerRect.reduce(shrinkAmount, shrinkAmount);
    parameterHandlerRect.setY(parameterHandlerRect.getY() - shrinkAmount);
    parameterHandlerRect.setHeight(parameterHandlerRect.getHeight() + shrinkAmount);
    parameterHandler->setBounds(parameterHandlerRect);
    
    Rectangle<int> midiInSelectorRect = windowSize.removeFromTop(windowSize.getHeight()*0.5);
    midiInSelectorRect.reduce(shrinkAmount, shrinkAmount);
    midiInSelectorRect.setY(midiInSelectorRect.getY() - shrinkAmount);
    midiInSelectorRect.setHeight(midiInSelectorRect.getHeight() + shrinkAmount);
    midiInSelector->setBounds(midiInSelectorRect);
    
    Rectangle<int> mainControlsRect = windowSize.removeFromTop(windowSize.getHeight());
    mainControlsRect.reduce(shrinkAmount, shrinkAmount);
    mainControlsRect.setY(mainControlsRect.getY() - shrinkAmount);
    mainControlsRect.setHeight(mainControlsRect.getHeight() + shrinkAmount);
    mainControls->setBounds(mainControlsRect);
}
