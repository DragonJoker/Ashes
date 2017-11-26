/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include "RendererPrerequisites.hpp"

#include <VkLib/DescriptorLayoutBinding.hpp>

namespace renderer
{
	/**
	*\brief
	*	Attache d'un layout de descripteur de ressources shader.
	*/
	class DescriptorSetLayoutBinding
	{
	public:
		/**
		*\brief
		*	Constructeur
		*\param[in] index
		*	L'index d'attache.
		*\param[in] type
		*	Le type d'attache.
		*\param[in] flags
		*	Les indicateurs d'étape shader.
		*\~english
		*\brief
		*	Constructor.
		*\param[in] index
		*	The binding index.
		*\param[in] type
		*	The binding type.
		*\param[in] flags
		*	The shader stage flags.
		*/ 
		DescriptorSetLayoutBinding( uint32_t index
			, DescriptorType type
			, ShaderStageFlags flags );
		/**
		*\~french
		*\return
		*	Le type de descripteur de l'attache.
		*\~english
		*\return
		*	The binding descriptor type.
		*/
		inline vk::DescriptorLayoutBinding const & getBinding()const
		{
			return m_binding;
		}

	private:
		vk::DescriptorLayoutBinding m_binding;
	};
}
