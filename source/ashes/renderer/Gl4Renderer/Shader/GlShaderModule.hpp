/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "renderer/Gl4Renderer/GlRendererPrerequisites.hpp"
#include "renderer/Gl4Renderer/Shader/GlShaderDesc.hpp"

namespace ashes::gl4
{
	class ShaderModule
	{
	public:
		ShaderModule( VkDevice device
			, VkShaderModuleCreateInfo createInfo );

		ShaderDesc compile( VkPipelineShaderStageCreateInfo const & state
			, VkPipelineLayout pipelineLayout
			, bool isRtot );

		inline ConstantsLayout const & getConstants()const
		{
			return m_constants;
		}

	private:
		VkDevice m_device;
		VkShaderModuleCreateFlags m_flags;
		UInt32Array m_code;
		std::string m_source;
		ConstantsLayout m_constants;
	};
}
