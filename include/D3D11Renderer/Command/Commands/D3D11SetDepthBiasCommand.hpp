/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

namespace ashes::d3d11
{
	class SetDepthBiasCommand
		: public CommandBase
	{
	public:
		SetDepthBiasCommand( Device const & device
			, float constantFactor
			, float clamp
			, float slopeFactor );

		void apply( Context const & context )const;
		CommandPtr clone()const;

	private:
		float m_constantFactor;
		float m_clamp;
		float m_slopeFactor;
	};
}
