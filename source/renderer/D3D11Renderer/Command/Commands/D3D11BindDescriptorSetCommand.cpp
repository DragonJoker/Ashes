/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11BindDescriptorSetCommand.hpp"

#include "Buffer/D3D11Buffer.hpp"
#include "Buffer/D3D11BufferView.hpp"
#include "Descriptor/D3D11DescriptorSet.hpp"
#include "Pipeline/D3D11PipelineLayout.hpp"
#include "Image/D3D11Sampler.hpp"
#include "Image/D3D11Image.hpp"
#include "Image/D3D11ImageView.hpp"

#include <Ashes/Buffer/UniformBuffer.hpp>
#include <Ashes/Descriptor/DescriptorSetLayoutBinding.hpp>

namespace ashes::d3d11
{
	namespace
	{
		ID3D11ShaderResourceView * getView( VkWriteDescriptorSet const & write, uint32_t index )
		{
			assert( index < write.imageInfo.size() );
			return static_cast< ImageView const & >( write.imageInfo[index].imageView.value().get() ).getShaderView();
		}
		
		ID3D11ShaderResourceView * getBufferView( VkWriteDescriptorSet const & write, uint32_t index )
		{
			assert( index < write.texelBufferView.size() );
			return static_cast< BufferView const & >( write.texelBufferView[index].get() ).getView();
		}
		
		ID3D11UnorderedAccessView * getImageUAV( VkWriteDescriptorSet const & write, uint32_t index )
		{
			assert( index < write.imageInfo.size() );
			return static_cast< ImageView const & >( write.imageInfo[index].imageView.value().get() ).getUnorderedAccessView();
		}

		ID3D11SamplerState * getSampler( VkWriteDescriptorSet const & write, uint32_t index )
		{
			assert( index < write.imageInfo.size() );
			return static_cast< Sampler const & >( write.imageInfo[index].sampler.value().get() ).getSampler();
		}

		ID3D11Buffer * getBuffer( VkWriteDescriptorSet const & write, uint32_t index )
		{
			assert( index < write.bufferInfo.size() );
			return static_cast< Buffer const & >( write.bufferInfo[index].buffer.get() ).getBuffer();
		}

		ID3D11UnorderedAccessView * getBufferUAV( VkWriteDescriptorSet const & write, uint32_t index )
		{
			assert( index < write.bufferInfo.size() );
			return static_cast< Buffer const & >( write.bufferInfo[index].buffer.get() ).getUnorderedAccessView();
		}

		template< VkShaderStageFlagBits Flag, bool Supports11_1 >
		struct Binder;

		template<>
		struct Binder< VK_SHADER_STAGE_COMPUTE_BIT, true >
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
		struct Binder< VK_SHADER_STAGE_VERTEX_BIT, true >
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
		struct Binder< VK_SHADER_STAGE_GEOMETRY_BIT, true >
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
		struct Binder< VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT, true >
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
		struct Binder< VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT, true >
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
		struct Binder< VK_SHADER_STAGE_FRAGMENT_BIT, true >
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

		inline void checkOffset( uint32_t offset )
		{
#ifndef NDEBUG
			if ( offset != 0u )
			{
				ashes::Logger::logWarning( "Binding buffer range not supported by this driver" );
			}
#endif
		}

		template<>
		struct Binder< VK_SHADER_STAGE_COMPUTE_BIT, false >
		{
			static void bind( ID3D11DeviceContext * context, UINT bindingIndex, ID3D11ShaderResourceView * image, ID3D11SamplerState * sampler )
			{
				bind( context, bindingIndex, image );
				bind( context, bindingIndex, sampler );
			}

			static void bind( ID3D11DeviceContext * context, UINT bindingIndex, ID3D11SamplerState * sampler )
			{
				context->CSSetSamplers( bindingIndex, 1u, &sampler );
			}

			static void bind( ID3D11DeviceContext * context, UINT bindingIndex, ID3D11ShaderResourceView * image )
			{
				context->CSSetShaderResources( bindingIndex, 1u, &image );
			}

			static void bind( ID3D11DeviceContext * context, UINT bindingIndex, ID3D11Buffer * buffer, UINT offset, UINT range )
			{
				checkOffset( offset );
				context->CSSetConstantBuffers( bindingIndex, 1u, &buffer );
			}

			static void bind( ID3D11DeviceContext * context, UINT bindingIndex, ID3D11UnorderedAccessView * view )
			{
				context->CSSetUnorderedAccessViews( bindingIndex, 1u, &view, nullptr );
			}
		};

		template<>
		struct Binder< VK_SHADER_STAGE_VERTEX_BIT, false >
		{
			static void bind( ID3D11DeviceContext * context, UINT bindingIndex, ID3D11ShaderResourceView * image, ID3D11SamplerState * sampler )
			{
				bind( context, bindingIndex, image );
				bind( context, bindingIndex, sampler );
			}

			static void bind( ID3D11DeviceContext * context, UINT bindingIndex, ID3D11SamplerState * sampler )
			{
				context->VSSetSamplers( bindingIndex, 1u, &sampler );
			}

			static void bind( ID3D11DeviceContext * context, UINT bindingIndex, ID3D11ShaderResourceView * image )
			{
				context->VSSetShaderResources( bindingIndex, 1u, &image );
			}

			static void bind( ID3D11DeviceContext * context, UINT bindingIndex, ID3D11Buffer * buffer, UINT offset, UINT range )
			{
				checkOffset( offset );
				context->VSSetConstantBuffers( bindingIndex, 1u, &buffer );
			}
		};

		template<>
		struct Binder< VK_SHADER_STAGE_GEOMETRY_BIT, false >
		{
			static void bind( ID3D11DeviceContext * context, UINT bindingIndex, ID3D11ShaderResourceView * image, ID3D11SamplerState * sampler )
			{
				bind( context, bindingIndex, image );
				bind( context, bindingIndex, sampler );
			}

			static void bind( ID3D11DeviceContext * context, UINT bindingIndex, ID3D11SamplerState * sampler )
			{
				context->GSSetSamplers( bindingIndex, 1u, &sampler );
			}

			static void bind( ID3D11DeviceContext * context, UINT bindingIndex, ID3D11ShaderResourceView * image )
			{
				context->GSSetShaderResources( bindingIndex, 1u, &image );
			}

			static void bind( ID3D11DeviceContext * context, UINT bindingIndex, ID3D11Buffer * buffer, UINT offset, UINT range )
			{
				checkOffset( offset );
				context->GSSetConstantBuffers( bindingIndex, 1u, &buffer );
			}
		};

		template<>
		struct Binder< VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT, false >
		{
			static void bind( ID3D11DeviceContext * context, UINT bindingIndex, ID3D11ShaderResourceView * image, ID3D11SamplerState * sampler )
			{
				bind( context, bindingIndex, image );
				bind( context, bindingIndex, sampler );
			}

			static void bind( ID3D11DeviceContext * context, UINT bindingIndex, ID3D11SamplerState * sampler )
			{
				context->HSSetSamplers( bindingIndex, 1u, &sampler );
			}

			static void bind( ID3D11DeviceContext * context, UINT bindingIndex, ID3D11ShaderResourceView * image )
			{
				context->HSSetShaderResources( bindingIndex, 1u, &image );
			}

			static void bind( ID3D11DeviceContext * context, UINT bindingIndex, ID3D11Buffer * buffer, UINT offset, UINT range )
			{
				checkOffset( offset );
				context->HSSetConstantBuffers( bindingIndex, 1u, &buffer );
			}
		};

		template<>
		struct Binder< VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT, false >
		{
			static void bind( ID3D11DeviceContext * context, UINT bindingIndex, ID3D11ShaderResourceView * image, ID3D11SamplerState * sampler )
			{
				bind( context, bindingIndex, image );
				bind( context, bindingIndex, sampler );
			}

			static void bind( ID3D11DeviceContext * context, UINT bindingIndex, ID3D11SamplerState * sampler )
			{
				context->DSSetSamplers( bindingIndex, 1u, &sampler );
			}

			static void bind( ID3D11DeviceContext * context, UINT bindingIndex, ID3D11ShaderResourceView * image )
			{
				context->DSSetShaderResources( bindingIndex, 1u, &image );
			}

			static void bind( ID3D11DeviceContext * context, UINT bindingIndex, ID3D11Buffer * buffer, UINT offset, UINT range )
			{
				checkOffset( offset );
				context->DSSetConstantBuffers( bindingIndex, 1u, &buffer );
			}
		};

		template<>
		struct Binder< VK_SHADER_STAGE_FRAGMENT_BIT, false >
		{
			static void bind( ID3D11DeviceContext * context, UINT bindingIndex, ID3D11ShaderResourceView * image, ID3D11SamplerState * sampler )
			{
				bind( context, bindingIndex, image );
				bind( context, bindingIndex, sampler );
			}

			static void bind( ID3D11DeviceContext * context, UINT bindingIndex, ID3D11SamplerState * sampler )
			{
				context->PSSetSamplers( bindingIndex, 1u, &sampler );
			}

			static void bind( ID3D11DeviceContext * context, UINT bindingIndex, ID3D11ShaderResourceView * image )
			{
				context->PSSetShaderResources( bindingIndex, 1u, &image );
			}

			static void bind( ID3D11DeviceContext * context, UINT bindingIndex, ID3D11Buffer * buffer, UINT offset, UINT range )
			{
				checkOffset( offset );
				context->PSSetConstantBuffers( bindingIndex, 1u, &buffer );
			}
		};

		template< VkShaderStageFlagBits Flag, bool Supports11_1, typename ... Params >
		void tryBindOne( ID3D11DeviceContext1 * context
			, UINT bindingIndex
			, VkShaderStageFlags const & flags
			, Params ... params )
		{
			if ( checkFlag( flags, Flag ) )
			{
				Binder< Flag, Supports11_1 >::bind( context
					, bindingIndex
					, params... );
			}
		}

		template< bool Supports11_1, typename ... Params >
		void tryBind( ID3D11DeviceContext1 * context
			, UINT bindingIndex
			, VkShaderStageFlags const & flags
			, Params ... params )
		{
			tryBindOne< VK_SHADER_STAGE_COMPUTE_BIT, Supports11_1 >( context
				, bindingIndex
				, flags
				, params... );
			tryBindOne< VK_SHADER_STAGE_VERTEX_BIT, Supports11_1 >( context
				, bindingIndex
				, flags
				, params... );
			tryBindOne< VK_SHADER_STAGE_GEOMETRY_BIT, Supports11_1 >( context
				, bindingIndex
				, flags
				, params... );
			tryBindOne< VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT, Supports11_1 >( context
				, bindingIndex
				, flags
				, params... );
			tryBindOne< VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT, Supports11_1 >( context
				, bindingIndex
				, flags
				, params... );
			tryBindOne< VK_SHADER_STAGE_FRAGMENT_BIT, Supports11_1 >( context
				, bindingIndex
				, flags
				, params... );
		}

		template< bool Supports11_1 >
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
				tryBind< Supports11_1 >( context, bindingIndex, flags, view, sampler );
			}
		}

		template< bool Supports11_1 >
		void bindSampler( ID3D11DeviceContext1 * context
			, WriteDescriptorSetBinding const & writeBinding )
		{
			auto & write = writeBinding.write;
			auto flags = writeBinding.binding.getShaderStageFlags();

			for ( auto i = 0u; i < write.imageInfo.size(); ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				auto sampler = getSampler( write, i );
				tryBind< Supports11_1 >( context, bindingIndex, flags, sampler );
			}
		}

		template< bool Supports11_1 >
		void bindSampledTexture( ID3D11DeviceContext1 * context
			, WriteDescriptorSetBinding const & writeBinding )
		{
			auto & write = writeBinding.write;
			auto flags = writeBinding.binding.getShaderStageFlags();

			for ( auto i = 0u; i < write.imageInfo.size(); ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				auto view = getView( write, i );
				tryBind< Supports11_1 >( context, bindingIndex, flags, view );
			}
		}

		template< bool Supports11_1 >
		void bindStorageImage( ID3D11DeviceContext1 * context
			, WriteDescriptorSetBinding const & writeBinding )
		{
			auto & write = writeBinding.write;
			auto flags = writeBinding.binding.getShaderStageFlags();

			for ( auto i = 0u; i < write.imageInfo.size(); ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				auto view = getImageUAV( write, i );
				tryBindOne< VK_SHADER_STAGE_COMPUTE_BIT, Supports11_1 >( context
					, bindingIndex
					, flags
					, view );
			}
		}

		template< bool Supports11_1 >
		void bindUniformBuffer( ID3D11DeviceContext1 * context
			, WriteDescriptorSetBinding const & writeBinding )
		{
			auto & write = writeBinding.write;
			auto flags = writeBinding.binding.getShaderStageFlags();

			for ( auto i = 0u; i < write.bufferInfo.size(); ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				auto buffer = getBuffer( write, i );
				auto range = std::min( write.bufferInfo[i].range, uint64_t( write.bufferInfo[i].buffer.get().getSize() ) );
				tryBind< Supports11_1 >( context
					, bindingIndex
					, flags
					, buffer
					, UINT( write.bufferInfo[i].offset / 16 )
					, UINT( ashes::getAlignedSize( range / 16ull, 16u ) ) );
			}
		}

		template< bool Supports11_1 >
		void bindTexelBuffer( ID3D11DeviceContext1 * context
			, WriteDescriptorSetBinding const & writeBinding )
		{
			auto & write = writeBinding.write;
			auto flags = writeBinding.binding.getShaderStageFlags();

			for ( auto i = 0u; i < write.bufferInfo.size(); ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				auto view = getBufferView( write, i );
				tryBind< Supports11_1 >( context
					, bindingIndex
					, flags
					, view );
			}
		}

		template< bool Supports11_1 >
		void bindStorageBuffer( ID3D11DeviceContext1 * context
			, WriteDescriptorSetBinding const & writeBinding )
		{
			auto & write = writeBinding.write;
			auto flags = writeBinding.binding.getShaderStageFlags();

			for ( auto i = 0u; i < write.bufferInfo.size(); ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				auto view = getBufferUAV( write, i );
				tryBindOne< VK_SHADER_STAGE_COMPUTE_BIT, Supports11_1 >( context
					, bindingIndex
					, flags
					, view );
			}
		}

		template< bool Supports11_1 >
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
				auto range = std::min( write.bufferInfo[i].range, uint64_t( write.bufferInfo[i].buffer.get().getSize() ) );
				tryBind< Supports11_1 >( context
					, bindingIndex
					, flags
					, buffer
					, UINT( ( offset + write.bufferInfo[i].offset ) / 16 )
					, UINT( ashes::getAlignedSize( range / 16ull, 16u ) ) );
			}
		}

		template< bool Supports11_1 >
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
				tryBindOne< VK_SHADER_STAGE_COMPUTE_BIT, Supports11_1 >( context
					, bindingIndex
					, flags
					, view );
			}
		}

		template< bool Supports11_1 >
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
					bindDynamicUniformBuffer< Supports11_1 >( context, writeBinding, offsets[i] );
					break;

				case ashes::DescriptorType::eStorageBufferDynamic:
					bindDynamicStorageBuffer< Supports11_1 >( context, writeBinding, offsets[i] );
					break;

				default:
					assert( false && "Unsupported dynamic descriptor type" );
					throw std::runtime_error{ "Unsupported dynamic descriptor type" };
					break;
				}
			}
		}

		template< bool Supports11_1 >
		void unbindCombinedSampler( ID3D11DeviceContext1 * context
			, WriteDescriptorSetBinding const & writeBinding )
		{
			auto & write = writeBinding.write;
			auto flags = writeBinding.binding.getShaderStageFlags();

			for ( auto i = 0u; i < write.imageInfo.size(); ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				tryBind< Supports11_1 >( context
					, bindingIndex
					, flags
					, ( ID3D11ShaderResourceView * )nullptr
					, ( ID3D11SamplerState * )nullptr );
			}
		}

		template< bool Supports11_1 >
		void unbindSampler( ID3D11DeviceContext1 * context
			, WriteDescriptorSetBinding const & writeBinding )
		{
			auto & write = writeBinding.write;
			auto flags = writeBinding.binding.getShaderStageFlags();

			for ( auto i = 0u; i < write.imageInfo.size(); ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				tryBind< Supports11_1 >( context
					, bindingIndex
					, flags
					, ( ID3D11SamplerState * )nullptr );
			}
		}

		template< bool Supports11_1 >
		void unbindSampledTexture( ID3D11DeviceContext1 * context
			, WriteDescriptorSetBinding const & writeBinding )
		{
			auto & write = writeBinding.write;
			auto flags = writeBinding.binding.getShaderStageFlags();

			for ( auto i = 0u; i < write.imageInfo.size(); ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				tryBind< Supports11_1 >( context
					, bindingIndex
					, flags
					, ( ID3D11ShaderResourceView * )nullptr );
			}
		}

		template< bool Supports11_1 >
		void unbindStorageImage( ID3D11DeviceContext1 * context
			, WriteDescriptorSetBinding const & writeBinding )
		{
			auto & write = writeBinding.write;
			auto flags = writeBinding.binding.getShaderStageFlags();

			for ( auto i = 0u; i < write.imageInfo.size(); ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				tryBindOne< VK_SHADER_STAGE_COMPUTE_BIT, Supports11_1 >( context
					, bindingIndex
					, flags
					, ( ID3D11UnorderedAccessView * )nullptr );
			}
		}

		template< bool Supports11_1 >
		void unbindUniformBuffer( ID3D11DeviceContext1 * context
			, WriteDescriptorSetBinding const & writeBinding )
		{
			auto & write = writeBinding.write;
			auto flags = writeBinding.binding.getShaderStageFlags();

			for ( auto i = 0u; i < write.bufferInfo.size(); ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				auto range = std::min( write.bufferInfo[i].range, uint64_t( write.bufferInfo[i].buffer.get().getSize() ) );
				tryBind< Supports11_1 >( context
					, bindingIndex
					, flags
					, ( ID3D11Buffer * )nullptr
					, UINT( write.bufferInfo[i].offset / 16 )
					, UINT( ashes::getAlignedSize( range / 16ull, 16u ) ) );
			}
		}

		template< bool Supports11_1 >
		void unbindTexelBuffer( ID3D11DeviceContext1 * context
			, WriteDescriptorSetBinding const & writeBinding )
		{
			auto & write = writeBinding.write;
			auto flags = writeBinding.binding.getShaderStageFlags();

			for ( auto i = 0u; i < write.bufferInfo.size(); ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				tryBind< Supports11_1 >( context
					, bindingIndex
					, flags
					, ( ID3D11ShaderResourceView * )nullptr );
			}
		}

		template< bool Supports11_1 >
		void unbindStorageBuffer( ID3D11DeviceContext1 * context
			, WriteDescriptorSetBinding const & writeBinding )
		{
			auto & write = writeBinding.write;
			auto flags = writeBinding.binding.getShaderStageFlags();

			for ( auto i = 0u; i < write.bufferInfo.size(); ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				tryBindOne< VK_SHADER_STAGE_COMPUTE_BIT, Supports11_1 >( context
					, bindingIndex
					, flags
					, ( ID3D11UnorderedAccessView * )nullptr );
			}
		}

		template< bool Supports11_1 >
		void unbindDynamicUniformBuffer( ID3D11DeviceContext1 * context
			, WriteDescriptorSetBinding const & writeBinding
			, uint32_t offset )
		{
			auto & write = writeBinding.write;
			auto flags = writeBinding.binding.getShaderStageFlags();

			for ( auto i = 0u; i < write.bufferInfo.size(); ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				auto range = std::min( write.bufferInfo[i].range, uint64_t( write.bufferInfo[i].buffer.get().getSize() ) );
				tryBind< Supports11_1 >( context
					, bindingIndex
					, flags
					, ( ID3D11Buffer * )nullptr
					, UINT( ( offset + write.bufferInfo[i].offset ) / 16 )
					, UINT( ashes::getAlignedSize( range / 16ull, 16u ) ) );
			}
		}

		template< bool Supports11_1 >
		void unbindDynamicStorageBuffer( ID3D11DeviceContext1 * context
			, WriteDescriptorSetBinding const & writeBinding
			, uint32_t offset )
		{
			auto & write = writeBinding.write;
			auto flags = writeBinding.binding.getShaderStageFlags();

			for ( auto i = 0u; i < write.bufferInfo.size(); ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				tryBindOne< VK_SHADER_STAGE_COMPUTE_BIT, Supports11_1 >( context
					, bindingIndex
					, flags
					, ( ID3D11UnorderedAccessView * )nullptr );
			}
		}

		template< bool Supports11_1 >
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
					unbindDynamicUniformBuffer< Supports11_1 >( context, writeBinding, offsets[i] );
					break;

				case ashes::DescriptorType::eStorageBufferDynamic:
					unbindDynamicStorageBuffer< Supports11_1 >( context, writeBinding, offsets[i] );
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
		if ( context.featureLevel >= D3D_FEATURE_LEVEL_11_1 )
		{
			apply11_1( context );
		}
		else
		{
			apply11_0( context );
		}
	}

	void BindDescriptorSetCommand::remove( Context const & context )const
	{
		if ( context.featureLevel >= D3D_FEATURE_LEVEL_11_1 )
		{
			remove11_1( context );
		}
		else
		{
			remove11_0( context );
		}
	}

	void BindDescriptorSetCommand::apply11_0( Context const & context )const
	{
		for ( auto & write : m_descriptorSet.getCombinedTextureSamplers() )
		{
			bindCombinedSampler< false >( context.context1, write );
		}

		for ( auto & write : m_descriptorSet.getSamplers() )
		{
			bindSampler< false >( context.context1, write );
		}

		for ( auto & write : m_descriptorSet.getSampledTextures() )
		{
			bindSampledTexture< false >( context.context1, write );
		}

		for ( auto & write : m_descriptorSet.getUniformBuffers() )
		{
			bindUniformBuffer< false >( context.context1, write );
		}

		for ( auto & write : m_descriptorSet.getTexelBuffers() )
		{
			bindTexelBuffer< false >( context.context1, write );
		}

		for ( auto & write : m_descriptorSet.getStorageBuffers() )
		{
			bindStorageBuffer< false >( context.context1, write );
		}

		for ( auto & write : m_descriptorSet.getStorageTextures() )
		{
			bindStorageImage< false >( context.context1, write );
		}

		bindDynamicBuffers< false >( context.context1, m_descriptorSet.getDynamicBuffers(), m_dynamicOffsets );
	}

	void BindDescriptorSetCommand::apply11_1( Context const & context )const
	{
		for ( auto & write : m_descriptorSet.getCombinedTextureSamplers() )
		{
			bindCombinedSampler< true >( context.context1, write );
		}

		for ( auto & write : m_descriptorSet.getSamplers() )
		{
			bindSampler< true >( context.context1, write );
		}

		for ( auto & write : m_descriptorSet.getSampledTextures() )
		{
			bindSampledTexture< true >( context.context1, write );
		}

		for ( auto & write : m_descriptorSet.getUniformBuffers() )
		{
			bindUniformBuffer< true >( context.context1, write );
		}

		for ( auto & write : m_descriptorSet.getTexelBuffers() )
		{
			bindTexelBuffer< true >( context.context1, write );
		}

		for ( auto & write : m_descriptorSet.getStorageBuffers() )
		{
			bindStorageBuffer< true >( context.context1, write );
		}

		for ( auto & write : m_descriptorSet.getStorageTextures() )
		{
			bindStorageImage< true >( context.context1, write );
		}

		bindDynamicBuffers< true >( context.context1, m_descriptorSet.getDynamicBuffers(), m_dynamicOffsets );
	}

	void BindDescriptorSetCommand::remove11_0( Context const & context )const
	{
		for ( auto & write : m_descriptorSet.getCombinedTextureSamplers() )
		{
			unbindCombinedSampler< false >( context.context1, write );
		}

		for ( auto & write : m_descriptorSet.getSamplers() )
		{
			unbindSampler< false >( context.context1, write );
		}

		for ( auto & write : m_descriptorSet.getSampledTextures() )
		{
			unbindSampledTexture< false >( context.context1, write );
		}

		for ( auto & write : m_descriptorSet.getUniformBuffers() )
		{
			unbindUniformBuffer< false >( context.context1, write );
		}

		for ( auto & write : m_descriptorSet.getTexelBuffers() )
		{
			unbindTexelBuffer< false >( context.context1, write );
		}

		for ( auto & write : m_descriptorSet.getStorageBuffers() )
		{
			unbindStorageBuffer< false >( context.context1, write );
		}

		for ( auto & write : m_descriptorSet.getStorageTextures() )
		{
			unbindStorageImage< false >( context.context1, write );
		}

		unbindDynamicBuffers< false >( context.context1, m_descriptorSet.getDynamicBuffers(), m_dynamicOffsets );
	}

	void BindDescriptorSetCommand::remove11_1( Context const & context )const
	{
		for ( auto & write : m_descriptorSet.getCombinedTextureSamplers() )
		{
			unbindCombinedSampler< true >( context.context1, write );
		}

		for ( auto & write : m_descriptorSet.getSamplers() )
		{
			unbindSampler< true >( context.context1, write );
		}

		for ( auto & write : m_descriptorSet.getSampledTextures() )
		{
			unbindSampledTexture< true >( context.context1, write );
		}

		for ( auto & write : m_descriptorSet.getUniformBuffers() )
		{
			unbindUniformBuffer< true >( context.context1, write );
		}

		for ( auto & write : m_descriptorSet.getTexelBuffers() )
		{
			unbindTexelBuffer< true >( context.context1, write );
		}

		for ( auto & write : m_descriptorSet.getStorageBuffers() )
		{
			unbindStorageBuffer< true >( context.context1, write );
		}

		for ( auto & write : m_descriptorSet.getStorageTextures() )
		{
			unbindStorageImage< true >( context.context1, write );
		}

		unbindDynamicBuffers< true >( context.context1, m_descriptorSet.getDynamicBuffers(), m_dynamicOffsets );
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
