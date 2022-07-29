#pragma once
#include "Agent.h"
#include "Behaviour.h"
#include "Condition.h"
#include <vector>

namespace AIForGames
{
	Agent::Agent(NodeMap* _nodeMap, Behaviour* _behaviour) : m_current(_behaviour), m_pathAgent(*_nodeMap), m_nodeMap(_nodeMap), m_color(m_color)
	{
		m_current->Enter(this);
	}

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

	Node* Agent::GetNode()
	{
		return m_pathAgent.GetNode();
	}

	void Agent::SetSpeed(float speed)
	{
		m_pathAgent.SetSpeed(speed);
	}

	void Agent::SetSearchType(int searchType)
	{
		m_searchType = searchType;
	}
	
	void Agent::Draw()
	{
		if (m_searchType == 1)
		{
			m_pathAgent.Draw(GREEN);
		}
		else if (m_searchType == 2)
		{
			m_pathAgent.Draw(MAGENTA);
		}
		else m_pathAgent.Draw(YELLOW);
	}

	void Agent::DrawFollow(int circleSize)
	{
		m_pathAgent.DrawFollow(m_color, circleSize);
	}

	void Agent::GoTo(glm::vec2 point)
	{
		m_pathAgent.SetSearchType(m_searchType);
		Node* end = m_nodeMap->GetClosestNode(point);
		if (end != nullptr)
		{
			m_pathAgent.GoToNode(end);
		}
	}

	void Agent::GoTo(Node* node)
	{
		m_pathAgent.GoToNode(node);
	}

	// wander functions
	void Agent::GotToRandom()
	{
		Node* random = m_nodeMap->GetRandomNode();
		m_pathAgent.GoToNode(random);
	}

	void Agent::FleeToRandom(glm::vec2 playerPos)
	{
		Node* random = m_nodeMap->GetRandomNode();
		float dist = glm::distance(playerPos, random->position);
		while (dist < 5)
		{
			random = m_nodeMap->GetRandomNode();
			dist = glm::distance(playerPos, random->position);
		}
		m_pathAgent.GoToNode(random);
	}

	bool Agent::PathComplete()
	{
		return m_pathAgent.GetPath().empty();
	}

	// follow funtions
	void Agent::SetTarget(Agent* agent) // set target to follow
	{
		// use agent to set to a destination target
		target = agent;
	}

	Agent* Agent::GetTarget()
	{
		// get target from agent
		return target;
	}

	glm::vec2 Agent::GetPosition()
	{
		return m_pathAgent.GetPosition();
	}

	NodeMap* Agent::GetNodeMap()
	{
		return m_nodeMap;
	}
	void Agent::Reset()
	{
		m_pathAgent.GetPath().clear();
	}
	void Agent::SetColor(Color color)
	{
		m_color = color;
	}
}
