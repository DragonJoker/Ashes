/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "renderer/Gl3Renderer/GlRendererPrerequisites.hpp"

namespace ashes::gl3
{
	class Semaphore
	{
	public:
		explicit Semaphore( VkDevice device );

		inline GLuint getSemaphore()const
		{
			return m_semaphore;
		}

	private:
		GLuint m_semaphore;
	};
}
