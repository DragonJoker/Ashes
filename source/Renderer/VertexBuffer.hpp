/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#ifndef ___Renderer_VertexBuffer_HPP___
#define ___Renderer_VertexBuffer_HPP___
#pragma once

#include "Buffer.hpp"

#include <vector>

namespace renderer
{
	/**
	*\brief
	*	Classe wrappant un vk::VertexBuffer.
	*/
	class VertexBufferBase
	{
	protected:
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
			, uint32_t size
			, BufferTargets target
			, MemoryPropertyFlags flags );

	public:
		/**
		*\return
		*	La taille du tampon.
		*/
		inline uint32_t getSize()const
		{
			return m_size;
		}
		/**
		*\return
		*	Le tampon GPU.
		*/
		inline BufferBase const & getBuffer()const
		{
			return *m_buffer;
		}

	protected:
		Device const & m_device;
		uint32_t m_size;
		BufferBasePtr m_buffer;
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
			, uint32_t count
			, BufferTargets target
			, MemoryPropertyFlags flags );
		/**
		*\return
		*	Le nombre d'éléments contenus dans ce tampon.
		*/
		inline uint32_t getCount()const
		{
			return m_buffer->getSize() / sizeof( T );
		}
		/**
		*\return
		*	Le tampon GPU.
		*/
		inline VertexBufferBase const & getVbo()const
		{
			return *m_vbo;
		}

	private:
		VertexBufferBasePtr m_vbo;
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
		, uint32_t count
		, BufferTargets target
		, MemoryPropertyFlags flags )
	{
		return std::make_unique< VertexBuffer< T > >( device
			, count
			, target
			, flags );
	}
}

#include "VertexBuffer.inl"

#endif
