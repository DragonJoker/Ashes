#include "NodesRenderer.hpp"

namespace vkapp
{
	NodesRenderer::NodesRenderer( renderer::Device const & device
		, std::string const & fragmentShaderFile
		, std::vector< renderer::PixelFormat > const & formats
		, bool clearViews
		, bool opaqueNodes
		, renderer::UniformBuffer< common::SceneData > const & sceneUbo
		, renderer::UniformBuffer< common::ObjectData > const & objectUbo
		, renderer::UniformBuffer< common::LightsData > const & lightsUbo )
		: common::NodesRenderer{ device
			, fragmentShaderFile
			, formats
			, clearViews
			, opaqueNodes }
		, m_sceneUbo{ sceneUbo }
		, m_objectUbo{ objectUbo }
		, m_lightsUbo{ lightsUbo }
	{
	}

	void NodesRenderer::doFillDescriptorLayoutBindings( renderer::DescriptorSetLayoutBindingArray & bindings )
	{
		bindings.emplace_back( 1u, renderer::DescriptorType::eUniformBuffer, renderer::ShaderStageFlag::eVertex );
		bindings.emplace_back( 2u, renderer::DescriptorType::eUniformBuffer, renderer::ShaderStageFlag::eVertex );
		bindings.emplace_back( 3u, renderer::DescriptorType::eUniformBuffer, renderer::ShaderStageFlag::eFragment );
	}

	void NodesRenderer::doFillDescriptorSet( renderer::DescriptorSetLayout & descriptorLayout
		, renderer::DescriptorSet & descriptorSet )
	{
		descriptorSet.createBinding( descriptorLayout.getBinding( 1u )
			, m_sceneUbo
			, 0u
			, 1u );
		descriptorSet.createBinding( descriptorLayout.getBinding( 2u )
			, m_objectUbo
			, 0u
			, 1u );
		descriptorSet.createBinding( descriptorLayout.getBinding( 3u )
			, m_lightsUbo
			, 0u
			, 1u );
	}
}
