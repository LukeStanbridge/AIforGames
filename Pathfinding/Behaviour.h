#pragma once
#include "Agent.h"

namespace AIForGames
{
	class Behaviour
	{
		public:
		virtual void Update(Agent* agent, float deltaTime) = 0;
	};

	class GotoPointBehaviour : public Behaviour
	{
	public:
		virtual void Update(Agent* agent, float deltaTime);
	};

	class WanderBehaviour : public Behaviour
	{
	public:
		virtual void Update(Agent* agent, float deltaTime);
	};

	class FollowBehaviour : public Behaviour
	{
	public:
		virtual void Update(Agent* agent, float deltaTime);
	private:
		Vector2 lastTargetPosition;
	};
}

