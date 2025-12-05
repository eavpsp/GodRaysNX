#ifndef OBJECTPOOL_H
#define OBJECTPOOL_H

#include <vector>
#include <map>
#include <raylib.h>
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
    std::vector<PoolObject<T>*> pooledObjects;
};
template <typename T>
struct DeadPoolObject
{
    T* obj;
    float timer;
};
template <typename T>
struct ObjectPoolManager
{
    ObjectPoolManager(int amount, T* object)
    {

        for (int i = 0; i < amount; i++)
        {
            objectPool.pooledObjects.push_back(new PoolObject<T>{object});
        }
    };
    ~ObjectPoolManager(){};
    ObjectPool<T> objectPool;
    float lifeTime = 5;
    std::map<T*, DeadPoolObject<T>*> deadPool;//pool pointer and timer time amount
    void Update()
    {
        for (auto it = deadPool.begin(); it != deadPool.end();)
        {
            it->second->timer += GetFrameTime();
            if (it->second->timer > lifeTime)
            {
                T* object = it->first;
                objectPool.pooledObjects.push_back(PoolObject<T>{object});  
                it = deadPool.erase(it);
            }
            else
            {
                it++;
            }
        }
    }
    T* GetObject()//update to make objects go into a queue
    {
        T* object = objectPool.pooledObjects.back().object;
        objectPool.pooledObjects.pop_back();
        DeadPoolObject<T>* deadObj = new DeadPoolObject<T>{object, 0};
        deadPool[object] = deadObj;
        return object;
    }

    void AddObject(T* object)
    {
        objectPool.pooledObjects.push_back(PoolObject<T>{object});
    }

};


#endif // OBJECTPOOL_H
