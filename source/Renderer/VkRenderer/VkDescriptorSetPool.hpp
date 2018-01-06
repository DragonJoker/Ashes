/**
*\file
*	DescriptorSetPool.h
*\author
*	Sylvain Doremus
*/
#ifndef ___VkRenderer_DescriptorSetPool_HPP___
#define ___VkRenderer_DescriptorSetPool_HPP___
#pragma once

#include "VkRendererPrerequisites.hpp"

#include <VkLib/DescriptorPool.hpp>
#include <Renderer/DescriptorSetPool.hpp>

#include <vector>

namespace vk_renderer
{
	/**
	*\brief
	*	Classe template wrappant un vk::DescriptorSetPool.
	*/
	class DescriptorSetPool
		: public renderer::DescriptorSetPool
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] layout
		*	Le layout à partir duquel sera créé le pool.
		*/
		DescriptorSetPool( renderer::DescriptorSetLayout const & layout
			, uint32_t maxSets );
		/**
		*\brief
		*	Crée un descriptor set correspondant au layout défini pour ce pool.
		*\return
		*	Le descriptor set créé.
		*/
		renderer::DescriptorSetPtr createDescriptorSet()const override;
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
