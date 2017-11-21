/**
*\file
*	VertexBuffer.h
*\author
*	Sylvain Doremus
*/
#ifndef ___Renderer_VertexBuffer_HPP___
#define ___Renderer_VertexBuffer_HPP___
#pragma once

#include "BufferTarget.hpp"
#include "MemoryPropertyFlag.hpp"
#include "RenderingResources.hpp"
#include "VertexLayout.hpp"

#include <VkLib/LogicalDevice.hpp>
#include <VkLib/VertexBuffer.hpp>

#include <vector>

namespace renderer
{
	/**
	*\brief
	*	Classe template wrappant un vk::VertexBuffer.
	*/
	template< typename T >
	class VertexBuffer
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] resources
		*	Les ressources de rendu.
		*\param[in] count
		*	La nombre d'éléments dans le tampon.
		*\param[in] target
		*	Les indicateurs d'utilisation du tampon.
		*\param[in] flags
		*	Les indicateurs de mémoire du tampon.
		*/
		inline VertexBuffer( RenderingResources const & resources
			, uint32_t bindingSlot
			, uint32_t count
			, BufferTargets target
			, MemoryPropertyFlags flags );
		/**
		*\brief
		*	Constructeur.
		*\param[in] resources
		*	Les ressources de rendu.
		*\param[in] data
		*	Les données du tampon.
		*\param[in] target
		*	Les indicateurs d'utilisation du tampon.
		*\param[in] flags
		*	Les indicateurs de mémoire du tampon.
		*/
		inline VertexBuffer( RenderingResources const & resources
			, uint32_t bindingSlot
			, std::vector< T > const & data
			, BufferTargets target
			, MemoryPropertyFlags flags );
		/**
		*\return
		*	Le tampon.
		*/
		inline vk::VertexBuffer const & getVbo()const
		{
			return *m_buffer;
		}
		/**
		*\brief
		*	Définit les attributs du tampon de sommets.
		*\param[in] layout
		*	Le nouveau layout.
		*/
		inline void setLayout( VertexLayoutPtr && layout )
		{
			m_layout = std::move( layout );
			m_buffer->setLayout( m_layout->getLayout() );
		}
		/**
		*\return
		*	Les attributs du tampon de sommets.
		*/
		inline VertexLayout const & getLayout()const
		{
			assert( m_layout );
			return *m_layout;
		}

	private:
		RenderingResources const & m_resources;
		vk::VertexBufferPtr m_buffer;
		VertexLayoutPtr m_layout;
	};
	/**
	*\brief
	*	Fonction d'aide à la création d'un Buffer.
	*\param[in] resources
	*	Les ressources de rendu.
	*\param[in] count
	*	La nombre d'éléments dans le tampon.
	*\param[in] target
	*	Les indicateurs d'utilisation du tampon.
	*\param[in] flags
	*	Les indicateurs de mémoire du tampon.
	*\return
	*	Le tampon créé.
	*/
	template< typename T >
	inline VertexBufferPtr< T > makeVertexBuffer( RenderingResources const & resources
		, uint32_t bindingSlot
		, uint32_t count
		, BufferTargets target
		, MemoryPropertyFlags flags )
	{
		return std::make_unique< VertexBuffer< T > >( resources
			, bindingSlot
			, count
			, target
			, flags );
	}
	/**
	*\brief
	*	Fonction d'aide à la création d'un Buffer.
	*\param[in] resources
	*	Les ressources de rendu.
	*\param[in] data
	*	Les données du tampon.
	*\param[in] target
	*	Les indicateurs d'utilisation du tampon.
	*\param[in] flags
	*	Les indicateurs de mémoire du tampon.
	*\return
	*	Le tampon créé.
	*/
	template< typename T >
	inline VertexBufferPtr< T > makeVertexBuffer( RenderingResources const & resources
		, uint32_t bindingSlot
		, std::vector< T > const & data
		, BufferTargets target
		, MemoryPropertyFlags flags )
	{
		return std::make_unique< VertexBuffer< T > >( resources
			, bindingSlot
			, data
			, target
			, flags );
	}
}

#include "VertexBuffer.inl"

#endif
