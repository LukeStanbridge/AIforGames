#pragma once
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

    float WanderBehaviour::Evaluate(Agent* agent)
    {
        Agent* target = agent->GetTarget();
        float dist = glm::distance(target->GetPosition(), agent->GetPosition());

        float eval = dist;
        if (eval < 0)
            eval = 0;
        return eval;
    }

    void FleeBehaviour::Enter(Agent* agent)
    {
        agent->Reset();
    }

    void FleeBehaviour::Update(Agent* agent, float deltaTime)
    {
        agent->SetColor(DARKBROWN);
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
        // eliminates agent stopping when player reaches endNode before flee agent and is still within flee distance
        else if (dist == 0)
        {
            if (agent->PathComplete())
            {
                agent->FleeToRandom(lastPlayerPos);
            }
        }
    }

    float FleeBehaviour::Evaluate(Agent* agent)
    {
        Agent* target = agent->GetTarget();
        float dist = glm::distance(target->GetPosition(), agent->GetPosition());

        float eval = 10 - dist;
        if (eval < 0)
            eval = 0;
        return eval;
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
    float FollowBehaviour::Evaluate(Agent* agent)
    {
        Agent* target = agent->GetTarget();
        float dist = glm::distance(target->GetPosition(), agent->GetPosition());

        float eval = 10 - dist;
        if (eval < 0)
            eval = 0;
        return eval;
    }

    UtilityAI::~UtilityAI()
    {
        for (Behaviour* b : m_behaviours)
            delete b;
    }

    void UtilityAI::Update(Agent* agent, float deltaTime)
    {
        float bestEval = 0;
        Behaviour* newBehaviour = nullptr;
        for (Behaviour* b : m_behaviours)
        {
            float eval = b->Evaluate(agent);
            if (eval > bestEval)
            {
                bestEval = eval;
                newBehaviour = b;
            }
        }

        if (newBehaviour != nullptr
            && newBehaviour != currentBehaviour)
        {
            if (currentBehaviour)
                currentBehaviour->Exit(agent);
            currentBehaviour = newBehaviour;
            currentBehaviour->Enter(agent);
        }

        currentBehaviour->Update(agent, deltaTime);
    }

    void UtilityAI::AddBehaviour(Behaviour* behaviour)
    {
        m_behaviours.push_back(behaviour);
    }
}