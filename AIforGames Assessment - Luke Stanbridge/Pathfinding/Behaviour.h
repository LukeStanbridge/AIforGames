#pragma once
#include "Agent.h"

namespace AIForGames
{
	// abstract base class for all actions
	class Behaviour
	{
		public:
			Behaviour() {}
			~Behaviour() {}
			virtual void Enter(Agent* agent) {}
			virtual void Update(Agent* agent, float deltaTime) = 0;
			virtual void Exit(Agent* agent) {}

			// used by UtilityAI to determine which behaviour to do
			virtual float Evaluate(Agent* agent) { return 0.0f; }
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
		float Evaluate(Agent* agent);
	};

	class FleeBehaviour : public Behaviour
	{
	public:
		void Enter(Agent* agent);
		virtual void Update(Agent* agent, float deltaTime);
		float Evaluate(Agent* agent);
	private:
		glm::vec2 lastPlayerPos;
	};

	class FollowBehaviour : public Behaviour
	{
	public:
		void Enter(Agent* agent);
		virtual void Update(Agent* agent, float deltaTime);
		float Evaluate(Agent* agent);
	private:
		glm::vec2 lastTargetPosition;
	};

	class UtilityAI : public Behaviour
	{
	public:
		~UtilityAI();
		virtual void Update(Agent* agent, float deltaTime);
		void AddBehaviour(Behaviour* behaviour);
	private:
		std::vector<Behaviour*> m_behaviours;
		Behaviour* currentBehaviour;
	};
}

