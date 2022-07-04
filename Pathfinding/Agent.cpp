#include "Agent.h"
#include "Behaviour.h"

namespace AIForGames
{
	void Agent::Update(float deltaTime)
	{
		if (m_current)
		{
			m_current->Update(this, deltaTime);
			m_pathAgent.Update(deltaTime);
		}	
	}
	
	void Agent::Draw()
	{
	}

	Agent::Agent()
	{
	}

	void Agent::GoTo(glm::vec2 point)
	{
		Node* end = m_nodeMap->GetClosestNode(point);
		m_pathAgent.GoToNode(end);
	}
}
