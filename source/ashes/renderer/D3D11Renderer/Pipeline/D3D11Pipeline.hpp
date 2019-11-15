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
	{
	public:
		Pipeline( VkDevice device
			, VkGraphicsPipelineCreateInfo createInfo );
		Pipeline( VkDevice device
			, VkComputePipelineCreateInfo createInfo );
		~Pipeline();
		PushConstantsBuffer findPushConstantBuffer( PushConstantsDesc const & pushConstants )const;
		VkDescriptorSetLayoutArray const & getDescriptorsLayouts()const;

		inline VkPipelineInputAssemblyStateCreateInfo const & getInputAssemblyState()const
		{
			return m_inputAssemblyState;
		}

		inline bool hasVertexLayout()const
		{
			return !m_vertexAttributeDescriptions.empty()
				&& !m_vertexBindingDescriptions.empty();
		}

		inline std::vector< PushConstantsBuffer > const & getConstantsPcbs()const
		{
			return m_constantsPcbs;
		}

		inline ID3D11DepthStencilState * getDSState()const
		{
			return m_dsState;
		}

		inline ID3D11RasterizerState * getRSState()const
		{
			return m_rsState;
		}

		inline ID3D11InputLayout * getIAState()const
		{
			return m_iaState;
		}

		inline ID3D11BlendState * getBDState()const
		{
			return m_bdState;
		}

		inline VkPipelineVertexInputStateCreateInfo const & getVertexInputState()const
		{
			return m_vertexInputState;
		}

		inline bool hasViewport()const
		{
			return !m_viewports.empty();
		}

		inline bool hasScissor()const
		{
			return !m_scissors.empty();
		}

		inline float const * getBlendFactor()const
		{
			return m_colorBlendState.blendConstants;
		}

		inline UINT getSampleMask()const
		{
			return m_colorBlendState.attachmentCount == 0u
				? 0u
				: m_colorBlendState.pAttachments->colorWriteMask;
		}

		inline UINT getStencilRef()const
		{
			return bool( m_depthStencilState )
				? m_depthStencilState->front.reference
				: 0u;
		}

		inline std::vector < D3D11_VIEWPORT > const & getViewports()const
		{
			assert( !m_viewports.empty() );
			return m_viewports;
		}

		inline std::vector< RECT > const & getScissors()const
		{
			assert( !m_scissors.empty() );
			return m_scissors;
		}

		inline std::list< CompiledShaderModule > const & getShaderStages()const
		{
			return m_programModules;
		}

		inline CompiledShaderModule const & getShaderStage()const
		{
			return m_programModules.front();
		}

		inline ProgramLayout const & getProgramLayout()const
		{
			return m_programLayout;
		}

		inline VkPipelineLayout getLayout()const
		{
			return m_layout;
		}

		inline size_t getVertexInputStateHash()const
		{
			return m_vertexInputStateHash;
		}

		inline bool hasDynamicStateEnable( VkDynamicState state )const
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
