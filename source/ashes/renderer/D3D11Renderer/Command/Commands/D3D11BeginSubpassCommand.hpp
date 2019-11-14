/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

namespace ashes::d3d11
{
	/**
	*\brief
	*	Démarre une passe de rendu en bindant son framebuffer, et en le vidant au besoin.
	*/
	class BeginSubpassCommand
		: public CommandBase
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] renderPass
		*	La passe de rendu.
		*\param[in] frameBuffer
		*	Le tampon d'image affecté par le rendu.
		*\param[in] clearValues
		*	Les valeurs de vidage, une par attache de la passe de rendu.
		*\param[in] contents
		*	Indique la manière dont les commandes de la première sous-passe sont fournies.
		*/
		BeginSubpassCommand( VkDevice device
			, VkRenderPass renderPass
			, VkFramebuffer frameBuffer
			, VkSubpassDescription const & subpass );

		void apply( Context const & context )const;
		CommandPtr clone()const;

	private:
		VkRenderPass m_renderPass;
		VkSubpassDescription const & m_subpass;
		VkFramebuffer m_frameBuffer;
		std::vector< ID3D11RenderTargetView * > m_attaches;
		ID3D11DepthStencilView * m_depthAttach{ nullptr };
	};
}
