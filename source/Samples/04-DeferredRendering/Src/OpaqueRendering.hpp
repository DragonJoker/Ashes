#pragma once

#include "GeometryPass.hpp"
#include "LightingPass.hpp"

namespace vkapp
{
	class OpaqueRendering
	{
	public:
		OpaqueRendering( renderer::Device const & device
			, common::Object const & submeshes
			, renderer::UniformBuffer< renderer::Mat4 > const & matrixUbo
			, renderer::UniformBuffer< renderer::Mat4 > const & objectUbo
			, renderer::UniformBuffer< common::LightsData > const & lightsUbo
			, renderer::StagingBuffer & stagingBuffer
			, renderer::TextureView const & colourView
			, renderer::TextureView const & depthView
			, common::TextureNodePtrArray const & textureNodes );
		void update( renderer::TextureView const & colourView
			, renderer::TextureView const & depthView );
		void draw()const;

	private:
		renderer::Device const & m_device;
		renderer::UniformBuffer< renderer::Mat4 > const & m_matrixUbo;
		renderer::UniformBuffer< renderer::Mat4 > const & m_objectUbo;
		renderer::UniformBuffer< common::LightsData > const & m_lightsUbo;
		renderer::TextureView const * m_colourView{ nullptr };
		renderer::TextureView const * m_depthView{ nullptr };
		GeometryPass m_geometryPass;
		LightingPass m_lightingPass;
	};
}
