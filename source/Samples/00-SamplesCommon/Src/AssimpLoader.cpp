#include "AssimpLoader.hpp"

#include <stdlib.h>
#include <fstream>
#include <sstream>

#include <Buffer/GeometryBuffers.hpp>
#include <Buffer/VertexBuffer.hpp>
#include <Descriptor/DescriptorSet.hpp>
#include <Descriptor/DescriptorSetLayout.hpp>
#include <Descriptor/DescriptorSetPool.hpp>
#include <Pipeline/Pipeline.hpp>
#include <Pipeline/PipelineLayout.hpp>
#include <Pipeline/VertexLayout.hpp>

#include <Utils/StringUtils.hpp>

#include <assimp/Importer.hpp> // C++ importer interface
#include <assimp/scene.h> // Output data structure
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/Importer.hpp>

namespace common
{
	namespace
	{
		template< typename aiMeshType >
		std::vector< Vertex > doCreateVertexBuffer( aiMeshType const & aiMesh
			, renderer::Vec3 & min
			, renderer::Vec3 & max )
		{
			std::vector< Vertex > result{ aiMesh.mNumVertices };
			uint32_t index{ 0u };

			for ( auto & vertex : result )
			{
				vertex.position[0] = aiMesh.mVertices[index].x;
				vertex.position[1] = aiMesh.mVertices[index].y;
				vertex.position[2] = aiMesh.mVertices[index].z;
				min[0] = std::min( vertex.position[0], min[0] );
				min[1] = std::min( vertex.position[1], min[1] );
				min[2] = std::min( vertex.position[2], min[2] );
				max[0] = std::max( vertex.position[0], max[0] );
				max[1] = std::max( vertex.position[1], max[1] );
				max[2] = std::max( vertex.position[2], max[2] );
				++index;
			}

			if ( aiMesh.HasNormals() )
			{
				index = 0u;

				for ( auto & vertex : result )
				{
					vertex.normal[0] = aiMesh.mNormals[index].x;
					vertex.normal[1] = aiMesh.mNormals[index].y;
					vertex.normal[2] = aiMesh.mNormals[index].z;
					++index;
				}
			}

			if ( aiMesh.HasTangentsAndBitangents() )
			{
				index = 0u;

				for ( auto & vertex : result )
				{
					vertex.tangent[0] = aiMesh.mTangents[index].x;
					vertex.tangent[1] = aiMesh.mTangents[index].y;
					vertex.tangent[2] = aiMesh.mTangents[index].z;
					++index;
				}

				index = 0u;

				for ( auto & vertex : result )
				{
					vertex.bitangent[0] = aiMesh.mBitangents[index].x;
					vertex.bitangent[1] = aiMesh.mBitangents[index].y;
					vertex.bitangent[2] = aiMesh.mBitangents[index].z;
					++index;
				}
			}

			if ( aiMesh.HasTextureCoords( 0 ) )
			{
				index = 0u;

				for ( auto & vertex : result )
				{
					vertex.texture[0] = aiMesh.mTextureCoords[0][index].x;
					vertex.texture[1] = aiMesh.mTextureCoords[0][index].y;
					++index;
				}
			}

			return result;
		}

		void doProcessPassBaseComponents( Material & material
			, aiMaterial const & aiMaterial )
		{
			aiColor3D ambient( 1, 1, 1 );
			aiMaterial.Get( AI_MATKEY_COLOR_AMBIENT, ambient );
			aiColor3D diffuse( 1, 1, 1 );
			aiMaterial.Get( AI_MATKEY_COLOR_DIFFUSE, diffuse );
			aiColor3D specular( 1, 1, 1 );
			aiMaterial.Get( AI_MATKEY_COLOR_SPECULAR, specular );
			aiColor3D emissive( 1, 1, 1 );
			aiMaterial.Get( AI_MATKEY_COLOR_EMISSIVE, emissive );
			float opacity = 1;
			aiMaterial.Get( AI_MATKEY_OPACITY, opacity );
			float shininess = 0.5f;
			aiMaterial.Get( AI_MATKEY_SHININESS, shininess );
			float shininessStrength = 1.0f;
			aiMaterial.Get( AI_MATKEY_SHININESS_STRENGTH, shininessStrength );
			int twoSided = 0;
			aiMaterial.Get( AI_MATKEY_TWOSIDED, twoSided );

			if ( ambient.IsBlack()
				&& diffuse.IsBlack()
				&& specular.IsBlack()
				&& emissive.IsBlack() )
			{
				diffuse.r = 1.0;
				diffuse.g = 1.0;
				diffuse.b = 1.0;
			}

			material.data.opacity = opacity;
			material.data.diffuse = renderer::RgbaColour{ diffuse.r, diffuse.g, diffuse.b, 1.0f };
			material.data.specular = renderer::RgbaColour{ specular.r, specular.g, specular.b, 1.0f };
			material.data.specular *= shininessStrength;
			material.data.emissive = renderer::RgbaColour{ emissive.r, emissive.g, emissive.b, 1.0f };

			if ( shininess > 0 )
			{
				material.data.shininess = shininess;
			}
		}

		bool doLoadTexture( std::string const & folder
			, aiString const & name
			, Image & data )
		{
			bool result = false;

			if ( name.length > 0 )
			{
				folder;
				std::string path = name.C_Str();
				auto index = 0u;
				utils::replace( path, R"(\)", "/" );

				if ( path.find( '/' ) != std::string::npos )
				{
					index = path.find_last_of( '/' )  + 1;
					path = path.substr( index );
				}

				path = folder / path;

				try
				{
					data = common::loadImage( path );
					result = true;
				}
				catch ( std::runtime_error & )
				{
					utils::replace( path, ".tga", ".jpg" );
					try
					{
						data = common::loadImage( path );
						result = true;
					}
					catch ( std::runtime_error & )
					{
						utils::replace( path, ".tga", ".png" );
						try
						{
							data = common::loadImage( path );
							result = true;
						}
						catch ( std::runtime_error & )
						{
						}
					}
				}
			}

			return result;
		}

		void doProcessPassTextures( std::string const & folder
			, Material & material
			, aiMaterial const & aiMaterial )
		{
			aiString ambTexName;
			aiMaterial.Get( AI_MATKEY_TEXTURE( aiTextureType_AMBIENT, 0 ), ambTexName );
			aiString difTexName;
			aiMaterial.Get( AI_MATKEY_TEXTURE( aiTextureType_DIFFUSE, 0 ), difTexName );
			aiString spcTexName;
			aiMaterial.Get( AI_MATKEY_TEXTURE( aiTextureType_SPECULAR, 0 ), spcTexName );
			aiString emiTexName;
			aiMaterial.Get( AI_MATKEY_TEXTURE( aiTextureType_EMISSIVE, 0 ), emiTexName );
			aiString nmlTexName;
			aiMaterial.Get( AI_MATKEY_TEXTURE( aiTextureType_NORMALS, 0 ), nmlTexName );
			aiString hgtTexName;
			aiMaterial.Get( AI_MATKEY_TEXTURE( aiTextureType_HEIGHT, 0 ), hgtTexName );
			aiString opaTexName;
			aiMaterial.Get( AI_MATKEY_TEXTURE( aiTextureType_OPACITY, 0 ), opaTexName );
			aiString shnTexName;
			aiMaterial.Get( AI_MATKEY_TEXTURE( aiTextureType_SHININESS, 0 ), shnTexName );
			Image image;
			auto index = 0u;

			if ( doLoadTexture( folder, difTexName, image ) )
			{
				material.textures.push_back( Texture{} );
				material.textures[index].data = std::move( image );
				material.data.textureOperators[index].diffuse = 1;

				if ( image.opacity )
				{
					material.data.textureOperators[index].opacity = 8u;
				}

				++index;
			}

			if ( doLoadTexture( folder, spcTexName, image ) )
			{
				material.textures.push_back( Texture{} );
				material.textures[index].data = std::move( image );
				material.data.textureOperators[index].specular = 1;

				if ( image.opacity )
				{
					material.data.textureOperators[index].shininess = 8u;
				}

				++index;
			}

			if ( doLoadTexture( folder, emiTexName, image ) )
			{
				material.textures.push_back( Texture{} );
				material.textures[index].data = std::move( image );
				material.data.textureOperators[index].emissive = 1;
				++index;
			}

			if ( doLoadTexture( folder, shnTexName, image ) )
			{
				material.textures.push_back( Texture{} );
				material.textures[index].data = std::move( image );
				material.data.textureOperators[index].shininess = 1;
				++index;
			}

			if ( doLoadTexture( folder, opaTexName, image ) )
			{
				material.textures.push_back( Texture{} );
				material.textures[index].data = std::move( image );

				if ( image.opacity )
				{
					material.data.textureOperators[index].opacity = 8;
				}
				else
				{
					material.data.textureOperators[index].opacity = 1;
				}

				++index;
			}

			material.data.texturesCount = index;
		}
	}

	Object loadObject( std::string const & folder
		, std::string const & fileName )
	{
		Assimp::Importer importer;
		uint32_t flags = aiProcess_Triangulate
			| aiProcess_JoinIdenticalVertices
			| aiProcess_OptimizeMeshes
			| aiProcess_OptimizeGraph
			| aiProcess_FixInfacingNormals
			| aiProcess_LimitBoneWeights
			| aiProcess_Debone
			| aiProcess_GenSmoothNormals
			| aiProcess_CalcTangentSpace
			| aiProcess_FlipWindingOrder
			| aiProcess_FlipUVs;
		aiScene const * aiScene = importer.ReadFile( folder / fileName, flags );
		Object result;

		if ( aiScene && aiScene->HasMeshes() )
		{
			renderer::Vec3 min{ std::numeric_limits< float >::max() };
			renderer::Vec3 max{ std::numeric_limits< float >::lowest() };

			for ( size_t meshIndex = 0; meshIndex < aiScene->mNumMeshes; ++meshIndex )
			{
				auto & aiMesh = *aiScene->mMeshes[meshIndex];
				Submesh submesh;

				if ( aiMesh.HasFaces() && aiMesh.HasPositions() )
				{
					if ( aiMesh.mMaterialIndex < aiScene->mNumMaterials )
					{
						auto & aiMaterial = *aiScene->mMaterials[aiMesh.mMaterialIndex];
						aiString mtlname;
						aiMaterial.Get( AI_MATKEY_NAME, mtlname );
						doProcessPassBaseComponents( submesh.material, aiMaterial );
						doProcessPassTextures( folder, submesh.material, aiMaterial );
					}

					submesh.vbo.data = doCreateVertexBuffer( aiMesh, min, max );

					for ( size_t faceIndex = 0u; faceIndex < aiMesh.mNumFaces; ++faceIndex )
					{
						auto & face = aiMesh.mFaces[faceIndex];

						if ( face.mNumIndices == 3 )
						{
							submesh.ibo.data.push_back( face.mIndices[0] );
							submesh.ibo.data.push_back( face.mIndices[1] );
							submesh.ibo.data.push_back( face.mIndices[2] );
						}
					}

					result.emplace_back( std::move( submesh ) );
				}
			}

			// Rescale the model to size 4.
			auto diff = max - min;
			float scale = 8.0f / std::max( diff[0], std::max( diff[1], diff[2] ) );

			for ( auto & submesh : result )
			{
				for ( auto & vertex : submesh.vbo.data )
				{
					vertex.position *= scale;
				}
			}
		}

		return result;
	}
}
