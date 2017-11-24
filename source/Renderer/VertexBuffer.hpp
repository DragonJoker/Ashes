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
	*	Classe wrappant un vk::VertexBuffer.
	*/
	class VertexBufferBase
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*\param[in] size
		*	La taille du tampon.
		*\param[in] target
		*	Les indicateurs d'utilisation du tampon.
		*\param[in] flags
		*	Les indicateurs de mémoire du tampon.
		*/
		VertexBufferBase( Device const & device
			, uint32_t bindingSlot
			, uint32_t size
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

	protected:
		Device const & m_device;
		vk::VertexBufferPtr m_buffer;
		VertexLayoutPtr m_layout;
	};
	/**
	*\brief
	*	Classe template wrappant un vk::VertexBuffer.
	*/
	template< typename T >
	class VertexBuffer
		: public VertexBufferBase
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*\param[in] count
		*	La nombre d'éléments dans le tampon.
		*\param[in] target
		*	Les indicateurs d'utilisation du tampon.
		*\param[in] flags
		*	Les indicateurs de mémoire du tampon.
		*/
		inline VertexBuffer( Device const & device
			, uint32_t bindingSlot
			, uint32_t count
			, BufferTargets target
			, MemoryPropertyFlags flags );
		/**
		*\return
		*	Le nombre d'éléments contenus dans ce tampon.
		*/
		inline uint32_t getCount()const
		{
			return m_buffer->getBuffer().getSize() / sizeof( T );
		}
	};
	/**
	*\brief
	*	Fonction d'aide à la création d'un Buffer.
	*\param[in] device
	*	Le périphérique logique.
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
	inline VertexBufferPtr< T > makeVertexBuffer( Device const & device
		, uint32_t bindingSlot
		, uint32_t count
		, BufferTargets target
		, MemoryPropertyFlags flags )
	{
		return std::make_unique< VertexBuffer< T > >( device
			, bindingSlot
			, count
			, target
			, flags );
	}
}

#include "VertexBuffer.inl"

#endif
