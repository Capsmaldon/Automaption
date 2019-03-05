//
//  AutomationAtlas.cpp
//  Automaption - App
//
//  Created by Cai Smaldon on 24/12/2018.
//

#include "AutomationAtlas.hpp"

AutomationAtlas::AutomationAtlas(ParameterHandler* paramHandler) : paramHandler(paramHandler)
{
    //Add all the automation maps to the same space and add a button corresponding to each map in a row at the top
    for(int i = 0; i < MAX_MAPS; i++)
    {
        Vector2D startPoint = i == 0? Vector2D(0.5,0.5) : Vector2D(0,0);
        
        automationMaps[i] = std::make_unique<AutomationMapContainer>(i, startPoint);
        addChildComponent(automationMaps[i].get());
        addAndMakeVisible(buttons[i]);
        buttons[i].setButtonText(juce::String(i+1));
        buttons[i].addListener(this);
    }
    addAndMakeVisible(automationMaps[0].get());
    buttons[0].setColour(TextButton::buttonColourId, juce::Colours::green);
    
    /* The precision of updates don't need to be incredibly accurate.
       For general use the user doesn't directly interact with the marker,
       A slightly noticeable delay in updates (maximum of ~50ms at 20Hz) doesn't impact
       the experience too much (Filtering of positions is far more problematic)
     
       20Hz was chosen to reduce CPU consumption since everything is
       rendered by the CPU 60Hz is very expensive
     */
    startTimerHz(20);
}

void AutomationAtlas::resized()
{
    Rectangle<int> componentSize = getLocalBounds();
    int buttonWidth = componentSize.getWidth()/MAX_MAPS;
    
    Rectangle<int> autoMapRect = componentSize.removeFromBottom(getHeight()*0.95);
    
    /*
     Line up all of the buttons - if more maps are added,
     more buttons should be added and allocated an equal amount of space
     */
    for(int i = 0; i < MAX_MAPS; i++)
    {
        Rectangle<int> button = componentSize.removeFromLeft(buttonWidth);
        button.reduce(2, 2);
        buttons[i].setBounds(button);
    }
    
    //Allocate the same space for all of the automation maps
    for(int i = 0; i < MAX_MAPS; i++)
    {
        automationMaps[i]->setBounds(autoMapRect);
    }
}

void AutomationAtlas::buttonClicked (Button *button)
{
    //Hide all maps when a button is clicked, then reveal the one that corresponds to the button
    for(int i = 0; i < MAX_MAPS; i++)
    {
        if(button == &buttons[i])
        {
            buttons[i].setColour(TextButton::buttonColourId, juce::Colours::green);
            automationMaps[i]->setVisible(true);
        }
        else
        {
            buttons[i].setColour(TextButton::buttonColourId, getLookAndFeel().findColour (TextButton::buttonColourId));
            automationMaps[i]->setVisible(false);
        }
    }
}

void AutomationAtlas::timerCallback()
{
    //Tick through all of the maps and pass on the data gotten from each of them to the parameter handler
    int id;
    float param1;
    float param2;
    
    for(int i = 0; i < MAX_MAPS; i++)
    {
        automationMaps[i]->tick(id, param1, param2);
        paramHandler->processMapData(id, param1, param2);
    }
}
