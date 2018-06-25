/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "Descriptor/DescriptorSetLayout.hpp"

#include "Core/Device.hpp"
#include "Core/Exception.hpp"
#include "Core/Renderer.hpp"
#include "Descriptor/DescriptorSetPool.hpp"

#include <algorithm>

namespace renderer
{
	DescriptorSetLayout::DescriptorSetLayout( Device const & device
		, DescriptorSetLayoutBindingArray && bindings )
		: m_device{ device }
		, m_bindings{ std::move( bindings ) }
	{
		auto it = std::find_if( m_bindings.begin()
			, m_bindings.end()
			, []( DescriptorSetLayoutBinding const & binding )
			{
				return binding.getDescriptorType() == DescriptorType::eStorageImage
					|| binding.getDescriptorType() == DescriptorType::eStorageTexelBuffer;
			} );

		if ( it != m_bindings.end()
			&& !device.getRenderer().getFeatures().hasImageTexture )
		{
			throw Exception{ Result::eErrorFeatureNotPresent, "Image storage" };
		}

		registerObject( m_device, "DescriptorSetLayout", this );
	}

	DescriptorSetLayout::~DescriptorSetLayout()
	{
		unregisterObject( m_device, this );
	}

	DescriptorSetLayoutBinding const & DescriptorSetLayout::getBinding( uint32_t bindingPoint
		, uint32_t index )const
	{
		auto it = std::find_if( m_bindings.begin()
			, m_bindings.end()
			, [bindingPoint, index]( DescriptorSetLayoutBinding const & lookup )
		{
			return lookup.getBindingPoint() == bindingPoint
				&& index < lookup.getDescriptorsCount();
		} );

		if ( it == m_bindings.end() )
		{
			throw std::range_error{ "Descriptor set layout binding was not found." };
		}

		return *it;
	}

	renderer::DescriptorSetPoolPtr DescriptorSetLayout::createPool( uint32_t maxSets
		, bool automaticFree )const
	{
		return std::make_unique< DescriptorSetPool >( m_device
			, *this
			, maxSets
			, automaticFree );
	}
}
