/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl4Renderer/GlRendererPrerequisites.hpp"
#include "Gl4Renderer/Enum/GlAttachmentPoint.hpp"
#include "Gl4Renderer/Enum/GlAttachmentType.hpp"

namespace ashes::gl4
{
	void checkCompleteness( GLenum status );
	/**
	*\brief
	*	Classe encapsulant le concept de Framebuffer.
	*\remarks
	*	Contient les tampon de profondeur et de couleur.
	*/
	class Framebuffer
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
		Framebuffer( VkDevice device
			, VkFramebufferCreateInfo createInfo );
		/**
		*\brief
		*	Destructeur
		*/
		~Framebuffer();
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
			, VkAttachmentReferenceArray const & attaches )const;
		/**
		*\~english
		*name
		*	Getters.
		*\~french
		*name
		*	Accesseurs.
		*/
		/**@{*/
		inline GLuint getInternal()const
		{
			assert( m_internal != GL_INVALID_INDEX );
			return m_internal;
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

		inline uint32_t getWidth()const
		{
			return m_width;
		}

		inline uint32_t getHeight()const
		{
			return m_height;
		}
		/**@}*/

	private:
		void doInitialiseAttach( VkImageView view
			, uint32_t index );

	private:
		struct Attachment
		{
			GlAttachmentPoint point;
			GLuint object;
			GlAttachmentType type;
		};
		VkDevice m_device;
		VkFramebufferCreateFlags m_flags;
		VkRenderPass m_renderPass;
		VkImageViewArray m_attachments;
		uint32_t m_width;
		uint32_t m_height;
		uint32_t m_layers;
		GLuint m_internal{ GL_INVALID_INDEX };
		std::vector< Attachment > m_allAttaches;
		std::vector< Attachment > m_colourAttaches;
		Optional< Attachment > m_depthStencilAttach;
		mutable UInt32Array m_drawBuffers;
		bool m_srgb{ false };
	};
}
