/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"
#include "D3D11Renderer/Image/D3D11ImageView.hpp"

namespace ashes::d3d11
{
	class GenerateMipsCommand
		: public CommandBase
	{
	public:
		GenerateMipsCommand( VkDevice device
			, Image const & texture );
		void apply( Context const & context )const;
		CommandPtr clone()const;

	private:
		Image const & m_texture;
		ImageView m_view;
	};
}
