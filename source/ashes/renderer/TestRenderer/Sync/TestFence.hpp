/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/TestRenderer/TestRendererPrerequisites.hpp"

namespace ashes::test
{
	class Fence
	{
	public:
		Fence( VkDevice device
			, VkFenceCreateFlags flags = 0 );

		VkResult wait( uint64_t timeout )const;
		void reset()const;
		VkResult getStatus()const;
	};
}
