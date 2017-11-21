/**
*\file
*	RenderingResources.h
*\author
*	Sylvain Doremus
*/
#ifndef ___Renderer_Buffer_HPP___
#define ___Renderer_Buffer_HPP___
#pragma once

#include "BufferTarget.hpp"
#include "MemoryPropertyFlag.hpp"
#include "RenderingResources.hpp"

#include <VkLib/Buffer.hpp>
#include <VkLib/LogicalDevice.hpp>

namespace renderer
{
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
		*\param[in] resources
		*	Les ressources de rendu.
		*\param[in] count
		*	Le nombre d'éléments du tampon.
		*\param[in] target
		*	Les indicateurs d'utilisation du tampon.
		*\param[in] flags
		*	Les indicateurs de mémoire du tampon.
		*/
		Buffer( RenderingResources const & resources
			, uint32_t count
			, BufferTargets target
			, MemoryPropertyFlags flags )
			: m_resources{ resources }
			, m_buffer{ std::make_unique< vk::Buffer >( resources.getDevice()
				, count * sizeof( T )
				, convert( target )
				, convert( flags ) ) }
		{
		}
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
		Buffer( RenderingResources const & resources
			, std::vector< T > const & data
			, BufferTargets target
			, MemoryPropertyFlags flags )
			: Buffer{ resources, uint32_t( data.size() ), target, flags }
		{
			m_resources.copyBufferData( data
				, *m_buffer );
		}
		/**
		*\return
		*	Le tampon.
		*/
		inline vk::Buffer const & getBuffer()const
		{
			return *m_buffer;
		}

	private:
		RenderingResources const & m_resources;
		vk::Buffer m_buffer;
	};
	/**
	*\brief
	*	Fonction d'aide à la création d'un Buffer.
	*\remarks
	*	Initialise les données du tampon avec celles données.
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
	BufferPtr< T > makeBuffer( RenderingResources const & resources
		, std::vector< T > const & data
		, BufferTargets target
		, MemoryPropertyFlags flags )
	{
		return std::make_unique< Buffer< T > >( resources
			, data
			, target
			, flags );
	}
	/**
	*\brief
	*	Fonction d'aide à la création d'un Buffer.
	*\remarks
	*	Le tampon n'ayant pas de taille définie, il faut impérativement
	*	appeler Buffer::resize, puis Buffer::upload pour lui attribuer
	*	des données.
	*\param[in] resources
	*	Les ressources de rendu.
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
	BufferPtr< T > makeBuffer( RenderingResources const & resources
		, uint32_t count
		, BufferTargets target
		, MemoryPropertyFlags flags )
	{
		return std::make_unique< Buffer< T > >( resources
			, count
			, target
			, flags );
	}
}

#include "Buffer.inl"

#endif
