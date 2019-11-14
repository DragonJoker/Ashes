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
		SpecializationInfo( VkSpecializationMapEntryArray mapEntries
			, ByteArray data )
			: mapEntries{ std::move( mapEntries ) }
			, data{ std::move( data ) }
			, vk
			{
				uint32_t( this->mapEntries.size() ),
				this->mapEntries.data(),
				uint32_t( this->data.size() ),
				this->data.data()
			}
		{
		}

		inline operator VkSpecializationInfo const &( )const
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
			, ShaderModulePtr module
			, std::string name
			, Optional< SpecializationInfo > specializationInfo )
			: module{ std::move( module ) }
			, name{ std::move( name ) }
			, specializationInfo{ std::move( specializationInfo ) }
			, vk
			{
				VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
				nullptr,
				flags,
				stage,
				*this->module,
				this->name.data(),
				nullptr,
			}
		{
			doInit();
		}

		PipelineShaderStageCreateInfo( PipelineShaderStageCreateInfo const & rhs )
			: module{ rhs.module }
			, name{ rhs.name }
			, specializationInfo{ rhs.specializationInfo }
			, vk
			{
				VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
				nullptr,
				rhs.vk.flags,
				rhs.vk.stage,
				*module,
				name.data(),
				nullptr,
			}
		{
			doInit();
		}

		PipelineShaderStageCreateInfo & operator=( PipelineShaderStageCreateInfo const & rhs )
		{
			module = rhs.module;
			name = rhs.name;
			specializationInfo = rhs.specializationInfo;
			vk =
			{
				VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
				nullptr,
				rhs.vk.flags,
				rhs.vk.stage,
				*module,
				name.data(),
				nullptr,
			};
			doInit();

			return *this;
		}

		PipelineShaderStageCreateInfo( PipelineShaderStageCreateInfo && rhs )
			: module{ std::move( rhs.module ) }
			, name{ std::move( rhs.name ) }
			, specializationInfo{ std::move( rhs.specializationInfo ) }
			, vk
			{
				VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
				nullptr,
				rhs.vk.flags,
				rhs.vk.stage,
				*this->module,
				this->name.data(),
				nullptr,
			}
		{
			rhs.vk =
			{
				VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
				nullptr,
				0u,
				rhs.vk.stage,
				nullptr,
				nullptr,
				nullptr,
			};
			doInit();
		}

		PipelineShaderStageCreateInfo & operator=( PipelineShaderStageCreateInfo && rhs )
		{
			module = std::move( rhs.module );
			name = std::move( rhs.name );
			specializationInfo = std::move( rhs.specializationInfo );
			vk =
			{
				VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
				nullptr,
				rhs.vk.flags,
				rhs.vk.stage,
				*this->module,
				this->name.data(),
				nullptr,
			};
			rhs.vk =
			{
				VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
				nullptr,
				0u,
				rhs.vk.stage,
				nullptr,
				nullptr,
				nullptr,
			};
			doInit();

			return *this;
		}

		inline operator VkPipelineShaderStageCreateInfo const &()const
		{
			return vk;
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
}

#endif
