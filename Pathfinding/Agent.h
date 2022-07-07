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
        Agent(NodeMap* _nodeMap, Behaviour* _behaviour) : m_current(_behaviour), m_pathAgent(*_nodeMap), m_nodeMap(_nodeMap), m_color(m_color) {}
        ~Agent() { delete m_current; }
        void GoTo(glm::vec2 point);
        void GoTo(Node* node);
        void Update(float deltaTime);
        void SetNode(Node* node);
        Node* GetNode();
        void SetSpeed(float speed);
        void Draw();
        void DrawFollow();

        // Wandering agent functions
        bool PathComplete();
        void GotToRandom();

        // Follow player functions
        void SetTarget(Agent* agent);
        Agent* GetTarget();
        glm::vec2 GetPosition();
        NodeMap* GetNodeMap();

        //Decsion making
        void Reset();
        void SetColor(Color color);

    private:
        glm::vec2 lastTargetPosition;
        PathAgent m_pathAgent;
        Behaviour* m_current;
        NodeMap* m_nodeMap;
        Color m_color;
        Agent* target;
    };
}
