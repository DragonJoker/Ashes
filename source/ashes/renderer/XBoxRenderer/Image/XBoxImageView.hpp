/**
*\file
*	Image.h
*\author
*	Sylvain Doremus
*/
#ifndef ___XBOXRenderer_TextureView_HPP___
#define ___XBOXRenderer_TextureView_HPP___
#pragma once

#include "renderer/XBoxRenderer/XBoxRendererPrerequisites.hpp"

namespace ashes::xbox
{
	class ImageView
		: public NonCopyable
	{
	public:
		ImageView( VkDevice device
			, VkImageViewCreateInfo createInfo );
		~ImageView()noexcept;

		UINT getSubresource( uint32_t layer )const;

		ID3D11View * getView()const
		{
			using ViewPtr = ID3D11View*;
			return m_renderTargetView
				? ViewPtr( m_renderTargetView )
				: ( m_depthStencilView
					? ViewPtr( m_depthStencilView )
					: ( m_unorderedAccessView
						? ViewPtr( m_unorderedAccessView )
						: ( m_shaderView 
							? ViewPtr( m_shaderView )
							: nullptr ) ) );
		}

		ID3D11RenderTargetView * getRenderTargetView()const
		{
			return m_renderTargetView;
		}

		ID3D11DepthStencilView * getDepthStencilView()const
		{
			return m_depthStencilView;
		}

		ID3D11UnorderedAccessView * getUnorderedAccessView()const
		{
			return m_unorderedAccessView;
		}

		ID3D11ShaderResourceView * getShaderView()const
		{
			return m_shaderView;
		}

		VkFormat getFormat()const
		{
			return m_createInfo.format;
		}

		VkImage getImage()const
		{
			return m_createInfo.image;
		}

		VkImageSubresourceRange const & getSubResourceRange()const
		{
			return m_createInfo.subresourceRange;
		}

	private:
		void doCreate1D();
		void doCreate1DArray();
		void doCreate2D();
		void doCreate2DArray();
		void doCreate3D();
		void doCreateCube();
		void doCreateCubeArray();

	private:
		VkDevice m_device;
		VkImageViewCreateInfo m_createInfo;
		ID3D11RenderTargetView * m_renderTargetView{ nullptr };
		ID3D11DepthStencilView * m_depthStencilView{ nullptr };
		ID3D11UnorderedAccessView * m_unorderedAccessView{ nullptr };
		ID3D11ShaderResourceView * m_shaderView{ nullptr };
	};
}

#endif
