#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <algorithm>
#include <raylib.h>

using namespace std;

namespace AIForGames
{
    struct Node;

    struct Edge
    {
        Node* target;
        float cost;
        Edge() { target = nullptr; cost = 0; }
        Edge(Node* _node, float _cost) : target(_node), cost(_cost) {}
    };

    struct Node
    {
        Node(float x, float y);
        glm::vec2 position;
        float gScore;
        float hScore;
        float fScore;
        Node* previous;
        vector<Edge> connections;
        void ConnectTo(Node* other, float cost);
    };

    class NodeMap
    {
        int m_width, m_height;
        int m_cellSize;
        Node** m_nodes;

    public:
        NodeMap();
        ~NodeMap();
        Node* GetNode(int x, int y) { return m_nodes[x + m_width * y]; }
        Node* GetRandomNode();
        void Initialise(vector<string> asciiMap, int m_cellSize);
        void Draw();
        void DrawPath(std::vector<Node*> path, Color lineColor, Node* start, Node* end);
        Node* GetClosestNode(glm::vec2 worldPos);
        int GetCellSize();
    };

    class PathAgent
    {
    private:
        glm::vec2 m_position;
        vector<Node*> m_path;
        int m_currentIndex;
        int m_searchType;
        Node* m_currentNode;
        NodeMap& nodeMap;
        float m_speed;
        string m_search;

    public:
        PathAgent(NodeMap& nodeMap);
        PathAgent();
        void Update(float deltaTime);
        void GoToNode(Node* node);
        void Draw(Color colour);
        void DrawFollow(Color m_color);
        void SetNode(Node* node);
        void SetSearchType(int searchType);
        Node* GetNode();
        void SetSpeed(float m_speed);
        vector<Node*> GetPath();
        glm::vec2 GetPosition();
    };

    vector<Node*> DijkstrasSearch(Node* startNode, Node* endNode);
    float Heuristic(Node* target, Node* endNode);
    vector<Node*> AStarSearch(Node* startNode, Node* endNode);
}

