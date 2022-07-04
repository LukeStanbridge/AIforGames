#pragma once
#include "Pathfinding.h"
#include <raylib.h>

namespace AIForGames
{
    class Agent
    {
        // abstract base class for all actions
        class Behaviour
        {
        public:
            virtual void Update(Agent* agent, float deltaTime) = 0;
        };

    public:
        Agent();
        Agent(NodeMap* _nodeMap, Behaviour* _behaviour) : m_current(_behaviour), m_nodeMap(_nodeMap), m_color({ 255, 255, 0, 255 }) {}
        ~Agent() { delete m_current; }
        void GoTo(glm::vec2 point);

        void Update(float deltaTime);
        void Draw();

    private:
        PathAgent m_pathAgent;
        Behaviour* m_current;
        NodeMap* m_nodeMap;
        Color m_color;
    };
}
