#pragma once
#include <vector>
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
        void GetTransitions();

    private:
        std::vector<Behaviour*> m_behaviours;
        std::vector<Transition> m_transitions;

    public:
        State();
        ~State();
        virtual void Enter(Agent* agent);
        virtual void Update(Agent* agent, float deltaTime);
        virtual void Exit(Agent* agent);
    };
}

