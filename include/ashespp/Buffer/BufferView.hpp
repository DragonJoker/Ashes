/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_BufferView_HPP___
#define ___AshesPP_BufferView_HPP___
#pragma once

#include "ashespp/AshesPPPrerequisites.hpp"

namespace ashes
{
	/**
	*\~french
	*\brief
	*	Description d'une vue sur un tampon.
	*\~english
	*\brief
	*	A view on a buffer.
	*/
	class BufferView
	{
	public:
		/**
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] device
		*	La connexion logique au GPU.
		*\param[in] buffer
		*	Le tampon sur lequel la vue est créée.
		*\param[in] format
		*	Le format des pixels de la vue.
		*\param[in] offset
		*	Le décalage dans le tampon.
		*\param[in] range
		*	Le nombre d'éléments dans le tampon.
		*\~english
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical connection to the GPU.
		*\param[in] buffer
		*	The buffer from which the view is created.
		*\param[in] format
		*	The view's pixels format.
		*\param[in] offset
		*	The offset in the buffer.
		*\param[in] range
		*	The number of elements from the buffer.
		*/
		BufferView( Device const & device
			, BufferBase const & buffer
			, VkFormat format
			, VkDeviceSize offset
			, VkDeviceSize range );
		/**
		*\~french
		*\brief
		*	Destructeur.
		*\~english
		*\brief
		*	Destructor.
		*/
		~BufferView();
		/**
		*\~french
		*\return
		*	Le format des pixels de la vue.
		*\~english
		*\return
		*	The view's pixels format.
		*/
		inline VkFormat getFormat()const
		{
			return m_format;
		}
		/**
		*\~french
		*\return
		*	Le tampon de la vue.
		*\~english
		*\return
		*	The view's buffer.
		*/
		inline BufferBase const & getBuffer()const
		{
			return m_buffer;
		}
		/**
		*\~french
		*\return
		*	Le décalage dans le tampon.
		*\~english
		*\return
		*	The offset in the buffer.
		*/
		inline VkDeviceSize getOffset()const
		{
			return m_offset;
		}
		/**
		*\~french
		*\return
		*	Le nombre d'éléments dans le tampon.
		*\~english
		*\return
		*	The number of elements from the buffer.
		*/
		inline VkDeviceSize getRange()const
		{
			return m_range;
		}
		/**
		*\~french
		*\brief
		*	Conversion implicite vers VkBufferView.
		*\~english
		*\brief
		*	VkBufferView implicit cast operator.
		*/
		inline operator VkBufferView const &()const
		{
			return m_internal;
		}

	private:
		Device const & m_device;
		BufferBase const & m_buffer;
		VkFormat m_format;
		VkDeviceSize m_offset;
		VkDeviceSize m_range;
		VkBufferView m_internal{ VK_NULL_HANDLE };
	};
}

#endif
