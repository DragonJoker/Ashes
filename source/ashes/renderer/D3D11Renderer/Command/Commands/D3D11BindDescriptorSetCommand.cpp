/*
This file belongs to Ashes.
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

#include "ashesd3d11_api.hpp"

namespace ashes::D3D11_NAMESPACE
{
	namespace
	{
		ID3D11ShaderResourceView * getView( VkWriteDescriptorSet const & write, uint32_t index )
		{
			assert( index < write.descriptorCount );
			return get( write.pImageInfo[index].imageView )->getShaderView();
		}
		
		ID3D11ShaderResourceView * getBufferView( VkWriteDescriptorSet const & write, uint32_t index )
		{
			assert( index < write.descriptorCount );
			return get( write.pTexelBufferView[index] )->getView();
		}
		
		ID3D11UnorderedAccessView * getImageUAV( VkWriteDescriptorSet const & write, uint32_t index )
		{
			assert( index < write.descriptorCount );
			return get( write.pImageInfo[index].imageView )->getUnorderedAccessView();
		}

		ID3D11SamplerState * getSampler( VkWriteDescriptorSet const & write, uint32_t index )
		{
			assert( index < write.descriptorCount );
			return get( write.pImageInfo[index].sampler )->getSampler();
		}

		ID3D11Buffer * getBuffer( VkWriteDescriptorSet const & write, uint32_t index )
		{
			assert( index < write.descriptorCount );
			return get( write.pBufferInfo[index].buffer )->getBuffer();
		}

		ID3D11UnorderedAccessView * getBufferUAV( VkWriteDescriptorSet const & write, uint32_t index )
		{
			assert( index < write.descriptorCount );
			return get( write.pBufferInfo[index].buffer )->getUnorderedAccessView();
		}

		template< VkShaderStageFlagBits Flag, bool Supports11_1 >
		struct Binder;

		template<>
		struct Binder< VK_SHADER_STAGE_COMPUTE_BIT, true >
		{
			// Combined Image + Sampler
			static void bind( ID3D11DeviceContext1 * context, UINT bindingIndex, ID3D11ShaderResourceView * image, ID3D11SamplerState * sampler )
			{
				bind( context, bindingIndex, image );
				bind( context, bindingIndex, sampler );
			}
			// Sampler
			static void bind( ID3D11DeviceContext1 * context, UINT bindingIndex, ID3D11SamplerState * sampler )
			{
				context->CSSetSamplers( bindingIndex, 1u, &sampler );
			}
			// Sampled Image
			static void bind( ID3D11DeviceContext1 * context, UINT bindingIndex, ID3D11ShaderResourceView * image )
			{
				context->CSSetShaderResources( bindingIndex, 1u, &image );
			}
			// Uniform Buffer
			static void bind( ID3D11DeviceContext1 * context, UINT bindingIndex, ID3D11Buffer * buffer, UINT offset, UINT range )
			{
				context->CSSetConstantBuffers1( bindingIndex, 1u, &buffer, &offset, &range );
			}
			// Storage buffer/image
			static void bind( ID3D11DeviceContext1 * context, UINT bindingIndex, ID3D11UnorderedAccessView * view )
			{
				context->CSSetUnorderedAccessViews( bindingIndex, 1u, &view, nullptr );
			}
			// Storage buffer/image
			static void gather( UINT, ID3D11UnorderedAccessView const *, std::map< UINT, ID3D11UnorderedAccessView * > const * )
			{
			}
		};

		template<>
		struct Binder< VK_SHADER_STAGE_VERTEX_BIT, true >
		{
			// Combined Image + Sampler
			static void bind( ID3D11DeviceContext1 * context, UINT bindingIndex, ID3D11ShaderResourceView * image, ID3D11SamplerState * sampler )
			{
				bind( context, bindingIndex, image );
				bind( context, bindingIndex, sampler );
			}
			// Sampler
			static void bind( ID3D11DeviceContext1 * context, UINT bindingIndex, ID3D11SamplerState * sampler )
			{
				context->VSSetSamplers( bindingIndex, 1u, &sampler );
			}
			// Sampled Image
			static void bind( ID3D11DeviceContext1 * context, UINT bindingIndex, ID3D11ShaderResourceView * image )
			{
				context->VSSetShaderResources( bindingIndex, 1u, &image );
			}
			// Uniform Buffer
			static void bind( ID3D11DeviceContext1 * context, UINT bindingIndex, ID3D11Buffer * buffer, UINT offset, UINT range )
			{
				context->VSSetConstantBuffers1( bindingIndex, 1u, &buffer, &offset, &range );
			}
			// Storage buffer/image
			static void gather( UINT bindingIndex, ID3D11UnorderedAccessView * view, std::map< UINT, ID3D11UnorderedAccessView * > * uavs )
			{
				( *uavs )[bindingIndex] = view;
			}
		};

		template<>
		struct Binder< VK_SHADER_STAGE_GEOMETRY_BIT, true >
		{
			// Combined Image + Sampler
			static void bind( ID3D11DeviceContext1 * context, UINT bindingIndex, ID3D11ShaderResourceView * image, ID3D11SamplerState * sampler )
			{
				bind( context, bindingIndex, image );
				bind( context, bindingIndex, sampler );
			}
			// Sampler
			static void bind( ID3D11DeviceContext1 * context, UINT bindingIndex, ID3D11SamplerState * sampler )
			{
				context->GSSetSamplers( bindingIndex, 1u, &sampler );
			}
			// Sampled Image
			static void bind( ID3D11DeviceContext1 * context, UINT bindingIndex, ID3D11ShaderResourceView * image )
			{
				context->GSSetShaderResources( bindingIndex, 1u, &image );
			}
			// Uniform Buffer
			static void bind( ID3D11DeviceContext1 * context, UINT bindingIndex, ID3D11Buffer * buffer, UINT offset, UINT range )
			{
				context->GSSetConstantBuffers1( bindingIndex, 1u, &buffer, &offset, &range );
			}
			// Storage buffer/image
			static void gather( UINT bindingIndex, ID3D11UnorderedAccessView * view, std::map< UINT, ID3D11UnorderedAccessView * > * uavs )
			{
				( *uavs )[bindingIndex] = view;
			}
		};

		template<>
		struct Binder< VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT, true >
		{
			// Combined Image + Sampler
			static void bind( ID3D11DeviceContext1 * context, UINT bindingIndex, ID3D11ShaderResourceView * image, ID3D11SamplerState * sampler )
			{
				bind( context, bindingIndex, image );
				bind( context, bindingIndex, sampler );
			}
			// Sampler
			static void bind( ID3D11DeviceContext1 * context, UINT bindingIndex, ID3D11SamplerState * sampler )
			{
				context->HSSetSamplers( bindingIndex, 1u, &sampler );
			}
			// Sampled Image
			static void bind( ID3D11DeviceContext1 * context, UINT bindingIndex, ID3D11ShaderResourceView * image )
			{
				context->HSSetShaderResources( bindingIndex, 1u, &image );
			}
			// Uniform Buffer
			static void bind( ID3D11DeviceContext1 * context, UINT bindingIndex, ID3D11Buffer * buffer, UINT offset, UINT range )
			{
				context->HSSetConstantBuffers1( bindingIndex, 1u, &buffer, &offset, &range );
			}
			// Storage buffer/image
			static void gather( UINT bindingIndex, ID3D11UnorderedAccessView * view, std::map< UINT, ID3D11UnorderedAccessView * > * uavs )
			{
				( *uavs )[bindingIndex] = view;
			}
		};

		template<>
		struct Binder< VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT, true >
		{
			// Combined Image + Sampler
			static void bind( ID3D11DeviceContext1 * context, UINT bindingIndex, ID3D11ShaderResourceView * image, ID3D11SamplerState * sampler )
			{
				bind( context, bindingIndex, image );
				bind( context, bindingIndex, sampler );
			}
			// Sampler
			static void bind( ID3D11DeviceContext1 * context, UINT bindingIndex, ID3D11SamplerState * sampler )
			{
				context->DSSetSamplers( bindingIndex, 1u, &sampler );
			}
			// Sampled Image
			static void bind( ID3D11DeviceContext1 * context, UINT bindingIndex, ID3D11ShaderResourceView * image )
			{
				context->DSSetShaderResources( bindingIndex, 1u, &image );
			}
			// Uniform Buffer
			static void bind( ID3D11DeviceContext1 * context, UINT bindingIndex, ID3D11Buffer * buffer, UINT offset, UINT range )
			{
				context->DSSetConstantBuffers1( bindingIndex, 1u, &buffer, &offset, &range );
			}
			// Storage buffer/image
			static void gather( UINT bindingIndex, ID3D11UnorderedAccessView * view, std::map< UINT, ID3D11UnorderedAccessView * > * uavs )
			{
				( *uavs )[bindingIndex] = view;
			}
		};

		template<>
		struct Binder< VK_SHADER_STAGE_FRAGMENT_BIT, true >
		{
			// Combined Image + Sampler
			static void bind( ID3D11DeviceContext1 * context, UINT bindingIndex, ID3D11ShaderResourceView * image, ID3D11SamplerState * sampler )
			{
				bind( context, bindingIndex, image );
				bind( context, bindingIndex, sampler );
			}
			// Sampler
			static void bind( ID3D11DeviceContext1 * context, UINT bindingIndex, ID3D11SamplerState * sampler )
			{
				context->PSSetSamplers( bindingIndex, 1u, &sampler );
			}
			// Sampled Image
			static void bind( ID3D11DeviceContext1 * context, UINT bindingIndex, ID3D11ShaderResourceView * image )
			{
				context->PSSetShaderResources( bindingIndex, 1u, &image );
			}
			// Uniform Buffer
			static void bind( ID3D11DeviceContext1 * context, UINT bindingIndex, ID3D11Buffer * buffer, UINT offset, UINT range )
			{
				context->PSSetConstantBuffers1( bindingIndex, 1u, &buffer, &offset, &range );
			}
			// Storage buffer/image
			static void gather( UINT bindingIndex, ID3D11UnorderedAccessView * view, std::map< UINT, ID3D11UnorderedAccessView * > * uavs )
			{
				( *uavs )[bindingIndex] = view;
			}
		};

		inline void checkOffset( uint32_t offset )
		{
#ifndef NDEBUG
			if ( offset != 0u )
			{
				std::cerr << "Binding buffer range not supported by this driver" << std::endl;
			}
#endif
		}

		template<>
		struct Binder< VK_SHADER_STAGE_COMPUTE_BIT, false >
		{
			// Combined Image + Sampler
			static void bind( ID3D11DeviceContext * context, UINT bindingIndex, ID3D11ShaderResourceView * image, ID3D11SamplerState * sampler )
			{
				bind( context, bindingIndex, image );
				bind( context, bindingIndex, sampler );
			}
			// Sampler
			static void bind( ID3D11DeviceContext * context, UINT bindingIndex, ID3D11SamplerState * sampler )
			{
				context->CSSetSamplers( bindingIndex, 1u, &sampler );
			}
			// Sampled Image
			static void bind( ID3D11DeviceContext * context, UINT bindingIndex, ID3D11ShaderResourceView * image )
			{
				context->CSSetShaderResources( bindingIndex, 1u, &image );
			}
			// Uniform Buffer
			static void bind( ID3D11DeviceContext * context, UINT bindingIndex, ID3D11Buffer * buffer, UINT offset, UINT )
			{
				checkOffset( offset );
				context->CSSetConstantBuffers( bindingIndex, 1u, &buffer );
			}
			// Storage buffer/image
			static void bind( ID3D11DeviceContext * context, UINT bindingIndex, ID3D11UnorderedAccessView * view )
			{
				context->CSSetUnorderedAccessViews( bindingIndex, 1u, &view, nullptr );
			}
			// Storage buffer/image
			static void gather( UINT, ID3D11UnorderedAccessView const *, std::map< UINT, ID3D11UnorderedAccessView * > const * )
			{
			}
		};

		template<>
		struct Binder< VK_SHADER_STAGE_VERTEX_BIT, false >
		{
			// Combined Image + Sampler
			static void bind( ID3D11DeviceContext * context, UINT bindingIndex, ID3D11ShaderResourceView * image, ID3D11SamplerState * sampler )
			{
				bind( context, bindingIndex, image );
				bind( context, bindingIndex, sampler );
			}
			// Sampler
			static void bind( ID3D11DeviceContext * context, UINT bindingIndex, ID3D11SamplerState * sampler )
			{
				context->VSSetSamplers( bindingIndex, 1u, &sampler );
			}
			// Sampled Image
			static void bind( ID3D11DeviceContext * context, UINT bindingIndex, ID3D11ShaderResourceView * image )
			{
				context->VSSetShaderResources( bindingIndex, 1u, &image );
			}
			// Uniform Buffer
			static void bind( ID3D11DeviceContext * context, UINT bindingIndex, ID3D11Buffer * buffer, UINT offset, UINT )
			{
				checkOffset( offset );
				context->VSSetConstantBuffers( bindingIndex, 1u, &buffer );
			}
			// Storage buffer/image
			static void gather( UINT bindingIndex, ID3D11UnorderedAccessView * view, std::map< UINT, ID3D11UnorderedAccessView * > * uavs )
			{
				( *uavs )[bindingIndex] = view;
			}
		};

		template<>
		struct Binder< VK_SHADER_STAGE_GEOMETRY_BIT, false >
		{
			// Combined Image + Sampler
			static void bind( ID3D11DeviceContext * context, UINT bindingIndex, ID3D11ShaderResourceView * image, ID3D11SamplerState * sampler )
			{
				bind( context, bindingIndex, image );
				bind( context, bindingIndex, sampler );
			}
			// Sampler
			static void bind( ID3D11DeviceContext * context, UINT bindingIndex, ID3D11SamplerState * sampler )
			{
				context->GSSetSamplers( bindingIndex, 1u, &sampler );
			}
			// Sampled Image
			static void bind( ID3D11DeviceContext * context, UINT bindingIndex, ID3D11ShaderResourceView * image )
			{
				context->GSSetShaderResources( bindingIndex, 1u, &image );
			}
			// Uniform Buffer
			static void bind( ID3D11DeviceContext * context, UINT bindingIndex, ID3D11Buffer * buffer, UINT offset, UINT )
			{
				checkOffset( offset );
				context->GSSetConstantBuffers( bindingIndex, 1u, &buffer );
			}
			// Storage buffer/image
			static void gather( UINT bindingIndex, ID3D11UnorderedAccessView * view, std::map< UINT, ID3D11UnorderedAccessView * > * uavs )
			{
				( *uavs )[bindingIndex] = view;
			}
		};

		template<>
		struct Binder< VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT, false >
		{
			// Combined Image + Sampler
			static void bind( ID3D11DeviceContext * context, UINT bindingIndex, ID3D11ShaderResourceView * image, ID3D11SamplerState * sampler )
			{
				bind( context, bindingIndex, image );
				bind( context, bindingIndex, sampler );
			}
			// Sampler
			static void bind( ID3D11DeviceContext * context, UINT bindingIndex, ID3D11SamplerState * sampler )
			{
				context->HSSetSamplers( bindingIndex, 1u, &sampler );
			}
			// Sampled Image
			static void bind( ID3D11DeviceContext * context, UINT bindingIndex, ID3D11ShaderResourceView * image )
			{
				context->HSSetShaderResources( bindingIndex, 1u, &image );
			}
			// Uniform Buffer
			static void bind( ID3D11DeviceContext * context, UINT bindingIndex, ID3D11Buffer * buffer, UINT offset, UINT )
			{
				checkOffset( offset );
				context->HSSetConstantBuffers( bindingIndex, 1u, &buffer );
			}
			// Storage buffer/image
			static void gather( UINT bindingIndex, ID3D11UnorderedAccessView * view, std::map< UINT, ID3D11UnorderedAccessView * > * uavs )
			{
				( *uavs )[bindingIndex] = view;
			}
		};

		template<>
		struct Binder< VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT, false >
		{
			// Combined Image + Sampler
			static void bind( ID3D11DeviceContext * context, UINT bindingIndex, ID3D11ShaderResourceView * image, ID3D11SamplerState * sampler )
			{
				bind( context, bindingIndex, image );
				bind( context, bindingIndex, sampler );
			}
			// Sampler
			static void bind( ID3D11DeviceContext * context, UINT bindingIndex, ID3D11SamplerState * sampler )
			{
				context->DSSetSamplers( bindingIndex, 1u, &sampler );
			}
			// Sampled Image
			static void bind( ID3D11DeviceContext * context, UINT bindingIndex, ID3D11ShaderResourceView * image )
			{
				context->DSSetShaderResources( bindingIndex, 1u, &image );
			}
			// Uniform Buffer
			static void bind( ID3D11DeviceContext * context, UINT bindingIndex, ID3D11Buffer * buffer, UINT offset, UINT )
			{
				checkOffset( offset );
				context->DSSetConstantBuffers( bindingIndex, 1u, &buffer );
			}
			// Storage buffer/image
			static void gather( UINT bindingIndex, ID3D11UnorderedAccessView * view, std::map< UINT, ID3D11UnorderedAccessView * > * uavs )
			{
				( *uavs )[bindingIndex] = view;
			}
		};

		template<>
		struct Binder< VK_SHADER_STAGE_FRAGMENT_BIT, false >
		{
			// Combined Image + Sampler
			static void bind( ID3D11DeviceContext * context, UINT bindingIndex, ID3D11ShaderResourceView * image, ID3D11SamplerState * sampler )
			{
				bind( context, bindingIndex, image );
				bind( context, bindingIndex, sampler );
			}
			// Sampler
			static void bind( ID3D11DeviceContext * context, UINT bindingIndex, ID3D11SamplerState * sampler )
			{
				context->PSSetSamplers( bindingIndex, 1u, &sampler );
			}
			// Sampled Image
			static void bind( ID3D11DeviceContext * context, UINT bindingIndex, ID3D11ShaderResourceView * image )
			{
				context->PSSetShaderResources( bindingIndex, 1u, &image );
			}
			// Uniform Buffer
			static void bind( ID3D11DeviceContext * context, UINT bindingIndex, ID3D11Buffer * buffer, UINT offset, UINT )
			{
				checkOffset( offset );
				context->PSSetConstantBuffers( bindingIndex, 1u, &buffer );
			}
			// Storage buffer/image
			static void gather( UINT bindingIndex, ID3D11UnorderedAccessView * view, std::map< UINT, ID3D11UnorderedAccessView * > * uavs )
			{
				( *uavs )[bindingIndex] = view;
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
			, LayoutBindingWrites const & writeBinding
			, ShaderBindingMap const & bindings
			, uint32_t setIndex )
		{
			for ( auto & write : writeBinding.writes )
			{
				auto flags = writeBinding.binding.stageFlags;
				auto it = bindings.find( makeShaderBindingKey( setIndex, write.dstBinding ) );
				assert( it != bindings.end() );
				auto dstBinding = it->second;

				for ( auto i = 0u; i < write.descriptorCount; ++i )
				{
					uint32_t bindingIndex = dstBinding + write.dstArrayElement + i;
					auto view = getView( write, i );
					auto sampler = getSampler( write, i );
					tryBind< Supports11_1 >( context, bindingIndex, flags, view, sampler );
				}
			}
		}
		
		template< bool Supports11_1 >
		void bindInputAttachment( ID3D11DeviceContext1 * context
			, VkSampler sampler
			, LayoutBindingWrites const & writeBinding
			, ShaderBindingMap const & bindings
			, uint32_t setIndex )
		{
			for ( auto & write : writeBinding.writes )
			{
				auto flags = writeBinding.binding.stageFlags;
				auto it = bindings.find( makeShaderBindingKey( setIndex, write.dstBinding ) );
				assert( it != bindings.end() );
				auto dstBinding = it->second;

				for ( auto i = 0u; i < write.descriptorCount; ++i )
				{
					uint32_t bindingIndex = dstBinding + write.dstArrayElement + i;
					auto view = getView( write, i );
					tryBind< Supports11_1 >( context, bindingIndex, flags, view, get( sampler )->getSampler() );
				}
			}
		}

		template< bool Supports11_1 >
		void bindSampler( ID3D11DeviceContext1 * context
			, LayoutBindingWrites const & writeBinding
			, ShaderBindingMap const & bindings
			, uint32_t setIndex )
		{
			for ( auto & write : writeBinding.writes )
			{
				auto flags = writeBinding.binding.stageFlags;
				auto it = bindings.find( makeShaderBindingKey( setIndex, write.dstBinding ) );
				assert( it != bindings.end() );
				auto dstBinding = it->second;

				for ( auto i = 0u; i < write.descriptorCount; ++i )
				{
					uint32_t bindingIndex = dstBinding + write.dstArrayElement + i;
					auto sampler = getSampler( write, i );
					tryBind< Supports11_1 >( context, bindingIndex, flags, sampler );
				}
			}
		}

		template< bool Supports11_1 >
		void bindSampledTexture( ID3D11DeviceContext1 * context
			, LayoutBindingWrites const & writeBinding
			, ShaderBindingMap const & bindings
			, uint32_t setIndex )
		{
			for ( auto & write : writeBinding.writes )
			{
				auto flags = writeBinding.binding.stageFlags;
				auto it = bindings.find( makeShaderBindingKey( setIndex, write.dstBinding ) );
				assert( it != bindings.end() );
				auto dstBinding = it->second;

				for ( auto i = 0u; i < write.descriptorCount; ++i )
				{
					uint32_t bindingIndex = dstBinding + write.dstArrayElement + i;
					auto view = getView( write, i );
					tryBind< Supports11_1 >( context, bindingIndex, flags, view );
				}
			}
		}

		template< bool Supports11_1 >
		void bindUniformBuffer( ID3D11DeviceContext1 * context
			, LayoutBindingWrites const & writeBinding
			, ShaderBindingMap const & bindings
			, uint32_t setIndex )
		{
			for ( auto & write : writeBinding.writes )
			{
				auto flags = writeBinding.binding.stageFlags;
				auto it = bindings.find( makeShaderBindingKey( setIndex, write.dstBinding ) );
				assert( it != bindings.end() );
				auto dstBinding = it->second;

				for ( auto i = 0u; i < write.descriptorCount; ++i )
				{
					uint32_t bindingIndex = dstBinding + write.dstArrayElement + i;
					auto buffer = getBuffer( write, i );
					auto range = std::min( write.pBufferInfo[i].range
						, get( write.pBufferInfo[i].buffer )->getSize() );
					tryBind< Supports11_1 >( context
						, bindingIndex
						, flags
						, buffer
						, UINT( write.pBufferInfo[i].offset ) / 16
						, UINT( ashes::getAlignedSize( range, 256ULL ) / 16ULL ) );
				}
			}
		}

		template< bool Supports11_1 >
		void bindUniformTexelBuffer( ID3D11DeviceContext1 * context
			, LayoutBindingWrites const & writeBinding
			, ShaderBindingMap const & bindings
			, uint32_t setIndex )
		{
			for ( auto & write : writeBinding.writes )
			{
				auto flags = writeBinding.binding.stageFlags;
				auto it = bindings.find( makeShaderBindingKey( setIndex, write.dstBinding ) );
				assert( it != bindings.end() );
				auto dstBinding = it->second;

				for ( auto i = 0u; i < write.descriptorCount; ++i )
				{
					uint32_t bindingIndex = dstBinding + write.dstArrayElement + i;
					auto view = getBufferView( write, i );
					tryBind< Supports11_1 >( context
						, bindingIndex
						, flags
						, view );
				}
			}
		}

		template< bool Supports11_1 >
		void bindDynamicUniformBuffer( ID3D11DeviceContext1 * context
			, LayoutBindingWrites const & writeBinding
			, ShaderBindingMap const & bindings
			, uint32_t setIndex
			, uint32_t offset )
		{
			for ( auto & write : writeBinding.writes )
			{
				auto flags = writeBinding.binding.stageFlags;
				auto it = bindings.find( makeShaderBindingKey( setIndex, write.dstBinding ) );
				assert( it != bindings.end() );
				auto dstBinding = it->second;

				for ( auto i = 0u; i < write.descriptorCount; ++i )
				{
					uint32_t bindingIndex = dstBinding + write.dstArrayElement + i;
					auto buffer = getBuffer( write, i );
					auto range = std::min( write.pBufferInfo[i].range
						, get( write.pBufferInfo[i].buffer )->getSize() );
					tryBind< Supports11_1 >( context
						, bindingIndex
						, flags
						, buffer
						, UINT( ( offset + write.pBufferInfo[i].offset ) / 16 )
						, UINT( ashes::getAlignedSize( range, 256ULL ) / 16ULL ) );
				}
			}
		}

		template< bool Supports11_1 >
		void bindStorageImage( ID3D11DeviceContext1 * context
			, LayoutBindingWrites const & writeBinding
			, ShaderBindingMap const & bindings
			, uint32_t setIndex )
		{
			for ( auto & write : writeBinding.writes )
			{
				auto flags = writeBinding.binding.stageFlags;
				auto it = bindings.find( makeShaderBindingKey( setIndex, write.dstBinding ) );
				assert( it != bindings.end() );
				auto dstBinding = it->second;

				for ( auto i = 0u; i < write.descriptorCount; ++i )
				{
					uint32_t bindingIndex = dstBinding + write.dstArrayElement + i;
					auto view = getImageUAV( write, i );
					tryBindOne< VK_SHADER_STAGE_COMPUTE_BIT, Supports11_1 >( context
						, bindingIndex
						, flags
						, view );
				}
			}
		}

		template< bool Supports11_1 >
		void bindStorageTexelBuffer( ID3D11DeviceContext1 * context
			, LayoutBindingWrites const & writeBinding
			, ShaderBindingMap const & bindings
			, uint32_t setIndex )
		{
			for ( auto & write : writeBinding.writes )
			{
				auto flags = writeBinding.binding.stageFlags;
				auto it = bindings.find( makeShaderBindingKey( setIndex, write.dstBinding ) );
				assert( it != bindings.end() );
				auto dstBinding = it->second;

				for ( auto i = 0u; i < write.descriptorCount; ++i )
				{
					uint32_t bindingIndex = dstBinding + write.dstArrayElement + i;
					auto view = getBufferView( write, i );
					tryBind< Supports11_1 >( context
						, bindingIndex
						, flags
						, view );
				}
			}
		}

		template< bool Supports11_1 >
		void bindStorageBuffer( ID3D11DeviceContext1 * context
			, LayoutBindingWrites const & writeBinding
			, ShaderBindingMap const & bindings
			, uint32_t setIndex )
		{
			for ( auto & write : writeBinding.writes )
			{
				auto flags = writeBinding.binding.stageFlags;
				auto it = bindings.find( makeShaderBindingKey( setIndex, write.dstBinding ) );
				assert( it != bindings.end() );
				auto dstBinding = it->second;

				for ( auto i = 0u; i < write.descriptorCount; ++i )
				{
					uint32_t bindingIndex = dstBinding + write.dstArrayElement + i;
					auto view = getBufferUAV( write, i );
					tryBindOne< VK_SHADER_STAGE_COMPUTE_BIT, Supports11_1 >( context
						, bindingIndex
						, flags
						, view );
				}
			}
		}

		template< bool Supports11_1 >
		void bindDynamicStorageBuffer( ID3D11DeviceContext1 * context
			, LayoutBindingWrites const & writeBinding
			, ShaderBindingMap const & bindings
			, uint32_t setIndex
			, uint32_t /*offset*/ )
		{
			for ( auto & write : writeBinding.writes )
			{
				auto flags = writeBinding.binding.stageFlags;
				auto it = bindings.find( makeShaderBindingKey( setIndex, write.dstBinding ) );
				assert( it != bindings.end() );
				auto dstBinding = it->second;

				for ( auto i = 0u; i < write.descriptorCount; ++i )
				{
					uint32_t bindingIndex = dstBinding + write.dstArrayElement + i;
					auto view = getBufferUAV( write, i );
					tryBindOne< VK_SHADER_STAGE_COMPUTE_BIT, Supports11_1 >( context
						, bindingIndex
						, flags
						, view );
				}
			}
		}

		template< bool Supports11_1 >
		void bindDynamicBuffers( ID3D11DeviceContext1 * context
			, LayoutBindingWritesArray const & writes
			, ShaderBindings const & bindings
			, uint32_t setIndex
			, UInt32Array const & offsets )
		{
			for ( auto i = 0u; i < offsets.size(); ++i )
			{
				auto & writeBinding = *writes[i];

				switch ( writeBinding.binding.descriptorType )
				{
				case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:
					bindDynamicUniformBuffer< Supports11_1 >( context, writeBinding, bindings.ubo, setIndex, offsets[i] );
					break;

				case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC:
					bindDynamicStorageBuffer< Supports11_1 >( context, writeBinding, bindings.sbo, setIndex, offsets[i] );
					break;

				default:
					assert( false && "Unsupported dynamic descriptor type" );
					throw ashes::BaseException{ "Unsupported dynamic descriptor type" };
					break;
				}
			}
		}

		template< VkShaderStageFlagBits Flag, typename ... Params >
		void tryGatherOne( UINT bindingIndex
			, VkShaderStageFlags const & flags
			, Params ... params )
		{
			if ( checkFlag( flags, Flag ) )
			{
				Binder< Flag, true >::gather( bindingIndex
					, params... );
			}
		}

		template< typename ... Params >
		void tryGather( UINT bindingIndex
			, VkShaderStageFlags const & flags
			, Params ... params )
		{
			tryGatherOne< VK_SHADER_STAGE_COMPUTE_BIT >( bindingIndex
				, flags
				, params... );
			tryGatherOne< VK_SHADER_STAGE_VERTEX_BIT >( bindingIndex
				, flags
				, params... );
			tryGatherOne< VK_SHADER_STAGE_GEOMETRY_BIT >( bindingIndex
				, flags
				, params... );
			tryGatherOne< VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT >( bindingIndex
				, flags
				, params... );
			tryGatherOne< VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT >( bindingIndex
				, flags
				, params... );
			tryGatherOne< VK_SHADER_STAGE_FRAGMENT_BIT >( bindingIndex
				, flags
				, params... );
		}

		inline void gatherStorageImage( LayoutBindingWrites const & writeBinding
			, ShaderBindingMap const & bindings
			, uint32_t setIndex
			, std::map< UINT, ID3D11UnorderedAccessView * > & uavs )
		{
			for ( auto & write : writeBinding.writes )
			{
				auto flags = writeBinding.binding.stageFlags;
				auto it = bindings.find( makeShaderBindingKey( setIndex, write.dstBinding ) );
				assert( it != bindings.end() );
				auto dstBinding = it->second;

				for ( auto i = 0u; i < write.descriptorCount; ++i )
				{
					uint32_t bindingIndex = dstBinding + write.dstArrayElement + i;
					auto view = getImageUAV( write, i );
					tryGather( bindingIndex
						, flags
						, view
						, &uavs );
				}
			}
		}

		inline void gatherStorageBuffer( LayoutBindingWrites const & writeBinding
			, ShaderBindingMap const & bindings
			, uint32_t setIndex
			, std::map< UINT, ID3D11UnorderedAccessView * > & uavs )
		{
			for ( auto & write : writeBinding.writes )
			{
				auto flags = writeBinding.binding.stageFlags;
				auto it = bindings.find( makeShaderBindingKey( setIndex, write.dstBinding ) );
				assert( it != bindings.end() );
				auto dstBinding = it->second;

				for ( auto i = 0u; i < write.descriptorCount; ++i )
				{
					uint32_t bindingIndex = dstBinding + write.dstArrayElement + i;
					auto view = getBufferUAV( write, i );
					tryGather( bindingIndex
						, flags
						, view
						, &uavs );
				}
			}
		}

		inline void gatherDynamicStorageBuffer( LayoutBindingWrites const & writeBinding
			, ShaderBindingMap const & bindings
			, uint32_t setIndex
			, std::map< UINT, ID3D11UnorderedAccessView * > & uavs
			, uint32_t /*offset*/ )
		{
			for ( auto & write : writeBinding.writes )
			{
				auto flags = writeBinding.binding.stageFlags;
				auto it = bindings.find( makeShaderBindingKey( setIndex, write.dstBinding ) );
				assert( it != bindings.end() );
				auto dstBinding = it->second;

				for ( auto i = 0u; i < write.descriptorCount; ++i )
				{
					uint32_t bindingIndex = dstBinding + write.dstArrayElement + i;
					auto view = getBufferUAV( write, i );
					tryGather( bindingIndex
						, flags
						, view
						, &uavs );
				}
			}
		}

		inline void gatherDynamicBuffers( LayoutBindingWritesArray const & writes
			, ShaderBindings const & bindings
			, uint32_t setIndex
			, std::map< UINT, ID3D11UnorderedAccessView * > & uavs
			, UInt32Array const & offsets )
		{
			for ( auto i = 0u; i < offsets.size(); ++i )
			{
				auto const & writeBinding = *writes[i];

				switch ( writeBinding.binding.descriptorType )
				{
				case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC:
					gatherDynamicStorageBuffer( writeBinding
						, bindings.sbo
						, setIndex
						, uavs
						, offsets[i] );
					break;

				default:
					assert( false && "Unsupported dynamic descriptor type" );
					throw ashes::BaseException{ "Unsupported dynamic descriptor type" };
					break;
				}
			}
		}

		template< bool Supports11_1 >
		void unbindCombinedSampler( ID3D11DeviceContext1 * context
			, LayoutBindingWrites const & writeBinding
			, ShaderBindingMap const & bindings
			, uint32_t setIndex )
		{
			for ( auto & write : writeBinding.writes )
			{
				auto flags = writeBinding.binding.stageFlags;
				auto it = bindings.find( makeShaderBindingKey( setIndex, write.dstBinding ) );
				assert( it != bindings.end() );
				auto dstBinding = it->second;

				for ( auto i = 0u; i < write.descriptorCount; ++i )
				{
					uint32_t bindingIndex = dstBinding + write.dstArrayElement + i;
					tryBind< Supports11_1 >( context
						, bindingIndex
						, flags
						, ( ID3D11ShaderResourceView * )nullptr
						, ( ID3D11SamplerState * )nullptr );
				}
			}
		}
		
		template< bool Supports11_1 >
		void unbindInputAttachment( ID3D11DeviceContext1 * context
			, LayoutBindingWrites const & writeBinding
			, ShaderBindingMap const & bindings
			, uint32_t setIndex )
		{
			for ( auto & write : writeBinding.writes )
			{
				auto flags = writeBinding.binding.stageFlags;
				auto it = bindings.find( makeShaderBindingKey( setIndex, write.dstBinding ) );
				assert( it != bindings.end() );
				auto dstBinding = it->second;

				for ( auto i = 0u; i < write.descriptorCount; ++i )
				{
					uint32_t bindingIndex = dstBinding + write.dstArrayElement + i;
					tryBind< Supports11_1 >( context
						, bindingIndex
						, flags
						, ( ID3D11ShaderResourceView * )nullptr
						, ( ID3D11SamplerState * )nullptr );
				}
			}
		}

		template< bool Supports11_1 >
		void unbindSampler( ID3D11DeviceContext1 * context
			, LayoutBindingWrites const & writeBinding
			, ShaderBindingMap const & bindings
			, uint32_t setIndex )
		{
			for ( auto & write : writeBinding.writes )
			{
				auto flags = writeBinding.binding.stageFlags;
				auto it = bindings.find( makeShaderBindingKey( setIndex, write.dstBinding ) );
				assert( it != bindings.end() );
				auto dstBinding = it->second;

				for ( auto i = 0u; i < write.descriptorCount; ++i )
				{
					uint32_t bindingIndex = dstBinding + write.dstArrayElement + i;
					tryBind< Supports11_1 >( context
						, bindingIndex
						, flags
						, ( ID3D11SamplerState * )nullptr );
				}
			}
		}

		template< bool Supports11_1 >
		void unbindSampledTexture( ID3D11DeviceContext1 * context
			, LayoutBindingWrites const & writeBinding
			, ShaderBindingMap const & bindings
			, uint32_t setIndex )
		{
			for ( auto & write : writeBinding.writes )
			{
				auto flags = writeBinding.binding.stageFlags;
				auto it = bindings.find( makeShaderBindingKey( setIndex, write.dstBinding ) );
				assert( it != bindings.end() );
				auto dstBinding = it->second;

				for ( auto i = 0u; i < write.descriptorCount; ++i )
				{
					uint32_t bindingIndex = dstBinding + write.dstArrayElement + i;
					tryBind< Supports11_1 >( context
						, bindingIndex
						, flags
						, ( ID3D11ShaderResourceView * )nullptr );
				}
			}
		}

		template< bool Supports11_1 >
		void unbindUniformBuffer( ID3D11DeviceContext1 * context
			, LayoutBindingWrites const & writeBinding
			, ShaderBindingMap const & bindings
			, uint32_t setIndex )
		{
			for ( auto & write : writeBinding.writes )
			{
				auto flags = writeBinding.binding.stageFlags;
				auto it = bindings.find( makeShaderBindingKey( setIndex, write.dstBinding ) );
				assert( it != bindings.end() );
				auto dstBinding = it->second;

				for ( auto i = 0u; i < write.descriptorCount; ++i )
				{
					uint32_t bindingIndex = dstBinding + write.dstArrayElement + i;
					auto range = std::min( write.pBufferInfo[i].range
						, get( write.pBufferInfo[i].buffer )->getSize() );
					tryBind< Supports11_1 >( context
						, bindingIndex
						, flags
						, ( ID3D11Buffer * )nullptr
						, UINT( write.pBufferInfo[i].offset / 16 )
						, UINT( ashes::getAlignedSize( range, 256ULL ) / 16ULL ) );
				}
			}
		}

		template< bool Supports11_1 >
		void unbindUniformTexelBuffer( ID3D11DeviceContext1 * context
			, LayoutBindingWrites const & writeBinding
			, ShaderBindingMap const & bindings
			, uint32_t setIndex )
		{
			for ( auto & write : writeBinding.writes )
			{
				auto flags = writeBinding.binding.stageFlags;
				auto it = bindings.find( makeShaderBindingKey( setIndex, write.dstBinding ) );
				assert( it != bindings.end() );
				auto dstBinding = it->second;

				for ( auto i = 0u; i < write.descriptorCount; ++i )
				{
					uint32_t bindingIndex = dstBinding + write.dstArrayElement + i;
					tryBind< Supports11_1 >( context
						, bindingIndex
						, flags
						, ( ID3D11ShaderResourceView * )nullptr );
				}
			}
		}

		template< bool Supports11_1 >
		void unbindDynamicUniformBuffer( ID3D11DeviceContext1 * context
			, LayoutBindingWrites const & writeBinding
			, ShaderBindingMap const & bindings
			, uint32_t setIndex
			, uint32_t offset )
		{
			for ( auto & write : writeBinding.writes )
			{
				auto flags = writeBinding.binding.stageFlags;
				auto it = bindings.find( makeShaderBindingKey( setIndex, write.dstBinding ) );
				assert( it != bindings.end() );
				auto dstBinding = it->second;

				for ( auto i = 0u; i < write.descriptorCount; ++i )
				{
					uint32_t bindingIndex = dstBinding + write.dstArrayElement + i;
					auto range = std::min( write.pBufferInfo[i].range
						, get( write.pBufferInfo[i].buffer )->getSize() );
					tryBind< Supports11_1 >( context
						, bindingIndex
						, flags
						, ( ID3D11Buffer * )nullptr
						, UINT( ( offset + write.pBufferInfo[i].offset ) / 16 )
						, UINT( ashes::getAlignedSize( range, 256ULL ) / 16ULL ) );
				}
			}
		}

		template< bool Supports11_1 >
		void unbindStorageImage( ID3D11DeviceContext1 * context
			, LayoutBindingWrites const & writeBinding
			, ShaderBindingMap const & bindings
			, uint32_t setIndex )
		{
			for ( auto & write : writeBinding.writes )
			{
				auto flags = writeBinding.binding.stageFlags;
				auto it = bindings.find( makeShaderBindingKey( setIndex, write.dstBinding ) );
				assert( it != bindings.end() );
				auto dstBinding = it->second;

				for ( auto i = 0u; i < write.descriptorCount; ++i )
				{
					uint32_t bindingIndex = dstBinding + write.dstArrayElement + i;
					tryBindOne< VK_SHADER_STAGE_COMPUTE_BIT, Supports11_1 >( context
						, bindingIndex
						, flags
						, ( ID3D11UnorderedAccessView * )nullptr );
				}
			}
		}

		template< bool Supports11_1 >
		void unbindStorageTexelBuffer( ID3D11DeviceContext1 * context
			, LayoutBindingWrites const & writeBinding
			, ShaderBindingMap const & bindings
			, uint32_t setIndex )
		{
			for ( auto & write : writeBinding.writes )
			{
				auto flags = writeBinding.binding.stageFlags;
				auto it = bindings.find( makeShaderBindingKey( setIndex, write.dstBinding ) );
				assert( it != bindings.end() );
				auto dstBinding = it->second;

				for ( auto i = 0u; i < write.descriptorCount; ++i )
				{
					uint32_t bindingIndex = dstBinding + write.dstArrayElement + i;
					tryBind< Supports11_1 >( context
						, bindingIndex
						, flags
						, ( ID3D11ShaderResourceView * )nullptr );
				}
			}
		}

		template< bool Supports11_1 >
		void unbindStorageBuffer( ID3D11DeviceContext1 * context
			, LayoutBindingWrites const & writeBinding
			, ShaderBindingMap const & bindings
			, uint32_t setIndex )
		{
			for ( auto & write : writeBinding.writes )
			{
				auto flags = writeBinding.binding.stageFlags;
				auto it = bindings.find( makeShaderBindingKey( setIndex, write.dstBinding ) );
				assert( it != bindings.end() );
				auto dstBinding = it->second;

				for ( auto i = 0u; i < write.descriptorCount; ++i )
				{
					uint32_t bindingIndex = dstBinding + write.dstArrayElement + i;
					tryBindOne< VK_SHADER_STAGE_COMPUTE_BIT, Supports11_1 >( context
						, bindingIndex
						, flags
						, ( ID3D11UnorderedAccessView * )nullptr );
				}
			}
		}

		template< bool Supports11_1 >
		void unbindDynamicStorageBuffer( ID3D11DeviceContext1 * context
			, LayoutBindingWrites const & writeBinding
			, ShaderBindingMap const & bindings
			, uint32_t setIndex
			, uint32_t /*offset*/ )
		{
			for ( auto & write : writeBinding.writes )
			{
				auto flags = writeBinding.binding.stageFlags;
				auto it = bindings.find( makeShaderBindingKey( setIndex, write.dstBinding ) );
				assert( it != bindings.end() );
				auto dstBinding = it->second;

				for ( auto i = 0u; i < write.descriptorCount; ++i )
				{
					uint32_t bindingIndex = dstBinding + write.dstArrayElement + i;
					tryBindOne< VK_SHADER_STAGE_COMPUTE_BIT, Supports11_1 >( context
						, bindingIndex
						, flags
						, ( ID3D11UnorderedAccessView * )nullptr );
				}
			}
		}

		template< bool Supports11_1 >
		void unbindDynamicBuffers( ID3D11DeviceContext1 * context
			, LayoutBindingWritesArray const & writes
			, ShaderBindings const & bindings
			, uint32_t setIndex
			, UInt32Array const & offsets )
		{
			for ( auto i = 0u; i < offsets.size(); ++i )
			{
				auto & writeBinding = *writes[i];

				switch ( writeBinding.binding.descriptorType )
				{
				case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:
					unbindDynamicUniformBuffer< Supports11_1 >( context, writeBinding, bindings.ubo, setIndex, offsets[i] );
					break;

				case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC:
					unbindDynamicStorageBuffer< Supports11_1 >( context, writeBinding, bindings.tbo, setIndex, offsets[i] );
					break;

				default:
					assert( false && "Unsupported dynamic descriptor type" );
					throw ashes::BaseException{ "Unsupported dynamic descriptor type" };
					break;
				}
			}
		}

		template< bool Supports11_1 >
		void bindAll( Context const & context
			, VkPipelineLayout pipelineLayout
			, VkDescriptorSet descriptorSet
			, UInt32Array const & dynamicOffsets )
		{
			auto d3dDescriptorSet = get( descriptorSet );
			auto & bindings = get( pipelineLayout )->getShaderBindings();
			auto setIndex = get( pipelineLayout )->getDescriptorSetIndex( descriptorSet );

			for ( auto & write : d3dDescriptorSet->getInputAttachments() )
			{
				bindInputAttachment< Supports11_1 >( context.context1, get( context.device )->getSampler(), *write, bindings.tex, setIndex );
			}

			for ( auto & write : d3dDescriptorSet->getCombinedTextureSamplers() )
			{
				bindCombinedSampler< Supports11_1 >( context.context1, *write, bindings.tex, setIndex );
			}

			for ( auto & write : d3dDescriptorSet->getSamplers() )
			{
				bindSampler< Supports11_1 >( context.context1, *write, bindings.tex, setIndex );
			}

			for ( auto & write : d3dDescriptorSet->getSampledTextures() )
			{
				bindSampledTexture< Supports11_1 >( context.context1, *write, bindings.tex, setIndex );
			}

			for ( auto & write : d3dDescriptorSet->getUniformBuffers() )
			{
				bindUniformBuffer< Supports11_1 >( context.context1, *write, bindings.ubo, setIndex );
			}

			for ( auto & write : d3dDescriptorSet->getInlineUniforms() )
			{
				bindUniformBuffer< Supports11_1 >( context.context1, *write, bindings.ubo, setIndex );
			}

			for ( auto & write : d3dDescriptorSet->getUniformTexelBuffers() )
			{
				bindUniformTexelBuffer< Supports11_1 >( context.context1, *write, bindings.tbo, setIndex );
			}

			for ( auto & write : d3dDescriptorSet->getStorageTexelBuffers() )
			{
				unbindStorageTexelBuffer< Supports11_1 >( context.context1, *write, bindings.uav, setIndex );
			}

			for ( auto & write : d3dDescriptorSet->getStorageBuffers() )
			{
				bindStorageBuffer< Supports11_1 >( context.context1, *write, bindings.uav, setIndex );
			}

			for ( auto & write : d3dDescriptorSet->getStorageTextures() )
			{
				bindStorageImage< Supports11_1 >( context.context1, *write, bindings.uav, setIndex );
			}

			bindDynamicBuffers< Supports11_1 >( context.context1, d3dDescriptorSet->getDynamicBuffers(), bindings, setIndex, dynamicOffsets );
		}

		template< bool Supports11_1 >
		void unbindAll( Context const & context
			, VkPipelineLayout pipelineLayout
			, VkDescriptorSet descriptorSet
			, UInt32Array const & dynamicOffsets )
		{
			auto d3dDescriptorSet = get( descriptorSet );
			auto & bindings = get( pipelineLayout )->getShaderBindings();
			auto setIndex = get( pipelineLayout )->getDescriptorSetIndex( descriptorSet );

			for ( auto & write : d3dDescriptorSet->getInputAttachments() )
			{
				unbindInputAttachment< Supports11_1 >( context.context1, *write, bindings.tex, setIndex );
			}

			for ( auto & write : d3dDescriptorSet->getCombinedTextureSamplers() )
			{
				unbindCombinedSampler< Supports11_1 >( context.context1, *write, bindings.tex, setIndex );
			}

			for ( auto & write : d3dDescriptorSet->getSamplers() )
			{
				unbindSampler< Supports11_1 >( context.context1, *write, bindings.tex, setIndex );
			}

			for ( auto & write : d3dDescriptorSet->getSampledTextures() )
			{
				unbindSampledTexture< Supports11_1 >( context.context1, *write, bindings.tex, setIndex );
			}

			for ( auto & write : d3dDescriptorSet->getUniformBuffers() )
			{
				unbindUniformBuffer< Supports11_1 >( context.context1, *write, bindings.ubo, setIndex );
			}

			for ( auto & write : d3dDescriptorSet->getInlineUniforms() )
			{
				unbindUniformBuffer< Supports11_1 >( context.context1, *write, bindings.ubo, setIndex );
			}

			for ( auto & write : d3dDescriptorSet->getUniformTexelBuffers() )
			{
				unbindUniformTexelBuffer< Supports11_1 >( context.context1, *write, bindings.tbo, setIndex );
			}

			for ( auto & write : d3dDescriptorSet->getStorageTexelBuffers() )
			{
				unbindStorageTexelBuffer< Supports11_1 >( context.context1, *write, bindings.uav, setIndex );
			}

			for ( auto & write : d3dDescriptorSet->getStorageBuffers() )
			{
				unbindStorageBuffer< Supports11_1 >( context.context1, *write, bindings.uav, setIndex );
			}

			for ( auto & write : d3dDescriptorSet->getStorageTextures() )
			{
				unbindStorageImage< Supports11_1 >( context.context1, *write, bindings.uav, setIndex );
			}

			unbindDynamicBuffers< Supports11_1 >( context.context1, d3dDescriptorSet->getDynamicBuffers(), bindings, setIndex, dynamicOffsets );
		}

		std::map< UINT, ID3D11UnorderedAccessView * > gatherUavs( VkDescriptorSet descriptorSet
			, VkPipelineLayout pipelineLayout
			, UInt32Array const & dynamicOffsets )
		{
			auto d3dDescriptorSet = get( descriptorSet );
			auto & bindings = get( pipelineLayout )->getShaderBindings();
			auto setIndex = get( pipelineLayout )->getDescriptorSetIndex( descriptorSet );
			std::map< UINT, ID3D11UnorderedAccessView * > result;

			for ( auto & write : d3dDescriptorSet->getStorageBuffers() )
			{
				gatherStorageBuffer( *write, bindings.uav, setIndex, result );
			}

			for ( auto & write : d3dDescriptorSet->getStorageTextures() )
			{
				gatherStorageImage( *write, bindings.uav, setIndex, result );
			}

			gatherDynamicBuffers( d3dDescriptorSet->getDynamicBuffers(), bindings, setIndex, result, dynamicOffsets );
			return result;
		}
	}

	BindDescriptorSetCommand::BindDescriptorSetCommand( VkDevice context
		, VkDescriptorSet descriptorSet
		, VkPipelineLayout layout
		, ArrayView< uint32_t const > const & dynamicOffsets
		, VkPipelineBindPoint bindingPoint )
		: CommandBase{ context }
		, m_descriptorSet{ descriptorSet }
		, m_layout{ layout }
		, m_bindingPoint{ bindingPoint }
		, m_dynamicOffsets{ dynamicOffsets.begin(), dynamicOffsets.end() }
		, m_uavs{ gatherUavs( m_descriptorSet, m_layout, m_dynamicOffsets ) }
	{
		assert( get( m_descriptorSet )->getDynamicBuffers().size() == m_dynamicOffsets.size()
			&& "Dynamic descriptors and dynamic offsets sizes must match." );
	}

	void BindDescriptorSetCommand::apply( Context const & context )const
	{
		if ( context.featureLevel >= D3D_FEATURE_LEVEL_11_1 )
		{
			bindAll< true >( context, m_layout, m_descriptorSet, m_dynamicOffsets );
		}
		else
		{
			bindAll< false >( context, m_layout, m_descriptorSet, m_dynamicOffsets );
		}

		std::map< UINT, ID3D11UnorderedAccessView * > newUavs = context.uavs;

		for ( auto const & [id, uav] : m_uavs )
		{
			newUavs[id] = uav;
		}

		if ( bool dirtyUavs = context.uavs != newUavs;
			dirtyUavs )
		{
			context.uavs = std::move( newUavs );
			context.rawUavs.clear();

			for ( auto const & [id, uav] : context.uavs )
			{
				context.rawUavs.push_back( uav );
			}

			if ( !context.rawUavs.empty() )
			{
				context.context1->OMSetRenderTargetsAndUnorderedAccessViews( D3D11_KEEP_RENDER_TARGETS_AND_DEPTH_STENCIL
					, nullptr
					, nullptr
					, context.uavStart
					, UINT( context.rawUavs.size() )
					, context.rawUavs.data()
					, nullptr );
			}
		}
	}

	void BindDescriptorSetCommand::remove( Context const & context )const
	{
		if ( context.featureLevel >= D3D_FEATURE_LEVEL_11_1 )
		{
			unbindAll< true >( context, m_layout, m_descriptorSet, m_dynamicOffsets );
		}
		else
		{
			unbindAll< false >( context, m_layout, m_descriptorSet, m_dynamicOffsets );
		}
	}

	void BindDescriptorSetCommand::fillContext( Context & context )const
	{
	}

	CommandPtr BindDescriptorSetCommand::clone()const
	{
		return std::make_unique< BindDescriptorSetCommand >( *this );
	}
}
