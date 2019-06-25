/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/Gl4Renderer/GlRendererPrerequisites.hpp"
#include "renderer/Gl4Renderer/Command/Commands/GlCommandBase.hpp"
#include "renderer/Gl4Renderer/Enum/GlAttachmentPoint.hpp"
#include "renderer/Gl4Renderer/Enum/GlAttachmentType.hpp"

namespace ashes::gl4
{
	bool isSRGBFormat( VkFormat format );
	GlAttachmentPoint getAttachmentPoint( VkFormat format );
	GlAttachmentPoint getAttachmentPoint( VkImageView texture );
	GlAttachmentType getAttachmentType( VkFormat format );
	GlAttachmentType getAttachmentType( VkImageView texture );
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
		Framebuffer( VkDevice device
			, GLuint name );
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
		bool hasOnlySwapchainImage()const;
		bool hasSwapchainImage()const;
		inline GLuint getInternal()const
		{
			assert( m_internal != GL_INVALID_INDEX );
			return m_internal;
		}
		
		inline GLuint & getInternal()
		{
			return m_internal;
		}

		inline auto const & getAttachments()const
		{
			return m_attachments;
		}

		inline auto const & getAllAttaches()const
		{
			return m_allAttaches;
		}

		inline auto const & getMsColourAttaches()const
		{
			return m_colourMsAttaches;
		}

		inline auto const & getColourAttaches()const
		{
			return m_colourAttaches;
		}

		inline auto const & getAllColourAttaches()const
		{
			return m_allColourAttaches;
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

		inline bool isMultisampled()const
		{
			return m_multisampled;
		}

		CmdList const & getBindAttaches()const
		{
			return m_bindAttaches;
		}
		/**@}*/

	private:
		void doInitialiseAttaches();
		void doBindAttaches();
		void doCreateFramebuffer();
		void doInitialiseAttach( VkImageView view
			, uint32_t index );

	private:
		VkDevice m_device;
		VkFramebufferCreateFlags m_flags;
		VkRenderPass m_renderPass;
		VkImageViewArray m_attachments;
		uint32_t m_width;
		uint32_t m_height;
		uint32_t m_layers;
		GLuint m_internal{ GL_INVALID_INDEX };
		FboAttachmentArray m_allAttaches;
		FboAttachmentArray m_allColourAttaches;
		FboAttachmentArray m_colourAttaches;
		FboAttachmentArray m_colourMsAttaches;
		Optional< FboAttachment > m_depthStencilAttach;
		Optional< FboAttachment > m_depthStencilMsAttach;
		mutable UInt32Array m_drawBuffers;
		CmdList m_bindAttaches;
		bool m_srgb{ false };
		bool m_multisampled{ false };
	};
}
