#include "DescriptorSet.hpp"

#include "DescriptorSetLayoutBinding.hpp"
#include "DescriptorSetPool.hpp"
#include "Sampler.hpp"
#include "Texture.hpp"

namespace renderer
{
	DescriptorSet::DescriptorSet( DescriptorSetPool const & pool )
		: m_descriptorSet{ pool.getPool().createDescriptorSet() }
	{
		pool.allocate( 1u );
	}

	CombinedTextureSamplerBinding DescriptorSet::createBinding( DescriptorSetLayoutBinding const & layoutBinding
		, Texture const & view
		, Sampler const & sampler )
	{
		auto binding = layoutBinding.getBinding();
		binding.setCount( 1u );
		m_descriptorSet->createBinding( binding
			, view.getImage().getView()
			, sampler.getSampler() );
		return CombinedTextureSamplerBinding
		{
			layoutBinding,
			view,
			sampler
		};
	}

	SampledTextureBinding DescriptorSet::createBinding( DescriptorSetLayoutBinding const & layoutBinding
		, Texture const & view )
	{
		auto binding = layoutBinding.getBinding();
		binding.setCount( 1u );
		m_descriptorSet->createBinding( binding
			, view.getImage().getView() );
		return SampledTextureBinding
		{
			layoutBinding,
			view
		};
	}

	void DescriptorSet::update()const
	{
		m_descriptorSet->update();
	}
}
