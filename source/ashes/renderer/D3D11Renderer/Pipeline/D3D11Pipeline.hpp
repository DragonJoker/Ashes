/**
*\file
*	Image.h
*\author
*	Sylvain Doremus
*/
#ifndef ___D3D11Renderer_Pipeline_HPP___
#define ___D3D11Renderer_Pipeline_HPP___
#pragma once

#include "renderer/D3D11Renderer/Shader/D3D11ShaderDesc.hpp"
#include "renderer/D3D11Renderer/Shader/D3D11ShaderModule.hpp"
#include "renderer/D3D11Renderer/Pipeline/D3D11DynamicStates.hpp"

namespace ashes::d3d11
{
	class Pipeline
		: public NonCopyable
	{
		friend class PipelineLayout;

	public:
		Pipeline( VkDevice device
			, VkGraphicsPipelineCreateInfo createInfo );
		Pipeline( VkDevice device
			, VkComputePipelineCreateInfo createInfo );
		~Pipeline()noexcept;
		PushConstantsBuffer findPushConstantBuffer( PushConstantsDesc const & pushConstants )const;
		VkDescriptorSetLayoutArray const & getDescriptorsLayouts()const;
		void update();

		VkPipelineInputAssemblyStateCreateInfo const & getInputAssemblyState()const
		{
			return m_inputAssemblyState;
		}

		bool hasVertexLayout()const
		{
			return !m_vertexAttributeDescriptions.empty()
				&& !m_vertexBindingDescriptions.empty();
		}

		std::vector< PushConstantsBuffer > const & getConstantsPcbs()const
		{
			return m_constantsPcbs;
		}

		ID3D11DepthStencilState * getDSState()const
		{
			return m_dsState;
		}

		ID3D11RasterizerState * getRSState()const
		{
			return m_rsState;
		}

		ID3D11InputLayout * getIAState()const
		{
			return m_iaState;
		}

		ID3D11BlendState * getBDState()const
		{
			return m_bdState;
		}

		VkPipelineVertexInputStateCreateInfo const & getVertexInputState()const
		{
			return m_vertexInputState;
		}

		bool hasViewport()const
		{
			return !m_viewports.empty();
		}

		bool hasScissor()const
		{
			return !m_scissors.empty();
		}

		float const * getBlendFactor()const
		{
			return m_colorBlendState.blendConstants;
		}

		UINT getSampleMask()const
		{
			return m_colorBlendState.attachmentCount == 0u
				? 0u
				: m_colorBlendState.pAttachments->colorWriteMask;
		}

		UINT getStencilRef()const
		{
			return bool( m_depthStencilState )
				? m_depthStencilState->front.reference
				: 0u;
		}

		std::vector < D3D11_VIEWPORT > const & getViewports()const
		{
			assert( !m_viewports.empty() );
			return m_viewports;
		}

		std::vector< RECT > const & getScissors()const
		{
			assert( !m_scissors.empty() );
			return m_scissors;
		}

		std::list< CompiledShaderModule > const & getShaderStages()const
		{
			return m_programModules;
		}

		CompiledShaderModule const & getShaderStage()const
		{
			return m_programModules.front();
		}

		ProgramLayout const & getProgramLayout()const
		{
			return m_programLayout;
		}

		VkPipelineLayout getLayout()const
		{
			return m_layout;
		}

		VkDevice getDevice()const
		{
			return m_device;
		}

		size_t getVertexInputStateHash()const
		{
			return m_vertexInputStateHash;
		}

		DynamicStates & getDynamicStates()
		{
			return m_dynamicStates;
		}

		bool hasDynamicStateEnable( VkDynamicState state )const
		{
			return m_dynamicStates.hasDynamicStateEnable( state );
		}

	private:
		void doCreateBlendState( VkDevice device );
		void doCreateRasterizerState( VkDevice device );
		void doCreateDepthStencilState( VkDevice device );
		void doCompileProgram( VkDevice device
			, VkPipelineShaderStageCreateInfoArray const & stages
			, VkPipelineCreateFlags createFlags );
		void doCreateInputLayout( VkDevice device );

	private:
		VkDevice m_device;
		VkPipelineLayout m_layout;
		//
		VkVertexInputBindingDescriptionArray m_vertexBindingDescriptions;
		VkVertexInputAttributeDescriptionArray m_vertexAttributeDescriptions;
		VkPipelineVertexInputStateCreateInfo m_vertexInputState;
		//
		VkPipelineInputAssemblyStateCreateInfo m_inputAssemblyState;
		//
		VkViewportArray m_stateViewports;
		VkScissorArray m_stateScissors;
		VkPipelineViewportStateCreateInfo m_viewportState;
		//
		VkPipelineRasterizationStateCreateInfo m_rasterizationState;
		//
		VkPipelineMultisampleStateCreateInfo m_multisampleState;
		//
		Optional< VkPipelineDepthStencilStateCreateInfo > m_depthStencilState;
		//
		VkPipelineColorBlendAttachmentStateArray m_colorBlendStateAttachments;
		VkPipelineColorBlendStateCreateInfo m_colorBlendState;
		//
		DynamicStates m_dynamicStates;
		//
		ID3D11DepthStencilState * m_dsState{ nullptr };
		ID3D11RasterizerState * m_rsState{ nullptr };
		ID3D11InputLayout * m_iaState{ nullptr };
		ID3D11BlendState * m_bdState{ nullptr };
		std::vector< RECT > m_scissors;
		std::vector< D3D11_VIEWPORT > m_viewports;
		std::vector< PushConstantsBuffer > m_constantsPcbs;
		ProgramLayout m_programLayout;
		std::list< CompiledShaderModule > m_programModules;
		size_t m_vertexInputStateHash;
	};
}

#endif
