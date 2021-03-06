#pragma once
#include "Pathfinding.h"
#include <raylib.h>

namespace AIForGames
{
    // abstract base class for all actions
    class Behaviour;
    class Condition;

    class Agent
    {
    public:
        Agent(NodeMap* _nodeMap, Behaviour* _behaviour);
        ~Agent() { delete m_current; }
        void GoTo(glm::vec2 point);
        void GoTo(Node* node);
        void Update(float deltaTime);
        void SetNode(Node* node);
        Node* GetNode();
        void SetSpeed(float speed);
        void SetSearchType(int searchType);
        void Draw();
        void DrawFollow(int circleSize);

        // Wandering agent functions
        bool PathComplete();
        void GotToRandom();
        void FleeToRandom(glm::vec2 playerPos);

        // Follow player functions
        void SetTarget(Agent* agent);
        Agent* GetTarget();
        glm::vec2 GetPosition();
        NodeMap* GetNodeMap();

        //Decsion making
        void Reset();
        void SetColor(Color color);

    private:
        PathAgent m_pathAgent;
        Behaviour* m_current;
        NodeMap* m_nodeMap;
        Color m_color;
        Agent* target;
        int m_searchType;
    };
}
