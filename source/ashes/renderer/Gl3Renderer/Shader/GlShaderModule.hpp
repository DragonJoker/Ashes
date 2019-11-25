/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "renderer/Gl3Renderer/GlRendererPrerequisites.hpp"
#include "renderer/Gl3Renderer/Shader/GlShaderDesc.hpp"

namespace ashes::gl3
{
	class ShaderModule
	{
	public:
		ShaderModule( VkDevice device
			, VkShaderModuleCreateInfo createInfo );

		GLuint compile( VkPipelineShaderStageCreateInfo const & state
			, VkPipelineLayout pipelineLayout
			, VkPipelineCreateFlags createFlags
			, bool isRtot )const;

		inline ConstantsLayout const & getConstants()const
		{
			return m_constants;
		}

		inline VkDevice getDevice()const
		{
			return m_device;
		}

	private:
		VkDevice m_device;
		VkShaderModuleCreateFlags m_flags;
		UInt32Array m_code;
		mutable std::string m_source;
		mutable ConstantsLayout m_constants;
	};
}
