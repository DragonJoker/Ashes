/**
*\file
*	DescriptorSetPool.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "TestRendererPrerequisites.hpp"

#include <Ashes/Descriptor/DescriptorPool.hpp>

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
