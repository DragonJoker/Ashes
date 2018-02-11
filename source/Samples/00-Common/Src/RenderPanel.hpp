#pragma once

#include "RenderTarget.hpp"

#include <Core/Connection.hpp>
#include <Core/Device.hpp>
#include <Pipeline/Pipeline.hpp>
#include <Pipeline/PipelineLayout.hpp>
#include <Image/Sampler.hpp>
#include <Core/SwapChain.hpp>

#include <Utils/UtilsSignal.hpp>

#include <wx/panel.h>

#include <array>

namespace common
{
	class RenderPanel
		: public wxPanel
	{
	public:
		RenderPanel( wxWindow * parent
			, wxSize const & size
			, std::string const & appName );
		~RenderPanel();
		void initialise( renderer::Renderer const & renderer );
		void update();
		void draw( std::chrono::microseconds & cpu
			, std::chrono::microseconds & gpu );
		void resize( wxSize const & size );

	private:
		virtual void doInitialise() = 0;
		void doCleanup();
		void doCreateDevice( renderer::Renderer const & renderer );
		void doCreateSwapChain();
		void doInitialiseLights();
		void doCreateDescriptorSet();
		void doCreateRenderPass();
		void doCreateVertexBuffer();
		void doCreatePipeline();
		void doPrepareFrames();

	private:
		std::string const & m_appName;
		std::vector< TexturedVertexData > m_vertexData;

		renderer::DevicePtr m_device;
		renderer::SwapChainPtr m_swapChain;
		renderer::StagingBufferPtr m_stagingBuffer;
		renderer::UniformBufferPtr< common::LightsData > m_lightsUbo;

		std::unique_ptr< RenderTarget > m_renderTarget;
		renderer::SamplerPtr m_sampler;

		renderer::RenderPassPtr m_renderPass;
		renderer::ShaderProgramPtr m_program;
		renderer::VertexBufferPtr< TexturedVertexData > m_vertexBuffer;
		renderer::VertexLayoutPtr m_vertexLayout;
		renderer::GeometryBuffersPtr m_geometryBuffers;
		renderer::DescriptorSetLayoutPtr m_descriptorLayout;
		renderer::DescriptorSetPoolPtr m_descriptorPool;
		renderer::DescriptorSetPtr m_descriptorSet;
		renderer::PipelineLayoutPtr m_pipelineLayout;
		renderer::PipelinePtr m_pipeline;

		std::vector< renderer::FrameBufferPtr > m_frameBuffers;
		std::vector< renderer::CommandBufferPtr > m_commandBuffers;
		renderer::SignalConnection< renderer::SwapChain::OnReset > m_swapChainReset;
	};
}
