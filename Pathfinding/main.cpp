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
    DistanceCondition* furtherThan5 = new DistanceCondition(5.1f, false);
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

    Node* start = nodeMap.GetNode(1, 1); // start node
    Node* end = nodeMap.GetNode(10, 3); // end node

    Agent agent(&nodeMap, new GotoPointBehaviour()); // player controlled agent
    agent.SetNode(start);
    agent.SetSpeed(10);

    Agent agent2(&nodeMap, new WanderBehaviour()); // wander agent and full speed
    agent2.SetNode(nodeMap.GetRandomNode());
    agent2.SetSpeed(10);

    Agent agent3(&nodeMap, fsm); // fsm wander/follow agent
    agent3.SetNode(nodeMap.GetRandomNode());
    agent3.SetTarget(&agent);
    /*agent3.SetSpeed(3);*/

    Agent agent4(&nodeMap, fsm2); // fsm wander/flee agent
    agent4.SetNode(nodeMap.GetRandomNode());
    agent4.SetTarget(&agent);
    /*agent4.SetSpeed(3);*/
    
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

            // Auto wandering agent
            agent2.Update(deltaTime);
            agent2.Draw();

            // Followig agent
            agent3.Update(deltaTime);
            agent3.DrawFollow();

            // Followig agent
            agent4.Update(deltaTime);
            agent4.DrawFollow();

            DrawCircle(70, 525, 8, GREEN);
            DrawText("Dijkstras (lmb)", 100, 500, 50, WHITE);

            DrawCircle(70, 625, 8, MAGENTA);
            DrawText("A-Star (rmb)", 100, 600, 50, WHITE);

            DrawCircle(70, 725, 8, SKYBLUE);
            DrawText("Wander", 100, 700, 50, WHITE);

            DrawCircle(70, 825, 8, ORANGE);
            DrawText("Follower", 100, 800, 50, WHITE);

            DrawCircle(70, 925, 8, BROWN);
            DrawText("Flee", 100, 900, 50, WHITE);

        EndDrawing();
    }
    CloseWindow();
    return 0;
}