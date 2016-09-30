#ifndef _TLPOLYGON_H_
#define _TLPOLYGON_H_
#include "TLVector3.h"
#include <assert.h>
#include <vector>
#include <map>

namespace TLunaEngine
{
	template<typename T>
	class Polygon
	{
	public:
		//typedef multimap<Vector3<T>, Vector3<T>>::type		EdgeMap;
		//typedef std::pair< Vector3<T>, Vector3<T>>		Edge;

	protected:
		std::vector<Vector3<T>> mVertexList;
		mutable Vector3<T>	mNormal;
		mutable TBOOL	mIsNormalSet;
		/** Updates the normal.
		*/
		TVOID updateNormal(TVOID) const
		{
			assert( getVertexCount() >= 3);

			if (mIsNormalSet)
				return;

			// vertex order is ccw
			const Vector3<T>& a = getVertex( 0 );
			const Vector3<T>& b = getVertex( 1 );
			const Vector3<T>& c = getVertex( 2 );

			// used method: Newell
			mNormal.x = 0.5f * ( (a.y - b.y) * (a.z + b.z) +
								   (b.y - c.y) * (b.z + c.z) + 
								   (c.y - a.y) * (c.z + a.z));

			mNormal.y = 0.5f * ( (a.z - b.z) * (a.x + b.x) +
								   (b.z - c.z) * (b.x + c.x) + 
								   (c.z - a.z) * (c.x + a.x));

			mNormal.z = 0.5f * ( (a.x - b.x) * (a.y + b.y) +
								   (b.x - c.x) * (b.y + c.y) + 
								   (c.x - a.x) * (c.y + a.y));

			mNormal.normalise();

			mIsNormalSet = TTRUE;
		}
	public:
		Polygon()
		{
			mNormal = Vector3<T>(0,0,0);
			mIsNormalSet = TFALSE;
			mVertexList.reserve(6);
		}
		~Polygon()
		{
		}
		Polygon( const Polygon<T>& cpy )
		{
			mVertexList = cpy.mVertexList;
			mNormal = cpy.mNormal;
			mIsNormalSet = cpy.mIsNormalSet;
		}

		/** Inserts a vertex at a specific position.
		@note Vertices must be coplanar.
		*/
		TVOID insertVertex(const Vector3<T>& vdata, size_t vertexIndex)
		{
			// TODO: optional: check planarity
			assert(vertexIndex <= getVertexCount());

			std::vector<Vector3<T>>::iterator it = mVertexList.begin();

			std::advance(it, vertexIndex);
			mVertexList.insert(it, vdata);
		}
		/** Inserts a vertex at the end of the polygon.
		@note Vertices must be coplanar.
		*/
		TVOID insertVertex(const Vector3<T>& vdata)
		{
			mVertexList.push_back(vdata);
		}

		// merge vertices from another one
		TVOID mergeVertices(const Polygon<T>& rhs)
		{
			std::vector<Vector3<T>>::const_iterator it = rhs.mVertexList.begin();
			for (; it != rhs.mVertexList.end(); ++it)
			{
				insertVertex(*it);
			}
		}

		/** Returns a vertex.
		*/
		const Vector3<T>& getVertex(size_t vertex) const
		{
			assert(vertex < getVertexCount());

			return mVertexList[vertex];
		}

		/** Sets a specific vertex of a polygon.
		@note Vertices must be coplanar.
		*/
		TVOID setVertex(const Vector3<T>& vdata, size_t vertexIndex)
		{
			// TODO: optional: check planarity
			assert(vertexIndex < getVertexCount());

			// set new vertex
			mVertexList[ vertexIndex ] = vdata;
		}

		/** Removes duplicate vertices from a polygon.
		*/
		TVOID removeDuplicates(TVOID)
		{
			for ( size_t i = 0; i < getVertexCount(); ++i )
			{
				const Vector3<T>& a = getVertex( i );
				const Vector3<T>& b = getVertex( (i + 1)%getVertexCount() );

				if (a.equals(b))
				{
					deleteVertex(i);
					--i;
				}
			}
		}

		/** Vertex count.
		*/
		size_t getVertexCount(TVOID) const
		{
			return mVertexList.size();
		}

		/** Returns the polygon normal.
		*/
		const Vector3<T>& getNormal(TVOID) const
		{
			assert( getVertexCount() >= 3 );
			updateNormal();
			return mNormal;
		}

		/** Deletes a specific vertex.
		*/
		TVOID deleteVertex(size_t vertex)
		{
			assert( vertex < getVertexCount() );

			std::vector<Vector3<T>>::iterator it = mVertexList.begin();
			std::advance(it, vertex);

			mVertexList.erase( it );
		}

		/** Determines if a point is inside the polygon.
		@remarks
			A point is inside a polygon if it is both on the polygon's plane, 
			and within the polygon's bounds. Polygons are assumed to be convex
			and planar.
		*/
		TBOOL isPointInside(const Vector3<T>& point) const
		{
			// sum the angles 
			TF32 anglesum = 0;
			size_t n = getVertexCount();
			for (size_t i = 0; i < n; i++) 
			{
				const Vector3<T>& p1 = getVertex(i);
				const Vector3<T>& p2 = getVertex((i + 1) % n);

				Vector3<T> v1 = p1 - point;
				Vector3<T> v2 = p2 - point;

				TF32 len1 = v1.getLength();
				TF32 len2 = v2.getLength();

				if (TLunaEngine::equals(len1 * len2, 0.0f, 1e-4f))
				{
					// We are on a vertex so consider this inside
					return TTRUE; 
				}
				else
				{
					TF32 costheta = v1.dotProduct(v2) / (len1 * len2);
					anglesum += acos(costheta);
				}
			}

			// result should be 2*PI if point is inside poly
			return TLunaEngine::equals(anglesum, 2.0f * TLunaEngine::PI, 1e-4f);
		}

		/** Stores the edges of the polygon in ccw order.
			The vertices are copied so the user has to take the 
			deletion into account.
		*/
		TVOID storeEdges(std::map<Vector3<T>, Vector3<T>> *edgeMap) const
		{
			assert( edgeMap != TNULL );

			size_t vertexCount = getVertexCount();

			for ( size_t i = 0; i < vertexCount; ++i )
			{
				edgeMap->insert( std::pair<Vector3<T>, Vector3<T>>( getVertex( i ), getVertex( ( i + 1 ) % vertexCount ) ) );
			}
		}

		/** Resets the object.
		*/
		TVOID reset(TVOID)
		{
			// could use swap() to free memory here, but assume most may be reused so avoid realloc
			mVertexList.clear();
			mIsNormalSet = TFALSE;
		}

		/** Determines if the current object is equal to the compared one.
		*/
		TBOOL operator == (const Polygon<T>& rhs) const
		{
			if ( getVertexCount() != rhs.getVertexCount() )
				return TFALSE;

			// Compare vertices. They may differ in its starting position.
			// find start
			size_t start = 0;
			TBOOL foundStart = TFALSE;
			for (size_t i = 0; i < getVertexCount(); ++i )
			{	
				if (getVertex(0).equals(rhs.getVertex(i)))
				{
					start = i;
					foundStart = TTRUE;
					break;
				}
			}

			if (!foundStart)
				return TFALSE;

			for (size_t i = 0; i < getVertexCount(); ++i )
			{
				const Vector3<T>& vA = getVertex( i );
				const Vector3<T>& vB = rhs.getVertex( ( i + start) % getVertexCount() );

				if (!vA.equals(vB))
					return TFALSE;
			}

			return TTRUE;
		}

		/** Determines if the current object is not equal to the compared one.
		*/
		TBOOL operator != (const Polygon<T>& rhs) const
		{ return !( *this == rhs ); }
	};
}

#endif