#pragma once
#include "Pathfinding.h"
#include <raylib.h>

namespace AIForGames
{
    // abstract base class for all actions
    class Behaviour;

    class Agent
    {
    public:
        Agent();
        Agent(NodeMap* _nodeMap, Behaviour* _behaviour) : m_current(_behaviour), m_pathAgent(*_nodeMap), m_nodeMap(_nodeMap), m_color({255, 255, 0, 255}) {}
        ~Agent() { delete m_current; }
        void GoTo(glm::vec2 point);
        void Update(float deltaTime);
        void SetNode(Node* node);
        void SetSpeed(float speed);
        void Draw();

        // Wandering agent functions
        bool PathComplete();
        void GotToRandom();

        // Follow player functions
        void SetTarget(Agent* agent);
        void GetTarget();
        void GetPostion();
        void GetNodeMap();

    private:
        PathAgent m_pathAgent;
        Behaviour* m_current;
        NodeMap* m_nodeMap;
        Color m_color;
    };
}
