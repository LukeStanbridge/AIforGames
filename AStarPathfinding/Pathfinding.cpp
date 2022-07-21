#include <iostream>
#include <raylib.h>
#include <set>
#include "Pathfinding.h"
using namespace std;

namespace AIForGames
{
    Node::Node(float x, float y)
    {
        position.x = x;
        position.y = y;
    }

    void Node::ConnectTo(Node* other, float cost)
    {
        connections.push_back(Edge(other, cost));
    }

#pragma region NodeMap
    NodeMap::NodeMap()
    {
    }

    NodeMap::~NodeMap() //destructor
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

        for (int y = 0; y < m_height; y++) //loop over the strings, creating Node entries as we go
        {
            std::string& line = asciiMap[y];

            if (line.size() != m_width) //report to the user that you have a mis-matched string length
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

                    // diagonals - look to (-1,-1)
                    Node* nodeSouthWest = (x == 0 || y == 0) ? nullptr : GetNode(x - 1, y - 1);
                    if (nodeSouthWest)
                    {
                        node->ConnectTo(nodeSouthWest, 1.414f); // TODO weights
                        nodeSouthWest->ConnectTo(node, 1.414f);
                    }

                    // and (+1, -1)
                    Node* nodeSouthEast = (x == m_width - 1 || y == 0) ? nullptr : GetNode(x + 1, y - 1);
                    if (nodeSouthEast)
                    {
                        node->ConnectTo(nodeSouthEast, 1.414f);
                        nodeSouthEast->ConnectTo(node, 1.414f);
                    }
                }
            }
        }
    }

    void NodeMap::Draw()
    {
        // red color for the blocks
        Color cellColor = PURPLE;
        Color lineColor = WHITE;

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

    void NodeMap::DrawPath(vector<Node*> path, Color lineColor, Node* start, Node* end)
    {
        for (int i = 0; i < path.size() - 1; i++)
        {
            Node* a = path[i];
            Node* b = path[i + 1];
            start = path[0];
            end = path[i + 1];
            DrawLine(((a->position.x + 0.5f) * m_cellSize), ((a->position.y + 0.5f) * m_cellSize), ((b->position.x + 0.5f) * m_cellSize), ((b->position.y + 0.5f) * m_cellSize), lineColor);
        }
        DrawCircle(((start->position.x + 0.5f) * 32), ((start->position.y + 0.5f) * 32), 8, BLUE);
        DrawCircle(((end->position.x + 0.5f) * 32), ((end->position.y + 0.5f) * 32), 8, RED);
    }

    Node* NodeMap::GetRandomNode()
    {
        Node* node = nullptr;
        while (node == nullptr)
        {
            int x = rand() % m_width;
            int y = rand() % m_height;
            node = GetNode(x, y);
        }
        return node;
    }

    Node* NodeMap::GetClosestNode(glm::vec2 worldPos)
    {
        int i = (int)(worldPos.x / m_cellSize);
        if (i < 0 || i >= m_width) return nullptr;

        int j = (int)(worldPos.y / m_cellSize);
        if (j < 0 || j >= m_height) return nullptr;

        return GetNode(i, j);
    }
    int NodeMap::GetCellSize()
    {
        return m_cellSize;
    }
    //bool NodeMap::IsVisibleFrom(Node* start, Node* end)
    //{
    //    //// calculate a vector from start to end that is one cellsize in length
    //    //Vector2 delta = Vector2Subtract(end->position, start->position);
    //    //float distance = Vector2Distance(end->position, start->position);
    //    //delta = Vector2Scale(delta, m_cellSize / distance);

    //    //// step forward in that direction one cell at a time from start towards end
    //    //for (float cells = 1.0f; cells < distance / m_cellSize; cells += 1.0f)
    //    //{
    //    //    Vector2 testPosition = Vector2Add(start->position,
    //    //        Vector2Scale(delta, cells));
    //    //    // if the square below in unpassable, then we don’t have line of sight          
    //    //    // from start to end
    //    //    if (GetClosestNode(testPosition) == nullptr)
    //    //        return false;
    //    //}

    //    //// we've travelled the whole path without hitting an obstacle!
    //    //return true;
    //}
#pragma endregion

#pragma region PathAgent

    PathAgent::PathAgent(NodeMap& nodeMap) : nodeMap(nodeMap)
    {

    }

    PathAgent::PathAgent() : nodeMap(nodeMap)
    {

    }

    void PathAgent::Update(float deltaTime)
    {
        if (m_path.empty()) return;

        float distance = glm::distance(m_position, m_path[m_currentIndex + 1]->position); // calc distance to next node
        glm::vec2 direction = m_path[m_currentIndex + 1]->position - m_position; //calc unit vector to next node
        direction = glm::normalize(direction); // normalize direction

        float distancePerFrame = distance - m_speed * deltaTime; //set the speed per frame

        if (distancePerFrame > 0) //if speed per frame is greater than zero
        {
            m_position += m_speed * deltaTime * direction; // move towards target node
        }
        else //overshot the node
        {
            m_currentNode = m_path[m_currentIndex + 1];//set current node to next node on path
            m_currentIndex++; //increase index

            if (m_currentIndex == m_path.size() - 1) //if reached end of path
            {
                m_position = m_currentNode->position; //snap to final node
                m_path.clear(); //clear contents of the path so future updates do nothing
            }
            else
            {
                float inverseDistance = -distance; //invert distance
                glm::vec2 newDirection = m_path[m_currentIndex - 1]->position - m_position; //calc unit vector from previous node to new next node
                newDirection = glm::normalize(newDirection); //normalize new direction
                m_position += inverseDistance * newDirection; //move along this vector by the overshoot distance from the previous next node.
            }
        }
    }

    void PathAgent::GoToNode(Node* node)
    {
        if (m_searchType == 0)
        {
            m_path = AStarSearch(m_currentNode, node);
            m_currentIndex = 0;
        }
        else
        {
            m_path = DijkstrasSearch(m_currentNode, node);
            m_currentIndex = 0;
        }
    }

    void PathAgent::Draw(Color colour)
    {
        DrawCircle(((m_position.x + 0.5f) * 32), ((m_position.y + 0.5f) * 32), 8, colour); //draw agent
        if (!m_path.empty()) //if there is a path
        {
            nodeMap.DrawPath(m_path, YELLOW, m_currentNode, m_currentNode); //draw path
        }
        string search;

        if (m_searchType == 1) m_search = "Dijkstras";
        else if (m_searchType == 2) m_search = "A Star";
       
        const char* c = m_search.c_str();
        DrawText(c, 700, 700, 50, RED); //draw search type text
    }

    void PathAgent::DrawFollow(Color m_color)
    {
        DrawCircle(((m_position.x + 0.5f) * 32), ((m_position.y + 0.5f) * 32), 12, m_color); //draw agent
    }

    void PathAgent::SetNode(Node* node)
    {
        m_currentNode = node; //set node passed into function as current node
        m_position = node->position; //set position of current node to position of node passed in
    }

    void PathAgent::SetSearchType(int searchType)
    {
        m_searchType = searchType;
    }

    void PathAgent::SetSpeed(float speed)
    {
        m_speed = speed; //set speed
    }

    vector<Node*> PathAgent::GetPath()
    {
        return m_path;
    }

    glm::vec2 PathAgent::GetPosition()
    {
        return m_position;
    }

    Node* PathAgent::GetNode()
    {
        return m_currentNode;
    }

#pragma endregion

    vector<Node*> DijkstrasSearch(Node* startNode, Node* endNode)
    {       
        vector<Node*> emptyPath; // creat empty path to return

        if (startNode == nullptr || endNode == nullptr) throw runtime_error("Both start and end node must be specified"); //if startNode is null OR endNode is null Raise Error

        if (startNode == endNode) return emptyPath; //if startNode == endNode return empty Path

        startNode->gScore = 0; // intialise gScore to 0;
        startNode->previous = nullptr; //set parent to null 

        set<Node*> closedList; //Let closedList be a Set of Nodes
        vector<Node*> openList;  //Let openList be a List of Nodes

        openList.push_back(startNode); //Add startNode to openList

        while (!openList.empty()) //While openList is not empty 
        {
            sort(openList.begin(), openList.end(), //Sort openList by Node.gScore
                [](Node* n1, Node* n2)
                {
                    return n1->gScore > n2->gScore;
                });

            Node* currentNode = openList.back(); //Set currrent node to first item in openList

            if (currentNode == endNode) break; //Exit while loop if currentNOde == endNode

            openList.pop_back(); //Remove currentNode from openList
            closedList.insert(currentNode); //Add currentNode to closedList

            for (Edge& c : currentNode->connections) //For all connections c in currentNode
            {
                if (closedList.count(c.target) == 0) //If c.target not in closedList
                {
                    float gScore = currentNode->gScore + c.cost;  // intitialise and set gScore = currentNode.gScore + c.cost

                    if (find(openList.begin(), openList.end(), c.target) == openList.end()) //If c.target not in openList
                    {
                        c.target->gScore = gScore; //Let c.target.gScore = gScore
                        c.target->previous = currentNode; //Let c.target.parent = currentNode
                        openList.push_back(c.target); //Add c.target to openList
                    }
                    else if (gScore < c.target->gScore) //Else if (gScore < c.target.gScore)
                    {
                        c.target->gScore = gScore; //Let c.target.gScore = gScore
                        c.target->previous = currentNode; //Let c.target.parent = currentNode
                    }
                }
            }
        }
        vector<Node*> path; //Let Path be a list of Nodes
        Node* currentNode = endNode; //Let currentNode = endNode

        while (currentNode != nullptr) //While currentNode is not null
        {
            path.push_back(currentNode); //Add currentNode to beginning of Path
            currentNode = currentNode->previous; //Let currentNode = currentNode.parent
        }

        reverse(path.begin(), path.end()); //Reverse path to correct order
        return path;
    }

    float Heuristic(Node* target, Node* endNode)
    {
        float h = (float)sqrt((target->position.x - endNode->position.x) * (target->position.x - endNode->position.x)
                            + (target->position.y - endNode->position.y) * (target->position.y - endNode->position.y));
        return h;
    }

    vector<Node*> AStarSearch(Node* startNode, Node* endNode)
    {
        // validate input
        vector<Node*> emptyPath;
        if (startNode == nullptr || endNode == nullptr) throw runtime_error("Both start and end node must be specified");
        if (startNode == endNode) return emptyPath;

        // initiialise starting node
        startNode->gScore = 0;
        startNode->previous = nullptr;

        // creat temp lists for storing nodes we're visiting/visited
        set<Node*> closedList;
        vector<Node*> openList; 

        //Add startNode to openList
        openList.push_back(startNode); 

        while (!openList.empty()) //While openList is not empty 
        {
            sort(openList.begin(), openList.end(), //Sort openList by Node.gScore
                [](Node* n1, Node* n2)
                {
                    return n1->fScore > n2->fScore;
                });

            Node* currentNode = openList.back(); //Set currrent node to first item in openList

            if (currentNode == endNode) break; //Exit while loop if currentNode == endNode

            openList.pop_back(); //Remove currentNode from openList
            closedList.insert(currentNode); //Add currentNode to closedList

            for (Edge& c : currentNode->connections) //For all connections c in currentNode
            {
                if (closedList.count(c.target) == 0) //If c.target not in closedList
                {
                    float gScore = currentNode->gScore + c.cost;  // intitialise and set gScore = currentNode.gScore + c.cost
                    float hScore = Heuristic(c.target, endNode);
                    float fScore = gScore + hScore;

                    if (find(openList.begin(), openList.end(), c.target) == openList.end()) //If c.target not in openList
                    {
                        c.target->gScore = gScore; 
                        c.target->fScore = fScore;
                        c.target->previous = currentNode; 
                        openList.push_back(c.target);
                    }
                    else if (fScore < c.target->fScore)
                    {
                        c.target->gScore = gScore;
                        c.target->fScore = fScore;
                        c.target->previous = currentNode; 
                    }
                }
            }
        }
        vector<Node*> path; //Let Path be a list of Nodes
        Node* currentNode = endNode; //Let currentNode = endNode

        while (currentNode != nullptr) //While currentNode is not null
        {
            path.push_back(currentNode); //Add currentNode to beginning of Path
            currentNode = currentNode->previous; //Let currentNode = currentNode.parent
        }

        reverse(path.begin(), path.end()); //Reverse path to correct order
        return path;
    }
}