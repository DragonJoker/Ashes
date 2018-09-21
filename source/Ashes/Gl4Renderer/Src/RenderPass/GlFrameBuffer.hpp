/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "GlRendererPrerequisites.hpp"

#include <RenderPass/FrameBuffer.hpp>

namespace gl_renderer
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
		*	Crée un FrameBuffer compatible avec la passe de rendu donnée.
		*\remarks
		*	Si la compatibilité entre les textures voulues et les formats de la passe de rendu
		*	n'est pas possible, une std::runtime_error est lancée.
		*\param[in] dimensions
		*	Les dimensions du tampon d'images.
		*/
		FrameBuffer( Device const & device
			, RenderPass const & renderPass
			, ashes::Extent2D const & dimensions
			, ashes::FrameBufferAttachmentArray && textures
			, bool backBuffer = false );
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
		void setDrawBuffers( ContextLock const & context
			, AttachmentDescriptionArray const & attaches )const;
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
		void setDrawBuffers( ContextLock const & context
			, ashes::AttachmentReferenceArray const & attaches )const;
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

		inline bool isSRGB()const
		{
			return m_srgb;
		}
		/**@}*/

	private:
		void doInitialiseBackBuffer();
		void doInitialiseFramebuffer();
		void doInitialiseBackAttach( ashes::FrameBufferAttachment const & attach );
		void doInitialiseFboAttach( ashes::FrameBufferAttachment const & attach );

	private:
		struct Attachment
		{
			GlAttachmentPoint point;
			GLuint object;
			GlAttachmentType type;
		};
		Device const & m_device;
		GLuint m_frameBuffer{ GL_INVALID_INDEX };
		RenderPass const & m_renderPass;
		std::vector< Attachment > m_allAttaches;
		std::vector< Attachment > m_colourAttaches;
		std::optional< Attachment > m_depthStencilAttach;
		mutable ashes::UInt32Array m_drawBuffers;
		bool m_srgb{ false };
	};
}
