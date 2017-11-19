#include "ObjLoader.h"

#include <stdlib.h>
#include <fstream>
#include <sstream>

#include <RenderLib/ElementsList.h>
#include <RenderLib/Material.h>
#include <RenderLib/Mesh.h>
#include <RenderLib/Object.h>
#include <RenderLib/StringUtils.h>
#include <RenderLib/Texture.h>

namespace utils
{
	namespace
	{
		inline render::TexturePtr loadTexture( std::string const & name
			, render::ByteArray const & fileContent
			, render::TextureList & textures )
		{
			auto ret = textures.findElement( name );

			if ( !ret )
			{
				ret = std::make_shared< render::Texture >();
				textures.addElement( name, ret );
				render::loadTexture( fileContent, *ret );
			}

			return ret;
		}
	}

	void loadMtlFile( std::string const & fileContent
		, render::MaterialList & materialsList
		, render::TextureList & texturesList )
	{
		std::istringstream file( fileContent );
		std::string line;
		render::MaterialPtr select;

		while ( std::getline( file, line ) )
		{
			render::trim( line );
			std::stringstream stream( line );
			std::string ident;
			stream >> ident;

			if ( ident == "newmtl" )
			{
				std::string name;
				stream >> name;
				select = std::make_shared< render::Material >();
				materialsList.addElement( name, select );
			}
			else if ( select )
			{
				if ( ident == "Ka" )
				{
					gl::RgbColour value;
					stream >> value.x >> value.y >> value.z;
					select->ambient( value );
				}
				else if ( ident == "Kd" )
				{
					gl::RgbColour value;
					stream >> value.x >> value.y >> value.z;
					select->diffuse( value );
				}
				else if ( ident == "Ks" )
				{
					gl::RgbColour value;
					stream >> value.x >> value.y >> value.z;
					select->specular( value );
				}
				else if ( ident == "Ns" )
				{
					float value;
					stream >> value;
					select->exponent( value );
				}
				else if ( ident == "d" )
				{
					float value;
					stream >> value;
					select->opacity( value );
				}
				else if ( ident == "map_Kd" )
				{
					std::string path;
					stream >> path;
					auto content = utils::getFileBinaryContent( path );
					select->diffuseMap( loadTexture( path
						, content
						, texturesList ) );
				}
			}
		}
	}

	bool doPreloadFile( std::string const & fileContent
		, render::MaterialList & materialsList
		, render::TextureList & texturesList
		, uint16_t & nv
		, uint16_t & nvt
		, uint16_t & nvn
		, uint16_t & nf
		, uint16_t & ntf
		, render::UInt16Array & faces )
	{
		std::istringstream file( fileContent );
		std::string line;
		std::string mtlfile;

		while ( std::getline( file, line ) )
		{
			render::trimLeft( line );
			std::stringstream stream( line );
			std::string ident;
			stream >> ident;

			if ( ident == "v" )
			{
				if ( ntf )
				{
					faces.push_back( ntf );
					ntf = 0u;
				}

				++nv;
			}
			else if ( ident == "vt" )
			{
				++nvt;
			}
			else if ( ident == "vn" )
			{
				++nvn;
			}
			else if ( ident == "f" )
			{
				++nf;
				++ntf;
			}
			else if ( ident == "g" || ident == "usemtl" )
			{
				if ( ntf )
				{
					faces.push_back( ntf );
					ntf = 0u;
				}
			}
			else if ( ident == "mtllib" )
			{
				mtlfile = line.substr( line.find_first_of( " " ) + 1 );
				render::trim( mtlfile );
			}
		}

		if ( ntf )
		{
			faces.push_back( ntf );
		}

		std::clog << "    Vertex count: " << nv << std::endl;
		std::clog << "    TexCoord count: " << nvt << std::endl;
		std::clog << "    Normal count: " << nvn << std::endl;
		std::clog << "    Group count: " << int( faces.size() ) << std::endl;

		if ( !mtlfile.empty() )
		{
			loadMtlFile( utils::getFileTextContent( mtlfile )
				, materialsList
				, texturesList );
		}

		return true;
	}

	void doLoadNewMesh( std::string const & name
		, std::string const & fileContent
		, render::MaterialList & materialsList
		, render::TextureList & texturesList
		, render::MeshList & meshesList
		, render::MeshPtr & mesh
		, render::MaterialArray & materials )
	{
		uint16_t nv = 0u;
		uint16_t nvt = 0u;
		uint16_t nvn = 0u;
		uint16_t nf = 0u;
		uint16_t ntf = 0u;
		render::UInt16Array faces;

		if ( !doPreloadFile( fileContent
			, materialsList
			, texturesList
			, nv
			, nvt
			, nvn
			, nf
			, ntf
			, faces ) )
		{
			std::cerr << "Failed to load the OBJ file" << std::endl;
			return;
		}

		std::istringstream file( fileContent );
		std::string line;
		render::Vec3Array allvtx( nv );
		render::Vec2Array alltex( nvt );
		render::Vec3Array allnml( nvn );
		auto vtxit = allvtx.begin();
		auto texit = alltex.begin();
		auto nmlit = allnml.begin();

		while ( std::getline( file, line ) )
		{
			render::trim( line );
			std::stringstream stream( line );
			std::string ident;
			stream >> ident;

			if ( ident == "v" )
			{
				stream >> vtxit->x >> vtxit->y >> vtxit->z;
				++vtxit;
			}
			else if ( ident == "vt" )
			{
				stream >> texit->x >> texit->y;
				++texit;
			}
			else if ( ident == "vn" )
			{
				stream >> nmlit->x >> nmlit->y >> nmlit->z;
				++nmlit;
			}
		}

		file.clear();
		file.seekg( 0, std::ios::beg );

		render::UInt16Array index( nf * 3 );
		render::Vec3Array vertex( nf * 3 );
		render::Vec3Array normal( nf * 3 );
		render::Vec2Array texcoord( nf * 3 );
		render::UInt16Array::iterator idxit = index.begin();
		vtxit = vertex.begin();
		texit = texcoord.begin();
		nmlit = normal.begin();
		render::UInt16Array::iterator facesit = faces.end();
		std::string mtlname;
		uint16_t normalsCount = 0u;

		materials.reserve( faces.size() );
		mesh = std::make_shared< render::Mesh >();
		meshesList.addElement( name, mesh );

		while ( std::getline( file, line ) )
		{
			render::trim( line );
			std::stringstream stream( line );
			std::string ident;
			stream >> ident;

			if ( ident == "g" )
			{
				if ( facesit == faces.end() )
				{
					facesit = faces.begin();
				}
				else
				{
					mesh->addSubmesh( render::UInt16Array( index.begin()
						, idxit ) );
					assert( materialsList.findElement( mtlname ) );
					materials.push_back( materialsList.findElement( mtlname ) );
					idxit = index.begin();
					++facesit;
				}

				std::clog << "    Group faces count: " << *facesit << std::endl;
			}
			else if ( ident == "usemtl" )
			{
				if ( facesit == faces.end() )
				{
					facesit = faces.begin();
				}
				else if ( index.begin() != idxit )
				{
					mesh->addSubmesh( render::UInt16Array( index.begin()
						, idxit ) );
					assert( materialsList.findElement( mtlname ) );
					materials.push_back( materialsList.findElement( mtlname ) );
					idxit = index.begin();
					++facesit;
				}

				stream >> mtlname;
				render::trim( mtlname );
			}
			else if ( ident == "f" )
			{
				for ( uint32_t i = 0u; i < 3u; i++ )
				{
					std::string face;
					stream >> face;

					size_t index1 = face.find( '/' );
					std::string component = face.substr( 0, index1 );
					uint16_t iv = atoi( component.c_str() ) - 1;
					*vtxit++ = allvtx[iv];

					++index1;
					size_t index2 = face.find( '/', index1 );

					if ( index2 > index1 )
					{
						component = face.substr( index1, index2 - index1 );
						uint32_t ivt = atoi( component.c_str() ) - 1;
						texit->x = alltex[ivt].x;
						texit->y = 1.0f - alltex[ivt].y;
						++texit;
					}

					++index2;
					component = face.substr( index2 );
					uint32_t ivn = atoi( component.c_str() ) - 1;
					*nmlit++ = allnml[ivn];

					*idxit++ = normalsCount;
					normalsCount++;
				}
			}
		}

		if ( idxit != index.begin()
			&& facesit != faces.end() )
		{
			mesh->addSubmesh( render::UInt16Array( index.begin(), idxit ) );
			assert( materialsList.findElement( mtlname ) );
			materials.push_back( materialsList.findElement( mtlname ) );
		}

		mesh->data( vertex
			, normal
			, alltex.empty()
				? render::Vec2Array{}
				: texcoord );
	}

	void doLoadMeshMaterials( std::string const & fileContent
		, render::MaterialList & materialsList
		, render::TextureList & texturesList
		, render::MaterialArray & materials )
	{
		uint16_t nv = 0u;
		uint16_t nvt = 0u;
		uint16_t nvn = 0u;
		uint16_t nf = 0u;
		uint16_t ntf = 0u;
		render::UInt16Array faces;

		if ( !doPreloadFile( fileContent
			, materialsList
			, texturesList
			, nv
			, nvt
			, nvn
			, nf
			, ntf
			, faces ) )
		{
			std::cerr << "Failed to load the OBJ file" << std::endl;
			return;
		}

		std::istringstream file( fileContent );
		std::string line;
		auto facesit = faces.end();
		std::string mtlname;
		uint16_t index = 0u;

		while ( std::getline( file, line ) )
		{
			render::trim( line );
			std::stringstream stream( line );
			std::string ident;
			stream >> ident;

			if ( ident == "g" )
			{
				if ( facesit == faces.end() )
				{
					facesit = faces.begin();
				}
				else
				{
					assert( materialsList.findElement( mtlname ) );
					materials.push_back( materialsList.findElement( mtlname ) );
					++facesit;
				}

				std::clog << "    Group faces count: " << *facesit << std::endl;
			}
			else if ( ident == "usemtl" )
			{
				if ( facesit == faces.end() )
				{
					facesit = faces.begin();
				}
				else if ( index != 0u )
				{
					assert( materialsList.findElement( mtlname ) );
					materials.push_back( materialsList.findElement( mtlname ) );
					++facesit;
					index = 0u;
				}

				stream >> mtlname;
				render::trim( mtlname );
			}
			else if ( ident == "f" )
			{
				++index;
			}
		}

		if ( index != 0u
			&& facesit != faces.end() )
		{
			assert( materialsList.findElement( mtlname ) );
			materials.push_back( materialsList.findElement( mtlname ) );
		}
	}

	render::ObjectPtr loadObjFile( std::string const & name
		, std::string const & fileContent
		, render::MaterialList & materialsList
		, render::TextureList & texturesList
		, render::MeshList & meshesList )
	{
		std::string objectName = name;
		render::MeshPtr mesh = meshesList.findElement( objectName );
		render::MaterialArray materials;

		if ( !mesh )
		{
			doLoadNewMesh( name
				, fileContent
				, materialsList
				, texturesList
				, meshesList
				, mesh
				, materials );
		}
		else
		{
			doLoadMeshMaterials( fileContent
				, materialsList
				, texturesList
				, materials );
		}

		if ( mesh && !materials.empty() )
		{
			return std::make_shared< render::Object >( name, mesh, materials );
		}

		return nullptr;

	}
}
