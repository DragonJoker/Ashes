/**
*\file
*	DescriptorSetPool.h
*\author
*	Sylvain Doremus
*/
#ifndef ___D3D11Renderer_DescriptorPool_HPP___
#define ___D3D11Renderer_DescriptorPool_HPP___
#pragma once

#include "D3D11RendererPrerequisites.hpp"

#include <Descriptor/DescriptorPool.hpp>

#include <vector>

namespace d3d11_renderer
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
