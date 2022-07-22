#include "Condition.h"
#include <glm/glm.hpp>
namespace AIForGames
{
	bool DistanceCondition::IsTrue(Agent* agent)
	{
		SetColour(agent);
		float dist = glm::distance(agent->GetPosition(), agent->GetTarget()->GetPosition());
		return (dist < m_distance) == m_lessThan;
	}
	void DistanceCondition::SetColour(Agent* agent)
	{
		if (m_lessThan) agent->SetColor(SKYBLUE);
		else agent->SetColor(ORANGE);
	}
}

