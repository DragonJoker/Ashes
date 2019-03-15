#include "NodesRenderer.hpp"

namespace vkapp
{
	NodesRenderer::NodesRenderer( ashes::Device const & device
		, ashes::CommandPool const & commandPool
		, ashes::Queue const & transferQueue
		, std::string const & fragmentShaderFile
		, std::vector< ashes::Format > const & formats
		, bool clearViews
		, bool opaqueNodes
		, ashes::UniformBuffer< common::SceneData > const & sceneUbo
		, ashes::UniformBuffer< common::ObjectData > const & objectUbo
		, ashes::UniformBuffer< common::LightsData > const & lightsUbo )
		: common::NodesRenderer{ device
			, commandPool
			, transferQueue
			, fragmentShaderFile
			, formats
			, clearViews
			, opaqueNodes }
		, m_sceneUbo{ sceneUbo }
		, m_objectUbo{ objectUbo }
		, m_lightsUbo{ lightsUbo }
	{
	}

	void NodesRenderer::doFillObjectDescriptorLayoutBindings( ashes::DescriptorSetLayoutBindingArray & bindings )
	{
		bindings.emplace_back( 1u, ashes::DescriptorType::eUniformBuffer, ashes::ShaderStageFlag::eVertex );
		bindings.emplace_back( 2u, ashes::DescriptorType::eUniformBuffer, ashes::ShaderStageFlag::eVertex );
		bindings.emplace_back( 3u, ashes::DescriptorType::eUniformBuffer, ashes::ShaderStageFlag::eFragment );
	}

	void NodesRenderer::doFillObjectDescriptorSet( ashes::DescriptorSetLayout & descriptorLayout
		, ashes::DescriptorSet & descriptorSet )
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
