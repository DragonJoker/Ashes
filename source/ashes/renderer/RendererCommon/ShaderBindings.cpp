/**
*\file
*	RenderLibPrerequisites.h
*\author
*	Sylvain Doremus
*/
#include "renderer/RendererCommon/ShaderBindings.hpp"

namespace ashes
{
	namespace
	{
		bool isUniformBuffer( VkDescriptorType type )
		{
			return type == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER
				|| type == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC
#if VK_EXT_inline_uniform_block
				|| type == VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK_EXT
#endif
				;
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
		, ShaderBindingIndices & indices )
	{
		if ( isUniformBuffer( binding.descriptorType ) )
		{
			bindings.ubo.emplace( makeShaderBindingKey( set, binding.binding ), indices.ubo );
			++indices.ubo;
		}
		else if ( isStorageBuffer( binding.descriptorType ) )
		{
			bindings.sbo.emplace( makeShaderBindingKey( set, binding.binding ), indices.sbo );
			++indices.sbo;
		}
		else if ( isStorageImage( binding.descriptorType ) )
		{
			bindings.img.emplace( makeShaderBindingKey( set, binding.binding ), indices.img );
			indices.img += binding.descriptorCount;
		}
		else if ( isSampledImage( binding.descriptorType ) )
		{
			bindings.tex.emplace( makeShaderBindingKey( set, binding.binding ), indices.tex );
			indices.tex += binding.descriptorCount;
		}
		else if ( isTextureBuffer( binding.descriptorType ) )
		{
			bindings.tbo.emplace( makeShaderBindingKey( set, binding.binding ), indices.tbo );
			indices.tbo += binding.descriptorCount;
		}
	}

	void addReplaceBinding( uint32_t set
		, uint32_t srcBinding
		, VkDescriptorSetLayoutBinding const & dstBinding
		, ShaderBindings & bindings )
	{
		uint32_t index = dstBinding.binding;

		if ( isUniformBuffer( dstBinding.descriptorType ) )
		{
			auto it = bindings.ubo.emplace( makeShaderBindingKey( set, srcBinding ), index ).first;
			it->second = index;
		}
		else if ( isStorageBuffer( dstBinding.descriptorType ) )
		{
			auto it = bindings.sbo.emplace( makeShaderBindingKey( set, srcBinding ), index ).first;
			it->second = index;
		}
		else if ( isStorageImage( dstBinding.descriptorType ) )
		{
			auto it = bindings.img.emplace( makeShaderBindingKey( set, srcBinding ), index ).first;
			it->second = index;
		}
		else if ( isSampledImage( dstBinding.descriptorType ) )
		{
			auto it = bindings.tex.emplace( makeShaderBindingKey( set, srcBinding ), index ).first;
			it->second = index;
		}
		else if ( isTextureBuffer( dstBinding.descriptorType ) )
		{
			auto it = bindings.tbo.emplace( makeShaderBindingKey( set, srcBinding ), index ).first;
			it->second = index;
		}
	}

	void copyBinding( uint32_t set
		, VkDescriptorSetLayoutBinding const & binding
		, ShaderBindings const & srcBindings
		, ShaderBindings & dstBindings )
	{
		auto key = makeShaderBindingKey( set, binding.binding );

		if ( isUniformBuffer( binding.descriptorType ) )
		{
			dstBindings.ubo.insert( *srcBindings.ubo.find( key ) );
		}
		else if ( isStorageBuffer( binding.descriptorType ) )
		{
			dstBindings.sbo.insert( *srcBindings.sbo.find( key ) );
		}
		else if ( isStorageImage( binding.descriptorType ) )
		{
			dstBindings.img.insert( *srcBindings.img.find( key ) );
		}
		else if ( isSampledImage( binding.descriptorType ) )
		{
			dstBindings.tex.insert( *srcBindings.tex.find( key ) );
		}
		else if ( isTextureBuffer( binding.descriptorType ) )
		{
			dstBindings.tbo.insert( *srcBindings.tbo.find( key ) );
		}
	}
}
