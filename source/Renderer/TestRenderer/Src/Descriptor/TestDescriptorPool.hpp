/**
*\file
*	DescriptorSetPool.h
*\author
*	Sylvain Doremus
*/
#ifndef ___TestRenderer_DescriptorPool_HPP___
#define ___TestRenderer_DescriptorPool_HPP___
#pragma once

#include "TestRendererPrerequisites.hpp"

#include <Descriptor/DescriptorPool.hpp>

#include <vector>

namespace test_renderer
{
	class DescriptorPool
		: public renderer::DescriptorPool
	{
	public:
		DescriptorPool( Device const & device
			, renderer::DescriptorPoolCreateFlags flags
			, uint32_t maxSets
			, renderer::DescriptorPoolSizeArray poolSizes );
		/**
		*\~french
		*\brief
		*	Destructeur.
		*\~english
		*\brief
		*	Destructor.
		*/
		~DescriptorPool();
		/**
		*\copydoc	renderer::DescriptorSetPool::createDescriptorSet
		*/
		renderer::DescriptorSetPtr createDescriptorSet( renderer::DescriptorSetLayout const & layout
			, uint32_t bindingPoint )const override;

	private:
		Device const & m_device;
	};
}

#endif
