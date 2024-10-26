#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H
#include <vector>
#include <raylib.h>
#include <mwmath.h>
#include <list>
#include <memory>
#include<array> 
#include <raymath.h>
#include<algorithm>
#include "../debug/debug.h"
#include <cfloat>   
#include <PostProcessing.h>
#include <ResourceManager.h>    
#include <rlights.h>
#include "rlgl.h"
#include <GameOverlays.h>
#include <GameObject.h>
constexpr size_t MAX_DEPTH = 8;
//Search using the cameraview
//all items returned will run their draw call to show on screen
//load in scene objects into the octree
//get them as gameobjects to render
//tree for ecs also
struct Plane 
{
    Vector3 point;
    Vector3 normal;
    Vector3 GetNormal() { return normal; }
    Vector3 GetPoint() { return point; }
};
struct Collider
{
    Vector3 *transformedPoints;// Array of points definig the transofrmed collider
    Vector3 *notTransformed;// Array of points defining the collider
    int numPoints;      // Number of points in the array
    Vector3 *normals; //Normal array, helps with detection
    int numNormals; //Number of normals
};

struct MightyBoundingBox 
{
    Color boxColor = RED;
    Vector3 corners[8];//udpate to vector
    int cornersCount = 8;
    Collider collider;
    Vector3 normal = { 0, 0, 0 };
    bool init = false;
    void UpdateCorners(Vector3 parentPos, Vector3 size)
    {
    
        for (int i = 0; i < 8; i++) {
            corners[i] = Vector3{
                parentPos.x + ((i & 1) ? size.x : -size.x),
                parentPos.y + ((i & 2) ? size.y : 0),
                parentPos.z + ((i & 4) ? size.z : -size.z)
            };
        }
    }
  
    Vector3 GetMin()
    {
        Vector3 minVector = corners[0];
        for (int i = 1; i < 8; i++) {
            if (corners[i].x < minVector.x) minVector.x = corners[i].x;
            if (corners[i].y < minVector.y) minVector.y = corners[i].y;
            if (corners[i].z < minVector.z) minVector.z = corners[i].z;
        }
        return minVector;
    }
   
    Vector3 GetMax()
        {
            Vector3 maxVector = corners[0];
            for (int i = 1; i < 8; i++) {
                if (corners[i].x > maxVector.x) maxVector.x = corners[i].x;
                if (corners[i].y > maxVector.y) maxVector.y = corners[i].y;
                if (corners[i].z > maxVector.z) maxVector.z = corners[i].z;
            }
            return maxVector;
        }
    Vector3 GetCenter() {
        Vector3 min = GetMin();
        Vector3 max = GetMax();
        return Vector3Add(Vector3Scale(min, 0.5f), Vector3Scale(max, 0.5f));
    }
    void DrawBoundingBox(Color color) 
    {
        
        // Draw bounding box lines
        DrawLine3D(corners[0], corners[1], color);
        DrawLine3D(corners[1], corners[3], color);
        DrawLine3D(corners[3], corners[2], color);
        DrawLine3D(corners[2], corners[0], color);

        DrawLine3D(corners[4], corners[5], color);
        DrawLine3D(corners[5], corners[7], color);
        DrawLine3D(corners[7], corners[6], color);
        DrawLine3D(corners[6], corners[4], color);

        DrawLine3D(corners[0], corners[4], color);
        DrawLine3D(corners[1], corners[5], color);
        DrawLine3D(corners[2], corners[6], color);
        DrawLine3D(corners[3], corners[7], color);
    }
    BoundingBox GetBoundingBox() 
    {
        Vector3 min = GetMinVector3(corners, 8);
        Vector3 max = GetMaxVector3(corners, 8);
        return {min, max};
    }
    
    void GetBoundingBoxCorners(Matrix rotationMat, Vector3* corners)
    {
        // Calculate the corners of the rotated bounding box
        for (int i = 0; i < 8; i++) 
        {
            Vector3 corner = Vector3Transform(GetCorner(i), rotationMat);
            corners[i] = corner;
        }
 
    }

    Vector3 GetCorner(int index)
    {
        return corners[index];
    }
    Vector3 GetMaxVector3(Vector3* vectors, int count)
    {
        Vector3 maxVector = vectors[0];
        for (int i = 1; i < count; i++) {
            if (vectors[i].x > maxVector.x) maxVector.x = vectors[i].x;
            if (vectors[i].y > maxVector.y) maxVector.y = vectors[i].y;
            if (vectors[i].z > maxVector.z) maxVector.z = vectors[i].z;
        }
        return maxVector;
    }
    Vector3 GetMinVector3(Vector3* vectors, int count) 
    {
        Vector3 minVector = vectors[0];
        for (int i = 1; i < count; i++) {
            if (vectors[i].x < minVector.x) minVector.x = vectors[i].x;
            if (vectors[i].y < minVector.y) minVector.y = vectors[i].y;
            if (vectors[i].z < minVector.z) minVector.z = vectors[i].z;
        }
        return minVector;
    }
    void Rotate(Quaternion *q)
    {
        GetBoundingBoxCorners(QuaternionToMatrix(*q), corners);
    }
    void GetNormals(Mesh mesh,Vector3 * normals) {
        for (int i = 0; i < mesh.vertexCount; i+=3) {//Loop trough the normals and add them to the array
            normals[i/3] = (Vector3){mesh.normals[i],mesh.normals[i+1],mesh.normals[i+2]};
        }
    }

    //Fogot to add a check to check if the mesh inst empty :P
    void SetupColliderMesh(Mesh mesh) {
        init = true;
        collider.numPoints = mesh.vertexCount; //Set the numPoints as num vertices
        collider.notTransformed = (Vector3 *)RL_MALLOC(mesh.vertexCount * sizeof(Vector3));//Init the non transofrmed array
        collider.transformedPoints = (Vector3 *)RL_MALLOC(mesh.vertexCount * sizeof(Vector3));//Init the transofrmed array
        int vertex = 0; //Init vertex counter
        for (int i = 0; i < collider.numPoints; i++) {
            collider.notTransformed[i] = (Vector3){mesh.vertices[vertex], mesh.vertices[vertex + 1], mesh.vertices[vertex + 2]}; // Create the vertex position using mesh data
            vertex += 3; // Add 3 since raylib uses a float array for vertices instead of Vector3, each float is either x, y or z 
        }
    
        /*The tirangle and normal count are the same, for this case, otherwise when using smoothshading
        i recommend to use sizeof function on the normals array*/
        collider.numNormals = mesh.triangleCount; 
        collider.normals = (Vector3 *)RL_MALLOC(mesh.triangleCount * sizeof(Vector3)); //Init normal array
        GetNormals(mesh,*&collider.normals);//Set the normal data
    }


    void GetMinMax(Collider b, Vector3 axis, float *min, float *max) {
        *min = Vector3DotProduct(b.transformedPoints[0], axis); //Initialize min as the first vertex
        *max = *min;  // Initialize max with the same value as min

        for (int i = 1; i < b.numPoints; i++) { //Loop trough the vertices
            //Find the dot product between the vertex and the axis
            float dot = Vector3DotProduct(b.transformedPoints[i], axis);
            //Check if the dot is smaller then min, if it is set the new min
            if (dot < *min) {
                *min = dot;
            }
            //Check if the dot is bigger then max, if it is set the new max
            if (dot > *max) {
                *max = dot;
            }
        }
    }

    Vector3 GetMiddlePoint(Vector3 *vertices,int numVertices){
        float x=0,y=0,z=0;//Init variables
        for(int i = 0; i < numVertices;i++){//Iterate trought the vertices and get the sum
        //Add all the vertex values togerher
        x+= vertices[i].x; 
        y+= vertices[i].y;
        z+= vertices[i].z;
        }
        return(Vector3){x/numVertices,y/numVertices,z/numVertices}; //Divide the values to get the average(Middle)
    } 

    bool CheckCollision(Collider a, Collider b, Vector3 *normal) {
        *normal = (Vector3){0, 0, 0}; //Init normal vector
        float depth = FLT_MAX; //Init depth as the max value it can be

        for (int i = 0; i < a.numNormals; i++) {
            float min1, max1, min2, max2;
        GetMinMax(a, a.normals[i], &min2, &max2);
        GetMinMax(b, a.normals[i], &min1, &max1);

            if (max1 < min2 || max2 < min1) {
                return false;  // No collision on this axis
            } else {
                float axisDepth = fminf(max2 - min1, max1 - min2);
                if (axisDepth < depth) {
                    depth = axisDepth;
                    *normal = a.normals[i];
                }
            }
        }

        for (int i = 0; i < b.numNormals; i++) {
            float min1, max1, min2, max2;
            GetMinMax(a, b.normals[i], &min2, &max2);
            GetMinMax(b, b.normals[i], &min1, &max1);

            if (max1 < min2 || max2 < min1) {
                return false;  // No collision on this axis
            } else {
                float axisDepth = fminf(max2 - min1, max1 - min2);
                if (axisDepth < depth) {
                    depth = axisDepth;
                    *normal = b.normals[i];
                }
            }
        }

        // Simplify direction calculation
        Vector3 direction = Vector3Subtract(GetMiddlePoint(a.transformedPoints, a.numPoints), GetMiddlePoint(b.transformedPoints,b.numPoints));

        if (Vector3DotProduct(direction, *normal) < 0.0f) {
            *normal = Vector3Negate(*normal);
        }
        *normal = Vector3Scale(*normal, depth);
        return true;
    }

    //Not optimal, doesn't include rotation and scaling, it's a simple demo
    void UpdateCollider(Vector3 parent){
        if(!init){
            return;
        }
        //Loop trough the points and add them to the 
        for(int i = 0; i < collider.numPoints;i++){
            collider.transformedPoints[i] = Vector3Add( collider.notTransformed[i],parent);
        }
    }

   
   MightyBoundingBox(BoundingBox boundingBox)
   {
       corners[0] = boundingBox.min;
       corners[1] = Vector3{boundingBox.max.x, boundingBox.min.y, boundingBox.min.z};
       corners[2] = Vector3{boundingBox.max.x, boundingBox.min.y, boundingBox.max.z};
       corners[3] = Vector3{boundingBox.min.x, boundingBox.min.y, boundingBox.max.z};
       corners[4] = Vector3{boundingBox.min.x, boundingBox.max.y, boundingBox.min.z};
       corners[5] = Vector3{boundingBox.max.x, boundingBox.max.y, boundingBox.min.z};
       corners[6] = Vector3{boundingBox.max.x, boundingBox.max.y, boundingBox.max.z};
       corners[7] = boundingBox.max;
   }
   MightyBoundingBox(){};

    void UnloadCollider(Collider *collider)
    {
    free(collider->normals);
    free(collider->transformedPoints);
    free(collider->notTransformed);
}
};
struct Frustum
{
    Plane topFace;
    Plane bottomFace;

    Plane rightFace;
    Plane leftFace;

    Plane farFace;
    Plane nearFace;
    void UpdateFrustumFromCamera(const Camera& cam, float aspect, float fovY,
                                                           float zNear, float zFar)
    {
        Vector3 forward = Vector3Normalize(Vector3Subtract(cam.target, cam.position));
        const float halfVSide = zFar * tanf(fovY * .5f);
        const float halfHSide = halfVSide * aspect;
        const Vector3 frontMultFar = Vector3Scale(forward, zFar);

        //start at camera position, move forward by zNear, then move up 
        nearFace = {Vector3Add(cam.position, Vector3Scale(forward, zNear)), forward};
        farFace = {Vector3Add(cam.position, frontMultFar), Vector3Scale(forward, -1)};

        leftFace = {Vector3Add(cam.position, Vector3Scale(Vector3CrossProduct(forward, (Vector3){0, 1, 0}), halfHSide)), Vector3Normalize(Vector3CrossProduct(forward, (Vector3){0, 1, 0}))};

        rightFace = {Vector3Add(cam.position, Vector3Scale(Vector3CrossProduct(forward, (Vector3){0, 1, 0}), -halfHSide)), Vector3Normalize(Vector3CrossProduct(forward, (Vector3){0, 1, 0}))};

        topFace = {Vector3Add(cam.position, Vector3Scale(Vector3CrossProduct(forward, (Vector3){0, 0, 1}), halfVSide)), Vector3Normalize(Vector3CrossProduct(forward, (Vector3){0, 0, 1}))};

        bottomFace = {Vector3Add(cam.position, Vector3Scale(Vector3CrossProduct(forward, (Vector3){0, 0, 1}), -halfVSide)), Vector3Normalize(Vector3CrossProduct(forward, (Vector3){0, 0, 1}))};
       
    }
    MightyBoundingBox GetFrustumBoundingBox() const {
     MightyBoundingBox boundingBox;
        Vector3 points[8] = {};
        points[0] = Vector3Add(farFace.point, Vector3Scale(farFace.normal, -Vector3Length(farFace.point)));
        points[1] = Vector3Add(farFace.point, Vector3Scale(farFace.normal, Vector3Length(farFace.point)));
        points[2] = Vector3Add(nearFace.point, Vector3Scale(nearFace.normal, -Vector3Length(nearFace.point)));
        points[3] = Vector3Add(nearFace.point, Vector3Scale(nearFace.normal, Vector3Length(nearFace.point)));
        points[4] = Vector3Add(rightFace.point, Vector3Scale(rightFace.normal, -Vector3Length(rightFace.point)));
        points[5] = Vector3Add(rightFace.point, Vector3Scale(rightFace.normal, Vector3Length(rightFace.point)));
        points[6] = Vector3Add(leftFace.point, Vector3Scale(leftFace.normal, -Vector3Length(leftFace.point)));
        points[7] = Vector3Add(leftFace.point, Vector3Scale(leftFace.normal, Vector3Length(leftFace.point)));
        

    
    return boundingBox;
}
  
    
    
};

struct MightyCam
{
    Camera *camToUse;
    Frustum frustum;
    float drawDistance = 100.0f;
    float clippingDistance = 0.5f;
    void SetCamPosition(Vector3 newpos)
    {
        camToUse->position = newpos;
        
    }
    void UpdateCamera()
    {
        UpdateViewFrustum();
    }
    void UpdateViewFrustum()
    {
        frustum.UpdateFrustumFromCamera(*camToUse, (1280/720) ,camToUse->fovy, clippingDistance, drawDistance);
    }
    MightyCam()
    {
        camToUse = new Camera();
        camToUse->position = (Vector3){ 6.0f, 6.0f, 6.0f };    // mainCamera position
        camToUse->target = (Vector3){ 0.0f, 2.0f, 0.0f };      // mainCamera looking at point
        camToUse->up = (Vector3){ 0.0f, 1.0f, 0.0f };          // mainCamera up vector (rotation towards target)
        camToUse->fovy = 90.0f;                                // mainCamera field-of-view Y
        camToUse->projection = CAMERA_PERSPECTIVE; 
        Vector3 camForward = Vector3Normalize(Vector3Subtract(camToUse->target, camToUse->position));
        Vector3 camRight = Vector3CrossProduct(camForward, camToUse->up);
        frustum.UpdateFrustumFromCamera(*camToUse, (1280/720) ,camToUse->fovy, clippingDistance, drawDistance);
    }
    bool IsObjectBehindCamera(MightyBoundingBox box)
    {
        Vector3 camForward = Vector3Normalize(Vector3Subtract(camToUse->target, camToUse->position));

        Vector3 camToCenter = Vector3Subtract(box.GetCenter(), camToUse->position);
        float dotProduct = Vector3DotProduct(camToCenter, camForward);
        return dotProduct < 0;
    }
};

template <typename T>
struct OctTreeItemLocation
{
    typename std::list<std::pair <BoundingBox, T>>* container;
    typename std::list<std::pair <BoundingBox, T>>::iterator iterator;

};


template <typename T>
class Octree
{
    public:
/**
 * Octree constructor.
 * 
 * @param rect The bounding box defining the space of the octree. Default is {0, 0, 0, 100, 100, 100}.
 * @param depth The depth of the octree. Default is 0.
 */
        Octree(const BoundingBox& rect = {0, 0, 0, 100, 100, 100}, size_t depth = 0)
        {
            m_depth = depth;
            resize(rect);
        };
        void resize(const BoundingBox& rect)
        {
            clearTree();
            m_rect = rect;
            Vector3 childSize = {rect.max.x - rect.min.x / 2.0f, rect.max.y - rect.min.y / 2.0f, rect.max.z - rect.min.z / 2.0f};
            m_rChild =
            {
                BoundingBox
                {rect.min.x, rect.min.y, rect.min.z, rect.min.x + childSize.x, rect.min.y + childSize.y, rect.min.z + childSize.z},
                BoundingBox
                {rect.min.x, rect.min.y, rect.max.z - rect.min.z / 2.0f, rect.min.x + childSize.x, rect.min.y + childSize.y, rect.max.z},
                BoundingBox
                {rect.min.x, rect.max.y - rect.min.y / 2.0f, rect.min.z, rect.min.x + childSize.x, rect.max.y, rect.min.z + childSize.z},
                BoundingBox
                {rect.max.x - rect.min.x / 2.0f, rect.min.y, rect.min.z, rect.max.x, rect.min.y + childSize.y, rect.min.z + childSize.z}
            };
        };
        void clearTree()
        {
            m_pItems.clear();
            for (auto& child : m_pChild)
            {
                if (child != nullptr)
                {
                    child->clearTree();
                }
                child.reset();
            }
           
        }
        size_t size() const
        {
            size_t sizeCount = m_pItems.size();
            for (auto& child : m_pChild)
            {
                if (child != nullptr)
                {
                    sizeCount += child->size();
                }
            }
            return sizeCount;
        }
        OctTreeItemLocation<T> insert(const T& item, const BoundingBox& rect)
        {
            for (size_t i = 0; i < 4; i++)
            {
                if (m_depth + 1 < MAX_DEPTH)
                {
                    if (!m_pChild[i])
                    {
                        m_pChild[i] = std::make_shared<Octree<T>>(m_rChild[i], m_depth + 1);
                    }
                    
                   return m_pChild[i]->insert(item, rect);
                    
                }
                
            }
            m_pItems.push_back({rect, item});
            return { &m_pItems, std::prev(m_pItems.end()) };
        }
        std::list<T> search(const BoundingBox& rect)
        {
            std::list<T> items;
            search(rect, items);
            return items;
        }
        std::list<T> search(const BoundingBox& rect, std::list<T>& items) const
        {
            for(const auto& item : m_pItems)
            {
                if (CheckCollisionBoxes(rect, item.first))
                {
                    items.push_back(item.second);
                }
            }
            for(int i = 0; i < 4; i++)
            {
                if(m_pChild[i] != nullptr)
                {
                    if (CheckCollisionBoxes(rect, m_rChild[i]))
                    {
                        m_pChild[i]->items(items);
                    }
                    else if (CheckCollisionBoxes(rect, m_rChild[i]))
                    {
                        m_pChild[i]->search(rect, items);
                    }
                    
                    
                }
            }
        }
        void items(std::list<T>& items)
        {
            for(const auto& item : m_pItems)
            {
                items.push_back(item.second);
            }
            for(int i = 0; i < 4; i++)
            {
                if(m_pChild[i] != nullptr)
                {
                    m_pChild[i]->items(items);
                }
            }
        }
        const BoundingBox& getRect()
        {
            return m_rect;
        }
        bool remove(T item)
        {
            auto it = find(m_pItems.begin(), m_pItems.end(), [item](const std::pair<BoundingBox, T>& p) { return p.second == item; });
            if (it != m_pItems.end())
            {
                m_pItems.erase(it);
                return true;
            }
            else
            {
                for (size_t i = 0; i < 4; i++)
                {
                    if(m_pChild[i] != nullptr)
                    {
                        if (m_pChild[i]->remove(item))
                        {
                            return true;
                        }
                    }
                }
            }   
            return false;
        }
        
        ~Octree(){};
    protected:
        size_t m_depth;
        //area of the node
        BoundingBox m_rect;
        //child nodes area
        std::array<BoundingBox, 4> m_rChild;
        //potentional items
        std::array<std::shared_ptr<Octree<T>>, 4> m_pChild{};
        //items in the node
        std::list<std::pair<BoundingBox, T>> m_pItems;
};

template <typename T>
struct OctreeItem
{
    T item;
    OctTreeItemLocation<typename std::list<OctreeItem<T>>::iterator> pItem;
};

template <typename T>
class DynamicOctreeContainer
{
    using OctreeWrap = std::list<OctreeItem<T>>;
    protected:
        OctreeWrap m_allItems;
        Octree<typename OctreeWrap::iterator> root;
    public:
        /**
         * \brief A container for an octree of items
         * 
         * \param rect The bounding box of the octree
         * \param depth The depth of the octree
         *
         * This class is a container for an octree of items. It provides
         * methods to insert items into the tree, to search for items
         * within a given bounding box, and to remove items from the tree.
         * The tree is represented by a recursive data structure, where
         * each node of the tree is an instance of the Octree class.
         */
        DynamicOctreeContainer(const BoundingBox& rect = {0, 0, 0, 100, 100, 100}, size_t depth = 0) : root(rect)
        {
          
        };
        void resize(const BoundingBox& rect)
        {
            root.resize(rect);
        };
        size_t size() const
        {
            return m_allItems.size();
        }
        bool empty() const
        {
            return m_allItems.empty();
        }
        /**
         * \brief Inserts an item into the octree
         *
         * \param item The item to insert
         * \param rect The bounding box of the item
         */
        void insert(const T& item, const BoundingBox& rect)
        {
            OctreeItem<T> newItem;
            newItem.item = item;
            m_allItems.push_back(newItem);
            m_allItems.back().pItem = root.insert(std::prev(m_allItems.end()), rect);
        }
        void clear()
        {
            root.clearTree();
            m_allItems.clear();
        }
        typename OctreeWrap::iterator begin()
        {
            return m_allItems.begin();
        }
        typename OctreeWrap::iterator end()
        {
            return m_allItems.end();
        }
        typename OctreeWrap::const_iterator cbegin() 
        {
            return m_allItems.cbegin();
        }
        typename OctreeWrap::const_iterator cend()
        {
            return m_allItems.cend();
        }
        /**
         * \brief Returns all items whose bounding box intersects with the given BoundingBox
         *
         * \param rect The BoundingBox to search within
         * \return A list of iterators pointing to the found items
         */
       std::list<typename OctreeWrap::iterator> search(const BoundingBox& rect) const
       {
            std::list<typename OctreeWrap::iterator> itemPointers;
            root.search(rect, itemPointers);
            return itemPointers;
       }
        /**
         * \brief Removes an item from the octree
         *
         * \param item Iterator pointing to the item to remove
         *
         * This function removes the item from the octree and also removes the
         * corresponding iterator from the list of all items. The item is not
         * destroyed, but only removed from the octree.
         */
       void remove(typename OctreeWrap::iterator item)
       {
           item->pItem.container->erase(item->pItem.iterator);
            m_allItems.erase(item);
       }
       
};

template <typename T>
class OctreeContainer
{
    using OctreeWrap = std::list<OctreeItem<T>>;
    protected:
        OctreeWrap m_allItems;
        Octree<typename OctreeWrap::iterator> root;
    public:
        /**
         * \brief A container for an octree of items
         * 
         * \param rect The bounding box of the octree
         * \param depth The depth of the octree
         *
         * This class is a container for an octree of items. It provides
         * methods to insert items into the tree, to search for items
         * within a given bounding box, and to remove items from the tree.
         * The tree is represented by a recursive data structure, where
         * each node of the tree is an instance of the Octree class.
         */
        OctreeContainer(const BoundingBox& rect = {0, 0, 0, 10000, 10000, 10000}, size_t depth = 0) : root(rect)
        {
          
        };
        void resize(const BoundingBox& rect)
        {
            root.resize(rect);
        };
        size_t size() const
        {
            return m_allItems.size();
        }
        bool empty() const
        {
            return m_allItems.empty();
        }
        /**
         * \brief Inserts an item into the octree
         *
         * \param item The item to insert
         * \param rect The bounding box of the item
         */
        void insert(const T& item, const BoundingBox& rect)
        {
            OctreeItem<T> newItem;
            newItem.item = item;
            m_allItems.push_back(newItem);
            m_allItems.back().pItem = root.insert(std::prev(m_allItems.end()), rect);
        }
        void clear()
        {
            root.clearTree();
            m_allItems.clear();
        }
        typename OctreeWrap::iterator begin()
        {
            return m_allItems.begin();
        }
        typename OctreeWrap::iterator end()
        {
            return m_allItems.end();
        }
        typename OctreeWrap::const_iterator cbegin() 
        {
            return m_allItems.cbegin();
        }
        typename OctreeWrap::const_iterator cend()
        {
            return m_allItems.cend();
        }
        /**
         * \brief Returns all items whose bounding box intersects with the given BoundingBox
         *
         * \param rect The BoundingBox to search within
         * \return A list of iterators pointing to the found items
         */
       std::list<typename OctreeWrap::iterator> search(const BoundingBox& rect) const
       {
            std::list<typename OctreeWrap::iterator> listItemPointers;
            root.search(rect, listItemPointers);
            return listItemPointers;
       }
       
};


template <typename OBJECT_TYPE>
class StaticQuadTree
{
public:
	StaticQuadTree(const BoundingBox& rect = {0, 0, 0, 100, 100, 100}, size_t depth = 0)
	{
		m_depth = depth;
		resize(rect);
	}

	// Force area change on Tree, invalidates this and all child layers
	void resize(const BoundingBox& rArea)
	{
		 clear();
            m_rect = rArea;
            Vector3 childSize = {rArea.max.x - rArea.min.x / 2.0f, rArea.max.y - rArea.min.y / 2.0f, rArea.max.z - rArea.min.z / 2.0f};
            m_rChild =
            {
                BoundingBox
                {rArea.min.x, rArea.min.y, rArea.min.z, rArea.min.x + childSize.x, rArea.min.y + childSize.y, rArea.min.z + childSize.z},
                BoundingBox
                {rArea.min.x, rArea.min.y, rArea.max.z - rArea.min.z / 2.0f, rArea.min.x + childSize.x, rArea.min.y + childSize.y, rArea.max.z},
                BoundingBox
                {rArea.min.x, rArea.max.y - rArea.min.y / 2.0f, rArea.min.z, rArea.min.x + childSize.x, rArea.max.y, rArea.min.z + childSize.z},
                BoundingBox
                {rArea.max.x - rArea.min.x / 2.0f, rArea.min.y, rArea.min.z, rArea.max.x, rArea.min.y + childSize.y, rArea.min.z + childSize.z}
            };

	}

	// Clears the contents of this layer, and all child layers
	void clear()
	{
		// Erase any items stored in this layer
		m_pItems.clear();

		// Iterate through children, erase them too
		for (int i = 0; i < 4; i++)
		{
			if (m_pChild[i])
				m_pChild[i]->clear();
			m_pChild[i].reset();
		}
	}

	// Returns a count of how many items are stored in this layer, and all children of this layer
	size_t size() const
	{
		size_t nCount = m_pItems.size();
		for (int i = 0; i < 4; i++)
			if (m_pChild[i]) nCount += m_pChild[i]->size();
		return nCount;
	}

	// Inserts an object into this layer (or appropriate child layer), given the area the item occupies
	void insert(const OBJECT_TYPE& item, const BoundingBox& itemsize)
	{
		// Check each child
		for (int i = 0; i < 4; i++)
		{
			// If the child can wholly contain the item being inserted
			if (MW_Math::contains(m_rChild[i],(itemsize)))
			{
				// Have we reached depth limit?
				if (m_depth + 1 < MAX_DEPTH)
				{
					// No, so does child exist?
					if (!m_pChild[i])
					{
						// No, so create it
						m_pChild[i] = std::make_shared<StaticQuadTree<OBJECT_TYPE>>(m_rChild[i], m_depth + 1);
					}

					// Yes, so add item to it
					m_pChild[i]->insert(item, itemsize);
					return;
				}
			}
		}

		// It didnt fit, so item must belong to this quad
		m_pItems.push_back({ itemsize, item });
	}

	// Returns a list of objects in the given search area
	std::list<OBJECT_TYPE> search(const BoundingBox& rArea) const
	{
		std::list<OBJECT_TYPE> listItems;
		search(rArea, listItems);
		return listItems;
	}

	// Returns the objects in the given search area, by adding to supplied list
	void search(const BoundingBox& rArea, std::list<OBJECT_TYPE>& listItems) const
	{
		// First, check for items belonging to this area, add them to the list
		// if there is overlap
		for (const auto& p : m_pItems)
		{
			if (MW_Math::overlaps(rArea,(p.first)))
				listItems.push_back(p.second);
		}

		// Second, recurse through children and see if they can
		// add to the list
		for (int i = 0; i < 4; i++)
		{
			if (m_pChild[i])
			{
				// If child is entirely contained within area, recursively
				// add all of its children, no need to check boundaries
				if (MW_Math::contains(rArea,(m_rChild[i])))
					m_pChild[i]->items(listItems);

				// If child overlaps with search area then checks need
				// to be made
				else if (MW_Math::overlaps(m_rChild[i],(rArea)))
					m_pChild[i]->search(rArea, listItems);
			}
		}
	}

	void items(std::list<OBJECT_TYPE>& listItems) const
	{
		// No questions asked, just return child items
		for (const auto& p : m_pItems)
			listItems.push_back(p.second);

		// Now add children of this layer's items
		for (int i = 0; i < 4; i++)
			if (m_pChild[i]) m_pChild[i]->items(listItems);
	}


	std::list<OBJECT_TYPE> items() const
	{
		// No questions asked, just return child items
		std::list<OBJECT_TYPE> listItems;
		items(listItems);
		return listItems;
	}

	// Returns area of this layer
	const BoundingBox& area()
	{
		return m_rect;
	}


protected:
	// Depth of this StaticQuadTree layer
	size_t m_depth = 0;

	// Area of this StaticQuadTree
	BoundingBox m_rect;

	// 4 child areas of this StaticQuadTree
	std::array<BoundingBox, 4> m_rChild{};

	// 4 potential children of this StaticQuadTree
	std::array<std::shared_ptr<StaticQuadTree<OBJECT_TYPE>>, 4> m_pChild{};

	// Items which belong to this StaticQuadTree
	std::vector<std::pair<BoundingBox, OBJECT_TYPE>> m_pItems;
};

template <typename OBJECT_TYPE>
class StaticQuadTreeContainer
{
	// Using a std::list as we dont want pointers to be invalidated to objects stored in the
	// tree should the contents of the tree change
	using QuadTreeContainer = std::list<OBJECT_TYPE>;

protected:
	// The actual container
	QuadTreeContainer m_allItems;

	// Use our StaticQuadTree to store "pointers" instead of objects - this reduces
	// overheads when moving or copying objects 
	StaticQuadTree<typename QuadTreeContainer::iterator> root;

public:
	StaticQuadTreeContainer(const BoundingBox& rect = {0, 0, 0, 10000, 10000, 10000}, size_t depth = 0) : root(rect, depth)
	{

	}

	// Sets the spatial coverage area of the quadtree
	// Invalidates tree
	void resize(const BoundingBox& rArea)
	{
		root.resize(rArea);
	}

	// Returns number of items within tree
	size_t size() const
	{
		return m_allItems.size();
	}

	// Returns true if tree is empty
	bool empty() const
	{
		return m_allItems.empty();
	}

	// Removes all items from tree
	void clear()
	{
		root.clear();
		m_allItems.clear();
	}


	// Convenience functions for ranged for loop
	typename QuadTreeContainer::iterator begin()
	{
		return m_allItems.begin();
	}

	typename QuadTreeContainer::iterator end()
	{
		return m_allItems.end();
	}

	typename QuadTreeContainer::const_iterator cbegin()
	{
		return m_allItems.cbegin();
	}

	typename QuadTreeContainer::const_iterator cend()
	{
		return m_allItems.cend();
	}


	// Insert item into tree in specified area
	void insert(const OBJECT_TYPE& item, const BoundingBox itemsize)
	{
		// Item is stored in container
		m_allItems.push_back(item);

		// Pointer/Area of item is stored in quad tree
		root.insert(std::prev(m_allItems.end()), itemsize);
	}

	// Returns a std::list of pointers to items within the search area
	std::list<typename QuadTreeContainer::iterator> search(const BoundingBox& rArea) const
	{
		std::list<typename QuadTreeContainer::iterator> listItemPointers;
		root.search(rArea, listItemPointers);
		return listItemPointers;
	}

};
struct RENDER_PROC//begin shader process //custom func for running in the render loop
{
    void (*draw)();
    RENDER_PROC(void (*draw)()):draw(draw){};
    bool operator==(const RENDER_PROC& other) const
    {
        return draw == other.draw;
    }
};
extern const int screenWidth;
extern const int screenHeight;
extern std::vector<std::string> ShaderPaths;
extern std::map<int, std::pair<std::string, std::string>> RES_Shaders;
struct PostProcessingFXConfig
{
    bool post_processing = true;
    bool bloom = true;
    bool blur = true;
    bool shadows = true;
    bool anti_aliasing = true;
    int shadow_map_size = 1024;

};
//handles all renderable data outside of ecs
struct RenderSystem
{
    bool debugMode;
    MightyCam mainCamera;
    Camera3D lightCam;

    RenderTexture2D post_process_target , shadowMap;
    Shader postProcessingShaders, shadow_shader;
    PostProcessingFXConfig ppfxConfig;
    Shader defaultShader;
    Light defaultLight;
    std::vector<RENDER_PROC> renderProcs;
    int lightVPLoc, shadowMapLoc,lightDirLoc;
    std::vector<Overlay*> overlays;
    std::vector<ObjectDrawable*> drawable2D;
    void DrawOverlays();
    void DrawOverlay(Overlay* overlay);
    void AddOverlay(Overlay* overlay);
    void RemoveOverlay(Overlay* overlay);
    RenderSystem()
    {
       
        if(ppfxConfig.anti_aliasing )
        {
            SetConfigFlags(FLAG_MSAA_4X_HINT);
        }
        Vector3 lightDir = (Vector3){ 0.35f, -1.0f, -0.35f };
        lightCam = (Camera3D){ 0 };
        lightCam.position = Vector3Scale(lightDir, -15.0f);
        lightCam.target = Vector3Zero();
        // Use an orthographic projection for directional lights
        lightCam.projection = CAMERA_ORTHOGRAPHIC;
        lightCam.up = (Vector3){ 0.0f, 1.0f, 0.0f };
        lightCam.fovy = 20.0f;
        defaultShader = LoadShader(RES_Shaders[_RES::ShaderFiles::LIGHT_SHADOW].first.c_str(), RES_Shaders[_RES::ShaderFiles::LIGHT_SHADOW].second.c_str());
        defaultShader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(defaultShader, "viewPos");
        SetLights();
        int ambientLoc = GetShaderLocation(defaultShader, "ambient");
        SetShaderValue(defaultShader, ambientLoc, (float[4]){ 0.12f, 0.12f, 0.12f, 1.0f }, SHADER_UNIFORM_VEC4);
        lightVPLoc = GetShaderLocation(defaultShader, "lightVP");
        shadowMapLoc = GetShaderLocation(defaultShader, "shadowMap");
        int shadowMapResolution = ppfxConfig.shadow_map_size;
        SetShaderValue(defaultShader, GetShaderLocation(defaultShader, "shadowMapResolution"), &shadowMapResolution, SHADER_UNIFORM_INT);

        post_process_target = LoadRenderTexture(screenWidth, screenHeight);
        shadowMap = LoadShadowmapRenderTexture(1024 , 1024);
        postProcessingShaders = LoadShader(0, "romfs:/shaders/ppfx.fs");
        int bloomFX = GetShaderLocation(postProcessingShaders, "bloomFX");
        int blurFX = GetShaderLocation(postProcessingShaders, "blurFX");
        SetShaderValue(postProcessingShaders, bloomFX, (float[1]){ ppfxConfig.bloom ? 1.0f : 0.0f}, SHADER_UNIFORM_FLOAT);//
        SetShaderValue(postProcessingShaders, blurFX, (float[1]){ ppfxConfig.blur ? 1.0f : 0.0f}, SHADER_UNIFORM_FLOAT);

    };
    void SetDefaultShader(Shader shader)
    {
        defaultShader = shader;
    }
    void AddRenderProc(RENDER_PROC *proc)
    {
        renderProcs.push_back(*proc);
    }
    void RemoveRenderProc(RENDER_PROC *proc)
    {
        renderProcs.erase(std::remove(renderProcs.begin(), renderProcs.end(), *proc), renderProcs.end());
    }
    void SetLights()
    {
        defaultLight = CreateLight(LIGHT_POINT, (Vector3){ 0.35f, -1.0f, -0.35f }, Vector3Zero(), DARKGRAY, defaultShader);
        defaultLight.enabled = true;
        Vector4 lightColorNormalized = ColorNormalize(defaultLight.color);
        lightDirLoc = GetShaderLocation(defaultShader, "lightDir");
        int lightColLoc = GetShaderLocation(defaultShader, "lightColor");
        SetShaderValue(defaultShader, lightDirLoc, &defaultLight.position, SHADER_UNIFORM_VEC3);
        SetShaderValue(defaultShader, lightColLoc, &lightColorNormalized, SHADER_UNIFORM_VEC4);
    
    }
    ~RenderSystem()
    {
        UnloadRenderTexture(post_process_target);
        UnloadRenderTexture(shadowMap);
        UnloadShader(postProcessingShaders);
        UnloadShader(defaultShader);
    };
      RenderTexture2D LoadShadowmapRenderTexture(int width, int height)
    {
        RenderTexture2D target = { 0 };

        target.id = rlLoadFramebuffer(); // Load an empty framebuffer
        target.texture.width = width;
        target.texture.height = height;

        if (target.id > 0)
        {
            rlEnableFramebuffer(target.id);

            // Create depth texture
            // We don't need a color texture for the shadowmap
            target.depth.id = rlLoadTextureDepth(width, height, false);
            target.depth.width = width;
            target.depth.height = height;
            target.depth.format = 19;       //DEPTH_COMPONENT_24BIT?
            target.depth.mipmaps = 1;

            // Attach depth texture to FBO
            rlFramebufferAttach(target.id, target.depth.id, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_TEXTURE2D, 0);

            // Check if fbo is complete with attachments (valid)
            if (rlFramebufferComplete(target.id)) debugLog("FBO: [ID %i] Framebuffer object created successfully", target.id);

            rlDisableFramebuffer();
        }
        else debugLog("FBO: Framebuffer object can not be created");

        return target;
    }

    // Unload shadowmap render texture from GPU memory (VRAM)
    void UnloadShadowmapRenderTexture(RenderTexture2D target)
    {
        if (target.id > 0)
        {
            // NOTE: Depth texture/renderbuffer is automatically
            // queried and deleted before deleting framebuffer
            rlUnloadFramebuffer(target.id);
        }
    }
    void RenderScene();
    static RenderSystem& getRenderSystem();


};


#endif // RENDER_SYSTEM_H
