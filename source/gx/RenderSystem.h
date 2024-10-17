#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H
#include <vector>
#include <raylib.h>
#include <mwmath.h>
#include <list>
#include <memory>
#include<array> 
constexpr size_t MAX_DEPTH = 8;

template <typename T>
class Octree
{
    public:
        Octree(const Rectangle& rect = {0, 0, 100, 100}, size_t depth = 0)
        {
            m_depth = depth;
            resize(rect);
        };
        void resize(const Rectangle& rect)
        {
            clearTree();
            m_rect = rect;
            Vector2 childSize = {rect.width / 2.0f, rect.height / 2.0f};
            m_rChild =
            {
                Rectangle(m_rect.x, m_rect.y, childSize.x, childSize.y),
                Rectangle(m_rect.x + childSize.x, m_rect.y, childSize.x, childSize.y),  
                Rectangle(m_rect.x, m_rect.y + childSize.y, childSize.x, childSize.y),
                Rectangle(m_rect.x + childSize.x, m_rect.y + childSize.y, childSize.x, childSize.y)
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
        void insert(const T& item, const Rectangle& rect)
        {
            for (size_t i = 0; i < 4; i++)
            {
                if (m_depth + 1 < MAX_DEPTH)
                {
                    if (!m_pChild[i])
                    {
                        m_pChild[i] = std::make_shared<Octree<T>>(m_rChild[i], m_depth + 1);
                    }
                    
                    m_pChild[i]->insert(item, rect);
                    return;
                }
                
            }
            m_pItems.push_back({rect, item});
        }
        std::list<T> search(const Rectangle& rect)
        {
            std::list<T> items;
            search(rect, items);
            return items;
        }
        std::list<T> search(const Rectangle& rect, std::list<T>& items)
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
                    if (MW_Math::contains<Rectangle>(rect, m_rChild[i]))
                    {
                        m_pChild[i]->items(items);
                    }
                    else if (MW_Math::overlaps<Rectangle>(rect, m_rChild[i]))
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
        const Rectangle& getRect()
        {
            return m_rect;
        }
        bool remove(T item)
        {
            auto it = std::find(m_pItems.begin(), m_pItems.end(), [&pItems](const std::pair<Rectangle, T>& p) { return p.second == item; });
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
        Rectangle m_rect;
        //child nodes area
        std::array<Rectangle, 4> m_rChild;
        //potentional items
        std::array<std::shared_ptr<Octree<T>>, 4> m_pChild{};
        //items in the node
        std::vector<std::pair<Rectangle, T>> m_pItems;
};

template <typename T>
class OctreeContainer
{
    protected:
        std::list<Octree<T>> m_allItems;
        Octree<typename std::list<Octree<T>>::iterator> root;
    public:
        OctreeContainer(const Rectangle& rect = {0, 0, 100, 100}, size_t depth = 0) : root(rect)
        {
          
        };
        void resize(const Rectangle& rect)
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
        void insert(const T& item, const Rectangle& rect)
        {
            m_allItems.push_back(item);
            root.insert(std::prev(m_allItems.end()), rect);
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
         * \brief Returns all items whose bounding box intersects with the given rectangle
         *
         * \param rect The rectangle to search within
         * \return A list of iterators pointing to the found items
         */
       std::list<typename std::list<Octree<T>>::iterator> search(const Rectangle& rect) const
       {
            std::list<typename std::list<Octree<T>>::iterator> itemPointers;
            root.search(rect, itemPointers);
            return itemPointers;
       }
       void remove(typename std::list<Octree<T>>::iterator item)
       {
            root.remove(item);
            m_allItems.erase(item);
       }
       
};


#endif // RENDER_SYSTEM_H
