#include "stdafx.h"
#include "TConvexBody.h"
#include "TAABBbox.h"

namespace TLunaEngine
{
	template<typename T>
	TAABBbox<T> TConvexBody<T>::getAABB( void ) const
	{
		TAABBbox<T> aab;

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