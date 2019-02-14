/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"
#include "D3D11Renderer/Image/D3D11TextureView.hpp"

namespace d3d11_renderer
{
	class GenerateMipsCommand
		: public CommandBase
	{
	public:
		GenerateMipsCommand( Device const & device
			, Texture const & texture );
		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		Texture const & m_texture;
		TextureView m_view;
	};
}
