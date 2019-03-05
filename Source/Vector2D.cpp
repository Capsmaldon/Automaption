//
//  VectorFilter.cpp
//  Automaption - App
//
//  Created by Cai Smaldon on 19/12/2018.
//

#include "Vector2D.hpp"


Vector2D VectorFilter::update(Vector2D newVector, float coefficient)
{
    //Simple low pass filter algorithm applied to both x and y of 2D vector
    Vector2D filteredVector;
    filteredVector.x = (newVector.x*coefficient) + (oldVector.x*(1.0-coefficient));
    filteredVector.y = (newVector.y*coefficient) + (oldVector.y*(1.0-coefficient));
    oldVector = filteredVector;
    return filteredVector;
}
