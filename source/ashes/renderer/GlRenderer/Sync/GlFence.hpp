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
			, bool forceWait )const;
		VkResult wait( uint64_t timeout
			, bool forceWait )const;
		void reset( ContextLock & context )const;
		VkResult getStatus( ContextLock & context )const;
		void reset()const;

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
	};
}
