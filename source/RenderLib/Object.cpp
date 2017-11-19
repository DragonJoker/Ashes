#include "Object.h"

namespace render
{
	Object::Object( std::string const & name
		, MeshPtr mesh
		, MaterialArray const & materials )noexcept
		: Movable{ name }
		, m_mesh{ mesh }
		, m_materials{ materials }
	{
		assert( m_materials.size() == m_mesh->size() );
	}
}
