#include "TLConvexBody.h"
#include "TLAABBox.h"

namespace TLunaEngine
{
	template<typename T>
	AABBox<T> ConvexBody<T>::getAABB( TVOID ) const
	{
		AABBox<T> aab;

		for ( size_t i = 0; i < getPolygonCount(); ++i )
		{
			for ( size_t j = 0; j < getVertexCount( i ); ++j )
			{
				aab.addInternalPoint( getVertex( i, j ) );
			}
		}

		return aab;
	}
}