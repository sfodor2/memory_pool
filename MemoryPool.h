/*
 * MemoryPool.h
 *
 *  Created on: Dec 11, 2013
 *      Author: silviu
 */

#ifndef MEMORYPOOL_H_
#define MEMORYPOOL_H_
#include <boost/pool/pool.hpp>
#include <boost/thread.hpp>

#include "Common/SpinLock.h"

#include "Admin/Node.h"


namespace AB
{
class MemoryPool:public admin::ObjNode<MemoryPool>
{

private:
	boost::pool<> memPool;
	Name poolName;
	SpinLock<> mtx;
	long curr_alloc, total_alloc;

public:
	MemoryPool(int s, long num):
		memPool(s, num),
		poolName(LC("Pool_") + LC((int)memPool.get_requested_size())),
		curr_alloc(0),
		total_alloc(0)
{
		LOG(INFO)<<poolName;
}
	~MemoryPool(){}

	template< class T >
	void reflectObject( T& f )
	{
		f("Name", poolName)
		("Curr_alloc", curr_alloc)
		 ("Total_alloc", total_alloc);
	}

	template< class T >
	void reflectSummary( T& f )
	{
		reflectObject(f);
	}

	void* allocate()
	{
		void* p = NULL;
		std::lock_guard<SpinLock<>> lock(mtx);
		++curr_alloc;
		++total_alloc;
		p = memPool.malloc();
		return p;
	}

	void deallocate(void* p)
	{
		std::lock_guard<SpinLock<>> lock(mtx);
		--curr_alloc;
		memPool.free(p);
	}

	void dump(std::stringstream& out)
	{
		out<<"MemPool obj size:";
		out<<memPool.get_requested_size();
		out<<" block:";
		out<<(memPool.get_next_size()*memPool.get_requested_size());
		out<<" curr_alloc:";
		out<<curr_alloc;
		out<<" total_alloc:";
		out<<total_alloc;
		out<<"\n";
	}

	void resetTotalAlloc()
	{
	    std::lock_guard<SpinLock<>> lock(mtx);
		total_alloc = 0;
	}

	size_t getSizeObj()
	{
		return memPool.get_requested_size();
	}
	unsigned long getSizeBlock()
	{
		return memPool.get_next_size()*memPool.get_requested_size();
	}
};

} /* namespace AB */
#endif /* MEMORYPOOL_H_ */
