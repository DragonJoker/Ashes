/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_PipelineShaderStageCreateInfo_HPP___
#define ___AshesPP_PipelineShaderStageCreateInfo_HPP___
#pragma once

#include "ashespp/AshesPPPrerequisites.hpp"

#include "ashespp/Shader/ShaderModule.hpp"

namespace ashes
{
	struct SpecializationInfo
	{
		SpecializationInfo( VkSpecializationMapEntryArray pmapEntries
			, ByteArray pdata )
			: mapEntries{ std::move( pmapEntries ) }
			, data{ std::move( pdata ) }
			, vk
			{
				uint32_t( mapEntries.size() ),
				mapEntries.data(),
				uint32_t( data.size() ),
				data.data()
			}
		{
		}

		operator VkSpecializationInfo const &()const
		{
			return vk;
		}

	private:
		VkSpecializationMapEntryArray mapEntries;
		ByteArray data;
		VkSpecializationInfo vk;
	};

	struct PipelineShaderStageCreateInfo
	{
		PipelineShaderStageCreateInfo( VkPipelineShaderStageCreateFlags flags
			, VkShaderStageFlagBits stage
			, ShaderModulePtr pmodule
			, std::string pname
			, Optional< SpecializationInfo > pspecializationInfo )
			: module{ std::move( pmodule ) }
			, name{ std::move( pname ) }
			, specializationInfo{ std::move( pspecializationInfo ) }
			, vk{ VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO
				, nullptr
				, flags
				, stage
				, *module
				, name.data()
				, nullptr }
		{
			doInit();
		}

		PipelineShaderStageCreateInfo( PipelineShaderStageCreateInfo const & rhs )
			: module{ rhs.module }
			, name{ rhs.name }
			, specializationInfo{ rhs.specializationInfo }
			, vk{ VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO
				, rhs.vk.pNext
				, rhs.vk.flags
				, rhs.vk.stage
				, *module
				, name.data()
				, nullptr }
		{
		}

		PipelineShaderStageCreateInfo( PipelineShaderStageCreateInfo && rhs )noexcept
			: module{ std::move( rhs.module ) }
			, name{ std::move( rhs.name ) }
			, specializationInfo{ std::move( rhs.specializationInfo ) }
			, vk{ VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO
				, rhs.vk.pNext
				, rhs.vk.flags
				, rhs.vk.stage
				, *module
				, name.data()
				, nullptr }
		{
			rhs.vk = { VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO
				, nullptr
				, 0u
				, rhs.vk.stage
				, VkShaderModule{}
				, nullptr
				, nullptr };
		}

		PipelineShaderStageCreateInfo & operator=( PipelineShaderStageCreateInfo const & rhs )
		{
			module = rhs.module;
			name = rhs.name;
			specializationInfo = rhs.specializationInfo;
			vk = { VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO
				, rhs.vk.pNext
				, rhs.vk.flags
				, rhs.vk.stage
				, *module
				, name.data()
				, nullptr };
			doInit();

			return *this;
		}

		PipelineShaderStageCreateInfo & operator=( PipelineShaderStageCreateInfo && rhs )noexcept
		{
			module = std::move( rhs.module );
			name = std::move( rhs.name );
			specializationInfo = std::move( rhs.specializationInfo );
			vk = { VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO
				, rhs.vk.pNext
				, rhs.vk.flags
				, rhs.vk.stage
				, *module
				, name.data()
				, nullptr };
			rhs.vk = { VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO
				, nullptr
				, 0u
				, rhs.vk.stage
				, VkShaderModule{}
				, nullptr
				, nullptr };
			doInit();

			return *this;
		}

		operator VkPipelineShaderStageCreateInfo const &()const
		{
			return vk;
		}

		VkPipelineShaderStageCreateInfo const * operator->()const
		{
			return &vk;
		}

		VkPipelineShaderStageCreateInfo * operator->()
		{
			return &vk;
		}

		ShaderModulePtr module;
		std::string name;
		Optional< SpecializationInfo > specializationInfo;

	private:
		void doInit()
		{
			if ( bool( specializationInfo ) )
			{
				vk.pSpecializationInfo = &static_cast< VkSpecializationInfo const & >( specializationInfo.value() );
			}
		}

	private:
		VkPipelineShaderStageCreateInfo vk;
	};

	using PipelineShaderStageCreateInfoArray = std::vector< PipelineShaderStageCreateInfo >;

	template< typename Type >
	std::vector< Type > makeArray( std::vector< VkPipelineShaderStageCreateInfo > const & values )
	{
		std::vector< Type > result;
		result.reserve( values.size() );

		for ( auto & value : values )
		{
			result.emplace_back( value );
		}

		return result;
	}

	template< typename Type >
	std::vector< Type > makeArray( VkPipelineShaderStageCreateInfo const * values
		, uint32_t count )
	{
		std::vector< Type > result;
		result.reserve( count );

		for ( uint32_t i = 0u; i < count; ++i )
		{
			result.emplace_back( values[i] );
		}

		return result;
	}
}

#endif
