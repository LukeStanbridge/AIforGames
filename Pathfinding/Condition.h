#pragma once
#include "Agent.h"

namespace AIForGames
{
	class Condition
	{
	public:
		virtual bool IsTrue(Agent* agent) = 0;
	};

	class DistanceCondition : public Condition
	{
	private:
		float m_distance;
		bool m_lessThan;

	public:
		DistanceCondition(float d, bool lt) : m_distance(d), m_lessThan(lt) {}
		virtual bool IsTrue(Agent* agent);
		void SetColour(Agent* agent);
	};
}


