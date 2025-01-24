#include "cMoveRelativeTime.h"

float CalculateDirection(const glm::vec3& velocity, const glm::vec3& forwardVector)
{
    if (glm::length(velocity) < 0.001f)
    {
        return 0.0f; // No significant movement
    }

    // Normalize the velocity and forward vector
    glm::vec3 normalizedVelocity = glm::normalize(velocity);
    glm::vec3 normalizedForward = glm::normalize(forwardVector);

    // Compute the right vector (assuming forward is along the X-axis in world space)
    glm::vec3 rightVector = glm::cross(normalizedForward, glm::vec3(0.0f, 1.0f, 0.0f)); // Y-axis as up

    // Dot products to project velocity onto the forward and right vectors
    float forwardDot = glm::dot(normalizedForward, normalizedVelocity);
    float rightDot = glm::dot(rightVector, normalizedVelocity);

    // Calculate the angle using atan2 (rightDot is the y-component, forwardDot is the x-component)
    float angle = glm::degrees(atan2(rightDot, forwardDot));

    return angle; // Angle in degrees
}

void cMoveRelativeTime::Init(cPhysics::sPhysInfo* pObject, glm::vec3 endXYZ, double timeToMove)
{
    this->m_pObject = pObject;
    this->m_endXYZ = endXYZ;
    this->m_timeToMove = (timeToMove > 0.0) ? timeToMove : 0.01; // Avoid invalid or zero time
    this->m_elapsedTime = 0.0;
    this->m_initialPosition = pObject->position; // Store the starting position
}

void cMoveRelativeTime::OnStart(void)
{
    this->m_elapsedTime = 0.0;

    glm::vec3 totalDistance = this->m_endXYZ - this->m_pObject->position;
    this->m_pObject->velocity = totalDistance / static_cast<float>(this->m_timeToMove);

    // Store the initial position for reference
    this->m_initialPosition = this->m_pObject->position;
}

void cMoveRelativeTime::Update(double deltaTime)
{
    this->m_elapsedTime += deltaTime;

    if (this->m_elapsedTime >= this->m_timeToMove)
    {
        this->m_pObject->position = this->m_endXYZ;
        this->m_pObject->velocity = glm::vec3(0.0f);
        return;
    }

    //glm::vec3 forwardVector = glm::vec3(0.0f, 0.0f, 1.0f); // Assuming +Z is forward
    //float targetAngle = CalculateDirection(this->m_pObject->velocity, forwardVector);

    //// Interpolate the Y rotation for smooth transitions
    //float currentAngle = this->m_pObject->rotation.y;
    //float smoothedAngle = glm::mix(currentAngle, targetAngle, static_cast<float>(deltaTime) * 5.0f); // Adjust 5.0f for speed of interpolation

    //// Update the object's rotation
    //this->m_pObject->rotation.y = smoothedAngle;
    //this->m_pObject->pAssociatedDrawingMeshInstance->rotationEulerXYZ.y = -smoothedAngle; // Flip if required for mesh
}

bool cMoveRelativeTime::isFinished(void)
{
    // Check if the elapsed time has reached or exceeded the duration
    return this->m_elapsedTime >= this->m_timeToMove;
}

void cMoveRelativeTime::OnFinished(void)
{
    // Final position correction
    this->m_pObject->position = this->m_endXYZ;

    // Stop all motion
    this->m_pObject->velocity = glm::vec3(0.0f);
    this->m_pObject->acceleration = glm::vec3(0.0f);
}

void cMoveRelativeTime::Stop()
{
    // Immediately stop the command
    this->m_pObject->velocity = glm::vec3(0.0f);
    this->m_pObject->acceleration = glm::vec3(0.0f);
    std::cout << "Command Stopped: Object halted prematurely.\n";
}
