/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_PipelineShaderStageCreateInfo_HPP___
#define ___Ashes_PipelineShaderStageCreateInfo_HPP___
#pragma once

#include "AshesPP/AshesPPPrerequisites.hpp"

#include "AshesPP/Shader/ShaderModule.hpp"

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
				bool( this->specializationInfo ) ? &this->specializationInfo.value() : nullptr,
			}
		{
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
				bool( this->specializationInfo ) ? &this->specializationInfo.value() : nullptr,
			}
		{
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
				bool( this->specializationInfo ) ? &this->specializationInfo.value() : nullptr,
			};

			return *this;
		}

		inline operator VkPipelineShaderStageCreateInfo const &()const
		{
			return vk;
		}

		ShaderModulePtr module;
		std::string name;
		Optional< VkSpecializationInfo > specializationInfo;

	private:
		VkPipelineShaderStageCreateInfo vk;
	};

	using PipelineShaderStageCreateInfoArray = std::vector< PipelineShaderStageCreateInfo >;
}

#endif
