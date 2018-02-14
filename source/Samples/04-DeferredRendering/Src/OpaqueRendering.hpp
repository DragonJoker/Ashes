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
			, common::Scene const & scene
			, renderer::StagingBuffer & stagingBuffer
			, GeometryPassResult const & gbuffer
			, renderer::TextureViewCRefArray const & views
			, common::TextureNodePtrArray const & textureNodes
			, renderer::UniformBuffer< common::SceneData > const & sceneUbo
			, renderer::UniformBuffer< common::LightsData > const & lightsUbo );
		void update( common::RenderTarget const & target )override;
		bool draw( std::chrono::nanoseconds & gpu )const override;

	private:
		renderer::UniformBuffer< common::SceneData > const & m_sceneUbo;
		renderer::UniformBuffer< common::LightsData > const & m_lightsUbo;
		renderer::StagingBuffer & m_stagingBuffer;
		LightingPass m_lightingPass;
	};
}
