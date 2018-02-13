#pragma once

#include "GeometryPass.hpp"
#include "LightingPass.hpp"

#include <OpaqueRendering.hpp>

namespace vkapp
{
	class OpaqueRendering
		: public common::OpaqueRendering
	{
	public:
		OpaqueRendering( std::unique_ptr< GeometryPass > && renderer
			, common::Object const & submeshes
			, renderer::StagingBuffer & stagingBuffer
			, GeometryPassResult const & gbuffer
			, renderer::TextureViewCRefArray const & views
			, common::TextureNodePtrArray const & textureNodes
			, renderer::UniformBuffer< renderer::Mat4 > const & matrixUbo
			, renderer::UniformBuffer< common::LightsData > const & lightsUbo );
		void update( common::RenderTarget const & target )override;
		bool draw( std::chrono::nanoseconds & gpu )const override;

	private:
		renderer::UniformBuffer< renderer::Mat4 > const & m_matrixUbo;
		renderer::UniformBuffer< common::LightsData > const & m_lightsUbo;
		renderer::StagingBuffer & m_stagingBuffer;
		LightingPass m_lightingPass;
	};
}
