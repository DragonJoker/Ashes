/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "GlRendererPrerequisites.hpp"

#include "Core/GlDevice.hpp"

namespace gl_renderer
{
	class CommandBase
	{
	public:
		CommandBase( Device const & device );
		virtual ~CommandBase()noexcept;

		virtual void apply( ContextLock const & context )const = 0;
		virtual CommandPtr clone()const = 0;

	protected:
		Device const & m_device;
	};
}
