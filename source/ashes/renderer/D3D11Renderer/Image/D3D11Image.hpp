/**
*\file
*	Image.h
*\author
*	Sylvain Doremus
*/
#ifndef ___D3D11Renderer_Texture_HPP___
#define ___D3D11Renderer_Texture_HPP___
#pragma once

#include "D3D11Renderer/D3D11RendererPrerequisites.hpp"

#include <Ashes/Image/Image.hpp>

namespace ashes::d3d11
{
	/**
	*\~french
	*\brief
	*	Classe encapsulant le concept d'image Vulkan.
	*\remarks
	*	Gère la transition de layouts.
	*	Dépendant du fait que l'image provienne de la swap chain
	*	ou d'une ressource, la TestImage sera détruite par le parent
	*	correspondant.
	*\~english
	*\brief
	*	Class wrapping the Vulkan image concept.
	*\remarks
	*	Handles the layouts transition.
	*	Depending on wheter the image comes from a resource or a swap chain,
	*	The TestImage will be destroyed or not.
	*/
	class Image
		: public VkImage
	{
	public:
		Image( Image const & ) = delete;
		Image & operator=( Image const & ) = delete;
		Image( Image && rhs );
		Image & operator=( Image && rhs );
		/**
		*\brief
		*	Constructeur.
		*/
		Image( VkDevice device
			, VkImageCreateInfo const & createInfo );
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*/
		Image( VkDevice device
			, VkFormat format
			, VkExtent2D const & dimensions
			, ID3D11Texture2D * image );
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*/
		Image( VkDevice device
			, VkFormat format
			, VkExtent2D const & dimensions
			, VkImageUsageFlags usageFlags
			, VkImageTiling tiling
			, VkMemoryPropertyFlags memoryFlags );
		/**
		*\brief
		*	Destructeur.
		*/
		~Image();
		/**
		*\copydoc	VkImage::createView
		*/
		VkMemoryRequirements getMemoryRequirements()const;
		/**
		*\copydoc	VkImage::createView
		*/
		ashes::ImageViewPtr createView( ashes::ImageViewCreateInfo const & createInfo )const;
		/**
		*\copydoc	VkImage::generateMipmaps
		*/
		void generateMipmaps( ashes::CommandBuffer & commandBuffer )const;

		inline ID3D11Resource * getResource()const
		{
			return m_image.tex1D;
		}

		inline ID3D11Texture1D * getTexture1D()const
		{
			assert( getType() == VK_IMAGE_TYPE_1D );
			return m_image.tex1D;
		}

		inline ID3D11Texture2D * getTexture2D()const
		{
			assert( getType() == VK_IMAGE_TYPE_2D );
			return m_image.tex2D;
		}

		inline ID3D11Texture3D * getTexture3D()const
		{
			assert( getType() == VK_IMAGE_TYPE_3D );
			return m_image.tex3D;
		}

		bool isRenderTarget()const
		{
			return ashes::d3d11::isRenderTarget( m_createInfo.usage );
		}

		bool isSamplable()const
		{
			return checkFlag( m_createInfo.usage, VK_IMAGE_USAGE_SAMPLED_BIT );
		}

		bool isStorage()const
		{
			return checkFlag( m_createInfo.usage, VK_IMAGE_USAGE_STORAGE_BIT );
		}

	private:
		void doBindMemory()override;

	private:
		VkDevice m_device;
		union
		{
			ID3D11Texture1D * tex1D;
			ID3D11Texture2D * tex2D;
			ID3D11Texture3D * tex3D;
		} m_image;
	};
}

#endif
