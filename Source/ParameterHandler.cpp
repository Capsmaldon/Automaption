//
//  ParameterHandler.cpp
//  Automaption - App
//
//  Created by Cai Smaldon on 25/12/2018.
//

#include "ParameterHandler.hpp"

ParameterHandler::ParameterHandler(CSynth* synth) : synth(synth)
{
    //Hard coded labels for each of the modules
    String names[5] {"Distortion","LPF","Delay Time", "Delay Level", "Feedback"};
    
    //Add all the generic juce components and set up values for the combo boxes
    for (int i = 0; i < 5; i++)
    {
        routingLabel[i].setText(names[i], dontSendNotification);
        addAndMakeVisible(routingLabel[i]);
    }
    
    for (int i = 0; i < 5; i++)
    {
        addAndMakeVisible(routingMap[i]);
        addAndMakeVisible(routingAxis[i]);
        
        routingMap[i].addItem(String("1"), 1);
        routingMap[i].addItem(String("2"), 2);
        routingMap[i].addItem(String("3"), 3);
        routingMap[i].addItem(String("4"), 4);
        
        routingAxis[i].addItem(String("x"), 1);
        routingAxis[i].addItem(String("y"), 2);
    }
    
    //Set defaults for all of the combo boxes
    routingMap[0].setSelectedId(1);
    routingMap[1].setSelectedId(1);
    routingMap[2].setSelectedId(2);
    routingMap[3].setSelectedId(3);
    routingMap[4].setSelectedId(3);
    
    routingAxis[0].setSelectedId(1);
    routingAxis[1].setSelectedId(2);
    routingAxis[2].setSelectedId(1);
    routingAxis[3].setSelectedId(1);
    routingAxis[4].setSelectedId(2);
    
    //Store a reference of each of the modules for access later
    modules.push_back(synth->getDistortionModule());
    modules.push_back(synth->getFilterModule());
    modules.push_back(synth->getDelayModule());
}

void ParameterHandler::processMapData(int &id, float &param1, float &param2)
{
    //Identify which map just sent it's co-ordinate values
    for(int i = 0; i < 5; i++)
    {
        int mapNum = routingMap[i].getSelectedId();
        
        //Route parameters set by the user to the corresponding effects unit
        if(mapNum == id+1)
        {
            float param = routingAxis[i].getSelectedId() == 1 ? param1 : param2;
            
            if(i < 3)
            {
                modules[i]->setParam(0, param);
            }
            else if(i == 3) modules[2]->setParam(1, param);
            else if(i == 4) modules[2]->setParam(2, param);
        }
    }
}

void ParameterHandler::resized()
{
    //Allocate space
    Rectangle<int> componentSize = getLocalBounds();
    
    for (int i = 0; i < 5; i++)
    {
        Rectangle<int> elementRect = componentSize.removeFromTop(getHeight()*0.2);
        routingLabel[i].setBounds(elementRect.removeFromRight(componentSize.getWidth()*0.3));
        routingMap[i].setBounds(elementRect.removeFromLeft(componentSize.getWidth()*0.35));
        routingAxis[i].setBounds(elementRect);
    }
}

void ParameterHandler::paint (Graphics& g)
{
    
}
