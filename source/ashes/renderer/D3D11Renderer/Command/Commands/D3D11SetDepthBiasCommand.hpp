/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

namespace ashes::d3d11
{
	class SetDepthBiasCommand
		: public CommandBase
	{
	public:
		SetDepthBiasCommand( VkDevice device
			, float constantFactor
			, float clamp
			, float slopeFactor );

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		float m_constantFactor;
		float m_clamp;
		float m_slopeFactor;
	};
}
