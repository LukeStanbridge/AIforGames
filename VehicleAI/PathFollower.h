#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "raylib.h"
using namespace std;

class PathFollower
{
public:
    void Update(float deltaTime);
    glm::vec2 GetClosestPointOnPath(int& segment);
    glm::vec2 GetPointAlongPath(glm::vec2 pos, int segment, float distanceAhead);
    void SetPosition(float x, float y);
    void SetSpeed(float acceleration, float topSpeed);
    void AddPathPoint(float x, float y);
    void Draw();

private:
    vector<glm::vec2> m_path; // array of points describing a closed path
    glm::vec2 m_position;
    glm::vec2 m_velocity;
    float m_speed;
    float m_acceleration;
    glm::vec2 m_debugClosest;
    glm::vec2 m_debugTarget;
};
