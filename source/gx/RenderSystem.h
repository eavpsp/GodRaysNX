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
constexpr size_t MAX_DEPTH = 8;
//Search using the cameraview
//all items returned will run their draw call to show on screen
//load in scene objects into the octree
//get them as gameobjects to render
//tree for ecs also
struct Plane {
    Vector3 point;
    Vector3 normal;
    Vector3 GetNormal() { return normal; }
    Vector3 GetPoint() { return point; }
};
struct MightyBoundingBox {
    Vector3 min; // minimum point of the box (x, y, z)
    Vector3 max; // maximum point of the box (x, y, z)

    void DrawBoundingBox(Color color) 
    {
        DrawCubeWires(GetCenter(), fabs(max.x - min.x), fabs(max.y - min.y), fabs(max.z - min.z), color);
    }
    Vector3 GetCenter() {
        return Vector3Add(min, Vector3Scale(Vector3Subtract(max, min), 0.5f));
    }
    BoundingBox GetBoundingBox() {
        return {min, max};
    }
    MightyBoundingBox(BoundingBox box)
     {
        min = box.min;
        max = box.max;
    }
    MightyBoundingBox();
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
        Vector3 min = {
        std::min(std::min(nearFace.point.x, farFace.point.x), std::min(leftFace.point.x, std::min(rightFace.point.x, std::min(topFace.point.x, bottomFace.point.x)))),
        std::min(std::min(nearFace.point.y, farFace.point.y), std::min(leftFace.point.y, std::min(rightFace.point.y, std::min(topFace.point.y, bottomFace.point.y)))),
        std::min(std::min(nearFace.point.z, farFace.point.z), std::min(leftFace.point.z, std::min(rightFace.point.z, std::min(topFace.point.z, bottomFace.point.z))))
    };

    
   Vector3 max = 
   {
    std::max(std::max(nearFace.point.x, farFace.point.x), std::max(leftFace.point.x, std::max(rightFace.point.x, std::max(topFace.point.x, bottomFace.point.x)))),
    std::max(std::max(nearFace.point.y, farFace.point.y), std::max(leftFace.point.y, std::max(rightFace.point.y, std::max(topFace.point.y, bottomFace.point.y)))),
    std::max(std::max(nearFace.point.z, farFace.point.z), std::max(leftFace.point.z, std::max(rightFace.point.z, std::max(topFace.point.z, bottomFace.point.z))))
    };

    //make sure bounding box is never too thin
    if (max.x - min.x < 3.0f)
    {
        min.x = (max.x + min.x) / 2.0f - 10.0f;
        max.x = (max.x + min.x) / 2.0f + 10.0f;
    }
    if (max.y - min.y < 3.0f)
    {
        min.y = (max.y + min.y) / 2.0f - 10.0f;
        max.y = (max.y + min.y) / 2.0f + 10.0f;
    }
    if (max.z - min.z < 3.0f)
    {
        min.z = (max.z + min.z) / 2.0f - 10.0f;
        max.z = (max.z + min.z) / 2.0f + 10.0f;
    }
    
    return MightyBoundingBox(BoundingBox{min, max});
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

#endif // RENDER_SYSTEM_H
