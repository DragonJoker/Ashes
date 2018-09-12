/**
*\file
*	Texture.h
*\author
*	Sylvain Doremus
*/
#ifndef ___D3D11Renderer_Texture_HPP___
#define ___D3D11Renderer_Texture_HPP___
#pragma once

#include "D3D11RendererPrerequisites.hpp"

#include <Image/Texture.hpp>

namespace d3d11_renderer
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
	class Texture
		: public renderer::Texture
	{
	public:
		Texture( Texture const & ) = delete;
		Texture & operator=( Texture const & ) = delete;
		Texture( Texture && rhs );
		Texture & operator=( Texture && rhs );
		/**
		*\brief
		*	Constructeur.
		*/
		Texture( Device const & device
			, renderer::ImageCreateInfo const & createInfo );
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*/
		Texture( Device const & device
			, renderer::Format format
			, renderer::Extent2D const & dimensions
			, ID3D11Texture2D * image );
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*/
		Texture( Device const & device
			, renderer::Format format
			, renderer::Extent2D const & dimensions
			, renderer::ImageUsageFlags usageFlags
			, renderer::ImageTiling tiling
			, renderer::MemoryPropertyFlags memoryFlags );
		/**
		*\brief
		*	Destructeur.
		*/
		~Texture();
		/**
		*\copydoc	renderer::Texture::createView
		*/
		renderer::MemoryRequirements getMemoryRequirements()const override;
		/**
		*\copydoc	renderer::Texture::createView
		*/
		renderer::TextureViewPtr createView( renderer::ImageViewCreateInfo const & createInfo )const override;
		/**
		*\copydoc	renderer::Texture::generateMipmaps
		*/
		void generateMipmaps( renderer::CommandBuffer & commandBuffer )const override;

		inline ID3D11Resource * getResource()const
		{
			return m_image.tex1D;
		}

		inline ID3D11Texture1D * getTexture1D()const
		{
			assert( getType() == renderer::TextureType::e1D );
			return m_image.tex1D;
		}

		inline ID3D11Texture2D * getTexture2D()const
		{
			assert( getType() == renderer::TextureType::e2D );
			return m_image.tex2D;
		}

		inline ID3D11Texture3D * getTexture3D()const
		{
			assert( getType() == renderer::TextureType::e3D );
			return m_image.tex3D;
		}

		bool isRenderTarget()const
		{
			return d3d11_renderer::isRenderTarget( m_createInfo.usage );
		}

		bool isSamplable()const
		{
			return checkFlag( m_createInfo.usage, renderer::ImageUsageFlag::eSampled );
		}

		bool isStorage()const
		{
			return checkFlag( m_createInfo.usage, renderer::ImageUsageFlag::eStorage );
		}

	private:
		void doBindMemory()override;

	private:
		Device const & m_device;
		renderer::ImageCreateInfo m_createInfo;
		union
		{
			ID3D11Texture1D * tex1D;
			ID3D11Texture2D * tex2D;
			ID3D11Texture3D * tex3D;
		} m_image;
	};
}

#endif
