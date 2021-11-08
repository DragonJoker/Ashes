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
		void copyBinding( uint32_t key
			, ShaderBindingMap const & srcBindings
			, ShaderBindingMap & dstBindings )
		{
			auto it = srcBindings.find( key );

			if ( it != srcBindings.end() )
			{
				dstBindings.insert( *it );
			}
		}
	}

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

	bool isSamplerBuffer( VkDescriptorType type )
	{
		return type == VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
	}

	bool isImageBuffer( VkDescriptorType type )
	{
		return type == VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
	}

	void addBinding( uint32_t set
		, VkDescriptorSetLayoutBinding const & binding
		, ShaderBindings & bindings
		, ShaderBindingIndices & indices
		, bool commonUavNamespace )
	{
		if ( !binding.descriptorCount )
		{
			return;
		}

		if ( isUniformBuffer( binding.descriptorType ) )
		{
			bindings.ubo.emplace( makeShaderBindingKey( set, binding.binding ), indices.ubo );
			indices.ubo += binding.descriptorCount;
		}
		else if ( isSampledImage( binding.descriptorType ) )
		{
			bindings.tex.emplace( makeShaderBindingKey( set, binding.binding ), indices.tex );
			indices.tex += binding.descriptorCount;
		}
		else if ( isSamplerBuffer( binding.descriptorType ) )
		{
			bindings.tbo.emplace( makeShaderBindingKey( set, binding.binding ), indices.tex );
			indices.tex += binding.descriptorCount;
		}
		else if ( isStorageBuffer( binding.descriptorType ) )
		{
			bindings.sbo.emplace( makeShaderBindingKey( set, binding.binding ), indices.sbo );
			indices.sbo += binding.descriptorCount;
			bindings.uav.emplace( makeShaderBindingKey( set, binding.binding ), indices.uav );
			indices.uav += binding.descriptorCount;
		}
		else if ( isStorageImage( binding.descriptorType ) )
		{
			bindings.img.emplace( makeShaderBindingKey( set, binding.binding ), indices.img );
			indices.img += binding.descriptorCount;
			bindings.uav.emplace( makeShaderBindingKey( set, binding.binding ), indices.uav );
			indices.uav += binding.descriptorCount;
		}
		else if ( isImageBuffer( binding.descriptorType ) )
		{
			bindings.ibo.emplace( makeShaderBindingKey( set, binding.binding ), indices.img );
			indices.img += binding.descriptorCount;
		}
	}

	void copyBinding( uint32_t set
		, VkDescriptorSetLayoutBinding const & binding
		, ShaderBindings const & srcBindings
		, ShaderBindings & dstBindings )
	{
		if ( !binding.descriptorCount )
		{
			return;
		}

		auto key = makeShaderBindingKey( set, binding.binding );

		if ( isUniformBuffer( binding.descriptorType ) )
		{
			copyBinding( key, srcBindings.ubo, dstBindings.ubo );
		}
		else if ( isStorageBuffer( binding.descriptorType ) )
		{
			copyBinding( key, srcBindings.sbo, dstBindings.sbo );
		}
		else if ( isStorageImage( binding.descriptorType ) )
		{
			copyBinding( key, srcBindings.img, dstBindings.img );
		}
		else if ( isSampledImage( binding.descriptorType ) )
		{
			copyBinding( key, srcBindings.tex, dstBindings.tex );
		}
		else if ( isSamplerBuffer( binding.descriptorType ) )
		{
			copyBinding( key, srcBindings.tbo, dstBindings.tbo );
		}
		else if ( isImageBuffer( binding.descriptorType ) )
		{
			copyBinding( key, srcBindings.ibo, dstBindings.ibo );
		}
	}
}
