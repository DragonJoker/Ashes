/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#ifndef ___Renderer_Buffer_HPP___
#define ___Renderer_Buffer_HPP___
#pragma once

#include "BufferTarget.hpp"
#include "Device.hpp"
#include "MemoryPropertyFlag.hpp"

namespace renderer
{
	/**
	*\brief
	*	Classe de tampon GPU.
	*/
	class BufferBase
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
		BufferBase( Device const & device
			, uint32_t size
			, BufferTargets target
			, MemoryPropertyFlags flags );

	public:
		/**
		*\~english
		*\brief
		*	Destructor.
		*\~french
		*\brief
		*	Destructeur.
		*/
		virtual ~BufferBase() = default;
		/**
		*\return
		*	La taille du tampon.
		*/
		inline uint32_t getSize()const
		{
			return m_size;
		}

	private:
		Device const & m_device;
		uint32_t m_size;
	};
	/**
	*\brief
	*	Classe regroupant les ressources de rendu nécessaires au dessin d'une image.
	*/
	template< typename T >
	class Buffer
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*\param[in] count
		*	Le nombre d'éléments du tampon.
		*\param[in] target
		*	Les indicateurs d'utilisation du tampon.
		*\param[in] flags
		*	Les indicateurs de mémoire du tampon.
		*/
		Buffer( Device const & device
			, uint32_t count
			, BufferTargets target
			, MemoryPropertyFlags flags );
		/**
		*\return
		*	Le nombre d'éléments.
		*/
		inline uint32_t getCount()const
		{
			return uint32_t( m_buffer->getSize() / sizeof( T ) );
		}
		/**
		*\return
		*	Le tampon.
		*/
		inline BufferBase const & getBuffer()const
		{
			return *m_buffer;
		}

	private:
		BufferBasePtr m_buffer;
	};
	/**
	*\brief
	*	Fonction d'aide à la création d'un Buffer.
	*\remarks
	*	Le tampon n'ayant pas de taille définie, il faut impérativement
	*	appeler Buffer::resize, puis Buffer::upload pour lui attribuer
	*	des données.
	*\param[in] device
	*	Les périphérique logique.
	*\param[in] count
	*	Le nombre d'éléments du tampon.
	*\param[in] target
	*	Les indicateurs d'utilisation du tampon.
	*\param[in] flags
	*	Les indicateurs de mémoire du tampon.
	*\return
	*	Le tampon créé.
	*/
	template< typename T >
	BufferPtr< T > makeBuffer( Device const & device
		, uint32_t count
		, BufferTargets target
		, MemoryPropertyFlags flags )
	{
		return std::make_unique< Buffer< T > >( device
			, count
			, target
			, flags );
	}
}

#include "Buffer.inl"

#endif
