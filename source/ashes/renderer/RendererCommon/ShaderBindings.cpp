/**
*\file
*	RenderLibPrerequisites.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "renderer/RendererCommon/ShaderBindings.hpp"

namespace ashes
{
	namespace
	{
		bool isUniformBuffer( VkDescriptorType type )
		{
			return type == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER
				|| type == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
		}
		
		bool isStorageBuffer( VkDescriptorType type )
		{
			return type == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER
				|| type == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
		}

		bool isStorageImage( VkDescriptorType type )
		{
			return type == VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
		}

		bool isSampledImage( VkDescriptorType type )
		{
			return type == VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE
				|| type == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
				|| type == VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT
				|| type == VK_DESCRIPTOR_TYPE_SAMPLER;
		}

		bool isTextureBuffer( VkDescriptorType type )
		{
			return type == VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER
				|| type == VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
		}
	}

	void addBinding( uint32_t set
		, VkDescriptorSetLayoutBinding const & binding
		, ShaderBindings & bindings
		, uint32_t & index )
	{
		if ( isUniformBuffer( binding.descriptorType ) )
		{
			bindings.ubo.emplace( makeShaderBindingKey( set, binding.binding ), index );
			++index;
		}
		else if ( isStorageBuffer( binding.descriptorType ) )
		{
			bindings.sbo.emplace( makeShaderBindingKey( set, binding.binding ), index );
			++index;
		}
		else if ( isStorageImage( binding.descriptorType ) )
		{
			bindings.img.emplace( makeShaderBindingKey( set, binding.binding ), index );
			index += binding.descriptorCount;
		}
		else if ( isSampledImage( binding.descriptorType ) )
		{
			bindings.tex.emplace( makeShaderBindingKey( set, binding.binding ), index );
			index += binding.descriptorCount;
		}
		else if ( isTextureBuffer( binding.descriptorType ) )
		{
			bindings.tbo.emplace( makeShaderBindingKey( set, binding.binding ), index );
			index += binding.descriptorCount;
		}
	}
}
