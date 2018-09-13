/*
This file belongs to RendererLib.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11CommandBase.hpp"

namespace d3d11_renderer
{
	/**
	*\brief
	*	Commande de fin d'une passe de rendu, unbind son framebuffer.
	*/
	class EndRenderPassCommand
		: public CommandBase
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*/
		EndRenderPassCommand( Device const & device
			, renderer::RenderPass const & renderPass
			, renderer::FrameBuffer const & frameBuffer );

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		RenderPass const & m_renderPass;
		FrameBuffer const & m_frameBuffer;
	};
}
