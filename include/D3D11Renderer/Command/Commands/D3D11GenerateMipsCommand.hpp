/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"
#include "D3D11Renderer/Image/D3D11ImageView.hpp"

namespace d3d11_renderer
{
	class GenerateMipsCommand
		: public CommandBase
	{
	public:
		GenerateMipsCommand( Device const & device
			, Image const & texture );
		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		Image const & m_texture;
		ImageView m_view;
	};
}
