#include "Condition.h"
#include <glm/glm.hpp>
namespace AIForGames
{
	bool DistanceCondition::IsTrue(Agent* agent)
	{
		float dist = glm::distance(agent->GetPosition(), agent->GetTarget()->GetPosition());
		return (dist < m_distance) == m_lessThan;
	}
}

