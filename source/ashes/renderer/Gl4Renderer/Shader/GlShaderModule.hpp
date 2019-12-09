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
		: public AutoIdIcdObject< ShaderModule >
	{
	public:
		ShaderModule( VkDevice device
			, VkShaderModuleCreateInfo createInfo );

		ShaderDesc compile( VkPipelineShaderStageCreateInfo const & state
			, VkPipelineLayout pipelineLayout
			, VkPipelineCreateFlags createFlags
			, bool invertY );

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
		std::string m_source;
		ConstantsLayout m_constants;
	};
}
