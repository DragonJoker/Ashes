#pragma once

#include "Prerequisites.hpp"

#include <Core/Connection.hpp>
#include <Core/Device.hpp>
#include <Pipeline/Pipeline.hpp>
#include <Pipeline/PipelineLayout.hpp>
#include <Image/Sampler.hpp>
#include <Core/SwapChain.hpp>

#include <Utils/UtilsSignal.hpp>

#include <AssimpLoader.hpp>

#include <wx/panel.h>

#include <array>

namespace vkapp
{
	class RenderPanel
		: public wxPanel
	{
	public:
		RenderPanel( wxWindow * parent
			, wxSize const & size
			, renderer::Renderer const & renderer );
		~RenderPanel();

	private:
		void doCleanup();
		void doUpdateProjection();
		void doCreateDevice( renderer::Renderer const & renderer );
		void doCreateSwapChain();
		void doCreateUniformBuffer();
		void doCreateStagingBuffer();
		void doInitialiseObject();
		void doCreateTextures();
		void doInitialiseLights();
		void doCreateOffscreenDescriptorSet();
		void doCreateOffscreenRenderPass();
		void doCreateFrameBuffer();
		void doCreateOffscreenProgram();
		void doPrepareOffscreenFrame();
		void doCreateMainDescriptorSet();
		void doCreateMainRenderPass();
		void doCreateMainVertexBuffer();
		void doCreateMainPipeline();
		void doPrepareMainFrames();
		void doUpdate();
		void doDraw();
		void doResetSwapChain();
		void onTimer( wxTimerEvent & event );
		void onSize( wxSizeEvent & event );

	private:
		wxTimer * m_timer{ nullptr };
		renderer::Mat4 m_rotate;

		renderer::DevicePtr m_device;
		renderer::SwapChainPtr m_swapChain;
		renderer::StagingBufferPtr m_stagingBuffer;
		renderer::SamplerPtr m_sampler;
		renderer::TexturePtr m_renderTargetColour;
		renderer::TextureViewPtr m_renderTargetColourView;
		renderer::TexturePtr m_renderTargetDepth;
		renderer::TextureViewPtr m_renderTargetDepthView;
		renderer::FrameBufferPtr m_frameBuffer;
		renderer::UniformBufferPtr< renderer::Mat4 > m_matrixUbo;
		renderer::UniformBufferPtr< renderer::Mat4 > m_objectUbo;
		renderer::UniformBufferPtr< common::MaterialData > m_materialUbo;
		renderer::UniformBufferPtr< common::LightsData > m_lightsUbo;
		renderer::CommandBufferPtr m_updateCommandBuffer;
		common::ImagePtrArray m_images;
		common::Object m_object;
		common::TextureNodePtrArray m_textureNodes;
		uint32_t m_nodesCount;

		renderer::CommandBufferPtr m_commandBuffer;
		renderer::RenderPassPtr m_offscreenRenderPass;
		renderer::ShaderProgramPtr m_offscreenProgram;
		common::ObjectNodes m_opaqueObject;
		common::ObjectNodes m_alphaBlendedObject;
		common::SubmeshNodes m_submeshNodes;
		renderer::DescriptorSetLayoutPtr m_offscreenDescriptorLayout;
		renderer::DescriptorSetPoolPtr m_offscreenDescriptorPool;

		renderer::RenderPassPtr m_mainRenderPass;
		renderer::ShaderProgramPtr m_mainProgram;
		renderer::PipelineLayoutPtr m_mainPipelineLayout;
		renderer::PipelinePtr m_mainPipeline;
		renderer::VertexBufferPtr< TexturedVertexData > m_mainVertexBuffer;
		renderer::VertexLayoutPtr m_mainVertexLayout;
		renderer::GeometryBuffersPtr m_mainGeometryBuffers;
		renderer::DescriptorSetLayoutPtr m_mainDescriptorLayout;
		renderer::DescriptorSetPoolPtr m_mainDescriptorPool;
		renderer::DescriptorSetPtr m_mainDescriptorSet;
		std::vector< TexturedVertexData > m_mainVertexData;

		std::vector< renderer::FrameBufferPtr > m_frameBuffers;
		std::vector< renderer::CommandBufferPtr > m_commandBuffers;
		renderer::SignalConnection< renderer::SwapChain::OnReset > m_swapChainReset;
	};
}
