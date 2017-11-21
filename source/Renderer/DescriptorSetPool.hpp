/**
*\file
*	DescriptorSetPool.h
*\author
*	Sylvain Doremus
*/
#ifndef ___Renderer_DescriptorSetPool_HPP___
#define ___Renderer_DescriptorSetPool_HPP___
#pragma once

#include "RendererPrerequisites.hpp"

#include <VkLib/DescriptorPool.hpp>

#include <vector>

namespace renderer
{
	/**
	*\brief
	*	Classe template wrappant un vk::DescriptorSetPool.
	*/
	class DescriptorSetPool
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] layout
		*	Le layout � partir duquel sera créé le pool.
		*/
		DescriptorSetPool( DescriptorSetLayout const & layout );
		/**
		*\brief
		*	Crée un descriptor set correspondant au layout d�fini pour ce pool.
		*\return
		*	Le descriptor set créé.
		*/
		DescriptorSet createDescriptorSet()const;
		/**
		*\return
		*	Le pool de descriptor set vulkan.
		*/
		inline vk::DescriptorPool const & getPool()const
		{
			return *m_pool;
		}

	private:
		vk::DescriptorPoolPtr m_pool;
	};
}

#endif
