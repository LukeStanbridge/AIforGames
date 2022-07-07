#pragma once
#include <vector>
#include "State.h"
#include "Behaviour.h"

using namespace std;

namespace AIForGames
{
    class FiniteStateMachine : public Behaviour
    {
    private:
        vector<State*> m_states;

        State* m_currentState;
        State* m_newState;

    public:
        FiniteStateMachine(State* s) : m_currentState(s), m_newState(nullptr) {}
        virtual ~FiniteStateMachine();

        void Update(Agent* agent, float deltaTime);
    };
}

