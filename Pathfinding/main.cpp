#include <iostream>
#include <raylib.h>
#include <glm/glm.hpp>
#include "Pathfinding.h"

using namespace AIForGames;
using namespace std;

int main(int argc, char* argv[])
{
    int screenWidth = 800;
    int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "pathfinding");
    SetTargetFPS(60);

    /*vector<string> asciiMap;
    asciiMap.push_back("000000000000");
    asciiMap.push_back("010111011100");
    asciiMap.push_back("010101110110");
    asciiMap.push_back("010100000000");
    asciiMap.push_back("010111111110");
    asciiMap.push_back("010000001000");
    asciiMap.push_back("011111111110");
    asciiMap.push_back("000000000000");

    NodeMap map;
    map.Initialise(asciiMap, 50);*/

    // create a map of nodes from some grid-based ASCII art
    NodeMap nodeMap;
    vector<std::string> asciiMap;
    asciiMap.push_back("000000000000");
    asciiMap.push_back("010111011100");
    asciiMap.push_back("010101110110");
    asciiMap.push_back("010100000000");
    asciiMap.push_back("010111111110");
    asciiMap.push_back("010000001000");
    asciiMap.push_back("011111111110");
    asciiMap.push_back("000000000000");
    nodeMap.Initialise(asciiMap, 32);

    Node* start = nodeMap.GetNode(1, 1);
    Node* end = nodeMap.GetNode(10, 2);
    vector<Node*> nodeMapPath = nodeMap.DijkstrasSearch(start, end);
    Color lineColor = { 255, 255, 255, 255 };

    if (IsMouseButtonPressed(0))
    {
        Vector2 mousePos = GetMousePosition();
        start = nodeMap.GetClosestNode(glm::vec2(mousePos.x, mousePos.y));
        nodeMapPath = nodeMap.DijkstrasSearch(start, end);
    }

    while (!WindowShouldClose())
    {
        BeginDrawing();

            ClearBackground(BLACK);

            /*map.Draw();*/
            nodeMap.Draw();
            nodeMap.DrawPath(nodeMapPath, lineColor);

            DrawFPS(10, 10);

        EndDrawing();
    }
 
    CloseWindow();

    return 0;
}
