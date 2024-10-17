#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H
#include <vector>
#include <raylib.h>
#include <mwmath.h>
#include <list>
#include <memory>
#include<array> 
#include <raymath.h>
constexpr size_t MAX_DEPTH = 8;
//Search using the cameraview
//all items returned will run their draw call to show on screen
//load in scene objects into the octree
//get them as gameobjects to render
//tree for ecs also
struct ViewFrustrum
{
    float nearPlane = 1.0f;
    float farPlane = 1000.0f;
    Vector3 corners[8];
    BoundingBox viewFrustumBox;
    ViewFrustrum(){};
    ViewFrustrum(Vector3 cameraPosition, Vector3 cameraDirection, Vector3 cameraRight, Vector3 cameraUp,
         float fov, float nearPlane = 1.0f, float farPlane = 1000.0f)
        {
            corners[0] = Vector3Scale(Vector3Add(cameraPosition, cameraDirection), nearPlane); // near top left
            corners[1] = Vector3Scale(Vector3Add(cameraPosition, Vector3Add(cameraDirection, cameraRight)), nearPlane); // near top right
            corners[2] = Vector3Scale(Vector3Add(cameraPosition, Vector3Add(cameraDirection, cameraUp)), nearPlane); // near bottom left
            corners[3] = Vector3Scale(Vector3Add(cameraPosition, Vector3Add(cameraDirection, Vector3Add(cameraRight, cameraUp))), nearPlane); // near bottom right
            corners[4] = Vector3Scale(Vector3Add(cameraPosition, Vector3Add(cameraDirection, Vector3Add(cameraRight, cameraUp))), farPlane); // far top left
            corners[5] = Vector3Scale(Vector3Add(cameraPosition, Vector3Add(cameraDirection, Vector3Add(cameraRight, cameraUp))), farPlane); // far top right
            corners[6] = Vector3Scale(Vector3Add(cameraPosition, Vector3Add(cameraDirection, cameraUp)), farPlane); // far bottom left
            corners[7] = Vector3Scale(Vector3Add(cameraPosition, Vector3Add(cameraDirection, Vector3Add(cameraRight, cameraUp))), farPlane); // far bottom right

            // Create a bounding box that encloses the view frustum
           viewFrustumBox.min = MW_Math::Min(corners, 8);
           viewFrustumBox.max = MW_Math::Max(corners, 8);
        }
};
struct MightyCam
{
    Camera *camToUse;
    ViewFrustrum frustrum;
    void SetCamPosition(Vector3 newpos)
    {
        camToUse->position = newpos;
        UpdateViewFrustum();
    }
    void UpdateViewFrustum()
    {
            Vector3 camForward = Vector3Normalize(Vector3Subtract(camToUse->target, camToUse->position));
            Vector3 camRight = Vector3CrossProduct(camForward, camToUse->up);
            frustrum.corners[0] = Vector3Scale(Vector3Add(camToUse->position, camForward), frustrum.nearPlane); // near top left
            frustrum.corners[1] = Vector3Scale(Vector3Add(camToUse->position, Vector3Add(camForward, camRight)), frustrum.nearPlane); // near top right
            frustrum.corners[2] = Vector3Scale(Vector3Add(camToUse->position, Vector3Add(camForward, camToUse->up)), frustrum.nearPlane); // near bottom left
            frustrum.corners[3] = Vector3Scale(Vector3Add(camToUse->position, Vector3Add(camForward, Vector3Add(camRight, camToUse->up))), frustrum.nearPlane); // near bottom right
            frustrum.corners[4] = Vector3Scale(Vector3Add(camToUse->position, Vector3Add(camForward, Vector3Add(camRight, camToUse->up))), frustrum.farPlane); // far top left
            frustrum.corners[5] = Vector3Scale(Vector3Add(camToUse->position, Vector3Add(camForward, Vector3Add(camRight, camToUse->up))), frustrum.farPlane); // far top right
            frustrum.corners[6] = Vector3Scale(Vector3Add(camToUse->position, Vector3Add(camForward, camToUse->up)), frustrum.farPlane); // far bottom left
            frustrum.corners[7] = Vector3Scale(Vector3Add(camToUse->position, Vector3Add(camForward, Vector3Add(camRight, camToUse->up))), frustrum.farPlane); // far bottom right

            // Create a bounding box that encloses the view frustum
            frustrum.viewFrustumBox.min = MW_Math::Min(frustrum.corners, 8);
            frustrum.viewFrustumBox.max = MW_Math::Max(frustrum.corners, 8);
         
    }
    MightyCam()
    {
        camToUse = new Camera();
        camToUse->position = (Vector3){ 6.0f, 6.0f, 6.0f };    // mainCamera position
        camToUse->target = (Vector3){ 0.0f, 2.0f, 0.0f };      // mainCamera looking at point
        camToUse->up = (Vector3){ 0.0f, 1.0f, 0.0f };          // mainCamera up vector (rotation towards target)
        camToUse->fovy = 45.0f;                                // mainCamera field-of-view Y
        camToUse->projection = CAMERA_PERSPECTIVE; 
        Vector3 camForward = Vector3Normalize(Vector3Subtract(camToUse->target, camToUse->position));
        Vector3 camRight = Vector3CrossProduct(camForward, camToUse->up);
        frustrum = ViewFrustrum(camToUse->position, camForward, camRight, camToUse->up, camToUse->fovy);
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
            return {&m_pItems, std::prev(m_pItems.end())};
        }
        std::list<T> search(const BoundingBox& rect)
        {
            std::list<T> items;
            search(rect, items);
            return items;
        }
        std::list<T> search(const BoundingBox& rect, std::list<T>& items)
        {
            for(const auto& item : m_pItems)
            {
                if (CheckCollisionBoxes(rect, item))
                {
                    items.push_back(item.second);
                }
            }
            for(int i = 0; i < 4; i++)
            {
                if(m_pChild[i] != nullptr)
                {
                    if (MW_Math::contains<BoundingBox>(rect, m_rChild[i]))
                    {
                        m_pChild[i]->items(items);
                    }
                    else if (MW_Math::overlaps<BoundingBox>(rect, m_rChild[i]))
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
        
        ~Octree();
    protected:
        size_t m_depth;
        //area of the node
        BoundingBox m_rect;
        //child nodes area
        std::array<BoundingBox, 4> m_rChild;
        //potentional items
        std::array<std::shared_ptr<Octree<T>>, 4> m_pChild{};
        //items in the node
        std::vector<std::pair<BoundingBox, T>> m_pItems;
};

template <typename T>
struct OctreeItem
{
    T item;
    OctTreeItemLocation<typename std::list<OctreeItem<T>>::iterator> pItem;
};


template <typename T>
class OctreeContainer
{
    protected:
        std::list<Octree<T>> m_allItems;
        Octree<typename std::list<Octree<T>>::iterator> root;
    public:
        OctreeContainer(const BoundingBox& rect = {0, 0, 0, 100, 100, 100}, size_t depth = 0) : root(rect)
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
        typename std::list<Octree<T>>::iterator begin()
        {
            return m_allItems.begin();
        }
        typename std::list<Octree<T>>::iterator end()
        {
            return m_allItems.end();
        }
        typename std::list<Octree<T>>::const_iterator cbegin() 
        {
            return m_allItems.cbegin();
        }
        typename std::list<Octree<T>>::const_iterator cend()
        {
            return m_allItems.cend();
        }
        /**
         * \brief Returns all items whose bounding box intersects with the given BoundingBox
         *
         * \param rect The BoundingBox to search within
         * \return A list of iterators pointing to the found items
         */
       std::list<typename std::list<Octree<T>>::iterator> search(const BoundingBox& rect) const
       {
            std::list<typename std::list<Octree<T>>::iterator> itemPointers;
            root.search(rect, itemPointers);
            return itemPointers;
       }
       void remove(typename std::list<Octree<T>>::iterator item)
       {
           item->pItem.container->erase(item->pItem.iterator);
            m_allItems.erase(item);
       }
       
};


#endif // RENDER_SYSTEM_H
