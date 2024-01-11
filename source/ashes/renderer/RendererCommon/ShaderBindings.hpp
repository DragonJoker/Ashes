/**
*\file
*	RenderLibPrerequisites.h
*\author
*	Sylvain Doremus
*/
#pragma once

#ifndef VK_NO_PROTOTYPES
#	define VK_NO_PROTOTYPES
#endif

#include <vulkan/vulkan.h>

#include <cstdint>
#pragma warning( push )
#pragma warning( disable: 4365 )
#include <map>
#pragma warning( pop )

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
		// Image buffer bindings
		ShaderBindingMap ibo;
		// Unordered access view bindings
		ShaderBindingMap uav;
	};

	struct ShaderBindingIndices
	{
		uint32_t ubo{ 0u };
		uint32_t sbo{ 0u };
		uint32_t img{ 0u };
		uint32_t tex{ 0u };
		uint32_t uav{ 0u };
	};

	bool isUniformBuffer( VkDescriptorType type );
	bool isSampledImage( VkDescriptorType type );
	bool isSamplerBuffer( VkDescriptorType type );

	bool isStorageBuffer( VkDescriptorType type );
	bool isStorageImage( VkDescriptorType type );
	bool isImageBuffer( VkDescriptorType type );

	void addBinding( uint32_t set
		, VkDescriptorSetLayoutBinding const & binding
		, ShaderBindings & bindings
		, ShaderBindingIndices & indices );

	void copyBinding( uint32_t set
		, VkDescriptorSetLayoutBinding const & binding
		, ShaderBindings const & srcBindings
		, ShaderBindings & dstBindings );
}
