#pragma once

#include "Camera.hpp"
#include "RenderTarget.hpp"
#include "Gui.hpp"

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
	private:
		struct MouseState
		{
			renderer::UIVec2 position;
			bool left{ false };
			bool right{ false };
		};

	public:
		RenderPanel( wxWindow * parent
			, wxSize const & size
			, std::string const & appName
			, std::string const & appDesc );
		virtual ~RenderPanel();
		void initialise( renderer::Renderer const & renderer );
		void update();
		void draw();

	private:
		virtual void doInitialise( renderer::Device const & device
			, renderer::UIVec2 const & size ) = 0;
		virtual void doUpdateOverlays( Gui const & gui ) = 0;

		virtual void doUpdate()
		{
		}

		void doCleanup();
		void doCreateDevice( renderer::Renderer const & renderer );
		void doCreateSwapChain();
		void doCreateDescriptorSet();
		void doCreateRenderPass();
		void doCreateVertexBuffer();
		void doCreatePipeline();
		void doPrepareFrames();
		void doUpdateGui( std::chrono::microseconds const & duration );
		void onSize( wxSizeEvent & event );
		void onMouseLDown( wxMouseEvent & event );
		void onMouseLUp( wxMouseEvent & event );
		void onMouseLDClick( wxMouseEvent & event );
		void onMouseRDown( wxMouseEvent & event );
		void onMouseRUp( wxMouseEvent & event );
		void onMouseRDClick( wxMouseEvent & event );
		void onMouseMove( wxMouseEvent & event );

	protected:
		std::unique_ptr< RenderTarget > m_renderTarget;
		MouseState m_mouse;

	private:
		static size_t constexpr FrameSamplesCount = 1000;
		std::string m_appName;
		std::string m_appDesc;
		bool m_ready{ false };
		std::chrono::microseconds m_frameTime;
		std::array< std::chrono::microseconds, FrameSamplesCount > m_framesTimes;
		uint32_t m_frameIndex{ 0 };
		size_t m_frameCount{ 0 };
		std::vector< TexturedVertexData > m_vertexData;
		std::unique_ptr< Gui > m_gui;

		renderer::DevicePtr m_device;
		renderer::SwapChainPtr m_swapChain;
		renderer::StagingBufferPtr m_stagingBuffer;

		renderer::SamplerPtr m_sampler;
		renderer::RenderPassPtr m_renderPass;
		renderer::ShaderProgramPtr m_program;
		renderer::VertexBufferPtr< TexturedVertexData > m_vertexBuffer;
		renderer::VertexLayoutPtr m_vertexLayout;
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
