/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_PipelineShaderStageCreateInfo_HPP___
#define ___Ashes_PipelineShaderStageCreateInfo_HPP___
#pragma once

#include "ashespp/AshesPPPrerequisites.hpp"

#include "ashespp/Shader/ShaderModule.hpp"

namespace ashes
{
	struct PipelineShaderStageCreateInfo
	{
		PipelineShaderStageCreateInfo( PipelineShaderStageCreateInfo const & ) = delete;
		PipelineShaderStageCreateInfo & operator=( PipelineShaderStageCreateInfo const & ) = delete;

		PipelineShaderStageCreateInfo( VkPipelineShaderStageCreateFlags flags
			, VkShaderStageFlagBits stage
			, ShaderModulePtr module
			, std::string name
			, Optional< VkSpecializationInfo > specializationInfo )
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
		Optional< VkSpecializationInfo > specializationInfo;
		VkSpecializationMapEntryArray specializationEntries;
		ByteArray specializationData;

	private:
		void doInit()
		{
			if ( bool( specializationInfo ) )
			{
				specializationInfo = deepCopy( specializationInfo.value()
					, specializationEntries
					, specializationData );
				vk.pSpecializationInfo = &specializationInfo.value();
			}
		}

	private:
		VkPipelineShaderStageCreateInfo vk;
	};

	using PipelineShaderStageCreateInfoArray = std::vector< PipelineShaderStageCreateInfo >;
}

#endif
