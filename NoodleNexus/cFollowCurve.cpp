#include "cFollowCurve.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

void cFollowCurve::Init(cPhysics::sPhysInfo* pObject,
    glm::vec3 start, glm::vec3 control1, glm::vec3 control2, glm::vec3 end, double duration)
{
    this->m_pObject = pObject;
    this->m_start = start;
    this->m_control1 = control1;
    this->m_control2 = control2;
    this->m_end = end;
    this->m_duration = duration;
    this->m_elapsedTime = 0.0;

    // Precompute arc lengths
    this->m_arcLengths = CalculateArcLengths(100, m_start, m_control1, m_control2, m_end);
    this->m_totalArcLength = m_arcLengths.back();
}

void cFollowCurve::OnStart(void)
{
    this->m_elapsedTime = 0.0;
    this->m_pObject->position = m_start; // Start at the beginning of the curve
}

void cFollowCurve::Update(double deltaTime)
{
    this->m_elapsedTime += deltaTime;
    float distance = glm::clamp(static_cast<float>(this->m_elapsedTime / this->m_duration) * m_totalArcLength, 0.0f, m_totalArcLength);

    float t = MapDistanceToParameter(distance, m_arcLengths);
    this->m_pObject->position = CalculateBezierPoint(t, m_start, m_control1, m_control2, m_end);

    // Optionally calculate velocity (derivative of position)
    this->m_pObject->velocity = CalculateBezierVelocity(t, m_start, m_control1, m_control2, m_end);
}

bool cFollowCurve::isFinished(void)
{
    return this->m_elapsedTime >= this->m_duration;
}

void cFollowCurve::OnFinished(void)
{
    this->m_pObject->velocity = glm::vec3(0.0f); // Stop the object
}

std::vector<float> cFollowCurve::CalculateArcLengths(int steps, const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
{
    std::vector<float> arcLengths(steps + 1);
    arcLengths[0] = 0.0f;
    glm::vec3 prevPoint = p0;

    for (int i = 1; i <= steps; ++i)
    {
        float t = static_cast<float>(i) / steps;
        glm::vec3 currentPoint = CalculateBezierPoint(t, p0, p1, p2, p3);
        arcLengths[i] = arcLengths[i - 1] + glm::length(currentPoint - prevPoint);
        prevPoint = currentPoint;
    }

    return arcLengths;
}

float cFollowCurve::MapDistanceToParameter(float distance, const std::vector<float>& arcLengths)
{
    for (size_t i = 1; i < arcLengths.size(); ++i)
    {
        if (distance <= arcLengths[i])
        {
            float segmentLength = arcLengths[i] - arcLengths[i - 1];
            float segmentT = (distance - arcLengths[i - 1]) / segmentLength;
            return (i - 1 + segmentT) / (arcLengths.size() - 1);
        }
    }
    return 1.0f; // If the distance exceeds the total arc length
}

glm::vec3 cFollowCurve::CalculateBezierPoint(float t, const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
{
    float u = 1.0f - t;
    float tt = t * t;
    float uu = u * u;
    float uuu = uu * u;
    float ttt = tt * t;

    glm::vec3 point = uuu * p0;      // (1 - t)^3 * p0
    point += 3 * uu * t * p1;        // 3 * (1 - t)^2 * t * p1
    point += 3 * u * tt * p2;        // 3 * (1 - t) * t^2 * p2
    point += ttt * p3;               // t^3 * p3
    return point;
}

glm::vec3 cFollowCurve::CalculateBezierVelocity(float t, const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
{
    float u = 1.0f - t;

    glm::vec3 velocity = 3 * u * u * (p1 - p0);  // 3 * (1 - t)^2 * (p1 - p0)
    velocity += 6 * u * t * (p2 - p1);          // 6 * (1 - t) * t * (p2 - p1)
    velocity += 3 * t * t * (p3 - p2);          // 3 * t^2 * (p3 - p2)

    return velocity;
}
