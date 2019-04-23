/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl3Renderer/Command/Commands/GlCommandBase.hpp"

namespace gl_renderer
{
	class SetDepthBiasCommand
		: public CommandBase
	{
	public:
		SetDepthBiasCommand( Device const & device
			, float constantFactor
			, float clamp
			, float slopeFactor );

		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		float m_constantFactor;
		float m_clamp;
		float m_slopeFactor;
	};
}
