#include "cFollowObject.h"
#include <glm/gtx/norm.hpp> // For glm::length2()

void cFollowObject::OnStart(void)
{
    // Initialize elapsed time and clear the follower's velocity
    m_elapsedTime = 0.0;
    m_pFollower->velocity = glm::vec3(0.0f);
}

void cFollowObject::Update(double deltaTime)
{
    // Update the elapsed time
    m_elapsedTime += deltaTime;

    // Calculate the target position (offset from the object to follow)
    glm::vec3 targetPosition = m_pToFollow->position + m_offset;

    // Calculate the progress (clamped between 0 and 1)
    double progress = glm::clamp(m_elapsedTime / m_duration, 0.0, 1.0);

    // Calculate the required velocity to reach the target position
    glm::vec3 currentPosition = m_pFollower->position;
    glm::vec3 direction = targetPosition - currentPosition;

    if (glm::length2(direction) > 0.0001f) // Avoid jitter for very small distances
    {
        // Normalize the direction vector
        glm::vec3 forward = glm::normalize(direction);

        // Calculate yaw (rotation around Y-axis)
        float yaw = glm::degrees(atan2(forward.x, forward.z));

        // Calculate pitch (rotation around X-axis)
        float pitch = glm::degrees(atan2(forward.y, glm::length(glm::vec2(forward.x, forward.z))));

        // Update the follower's rotation
        m_pFollower->rotation = glm::vec3(pitch, yaw, 0.0f); // Assuming no roll is needed

        // Update velocity to move towards the target
        glm::vec3 desiredVelocity = direction / static_cast<float>(m_duration * (1.0 - progress));
        m_pFollower->velocity = desiredVelocity;
    }
    else
    {
        // Stop the follower if close enough to the target
        m_pFollower->velocity = glm::vec3(0.0f);
    }
}


bool cFollowObject::isFinished(void)
{
    // The command is finished when the elapsed time exceeds the duration
    return m_elapsedTime >= m_duration;
}

void cFollowObject::OnFinished(void)
{
    // Stop the follower and ensure it matches the target position
    m_pFollower->velocity = glm::vec3(0.0f);
    m_pFollower->position = m_pToFollow->position + m_offset;
}

void cFollowObject::Init(cPhysics::sPhysInfo* pFollower, cPhysics::sPhysInfo* pToFollow, glm::vec3 offset, double duration)
{
    // Initialize the follower, object to follow, offset, and duration
    this->m_pFollower = pFollower;
    this->m_pToFollow = pToFollow;
    this->m_offset = offset;
    this->m_duration = duration;
    this->m_elapsedTime = 0.0;
}
