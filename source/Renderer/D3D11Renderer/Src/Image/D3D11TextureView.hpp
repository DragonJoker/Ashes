/**
*\file
*	Texture.h
*\author
*	Sylvain Doremus
*/
#ifndef ___D3D11Renderer_TextureView_HPP___
#define ___D3D11Renderer_TextureView_HPP___
#pragma once

#include "D3D11RendererPrerequisites.hpp"

#include <Image/TextureView.hpp>

namespace d3d11_renderer
{
	/**
	*\~french
	*\brief
	*	Description d'une vue sur une image Vulkan.
	*\~english
	*\brief
	*	Vulkan image view wrapper.
	*/
	class TextureView
		: public renderer::TextureView
	{
	public:
		TextureView( Device const & device
			, Texture const & image
			, renderer::ImageViewCreateInfo const & createInfo );
		~TextureView();

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

	private:
		void doCreate1D();
		void doCreate1DArray();
		void doCreate2D();
		void doCreate2DArray();
		void doCreate3D();
		void doCreateCube();
		void doCreateCubeArray();

	private:
		Device const & m_device;
		Texture const & m_image;
		ID3D11RenderTargetView * m_renderTargetView{ nullptr };
		ID3D11DepthStencilView * m_depthStencilView{ nullptr };
		ID3D11UnorderedAccessView * m_unorderedAccessView{ nullptr };
		ID3D11ShaderResourceView * m_shaderView{ nullptr };
	};
}

#endif
