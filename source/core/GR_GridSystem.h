#ifndef GR_GRIDSYSTEM_H
#define GR_GRIDSYSTEM_H
#include <raylib.h>
#include <raymath.h>
#include <cstddef>
#include <cmath>
#include <vector>
#include "GameObject.h"
#include <bits/algorithmfwd.h>
#include "../debug/debug.h"
std::vector<PathFindNode*> levelPathNodes;
struct PathFindNode
{
     bool didRemove;
     bool isWalkable;
     GameObject* unit;
     PathFindNode* cameFromNode;
     int gCost;
     int fCost;
     int hCost;
     int x;
     int z;
     int y;
     int index;
    void CalculateFCost()
    {
        fCost = gCost + hCost;
    }
    PathFindNode()
    {
        levelPathNodes.push_back(this);
    }

};
template<typename T>
struct GR_GridSystem
{
    Vector3 position = {0,0,0};
    int width = 0;
    int length = 0;
    int cellSize;
    T* gridArray;
    int ReturnGridWidth()
    {
        return width;
    }
    int ReturnGridLength()
    {
        return length;
    }

    Vector2 GetXZ(Vector3 worldPos)
    {
        return  Vector2{floor((Vector3Subtract(worldPos, position).x / cellSize)), floor((Vector3Subtract(worldPos, position)).z / cellSize)};
    }
    T* GetData(int x, int z)
    {
        if (x >= 0 && x < width && z >= 0 && z < length)
            {
                return gridArray[x, z];
            }

            return new T();
    }
    void SetValue(int x, int z, T value)
    {
        if (x >= 0 && x < width && z >= 0 && z < length)
        {
            gridArray[x, z] = value;

        }
    }
    void SetValue(Vector3 worldPos, T value)
    {
        Vector2 pos = GetXZ(worldPos);
        SetValue(pos.x,pos.y, value);

    }
    Vector3 Between(Vector3 v1, Vector3 v2, float percentage)
    {
        return Vector3Add(v1, Vector3Scale(Vector3Subtract(v2, v1), percentage));
    }
    T* GetCloseNode(Vector3 origin, Vector3 endPos, float distance)
        {
            int distanceToEnd = std::round(Vector3Distance(origin, endPos));
            for (int x = 0; x < distance; x++)
            {
                for (int z = 0; z < distance; z++)
                {
                    if (dynamic_cast<T*>(GetData(std::round(std::abs(endPos.x - x)), std::round(std::abs(endPos.z - z))))->isWalkable)
                    {
                        Vector3 newEnd = GetWorldPosition(std::round(endPos.x), std::round(endPos.z));
                        return GetData(std::round(newEnd.x - x), std::round(newEnd.z - z));
                    }
                }
            }
            return nullptr;
        }
    T* GetData(Vector3 worldPos)
    {
        Vector2 pos = GetXZ(worldPos);
        if (pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < length)
        {
            return gridArray[pos.x, pos.y];
        }
        
            return nullptr;
        
    }
    Vector3 GetWorldPosition(int x, int z)
    {
        float y = 0;
        Vector3 pos = {0,0,0};
        if (static_cast<T*>(GetData(x, z)))
        {
            y = std::round(static_cast<T*>(GetData(x, z))->y);
            pos =  Vector3Add(position, Vector3Scale( Vector3{x, y, z}, cellSize));

        }
        else
        {
            pos =  Vector3Add(position, Vector3Scale( Vector3{x, 0, z}, cellSize));

        }
    }
    GR_GridSystem() {} 
   GR_GridSystem(int w, int l, int cs, Vector3 p ) : width(w), length(l), cellSize(cs), position(p)
    {
        gridArray = new T[w,l];
        for (size_t x = 0; x < width; x++)
        {
            for (size_t z = 0; z < length; z++)
            {
                gridArray[x,z] = T();
                T& node = static_cast<T&>(gridArray[x,z]);
                node->x = x;
                node->z = z;
            }
            
        }
        
    }

};

struct GR_PathfindingSystem
{
    GR_GridSystem<PathFindNode*> grid;
    std::vector<PathFindNode*> openList;
    std::vector<PathFindNode*> closedList;
    std::vector<Vector2> nonWalkableInt;

    float radius, distance;
    int totalNodes;
    int width;
    int length;
    int cells;
    GameObject* origin;
    GR_PathfindingSystem(int w, int l, int cs, Vector3 p) 
    {
        grid = GR_GridSystem<PathFindNode*>(w, l, cs, p);
    }
    bool ReturnWalkData(int x, int z)
    {
        Vector2 tempInt = {x,z};
        for (auto& x : nonWalkableInt)
        {
            if (x.x == tempInt.x && x.y == tempInt.y)
            {
                return false;
            }
        }
        return true;
    }
    bool ReturnIsWalkable(int x, int z)
    {
        for (PathFindNode* node : closedList)
        {
            if (node->x == x && node->z == z)
            {
               
                return false;
            }
        }
        return true;
       
    }
    PathFindNode* GetNodeInPos(int x, int z)
    {
        for (PathFindNode* node : openList)
        {
            if (node->x == x && node->z == z)
            {
                return node;
            }
            
        }

    }
    GameObject* GetNodeObjInPos(int x, int z)
    {
        for (PathFindNode* node : openList)
        {
            if (node->x == x && node->z == z)
            {
                return node->unit;
            }
            
        }
       
    }
    std::vector<PathFindNode*> CalculatePath(PathFindNode* endNode)
    {
        std::vector<PathFindNode*> path;
        path.push_back(endNode);
        PathFindNode* currentNode = endNode;
        while (currentNode->cameFromNode != nullptr)
        {
            path.push_back(currentNode->cameFromNode);
            currentNode = currentNode->cameFromNode;
        }
        std::reverse(path.begin(), path.end());
        return path;
    }
    PathFindNode* GetLowestFCostNode(std::vector<PathFindNode*> pathNodeList)
    {
        PathFindNode* lowestFCostNode = pathNodeList[0];
        for (int i = 0; i < pathNodeList.size(); i++)
        {
            if (pathNodeList[i]->fCost < lowestFCostNode->fCost)
            {
                lowestFCostNode = pathNodeList[i];
            }
        }
        return lowestFCostNode;
    }

    float CaluclateDistance(PathFindNode* startNode, PathFindNode* endNode)
    {
        float xDistance = std::abs(startNode->x - endNode->x);
        float yDistance = std::abs(startNode->z - endNode->z);
        float remaining = std::abs(xDistance - yDistance);

        return 15 * std::min(xDistance, yDistance) + 5.5f * remaining;
    }
    std::vector<PathFindNode*> GetNearbyList( PathFindNode* pathNode)
    {
        std::vector<PathFindNode*> nearbyNodesList;
        float tileDistance = 1.0f; // Assuming a fixed tile distance
        PathFindNode* leftTile = nullptr;
        PathFindNode* rightTile = nullptr;
        PathFindNode* upTile = nullptr;
        PathFindNode* downTile = nullptr;
        PathFindNode* topLeftTile = nullptr;
        PathFindNode* bottomLeftTile = nullptr;
        PathFindNode* topRightTile = nullptr;
        PathFindNode* bottomRightTile = nullptr;

        for (PathFindNode* node : levelPathNodes) {
            if (node->x == pathNode->x - 1 && node->z == pathNode->z) {
                leftTile = node;
            }
            if (node->x == pathNode->x + 1 && node->z == pathNode->z) {
                rightTile = node;
            }
            if (node->z == pathNode->z + 1 && node->x == pathNode->x) {
                upTile = node;
            }
            if (node->z == pathNode->z - 1 && node->x == pathNode->x) {
                downTile = node;
            }
            if (node->z == pathNode->z + 1 && node->x == pathNode->x - 1) {
                topLeftTile = node;
            }
            if (node->z == pathNode->z - 1 && node->x == pathNode->x - 1) {
                bottomLeftTile = node;
            }
            if (node->z == pathNode->z + 1 && node->x == pathNode->x + 1) {
                topRightTile = node;
            }
            if (node->z == pathNode->z - 1 && node->x == pathNode->x + 1) {
                bottomRightTile = node;
            }
        }
        if (leftTile != nullptr) { nearbyNodesList.push_back(leftTile); }
        if (rightTile != nullptr) { nearbyNodesList.push_back(rightTile); }
        if (upTile != nullptr) { nearbyNodesList.push_back(upTile); }
        if (downTile != nullptr) { nearbyNodesList.push_back(downTile); }
        if (topLeftTile != nullptr) { nearbyNodesList.push_back(topLeftTile); }
        if (bottomLeftTile != nullptr) { nearbyNodesList.push_back(bottomLeftTile); }
        if (topRightTile != nullptr) { nearbyNodesList.push_back(topRightTile); }
        if (bottomRightTile != nullptr) { nearbyNodesList.push_back(bottomRightTile); }

        return nearbyNodesList;
    }

    std::vector<PathFindNode*> FindPath(PathFindNode* startPos, PathFindNode* endPos)
    {
        PathFindNode* startNode = startPos;
        PathFindNode* endNode = endPos;
        openList = std::vector<PathFindNode*> { startNode };
        closedList = std::vector<PathFindNode*>();
        for (int i = 0; i < levelPathNodes.size(); i++)
        {
            PathFindNode* pathNode = levelPathNodes[i];
            pathNode->gCost = std::numeric_limits<float>::max();
            pathNode->CalculateFCost();
            pathNode->cameFromNode = nullptr;
        }

        startNode->gCost = 0;
        startNode->hCost = CaluclateDistance(startNode, endNode);
        startNode->CalculateFCost();
        while (openList.size() > 0)
        {
            PathFindNode* currentNode = GetLowestFCostNode(openList);
            if (currentNode == endNode)
            {
                return CalculatePath(endNode);
            }
            openList.erase(std::remove(openList.begin(), openList.end(), currentNode), openList.end());
            closedList.push_back(currentNode);
            for (PathFindNode* nearbyNode : GetNearbyList(currentNode))
            {
                if (std::find(closedList.begin(), closedList.end(), nearbyNode) != closedList.end())
                {
                    continue;
                }
                if (!nearbyNode->isWalkable)
                {
                    closedList.push_back(nearbyNode);
                    continue;
                }

                float tenativeGCost = currentNode->gCost + CaluclateDistance(currentNode, nearbyNode);
                if (tenativeGCost < nearbyNode->gCost)
                {
                    nearbyNode->cameFromNode = currentNode;
                    nearbyNode->gCost = tenativeGCost;
                    nearbyNode->hCost = CaluclateDistance(nearbyNode, endNode);
                    nearbyNode->CalculateFCost();

                    if (std::find(openList.begin(), openList.end(), nearbyNode) == openList.end())
                    {
                        openList.push_back(nearbyNode);

                    }
                }

            }

        }
        debugLog("No Path");

        return std::vector<PathFindNode*>{};

    }
};

#endif // GR_GRIDSYSTEM_H
