//
//  AutomationMapHandler.cpp
//  Automaption - App
//
//  Created by Cai Smaldon on 20/12/2018.
//

#include "AutomationMapContainer.hpp"

AutomationMapContainer::AutomationMapContainer(int id, Vector2D startPoint) : id(id)
{
    //Make an automation map
    automationMap = std::make_unique<AutomationMap>(startPoint);
    addAndMakeVisible(automationMap.get());
    
    //Add sliders and corresponding labels
    sliders.push_back(std::make_unique<Slider>("Weight"));
    labels.push_back(std::make_unique<Label>("Weight"));
    labels[0]->setText("Weight", NotificationType::dontSendNotification);
    addAndMakeVisible(sliders[0].get());
    addAndMakeVisible(labels[0].get());
    sliders[0]->setRange(0.0000001, 1);
    sliders[0]->addListener(this);
    sliders[0]->setValue(0.7);
    
    sliders.push_back(std::make_unique<Slider>("Speed"));
    labels.push_back(std::make_unique<Label>("Speed"));
    labels[1]->setText("Speed", NotificationType::dontSendNotification);
    addAndMakeVisible(sliders[1].get());
    addAndMakeVisible(labels[1].get());
    sliders[1]->setRange(0.0000001, 0.5);
    sliders[1]->addListener(this);
    sliders[1]->setValue(0.02);
}

void AutomationMapContainer::resized()
{
    //Allocate space for the map, sliders and labels
    Rectangle<int> componentSize = getLocalBounds();
    
    componentSize.removeFromLeft(borderSize);
    componentSize.removeFromRight(borderSize);
    componentSize.removeFromTop(borderSize);
    componentSize.removeFromBottom(borderSize);
    
    int remainingHeight = componentSize.getHeight();
    automationMap->setBounds(componentSize.removeFromTop(remainingHeight*0.9));
    
    Rectangle<int> slider0Rect = componentSize.removeFromTop(remainingHeight*0.05);
    sliders[0]->setBounds(slider0Rect.removeFromRight(slider0Rect.getWidth() * 0.9));
    labels[0]->setBounds(slider0Rect);
    
    Rectangle<int> slider1Rect = componentSize.removeFromTop(remainingHeight*0.05);
    sliders[1]->setBounds(slider1Rect.removeFromRight(slider1Rect.getWidth() * 0.9));
    labels[1]->setBounds(slider1Rect);
}

void AutomationMapContainer::paint(Graphics &g)
{
    //Background colour
    g.setColour(juce::Colours::lightskyblue);
    g.fillAll();
}

void AutomationMapContainer::sliderValueChanged (Slider *slider)
{
    if(slider->getName() == "Weight")
    {
        //Scaled to make the range more intuitive
        automationMap->setFilterCoefficient(1- (pow((slider->getValue()), 2)));
    }
    else if(slider->getName() == "Speed")
    {
        automationMap->setMaxSpeed(slider->getValue());
    }
}

void AutomationMapContainer::tick(int &id, float &param1, float &param2)
{
    id = this->id;
    automationMap->tick(param1, param2);
}
