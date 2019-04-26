/**
*\file
*	Image.h
*\author
*	Sylvain Doremus
*/
#ifndef ___D3D11Renderer_TextureView_HPP___
#define ___D3D11Renderer_TextureView_HPP___
#pragma once

#include "renderer/D3D11Renderer/D3D11RendererPrerequisites.hpp"

namespace ashes::d3d11
{
	/**
	*\~french
	*\brief
	*	Description d'une vue sur une image Vulkan.
	*\~english
	*\brief
	*	Vulkan image view wrapper.
	*/
	class ImageView
	{
	public:
		ImageView( VkDevice device
			, VkImageViewCreateInfo createInfo );
		~ImageView();

		inline ID3D11RenderTargetView * getRenderTargetView()const
		{
			return m_renderTargetView;
		}

		inline ID3D11DepthStencilView * getDepthStencilView()const
		{
			return m_depthStencilView;
		}

		inline ID3D11UnorderedAccessView * getUnorderedAccessView()const
		{
			return m_unorderedAccessView;
		}

		inline ID3D11ShaderResourceView * getShaderView()const
		{
			return m_shaderView;
		}

		inline VkFormat getFormat()const
		{
			return m_createInfo.format;
		}

		inline VkImageSubresourceRange const & getSubResourceRange()const
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
		VkImage m_image;
		VkImageViewCreateInfo m_createInfo;
		ID3D11RenderTargetView * m_renderTargetView{ nullptr };
		ID3D11DepthStencilView * m_depthStencilView{ nullptr };
		ID3D11UnorderedAccessView * m_unorderedAccessView{ nullptr };
		ID3D11ShaderResourceView * m_shaderView{ nullptr };
	};
}

#endif
