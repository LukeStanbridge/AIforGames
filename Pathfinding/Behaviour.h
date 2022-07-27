#pragma once
#include "Agent.h"

namespace AIForGames
{
	class Behaviour
	{
		public:
			Behaviour() {}
			~Behaviour() {}
			virtual void Enter(Agent* agent) {}
			virtual void Update(Agent* agent, float deltaTime) = 0;
			virtual void Exit(Agent* agent) {}
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

	class FleeBehaviour : public Behaviour
	{
	public:
		void Enter(Agent* agent);
		virtual void Update(Agent* agent, float deltaTime);
	private:
		glm::vec2 lastPlayerPos;
	};

	class FollowBehaviour : public Behaviour
	{
	public:
		void Enter(Agent* agent);
		virtual void Update(Agent* agent, float deltaTime);
	private:
		glm::vec2 lastTargetPosition;
	};

	/*class SelectorBehaviour : public Behaviour
	{
	private:
		Behaviour* m_b1;
		Behaviour* m_b2;
		Behaviour* m_selected;

	public:
		SelectorBehaviour(Behaviour* b1, Behaviour* b2);
		~SelectorBehaviour() { m_b1; m_b2; }
		virtual void Update(Agent* agent, float deltaTime);
		void SetBehaviour(Behaviour* b, Agent* agent);
		
	};*/
}

