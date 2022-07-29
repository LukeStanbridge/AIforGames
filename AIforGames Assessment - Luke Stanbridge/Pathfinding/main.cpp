#pragma once
#include <iostream>
#include <raylib.h>
#include <glm/glm.hpp>
#include "Pathfinding.h"
#include "Behaviour.h"
#include "Agent.h"
#include "Condition.h"
#include "State.h"
#include "FiniteStateMachine.h"

using namespace AIForGames;
using namespace std;

int main(int argc, char* argv[])
{
    int screenWidth = 1600;
    int screenHeight = 1000;

    InitWindow(screenWidth, screenHeight, "pathfinding");
    SetTargetFPS(60);

    // create a map of nodes from some grid-based ASCII art
    NodeMap nodeMap;
    vector<std::string> asciiMap;
    asciiMap.push_back("000000000000000000000000000000000000000000000000");
    asciiMap.push_back("011111111111111111111111111111111111111111111110");
    asciiMap.push_back("011111111111111000001111111111100011111111111110");
    asciiMap.push_back("011000000111111111111111111111111000000001111110");
    asciiMap.push_back("011111111111111100000111111111111000001111111110");
    asciiMap.push_back("011111110000111111111111111111111111111111111110");
    asciiMap.push_back("011111111111111111111000000000111111111111011110");
    asciiMap.push_back("011111111111110011111111000000111111110111011110");
    asciiMap.push_back("011000000111110011111111111111111111110111011110");
    asciiMap.push_back("010000001111110000011111110000000111110111011110");
    asciiMap.push_back("011100001111110000011111111000000111111111011110");
    asciiMap.push_back("011111111111111111111111111111111111111111111110");
    asciiMap.push_back("011111111111111111111111111111111111111111111110");
    asciiMap.push_back("000000000000000000000000000000000000000000000000");
    nodeMap.Initialise(asciiMap, 32);

    // set up a FSM, we're going to have two states with their own conditions
    DistanceCondition* closerThan5 = new DistanceCondition(5.0f, true);
    DistanceCondition* furtherThan5 = new DistanceCondition(5.0f, false);
    DistanceCondition* furtherThan7 = new DistanceCondition(7.0f, false);

    // register these states with the FSM, so its responsible for deleting them now
    State* runWanderState = new State(new WanderBehaviour());
    State* wanderState = new State(new WanderBehaviour());
    State* followState = new State(new FollowBehaviour());
    State* fleeState = new State(new FleeBehaviour());
    
    wanderState->AddTransition(closerThan5, followState);
    followState->AddTransition(furtherThan7, wanderState);

    runWanderState->AddTransition(closerThan5, fleeState);
    fleeState->AddTransition(furtherThan5, runWanderState);

    // make a finite state machine that starts off wandering
    FiniteStateMachine* fsm = new FiniteStateMachine(wanderState);
    fsm->AddState(wanderState);
    fsm->AddState(followState);

    FiniteStateMachine* fsm2 = new FiniteStateMachine(runWanderState);
    fsm2->AddState(runWanderState);
    fsm2->AddState(fleeState);

    UtilityAI* utilityAI = new UtilityAI();
    utilityAI->AddBehaviour(new WanderBehaviour());
    utilityAI->AddBehaviour(new FollowBehaviour());

    UtilityAI* utilityAIFlee = new UtilityAI();
    utilityAIFlee->AddBehaviour(new WanderBehaviour());
    utilityAIFlee->AddBehaviour(new FleeBehaviour());

    Node* start = nodeMap.GetNode(1, 1); // start node
    Node* end = nodeMap.GetNode(10, 3); // end node

    Agent agent(&nodeMap, new GotoPointBehaviour()); // player controlled agent
    agent.SetNode(start);
    agent.SetSpeed(10);

    Agent agent3(&nodeMap, fsm); // fsm wander/follow agent
    agent3.SetNode(nodeMap.GetRandomNode());
    agent3.SetTarget(&agent);

    Agent agent4(&nodeMap, fsm2); // fsm wander/flee agent
    agent4.SetNode(nodeMap.GetRandomNode());
    agent4.SetTarget(&agent);

    Agent agent5(&nodeMap, utilityAI);
    agent5.SetNode(nodeMap.GetRandomNode());
    agent5.SetTarget(&agent);

    Agent agent6(&nodeMap, utilityAIFlee);
    agent6.SetNode(nodeMap.GetRandomNode());
    agent6.SetTarget(&agent);
    
    float time = (float)GetTime();
    float deltaTime;

    while (!WindowShouldClose())
    {
        float fTime = (float)GetTime();
        deltaTime = fTime - time;
        time = fTime;

        BeginDrawing();

            ClearBackground(BLACK);

            nodeMap.Draw();

            // Click and go agent
            agent.Update(deltaTime);
            agent.Draw();

            // Following agent
            agent3.Update(deltaTime);
            agent3.DrawFollow(8);

            // Flee agent
            agent4.Update(deltaTime);
            agent4.DrawFollow(8);

            // Utility agent follow
            agent5.Update(deltaTime);
            agent5.DrawFollow(18);

            // Utility agent flee
            agent6.Update(deltaTime);
            agent6.DrawFollow(18);

            DrawCircle(70, 525, 12, GREEN);
            DrawText("Dijkstras (lmb)", 100, 500, 50, WHITE);

            DrawCircle(70, 625, 12, MAGENTA);
            DrawText("A-Star (rmb)", 100, 600, 50, WHITE);

            DrawCircle(30, 725, 18, SKYBLUE);
            DrawCircle(70, 725, 8, SKYBLUE);
            DrawText("Wander (Bigboi = UtilityAI, Littleboi = FSM)", 100, 700, 50, WHITE);

            DrawCircle(30, 825, 18, ORANGE);
            DrawCircle(70, 825, 8, ORANGE);
            DrawText("Follower", 100, 800, 50, WHITE);

            DrawCircle(30, 925, 18, DARKBROWN);
            DrawCircle(70, 925, 8, DARKBROWN);
            DrawText("Flee", 100, 900, 50, WHITE);

        EndDrawing();
    }
    CloseWindow();
    return 0;
}