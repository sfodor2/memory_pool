/*
 * PooledObject.h
 *
 *  Created on: Dec 11, 2013
 *      Author: silviu
 */

#ifndef POOLEDOBJECT_H_
#define POOLEDOBJECT_H_

#include "MemoryPools.h"

namespace AB
{

template<class Derived>
class PooledObject
{
public:

    PooledObject():m_pool_instance(NULL) {}
    PooledObject(MemoryPools *a_pool_instance):m_pool_instance(a_pool_instance) {}

    virtual ~PooledObject() {}

    //static Derived clsmon;

	void* operator new(size_t obj_size)
	{

		return memory_pools_aux::getPool<Derived>().allocate();
	}

	void* operator new(size_t obj_size, MemoryPools &a_pool_instance)
    {
        return memory_pools_aux::getPool<Derived>(a_pool_instance).allocate();
    }

	void operator delete(void* p)
	{
	    Derived *po = (Derived*)(p);
	    if(po->m_pool_instance)
	    {
	        memory_pools_aux::getPool<Derived>(*po->m_pool_instance).deallocate(p);
	    }
	    else
	        memory_pools_aux::getPool<Derived>().deallocate(p);
	}

	void operator delete(void* p, size_t)
	{
		memory_pools_aux::getPool<Derived>().deallocate(p);
	}


private:
	MemoryPools *m_pool_instance = NULL;
};



} /* namespace AB */
#endif /* POOLEDOBJECT_H_ */
