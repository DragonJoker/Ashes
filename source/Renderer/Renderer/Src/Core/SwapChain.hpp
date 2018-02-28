/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#ifndef ___Renderer_SwapChain_HPP___
#define ___Renderer_SwapChain_HPP___
#pragma once

#include "Core/RenderingResources.hpp"

namespace renderer
{
	/**
	*\~english
	*\brief
	*	Class holding the concept of a swapchain.
	*\remarks
	*	Provides the ability to present rendering results to a surface.
	*\~french
	*\brief
	*	Classe gérant le concept de swapchain.
	*\remarks
	*	Fournit la possibilité de présenter le résultat d'un rendu à une surface.
	*/
	class SwapChain
	{
	protected:
		/**
		*\~english
		*\brief
		*	Constructor.
		*\~french
		*\brief
		*	Constructeur.
		*/
		SwapChain( Device const & device
			, UIVec2 const & size );

	public:
		/**
		*\~english
		*\brief
		*	Destructor.
		*\~french
		*\brief
		*	Destructeur.
		*/
		virtual ~SwapChain() = default;
		/**
		*\~french
		*\return
		*	Retrieve the active rendering resources..
		*\remarks
		*	In Vulkan, this function takes care of the backbuffer image acquisition.
		*\~french
		*\return
		*	Récupère les ressources de rendu actives.
		*\remarks
		*	En Vulkan, cette fonction s'occupe de la récupération de l'image depuis le backbuffer.
		*/
		virtual RenderingResources * getResources() = 0;
		/**
		*\~french
		*\brief
		*	Resets the swap chain.
		*\~french
		*\brief
		*	Réinitialise la swap chain.
		*/
		virtual void reset( UIVec2 const & size ) = 0;
		/**
		*\~french
		*\brief
		*	Creates the backbuffers' framebuffers, compativle with given render pass.
		*\param[in] renderPass
		*	The render pass.
		*\return
		*	The framebuffers.
		*\~french
		*\brief
		*	Crée les tampons d'image des back buffers, compatibles avec la passe de rendu donnée.
		*\param[in] renderPass
		*	La passe de rendu.
		*\return
		*	Les tampons d'images.
		*/
		virtual FrameBufferPtrArray createFrameBuffers( RenderPass const & renderPass )const = 0;
		/**
		*\~french
		*\brief
		*	Creates the backbuffers' framebuffers
		*\return
		*	The framebuffers.
		*\~french
		*\brief
		*	Crée les tampons d'image des back buffers.
		*\return
		*	Les tampons d'images.
		*/
		virtual CommandBufferPtrArray createCommandBuffers()const = 0;
		/**
		*\~french
		*\return
		*	Presents the rendering resources.
		*\~french
		*\return
		*	Présente les ressources de rendu.
		*/
		virtual void present( RenderingResources & resources ) = 0;
		/**
		*\~french
		*\brief
		*	Defines the clear colour of the swapchain.
		*\param[in] value
		*	The new value.
		*\~french
		*\brief
		*	Définit la couleur de vidage de la swapchain.
		*\param[in] value
		*	La nouvelle valeur.
		*/
		virtual void setClearColour( RgbaColour const & value ) = 0;
		/**
		*\~french
		*\return
		*	The clear colour of the swapchain.
		*\~french
		*\return
		*	La couleur de vidage de la swapchain.
		*/
		virtual RgbaColour getClearColour()const = 0;
		/**
		*\~french
		*\return
		*	The swap chain dimensions.
		*\~french
		*\return
		*	Les dimensions de la swap chain.
		*/
		virtual UIVec2 getDimensions()const = 0;
		/**
		*\~french
		*\return
		*	The swapchain pixe format.
		*\~french
		*\return
		*	Les format des pixels de la swap chain.
		*/
		virtual PixelFormat getFormat()const = 0;
		/**
		*\~french
		*\return
		*	Retrieves the default rendering resources.
		*\~french
		*\return
		*	Récupère les ressources de rendu par défaut.
		*/
		inline RenderingResources const & getDefaultResources()const
		{
			return *m_renderingResources[0];
		}

	public:
		using OnResetFunc = std::function< void() >;
		using OnReset = Signal< OnResetFunc >;
		//!\~english	The signal raised when the swapchain is reset.
		//!\~french		Le signal levé lorsque la swap chain est recréée.
		OnReset onReset;

	protected:
		Device const & m_device;
		UIVec2 m_dimensions;
		std::vector< RenderingResourcesPtr > m_renderingResources;
		mutable size_t m_resourceIndex{ 0 };
	};
}

#endif
