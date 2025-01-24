#pragma once

#include "iCommand.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include "cPhysics.h"

// Command to follow another object with an offset over a duration
class cOrientToMove : public iCommand
{
public:
    // Call this to set the values
    void Init(cPhysics::sPhysInfo* pFollower, cPhysics::sPhysInfo* pToFollow, glm::vec3 offset, double duration);

    // From iCommand interface
    virtual void OnStart(void);
    virtual void Update(double deltaTime);
    virtual bool isFinished(void);
    virtual void OnFinished(void);

private:
    glm::vec3 m_offset;           // Offset from the object being followed
    cPhysics::sPhysInfo* m_pFollower;   // The object that will follow
    cPhysics::sPhysInfo* m_pToFollow;   // The object to follow
    double m_duration;             // Duration to move to the offset
    double m_elapsedTime;          // Time elapsed since the start of the movement
};
