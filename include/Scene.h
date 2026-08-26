#ifndef SCENE_H
#define SCENE_H

#include "Observation.h"

class Scene
{
public:
    Observation getObservation() const;
    float updateReward(const Observation& prev, const Observation& curr); 
};

#endif // SCENE_H
