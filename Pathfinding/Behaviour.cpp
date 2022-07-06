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
        Agent* target = agent->GetTarget();

        float dist = glm::distance(target->GetPosition(), lastTargetPosition);
        if (dist > agent->GetNodeMap()->GetCellSize())
        {
            lastTargetPosition = target->GetPosition();
            agent->GoTo(lastTargetPosition);
        }
    }

}