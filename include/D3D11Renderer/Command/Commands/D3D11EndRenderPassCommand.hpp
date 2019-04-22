/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

namespace ashes::d3d11
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
			, ashes::RenderPass const & renderPass
			, ashes::FrameBuffer const & frameBuffer );

		void apply( Context const & context )const;
		CommandPtr clone()const;

	private:
		RenderPass const & m_renderPass;
		FrameBuffer const & m_frameBuffer;
	};
}
