#ifndef OBJECTPOOL_H
#define OBJECTPOOL_H

#include <vector>
template <typename T>
struct PoolObject
{
    T* object;
};
template <typename T>
struct ObjectPool
{
    ObjectPool(){};
    ~ObjectPool(){};
    std::vector<PoolObject<T>> pooledObjects;
};
template <typename T>
struct ObjectPoolManager
{
    ObjectPoolManager(int amount, T* object)
    {

        for (int i = 0; i < amount; i++)
        {
            objectPool.pooledObjects.push_back(PoolObject<T>{object});
        }
    };
    ~ObjectPoolManager(){};
    ObjectPool<T> objectPool;
    T* GetObject()
    {
        return objectPool.pooledObjects.back().object;
    }
    void AddObject(T* object)
    {
        objectPool.pooledObjects.push_back(PoolObject<T>{object});
    }

};


#endif // OBJECTPOOL_H
