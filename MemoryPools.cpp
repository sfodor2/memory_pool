/*
 * MemoryPools.cpp
 *
 *  Created on: Dec 11, 2013
 *      Author: silviu
 */

#include "MemoryPools.h"
#include "Admin/NodeCollections.h"


AB::MemoryPools* AB::MemoryPools::instance = nullptr;


AB::MemoryPools::MemoryPools( const long factor, bool a_non_singletion )
	:
	  pool_32(    32,  1000 * factor)
	, pool_64(    64,  1000 * factor)
	, pool_96(    96,  1000 * factor)
	, pool_128(  128,  1000 * factor)
	, pool_192(  192,  1000 * factor)
	, pool_256(  256,  1000 * factor)
	, pool_384(  384,  1000 * factor)
	, pool_512(  512,  1000 * factor)
	, pool_640(  640,  1000 * factor)
	, pool_768(  768,  1000 * factor)
	, pool_896(  896,  1000 * factor)
	, pool_1024(1024,  1000 * factor)
	, pools(new NavStdVector<MemoryPool>())
	{
		pools->push_back(&pool_32);
		pools->push_back(&pool_64);
		pools->push_back(&pool_96);
		pools->push_back(&pool_128);
		pools->push_back(&pool_192);
		pools->push_back(&pool_256);
		pools->push_back(&pool_384);
		pools->push_back(&pool_512);
		pools->push_back(&pool_640);
		pools->push_back(&pool_768);
		pools->push_back(&pool_896);
		pools->push_back(&pool_1024);

		if(factor <= 0)
		    LOG(FATAL)<<"MemoryPools factor < 0";

		DLOG<<"pool_32:"<<&pool_32;
		DLOG<<"pool_64:"<<&pool_64;
		DLOG<<"pool_96:"<<&pool_96;
		DLOG<<"pool_128:"<<&pool_128;
		DLOG<<"pool_192:"<<&pool_192;
		DLOG<<"pool_256:"<<&pool_256;
		DLOG<<"pool_384:"<<&pool_384;
		DLOG<<"pool_512:"<<&pool_512;
		DLOG<<"pool_640:"<<&pool_640;
		DLOG<<"pool_768:"<<&pool_768;
		DLOG<<"pool_896:"<<&pool_896;
		DLOG<<"pool_1024:"<<&pool_1024;


		LOG(INFO)<<"Memory pools created";

		if(!a_non_singletion)
		{
            if (instance)
                LOG(FATAL)<<"Multiple pools!!!!";

            instance = this;

            DLOG<<"instance:"<<&instance;
		}
	}





void AB::MemoryPools::dump( std::stringstream& out)
{
	if (instance)
	{
		 out<<"Memory Pools:";
		 out<<"\n";
		 pool_32.dump(out);
		 pool_64.dump(out);
		 pool_96.dump(out);
		 pool_128.dump(out);
		 pool_192.dump(out);
		 pool_256.dump(out);
		 pool_384.dump(out);
		 pool_512.dump(out);
		 pool_640.dump(out);
		 pool_768.dump(out);
		 pool_896.dump(out);
		 pool_1024.dump(out);
	}
	else
	{
		out<<"Memory Pools instance is null";
	}
}


void AB::MemoryPools::dump()
{
	if (instance)
	{
	    std::stringstream out;
		dump(out);
		LOG(INFO)<<out.str();
	}
}


//bool AB::MemoryPools::onAdmin( admin::AdminCtx& adm )
//{
//	LOG(INFO) << "Admin :" << adm.req << " nArgs:" << adm.args.size();
//	// Eval for Admin Commands
//	if ( adm.onCmd( "dump", "dump", "dump <all,mb,lt,msu,ssu,ord,ord_ord,ord_cxl,ord_rpl,ord_exec>", 1, 1, true, boost::bind( &MemoryPools::admin_dump, this, _1 ) ) ) goto Done;
//
//
//	LOG(INFO) << "No command!!!";
//
//	return false; // no command executed - Error
//
//	Done:
//	return true; // command executed
//
//}
//
//void AB::MemoryPools::admin_dump( admin::AdminCtx& adm )
//{
//
//	LOG(INFO) << "Dump " << adm.args[1] << '\n';
//	if (adm.args[1] == "all")
//	{
//		dump( adm.resp );
//	} else
//		if (adm.args[1] == "mb")
//		{
//			AB::memory_pools_aux::getPool<AB::market::MarketBook>().dump( adm.resp );
//		} else
//			if (adm.args[1] == "lt")
//			{
//				AB::memory_pools_aux::getPool<AB::market::LastTrade>().dump( adm.resp );
//			} else
//				if (adm.args[1] == "msu")
//				{
//					AB::memory_pools_aux::getPool<AB::market::MarketStatisticUpdate>().dump( adm.resp );
//				} else
//					if (adm.args[1] == "ssu")
//					{
//						AB::memory_pools_aux::getPool<AB::market::SecurityStatusUpdate>().dump( adm.resp );
//					} else
//						if (adm.args[1] == "ord_std")
//						{
//							AB::memory_pools_aux::getPool<AB::order::OrderDesignStandard>().dump( adm.resp );
//						}
//						else
//							if (adm.args[1] == "ord_ord")
//							{
//								AB::memory_pools_aux::getPool<AB::order::OrdOrder>().dump( adm.resp );
//							} else
//								if (adm.args[1] == "ord_cxl")
//								{
//									AB::memory_pools_aux::getPool<AB::order::OrdCancel>().dump( adm.resp );
//								} else
//									if (adm.args[1] == "ord_rpl")
//									{
//										AB::memory_pools_aux::getPool<AB::order::OrdReplace>().dump( adm.resp );
//									}
//									    if (adm.args[1] == "ord_exec")
//										{
//											AB::memory_pools_aux::getPool<AB::order::Execution>().dump( adm.resp );
//										}
//}
