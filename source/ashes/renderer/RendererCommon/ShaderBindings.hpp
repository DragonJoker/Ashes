/**
*\file
*	RenderLibPrerequisites.h
*\author
*	Sylvain Doremus
*/
#pragma once

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

#include <cstdint>
#include <map>

namespace ashes
{
	inline uint32_t makeShaderBindingKey( uint32_t set, uint32_t binding )
	{
		return ( set << 16u ) | ( binding << 0u );
	}

	inline uint64_t makeDescriptorKey( VkDescriptorSet descriptorSet
		, uint32_t descriptorSetIndex )
	{
		return uint64_t( descriptorSetIndex ) << 32
			| ( uint64_t( descriptorSet ) & 0x00000000FFFFFFFF );
	}

	using ShaderBindingMap = std::map< uint32_t, uint32_t >;

	struct ShaderBindings
	{
		// UBO bindings
		ShaderBindingMap ubo;
		// SSBO bindings
		ShaderBindingMap sbo;
		// Storage image bindings
		ShaderBindingMap img;
		// Sampled image bindings
		ShaderBindingMap tex;
		// Texture buffer bindings
		ShaderBindingMap tbo;
	};

	void addBinding( uint32_t set
		, VkDescriptorSetLayoutBinding const & binding
		, ShaderBindings & bindings
		, uint32_t & index );

	void addReplaceBinding( uint32_t set
		, uint32_t srcBinding
		, VkDescriptorSetLayoutBinding const & binding
		, ShaderBindings & bindings );

	void copyBinding( uint32_t set
		, VkDescriptorSetLayoutBinding const & binding
		, ShaderBindings const & srcBindings
		, ShaderBindings & dstBindings );
}
