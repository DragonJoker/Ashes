/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_RenderPass_HPP___
#define ___AshesPP_RenderPass_HPP___
#pragma once

#include "RenderPassCreateInfo.hpp"

namespace ashes
{
	/**
	*\~french
	*\brief
	*	Description d'une passe de rendu (pouvant contenir plusieurs sous-passes).
	*\~english
	*\brief
	*	Describes a render pass (which can contain one or more render subpasses).
	*/
	class RenderPass
	{
	public:
		/**
		*\~english
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical connection to the GPU.
		*\param[in] createInfo
		*	The creation informations.
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] device
		*	La connexion logique au GPU.
		*\param[in] createInfo
		*	Les informations de création.
		*/
		RenderPass( Device const & device
			, RenderPassCreateInfo createInfo );
		/**
		*\~english
		*\brief
		*	Destructor.
		*\~french
		*\brief
		*	Destructeur.
		*/
		~RenderPass();
		/**
		*\~french
		*\brief
		*	Crée un tampon d'images compatible avec la passe de rendu.
		*\remarks
		*	Si la compatibilité entre les textures voulues et les formats de la passe de rendu
		*	n'est pas possible, une std::runtime_error est lancée.
		*\param[in] dimensions
		*	Les dimensions du tampon d'images.
		*\param[in] views
		*	Les vues pour le tampon d'images à créer.
		*\return
		*	Le FrameBuffer créé.
		*\~english
		*\brief
		*	Creates a frame buffer compatible with this render pass.
		*\remarks
		*	If the compatibility between wanted views and the render pass' formats
		*	is not possible, a std::runtime_error will be thrown.
		*\param[in] dimensions
		*	The frame buffer's dimensions.
		*\param[in] views
		*	The views for the frame buffer to create.
		*\return
		*	The created frame buffer.
		*/
		FrameBufferPtr createFrameBuffer( VkExtent2D const & dimensions
			, ImageViewArray views )const;
		/**
		*\~english
		*name
		*	Getters.
		*\~french
		*name
		*	Accesseurs.
		*/
		/**@{*/
		inline size_t getAttachmentCount()const
		{
			return m_createInfo.attachments.size();
		}

		inline VkAttachmentDescriptionArray const & getAttachments()const
		{
			return m_createInfo.attachments;
		}

		inline Device const & getDevice()const
		{
			return m_device;
		}

		inline size_t getSubpassCount()const
		{
			return m_createInfo.subpasses.size();
		}

		inline SubpassDescriptionArray const & getSubpasses()const
		{
			return m_createInfo.subpasses;
		}
		/**@}*/
		/**
		*\~french
		*\brief
		*	Conversion implicite vers VkRenderPass.
		*\~english
		*\brief
		*	VkRenderPass implicit cast operator.
		*/
		inline operator VkRenderPass const & ()const
		{
			return m_internal;
		}

	private:
		Device const & m_device;
		RenderPassCreateInfo m_createInfo;
		VkRenderPass m_internal{ VK_NULL_HANDLE };
	};
}

#endif
