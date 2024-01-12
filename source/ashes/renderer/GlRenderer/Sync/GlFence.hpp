/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/GlRenderer/GlRendererPrerequisites.hpp"

namespace ashes::gl
{
	class Fence
		: private IcdObject
	{
	public:
		Fence( VkAllocationCallbacks const * allocInfo
			, VkDevice device
			, VkFenceCreateFlags flags = 0 );
		~Fence()noexcept;

		void insert( ContextLock const & context );
		VkResult wait( ContextLock const & context
			, uint64_t timeout
			, bool forceWait
			, bool ignoreFirst );
		void reset( ContextLock const & context );
		VkResult getStatus( ContextLock const & context );

		GLsync getInternal()const
		{
			return m_fence;
		}

		bool hasInternal()const
		{
			return m_fence != nullptr;
		}

		VkDevice getDevice()const
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
