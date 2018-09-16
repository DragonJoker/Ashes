/**
*\file
*	Texture.h
*\author
*	Sylvain Doremus
*/
#ifndef ___D3D11Renderer_ComputePipeline_HPP___
#define ___D3D11Renderer_ComputePipeline_HPP___
#pragma once

#include "Shader/D3D11ShaderDesc.hpp"

#include <Pipeline/ComputePipeline.hpp>

namespace d3d11_renderer
{
	class ComputePipeline
		: public ashes::ComputePipeline
	{
	public:
		ComputePipeline( Device const & device
			, ashes::PipelineLayout const & layout
			, ashes::ComputePipelineCreateInfo && createInfo );
		PushConstantsBuffer findPushConstantBuffer( PushConstantsDesc const & pushConstants )const;

		inline ashes::ShaderStageState const & getProgram()const
		{
			return m_createInfo.stage;
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
		void doCompileProgram( Device const & device );

	private:
		std::vector< PushConstantsBuffer > m_constantsPcbs;
		ProgramLayout m_programLayout;
	};
}

#endif
