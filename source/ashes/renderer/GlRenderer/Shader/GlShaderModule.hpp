/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "renderer/GlRenderer/GlRendererPrerequisites.hpp"
#include "renderer/GlRenderer/Shader/GlShaderDesc.hpp"

namespace ashes::gl
{
	bool checkLinkErrors( ContextLock const & context
		, VkPipeline pipeline
		, GLuint programName
		, int modulesCount
		, std::string const & from
		, std::string const & source = std::string{} );

	class ShaderModule
		: public AutoIdIcdObject< ShaderModule >
	{
	public:
		ShaderModule( VkAllocationCallbacks const * allocInfo
			, VkDevice device
			, VkShaderModuleCreateInfo const & createInfo );
		~ShaderModule()noexcept;

		VkResult compile( VkPipeline pipeline
			, VkPipelineShaderStageCreateInfo const * previousState
			, VkPipelineShaderStageCreateInfo const & currentState
			, VkPipelineLayout pipelineLayout
			, VkPipelineCreateFlags createFlags
			, bool invertY
			, ShaderDesc & result );

		ConstantsLayout const & getConstants()const
		{
			return m_constants;
		}

		VkDevice getDevice()const
		{
			return m_device;
		}

	private:
		ShaderDesc compileCombined( ContextLock const & context
			, VkPipelineShaderStageCreateInfo const & state );
		ShaderDesc compileSeparate( ContextLock const & context
			, VkPipeline pipeline
			, VkPipelineShaderStageCreateInfo const & state
			, bool isGlsl );

	private:
		VkDevice m_device;
		UInt32Array m_code;
		mutable std::string m_source;
		mutable ConstantsLayout m_constants;
	};
}
