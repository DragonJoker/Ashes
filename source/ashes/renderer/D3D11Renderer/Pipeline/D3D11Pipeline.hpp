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

		inline VkPipelineInputAssemblyStateCreateInfo const & getInputAssemblyState()const
		{
			assert( m_graphicsCreateInfo.pInputAssemblyState );
			return *m_graphicsCreateInfo.pInputAssemblyState;
		}

		inline bool hasVertexLayout()const
		{
			return m_graphicsCreateInfo.pVertexInputState
				&& m_graphicsCreateInfo.pVertexInputState->vertexBindingDescriptionCount > 0
				&& m_graphicsCreateInfo.pVertexInputState->vertexAttributeDescriptionCount > 0;
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
			assert( m_graphicsCreateInfo.pVertexInputState );
			return *m_graphicsCreateInfo.pVertexInputState;
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
			assert( m_graphicsCreateInfo.pColorBlendState );
			return m_graphicsCreateInfo.pColorBlendState->blendConstants;
		}

		inline UINT getSampleMask()const
		{
			assert( m_graphicsCreateInfo.pColorBlendState );
			return m_graphicsCreateInfo.pColorBlendState->attachmentCount == 0u
				? 0u
				: m_graphicsCreateInfo.pColorBlendState->pAttachments->colorWriteMask;
		}

		inline UINT getStencilRef()const
		{
			return m_graphicsCreateInfo.pDepthStencilState
				? m_graphicsCreateInfo.pDepthStencilState->front.reference
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
			return bool( m_graphicsCreateInfo.pDynamicState )
				&& ( m_graphicsCreateInfo.pDynamicState->pDynamicStates + m_graphicsCreateInfo.pDynamicState->dynamicStateCount )
					!= std::find( m_graphicsCreateInfo.pDynamicState->pDynamicStates
						, m_graphicsCreateInfo.pDynamicState->pDynamicStates + m_graphicsCreateInfo.pDynamicState->dynamicStateCount
						, state );
		}

	private:
		void doCreateBlendState( VkDevice device );
		void doCreateRasterizerState( VkDevice device );
		void doCreateDepthStencilState( VkDevice device );
		void doCompileProgram( VkDevice device
			, VkPipelineShaderStageCreateInfoArray const & stages );
		void doCreateInputLayout( VkDevice device );

	private:
		VkDevice m_device;
		VkPipelineLayout m_layout;
		VkGraphicsPipelineCreateInfo m_graphicsCreateInfo;
		VkComputePipelineCreateInfo m_computeCreateInfo;
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
