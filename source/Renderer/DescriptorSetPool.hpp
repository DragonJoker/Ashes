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
		*	Le layout à partir duquel sera créé le pool.
		*/
		DescriptorSetPool( DescriptorSetLayout const & layout, uint32_t maxSets );
		/**
		*\brief
		*	Alloue des descripteurs.
		*\param[in] count
		*	Le nombre de descripteurs.
		*/
		void allocate( uint32_t count )const;
		/**
		*\brief
		*	Crée un descriptor set correspondant au layout défini pour ce pool.
		*\return
		*	Le descriptor set créé.
		*/
		DescriptorSet createDescriptorSet()const;
		/**
		*\return
		*	Le layout de descriptor set.
		*/
		inline DescriptorSetLayout const & getLayout()const
		{
			return m_layout;
		}
		/**
		*\return
		*	Le pool de descriptor set vulkan.
		*/
		inline vk::DescriptorPool const & getPool()const
		{
			return *m_pool;
		}

	private:
		DescriptorSetLayout const & m_layout;
		vk::DescriptorPoolPtr m_pool;
		uint32_t m_maxSets;
		mutable uint32_t m_allocated{ 0u };
	};
}

#endif
