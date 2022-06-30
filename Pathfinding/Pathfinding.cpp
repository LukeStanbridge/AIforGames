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
        Color lineColor = GREEN;

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
                        DrawLine(((x + 0.5f) * m_cellSize), ((y + 0.5f) * m_cellSize), ((other->position.x + 0.5f) * m_cellSize), ((other->position.y + 0.5f) * m_cellSize), lineColor);
                    }
                }
            }
        }
    }

    void NodeMap::DrawPath(vector<Node*> path, Color lineColor)
    {
        for (int i = 0; i < path.size() - 1; i++)
        {
            Node* a = path[i];
            Node* b = path[i + 1];
            DrawLine(((a->position.x + 0.5f) * m_cellSize), ((a->position.y + 0.5f) * m_cellSize), ((b->position.x + 0.5f) * m_cellSize), ((b->position.y + 0.5f) * m_cellSize), lineColor);
        }
    }

    Node* NodeMap::GetClosestNode(glm::vec2 worldPos)
    {
        int i = (int)(worldPos.x / m_cellSize);
        if (i < 0 || i >= m_width) return nullptr;

        int j = (int)(worldPos.y / m_cellSize);
        if (j < 0 || j >= m_height) return nullptr;

        return GetNode(i, j);
    }

    vector<Node*> DijkstrasSearch(Node* startNode, Node* endNode)
    {
        std::vector<Node*> emptyPath;

        //if startNode is null OR endNode is null
        // Raise Error
        if (startNode == nullptr || endNode == nullptr) throw runtime_error("Both start and end node must be specified");

        // if startNode == endNode
        // return empty Path
        if (startNode == endNode)
            return emptyPath;

        // Set startNode.gScore to 0
        // Set startNode.parent to null
        startNode->gScore = 0;
        startNode->previous = nullptr;

        // Let openList be a List of Nodes
        // Let closedList be a List of Nodes
        set<Node*> closedList;
        vector<Node*> openList;

        // Add startNode to openList
        openList.push_back(startNode);

        // While openList is not empty    
        while (!openList.empty())
        {
            // Sort openList by Node.gScore
            sort(openList.begin(), openList.end(),
                [](Node* n1, Node* n2)
                {
                    return n1->gScore > n2->gScore;
                });

            // Let currentNode = first item in openList
            Node* currentNode = openList.back();

            // If currentNode is endNode
            // Exit While Loop
            if (currentNode == endNode) break;

            // Remove currentNode from openList
            // Add currentNode to closedList
            openList.pop_back();
            closedList.insert(currentNode);

            //For all connections c in currentNode
            for (Edge& c : currentNode->connections)
            {
                // If c.target not in closedList
                if (closedList.count(c.target) == 0)
                {
                    // Let gScore = currentNode.gScore + c.cost
                    float gScore = currentNode->gScore + c.cost;

                    //If c.target not in openList

                    if (find(openList.begin(), openList.end(), c.target) == openList.end())
                    {
                        //Let c.target.gScore = gScore
                        //Let c.target.parent = currentNode
                        //Add c.target to openList
                        c.target->gScore = gScore;
                        c.target->previous = currentNode;
                        openList.push_back(c.target);
                    }

                    //Else if (gScore < c.target.gScore)
                    else if (gScore < c.target->gScore)
                    {
                        //Let c.target.gScore = gScore
                        //Let c.target.parent = currentNode
                        c.target->gScore = gScore;
                        c.target->previous = currentNode;
                    }
                }
            }
        }

        // Let Path be a list of Nodes
        // Let currentNode = endNode
        vector<Node*> path;
        Node* currentNode = endNode;

        // While currentNode is not null
        while (currentNode != nullptr)
        {
            // Add currentNode to beginning of Path
            // Let currentNode = currentNode.parent
            path.push_back(currentNode);
            currentNode = currentNode->previous;
        }

        reverse(path.begin(), path.end());

        // Return the path for navigation between startNode/endNode
        return path;
    }

    void PathAgent::Update(float deltaTime)
    {
        if (m_path.empty()) return;

        // Calculate the distance to the next node and the unit vector to that node
        float distance = glm::distance(m_position, m_path[m_currentIndex + 1]->position);
        glm::vec2 direction = m_path[m_currentIndex + 1]->position - m_position;
        direction = glm::normalize(direction);

        // Subtract speed * deltaTime from the distance (how much we’re going to move this frame)
        distance -= m_speed * deltaTime;

        // If distance is less than zero, then this frame we’re just moving towards the target node. Add speed*deltaTime*unit vector to our position
        if (distance > 0)
        {
            m_position += m_speed * deltaTime * direction;
        }
        // Otherwise, we’ve overshot the node. Add one to currentIndex. 
        else
        {
            m_currentNode = m_path[m_currentIndex + 1];
            m_currentIndex++;

            // If we’ve reached the end of our path, snap to the final node and empty the path so future updates do nothing.
            if (m_currentIndex == m_path.size() - 1)
            {
                
                m_path.clear();
            }
            else
            {
                // If we have a next node, then distance with the subtracted speed*deltaTime tells us how far we’ve overshot the node if we invert it.
                // Find the unit vector from our previous node to the new next node, and move along this vector by the overshoot distance from the previous next node.
                float inverseDistance = -distance;
                glm::vec2 newDirection = m_path[m_currentIndex + 1]->position - m_position;
                newDirection = glm::normalize(newDirection);
                m_position += inverseDistance * newDirection;
            }
        }
    }

    void PathAgent::GoToNode(Node* node)
    {
        m_path = DijkstrasSearch(m_currentNode, node);
        m_currentIndex = 0;
    }


    void PathAgent::Draw()
    {
        DrawCircle((int)((m_position.x + 0.5f) * 32), (int)((m_position.y +0.5f) * 32), 8, { 255,255,0,255 });

        /*float cellSize = m_map.GetCellSize();

        if (m_path.size() > 0)
        {
            m_map.DrawPath(m_path, ORANGE);
        }
        float stretch = 1 + glm::abs(glm::cos(m_step * 200.0f + 50)) * 0.15f;
        float squash = 1 - (stretch) * 0.015f;

        DrawTextPro(m_sprite,
            Rectangle{ 0, 0, (float)m_sprite.width, (float)m_sprite.height },
            Rectangle{ m_position.x, m_position.y, cellSize * squash, cellSize * stretch },
            Vector2{ cellSize * squash * 0.5f, cellSize * stretch * 0.05f },
            0, WHITE);*/
    }

    void PathAgent::SetNode(Node* node)
    {
        m_currentNode = node;
        m_position = node->position;
    }

    void PathAgent::SetSpeed(float speed)
    {
        m_speed = speed;
    }

}

