/**
*\file
*	Image.h
*\author
*	Sylvain Doremus
*/
#ifndef ___D3D11Renderer_ComputePipeline_HPP___
#define ___D3D11Renderer_ComputePipeline_HPP___
#pragma once

#include "D3D11Renderer/Shader/D3D11ShaderDesc.hpp"
#include "D3D11Renderer/Shader/D3D11ShaderModule.hpp"

#include <Ashes/Pipeline/ComputePipeline.hpp>

namespace ashes::d3d11
{
	class ComputePipeline
		: public ashes::ComputePipeline
	{
	public:
		ComputePipeline( VkDevice device
			, ashes::PipelineLayout const & layout
			, ashes::ComputePipelineCreateInfo createInfo );
		PushConstantsBuffer findPushConstantBuffer( PushConstantsDesc const & pushConstants )const;

		inline CompiledShaderModule const & getProgram()const
		{
			return m_compiled.front();
		}

		inline std::vector< PushConstantsBuffer > const & getConstantsPcbs()const
		{
			return m_constantsPcbs;
		}

		inline ProgramLayout const & getProgramLayout()const
		{
			return m_programLayout;
		}

	private:
		void doCompileProgram( VkDevice device );

	private:
		std::vector< PushConstantsBuffer > m_constantsPcbs;
		ProgramLayout m_programLayout;
		std::list< CompiledShaderModule > m_compiled;
	};
}

#endif
