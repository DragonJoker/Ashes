#pragma once

#include "Prerequisites.hpp"

#include <Renderer/Connection.hpp>
#include <Renderer/Device.hpp>
#include <Renderer/Pipeline.hpp>
#include <Renderer/PipelineLayout.hpp>
#include <Renderer/Sampler.hpp>

#include <wx/panel.h>

#include <array>

namespace vkapp
{
	class RenderPanel
		: public wxPanel
	{
	public:
		RenderPanel( wxWindow * parent
			, wxSize const & size
			, renderer::Renderer const & vulkan );
		~RenderPanel();

	private:
		/**
		*@~french
		*@brief
		*	Cr�e le device logique.
		*@~english
		*@brief
		*	Creates the logical device.
		*/
		void doCreateDevice( renderer::Renderer const & vulkan );
		/**
		*@~french
		*@brief
		*	Cr�e la swap chain.
		*@~english
		*@brief
		*	Creates the swap chain.
		*/
		void doCreateSwapChain();
		/**
		*@~french
		*@brief
		*	Cr�e la texture.
		*@~english
		*@brief
		*	Creates the texture.
		*/
		void doCreateTexture();
		/**
		*@~french
		*@brief
		*	Cr�e le tampon de variables uniformes.
		*@~english
		*@brief
		*	Creates the uniform variables buffer.
		*/
		void doCreateUniformBuffer();
		/**
		*@~french
		*@brief
		*	Cr�e le descriptors layout/pool/set.
		*@~english
		*@brief
		*	Creates the descriptors layout/pool/set.
		*/
		void doCreateDescriptorSet();
		/**
		*@~french
		*@brief
		*	Cr�e la passe de rendu.
		*@~english
		*@brief
		*	Creates the render pass.
		*/
		void doCreateRenderPass();
		/**
		*@~french
		*@brief
		*	Cr�e le tampon de sommets.
		*@~english
		*@brief
		*	Creates the vertex buffer.
		*/
		void doCreateVertexBuffer();
		/**
		*@~french
		*@brief
		*	Cr�e le tampon de transfert.
		*@~english
		*@brief
		*	Creates the transfer buffer.
		*/
		void doCreateStagingBuffer();
		/**
		*@~french
		*@brief
		*	Cr�e le pipeline de rendu.
		*@~english
		*@brief
		*	Creates the rendering pipeline.
		*/
		void doCreatePipeline();
		/**
		*@~french
		*@brief
		*	Cr�e un framebuffer pour les ressources de rendu donn�es, partir de la vue donn�e.
		*@param[in] view
		*	La vue pour laquelle est cr�� le framebuffer.
		*@param[out] resources
		*	Re�oit le framebuffer cr��.
		*@~english
		*@brief
		*	Creates the framebuffer for given rendering resouces, from given view.
		*@param[in] view
		*	The view from which is created the framebuffer.
		*@param[out] resources
		*	Receives the created framebuffer.
		*/
		void doCreateFrameBuffer( vk::ImageView const & view
			, renderer::RenderingResources & resources );
		/**
		*@~french
		*@brief
		*	Enregistre les commandes de dessin d'une frame.
		*@param[in] resources
		*	Les ressources de rendu de la frame.
		*@param[in] backBuffer
		*	L'image sur laquelle sera dessin�e la frame.
		*@~english
		*@brief
		*	Records the commands necessary to draw one frame.
		*@param[in] resources
		*	The frame's rendering resources.
		*@param[in] backBuffer
		*	The image on which the frame will be rendered.
		*/
		bool doPrepareFrame( renderer::RenderingResources & resources );
		/**
		*@~french
		*@brief
		*	Dessine la sc�ne.
		*@~english
		*@brief
		*	Draws the scene.
		*/
		void doDraw();
		/**
		*@~french
		*@brief
		*	V�rifie si le code r�sultat donn� correspond � un besoin de r�initialiser la swapchain
		*@return
		*	\p true si le rendu peut continuer.
		*@~english
		*@brief
		*	Check if the given result code is pointing to a need of swap chain reinitialisation.
		*@return
		*	\p true if the render can carry on.
		*/
		bool doCheckNeedReset( VkResult errCode
			, bool acquisition
			, char const * const action );
		/**
		*@~french
		*@brief
		*	R�initialise la swap chain et toutes ses d�pendances.
		*@remarks
		*	Peut �tre n�cessaire en cas de redimensionnement de la fen�tre, par exemple.
		*@~english
		*@brief
		*	Resets the swap chain and all needed dependencies.
		*@remarks
		*	Can be necessary, for example when the window is resized.
		*/
		void doResetSwapChain();
		/**
		*@~french
		*@brief
		*	Fonction appel�e par le timer de rendu.
		*@~english
		*@brief
		*	Function called by the render timer.
		*/
		void onTimer( wxTimerEvent & event );
		/**
		*@~french
		*@brief
		*	Fonction appel�e lors du redimensionnement de la fen�tre.
		*@~english
		*@brief
		*	Function called when the window is resized.
		*/
		void onSize( wxSizeEvent & event );

	private:
		wxTimer * m_timer{ nullptr };
		renderer::DevicePtr m_device;
		renderer::SwapChainPtr m_swapChain;
		vk::RenderPassPtr m_renderPass;
		renderer::ShaderProgramPtr m_program;
		renderer::PipelineLayoutPtr m_pipelineLayout;
		renderer::PipelinePtr m_pipeline;
		renderer::VertexBufferPtr< TexturedVertexData > m_vertexBuffer;
		renderer::StagingBufferPtr m_stagingBuffer;
		renderer::TexturePtr m_texture;
		renderer::SamplerPtr m_sampler;
		renderer::DescriptorSetLayoutPtr m_descriptorLayout;
		renderer::DescriptorSetPoolPtr m_descriptorPool;
		renderer::DescriptorSetPtr m_descriptorSet;
		renderer::UniformBufferPtr< utils::Mat4 > m_uniformBuffer;
		std::vector< TexturedVertexData > m_vertexData;
	};
}
