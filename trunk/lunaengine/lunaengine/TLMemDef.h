#ifndef _TLMEMDEF_H_
#define _TLMEMDEF_H_

#include "TSMemDef.h"

namespace TLunaEngine {
	// please rewrite this in app

	// get engine block mem allocator
	TSun::MemAllocator* getEngineBlockMemAllocator();
	// get engine structure mem allocator
	TSun::MemAllocator* getEngineStructMemAllocator();
	// get render block mem allocator
	TSun::MemAllocator* getRenderBlockMemAllocator();
	// get render struct mem allocator
	TSun::MemAllocator* getRenderStructMemAllocator();
}

#endif