/*
 * MemoryPools.h
 *
 *  Created on: Dec 11, 2013
 *      Author: silviu
 */

#pragma once

#include "MemoryPool.h"
#include "Definition/ABException.h"
#include "Definition/Types.h"

namespace admin
{
	template<class T>
	class NavStdVector;
}


namespace AB
{

template <typename T>
struct variable_size_object{
	T t;
};


template<int N>
struct _{ operator char() { return N + 256; } };

class MemoryPools
    : public admin::ObjNode<MemoryPools>
{
public:
    typedef typename PtrType< MemoryPools >::type Ptr;
	static MemoryPools* instance;
	MemoryPool
		          pool_32
		        , pool_64
		        , pool_96
		        , pool_128
		        , pool_192
		        , pool_256
		        , pool_384
		        , pool_512
		        , pool_640
		        , pool_768
		        , pool_896
		        , pool_1024
		        ;

	admin::NavStdVector<MemoryPool>* pools;


	static MemoryPools& get()
	{
		if (!instance)
		{
			LOG(FATAL)<<"Initialize first!!!";
		}
		return *instance;
	}


	MemoryPools( const long factor, bool a_non_singleton = false );

	~MemoryPools(){}

	template< class T >
	void reflectObject( T& f )
	{
	   // pools->nodeChildren(f);
	}


	void dump( std::stringstream& out );

	void dump();

//
//    bool onAdmin( admin::AdminCtx& adm );
//	void admin_dump( admin::AdminCtx& adm );
};

namespace memory_pools_aux {

template<int X> struct MemoryPoolSelector {};
inline MemoryPool& get(MemoryPoolSelector<12>) { return MemoryPools::instance->pool_32; }
inline MemoryPool& get(MemoryPoolSelector<11>) { return MemoryPools::instance->pool_64; }
inline MemoryPool& get(MemoryPoolSelector<10>) { return MemoryPools::instance->pool_96; }
inline MemoryPool& get(MemoryPoolSelector<9>) { return MemoryPools::instance->pool_128; }
inline MemoryPool& get(MemoryPoolSelector<8>) { return MemoryPools::instance->pool_192; }
inline MemoryPool& get(MemoryPoolSelector<7>) { return MemoryPools::instance->pool_256; }
inline MemoryPool& get(MemoryPoolSelector<6>) { return MemoryPools::instance->pool_384; }
inline MemoryPool& get(MemoryPoolSelector<5>) { return MemoryPools::instance->pool_512; }
inline MemoryPool& get(MemoryPoolSelector<4>) { return MemoryPools::instance->pool_640; }
inline MemoryPool& get(MemoryPoolSelector<3>) { return MemoryPools::instance->pool_768; }
inline MemoryPool& get(MemoryPoolSelector<2>) { return MemoryPools::instance->pool_896; }
inline MemoryPool& get(MemoryPoolSelector<1>) { return MemoryPools::instance->pool_1024; }


inline MemoryPool& get(MemoryPoolSelector<12>, MemoryPools &a_pool_instance) { return a_pool_instance.pool_32; }
inline MemoryPool& get(MemoryPoolSelector<11>, MemoryPools &a_pool_instance) { return a_pool_instance.pool_64; }
inline MemoryPool& get(MemoryPoolSelector<10>, MemoryPools &a_pool_instance) { return a_pool_instance.pool_96; }
inline MemoryPool& get(MemoryPoolSelector<9>, MemoryPools &a_pool_instance) { return a_pool_instance.pool_128; }
inline MemoryPool& get(MemoryPoolSelector<8>, MemoryPools &a_pool_instance) { return a_pool_instance.pool_192; }
inline MemoryPool& get(MemoryPoolSelector<7>, MemoryPools &a_pool_instance) { return a_pool_instance.pool_256; }
inline MemoryPool& get(MemoryPoolSelector<6>, MemoryPools &a_pool_instance) { return a_pool_instance.pool_384; }
inline MemoryPool& get(MemoryPoolSelector<5>, MemoryPools &a_pool_instance) { return a_pool_instance.pool_512; }
inline MemoryPool& get(MemoryPoolSelector<4>, MemoryPools &a_pool_instance) { return a_pool_instance.pool_640; }
inline MemoryPool& get(MemoryPoolSelector<3>, MemoryPools &a_pool_instance) { return a_pool_instance.pool_768; }
inline MemoryPool& get(MemoryPoolSelector<2>, MemoryPools &a_pool_instance) { return a_pool_instance.pool_896; }
inline MemoryPool& get(MemoryPoolSelector<1>, MemoryPools &a_pool_instance) { return a_pool_instance.pool_1024; }

template<size_t Size>
struct PoolAllocatorImp
{
    typedef MemoryPoolSelector<
          (Size <= 32)
        + (Size <= 64)
        + (Size <= 96)
        + (Size <= 128)
        + (Size <= 192)
        + (Size <= 256)
        + (Size <= 384)
        + (Size <= 512)
        + (Size <= 640)
        + (Size <= 768)
        + (Size <= 896)
        + (Size <= 1024)

    > Selector;


    static MemoryPool& get()
    {
    	return memory_pools_aux::get(Selector());
    }

    //Overload to get instance specific pool
    static MemoryPool& get(MemoryPools &a_pool_instance)
    {
        return memory_pools_aux::get(Selector(), a_pool_instance);
    }

    static void* allocate()
    {
        return memory_pools_aux::get(Selector()).allocate();
    }

    static void deallocate(void* p)
    {
    	memory_pools_aux::get(Selector()).deallocate(p);
    }
};



template<class T>
inline MemoryPool& getPool()
{


	return memory_pools_aux::PoolAllocatorImp<sizeof(T)>::get();
}


//Overload to get instance specific pool
template<class T>
inline MemoryPool& getPool(MemoryPools &a_pool_instance)
{
    return memory_pools_aux::PoolAllocatorImp<sizeof(T)>::get(a_pool_instance);
}

template<class T>
inline void warm(int num)
{
	COUT<<"Warming up the pools ...\n";
	LOG(INFO)<<"Warming up the pools ...";

	T* arr[num];
	for (int i = 0; i < num;  i++)
	{
		arr[i] = new T();
	}

	for (int i = 0; i < num;  i++)
	{
		delete arr[i];
	}

	getPool<T>().resetTotalAlloc();
}

template<class T>
inline void warm(int num, MemoryPools &a_pool_instance)
{
    COUT<<"Warming up instance pools ...\n";
    LOG(INFO)<<"Warming up instance pools ...";

    T* arr[num];
    for (int i = 0; i < num;  i++)
    {
        arr[i] = new (a_pool_instance)T(&a_pool_instance);
    }

    for (int i = 0; i < num;  i++)
    {
        delete arr[i];
    }

    getPool<T>(a_pool_instance).resetTotalAlloc();
}

template<class T>
inline void* poolAllocate()
{
	return memory_pools_aux::PoolAllocatorImp<sizeof(T)>::allocate();
}

template<class T>
inline void poolDeallocate(void* p)
{
	// "error: incomplete type" here means the object size is too big to fit in any of the
	// defined pools.
	return memory_pools_aux::PoolAllocatorImp<sizeof(T)>::deallocate(p);
}

template<class T>
inline T* allocate(size_t n, void*)
{
    if(1 != n)
        abort();
    if(void* p = poolAllocate<T>())
        return static_cast<T*>(p);
    throw ABException(" Bad Allocation");
}

template<class T>
inline void deallocate(T* p, size_t n)
{
    if(1 != n)
        abort();
    poolDeallocate<T>(p);
}

}

} /* namespace AB */

