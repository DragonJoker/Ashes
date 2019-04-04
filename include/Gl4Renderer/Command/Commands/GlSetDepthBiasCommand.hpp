/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl4Renderer/Command/Commands/GlCommandBase.hpp"

namespace ashes::gl4
{
	class SetDepthBiasCommand
		: public CommandBase
	{
	public:
		SetDepthBiasCommand( VkDevice device
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
