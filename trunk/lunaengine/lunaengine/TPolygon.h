#pragma once
#include "TVector3.h"
#include <assert.h>
#include <vector>
#include <map>

namespace TLunaEngine
{
	template<typename T>
	class TPolygon
	{
	public:
		//typedef multimap<TVector3<T>, TVector3<T>>::type		EdgeMap;
		//typedef std::pair< TVector3<T>, TVector3<T>>		Edge;

	protected:
		std::vector<TVector3<T>> mVertexList;
		mutable TVector3<T>	mNormal;
		mutable bool	mIsNormalSet;
		/** Updates the normal.
		*/
		void updateNormal(void) const
		{
			assert( getVertexCount() >= 3);

			if (mIsNormalSet)
				return;

			// vertex order is ccw
			const TVector3<T>& a = getVertex( 0 );
			const TVector3<T>& b = getVertex( 1 );
			const TVector3<T>& c = getVertex( 2 );

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

			mIsNormalSet = true;
		}
	public:
		TPolygon()
		{
			mNormal = TVector3<T>(0,0,0);
			mIsNormalSet = false;
			mVertexList.reserve(6);
		}
		~TPolygon()
		{
		}
		TPolygon( const TPolygon<T>& cpy )
		{
			mVertexList = cpy.mVertexList;
			mNormal = cpy.mNormal;
			mIsNormalSet = cpy.mIsNormalSet;
		}

		/** Inserts a vertex at a specific position.
		@note Vertices must be coplanar.
		*/
		void insertVertex(const TVector3<T>& vdata, size_t vertexIndex)
		{
			// TODO: optional: check planarity
			assert(vertexIndex <= getVertexCount());

			std::vector<TVector3<T>>::iterator it = mVertexList.begin();

			std::advance(it, vertexIndex);
			mVertexList.insert(it, vdata);
		}
		/** Inserts a vertex at the end of the polygon.
		@note Vertices must be coplanar.
		*/
		void insertVertex(const TVector3<T>& vdata)
		{
			mVertexList.push_back(vdata);
		}

		/** Returns a vertex.
		*/
		const TVector3<T>& getVertex(size_t vertex) const
		{
			assert(vertex < getVertexCount());

			return mVertexList[vertex];
		}

		/** Sets a specific vertex of a polygon.
		@note Vertices must be coplanar.
		*/
		void setVertex(const TVector3<T>& vdata, size_t vertexIndex)
		{
			// TODO: optional: check planarity
			assert(vertexIndex < getVertexCount());

			// set new vertex
			mVertexList[ vertexIndex ] = vdata;
		}

		/** Removes duplicate vertices from a polygon.
		*/
		void removeDuplicates(void)
		{
			for ( size_t i = 0; i < getVertexCount(); ++i )
			{
				const TVector3<T>& a = getVertex( i );
				const TVector3<T>& b = getVertex( (i + 1)%getVertexCount() );

				if (a.equals(b))
				{
					deleteVertex(i);
					--i;
				}
			}
		}

		/** Vertex count.
		*/
		size_t getVertexCount(void) const
		{
			return mVertexList.size();
		}

		/** Returns the polygon normal.
		*/
		const TVector3<T>& getNormal(void) const
		{
			assert( getVertexCount() >= 3 );
			updateNormal();
			return mNormal;
		}

		/** Deletes a specific vertex.
		*/
		void deleteVertex(size_t vertex)
		{
			assert( vertex < getVertexCount() );

			std::vector<TVector3<T>>::iterator it = mVertexList.begin();
			std::advance(it, vertex);

			mVertexList.erase( it );
		}

		/** Determines if a point is inside the polygon.
		@remarks
			A point is inside a polygon if it is both on the polygon's plane, 
			and within the polygon's bounds. Polygons are assumed to be convex
			and planar.
		*/
		bool isPointInside(const TVector3<T>& point) const
		{
			// sum the angles 
			TF32 anglesum = 0;
			size_t n = getVertexCount();
			for (size_t i = 0; i < n; i++) 
			{
				const TVector3<T>& p1 = getVertex(i);
				const TVector3<T>& p2 = getVertex((i + 1) % n);

				TVector3<T> v1 = p1 - point;
				TVector3<T> v2 = p2 - point;

				TF32 len1 = v1.getLength();
				TF32 len2 = v2.getLength();

				if (TLunaEngine::equals(len1 * len2, 0.0f, 1e-4f))
				{
					// We are on a vertex so consider this inside
					return true; 
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
		void storeEdges(std::map<TVector3<T>, TVector3<T>> *edgeMap) const
		{
			assert( edgeMap != NULL );

			size_t vertexCount = getVertexCount();

			for ( size_t i = 0; i < vertexCount; ++i )
			{
				edgeMap->insert( std::pair<TVector3<T>, TVector3<T>>( getVertex( i ), getVertex( ( i + 1 ) % vertexCount ) ) );
			}
		}

		/** Resets the object.
		*/
		void reset(void)
		{
			// could use swap() to free memory here, but assume most may be reused so avoid realloc
			mVertexList.clear();
			mIsNormalSet = false;
		}

		/** Determines if the current object is equal to the compared one.
		*/
		bool operator == (const TPolygon<T>& rhs) const
		{
			if ( getVertexCount() != rhs.getVertexCount() )
				return false;

			// Compare vertices. They may differ in its starting position.
			// find start
			size_t start = 0;
			bool foundStart = false;
			for (size_t i = 0; i < getVertexCount(); ++i )
			{	
				if (getVertex(0).equals(rhs.getVertex(i)))
				{
					start = i;
					foundStart = true;
					break;
				}
			}

			if (!foundStart)
				return false;

			for (size_t i = 0; i < getVertexCount(); ++i )
			{
				const TVector3<T>& vA = getVertex( i );
				const TVector3<T>& vB = rhs.getVertex( ( i + start) % getVertexCount() );

				if (!vA.equals(vB))
					return false;
			}

			return true;
		}

		/** Determines if the current object is not equal to the compared one.
		*/
		bool operator != (const TPolygon<T>& rhs) const
		{ return !( *this == rhs ); }
	};
}