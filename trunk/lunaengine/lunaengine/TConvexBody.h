#pragma once
#include "TPolygon.h"
#include "TPlane.h"
#include <assert.h>
#include <vector>

namespace TLunaEngine
{
	// template class declaration
	template<typename T>
	class TAABBbox;

	template<typename T>
	class TConvexBody
	{
	protected:
		std::vector<TPolygon<T>> mPolygons;

	public:
		TConvexBody()
		{
			mPolygons.reserve(8);
		}
		~TConvexBody()
		{
			reset();
		}
		TConvexBody( const TConvexBody<T>& cpy )
		{
			for ( size_t i = 0; i < cpy.getPolygonCount(); ++i )
			{
				Polygon *p = new TPolygon<T>();
				*p = cpy.getPolygon( i );
				mPolygons.push_back( p );
			}
		}

		/** Build a new polygon representation from a frustum.
		*/
		void define(const TVector3<T>* frustumCorners)
		{
			// ordering of the points:
			// near (0-3), far (4-7); each (top-right, top-left, bottom-left, bottom-right)
			//	   5-----4
			//	  /|    /|
			//	 / |   / |
			//	1-----0  |
			//	|  6--|--7
			//	| /   | /
			//	|/    |/
			//	2-----3
		
			const TVector3<T> *pts = frustumCorners;

			/// reset ConvexBody
			reset();

			/// update vertices: near, far, left, right, bottom, top; fill in ccw
			TPolygon<T> *poly;

			// near
			poly = new TPolygon<T>();
			poly->insertVertex( pts[0] );
			poly->insertVertex( pts[1] );
			poly->insertVertex( pts[2] );
			poly->insertVertex( pts[3] );
			mPolygons.push_back( poly );

			// far
			poly = new TPolygon<T>();
			poly->insertVertex( pts[5] );
			poly->insertVertex( pts[4] );
			poly->insertVertex( pts[7] );
			poly->insertVertex( pts[6] );
			mPolygons.push_back( poly );

			// left
			poly = new TPolygon<T>();
			poly->insertVertex( pts[5] );
			poly->insertVertex( pts[6] );
			poly->insertVertex( pts[2] );
			poly->insertVertex( pts[1] );
			mPolygons.push_back( poly ); 

			// right
			poly = new TPolygon<T>();
			poly->insertVertex( pts[4] );
			poly->insertVertex( pts[0] );
			poly->insertVertex( pts[3] );
			poly->insertVertex( pts[7] );
			mPolygons.push_back( poly ); 

			// bottom
			poly = new TPolygon<T>();
			poly->insertVertex( pts[6] );
			poly->insertVertex( pts[7] );
			poly->insertVertex( pts[3] );
			poly->insertVertex( pts[2] );
			mPolygons.push_back( poly ); 

			// top
			poly = new TPolygon<T>();
			poly->insertVertex( pts[4] );
			poly->insertVertex( pts[5] );
			poly->insertVertex( pts[1] );
			poly->insertVertex( pts[0] );
			mPolygons.push_back( poly ); 
		}

		/** Build a new polygon representation from an AAB.
		*/
		void define(const TVector3<T>& min, const TVector3<T>& max)
		{
			// ordering of the AAB points:
			//		1-----2
			//	   /|    /|
			//	  / |   / |
			//   5-----4  |
			//   |  0--|--3
			//   | /   | /
			//   |/    |/
			//   6-----7
		
			//const Vector3& min = aab.getMinimum();
			//const Vector3& max = aab.getMaximum();

			TVector3<T> currentVertex = min;

			TPolygon<T> *poly;

			// reset body
			reset();

			// far
			poly = new TPolygon<T>();
			poly->insertVertex( currentVertex ); // 0 
			currentVertex.Y = max.Y;
			poly->insertVertex( currentVertex ); // 1
			currentVertex.X = max.X;
			poly->insertVertex( currentVertex ); // 2
			currentVertex.Y = min.Y;
			poly->insertVertex( currentVertex ); // 3
			insertPolygon( poly );

			// right
			poly = new TPolygon<T>();
			poly->insertVertex( currentVertex ); // 3
			currentVertex.Y = max.Y;
			poly->insertVertex( currentVertex ); // 2
			currentVertex.Z = max.Z;
			poly->insertVertex( currentVertex ); // 4
			currentVertex.Y = min.Y;
			poly->insertVertex( currentVertex ); // 7
			insertPolygon( poly ); 

			// near
			poly = new TPolygon<T>();
			poly->insertVertex( currentVertex ); // 7
			currentVertex.Y = max.Y;
			poly->insertVertex( currentVertex ); // 4
			currentVertex.X = min.X;
			poly->insertVertex( currentVertex ); // 5
			currentVertex.Y = min.Y;
			poly->insertVertex( currentVertex ); // 6
			insertPolygon( poly );

			// left
			poly = new TPolygon<T>();
			poly->insertVertex( currentVertex ); // 6
			currentVertex.Y = max.Y;
			poly->insertVertex( currentVertex ); // 5
			currentVertex.Z = min.Z;
			poly->insertVertex( currentVertex ); // 1
			currentVertex.Y = min.Y;
			poly->insertVertex( currentVertex ); // 0
			insertPolygon( poly ); 

			// bottom
			poly = new TPolygon<T>();
			poly->insertVertex( currentVertex ); // 0 
			currentVertex.X = max.X;
			poly->insertVertex( currentVertex ); // 3
			currentVertex.Z = max.Z;
			poly->insertVertex( currentVertex ); // 7 
			currentVertex.X = min.X;
			poly->insertVertex( currentVertex ); // 6
			insertPolygon( poly );

			// top
			poly = new TPolygon<T>();
			currentVertex = max;
			poly->insertVertex( currentVertex ); // 4
			currentVertex.Z = min.Z;
			poly->insertVertex( currentVertex ); // 2
			currentVertex.X = min.X;
			poly->insertVertex( currentVertex ); // 1
			currentVertex.Z = max.Z;
			poly->insertVertex( currentVertex ); // 5
			insertPolygon( poly );
		}

		/** Clips the body with a frustum. The resulting holes
			are filled with new polygons.
		*/
		void clip( const TPlane<T>* frustumPlanes )
		{
			// clip the body with each plane
			for ( unsigned short i = 0; i < 6; ++i )
			{
				// clip, but keep positive space this time since frustum planes are 
				// the opposite to other cases (facing inwards rather than outwards)
				clip(frustumPlanes[i], false);
			}
		}

		/** Clips the body with an AAB. The resulting holes
			are filled with new polygons.
		*/
		void clip( const TVector3<T>& min, const TVector3<T>& max )
		{
			//// only process finite boxes
			//if (!aab.isFinite())
			//	return;
			// ordering of the AAB points:
			//		1-----2
			//	   /|    /|
			//	  / |   / |
			//   5-----4  |
			//   |  0--|--3
			//   | /   | /
			//   |/    |/
			//   6-----7

			//const Vector3& min = aab.getMinimum();
			//const Vector3& max = aab.getMaximum();

			// clip object for each plane of the AAB
			TPlane<T> p;


			// front
			p.setPlane(max,TVector3<T>(0,0,1));
			clip(p);

			// back
			p.setPlane(min,TVector3<T>(0,0,-1));
			clip(p);
		
			// left
			p.setPlane(min,TVector3<T>(-1,0,0));
			clip(p);
		
			// right
			p.setPlane(max,TVector3<T>(1,0,0));
			clip(p);
		
			// bottom
			p.setPlane(min,TVector3<T>(0,-1,0));
			clip(p);
		
			// top
			p.setPlane(max,TVector3<T>(0,1,0));
			clip(p);
		}

		/** Clips the body with another body.
		*/
		void clip(const TConvexBody<T>& body)
		{
			if ( this == &body )
				return;

			// for each polygon; clip 'this' with each plane of 'body'
			// front vertex representation is ccw

			TPlane<T> pl;

			for ( size_t iPoly = 0; iPoly < body.getPolygonCount(); ++iPoly )
			{
				const TPolygon<T>& p = body.getPolygon( iPoly );

				assert( p.getVertexCount() >= 3 );

				// set up plane with first three vertices of the polygon (a polygon is always planar)
				pl.setPlane(p.getVertex( 0 ), p.getVertex( 1 ), p.getVertex( 2 ));

				clip(pl);
			}
		}

		/** Clips the object by the positive half space of a plane
		*/
		void clip(const TPlane<T>& pl, bool keepNegative = true)
		{
			if ( getPolygonCount() == 0 )
				return;

			// current will be used as the reference body
			TConvexBody<T> current;
			current.moveDataFromBody(*this);
		
			assert( this->getPolygonCount() == 0 );
			assert( current.getPolygonCount() != 0 );

			// holds all intersection edges for the different polygons
			std::map<TVector3<T>, TVector3<T>> intersectionEdges;

			// clip all polygons by the intersection plane
			// add only valid or intersected polygons to *this
			for ( size_t iPoly = 0; iPoly < current.getPolygonCount(); ++iPoly )
			{

				// fetch vertex count and ignore polygons with less than three vertices
				// the polygon is not valid and won't be added
				const size_t vertexCount = current.getVertexCount( iPoly );
				if ( vertexCount < 3 )
					continue;

				// current polygon
				const TPolygon<T>& p = current.getPolygon( iPoly );

				// the polygon to assemble
				TPolygon<T> *pNew = new TPolygon<T>();

				// the intersection polygon (indeed it's an edge or it's empty)
				TPolygon<T> *pIntersect = new TPolygon<T>();
			
				// check if polygons lie inside or outside (or on the plane)
				// for each vertex check where it is situated in regard to the plane
				// three possibilities appear:
				EIntersectionRelation3D clipSide = keepNegative ? EIntersectionRelation3D::ISREL3D_FRONT : EIntersectionRelation3D::ISREL3D_BACK;
				// - side is clipSide: vertex will be clipped
				// - side is !clipSide: vertex will be untouched
				// - side is NOSIDE:   vertex will be untouched
				EIntersectionRelation3D *side = new EIntersectionRelation3D[vertexCount];
				for ( size_t iVertex = 0; iVertex < vertexCount; ++iVertex )
				{
					side[ iVertex ] = pl.getSide( p.getVertex( iVertex ) );
				}

				// now we check the side combinations for the current and the next vertex
				// four different combinations exist:
				// - both points inside (or on the plane): keep the second (add it to the body)
				// - both points outside: discard both (don't add them to the body)
				// - first vertex is inside, second is outside: add the intersection point
				// - first vertex is outside, second is inside: add the intersection point, then the second
				for ( size_t iVertex = 0; iVertex < vertexCount; ++iVertex )
				{
					// determine the next vertex
					size_t iNextVertex = ( iVertex + 1 ) % vertexCount;

					const Vector3& vCurrent = p.getVertex( iVertex );
					const Vector3& vNext    = p.getVertex( iNextVertex );

					// case 1: both points inside (store next)
					if ( side[ iVertex ]     != clipSide &&		// NEGATIVE or NONE
						 side[ iNextVertex ] != clipSide )		// NEGATIVE or NONE
					{
						// keep the second
						pNew->insertVertex( vNext );
					}

					// case 3: inside -> outside (store intersection)
					else if ( side[ iVertex ]		!= clipSide &&
							  side[ iNextVertex ]	== clipSide )
					{
						// Do an intersection with the plane. We use a ray with a start point and a direction.
						// The ray is forced to hit the plane with any option available (eigher current or next
						// is the starting point)

						// intersect from the outside vertex towards the inside one
						TVector3<T> vDirection = vCurrent - vNext;
						vDirection.normalise();
						Ray ray( vNext, vDirection );
						std::pair< bool, Real > intersect = ray.intersects( pl );

						// store intersection
						if ( intersect.first )
						{
							// convert distance to vector
							TVector3<T> vIntersect = ray.getPoint( intersect.second );	

							// store intersection
							pNew->insertVertex( vIntersect );
							pIntersect->insertVertex( vIntersect );
						}
					}

					// case 4: outside -> inside (store intersection, store next)
					else if ( side[ iVertex ]		== clipSide &&
						side[ iNextVertex ]			!= clipSide )
					{
						// Do an intersection with the plane. We use a ray with a start point and a direction.
						// The ray is forced to hit the plane with any option available (eigher current or next
						// is the starting point)

						// intersect from the outside vertex towards the inside one
						TVector3<T> vDirection = vNext - vCurrent;
						vDirection.normalise();
						TVector3<T> vIntersect;
						bool bIntersect = pl.getIntersectionWithLine(vCurrent,vDirection,vIntersect);

						// store intersection
						if ( bIntersect )
						{
							// store intersection
							pNew->insertVertex( vIntersect );
							pIntersect->insertVertex( vIntersect );
						}

						pNew->insertVertex( vNext );

					}
					// else:
					// case 2: both outside (do nothing)
					
				}

				// insert the polygon only, if at least three vertices are present
				if ( pNew->getVertexCount() >= 3 )
				{
					// in case there are double vertices, remove them
					pNew->removeDuplicates();

					// in case there are still at least three vertices, insert the polygon
					if ( pNew->getVertexCount() >= 3 )
					{
						this->insertPolygon( pNew );
					}
					else
					{
						// delete pNew because it's empty or invalid
						delete pNew;
						pNew = 0;
					}
				}
				else
				{
					// delete pNew because it's empty or invalid
					delete pNew;
					pNew = 0;
				}

				// insert intersection polygon only, if there are two vertices present
				if ( pIntersect->getVertexCount() == 2 )
				{
					intersectionEdges.insert( std::pair< TVector3<T>, TVector3<T>>( pIntersect->getVertex( 0 ),
															  pIntersect->getVertex( 1 ) ) );
				}

				// delete intersection polygon
				// vertices were copied (if there were any)
				delete pIntersect;
				pIntersect = 0;

				// delete side info
				delete [] side;
				side = 0;
			}

			// if the polygon was partially clipped, close it
			// at least three edges are needed for a polygon
			if ( intersectionEdges.size() >= 3 )
			{
				TPolygon<T> *pClosing = new TPolygon<T>();

				// Analyze the intersection list and insert the intersection points in ccw order
				// Each point is twice in the list because of the fact that we have a convex body
				// with convex polygons. All we have to do is order the edges (an even-odd pair)
				// in a ccw order. The plane normal shows us the direction.
				std::map<TVector3<T>, TVector3<T>>::iterator it = intersectionEdges.begin();

				// check the cross product of the first two edges
				TVector3<T> vFirst  = it->first;
				TVector3<T> vSecond = it->second;

				// remove inserted edge
				intersectionEdges.erase( it );

				TVector3<T> vNext;

				// find mating edge
				if (findAndEraseEdgePair(vSecond, intersectionEdges, vNext))
				{
					// detect the orientation
					// the polygon must have the same normal direction as the plane and then n
					TVector3<T> vCross = ( vFirst - vSecond ).crossProduct( vNext - vSecond );
					bool frontside = ( pl.Normal ).directionEquals( vCross, (T)(DEGTORAD * 1) );

					// first inserted vertex
					TVector3<T> firstVertex;
					// currently inserted vertex
					TVector3<T> currentVertex;
					// direction equals -> front side (walk ccw)
					if ( frontside )
					{
						// start with next as first vertex, then second, then first and continue with first to walk ccw
						pClosing->insertVertex( vNext );
						pClosing->insertVertex( vSecond );
						pClosing->insertVertex( vFirst );
						firstVertex		= vNext;
						currentVertex	= vFirst;

					#ifdef _DEBUG_INTERSECTION_LIST
						std::cout << "Plane: n=" << pl.normal << ", d=" << pl.d << std::endl;
						std::cout << "First inserted vertex: " << *next << std::endl;
						std::cout << "Second inserted vertex: " << *vSecond << std::endl;
						std::cout << "Third inserted vertex: " << *vFirst << std::endl;
					#endif
					}
					// direction does not equal -> back side (walk cw)
					else
					{
						// start with first as first vertex, then second, then next and continue with next to walk ccw
						pClosing->insertVertex( vFirst );
						pClosing->insertVertex( vSecond );
						pClosing->insertVertex( vNext );
						firstVertex		= vFirst;
						currentVertex	= vNext;

						#ifdef _DEBUG_INTERSECTION_LIST
							std::cout << "Plane: n=" << pl.normal << ", d=" << pl.d << std::endl;
							std::cout << "First inserted vertex: " << *vFirst << std::endl;
							std::cout << "Second inserted vertex: " << *vSecond << std::endl;
							std::cout << "Third inserted vertex: " << *next << std::endl;
						#endif
					}

					// search mating edges that have a point in common
					// continue this operation as long as edges are present
					while ( !intersectionEdges.empty() )
					{

						if (findAndEraseEdgePair(currentVertex, intersectionEdges, vNext))
						{
							// insert only if it's not the last (which equals the first) vertex
							if ( !intersectionEdges.empty() )
							{
								currentVertex = vNext;
								pClosing->insertVertex( vNext );
							}
						}
						else
						{
							// degenerated...
							break;
						}

					} // while intersectionEdges not empty

					// insert polygon (may be degenerated!)
					this->insertPolygon( pClosing );

				}
				// mating intersection edge NOT found!
				else
				{
					delete pClosing;
				}

			} // if intersectionEdges contains more than three elements
		}

		/** Extends the existing body to incorporate the passed in point as a
			convex hull.
		@remarks
			You must already have constructed a basic body using a 'construct' 
			method.
		*/
		void extend(const TVector3<T>& pt)
		{
			// Erase all polygons facing towards the point. For all edges that
			// are not removed twice (once in AB and once BA direction) build a
			// convex polygon (triangle) with the point.
			std::map<TVector3<T>, TVector3<T>> edgeMap;

			for ( size_t i = 0; i < getPolygonCount(); ++i )
			{
				const TVector3<T>& normal = getNormal( i );
				// direction of the point in regard to the polygon
				// the polygon is planar so we can take an arbitrary vertex
				TVector3<T> ptDir  = pt - getVertex( i, 0 );
				ptDir.normalise();

				// remove polygon if dot product is greater or equals null.
				if ( normal.dotProduct( ptDir ) >= 0 )
				{
					// store edges (copy them because if the polygon is deleted
					// its vertices are also deleted)
					storeEdgesOfPolygon( i, &edgeMap );

					// remove polygon
					deletePolygon( i );

					// decrement iterator because of deleted polygon
					--i; 
				}
			}

			// point is already a part of the hull (point lies inside)
			if ( edgeMap.empty() )
				return;

			// remove the edges that are twice in the list (once from each side: AB,BA)

			std::map<TVector3<T>, TVector3<T>>::iterator it;
			// iterate from first to the element before the last one
			for (std::map<TVector3<T>, TVector3<T>>::iterator itStart = edgeMap.begin(); 
				itStart != edgeMap.end(); )
			{
				// compare with iterator + 1 to end
				// don't need to skip last entry in itStart since omitted in inner loop
				it = itStart;
				++it;

				bool erased = false;
				// iterate from itStart+1 to the element before the last one
				for ( ; it != edgeMap.end(); ++it )
				{	
					if (itStart->first.equals(it->second) &&
						 itStart->second.equals(it->first))
					{
						edgeMap.erase(it);
						// increment itStart before deletion (iterator invalidation)
						std::map<TVector3<T>, TVector3<T>>::iterator delistart = itStart++;
						edgeMap.erase(delistart);
						erased = true;

						break; // found and erased
					}
				}
				// increment itStart if we didn't do it when erasing
				if (!erased)
					++itStart;

			}

			// use the remaining edges to build triangles with the point
			// the vertices of the edges are in ccw order (edgePtA-edgePtB-point
			// to form a ccw polygon)
			while ( !edgeMap.empty() )
			{
				std::map<TVector3<T>, TVector3<T>>::iterator mapIt = edgeMap.begin();

				// build polygon it.first, it.second, point
				TPolygon<T> *p = new TPolygon<T>();

				p->insertVertex(mapIt->first);
				p->insertVertex(mapIt->second);

				p->insertVertex( pt );
				// attach polygon to body
				insertPolygon( p );

				// erase the vertices from the list
				// pointers are now held by the polygon
				edgeMap.erase( mapIt );
			}
		}

		/** Resets the object.
		*/
		void reset( void )
		{
			for (PolygonList::iterator it = mPolygons.begin(); 
				it != mPolygons.end(); ++it)
			{
				delete *it;
			}
			mPolygons.clear();
		}

		/** Returns the current number of polygons.
		*/
		size_t getPolygonCount( void ) const
		{
			return mPolygons.size();
		}

		/** Returns the number of vertices for a polygon
		*/
		size_t getVertexCount( size_t poly ) const
		{
			assert(poly < getPolygonCount());
		
			return mPolygons[ poly ]->getVertexCount();
		}

		/** Returns a polygon.
		*/
		const TPolygon<T>& getPolygon( size_t poly ) const
		{
			assert(poly < getPolygonCount());

			return *mPolygons[poly];
		}

		/** Returns a specific vertex of a polygon.
		*/
		const TVector3<T>& getVertex( size_t poly, size_t vertex ) const
		{
			assert( poly < getPolygonCount() );
		
			return mPolygons[poly]->getVertex(vertex);
		}

		/** Returns the normal of a specified polygon.
		*/
		const TVector3<T>& getNormal( size_t poly )
		{
			assert( poly < getPolygonCount() );
		
			return mPolygons[ poly ]->getNormal();
		}

		/** Returns an AABB representation.
		*/
		TAABBbox<T> getAABB( void ) const;

		/** Checks if the body has a closed hull.
		*/
		bool hasClosedHull( void ) const
		{
			// if this map is returned empty, the body is closed
			std::map<TVector3<T>, TVector3<T>> edgeMap = getSingleEdges();

			return edgeMap.empty();
		}

		/** Merges all neighboring polygons into one single polygon if they are
			lay in the same plane.
		*/
		void mergePolygons( void )
		{
			// Merge all polygons that lay in the same plane as one big polygon.
			// A convex body does not have two separate regions (separated by polygons
			// with different normals) where the same normal occurs, so we can simply
			// search all similar normals of a polygon. Two different options are 
			// possible when the normals fit:
			// - the two polygons are neighbors
			// - the two polygons aren't neighbors (but a third, fourth,.. polygon lays
			//   in between)

			// Signals if the body holds polygons which aren't neighbors but have the same
			// normal. That means another step has to be processed.
			bool bDirty = false;

			for ( size_t iPolyA = 0; iPolyA < getPolygonCount(); ++iPolyA )
			{

				for ( size_t iPolyB = iPolyA+1; iPolyB < getPolygonCount(); ++iPolyB )
				{
					const TVector3<T>& n1 = getNormal( iPolyA );
					const TVector3<T>& n2 = getNormal( iPolyB );

					// if the normals point into the same direction
					if ( n1.directionEquals( n2, (T)(DEGTORAD * 0.00001) )  )
					{
						// indicates if a neighbor has been found and joined
						bool bFound = false;

						// search the two fitting vertices (if there are any) for the common edge
						const size_t numVerticesA = getVertexCount( iPolyA );
						for ( size_t iVertexA = 0; iVertexA < numVerticesA; ++iVertexA )
						{
							const size_t numVerticesB = getVertexCount( iPolyB );
							for ( size_t iVertexB = 0; iVertexB < numVerticesB; ++iVertexB )
							{
								const TVector3<T>& aCurrent	= getVertex( iPolyA, iVertexA );
								const TVector3<T>& aNext		= getVertex( iPolyA, (iVertexA + 1) % getVertexCount( iPolyA ) );
								const TVector3<T>& bCurrent	= getVertex( iPolyB, iVertexB );
								const TVector3<T>& bNext		= getVertex( iPolyB, (iVertexB + 1) % getVertexCount( iPolyB ) );

								// if the edge is the same the current vertex of A has to be equal to the next of B and the other
								// way round
								if ( aCurrent.equals(bNext) &&
									 bCurrent.equals(aNext))
								{
									// polygons are neighbors, assemble new one
									TPolygon<T> *pNew = new TPolygon<T>();

									// insert all vertices of A up to the join (including the common vertex, ignoring
									// whether the first vertex of A may be a shared vertex)
									for ( size_t i = 0; i <= iVertexA; ++i )
									{
										pNew->insertVertex( getVertex( iPolyA, i%numVerticesA ) );
									}

									// insert all vertices of B _after_ the join to the end
									for ( size_t i = iVertexB + 2; i < numVerticesB; ++i )
									{
										pNew->insertVertex( getVertex( iPolyB, i ) );
									}

									// insert all vertices of B from the beginning up to the join (including the common vertex
									// and excluding the first vertex if the first is part of the shared edge)
									for ( size_t i = 0; i <= iVertexB; ++i )
									{
										pNew->insertVertex( getVertex( iPolyB, i%numVerticesB ) );
									}

									// insert all vertices of A _after_ the join to the end
									for ( size_t i = iVertexA + 2; i < numVerticesA; ++i )
									{
										pNew->insertVertex( getVertex( iPolyA, i ) );
									}

									// in case there are double vertices (in special cases), remove them
									for ( size_t i = 0; i < pNew->getVertexCount(); ++i )
									{
										const TVector3<T>& a = pNew->getVertex( i );
										const TVector3<T>& b = pNew->getVertex( (i + 1) % pNew->getVertexCount() );

										// if the two vertices are the same...
										if (a.equals(b))
										{
											// remove a
											pNew->deleteVertex( i );

											// decrement counter
											--i;
										}
									}

									// delete the two old ones
									assert( iPolyA != iPolyB );
								
									// polyB is always higher than polyA, so delete polyB first
									deletePolygon( iPolyB );
									deletePolygon( iPolyA );

									// continue with next (current is deleted, so don't jump to the next after the next)
									--iPolyA;
									--iPolyB;

									// insert new polygon
									insertPolygon( pNew );

									bFound = true;
									break;
								}
							}
						
							if ( bFound )
							{
								break;
							}
						}

						if ( bFound == false )
						{
							// there are two polygons available with the same normal direction, but they
							// could not be merged into one single because of no shared edge
							bDirty = true;
							break;
						}
					}
				}
			}

			// recursion to merge the previous non-neighbors
			if ( bDirty )
			{
				mergePolygons();
			}
		}

		/** Determines if the current object is equal to the compared one.
		*/
		bool operator == ( const TConvexBody<T>& rhs ) const
		{
			if ( getPolygonCount() != rhs.getPolygonCount() )
				return false;

			// Compare the polygons. They may not be in correct order.
			// A correct convex body does not have identical polygons in its body.
			bool *bChecked = new bool[getPolygonCount()];
			for ( size_t i=0; i<getPolygonCount(); ++i )
			{
				bChecked[ i ] = false;
			}

			for ( size_t i=0; i<getPolygonCount(); ++i )
			{
				bool bFound = false;

				for ( size_t j=0; j<getPolygonCount(); ++j )
				{
					const TPolygon<T>& pA = getPolygon( i );
					const TPolygon<T>& pB = rhs.getPolygon( j );

					if ( pA == pB )
					{
						bFound = true;
						bChecked[ i ] = true;
						break;
					}
				}

				if ( bFound == false )
				{
					delete [] bChecked;
					bChecked = 0;
					return false;
				}
			}

			for ( size_t i=0; i<getPolygonCount(); ++i )
			{
				if ( bChecked[ i ] != true )
				{
					delete [] bChecked;
					bChecked = 0;
					return false;
				}
			}

			delete [] bChecked;
			bChecked = 0;
			return true;
		}

		/** Determines if the current object is not equal to the compared one.
		*/
		bool operator != ( const TConvexBody<T>& rhs ) const
		{ return !( *this == rhs ); }


	protected:
		/** Inserts a polygon at a particular point in the body.
		@note
			After this method is called, the ConvexBody 'owns' this Polygon
			and will be responsible for deleting it.
		*/
		void insertPolygon(TPolygon<T>* pdata, size_t poly)
		{
			assert(poly <= getPolygonCount());
			assert( pdata != NULL );

			std::vector<TPolygon<T>>::iterator it = mPolygons.begin();
			std::advance(it, poly);

			mPolygons.insert( it, pdata );
		}
		/** Inserts a polygon at the end.
		@note
			After this method is called, the ConvexBody 'owns' this Polygon
			and will be responsible for deleting it.
		*/
		void insertPolygon(TPolygon<T>* pdata)
		{
			assert( pdata != NULL );

			mPolygons.push_back( pdata );
		}

		/** Inserts a vertex for a polygon at a particular point.
		@note
			No checks are done whether the assembled polygon is (still) planar, 
			the caller must ensure that this is the case.
		*/
		void insertVertex(size_t poly, const TVector3<T>& vdata, size_t vertex)
		{
			assert(poly < getPolygonCount() );
		
			mPolygons[poly]->insertVertex(vdata, vertex);
		}
		/** Inserts a vertex for a polygon at the end.
		@note
			No checks are done whether the assembled polygon is (still) planar, 
			the caller must ensure that this is the case.
		*/
		void insertVertex(size_t poly, const TVector3<T>& vdata)
		{
			assert(poly < getPolygonCount() );

			mPolygons[poly]->insertVertex(vdata);
		}
		/** Deletes a specific polygon.
		*/
		void deletePolygon(size_t poly)
		{
			assert(poly < getPolygonCount() );

			std::vector<TPolygon<T>>::iterator it = mPolygons.begin();
			std::advance(it, poly);
		
			delete *it;
			mPolygons.erase(it);
		}

		/** Removes a specific polygon from the body without deleting it.
		@note
			The retrieved polygon needs to be deleted later by the caller.
		*/
		TPolygon<T>* unlinkPolygon(size_t poly)
		{
			assert( poly < getPolygonCount() );

			std::vector<TPolygon<T>>::iterator it = mPolygons.begin();
			std::advance(it, poly);

			// safe address
			TPolygon<T> *pRet = *it;
		
			// delete entry
			mPolygons.erase(it);	

			// return polygon pointer

			return pRet;
		}

		/** Moves all polygons from the parameter body to this instance.
		@note Both the passed in object and this instance are modified
		*/
		void moveDataFromBody(TConvexBody<T>& body)
		{
			body.mPolygons.swap(this->mPolygons);
		}

		/** Deletes a specific vertex of a specific polygon.
		*/
		void deleteVertex(size_t poly, size_t vertex)
		{
			assert(poly < getPolygonCount() );

			mPolygons[poly]->deleteVertex(vertex);
		}

		/** Replace a polygon at a particular index.
		@note Again, the passed in polygon is owned by this object after this
			call returns, and this object is resonsible for deleting it.
		*/
		void setPolygon(TPolygon<T>* pdata, size_t poly )
		{
			assert(poly < getPolygonCount() );
			assert(pdata != NULL );

			if (pdata != mPolygons[poly])
			{
				// delete old polygon
				delete mPolygons[ poly ];

				// set new polygon
				mPolygons[poly] = pdata;
			}
		}

		/** Replace a specific vertex of a polygon.
		@note
			No checks are done whether the assembled polygon is (still) planar, 
			the caller must ensure that this is the case.
		*/
		void setVertex( size_t poly, const TVector3<T>& vdata, size_t vertex )
		{
			assert(poly < getPolygonCount());
		
			mPolygons[poly]->setVertex(vdata, vertex);
		}

		/** Returns the single edges in an EdgeMap (= edges where one side is a vertex and the
			other is empty space (a hole in the body)).
		*/
		std::map<TVector3<T>, TVector3<T>> getSingleEdges() const
		{
			std::map<TVector3<T>, TVector3<T>> edgeMap;

			// put all edges of all polygons into a list every edge has to be
			// walked in each direction once	
			for ( size_t i = 0; i < getPolygonCount(); ++i )
			{
				const TPolygon<T>& p = getPolygon( i );

				for ( size_t j = 0; j < p.getVertexCount(); ++j )
				{
					const TVector3<T>& a = p.getVertex( j );
					const TVector3<T>& b = p.getVertex( ( j + 1 ) % p.getVertexCount() );

					edgeMap.insert( std::pair< TVector3<T>, TVector3<T>>( a, b ) );
				}
			}

			// search corresponding parts
			std::map<TVector3<T>, TVector3<T>>::iterator it;
			std::map<TVector3<T>, TVector3<T>>::iterator itStart;
			std::map<TVector3<T>, TVector3<T>>::const_iterator itEnd;
			while( !edgeMap.empty() )
			{
				it = edgeMap.begin(); ++it;	// start one element after itStart
				itStart = edgeMap.begin();	// the element to be compared with the others
				itEnd = edgeMap.end();		// beyond the last element
			
				bool bFound = false;

				for ( ; it != itEnd; ++it )
				{
					if (itStart->first.equals(it->second) &&
						 itStart->second.equals(it->first))
					{
						// erase itStart and it
						edgeMap.erase( it );
						edgeMap.erase( itStart );

						bFound = true;

						break; // found
					}
				}

				if ( bFound == false )
				{
					break;	// not all edges could be matched
							// body is not closed
				}
			}

			return edgeMap;
		}

		/** Stores the edges of a specific polygon in a passed in structure.
		*/
		void storeEdgesOfPolygon(size_t poly, std::map<TVector3<T>, TVector3<T>> *edgeMap) const
		{
			assert(poly <= getPolygonCount() );
			assert( edgeMap != NULL );

			mPolygons[poly]->storeEdges(edgeMap);
		}
			
		/** Allocates space for an specified amount of polygons with
			each of them having a specified number of vertices.
			@note
				Old data (if available) will be erased.
		*/
		void allocateSpace(size_t numPolygons, size_t numVertices)
		{
			reset();

			// allocate numPolygons polygons with each numVertices vertices
			for ( size_t iPoly = 0; iPoly < numPolygons; ++iPoly )
			{
				TPolygon<T> *poly = new TPolygon<T>;

				for ( size_t iVertex = 0; iVertex < numVertices; ++iVertex )
				{
					poly->insertVertex( Vector3::ZERO );
				}

				mPolygons.push_back( poly );
			}
		}

		/** Searches for a pair (an edge) in the intersectionList with an entry
			that equals vec, and removes it from the passed in list.
		@param vec The vertex to search for in intersectionEdges
		@param intersectionEdges A list of edges, which is updated if a match is found
		@param vNext A reference to a vector which will be filled with the other
			vertex at the matching edge, if found.
		@return True if a match was found
		*/
		bool findAndEraseEdgePair(const TVector3<T>& vec, 
			std::map<TVector3<T>, TVector3<T>>& intersectionEdges, TVector3<T>& vNext ) const
		{
			for (std::map<TVector3<T>, TVector3<T>>::iterator it = intersectionEdges.begin(); 
				it != intersectionEdges.end(); ++it)
			{
				if (it->first.equals(vec))
				{
					vNext = it->second;

					// erase found edge
					intersectionEdges.erase( it );

					return true; // found!
				}
				else if (it->second.equals(vec))
				{
					vNext = it->first;

					// erase found edge
					intersectionEdges.erase( it );

					return true; // found!
				}
			}

			return false; // not found!
		}
	};
}