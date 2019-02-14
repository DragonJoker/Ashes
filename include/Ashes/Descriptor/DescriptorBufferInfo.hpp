/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_DescriptorBufferInfo_HPP___
#define ___Ashes_DescriptorBufferInfo_HPP___
#pragma once

#include "Ashes/AshesPrerequisites.hpp"

namespace ashes
{
	struct DescriptorBufferInfo
	{
		BufferCRef buffer;
		uint64_t offset;
		uint64_t range;
	};
}

#endif
