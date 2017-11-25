/*
This file belongs to VkLib.
See LICENSE file in root folder
*/
#pragma once

#include "RendererPrerequisites.hpp"

#include <VkLib/FrameBuffer.hpp>

namespace renderer
{
	/**
	*\brief
	*	Classe encapsulant le concept de Framebuffer.
	*\remarks
	*	Contient les tampon de profondeur et de couleur.
	*/
	class FrameBuffer
	{
	public:
		/**
		*\brief
		*	Crée un FrameBuffer compatible avec la passe de rendu donnée.
		*\remarks
		*	Si la compatibilité entre les textures voulues et les formats de la passe de rendu
		*	n'est pas possible, une std::runtime_error est lancée.
		*\param[in] dimensions
		*	Les dimensions du tampon d'images.
		*\param[in] textures
		*	Les textures voulues pour le tampon d'images à créer.
		*/
		FrameBuffer( RenderPass const & renderPass
			, utils::IVec2 const & dimensions
			, TextureCRefArray const & textures );
		/**
		*\brief
		*	Wrappe un vk::FrameBuffer.
		*\param[in] frameBuffer
		*	Lee vk::FrameBuffer à wrapper.
		*/
		FrameBuffer( vk::FrameBufferPtr && frameBuffer );
		/**
		*\return
		*	Le Framebuffer vulkan.
		*/
		inline vk::FrameBuffer const & getFrameBuffer()const
		{
			assert( m_frameBuffer );
			return *m_frameBuffer;
		}

	private:
		vk::FrameBufferPtr m_frameBuffer;
	};
}
