#ifndef _C_FOLLOW_CURVE_H_
#define _C_FOLLOW_CURVE_H_

#include "iCommand.h"
#include <glm/vec3.hpp>
#include <vector>
#include "cPhysics.h" // Include your physics info structure

class cFollowCurve : public iCommand
{
public:
    void Init(cPhysics::sPhysInfo* pObject,
        glm::vec3 start, glm::vec3 control1, glm::vec3 control2, glm::vec3 end, double duration);

    void OnStart(void);
    void Update(double deltaTime);
    bool isFinished(void);
    void OnFinished(void);

private:
    cPhysics::sPhysInfo* m_pObject;

    glm::vec3 m_start;
    glm::vec3 m_control1;
    glm::vec3 m_control2;
    glm::vec3 m_end;

    double m_duration;
    double m_elapsedTime;

    std::vector<float> m_arcLengths;
    float m_totalArcLength;

    // Helper methods
    std::vector<float> CalculateArcLengths(int steps, const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3);
    float MapDistanceToParameter(float distance, const std::vector<float>& arcLengths);

    glm::vec3 CalculateBezierPoint(float t, const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3);
    glm::vec3 CalculateBezierVelocity(float t, const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3);
};

#endif // _C_FOLLOW_CURVE_H_
