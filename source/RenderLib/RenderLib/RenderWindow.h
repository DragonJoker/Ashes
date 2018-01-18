/**
*\file
*	RenderWindow.h
*\author
*	Sylvain Doremus
*/
#ifndef ___RenderLib_RenderWindow_HPP___
#define ___RenderLib_RenderWindow_HPP___
#pragma once

#include "CameraState.h"
#include "Debug.h"
#include "FontTexture.h"
#include "OverlayRenderer.h"
#include "RenderTarget.h"
#include "Scene.h"

#include <Renderer/Attribute.hpp>
#include <Renderer/CommandBuffer.hpp>
#include <Renderer/CommandPool.hpp>
#include <Renderer/DescriptorSet.hpp>
#include <Renderer/DescriptorSetLayout.hpp>
#include <Renderer/DescriptorSetPool.hpp>
#include <Renderer/ShaderProgram.hpp>
#include <Renderer/StagingBuffer.hpp>
#include <Renderer/Pipeline.hpp>
#include <Renderer/PipelineLayout.hpp>
#include <Renderer/SwapChain.hpp>

namespace render
{
	/**
	*\brief
	*	Classe de fenêtre de rendu.
	*\remarks
	*	Utilise une RenderTarget pour faire un rendu en texture,
	*	puis dessine le résultat dans la fenêtre,
	*	enfin, dessine les incrustations.
	*/
	class RenderWindow
	{
	public:
		/**
		*\brief
		*	Un sommet pour le rendu dans la fenêtre.
		*/
		struct Vertex
		{
			//! Une position en 2D.
			renderer::Vec2 position;
			//! Les coordonnées de texture.
			renderer::Vec2 texture;
		};

	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] dimensions
		*	Les dimensions de la texture recevant le rendu.
		*\param[in] debug
		*	\p true pour activer les informations de débogage.
		*\param[in] loader
		*	Le loader de police.
		*/
		RenderWindow( renderer::Device const & device
			, renderer::IVec2 const & dimensions
			, render::FontLoader & loader
			, bool debug );
		/**
		*\brief
		*	Destructeur.
		*/
		~RenderWindow();
		/**
		*\brief
		*	Démarre le dessin d'une image.
		*/
		void beginFrame();
		/**
		*\brief
		*	Met à jour la scène.
		*/
		void update();
		/**
		*\brief
		*	Met à jour les incrustations de la scène.
		*/
		void updateOverlays();
		/**
		*\brief
		*	Met à jour les données de la scène en VRAM.
		*/
		void updateUbos();
		/**
		*\brief
		*	Dessine la scène.
		*\remarks
		*	Le dessin se fait dans la cible de rendu, puis les effets éventuels
		*	sont appliqués et enfin la cible de rendu est dessinée dans le
		*	tampon de la fenêtre.
		*/
		void draw()const;
		/**
		*\brief
		*	Termine le dessin d'une image.
		*/
		void endFrame();
		/**
		*\brief
		*	Redimensionne le viewport.
		*\param[in] size
		*	Les dimensions du viewport.
		*/
		void resize( renderer::IVec2 const & size )noexcept;
		/**
		*\return
		*	Récupère les ressources de rendu par défaut.
		*/
		renderer::RenderingResources const & getDefaultResources()const
		{
			return m_swapChain->getDefaultResources();
		}
		/**
		*\return
		*	La scène.
		*/
		inline Scene const & scene()const noexcept
		{
			return m_scene;
		}
		/**
		*\return
		*	La scène.
		*/
		inline Scene & scene()noexcept
		{
			return m_scene;
		}
		/**
		*\return
		*	Le viewport de la scène.
		*/
		inline Viewport const & viewport()const noexcept
		{
			return m_scene.viewport();
		}
		/**
		*\return
		*	Le viewport de la scène.
		*/
		inline Viewport & viewport()noexcept
		{
			return m_scene.viewport();
		}
		/**
		*\return
		*	L'instance de picking.
		*/
		//inline Picking const & picking()const noexcept
		//{
		//	return m_picking;
		//}
		/**
		*\return
		*	L'état de la caméra.
		*/
		inline CameraState const & state()const noexcept
		{
			return m_scene.state();
		}
		/**
		*\return
		*	L'état de la caméra.
		*/
		inline CameraState & state()noexcept
		{
			return m_scene.state();
		}
		/**
		*\brief
		*	Active le picking pour la prochaine frame.
		*\param[in] position
		*	La position de la souris.
		*/
		inline void pick( renderer::IVec2 const & position )
		{
			m_pickPosition = position;
			m_pick = true;
		}
		/**
		*\return
		*	Le device logique.
		*/
		inline renderer::Device const & getDevice()const noexcept
		{
			return m_device;
		}
		/**
		*\return
		*	Le tampon de transfert.
		*/
		inline renderer::StagingBuffer const & getStagingBuffer()const noexcept
		{
			return *m_stagingBuffer;
		}

	private:
		/**
		*\brief
		*	Dessine une texture dans le backbuffer.
		*/
		void doPrepareCommandBuffer( uint32_t index
			, renderer::Texture const & texture )const;

	private:
		//! La swap chain.
		renderer::Device const & m_device;
		//! La swap chain.
		renderer::SwapChainPtr m_swapChain;
		//! La passe de rendu de picking.
		renderer::RenderPassPtr m_renderPass;
		//! Les tampons d'image de la swap chain.
		std::vector< renderer::FrameBufferPtr > m_frameBuffers;
		//! Les tampons de commandes, un par back buffer de la swap chain.
		std::vector< renderer::CommandBufferPtr > m_commandBuffers;
		//! Le pool de tampons de commandes de dessin de la scène.
		renderer::CommandPoolPtr m_drawCommandPool;
		//! Le tampons de commandes de dessin de la scène.
		renderer::CommandBufferPtr m_drawCommandBuffer;
		//! Le tampons de commandes de dessin de la scène.
		renderer::SemaphorePtr m_drawSemaphore;
		//! Le tampon de transfert.
		renderer::StagingBufferPtr m_stagingBuffer;
		//! Le layout des descripteurs de rendu dans la fenêtre.
		renderer::DescriptorSetLayoutPtr m_descriptorLayout;
		//! Le layout du pipeline de rendu dans la fenêtre.
		renderer::PipelineLayoutPtr m_pipelineLayout;
		//! Le pipeline de rendu dans la fenêtre.
		renderer::PipelinePtr m_pipeline;
		//! La cible de rendu.
		RenderTargetPtr m_target;
		//! Le renderer d'incrustations
		OverlayRenderer m_overlayRenderer;
		//! La scène qui sera dessinée.
		Scene m_scene;
		//! Les dimensions de la fenêtre.
		renderer::IVec2 m_size;
		//! L'échantillonneur de la texture de la cible de rendu.
		renderer::SamplerPtr m_sampler;
		//! Le programme shader utilisé pour le rendu dans la fenêtre.
		renderer::ShaderProgramPtr m_program;
		//! Le tampon GPU contenant les sommets du rendu dans la fenêtre.
		renderer::VertexBufferPtr< Vertex > m_vbo;
		//! Le layout de sommets du tampon.
		renderer::VertexLayoutPtr m_layout;
		//! Le tampon ¨de géométries.
		renderer::GeometryBuffersPtr m_vao;
		//! Le pool de descripteurs.
		renderer::DescriptorSetPoolPtr m_descriptorPool;
		//! Le set de descripteurs.
		renderer::DescriptorSetPtr m_descriptor;
		//! Le viewport du rendu dans la fenêtre.
		Viewport m_viewport;
		//! La position voulue pour le picking.
		renderer::IVec2 m_pickPosition;
		//! L'instance de picking.
		//Picking m_picking;
		//! Dit si on doit exécuter le picking lors du dessin de la prochaine frame.
		mutable bool m_pick{ false };
		//! Les informations de débogage.
		Debug m_debug;
		//! Dit si le VBO est initialisé.
		bool m_vboInitialised{ false };
		//! Les ressources de rendu de la frame actuelle.
		renderer::RenderingResources * m_resources{ nullptr };
	};
}

#endif
