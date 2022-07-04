#include <iostream>
#include <raylib.h>
#include <glm/glm.hpp>
#include "Pathfinding.h"
#include "GotoPointBehaviour.h"
#include "Agent.h"

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
    asciiMap.push_back("111111111111111111111111111111111111111111111111");
    asciiMap.push_back("111111111111111111111111111111111111111111111111");
    asciiMap.push_back("111111111111111000001111111111100011111111111111");
    asciiMap.push_back("111000000111111111111111111111111000000001111111");
    asciiMap.push_back("111111111111111100000111111111111000001111111111");
    asciiMap.push_back("111111110000111111111111111111111111111111111111");
    asciiMap.push_back("111111111111111111111000000000111111111111011111");
    asciiMap.push_back("111111111111110011111111000000111111110111011111");
    asciiMap.push_back("111000000111110011111111111111111111110111011111");
    asciiMap.push_back("110000001111110000011111110000000111110111011111");
    asciiMap.push_back("111100001111110000011111111000000111111111011111");
    asciiMap.push_back("111111111111111111111111111111111111111111111111");
    asciiMap.push_back("111111111111111111111111111111111111111111111111");
    asciiMap.push_back("111111111111111111111111111111111111111111111111");
    nodeMap.Initialise(asciiMap, 32);

    Node* start = nodeMap.GetNode(1, 1); // start node
    Node* end = nodeMap.GetNode(10, 3); // end node
    vector<Node*> nodeMapPath = DijkstrasSearch(start, end);

    PathAgent agent(nodeMap);
    agent.SetNode(start);
    agent.SetSpeed(5);
    vector<Node*> agentPath = DijkstrasSearch(start, end);
    
    float time = (float)GetTime();
    float deltaTime;
    
    Agent wanderAgent(&nodeMap, new GotoPointBehaviour());

    while (!WindowShouldClose())
    {
        float fTime = (float)GetTime();
        deltaTime = fTime - time;
        time = fTime;

        BeginDrawing();

            ClearBackground(BLACK);

            nodeMap.Draw();



            if (IsMouseButtonPressed(0))
            {
                Vector2 mousePos = GetMousePosition();
                end = nodeMap.GetClosestNode(glm::vec2(mousePos.x, mousePos.y));
                if (end != nullptr)
                {
                    agent.GoToNode(end);
                }
            }
          
            agent.Update(deltaTime);
            agent.Draw();

            DrawFPS(10, 10);

        EndDrawing();
    }
 
    CloseWindow();

    return 0;

}
