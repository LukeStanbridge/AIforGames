#include "Behaviour.h"

namespace AIForGames
{
    void GotoPointBehaviour::Update(Agent* agent, float deltaTime)
    {
        // read mouse clicks
        if (IsMouseButtonPressed(0)) // rmb for dijkstra
        {
            agent->SetSearchType(1);
            Vector2 mousePos = GetMousePosition();
            agent->GoTo(glm::vec2(mousePos.x, mousePos.y));
        }
        else if (IsMouseButtonPressed(1)) // lmb for A-Star
        {
            agent->SetSearchType(2);
            Vector2 mousePos = GetMousePosition();
            agent->GoTo(glm::vec2(mousePos.x, mousePos.y));
        }
    }

    void WanderBehaviour::Update(Agent* agent, float deltaTime) // Move to random node on map
    {
        agent->SetColor(SKYBLUE);
        agent->SetSpeed(3);
        if (agent->PathComplete())
        {
            // move to a random node on node map
            agent->GotToRandom();
        }
    }

    void FleeBehaviour::Enter(Agent* agent)
    {
        agent->Reset();
    }

    void FleeBehaviour::Update(Agent* agent, float deltaTime)
    {
        agent->SetColor(BROWN);
        agent->SetSpeed(8);
        Agent* playerAgent = agent->GetTarget();
        float dist = glm::distance(playerAgent->GetPosition(), lastPlayerPos);
        if (dist > 0) // if target position changes by a whole square
        {
            //recalculate path
            lastPlayerPos = playerAgent->GetPosition();
            if (agent->PathComplete())
            {
                // move to a random node on node map
                agent->FleeToRandom(lastPlayerPos);
            }
        }
    }

    void FollowBehaviour::Enter(Agent* agent)
    {
        agent->Reset();
    }

    void FollowBehaviour::Update(Agent* agent, float deltaTime)
    {
        agent->SetColor(ORANGE);
        agent->SetSpeed(5);
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
}