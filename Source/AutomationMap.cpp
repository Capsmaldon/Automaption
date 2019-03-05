//
//  AutomationMap.cpp
//  Automaption - App
//
//  Created by Cai Smaldon on 18/12/2018.
//

#include "AutomationMap.hpp"

AutomationMap::AutomationMap(Vector2D startPoint)
{
    //The starting point is given on creation
    waypoints.push_back(startPoint);
    
    //Technically every marker starts in the middle but it's not too noticeable
    markerPosition.x = 0.5f;
    markerPosition.y = 0.5f;
    
    //Needs to have some velocity to trigger the point incrementation,
    //otherwise marker will just be frozen forever
    markerVelocity.x = 0.0001f;
    markerVelocity.y = 0.0001f;
    
    //Get the ball rolling
    calculateMarkerVelocity();
}

void AutomationMap::paint (Graphics& g)
{
    //Make the background
    Colour defaultColour = Desktop::getInstance().getDefaultLookAndFeel()
    .findColour (ResizableWindow::backgroundColourId);
    g.setColour(defaultColour);
    g.fillAll();
    
    float width = getWidth();
    float height = getHeight();
    
    g.setColour(juce::Colours::lightskyblue);
    
    //Draw all the lines
    for (int i = 0; i < waypoints.size(); i++)
    {
        float point1x = waypoints[i].x*width;
        float point1y = height-(waypoints[i].y*height);
        
        float point2x, point2y;
        
        if(i == waypoints.size()-1)
        {
            point2x = waypoints[0].x*width;
            point2y = height-(waypoints[0].y*height);
        }
        else
        {
            point2x = waypoints[i+1].x*width;
            point2y = height-(waypoints[i+1].y*height);
        }
        g.drawLine(point1x, point1y, point2x, point2y, trailThickness);
    }
    
    //Draw the point map
    for(Vector2D point : waypoints)
    {
        float point1x = (point.x*width) - waypointSize/2;
        float point1y = (height-(point.y*height)) - waypointSize/2;
        
        //Draw each of the points
        g.setColour(juce::Colours::lightskyblue);
        g.drawEllipse(point1x, point1y, waypointSize, waypointSize, waypointThickness);
        
        g.setColour(defaultColour);
        g.fillEllipse(point1x, point1y, waypointSize, waypointSize);
    }
    
    //Draw the marker
    g.setColour(juce::Colours::white);
    g.fillEllipse((markerPosition.x*width) - markerSize/2, ((height-markerPosition.y*height)) - markerSize/2, markerSize, markerSize);
}

void AutomationMap::resized()
{
    //Everything is handled in paint
}

void AutomationMap::calculateMarkerVelocity()
{
    if((waypoints[nextWaypoint].x != markerPosition.x) || (waypoints[nextWaypoint].y != markerPosition.y))
    {
        //Start moving towards the next point
        float xSpeed = abs((waypoints[nextWaypoint].x - markerPosition.x));
        float ySpeed = abs((waypoints[nextWaypoint].y - markerPosition.y));
        
        float xDirection = (waypoints[nextWaypoint].x - markerPosition.x);
        if(xDirection > 0) xDirection = 1;
        else xDirection = -1;
        
        float yDirection = (waypoints[nextWaypoint].y - markerPosition.y);
        if(yDirection > 0) yDirection = 1;
        else yDirection = -1;
        
        float xRatio = xSpeed / (xSpeed + ySpeed);
        float yRatio = 1 - xRatio;
        
        markerVelocity.x =  (xRatio * maxSpeed) * xDirection;
        markerVelocity.y =  (yRatio * maxSpeed) * yDirection;
    }
}

void AutomationMap::incrementPoint()
{
    //Increment/Wrap-around for waypoints
    prevWaypoint = nextWaypoint;
    nextWaypoint++;
    if(nextWaypoint >= waypoints.size())
    {
        nextWaypoint = 0;
    }
    
}

int AutomationMap::pointDetection(int x, int y)
{
    float width = getWidth();
    float height = getHeight();
    
    int pointHit = NO_HIT;
    
    //When you click, detect if the click was within the hitbox of the point
    for(int i = 0; i < waypoints.size(); i++)
    {
        Vector2D point = waypoints[i];
        
        //Magic numbers - it's a display/interaction thing, these numbers feel nice
        if(x > (point.x*width)-(waypointSize) &&
           x < (point.x*width)+(waypointSize) &&
           y > ((1-point.y)*height)-(waypointSize) &&
           y < ((1-point.y)*height)+(waypointSize))
        {
            pointHit = i;
        }
    }
    //Returns the index of the point hit
    return pointHit;
}

//Redraws only the point and some area around it to save on processing
void AutomationMap::repaintPoint(Vector2D point, int circleSize)
{
    repaint(point.x*getWidth()-(circleSize+1), (getHeight()-point.y*getHeight())-(circleSize+1), circleSize*3, circleSize*3);
}

void AutomationMap::tick(float &param1, float &param2)
{
    //Remember where the marker was before for redrawing later
    prevMarkerPosition = markerPosition;
    
    //Every tick calculate the velocity to get to the next point
    calculateMarkerVelocity();
    
    //If there are no points on the map then the marker moves to 0, 0
    if(waypoints.size() == 0)
    {
        markerVelocity.x = 0;
        markerVelocity.y = 0;
    }
    
    //Smooth any new position of the marker given with the current position - coefficient controlled by weight slider
    Vector2D filteredVelocity = markerFilter.update(markerVelocity, filterCoefficient);
    markerPosition.x += filteredVelocity.x;
    markerPosition.y += filteredVelocity.y;
    
    //Marker circles because velocity is reversed when one of the axis haven't been crossed yet
    //Needs to cross both axis at the same time
    if(((markerVelocity.x >= 0 && markerPosition.x >= waypoints[nextWaypoint].x) ||
       (markerVelocity.x <= 0 && markerPosition.x <= waypoints[nextWaypoint].x)) &&
       ((markerVelocity.y >= 0 && markerPosition.y >= waypoints[nextWaypoint].y) ||
       (markerVelocity.y <= 0 && markerPosition.y <= waypoints[nextWaypoint].y)))
    {
        if(filterCoefficient >= 0.999) markerPosition = waypoints[nextWaypoint];
        incrementPoint();
    }
    
    //Marker sticks to the waypoint if there's only one waypoint on the map
    if(waypoints.size() == 1) markerPosition = waypoints[0];
    
    //Lock the marker position to within the boundaries of the component
    if(markerPosition.x < 0) markerPosition.x = 0;
    if(markerPosition.x > 1) markerPosition.x = 1;
    if(markerPosition.y < 0) markerPosition.y = 0;
    if(markerPosition.y > 1) markerPosition.y = 1;
    
    /*This method of selecting the area to repaint cuts down on most of the cost
     (Compared to repainting the entire window)*/
    repaintPoint(markerPosition, markerSize);
    repaintPoint(prevMarkerPosition, markerSize);
    
    //Update the parameters
    param1 = markerPosition.x;
    param2 = markerPosition.y;
}

void AutomationMap::mouseDown(const MouseEvent &event)
{
    //Get index of point hit if there is a hit
    selectedWaypoint = pointDetection(event.getMouseDownX(), event.getMouseDownY());
    
    //If no points were hit, make a new one
    if(selectedWaypoint == NO_HIT)
    {
        Vector2D newPoint;
        newPoint.x = (float)event.getMouseDownX() /getWidth();
        newPoint.y = (float)event.getMouseDownY() /getHeight();
        waypoints.push_back(newPoint);
        selectedWaypoint = (int)waypoints.size()-1;
        calculateMarkerVelocity();
        repaint();
    }
}

void AutomationMap::mouseDrag(const MouseEvent &event)
{
    float mousePosX = event.position.x;
    float mousePosY = event.position.y;
    
    //Stop waypoint from being dragged off map
    if(mousePosX < 0)
    {
        mousePosX = 0;
    }
    else if(mousePosX > getWidth())
    {
        mousePosX = getWidth();
    }
    
    if(mousePosY < 0)
    {
        mousePosY = 0;
    }
    else if(mousePosY > getHeight())
    {
        mousePosY = getHeight();
    }
    
    //Convert the waypoint into a screen position
    waypoints[selectedWaypoint].x = mousePosX / getWidth();
    waypoints[selectedWaypoint].y = 1-(mousePosY / getHeight());
    
    //Calculate new velocity and redraw (Won't move because it's not ticked)
    calculateMarkerVelocity();
    repaint();
}

void AutomationMap::mouseDoubleClick (const MouseEvent &event)
{
    //If a point is double clicked, delete it
    selectedWaypoint = pointDetection(event.getMouseDownX(), event.getMouseDownY());
    
    if(selectedWaypoint != NO_HIT)
    {
        waypoints.erase(waypoints.begin() + selectedWaypoint);
        calculateMarkerVelocity();
        repaint();
    }
}

void AutomationMap::setMaxSpeed(float value)
{
    maxSpeed = value;
    calculateMarkerVelocity();
}

void AutomationMap::setFilterCoefficient(float value)
{
    filterCoefficient = value;
}
