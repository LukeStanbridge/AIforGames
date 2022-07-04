#pragma once
#include "Agent.h"

namespace AIForGames
{
	class Behaviour
	{
		public:
		virtual void Update(Agent* agent, float deltaTime) = 0;
	};
}

