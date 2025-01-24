#pragma once

#include "iCommand.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/string_cast.hpp> // For debugging/logging
#include "cPhysics.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp> // For rotation matrices
#include <glm/gtx/vector_angle.hpp>     // For angle calculations

// Move from the current location to a new location over a certain amount of time
class cMoveRelativeTime : public iCommand
{
public:
    // Call this to set the values specific to this particular command
    void Init(cPhysics::sPhysInfo* pObject, glm::vec3 endXYZ, double timeToMove);

    // From iCommand interface
    virtual void OnStart(void);
    virtual void Update(double deltaTime);
    virtual bool isFinished(void);
    virtual void OnFinished(void);
    void Stop(); // Optional: Stop the command prematurely

private:
    glm::vec3 m_endXYZ;
    double m_timeToMove;
    cPhysics::sPhysInfo* m_pObject;

    // Calculated values
    double m_elapsedTime;
    glm::vec3 m_initialPosition;
};
