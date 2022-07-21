#pragma once
#include <iostream>
#include "PathFollower.h"
//#include "raylib.h"

int main(int argc, char* argv[])
{
    int screenWidth = 800;
    int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "pathfinding");
    SetTargetFPS(60);

    // Initialization
    PathFollower follower;

    // create a circular path for the follower, centred to fit on the screen
    float radius = 160;
    for (int i = 0; i < 24; i++)
    {
        float angle = (i * 6.283f) / 24;
        follower.AddPathPoint((1.5f + cosf(angle)) * radius, (1.5f + sinf(angle)) * radius);
    }

    // position the agent
    follower.SetPosition(radius / 2, radius / 2);

    // set acceleration and speed
    follower.SetSpeed(1000, 1100);

    float time = (float)GetTime();
    float deltaTime;

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        float fTime = (float)GetTime();
        deltaTime = fTime - time;
        time = fTime;

        // cap the elapsed time for when we stop and debug
        if (deltaTime > 0.1f)
            deltaTime = 0.1f;

        // Update
        follower.Update(deltaTime);

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);
        follower.Draw();

        EndDrawing();
        //----------------------------------------------------------------------------------
    }
    // De-Initialization
    //--------------------------------------------------------------------------------------  
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    return 0;
}
