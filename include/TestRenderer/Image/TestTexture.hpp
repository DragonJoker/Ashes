/**
*\file
*	Texture.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "TestRendererPrerequisites.hpp"

#include <Ashes/Image/Texture.hpp>

namespace test_renderer
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
		: public ashes::Texture
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
			, ashes::ImageCreateInfo const & createInfo );
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*/
		Texture( Device const & device
			, ashes::Format format
			, ashes::Extent2D const & dimensions );
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*/
		Texture( Device const & device
			, ashes::Format format
			, ashes::Extent2D const & dimensions
			, ashes::ImageUsageFlags usageFlags
			, ashes::ImageTiling tiling
			, ashes::MemoryPropertyFlags memoryFlags );
		/**
		*\brief
		*	Destructeur.
		*/
		~Texture();
		/**
		*\copydoc	ashes::Texture::createView
		*/
		ashes::MemoryRequirements getMemoryRequirements()const override;
		/**
		*\copydoc	ashes::Texture::createView
		*/
		ashes::TextureViewPtr createView( ashes::ImageViewCreateInfo const & createInfo )const override;

	private:
		void doBindMemory()override;

	private:
		Device const & m_device;
	};
}
