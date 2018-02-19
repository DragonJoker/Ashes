/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#ifndef ___Renderer_FormatGetter_HPP___
#define ___Renderer_FormatGetter_HPP___
#pragma once

namespace renderer
{
	namespace details
	{
		/**
		*\~english
		*\brief
		*	Template struct used to retrieve the attribute format type associated with a C type.
		*\~french
		*\brief
		*	Structure template utilisée pour récupérer un format d'attribut depuis un type C.
		*/
		template< typename T >
		struct FormatGetter;
		/**
		*\~english
		*\brief
		*	Template struct used to retrieve the attribute format type associated with a C type.
		*\remarks
		*	Specialisation for \p float.
		*\~french
		*\brief
		*	Structure template utilisée pour récupérer un format d'attribut depuis un type C.
		*\remarks
		*	Spécialisation pour \p float.
		*/
		template<>
		struct FormatGetter< float >
		{
			static AttributeFormat constexpr value = AttributeFormat::eFloat;
		};
		/**
		*\~english
		*\brief
		*	Template struct used to retrieve the attribute format type associated with a C type.
		*\remarks
		*	Specialisation for \p Vec2.
		*\~french
		*\brief
		*	Structure template utilisée pour récupérer un format d'attribut depuis un type C.
		*\remarks
		*	Spécialisation pour \p Vec2.
		*/
		template<>
		struct FormatGetter< Vec2 >
		{
			static AttributeFormat constexpr value = AttributeFormat::eVec2f;
		};
		/**
		*\~english
		*\brief
		*	Template struct used to retrieve the attribute format type associated with a C type.
		*\remarks
		*	Specialisation for \p Vec3.
		*\~french
		*\brief
		*	Structure template utilisée pour récupérer un format d'attribut depuis un type C.
		*\remarks
		*	Spécialisation pour \p Vec3.
		*/
		template<>
		struct FormatGetter< Vec3 >
		{
			static AttributeFormat constexpr value = AttributeFormat::eVec3f;
		};
		/**
		*\~english
		*\brief
		*	Template struct used to retrieve the attribute format type associated with a C type.
		*\remarks
		*	Specialisation for \p Vec4.
		*\~french
		*\brief
		*	Structure template utilisée pour récupérer un format d'attribut depuis un type C.
		*\remarks
		*	Spécialisation pour \p Vec4.
		*/
		template<>
		struct FormatGetter< Vec4 >
		{
			static AttributeFormat constexpr value = AttributeFormat::eVec4f;
		};
		/**
		*\~english
		*\brief
		*	Template struct used to retrieve the attribute format type associated with a C type.
		*\remarks
		*	Specialisation for \p Mat4.
		*\~french
		*\brief
		*	Structure template utilisée pour récupérer un format d'attribut depuis un type C.
		*\remarks
		*	Spécialisation pour \p Mat4.
		*/
		template<>
		struct FormatGetter< Mat4 >
		{
			static AttributeFormat constexpr value = AttributeFormat::eMat4f;
		};
		/**
		*\~english
		*\brief
		*	Template struct used to retrieve the attribute format type associated with a C type.
		*\remarks
		*	Specialisation for \p int.
		*\~french
		*\brief
		*	Structure template utilisée pour récupérer un format d'attribut depuis un type C.
		*\remarks
		*	Spécialisation pour \p int.
		*/
		template<>
		struct FormatGetter< int >
		{
			static AttributeFormat constexpr value = AttributeFormat::eInt;
		};
		/**
		*\~english
		*\brief
		*	Template struct used to retrieve the attribute format type associated with a C type.
		*\remarks
		*	Specialisation for \p IVec2.
		*\~french
		*\brief
		*	Structure template utilisée pour récupérer un format d'attribut depuis un type C.
		*\remarks
		*	Spécialisation pour \p IVec2.
		*/
		template<>
		struct FormatGetter< IVec2 >
		{
			static AttributeFormat constexpr value = AttributeFormat::eVec2i;
		};
		/**
		*\~english
		*\brief
		*	Template struct used to retrieve the attribute format type associated with a C type.
		*\remarks
		*	Specialisation for \p IVec3.
		*\~french
		*\brief
		*	Structure template utilisée pour récupérer un format d'attribut depuis un type C.
		*\remarks
		*	Spécialisation pour \p IVec3.
		*/
		template<>
		struct FormatGetter< IVec3 >
		{
			static AttributeFormat constexpr value = AttributeFormat::eVec3i;
		};
		/**
		*\~english
		*\brief
		*	Template struct used to retrieve the attribute format type associated with a C type.
		*\remarks
		*	Specialisation for \p IVec4.
		*\~french
		*\brief
		*	Structure template utilisée pour récupérer un format d'attribut depuis un type C.
		*\remarks
		*	Spécialisation pour \p IVec4.
		*/
		template<>
		struct FormatGetter< IVec4 >
		{
			static AttributeFormat constexpr value = AttributeFormat::eVec4i;
		};
		/**
		*\~english
		*\brief
		*	Template struct used to retrieve the attribute format type associated with a C type.
		*\remarks
		*	Specialisation for \p uint32_t.
		*\~french
		*\brief
		*	Structure template utilisée pour récupérer un format d'attribut depuis un type C.
		*\remarks
		*	Spécialisation pour \p uint32_t.
		*/
		template<>
		struct FormatGetter< uint32_t >
		{
			static AttributeFormat constexpr value = AttributeFormat::eUInt;
		};
		/**
		*\~english
		*\brief
		*	Template struct used to retrieve the attribute format type associated with a C type.
		*\remarks
		*	Specialisation for \p UIVec2.
		*\~french
		*\brief
		*	Structure template utilisée pour récupérer un format d'attribut depuis un type C.
		*\remarks
		*	Spécialisation pour \p UIVec2.
		*/
		template<>
		struct FormatGetter< UIVec2 >
		{
			static AttributeFormat constexpr value = AttributeFormat::eVec2ui;
		};
		/**
		*\~english
		*\brief
		*	Template struct used to retrieve the attribute format type associated with a C type.
		*\remarks
		*	Specialisation for \p UIVec3.
		*\~french
		*\brief
		*	Structure template utilisée pour récupérer un format d'attribut depuis un type C.
		*\remarks
		*	Spécialisation pour \p UIVec3.
		*/
		template<>
		struct FormatGetter< UIVec3 >
		{
			static AttributeFormat constexpr value = AttributeFormat::eVec3ui;
		};
		/**
		*\~english
		*\brief
		*	Template struct used to retrieve the attribute format type associated with a C type.
		*\remarks
		*	Specialisation for \p UIVec4.
		*\~french
		*\brief
		*	Structure template utilisée pour récupérer un format d'attribut depuis un type C.
		*\remarks
		*	Spécialisation pour \p UIVec4.
		*/
		template<>
		struct FormatGetter< UIVec4 >
		{
			static AttributeFormat constexpr value = AttributeFormat::eVec4ui;
		};
	}
}

#endif
