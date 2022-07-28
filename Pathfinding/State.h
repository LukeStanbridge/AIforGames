#pragma once

#include "Agent.h"
#include "Condition.h"
using namespace std;

namespace AIForGames
{
    class Behaviour;

    class State
    {
    public:
        struct Transition
        {
            Condition* condition;
            State* targetState;
        };

        State(Behaviour* behaviour);
        ~State();
        virtual void Enter(Agent* agent);
        virtual void Update(Agent* agent, float deltaTime);
        virtual void Exit(Agent* agent);
        vector<Transition> GetTransitions();
        void AddTransition(DistanceCondition* condition, State* targetState);
        
    private:
        vector<Behaviour*> m_behaviours;
        vector<Transition> m_transitions;
    };
}

