/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "TestRendererPrerequisites.hpp"

#include <Ashes/RenderPass/FrameBuffer.hpp>

namespace test_renderer
{
	/**
	*\brief
	*	Classe encapsulant le concept de Framebuffer.
	*\remarks
	*	Contient les tampon de profondeur et de couleur.
	*/
	class FrameBuffer
		: public ashes::FrameBuffer
	{
	public:
		/**
		*\brief
		*	Constructeur, crée un FrameBuffer compatible avec la passe de rendu donnée.
		*\remarks
		*	Si la compatibilité entre les textures voulues et les formats de la passe de rendu
		*	n'est pas possible, une std::runtime_error est lancée.
		*\param[in] dimensions
		*	Les dimensions du tampon d'images.
		*\param[in] textures
		*	Les textures voulues pour le tampon d'images à créer.
		*\~english
		*\brief
		*	Constructor, creates a frame buffer compatible with given render pass.
		*\param[in] device
		*	The logical connection to the GPU.
		*\param[in] renderPass
		*	The render pass with which this framebuffer is compatible.
		*\param[in] dimensions
		*	The frame buffer's dimensions.
		*\param[in] textures
		*	The attachments.
		*/
		FrameBuffer( Device const & device
			, RenderPass const & renderPass
			, ashes::Extent2D const & dimensions
			, ashes::FrameBufferAttachmentArray && attachments );
		/**
		*\~french
		*\brief
		*	Destructeur.
		*\~english
		*\brief
		*	Destructor.
		*/
		~FrameBuffer();
		/**
		*\return
		*	Les dimensions du tampon.
		*/
		inline ashes::Extent2D const & getDimensions()const noexcept
		{
			return m_dimensions;
		}

	private:
		Device const & m_device;
		TextureViewCRefArray m_views;
		ashes::Extent2D m_dimensions;
	};
}
