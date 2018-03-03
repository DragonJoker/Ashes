/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "Pipeline/RasterisationState.hpp"

namespace renderer
{
	RasterisationState::RasterisationState( RasterisationState const & rhs )
		: m_flags{ rhs.m_flags }
		, m_depthClampEnable{ rhs.m_depthClampEnable }
		, m_rasteriserDiscardEnable{ rhs.m_rasteriserDiscardEnable }
		, m_polygonMode{ rhs.m_polygonMode }
		, m_cullMode{ rhs.m_cullMode }
		, m_frontFace{ rhs.m_frontFace }
		, m_depthBiasEnable{ rhs.m_depthBiasEnable }
		, m_depthBiasConstantFactor{ rhs.m_depthBiasConstantFactor }
		, m_depthBiasClamp{ rhs.m_depthBiasClamp }
		, m_depthBiasSlopeFactor{ rhs.m_depthBiasSlopeFactor }
		, m_lineWidth{ rhs.m_lineWidth ? std::make_unique< float >( *rhs.m_lineWidth ) : nullptr }
		, m_hash
		{ uint16_t(
			( uint16_t( m_depthClampEnable ) << 15 )
			| ( uint16_t( m_rasteriserDiscardEnable ) << 14 )
			| ( uint16_t( m_polygonMode ) << 11 )
			| ( uint16_t( m_cullMode ) << 9 )
			| ( uint16_t( m_frontFace ) << 7 )
			| ( uint16_t( m_depthBiasEnable ) << 6 )
		) }
	{
	}

	RasterisationState & RasterisationState::operator=( RasterisationState const & rhs )
	{
		m_flags = rhs.m_flags;
		m_depthClampEnable = rhs.m_depthClampEnable;
		m_rasteriserDiscardEnable = rhs.m_rasteriserDiscardEnable;
		m_polygonMode = rhs.m_polygonMode;
		m_cullMode = rhs.m_cullMode;
		m_frontFace = rhs.m_frontFace;
		m_depthBiasEnable = rhs.m_depthBiasEnable;
		m_depthBiasConstantFactor = rhs.m_depthBiasConstantFactor;
		m_depthBiasClamp = rhs.m_depthBiasClamp;
		m_depthBiasSlopeFactor = rhs.m_depthBiasSlopeFactor;
		m_lineWidth = rhs.m_lineWidth ? std::make_unique< float >( *rhs.m_lineWidth ) : nullptr;
		m_hash = uint16_t(
			( uint16_t( m_depthClampEnable ) << 15 )
			| ( uint16_t( m_rasteriserDiscardEnable ) << 14 )
			| ( uint16_t( m_polygonMode ) << 11 )
			| ( uint16_t( m_cullMode ) << 9 )
			| ( uint16_t( m_frontFace ) << 7 )
			| ( uint16_t( m_depthBiasEnable ) << 6 )
		);
		return *this;
	}

	RasterisationState::RasterisationState( float lineWidth
		, RasterisationStateFlags flags
		, bool depthClampEnable
		, bool rasteriserDiscardEnable
		, PolygonMode polygonMode
		, CullModeFlags cullMode
		, FrontFace frontFace
		, bool depthBiasEnable
		, float depthBiasConstantFactor
		, float depthBiasClamp
		, float depthBiasSlopeFactor )
		: m_flags{ flags }
		, m_depthClampEnable{ depthClampEnable }
		, m_rasteriserDiscardEnable{ rasteriserDiscardEnable }
		, m_polygonMode{ polygonMode }
		, m_cullMode{ cullMode }
		, m_frontFace{ frontFace }
		, m_depthBiasEnable{ depthBiasEnable }
		, m_depthBiasConstantFactor{ depthBiasConstantFactor }
		, m_depthBiasClamp{ depthBiasClamp }
		, m_depthBiasSlopeFactor{ depthBiasSlopeFactor }
		, m_lineWidth{ std::make_unique< float >( lineWidth ) }
		, m_hash
		{ uint16_t(
			( uint16_t( m_depthClampEnable ) << 15 )
			| ( uint16_t( m_rasteriserDiscardEnable ) << 14 )
			| ( uint16_t( m_polygonMode ) << 11 )
			| ( uint16_t( m_cullMode ) << 9 )
			| ( uint16_t( m_frontFace ) << 7 )
			| ( uint16_t( m_depthBiasEnable ) << 6 )
		) }
	{
	}

	RasterisationState::RasterisationState( RasterisationStateFlags flags
		, bool depthClampEnable
		, bool rasteriserDiscardEnable
		, PolygonMode polygonMode
		, CullModeFlags cullMode
		, FrontFace frontFace
		, bool depthBiasEnable
		, float depthBiasConstantFactor
		, float depthBiasClamp
		, float depthBiasSlopeFactor )
		: m_flags{ flags }
		, m_depthClampEnable{ depthClampEnable }
		, m_rasteriserDiscardEnable{ rasteriserDiscardEnable }
		, m_polygonMode{ polygonMode }
		, m_cullMode{ cullMode }
		, m_frontFace{ frontFace }
		, m_depthBiasEnable{ depthBiasEnable }
		, m_depthBiasConstantFactor{ depthBiasConstantFactor }
		, m_depthBiasClamp{ depthBiasClamp }
		, m_depthBiasSlopeFactor{ depthBiasSlopeFactor }
		, m_hash
		{ uint16_t(
			( uint16_t( m_depthClampEnable ) << 15 )
			| ( uint16_t( m_rasteriserDiscardEnable ) << 14 )
			| ( uint16_t( m_polygonMode ) << 11 )
			| ( uint16_t( m_cullMode ) << 9 )
			| ( uint16_t( m_frontFace ) << 7 )
			| ( uint16_t( m_depthBiasEnable ) << 6 )
		) }
	{
	}
}
