/**
*\file
*	DescriptorSetPool.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_DescriptorPool_HPP___
#define ___GlRenderer_DescriptorPool_HPP___
#pragma once

#include "Gl4Renderer/GlRendererPrerequisites.hpp"

#include <Ashes/Descriptor/DescriptorPool.hpp>

namespace gl_renderer
{
	class DescriptorPool
		: public ashes::DescriptorPool
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] layout
		*	Le layout à partir duquel sera créé le pool.
		*/
		DescriptorPool( Device const & device
			, VkDescriptorPoolCreateFlags flags
			, uint32_t maxSets
			, ashes::DescriptorPoolSizeArray poolSizes );
		/**
		*\copydoc	ashes::DescriptorSetPool::createDescriptorSet
		*/
		ashes::DescriptorSetPtr createDescriptorSet( ashes::DescriptorSetLayout const & layout
			, uint32_t bindingPoint )const override;
	};
}

#endif
