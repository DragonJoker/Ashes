/**
*\file
*	CameraState.h
*\author
*	Sylvain Doremus
*/
#ifndef ___DesktopUtils_CameraState_HPP___
#define ___DesktopUtils_CameraState_HPP___
#pragma once

#include <Renderer/Angle.hpp>
#include <Renderer/RangedValue.hpp>

namespace render
{
	/**
	*\brief
	*	Classe de gestion des déplacements de la caméra.
	*/
	class CameraState
	{
	public:
		/**
		*\brief
		*	Remet les vitesses et angles à 0.
		*/
		void reset();
		/**
		*\brief
		*	Met à jour l'angle et le zoom en fonction des vitesses.
		*	
		*/
		void update();
		/**
		*\return
		*	La rotation voulue pour la caméra.
		*/
		renderer::Quaternion rotation()const noexcept;
		/**
		*\brief
		*	Définit les angles d'ouverture minimal et maximal.
		*\param[in] fovyMin, fovyMax
		*	Les bornes.
		*/
		inline void zoomBounds( renderer::Angle const & fovyMin
			, renderer::Angle const & fovyMax )noexcept
		{
			m_fovy.updateRange( makeRange( fovyMin, fovyMax ) );
		}
		/**
		*\return
		*	Les bornes du zoom.
		*/
		inline renderer::Range< renderer::Angle > const & zoomBounds()const noexcept
		{
			return m_fovy.range();
		}
		/**
		*\brief
		*	Définit la vitesse de rotation de la caméra.
		*param[in] value
		*	La nouvelle valeur.
		*/
		inline void velocity( renderer::Vec2 const & value )noexcept
		{
			m_velocityX = renderer::Degrees{ value.x };
			m_velocityY = renderer::Degrees{ value.y };
		}
		/**
		*\brief
		*	Définit la vitesse de zoom de la caméra.
		*param[in] value
		*	La nouvelle valeur.
		*/
		inline void zoomVelocity( float value )noexcept
		{
			m_zoomVelocity = renderer::Degrees{ value };
		}
		/**
		*\return
		*	La valeur du zoom.
		*/
		inline renderer::Angle const & zoom()const noexcept
		{
			return m_fovy.value();
		}

	private:
		//! L'intervalle d'angles de rotation.
		renderer::Range< renderer::Radians > m_angleRange
		{
			renderer::Radians{ float( -renderer::PiDiv2 ) },
			renderer::Radians{ float( renderer::PiDiv2 ) }
		};
		//! La rotation sur l'axe X.
		renderer::RangedValue< renderer::Radians > m_angleX
		{
			0.0_radians,
			m_angleRange
		};
		//! La rotation sur l'axe Y.
		renderer::RangedValue< renderer::Radians > m_angleY
		{
			0.0_radians,
			m_angleRange
		};
		//! La rotation sur l'axe X.
		renderer::Quaternion m_quatX;
		//! La rotation sur l'axe Y.
		renderer::Quaternion m_quatY;
		//! L'intervalle de vitesse de rotation.
		renderer::Range< renderer::Degrees > m_velocityRange
		{
			-5.0_degrees,
			5.0_degrees
		};
		//! La vitesse de rotation sur l'axe X.
		renderer::RangedValue< renderer::Degrees > m_velocityX
		{
			0.0_degrees,
			m_velocityRange
		};
		//! La vitesse de rotation sur l'axe Y.
		renderer::RangedValue< renderer::Degrees > m_velocityY
		{
			0.0_degrees,
			m_velocityRange
		};
		//! La vitesse de zoom.
		renderer::RangedValue< renderer::Degrees > m_zoomVelocity
		{
			0.0_degrees,
			renderer::makeRange( -2.0_degrees, 2.0_degrees )
		};
		//! Le FovY utilisé pour le zoom, borné dans son intervalle.
		renderer::RangedValue< renderer::Angle > m_fovy
		{
			renderer::Angle{ 45.0_degrees },
			renderer::makeRange( renderer::Angle{ 0.1_degrees }, renderer::Angle{ 45.0_degrees } )
		};
	};
}

#endif
