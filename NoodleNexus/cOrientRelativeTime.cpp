#include "cOrientRelativeTime.h"
#include <iostream>

void cOrientRelativeTime::Init(cPhysics::sPhysInfo* pObject, glm::vec3 rotationXYZ, double timeToMove)
{
	this->m_timeToRotate = timeToMove;
	this->m_pObject = pObject;
	this->m_rotationXYZ = rotationXYZ;
	this->m_elapsedTime = 0.0;

	return;
}
//glm::vec3 deltaRotation = glm::vec3(0.0f);
void cOrientRelativeTime::OnStart(void)
{
	// TODO: 
	this->m_elapsedTime = 0.0;
}

void cOrientRelativeTime::Update(double deltaTime)
{
	// Let the physics thing move the object
	// Keep track of the elapsed time
	this->m_elapsedTime += deltaTime;

	// Calculate the percentage of movement completed (from 0 to 1)
	float t = this->m_elapsedTime / m_timeToRotate;
	t = glm::clamp(t, 0.0f, 1.0f);  // Ensure t is between 0 and 1
	if (t < 0.2f) 
	{
		t *= t;
	}
	else if (t > 0.8f) 
	{
		t *= t;
		t *= t;
	}
	else {
		t = t;
	}

	glm::vec3 startRotation = this->m_pObject->rotation;

	// Interpolate the position from start to target based on time
	glm::vec3 newRotation = glm::mix(startRotation, m_rotationXYZ, t);
	//std::cout << "NewRot : " << newRotation.x << std::endl;

	this->m_pObject->rotation = newRotation;
	this->m_pObject->pAssociatedDrawingMeshInstance->rotationEulerXYZ = this->m_pObject->rotation;

}

bool cOrientRelativeTime::isFinished(void)
{
	// Has the amount of time passed
	if (this->m_elapsedTime >= this->m_timeToRotate)
	{
		// We've arrived
		return true;
	}

	// Keep going...
	return false;
}

void cOrientRelativeTime::OnFinished(void)
{
	return;
}
