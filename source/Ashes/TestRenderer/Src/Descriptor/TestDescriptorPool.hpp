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
		: public ashes::DescriptorPool
	{
	public:
		DescriptorPool( Device const & device
			, ashes::DescriptorPoolCreateFlags flags
			, uint32_t maxSets
			, ashes::DescriptorPoolSizeArray poolSizes );
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
		*\copydoc	ashes::DescriptorSetPool::createDescriptorSet
		*/
		ashes::DescriptorSetPtr createDescriptorSet( ashes::DescriptorSetLayout const & layout
			, uint32_t bindingPoint )const override;

	private:
		Device const & m_device;
	};
}

#endif
