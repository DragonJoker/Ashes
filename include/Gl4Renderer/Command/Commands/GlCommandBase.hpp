/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl4Renderer/GlRendererPrerequisites.hpp"

#include "Gl4Renderer/Core/GlDevice.hpp"

namespace ashes::gl4
{
	class CommandBase
	{
	public:
		CommandBase( VkDevice device );
		virtual ~CommandBase()noexcept;

		virtual void apply( ContextLock const & context )const = 0;
		virtual CommandPtr clone()const = 0;

	protected:
		VkDevice m_device;
	};
}
