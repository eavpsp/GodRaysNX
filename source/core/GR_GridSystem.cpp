#include <GR_GridSystem.h>

std::vector<PathFindNode *> GR_PathfindingSystem::GetNearbyList(PathFindNode *pathNode)
 {
        std::vector<PathFindNode*> nearbyNodesList;
        PathFindNode* leftTile = nullptr;
        PathFindNode* rightTile = nullptr;
        PathFindNode* upTile = nullptr;
        PathFindNode* downTile = nullptr;
        PathFindNode* topLeftTile = nullptr;
        PathFindNode* bottomLeftTile = nullptr;
        PathFindNode* topRightTile = nullptr;
        PathFindNode* bottomRightTile = nullptr;

        for (PathFindNode* node : grid->totalNodes) 
        {
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

std::vector<PathFindNode*> GR_PathfindingSystem::FindPath(PathFindNode* startNode, PathFindNode* endNode)
{
    if (startNode == nullptr || endNode == nullptr)
    {
        return {};
    }

    for (PathFindNode* node : grid->totalNodes)
    {
        if (node != nullptr)
        {
            node->gCost = INT_MAX;
            node->CalculateFCost();
            node->cameFromNode = nullptr;
        }
    }

    startNode->gCost = 0;
    startNode->hCost = CalculateDistance(startNode, endNode);
    startNode->CalculateFCost();
    openList = {startNode};
    closedList = {};

    while (!openList.empty())
    {
        PathFindNode* lowestFCostNode = GetLowestFCostNode(openList);
        if (lowestFCostNode == nullptr)
        {
            return {};
        }

        if (lowestFCostNode->x == endNode->x && lowestFCostNode->z == endNode->z)
        {
            return CalculatePath(lowestFCostNode);
        }

        openList.erase(std::remove(openList.begin(), openList.end(), lowestFCostNode), openList.end());
        closedList.push_back(lowestFCostNode);

        for (PathFindNode* nearbyNode : GetNearbyList(lowestFCostNode))
        {
            if (nearbyNode == nullptr || nearbyNode->isWalkable == false)
            {
                continue;
            }

            bool isInClosedList = false;
            for (const auto& node : closedList)
            {
                if (node->x == nearbyNode->x && node->z == nearbyNode->z)
                {
                    isInClosedList = true;
                    break;
                }
            }

            if (isInClosedList)
            {
                continue;
            }

            int tentativeGCost = lowestFCostNode->gCost + CalculateDistance(lowestFCostNode, nearbyNode);
            if (tentativeGCost < nearbyNode->gCost)
            {
                nearbyNode->gCost = tentativeGCost;
                nearbyNode->hCost = CalculateDistance(nearbyNode, endNode);
                nearbyNode->CalculateFCost();
                nearbyNode->cameFromNode = lowestFCostNode;

                bool isInOpenList = false;
                for (const auto& node : openList)
                {
                    if (node->x == nearbyNode->x && node->z == nearbyNode->z)
                    {
                        isInOpenList = true;
                        break;
                    }
                }

                if (!isInOpenList)
                {
                    openList.push_back(nearbyNode);
                }
            }
        }
    }

    return {};
}

PathFindNode::PathFindNode()
{
 
}



