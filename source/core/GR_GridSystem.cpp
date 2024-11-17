#include <GR_GridSystem.h>
std::vector<PathFindNode*> levelPathNodes;

std::vector<PathFindNode *> GR_PathfindingSystem::GetNearbyList(PathFindNode *pathNode)
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
std::vector<PathFindNode *> GR_PathfindingSystem::FindPath(PathFindNode *startPos, PathFindNode *endPos)
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

PathFindNode::PathFindNode()
{
    levelPathNodes.push_back(this);
}
