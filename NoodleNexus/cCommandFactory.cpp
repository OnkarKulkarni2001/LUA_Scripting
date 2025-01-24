#include "cCommandFactory.h"

// Abstract factory: includes specific classes here
// NEVER in the header
#include "cMoveRelativeTime.h"
#include "cFollowObject_A.h"
#include "cOrientRelativeTime.h"
#include "cFollowCurve.h"
#include "cFollowObject.h"
#include <iostream>



cCommandFactory::cCommandFactory()
{
	this->m_pPhysicEngine = NULL;
	this->m_vecMeshes = NULL;
}

void cCommandFactory::setPhysics(cPhysics* m_pPhysicEngine)
{
	this->m_pPhysicEngine = m_pPhysicEngine;
	return;
}

// Maybe also the list of meshes?
void cCommandFactory::setVectorOfMeshes(std::vector< sMesh* >* p_vecMeshes)
{
	this->m_vecMeshes = p_vecMeshes;
	return;
}


// If the command is NOT known, will return NULL
iCommand* cCommandFactory::pCreateCommandObject(
	std::string command,
	std::vector< std::string > vecDetails)
{
	// Determine which commands I want
	if (command == "Move Relative ConstVelocity+Time")
	{
		cMoveRelativeTime* pMoveCommand = new cMoveRelativeTime();
		// [0] = friendly name of the object to control
		// [1]-[3] = x,y,z destination of the obejct
		// [4] = time to move 
		// 
		// TODO: Maybe add some error handling if the command is wrong
		// (for now, we are just going to "go for it"!

		cPhysics::sPhysInfo* pMeshToControl = 
			this->m_pPhysicEngine->pFindAssociateMeshByFriendlyName(vecDetails[1]);		// "New_Viper_Player"
		//
		glm::vec3 destinationXYZ = 
			glm::vec3( atof(vecDetails[2].c_str()),
		               atof(vecDetails[3].c_str()),
		               atof(vecDetails[4].c_str()) );
		//		
		float timeToMove = atof(vecDetails[5].c_str());

		// Finally call the Init
		pMoveCommand->Init(pMeshToControl, destinationXYZ, timeToMove);
		return pMoveCommand;
	}
	if (command == "Follow Object, offset & max velocity")
	{
		iCommand* pFollow = new cFollowObject_A();


		return pFollow;
	}
	if (command == "Orient Relative Time")
	{
		cOrientRelativeTime* pOrientCommand = new cOrientRelativeTime();

		cPhysics::sPhysInfo* pMeshToControl =
			this->m_pPhysicEngine->pFindAssociateMeshByFriendlyName(vecDetails[1]);		// "New_Viper_Player"
		//
		glm::vec3 rotationXYZ =
			glm::vec3(atof(vecDetails[2].c_str()),
				atof(vecDetails[3].c_str()),
				atof(vecDetails[4].c_str()));
		//		
		float timeToMove = atof(vecDetails[5].c_str());

		// Finally call the Init
		pOrientCommand->Init(pMeshToControl, rotationXYZ, timeToMove);

		return pOrientCommand;
	}
	if (command == "Follow Curve")
	{
		cFollowCurve* pFollowCurveCommand = new cFollowCurve();
			cPhysics::sPhysInfo * pMeshToControl =
			this->m_pPhysicEngine->pFindAssociateMeshByFriendlyName(vecDetails[1]);		// "New_Viper_Player"
		//
		glm::vec3 startPos =
			glm::vec3(atof(vecDetails[2].c_str()),
				atof(vecDetails[3].c_str()),
				atof(vecDetails[4].c_str()));

		glm::vec3 controlPos1 =
			glm::vec3(atof(vecDetails[5].c_str()),
				atof(vecDetails[6].c_str()),
				atof(vecDetails[7].c_str()));

		glm::vec3 controlPos2 =
			glm::vec3(atof(vecDetails[8].c_str()),
				atof(vecDetails[9].c_str()),
				atof(vecDetails[10].c_str()));

		glm::vec3 endPosition =
			glm::vec3(atof(vecDetails[11].c_str()),
				atof(vecDetails[12].c_str()),
				atof(vecDetails[13].c_str()));

		double duration = atof(vecDetails[14].c_str());

		// Finally call the Init
		pFollowCurveCommand->Init(pMeshToControl, startPos, controlPos1, controlPos2, endPosition, duration);

		return pFollowCurveCommand;
	}
	if (command == "Follow Object")
	{
		cFollowObject* pFollowObjectCommand = new cFollowObject();

		cPhysics::sPhysInfo* pMeshFollower =
			this->m_pPhysicEngine->pFindAssociateMeshByFriendlyName(vecDetails[1]);

		cPhysics::sPhysInfo* pMeshToFollow = 
			this->m_pPhysicEngine->pFindAssociateMeshByFriendlyName(vecDetails[2]);

		glm::vec3 offsetPosition =
			glm::vec3(atof(vecDetails[3].c_str()),
				atof(vecDetails[4].c_str()),
				atof(vecDetails[5].c_str()));

		double duration = atof(vecDetails[6].c_str());

		// Finally call the Init
		pFollowObjectCommand->Init(pMeshFollower, pMeshToFollow, offsetPosition, duration);

		return pFollowObjectCommand;
	}

	// Don't know that command
	return NULL;
}
