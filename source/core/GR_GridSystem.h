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
#include <algorithm>
#include <climits>
#include <map>
struct PathFindNode
{
     bool didRemove;
     bool isWalkable = true;
     GameObject* unit;
     PathFindNode* cameFromNode;
     int gCost = 0;
     int fCost = 0;
     int hCost = 0;
     int x;
     int z;
     int y;
     int index;
     bool nulled = false;
    void CalculateFCost()
    {
        fCost = gCost + hCost;
    }
    PathFindNode();
   

};


struct GR_GridSystem
{
    Vector3 position = {0,0,0};
    int width = 0;
    int length = 0;
    int cellSize;
    std::map<Vector2, PathFindNode> gridArray;
    std::vector<PathFindNode*> totalNodes;
    int ReturnGridWidth()
    {
        return width;
    }
    int ReturnGridLength()
    {
        return length;
    }
    int ReturnGridSize()
    {
        return width * length;
    }
    Vector2 GetXZ(Vector3 worldPos)
    {
        return  Vector2{floor((Vector3Subtract(worldPos, position).x / cellSize)), floor((Vector3Subtract(worldPos, position)).z / cellSize)};
    }
    PathFindNode GetData(int x, int z)
    {
        if (x >= 0 && x <= width-1 && z >= 0 && z <= length-1)
        {
            PathFindNode node = gridArray[Vector2{x,z}];
            return gridArray[Vector2{x,z}];
        }
        else
        {
            PathFindNode node;
            node.nulled = true;
            return node;
        }

           
    }
    void SetValue(int x, int z, PathFindNode value)
    {
        if (x >= 0 && x < width && z >= 0 && z < length)
        {
            gridArray[Vector2{x,z}] = value;

        }
    }
    void SetValue(Vector3 worldPos, PathFindNode value)
    {
        Vector2 pos = GetXZ(worldPos);
        SetValue(pos.x,pos.y, value);

    }
    Vector3 Between(Vector3 v1, Vector3 v2, float percentage)
    {
        return Vector3Add(v1, Vector3Scale(Vector3Subtract(v2, v1), percentage));
    }
    PathFindNode GetCloseNode(Vector3 origin, Vector3 endPos, float distance)
    {
        int distanceToEnd = std::round(Vector3Distance(origin, endPos));
        for (int x = 0; x < distance; x++)
        {
            for (int z = 0; z < distance; z++)
            {
                if (GetData(std::round(std::abs(endPos.x - x)), std::round(std::abs(endPos.z - z))).isWalkable)
                {
                    Vector3 newEnd = GetWorldPosition(std::round(endPos.x), std::round(endPos.z));
                    return GetData(std::round(newEnd.x - x), std::round(newEnd.z - z));
                }
            }
        }
            PathFindNode node;
            node.nulled = true;
            return node;
        }
    PathFindNode GetData(Vector3 worldPos)
    {
        Vector2 pos = GetXZ(worldPos);
        if (pos.x >= 0 && pos.x <= width-1 && pos.y >= 0 && pos.y <= length-1)
        {
            int x = pos.x;
            int z = pos.y;
            return gridArray[Vector2{x,z}];
        }
        
            PathFindNode node;
            node.nulled = true;
            return node;
        
    }
    Vector3 GetWorldPosition(int x, int z)
    {
        float y = 0;
        Vector3 pos = {0,0,0};
        if (!(GetData(x, z)).nulled)
        {
            y = std::round(GetData(x, z).y);
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
      int i = 0;
      for (size_t x = 0; x < width; x++)
      {
          for (size_t z = 0; z < length; z++)
          {
                PathFindNode node;
                node.x = x;
                node.z = z;
                gridArray[Vector2{x,z}] = node;
                totalNodes.push_back(&gridArray[Vector2{x,z}]);
                i++;
          }
      }
  }

};

struct GR_PathfindingSystem
{
    GR_GridSystem* grid;
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
        grid = new GR_GridSystem(w, l, cs, p);
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
        if (endNode == nullptr)
        {
            return {};
        }

        std::vector<PathFindNode*> path;
        PathFindNode* currentNode = endNode;
        while (currentNode != nullptr)
        {
            path.push_back(currentNode);
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

    int CalculateDistance(PathFindNode* startNode, PathFindNode* endNode)
    {
        int xDistance = std::abs(startNode->x - endNode->x);
        int zDistance = std::abs(startNode->z - endNode->z);
        return 10 * (xDistance + zDistance); // Manhattan distance (L1 distance)
    }

    std::vector<PathFindNode*> GetNearbyList( PathFindNode* pathNode);

    std::vector<PathFindNode*> FindPath(PathFindNode* startPos, PathFindNode* endPos);

};
#endif // GR_GRIDSYSTEM_H
