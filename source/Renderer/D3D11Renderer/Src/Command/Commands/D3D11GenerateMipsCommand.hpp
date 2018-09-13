/*
This file belongs to RendererLib.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11CommandBase.hpp"

#include "Image/D3D11TextureView.hpp"

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
