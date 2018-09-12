/**
*\file
*	Texture.h
*\author
*	Sylvain Doremus
*/
#ifndef ___D3D11Renderer_ComputePipeline_HPP___
#define ___D3D11Renderer_ComputePipeline_HPP___
#pragma once

#include "D3D11RendererPrerequisites.hpp"

#include <Pipeline/ComputePipeline.hpp>

namespace d3d11_renderer
{
	/**
	*\brief
	*	Un pipeline de rendu.
	*/
	class ComputePipeline
		: public renderer::ComputePipeline
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le LogicalDevice parent.
		*\param[in] layout
		*	Le layout du pipeline.
		*\param[in] program
		*	Les informations de creation du pipeline.
		*/
		ComputePipeline( Device const & device
			, renderer::PipelineLayout const & layout
			, renderer::ComputePipelineCreateInfo && createInfo );
		/**
		*\~french
		*\brief
		*	Destructeur.
		*\~english
		*\brief
		*	Destructor.
		*/
		~ComputePipeline();

		inline renderer::ShaderStageState const & getProgram()const
		{
			return m_createInfo.stage;
		}

		inline std::vector< renderer::PushConstantsBufferPtr > const & getConstantsPcbs()const
		{
			return m_constantsPcbs;
		}

	private:
		void doCompileProgram( Device const & device );

	private:
		std::vector< renderer::PushConstantsBufferPtr > m_constantsPcbs;
	};
}

#endif
