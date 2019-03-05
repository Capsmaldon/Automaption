//
//  VectorFilter.hpp
//  Automaption - App
//
//  Created by Cai Smaldon on 19/12/2018.
//

#ifndef VectorFilter_hpp
#define VectorFilter_hpp

#include <iostream>


/**
 * Utility container for co-ordinates
 */
struct Vector2D
{
    Vector2D(){}
    
    Vector2D(float x, float y)
    {
        this->x = x;
        this->y = y;
    }
    
    float x;
    float y;
};

/**
 * Filter to smooth out the position of the marker as it travels between points
 */
class VectorFilter
{
public:
    Vector2D update(Vector2D newVector, float coefficient);
private:
    Vector2D oldVector;
};

#endif /* VectorFilter_hpp */
