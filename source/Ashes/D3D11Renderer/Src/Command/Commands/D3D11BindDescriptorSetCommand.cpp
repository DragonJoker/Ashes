/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "D3D11BindDescriptorSetCommand.hpp"

#include "Buffer/D3D11Buffer.hpp"
#include "Buffer/D3D11BufferView.hpp"
#include "Descriptor/D3D11DescriptorSet.hpp"
#include "Pipeline/D3D11PipelineLayout.hpp"
#include "Image/D3D11Sampler.hpp"
#include "Image/D3D11Texture.hpp"
#include "Image/D3D11TextureView.hpp"
#include "Buffer/D3D11UniformBuffer.hpp"

#include <Descriptor/DescriptorSetLayoutBinding.hpp>

namespace d3d11_renderer
{
	namespace
	{
		ID3D11ShaderResourceView * getView( ashes::WriteDescriptorSet const & write, uint32_t index )
		{
			assert( index < write.imageInfo.size() );
			return static_cast< TextureView const & >( write.imageInfo[index].imageView.value().get() ).getShaderView();
		}
		
		ID3D11ShaderResourceView * getBufferView( ashes::WriteDescriptorSet const & write, uint32_t index )
		{
			assert( index < write.texelBufferView.size() );
			return static_cast< BufferView const & >( write.texelBufferView[index].get() ).getView();
		}
		
		ID3D11UnorderedAccessView * getImageUAV( ashes::WriteDescriptorSet const & write, uint32_t index )
		{
			assert( index < write.imageInfo.size() );
			return static_cast< TextureView const & >( write.imageInfo[index].imageView.value().get() ).getUnorderedAccessView();
		}

		ID3D11SamplerState * getSampler( ashes::WriteDescriptorSet const & write, uint32_t index )
		{
			assert( index < write.imageInfo.size() );
			return static_cast< Sampler const & >( write.imageInfo[index].sampler.value().get() ).getSampler();
		}

		ID3D11Buffer * getBuffer( ashes::WriteDescriptorSet const & write, uint32_t index )
		{
			assert( index < write.bufferInfo.size() );
			return static_cast< Buffer const & >( write.bufferInfo[index].buffer.get() ).getBuffer();
		}

		ID3D11UnorderedAccessView * getBufferUAV( ashes::WriteDescriptorSet const & write, uint32_t index )
		{
			assert( index < write.imageInfo.size() );
			return nullptr;// static_cast< Buffer const & >( write.bufferInfo[index].buffer.get() ).getUnorderedAccessView();
		}

		template< ashes::ShaderStageFlag Flag >
		struct Binder;

		template<>
		struct Binder< ashes::ShaderStageFlag::eCompute >
		{
			static void bind( ID3D11DeviceContext1 * context, UINT bindingIndex, ID3D11ShaderResourceView * image, ID3D11SamplerState * sampler )
			{
				bind( context, bindingIndex, image );
				bind( context, bindingIndex, sampler );
			}

			static void bind( ID3D11DeviceContext1 * context, UINT bindingIndex, ID3D11SamplerState * sampler )
			{
				context->CSSetSamplers( bindingIndex, 1u, &sampler );
			}

			static void bind( ID3D11DeviceContext1 * context, UINT bindingIndex, ID3D11ShaderResourceView * image )
			{
				context->CSSetShaderResources( bindingIndex, 1u, &image );
			}

			static void bind( ID3D11DeviceContext1 * context, UINT bindingIndex, ID3D11Buffer * buffer, UINT offset, UINT range )
			{
				context->CSSetConstantBuffers1( bindingIndex, 1u, &buffer, &offset, &range );
			}

			static void bind( ID3D11DeviceContext1 * context, UINT bindingIndex, ID3D11UnorderedAccessView * view )
			{
				context->CSSetUnorderedAccessViews( bindingIndex, 1u, &view, nullptr );
			}
		};

		template<>
		struct Binder< ashes::ShaderStageFlag::eVertex >
		{
			static void bind( ID3D11DeviceContext1 * context, UINT bindingIndex, ID3D11ShaderResourceView * image, ID3D11SamplerState * sampler )
			{
				bind( context, bindingIndex, image );
				bind( context, bindingIndex, sampler );
			}

			static void bind( ID3D11DeviceContext1 * context, UINT bindingIndex, ID3D11SamplerState * sampler )
			{
				context->VSSetSamplers( bindingIndex, 1u, &sampler );
			}

			static void bind( ID3D11DeviceContext1 * context, UINT bindingIndex, ID3D11ShaderResourceView * image )
			{
				context->VSSetShaderResources( bindingIndex, 1u, &image );
			}

			static void bind( ID3D11DeviceContext1 * context, UINT bindingIndex, ID3D11Buffer * buffer, UINT offset, UINT range )
			{
				context->VSSetConstantBuffers1( bindingIndex, 1u, &buffer, &offset, &range );
			}
		};

		template<>
		struct Binder< ashes::ShaderStageFlag::eGeometry >
		{
			static void bind( ID3D11DeviceContext1 * context, UINT bindingIndex, ID3D11ShaderResourceView * image, ID3D11SamplerState * sampler )
			{
				bind( context, bindingIndex, image );
				bind( context, bindingIndex, sampler );
			}

			static void bind( ID3D11DeviceContext1 * context, UINT bindingIndex, ID3D11SamplerState * sampler )
			{
				context->GSSetSamplers( bindingIndex, 1u, &sampler );
			}

			static void bind( ID3D11DeviceContext1 * context, UINT bindingIndex, ID3D11ShaderResourceView * image )
			{
				context->GSSetShaderResources( bindingIndex, 1u, &image );
			}

			static void bind( ID3D11DeviceContext1 * context, UINT bindingIndex, ID3D11Buffer * buffer, UINT offset, UINT range )
			{
				context->GSSetConstantBuffers1( bindingIndex, 1u, &buffer, &offset, &range );
			}
		};

		template<>
		struct Binder< ashes::ShaderStageFlag::eTessellationControl >
		{
			static void bind( ID3D11DeviceContext1 * context, UINT bindingIndex, ID3D11ShaderResourceView * image, ID3D11SamplerState * sampler )
			{
				bind( context, bindingIndex, image );
				bind( context, bindingIndex, sampler );
			}

			static void bind( ID3D11DeviceContext1 * context, UINT bindingIndex, ID3D11SamplerState * sampler )
			{
				context->HSSetSamplers( bindingIndex, 1u, &sampler );
			}

			static void bind( ID3D11DeviceContext1 * context, UINT bindingIndex, ID3D11ShaderResourceView * image )
			{
				context->HSSetShaderResources( bindingIndex, 1u, &image );
			}

			static void bind( ID3D11DeviceContext1 * context, UINT bindingIndex, ID3D11Buffer * buffer, UINT offset, UINT range )
			{
				context->HSSetConstantBuffers1( bindingIndex, 1u, &buffer, &offset, &range );
			}
		};

		template<>
		struct Binder< ashes::ShaderStageFlag::eTessellationEvaluation >
		{
			static void bind( ID3D11DeviceContext1 * context, UINT bindingIndex, ID3D11ShaderResourceView * image, ID3D11SamplerState * sampler )
			{
				bind( context, bindingIndex, image );
				bind( context, bindingIndex, sampler );
			}

			static void bind( ID3D11DeviceContext1 * context, UINT bindingIndex, ID3D11SamplerState * sampler )
			{
				context->DSSetSamplers( bindingIndex, 1u, &sampler );
			}

			static void bind( ID3D11DeviceContext1 * context, UINT bindingIndex, ID3D11ShaderResourceView * image )
			{
				context->DSSetShaderResources( bindingIndex, 1u, &image );
			}

			static void bind( ID3D11DeviceContext1 * context, UINT bindingIndex, ID3D11Buffer * buffer, UINT offset, UINT range )
			{
				context->DSSetConstantBuffers1( bindingIndex, 1u, &buffer, &offset, &range );
			}
		};

		template<>
		struct Binder< ashes::ShaderStageFlag::eFragment >
		{
			static void bind( ID3D11DeviceContext1 * context, UINT bindingIndex, ID3D11ShaderResourceView * image, ID3D11SamplerState * sampler )
			{
				bind( context, bindingIndex, image );
				bind( context, bindingIndex, sampler );
			}

			static void bind( ID3D11DeviceContext1 * context, UINT bindingIndex, ID3D11SamplerState * sampler )
			{
				context->PSSetSamplers( bindingIndex, 1u, &sampler );
			}

			static void bind( ID3D11DeviceContext1 * context, UINT bindingIndex, ID3D11ShaderResourceView * image )
			{
				context->PSSetShaderResources( bindingIndex, 1u, &image );
			}

			static void bind( ID3D11DeviceContext1 * context, UINT bindingIndex, ID3D11Buffer * buffer, UINT offset, UINT range )
			{
				context->PSSetConstantBuffers1( bindingIndex, 1u, &buffer, &offset, &range );
			}
		};

		template< ashes::ShaderStageFlag Flag, typename ... Params >
		void tryBindOne( ID3D11DeviceContext1 * context
			, UINT bindingIndex
			, ashes::ShaderStageFlags const & flags
			, Params ... params )
		{
			if ( checkFlag( flags, Flag ) )
			{
				Binder< Flag >::bind( context
					, bindingIndex
					, params... );
			}
		}

		template< typename ... Params >
		void tryBind( ID3D11DeviceContext1 * context
			, UINT bindingIndex
			, ashes::ShaderStageFlags const & flags
			, Params ... params )
		{
			tryBindOne< ashes::ShaderStageFlag::eCompute >( context
				, bindingIndex
				, flags
				, params... );
			tryBindOne< ashes::ShaderStageFlag::eVertex >( context
				, bindingIndex
				, flags
				, params... );
			tryBindOne< ashes::ShaderStageFlag::eGeometry >( context
				, bindingIndex
				, flags
				, params... );
			tryBindOne< ashes::ShaderStageFlag::eTessellationControl >( context
				, bindingIndex
				, flags
				, params... );
			tryBindOne< ashes::ShaderStageFlag::eTessellationEvaluation >( context
				, bindingIndex
				, flags
				, params... );
			tryBindOne< ashes::ShaderStageFlag::eFragment >( context
				, bindingIndex
				, flags
				, params... );
		}

		void bindCombinedSampler( ID3D11DeviceContext1 * context
			, WriteDescriptorSetBinding const & writeBinding )
		{
			auto & write = writeBinding.write;
			auto flags = writeBinding.binding.getShaderStageFlags();

			for ( auto i = 0u; i < write.imageInfo.size(); ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				auto view = getView( write, i );
				auto sampler = getSampler( write, i );
				tryBind( context, bindingIndex, flags, view, sampler );
			}
		}

		void bindSampler( ID3D11DeviceContext1 * context
			, WriteDescriptorSetBinding const & writeBinding )
		{
			auto & write = writeBinding.write;
			auto flags = writeBinding.binding.getShaderStageFlags();

			for ( auto i = 0u; i < write.imageInfo.size(); ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				auto sampler = getSampler( write, i );
				tryBind( context, bindingIndex, flags, sampler );
			}
		}

		void bindSampledTexture( ID3D11DeviceContext1 * context
			, WriteDescriptorSetBinding const & writeBinding )
		{
			auto & write = writeBinding.write;
			auto flags = writeBinding.binding.getShaderStageFlags();

			for ( auto i = 0u; i < write.imageInfo.size(); ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				auto view = getView( write, i );
				tryBind( context, bindingIndex, flags, view );
			}
		}

		void bindStorageImage( ID3D11DeviceContext1 * context
			, WriteDescriptorSetBinding const & writeBinding )
		{
			auto & write = writeBinding.write;
			auto flags = writeBinding.binding.getShaderStageFlags();

			for ( auto i = 0u; i < write.imageInfo.size(); ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				auto view = getImageUAV( write, i );
				tryBindOne< ashes::ShaderStageFlag::eCompute >( context
					, bindingIndex
					, flags
					, view );
			}
		}

		void bindUniformBuffer( ID3D11DeviceContext1 * context
			, WriteDescriptorSetBinding const & writeBinding )
		{
			auto & write = writeBinding.write;
			auto flags = writeBinding.binding.getShaderStageFlags();

			for ( auto i = 0u; i < write.bufferInfo.size(); ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				auto buffer = getBuffer( write, i );
				tryBind( context
					, bindingIndex
					, flags
					, buffer
					, UINT( write.bufferInfo[i].offset / 16 )
					, UINT( write.bufferInfo[i].range / 16 ) );
			}
		}

		void bindTexelBuffer( ID3D11DeviceContext1 * context
			, WriteDescriptorSetBinding const & writeBinding )
		{
			auto & write = writeBinding.write;
			auto flags = writeBinding.binding.getShaderStageFlags();

			for ( auto i = 0u; i < write.bufferInfo.size(); ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				auto view = getBufferView( write, i );
				tryBind( context
					, bindingIndex
					, flags
					, view );
			}
		}

		void bindStorageBuffer( ID3D11DeviceContext1 * context
			, WriteDescriptorSetBinding const & writeBinding )
		{
			auto & write = writeBinding.write;
			auto flags = writeBinding.binding.getShaderStageFlags();

			for ( auto i = 0u; i < write.bufferInfo.size(); ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				auto view = getBufferUAV( write, i );
				tryBindOne< ashes::ShaderStageFlag::eCompute >( context
					, bindingIndex
					, flags
					, view );
			}
		}

		void bindDynamicUniformBuffer( ID3D11DeviceContext1 * context
			, WriteDescriptorSetBinding const & writeBinding
			, uint32_t offset )
		{
			auto & write = writeBinding.write;
			auto flags = writeBinding.binding.getShaderStageFlags();

			for ( auto i = 0u; i < write.bufferInfo.size(); ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				auto buffer = getBuffer( write, i );
				tryBind( context
					, bindingIndex
					, flags
					, buffer
					, UINT( ( offset + write.bufferInfo[i].offset ) / 16 )
					, UINT( write.bufferInfo[i].range / 16 ) );
			}
		}

		void bindDynamicStorageBuffer( ID3D11DeviceContext1 * context
			, WriteDescriptorSetBinding const & writeBinding
			, uint32_t offset )
		{
			auto & write = writeBinding.write;
			auto flags = writeBinding.binding.getShaderStageFlags();

			for ( auto i = 0u; i < write.bufferInfo.size(); ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				auto view = getBufferUAV( write, i );
				tryBindOne< ashes::ShaderStageFlag::eCompute >( context
					, bindingIndex
					, flags
					, view );
			}
		}

		void bindDynamicBuffers( ID3D11DeviceContext1 * context
			, WriteDescriptorSetBindingArray const & writes
			, ashes::UInt32Array const & offsets )
		{
			for ( auto i = 0u; i < offsets.size(); ++i )
			{
				auto & writeBinding = writes[i];

				switch ( writeBinding.write.descriptorType )
				{
				case ashes::DescriptorType::eUniformBufferDynamic:
					bindDynamicUniformBuffer( context, writeBinding, offsets[i] );
					break;

				case ashes::DescriptorType::eStorageBufferDynamic:
					bindDynamicStorageBuffer( context, writeBinding, offsets[i] );
					break;

				default:
					assert( false && "Unsupported dynamic descriptor type" );
					throw std::runtime_error{ "Unsupported dynamic descriptor type" };
					break;
				}
			}
		}

		void unbindCombinedSampler( ID3D11DeviceContext1 * context
			, WriteDescriptorSetBinding const & writeBinding )
		{
			auto & write = writeBinding.write;
			auto flags = writeBinding.binding.getShaderStageFlags();

			for ( auto i = 0u; i < write.imageInfo.size(); ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				tryBind( context
					, bindingIndex
					, flags
					, ( ID3D11ShaderResourceView * )nullptr
					, ( ID3D11SamplerState * )nullptr );
			}
		}

		void unbindSampler( ID3D11DeviceContext1 * context
			, WriteDescriptorSetBinding const & writeBinding )
		{
			auto & write = writeBinding.write;
			auto flags = writeBinding.binding.getShaderStageFlags();

			for ( auto i = 0u; i < write.imageInfo.size(); ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				tryBind( context
					, bindingIndex
					, flags
					, ( ID3D11SamplerState * )nullptr );
			}
		}

		void unbindSampledTexture( ID3D11DeviceContext1 * context
			, WriteDescriptorSetBinding const & writeBinding )
		{
			auto & write = writeBinding.write;
			auto flags = writeBinding.binding.getShaderStageFlags();

			for ( auto i = 0u; i < write.imageInfo.size(); ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				tryBind( context
					, bindingIndex
					, flags
					, ( ID3D11ShaderResourceView * )nullptr );
			}
		}

		void unbindStorageImage( ID3D11DeviceContext1 * context
			, WriteDescriptorSetBinding const & writeBinding )
		{
			auto & write = writeBinding.write;
			auto flags = writeBinding.binding.getShaderStageFlags();

			for ( auto i = 0u; i < write.imageInfo.size(); ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				tryBindOne< ashes::ShaderStageFlag::eCompute >( context
					, bindingIndex
					, flags
					, ( ID3D11UnorderedAccessView * )nullptr );
			}
		}

		void unbindUniformBuffer( ID3D11DeviceContext1 * context
			, WriteDescriptorSetBinding const & writeBinding )
		{
			auto & write = writeBinding.write;
			auto flags = writeBinding.binding.getShaderStageFlags();

			for ( auto i = 0u; i < write.bufferInfo.size(); ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				tryBind( context
					, bindingIndex
					, flags
					, ( ID3D11Buffer * )nullptr
					, UINT( write.bufferInfo[i].offset / 16 )
					, UINT( write.bufferInfo[i].range / 16 ) );
			}
		}

		void unbindTexelBuffer( ID3D11DeviceContext1 * context
			, WriteDescriptorSetBinding const & writeBinding )
		{
			auto & write = writeBinding.write;
			auto flags = writeBinding.binding.getShaderStageFlags();

			for ( auto i = 0u; i < write.bufferInfo.size(); ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				tryBind( context
					, bindingIndex
					, flags
					, ( ID3D11ShaderResourceView * )nullptr );
			}
		}

		void unbindStorageBuffer( ID3D11DeviceContext1 * context
			, WriteDescriptorSetBinding const & writeBinding )
		{
			auto & write = writeBinding.write;
			auto flags = writeBinding.binding.getShaderStageFlags();

			for ( auto i = 0u; i < write.bufferInfo.size(); ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				tryBindOne< ashes::ShaderStageFlag::eCompute >( context
					, bindingIndex
					, flags
					, ( ID3D11UnorderedAccessView * )nullptr );
			}
		}

		void unbindDynamicUniformBuffer( ID3D11DeviceContext1 * context
			, WriteDescriptorSetBinding const & writeBinding
			, uint32_t offset )
		{
			auto & write = writeBinding.write;
			auto flags = writeBinding.binding.getShaderStageFlags();

			for ( auto i = 0u; i < write.bufferInfo.size(); ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				tryBind( context
					, bindingIndex
					, flags
					, ( ID3D11Buffer * )nullptr
					, UINT( write.bufferInfo[i].offset / 16 )
					, UINT( write.bufferInfo[i].range / 16 ) );
			}
		}

		void unbindDynamicStorageBuffer( ID3D11DeviceContext1 * context
			, WriteDescriptorSetBinding const & writeBinding
			, uint32_t offset )
		{
			auto & write = writeBinding.write;
			auto flags = writeBinding.binding.getShaderStageFlags();

			for ( auto i = 0u; i < write.bufferInfo.size(); ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				tryBindOne< ashes::ShaderStageFlag::eCompute >( context
					, bindingIndex
					, flags
					, ( ID3D11UnorderedAccessView * )nullptr );
			}
		}

		void unbindDynamicBuffers( ID3D11DeviceContext1 * context
			, WriteDescriptorSetBindingArray const & writes
			, ashes::UInt32Array const & offsets )
		{
			for ( auto i = 0u; i < offsets.size(); ++i )
			{
				auto & writeBinding = writes[i];

				switch ( writeBinding.write.descriptorType )
				{
				case ashes::DescriptorType::eUniformBufferDynamic:
					unbindDynamicUniformBuffer( context, writeBinding, offsets[i] );
					break;

				case ashes::DescriptorType::eStorageBufferDynamic:
					unbindDynamicStorageBuffer( context, writeBinding, offsets[i] );
					break;

				default:
					assert( false && "Unsupported dynamic descriptor type" );
					throw std::runtime_error{ "Unsupported dynamic descriptor type" };
					break;
				}
			}
		}
	}

	BindDescriptorSetCommand::BindDescriptorSetCommand( Device const & context
		, ashes::DescriptorSet const & descriptorSet
		, ashes::PipelineLayout const & layout
		, ashes::UInt32Array const & dynamicOffsets
		, ashes::PipelineBindPoint bindingPoint )
		: CommandBase{ context }
		, m_descriptorSet{ static_cast< DescriptorSet const & >( descriptorSet ) }
		, m_layout{ static_cast< PipelineLayout const & >( layout ) }
		, m_bindingPoint{ bindingPoint }
		, m_dynamicOffsets{ dynamicOffsets }
	{
		assert( m_descriptorSet.getDynamicBuffers().size() == m_dynamicOffsets.size()
			&& "Dynamic descriptors and dynamic offsets sizes must match." );
	}

	void BindDescriptorSetCommand::apply( Context const & context )const
	{
		for ( auto & write : m_descriptorSet.getCombinedTextureSamplers() )
		{
			bindCombinedSampler( context.context1, write );
		}

		for ( auto & write : m_descriptorSet.getSamplers() )
		{
			bindSampler( context.context1, write );
		}

		for ( auto & write : m_descriptorSet.getSampledTextures() )
		{
			bindSampledTexture( context.context1, write );
		}

		for ( auto & write : m_descriptorSet.getUniformBuffers() )
		{
			bindUniformBuffer( context.context1, write );
		}

		for ( auto & write : m_descriptorSet.getTexelBuffers() )
		{
			bindTexelBuffer( context.context1, write );
		}

		for ( auto & write : m_descriptorSet.getStorageBuffers() )
		{
			bindStorageBuffer( context.context1, write );
		}

		for ( auto & write : m_descriptorSet.getStorageTextures() )
		{
			bindStorageImage( context.context1, write );
		}

		bindDynamicBuffers( context.context1, m_descriptorSet.getDynamicBuffers(), m_dynamicOffsets );
	}

	void BindDescriptorSetCommand::remove( Context const & context )const
	{
		for ( auto & write : m_descriptorSet.getCombinedTextureSamplers() )
		{
			unbindCombinedSampler( context.context1, write );
		}

		for ( auto & write : m_descriptorSet.getSamplers() )
		{
			unbindSampler( context.context1, write );
		}

		for ( auto & write : m_descriptorSet.getSampledTextures() )
		{
			unbindSampledTexture( context.context1, write );
		}

		for ( auto & write : m_descriptorSet.getUniformBuffers() )
		{
			unbindUniformBuffer( context.context1, write );
		}

		for ( auto & write : m_descriptorSet.getTexelBuffers() )
		{
			unbindTexelBuffer( context.context1, write );
		}

		for ( auto & write : m_descriptorSet.getStorageBuffers() )
		{
			unbindStorageBuffer( context.context1, write );
		}

		for ( auto & write : m_descriptorSet.getStorageTextures() )
		{
			unbindStorageImage( context.context1, write );
		}

		unbindDynamicBuffers( context.context1, m_descriptorSet.getDynamicBuffers(), m_dynamicOffsets );
	}

	void BindDescriptorSetCommand::fillContext( Context & context )const
	{
		context.uavs.insert( context.uavs.end()
			, m_descriptorSet.getStorageBuffers().begin()
			, m_descriptorSet.getStorageBuffers().end() );
		context.uavs.insert( context.uavs.end()
			, m_descriptorSet.getStorageTextures().begin()
			, m_descriptorSet.getStorageTextures().end() );
	}

	CommandPtr BindDescriptorSetCommand::clone()const
	{
		return std::make_unique< BindDescriptorSetCommand >( *this );
	}
}
