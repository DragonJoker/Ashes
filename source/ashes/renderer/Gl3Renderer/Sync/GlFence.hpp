/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/Gl3Renderer/GlRendererPrerequisites.hpp"

namespace ashes::gl3
{
	class Fence
	{
	public:
		Fence( VkDevice device
			, VkFenceCreateFlags flags = 0 );
		~Fence();

		VkResult wait( ContextLock & context
			, uint64_t timeout )const;
		VkResult wait( uint64_t timeout )const;
		void reset( ContextLock & context )const;
		VkResult getStatus( ContextLock & context )const;
		void reset()const;

		inline GLsync getInternal()const
		{
			return m_fence;
		}

	private:
		VkDevice m_device;
		mutable GLsync m_fence{ nullptr };
	};
}
