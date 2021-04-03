/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/GlRenderer/GlRendererPrerequisites.hpp"

namespace ashes::gl
{
	class Fence
		: public IcdObject
	{
	public:
		Fence( VkAllocationCallbacks const * allocInfo
			, VkDevice device
			, VkFenceCreateFlags flags = 0 );
		~Fence();

		void insert( ContextLock & context );
		VkResult wait( ContextLock & context
			, uint64_t timeout
			, bool forceWait
			, bool ignoreFirst );
		void reset( ContextLock & context );
		VkResult getStatus( ContextLock & context );

		inline GLsync getInternal()const
		{
			return m_fence;
		}

		inline VkDevice getDevice()const
		{
			return m_device;
		}

	private:
		mutable GLsync m_fence{ nullptr };
		VkDevice m_device;
		bool m_firstUse{ true };
		bool m_signaled{ false };
	};
}
