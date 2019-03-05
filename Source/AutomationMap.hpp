//
//  AutomationMap.hpp
//  Automaption - App
//
//  Created by Cai Smaldon on 18/12/2018.
//

#ifndef AutomationMap_hpp
#define AutomationMap_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "Vector2D.hpp"

enum
{
    NO_HIT = -1
};

/**
 * The main feature of the program, used to draw a 'map' of how to alter parameters
 * as time progresses, the output is equal to the marker position scaled to be within
 * the range 0.0 to 1.0
 */

class AutomationMap : public Component
{
public:
    //==============================================================================
    AutomationMap(Vector2D startPoint);
    
    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;
    
    //==============================================================================
    void calculateMarkerVelocity();
    void incrementPoint();
    int pointDetection(int x, int y);
    void repaintPoint(Vector2D point, int circleSize);
    
    //==============================================================================
    void tick(float &param1, float &param2);
    void mouseDown(const MouseEvent &event) override;
    void mouseDrag(const MouseEvent &event) override;
    void mouseDoubleClick (const MouseEvent &event) override;
    
    //==============================================================================
    
    /**
     * How far to move across the map in one tick, 0 is don't move,
     * 1 is in one tick the marker will move across the entire map in one tick
     */
    void setMaxSpeed(float value);
    void setFilterCoefficient(float value);
    
private:
    //==============================================================================
    
    int nextWaypoint = 0;
    int prevWaypoint = 0;
    std::vector<Vector2D> waypoints;
    int selectedWaypoint;
    Vector2D prevMarkerPosition;
    Vector2D markerPosition;
    Vector2D markerVelocity;
    VectorFilter markerFilter;
    
    int waypointSize = 10;
    int waypointThickness = 3;
    int markerSize = 10;
    int markerThickness = 3;
    int trailThickness = 2;
    float maxSpeed = 0.02f;
    float filterCoefficient = 1.0f;
    
    bool xPassed = false;
    bool yPassed = false;
};

#endif /* AutomationMap_hpp */
