#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <algorithm>
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
        Node(float x, float y)
        {
            position.x = x;
            position.y = y;
        }

        glm::vec2 position;
        float gScore;
        Node* previous;
        vector<Edge> connections;
        void ConnectTo(Node* other, float cost);
    };

    class NodeMap
    {
        int m_width, m_height;
        int m_cellSize;

        Node** m_nodes;
        /*Node* GetNode(int x, int y) { return m_nodes[x + m_width * y]; }
        vector<Node*> DijkstrasSearch(Node* startNode, Node* endNode);*/


    public:
        ~NodeMap();
        Node* GetNode(int x, int y) { return m_nodes[x + m_width * y]; }
        vector<Node*> DijkstrasSearch(Node* startNode, Node* endNode);
        void Initialise(vector<string> asciiMap, int m_cellSize);
        void Draw();
        void DrawPath(std::vector<Node*> path, Color lineColor);
        Node* GetClosestNode(glm::vec2 worldPos);
    };
}

