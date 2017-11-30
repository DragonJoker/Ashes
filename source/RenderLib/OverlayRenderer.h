/**
*\file
*	OverlayRenderer.h
*\author
*	Sylvain Doremus
*/
#ifndef ___RenderLib_OverlayRenderer_H___
#define ___RenderLib_OverlayRenderer_H___
#pragma once

#include "FontTexture.h"
#include "BorderPanelOverlay.h"
#include "Viewport.h"
#include "UberShader.h"

#include <Renderer/DescriptorSet.hpp>
#include <Renderer/DescriptorSetLayout.hpp>
#include <Renderer/DescriptorSetPool.hpp>
#include <Renderer/Pipeline.hpp>
#include <Renderer/PipelineLayout.hpp>
#include <Renderer/ShaderProgram.hpp>
#include <Renderer/UniformBuffer.hpp>

#include <unordered_map>

namespace render
{
	/**
	*\brief
	*	Description du contenu du tampon d'uniformes pour les incrustations.
	*/
	struct OverlayUbo
	{
		utils::Mat4 modelProj;
		utils::Vec4 colour;
	};
	/**
	*\brief
	*	Noeud de rendu utilisé pour dessiner des incrustations.
	*/
	struct OverlayNode
	{
		/**
		*\brief
		*	Constructeur.
		*\param[in] text
		*	Dit si c'est un programme pour les incrustations texte.
		*/
		OverlayNode( renderer::Device const & device
			, renderer::RenderPass const & renderPass
			, bool text
			, OpacityType opacity
			, TextureFlags textures );

		//! Le programme sshader utilisé pour dessiner les incrustations.
		renderer::ShaderProgramPtr m_program;
		//! Le layout du tampon de sommets.
		renderer::VertexLayoutPtr m_layout;
		//! Le tampon de variables uniformes pour les incrustations.
		renderer::UniformBufferPtr< OverlayUbo > m_overlayUbo;
		//! Le layout des descriptor sets du noeud.
		renderer::DescriptorSetLayoutPtr m_descriptorLayout;
		//! Le pool de descriptor set du noeud.
		renderer::DescriptorSetPoolPtr m_descriptorPool;
		//! Le layout du pipeline.
		renderer::PipelineLayoutPtr m_pipelineLayout;
		//! Le pipeline.
		renderer::PipelinePtr m_pipeline;
	};
	//! Tableau de noeuds de rendu d'incrustations.
	using OverlayNodeArray = std::array< OverlayNode, size_t( NodeType::eCount ) >;
	/**
	*\brief
	*	L'incrustation et son matériau.
	*/
	template< typename OvType >
	struct RenderOverlay
	{
		RenderOverlay( renderer::DescriptorSetPool const & pool
			, std::shared_ptr< OvType > overlay
			, uint32_t index
			, renderer::UniformBuffer< OverlayUbo > const & overlayUbo
			, Texture const * opacity );
		/**
		*\brief
		*	Crée le descripteur.
		*/
		void createDescriptor( uint32_t index );
		//! Le pool de set de descripteurs.
		renderer::DescriptorSetPool const & m_pool;
		//! Le tampon d'uniformes.
		renderer::UniformBuffer< OverlayUbo > const & m_overlayUbo;
		//! L'indice de l'instance.
		uint32_t m_index;
		//! La texture d'opacité.
		Texture const * m_opacity;
		//! L'incrustation.
		std::shared_ptr< OvType > m_overlay;
		//! Le descriptor set.
		renderer::DescriptorSetPtr m_descriptor;
	};
	template< typename OvType >
	using RenderOverlayPtr = std::unique_ptr< RenderOverlay< OvType > >;
	template< typename Data, typename OvType >
	struct OverlayVbo
	{
		OverlayVbo( renderer::Device const & device
			, uint32_t count = 1u );
		renderer::VertexBufferPtr< Data > m_buffer;
		std::vector< Data > m_data;
		std::vector< RenderOverlayPtr< OvType > > m_overlays;
	};
	using PanelOverlayVbo = OverlayVbo< Overlay::Quad, PanelOverlay >;
	using BorderOverlayPanelVbo = OverlayVbo< Overlay::Quad, BorderPanelOverlay >;
	using BorderOverlayBorderVbo = OverlayVbo< BorderPanelOverlay::BorderQuads, BorderPanelOverlay >;
	using TextOverlayVbo = OverlayVbo< Overlay::Quad, TextOverlay >;
	using PanelOverlayVboArray = std::array< PanelOverlayVbo, size_t( NodeType::eCount ) >;
	using BorderOverlayPanelVboArray = std::array< BorderOverlayPanelVbo, size_t( NodeType::eCount ) >;
	using BorderOverlayBorderVboArray = std::array< BorderOverlayBorderVbo, size_t( NodeType::eCount ) >;
	using TextOverlayVboArray = std::array< TextOverlayVbo, size_t( NodeType::eCount ) >;
	/**
	*\brief
	*	Le renderer d'incrustations.
	*/
	class OverlayRenderer
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] maxCharsPerBuffer
		*	Le nombre maximal de caractères par tampon de sommets de texte.
		*/
		explicit OverlayRenderer( renderer::Device const & device
			, renderer::RenderPass const & renderPass
			, renderer::CommandPool const & commandPool
			, utils::IVec2 const & size
			, uint32_t maxCharsPerBuffer = 600 );
		/**
		*brief
		*	Destructeur.
		*/
		~OverlayRenderer();
		/**
		*brief
		*	Enregistre un PanelOverlay.
		*\param[in] overlay
		*	L'incrustation à enregistrer.
		*/
		void registerOverlay( PanelOverlayPtr overlay );
		/**
		*brief
		*	Enregistre un BorderPanelOverlay.
		*\param[in] overlay
		*	L'incrustation à enregistrer.
		*/
		void registerOverlay( BorderPanelOverlayPtr overlay );
		/**
		*brief
		*	Enregistre un TextOverlay.
		*\param[in] overlay
		*	L'incrustation à enregistrer.
		*/
		void registerOverlay( TextOverlayPtr overlay );
		/**
		*brief
		*	Désenregistre un PanelOverlay.
		*\param[in] overlay
		*	L'incrustation à désenregistrer.
		*/
		void unregisterOverlay( OverlayPtr overlay );
		/**
		*brief
		*	Désenregistre un PanelOverlay.
		*\param[in] overlay
		*	L'incrustation à désenregistrer.
		*/
		void unregisterOverlay( PanelOverlayPtr overlay );
		/**
		*brief
		*	Désenregistre un BorderPanelOverlay.
		*\param[in] overlay
		*	L'incrustation à désenregistrer.
		*/
		void unregisterOverlay( BorderPanelOverlayPtr overlay );
		/**
		*brief
		*	Désenregistre un TextOverlay.
		*\param[in] overlay
		*	L'incrustation à désenregistrer.
		*/
		void unregisterOverlay( TextOverlayPtr overlay );
		/**
		*brief
		*	Met à jour le viewport de rendu des incrustation.
		*param[in] size
		*	Les dimensions de la fenêtre de rendu.
		*/
		void resize( utils::IVec2 const & size );
		/**
		*brief
		*	Met à jour les tampons des incrustations.
		*/
		void update();
		/**
		*brief
		*	Dessine les incrustations.
		*/
		void draw( renderer::CommandBuffer const & commandBuffer
			, OverlayList const & overlays )const;
		/**
		*brief
		*	Dessine un PanelOverlay.
		*\param[in] overlay
		*	L'incrustation à dessiner.
		*/
		void drawPanel( renderer::CommandBuffer const & commandBuffer
			, PanelOverlay const & overlay )const;
		/**
		*brief
		*	Dessine un BorderPanelOverlay.
		*\param[in] overlay
		*	L'incrustation à dessiner.
		*/
		void drawBorderPanel( renderer::CommandBuffer const & commandBuffer
			, BorderPanelOverlay const & overlay )const;
		/**
		*brief
		*	Dessine un TextOverlay.
		*\param[in] overlay
		*	L'incrustation à dessiner.
		*/
		void drawText( renderer::CommandBuffer const & commandBuffer
			, TextOverlay const & overlay )const;

	private:
		/**
		*brief
		*	Fonction de dessin d'une incrustation.
		*param[in] buffer
		*	Le tampon de la géométrie de l'incrustation.
		*param[in] count
		*	Le nombre de sommets.
		*param[in] transform
		*	La matrice de transformation de l'incrustation.
		*param[in] material
		*	Le matériau de l'incrustation.
		*param[in] node
		*	Le noeud de rendu.
		*/
		void doDrawBuffer( renderer::CommandBuffer const & commandBuffer
			, renderer::VertexBuffer< Overlay::Quad > const & buffer
			, uint32_t offset
			, uint32_t count
			, OverlayNode const & node
			, renderer::DescriptorSet const & descriptor )const;
		/**
		*brief
		*	Fonction de dessin d'une incrustation.
		*param[in] buffer
		*	Le tampon de la géométrie de l'incrustation.
		*param[in] count
		*	Le nombre de sommets.
		*param[in] transform
		*	La matrice de transformation de l'incrustation.
		*param[in] material
		*	Le matériau de l'incrustation.
		*param[in] node
		*	Le noeud de rendu.
		*/
		void doDrawBuffer( renderer::CommandBuffer const & commandBuffer
			, renderer::VertexBuffer< BorderPanelOverlay::BorderQuads > const & buffer
			, uint32_t offset
			, uint32_t count
			, OverlayNode const & node
			, renderer::DescriptorSet const & descriptor )const;
		void onOverlayChanged( Overlay & overlay );

	private:
		renderer::Device const & m_device;
		renderer::RenderPass const & m_renderPass;
		OverlayNodeArray m_panelNodes;
		OverlayNodeArray m_borderNodesPanel;
		OverlayNodeArray m_borderNodesBorder;
		OverlayNode m_textNode;
		renderer::CommandBufferPtr m_updateCommandBuffer;
		renderer::StagingBufferPtr m_stagingBuffer;
		PanelOverlayVboArray m_panelOverlays;
		BorderOverlayPanelVboArray m_borderOverlaysPanels;
		BorderOverlayBorderVboArray m_borderOverlaysBorders;
		std::vector< TextOverlayVbo > m_textOverlays;
		std::vector< utils::Connection< OnOverlayChanged > > m_connections;
		uint32_t m_maxCharsPerBuffer;
		mutable bool m_sizeChanged{ true };
		mutable bool m_changed{ true };
		utils::Mat4 m_transform;
		Viewport m_viewport;
	};
}

#endif
