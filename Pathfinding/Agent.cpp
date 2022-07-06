#include "Agent.h"
#include "Behaviour.h"
#include <vector>

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

	void Agent::SetNode(Node* node)
	{
		m_pathAgent.SetNode(node);
	}

	void Agent::SetSpeed(float speed)
	{
		m_pathAgent.SetSpeed(speed);
	}
	
	void Agent::Draw()
	{
		m_pathAgent.Draw();
	}

	Agent::Agent()
	{
	}

	// wander functions
	void Agent::GoTo(glm::vec2 point)
	{
		Node* end = m_nodeMap->GetClosestNode(point);
		m_pathAgent.GoToNode(end);
	}

	void Agent::GotToRandom()
	{
		Node* random = m_nodeMap->GetRandomNode();
		m_pathAgent.GoToNode(random);
	}

	bool Agent::PathComplete()
	{
		return m_pathAgent.GetPath().empty();
	}

	// follow funtions
	void Agent::SetTarget(Agent* agent)
	{
	}

	void Agent::GetTarget()
	{
	}

	void Agent::GetPostion()
	{
	}

	void Agent::GetNodeMap()
	{

	}
}
