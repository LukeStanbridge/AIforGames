#include "Behaviour.h"

namespace AIForGames
{
    void GotoPointBehaviour::Update(Agent* agent, float deltaTime)
    {
        // read mouse clicks
        if (IsMouseButtonPressed(0))
        {
            Vector2 mousePos = GetMousePosition();
            agent->GoTo(glm::vec2(mousePos.x, mousePos.y));
        }
    }

    void WanderBehaviour::Update(Agent* agent, float deltaTime) // Move to random node on map
    {
        if (agent->PathComplete())
        {
            // move to a random node on node map
            agent->GotToRandom();
        }
    }

    void FollowBehaviour::Update(Agent* agent, float deltaTime)
    {
        // check if the agent has moved significantly from its last position
        // if so we want to repath towards it
        Agent* targetAgent = agent->GetTarget();

        float dist = glm::distance(targetAgent->GetPosition(), lastTargetPosition);

        if (dist > 0) // if target position changes by a whole square
        {
            //recalculate path
            lastTargetPosition = targetAgent->GetPosition();
            agent->GoTo(targetAgent->GetNode());
        }
    }

    void SelectorBehaviour::Update(Agent* agent, float deltaTime)
    {
        float dist = glm::distance(agent->GetPosition(), agent->GetTarget()->GetPosition());

        if (dist < 5)
        {
            SetBehaviour(m_b1, agent);
            agent->SetColor(ORANGE);
        }
        else
        {
            SetBehaviour(m_b2, agent);
            agent->SetColor(SKYBLUE);
        }

        m_selected->Update(agent, deltaTime);
    }

    void SelectorBehaviour::SetBehaviour(Behaviour* b, Agent* agent)
    {
        if (m_selected != b)
        {
            m_selected = b;
            agent->Reset();
        }
    }
}