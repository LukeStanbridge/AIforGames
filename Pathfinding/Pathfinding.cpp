#include <iostream>
#include <raylib.h>
#include <set>
#include "Pathfinding.h"
using namespace std;

namespace AIForGames
{
    void Node::ConnectTo(Node* other, float cost)
    {
        connections.push_back(Edge(other, cost));
    }
    
    vector<Node*> NodeMap::DijkstrasSearch(Node* startNode, Node* endNode)
    {
        std::vector<Node*> emptyPath;
        
        //Validate the input
        //if startNode is null OR endNode is null
        // Raise Error
        if (startNode == nullptr || endNode == nullptr) throw runtime_error("Both start and end node must be specified");               

        // if startNode == endNode
        // return empty Path
        if (startNode == endNode)
            return emptyPath;

        // Initialise the starting node
        // Set startNode.gScore to 0
        // Set startNode.parent to null
        startNode->gScore = 0;
        startNode->previous = nullptr;

        // Create our temporary lists for storing nodes we’re visiting/visited
        // Let openList be a List of Nodes
        // Let closedList be a List of Nodes
        set<Node*> closedList;
        vector<Node*> openList;

        //        Add startNode to openList
        openList.push_back(startNode);

        // While openList is not empty    
        while (!openList.empty())
        {
            // Sort openList by Node.gScore
            sort(openList.begin(), openList.end(),
                [](Node* n1, Node* n2)
                {
                    return n1->gScore < n2->gScore;
                });

            // Let currentNode = first item in openList
            Node* currentNode = openList.front();

            // If we visit the endNode, then we can exit early.
            // Sorting the openList above guarantees the shortest path is found,
            // given no negative costs (a prerequisite of the algorithm).
            // This is an optional optimisation that improves performance,
            // but doesn’t always guarantee the shortest path.
            // 
            //        If currentNode is endNode
            //        Exit While Loop
            if (currentNode == endNode) break;

            // Remove currentNode from openList
            // Add currentNode to closedList
            remove(openList.begin(), openList.end(), currentNode);
            closedList.insert(currentNode);

            //For all connections c in currentNode
            for (Edge& c : currentNode->connections)
            {
                // If c.target not in closedList
                // Let gScore = currentNode.gScore + c.cost
                if (closedList.count(c.target) != 0)
                {
                    startNode->gScore = currentNode->gScore + c.cost;

                    // Have not yet visited the node.
                    // So calculate the Score and update its parent.
                    // Also add it to the openList for processing.
                    //If c.target not in openList
                    //Let c.target.gScore = gScore
                    //Let c.target.parent = currentNode
                    //Add c.target to openList
                    if (find(openList.begin(), openList.end(), c.target) != openList.end())
                    {
                        c.target->gScore = currentNode->gScore;
                        c.target->previous = currentNode;
                        openList.push_back(c.target);
                    }

                    // Node is already in the openList with a valid Score.
                    // So compare the calculated Score with the existing
                    // to find the shorter path.
                    //Else if (gScore < c.target.gScore)
                    //Let c.target.gScore = gScore
                    //Let c.target.parent = currentNode
                    else if (currentNode->gScore < c.target->gScore)
                    {
                        c.target->gScore = currentNode->gScore;
                        c.target->previous = currentNode;
                    }
                }             
            }                
        }

        //        // Create Path in reverse from endNode to startNode
        //        Let Path be a list of Nodes
        //        Let currentNode = endNode
        std::vector<Node*> path;
        Node* currentNode = endNode;

        //        While currentNode is not null
        //        Add currentNode to beginning of Path
        //        Let currentNode = currentNode.parent

        while (currentNode != nullptr)
        {
            path.push_back(currentNode);
            currentNode = currentNode->previous;
        }

        std::reverse(path.begin(), path.end());

        // Return the path for navigation between startNode/endNode
        return path;
    }

    NodeMap::~NodeMap()
    {
        for (int y = 0; y < m_height; y++)
        {
            for (int x = 0; x < m_width; x++)
            {
                Node* node = GetNode(x, y);
                if (node) delete node;
            }
        }

        delete[] m_nodes;
    }

    void NodeMap::Initialise(vector<string> asciiMap, int cellSize)
    {
        m_cellSize = cellSize;
        const char emptySquare = '0';

        // assume all strings are the same length, so we'll size the map according to the number of strings and the length of the first one
        m_height = asciiMap.size();
        m_width = asciiMap[0].size();

        m_nodes = new Node * [m_width * m_height];

        // loop over the strings, creating Node entries as we go
        for (int y = 0; y < m_height; y++)
        {
            std::string& line = asciiMap[y];
            // report to the user that you have a mis-matched string length
            if (line.size() != m_width)
                cout << "Mismatched line #" << y << " in ASCII map (" << line.size() << " instead of " << m_width << ")" << std::endl;

            for (int x = 0; x < m_width; x++)
            {
                // get the x-th character, or return an empty node if the string isn't long enough
                char tile = x < line.size() ? line[x] : emptySquare;

                // create a node for anything but a '.' character
                m_nodes[x + m_width * y] = (tile == emptySquare) ? nullptr : new Node(x, y);
            }
        }

        // now loop over the nodes, creating connections between each node and its neighbour to the West and South on the grid. this will link up all nodes
        for (int y = 0; y < m_height; y++)
        {
            for (int x = 0; x < m_width; x++)
            {
                Node* node = GetNode(x, y);
                if (node)
                {
                    // see if there's a node to our west, checking for array overruns first if we're on the west-most edge
                    Node* nodeWest = x == 0 ? nullptr : GetNode(x - 1, y);
                    if (nodeWest)
                    {
                        node->ConnectTo(nodeWest, 1); // TODO weights
                        nodeWest->ConnectTo(node, 1);
                    }

                    // see if there's a node south of us, checking for array index overruns again
                    Node* nodeSouth = y == 0 ? nullptr : GetNode(x, y - 1);
                    if (nodeSouth)
                    {
                        node->ConnectTo(nodeSouth, 1);
                        nodeSouth->ConnectTo(node, 1);
                    }
                }
            }
        }
    }

    void NodeMap::Draw()
    {
        // red color for the blocks
        Color cellColor;
        cellColor.a = 255;
        cellColor.r = 255;
        cellColor.g = 0;
        cellColor.b = 0;
        Color lineColor = BLACK;

        for (int y = 0; y < m_height; y++)
        {
            for (int x = 0; x < m_width; x++)
            {
                Node* node = GetNode(x, y);
                if (node == nullptr)
                {
                    // draw a solid block in empty squares without a navigation node
                    DrawRectangle((int)(x * m_cellSize), (int)(y * m_cellSize), (int)m_cellSize - 1, (int)m_cellSize - 1, cellColor);
                }
                else
                {
                    // draw the connections between the node and its neighbours
                    for (int i = 0; i < node->connections.size(); i++)
                    {
                        Node* other = node->connections[i].target;
                        DrawLine((x + 0.5f) * m_cellSize, (y + 0.5f) * m_cellSize, (other->position.x + 0.5f) * m_cellSize, (other->position.y + 0.5f) * m_cellSize, lineColor);
                    }
                }
            }
        }
    }

    void NodeMap::DrawPath(std::vector<Node*> path, Color lineColor)
    {
        for (int y = 0; y < m_height; y++)
        {
            for (int x = 0; x < m_width; x++)
            {
                Node* node = GetNode(x, y);
                    // draw the connections between the node and its neighbours
                for (int i = 0; i < node->connections.size(); i++)
                {
                    Node* other = node->connections[i].target;
                    DrawLine((x + 0.5f) * m_cellSize, (y + 0.5f) * m_cellSize, (other->position.x + 0.5f) * m_cellSize, (other->position.y + 0.5f) * m_cellSize, lineColor);
                }
            }
        }
    }

    Node* NodeMap::GetClosestNode(glm::vec2 worldPos)
    {
        int i = (int)(worldPos.x / m_cellSize);
        if (i < 0 || i >= m_width) return nullptr;

        int j = (int)(worldPos.y / m_cellSize);
        if (j < 0 || j >= m_width) return nullptr;

        return GetNode(i, j);
    }
}

