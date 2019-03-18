#pragma once

#include "Prerequisites.hpp"

#include <Buffer/StagingBuffer.hpp>
#include <Core/Surface.hpp>
#include <Core/Device.hpp>
#include <Pipeline/Pipeline.hpp>
#include <Pipeline/PipelineLayout.hpp>
#include <Image/Sampler.hpp>
#include <Core/SwapChain.hpp>

#include <Utils/Signal.hpp>

#include <ObjLoader.hpp>

#include <wx/panel.h>

#include <array>

namespace vkapp
{
	class EquirectangularToCube
	{
	public:
		EquirectangularToCube( std::string const & filePath
			, ashes::Device const & device
			, ashes::Queue const & queue
			, ashes::CommandPool const & commandPool
			, ashes::Texture & texture );

		void render();
		void render( ashes::CommandBuffer & commandBuffer );

	private:
		struct FacePipeline
		{
			ashes::TextureViewPtr view;
			ashes::FrameBufferPtr frameBuffer;
			ashes::PipelinePtr pipeline;
			ashes::DescriptorSetPtr descriptorSet;
		};

		ashes::Device const & m_device;
		ashes::Queue const & m_queue;
		ashes::CommandBufferPtr m_commandBuffer;
		common::ImageData m_image;
		ashes::StagingBuffer m_stagingBuffer;
		ashes::TexturePtr m_texture;
		ashes::TextureViewPtr m_view;
		ashes::SamplerPtr m_sampler;
		ashes::UniformBufferPtr< utils::Mat4 > m_matrixUbo;
		ashes::VertexBufferPtr< VertexData > m_vertexBuffer;
		ashes::VertexLayoutPtr m_vertexLayout;
		ashes::DescriptorSetLayoutPtr m_descriptorLayout;
		ashes::DescriptorSetPoolPtr m_descriptorPool;
		ashes::PipelineLayoutPtr m_pipelineLayout;
		ashes::RenderPassPtr m_renderPass;
		std::array< FacePipeline, 6u > m_faces;
	};
}
