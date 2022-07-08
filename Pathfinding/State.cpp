#include "State.h"
#include "Behaviour.h"

namespace AIForGames
{
    State::~State()
    {
        // we own the behaviours assigned to us
        for (Behaviour* b : m_behaviours)
            delete b;

        // we also own the Conditions in each Transition
        // (but the states are references, so don’t clean them up here)
        for (Transition t : m_transitions)
            delete t.condition;
    }

    void State::Update(Agent* agent, float deltaTime)
    {
        for (Behaviour* b : m_behaviours)
            b->Update(agent, deltaTime);
    }
}