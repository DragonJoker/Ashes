/*
This file belongs to RendererLib.
See LICENSE file in root folder
*/
#pragma once

#include "GlRendererPrerequisites.hpp"

#include <RenderPass/FrameBuffer.hpp>

namespace gl_renderer
{
	GlAttachmentPoint getAttachmentPoint( GlInternal format );
	GlAttachmentPoint getAttachmentPoint( renderer::Format format );
	GlAttachmentPoint getAttachmentPoint( TextureView const & texture );
	GlAttachmentType getAttachmentType( GlInternal format );
	GlAttachmentType getAttachmentType( renderer::Format format );
	GlAttachmentType getAttachmentType( TextureView const & texture );
	void checkCompleteness( GLenum status );

	struct FboAttachment
	{
		GlAttachmentPoint point;
		GLuint object;
		GlAttachmentType type;
	};
	/**
	*\brief
	*	Classe encapsulant le concept de Framebuffer.
	*\remarks
	*	Contient les tampon de profondeur et de couleur.
	*/
	class FrameBuffer
		: public renderer::FrameBuffer
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
		*/
		FrameBuffer( RenderPass const & renderPass
			, renderer::Extent2D const & dimensions );
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
			, renderer::Extent2D const & dimensions
			, renderer::FrameBufferAttachmentArray && textures );
		/**
		*\brief
		*	Destructeur
		*/
		~FrameBuffer();
		/**
		*\~english
		*\brief
		*	Sets the draw buffers.
		*\param[in] attaches
		*	The attaches.
		*\~french
		*\brief
		*	Définit les tampons d'écriture.
		*\param[in] attaches
		*	Les attaches.
		*/
		void setDrawBuffers( AttachmentDescriptionArray const & attaches )const;
		/**
		*\~english
		*\brief
		*	Sets the draw buffers.
		*\param[in] attaches
		*	The attaches.
		*\~french
		*\brief
		*	Définit les tampons d'écriture.
		*\param[in] attaches
		*	Les attaches.
		*/
		void setDrawBuffers( renderer::AttachmentReferenceArray const & attaches )const;
		/**
		*\~english
		*name
		*	Getters.
		*\~french
		*name
		*	Accesseurs.
		*/
		/**@{*/
		inline GLuint getFrameBuffer()const
		{
			assert( m_frameBuffer != GL_INVALID_INDEX );
			return m_frameBuffer;
		}

		inline auto const & getAllAttaches()const
		{
			return m_allAttaches;
		}

		inline auto const & getColourAttaches()const
		{
			return m_colourAttaches;
		}

		inline bool hasDepthStencilAttach()const
		{
			return bool( m_depthStencilAttach );
		}

		inline auto const & getDepthStencilAttach()const
		{
			assert( hasDepthStencilAttach() );
			return m_depthStencilAttach.value();
		}
		/**@}*/

	private:
		GLuint m_frameBuffer{ GL_INVALID_INDEX };
		RenderPass const & m_renderPass;
		std::vector< FboAttachment > m_allAttaches;
		std::vector< FboAttachment > m_colourAttaches;
		std::optional< FboAttachment > m_depthStencilAttach;
		mutable renderer::UInt32Array m_drawBuffers;
	};
}
