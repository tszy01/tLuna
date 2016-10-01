#ifndef _TLCONVEXBODY_H_
#define _TLCONVEXBODY_H_
#include "TLPolygon.h"
#include "TLPlane.h"
#include <assert.h>
#include "TLList.h"
#include "TLMathUtils.h"
#include "TLMap.h"

namespace TLunaEngine
{
	// template class declaration
	template<typename T>
	class AABBox;

	template<typename T>
	class ConvexBody
	{
	protected:
		List<Polygon<T>*> mPolygons;

	public:
		ConvexBody()
		{
			//mPolygons.reserve(8);
		}
		~ConvexBody()
		{
			reset();
		}
		ConvexBody( const ConvexBody<T>& cpy )
		{
			for (TU32 i = 0; i < cpy.getPolygonCount(); ++i )
			{
				Polygon *p = new Polygon<T>();
				*p = cpy.getPolygon( i );
				mPolygons.push_back( p );
			}
		}

		/** Build a new polygon representation from a frustum.
		*/
		TVOID define(const Vector3<T>* frustumCorners)
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
		
			const Vector3<T> *pts = frustumCorners;

			/// reset ConvexBody
			reset();

			/// update vertices: near, far, left, right, bottom, top; fill in ccw
			Polygon<T> *poly;

			// near
			poly = new Polygon<T>();
			poly->insertVertex( pts[0] );
			poly->insertVertex( pts[1] );
			poly->insertVertex( pts[2] );
			poly->insertVertex( pts[3] );
			mPolygons.push_back( poly );

			// far
			poly = new Polygon<T>();
			poly->insertVertex( pts[5] );
			poly->insertVertex( pts[4] );
			poly->insertVertex( pts[7] );
			poly->insertVertex( pts[6] );
			mPolygons.push_back( poly );

			// left
			poly = new Polygon<T>();
			poly->insertVertex( pts[5] );
			poly->insertVertex( pts[6] );
			poly->insertVertex( pts[2] );
			poly->insertVertex( pts[1] );
			mPolygons.push_back( poly ); 

			// right
			poly = new Polygon<T>();
			poly->insertVertex( pts[4] );
			poly->insertVertex( pts[0] );
			poly->insertVertex( pts[3] );
			poly->insertVertex( pts[7] );
			mPolygons.push_back( poly ); 

			// bottom
			poly = new Polygon<T>();
			poly->insertVertex( pts[6] );
			poly->insertVertex( pts[7] );
			poly->insertVertex( pts[3] );
			poly->insertVertex( pts[2] );
			mPolygons.push_back( poly ); 

			// top
			poly = new Polygon<T>();
			poly->insertVertex( pts[4] );
			poly->insertVertex( pts[5] );
			poly->insertVertex( pts[1] );
			poly->insertVertex( pts[0] );
			mPolygons.push_back( poly ); 
		}

		/** Build a new polygon representation from an AAB.
		*/
		TVOID define(const Vector3<T>& min, const Vector3<T>& max)
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

			Vector3<T> currentVertex = min;

			Polygon<T> *poly;

			// reset body
			reset();

			// far
			poly = new Polygon<T>();
			poly->insertVertex( currentVertex ); // 0 
			currentVertex.Y = max.Y;
			poly->insertVertex( currentVertex ); // 1
			currentVertex.X = max.X;
			poly->insertVertex( currentVertex ); // 2
			currentVertex.Y = min.Y;
			poly->insertVertex( currentVertex ); // 3
			insertPolygon( poly );

			// right
			poly = new Polygon<T>();
			poly->insertVertex( currentVertex ); // 3
			currentVertex.Y = max.Y;
			poly->insertVertex( currentVertex ); // 2
			currentVertex.Z = max.Z;
			poly->insertVertex( currentVertex ); // 4
			currentVertex.Y = min.Y;
			poly->insertVertex( currentVertex ); // 7
			insertPolygon( poly ); 

			// near
			poly = new Polygon<T>();
			poly->insertVertex( currentVertex ); // 7
			currentVertex.Y = max.Y;
			poly->insertVertex( currentVertex ); // 4
			currentVertex.X = min.X;
			poly->insertVertex( currentVertex ); // 5
			currentVertex.Y = min.Y;
			poly->insertVertex( currentVertex ); // 6
			insertPolygon( poly );

			// left
			poly = new Polygon<T>();
			poly->insertVertex( currentVertex ); // 6
			currentVertex.Y = max.Y;
			poly->insertVertex( currentVertex ); // 5
			currentVertex.Z = min.Z;
			poly->insertVertex( currentVertex ); // 1
			currentVertex.Y = min.Y;
			poly->insertVertex( currentVertex ); // 0
			insertPolygon( poly ); 

			// bottom
			poly = new Polygon<T>();
			poly->insertVertex( currentVertex ); // 0 
			currentVertex.X = max.X;
			poly->insertVertex( currentVertex ); // 3
			currentVertex.Z = max.Z;
			poly->insertVertex( currentVertex ); // 7 
			currentVertex.X = min.X;
			poly->insertVertex( currentVertex ); // 6
			insertPolygon( poly );

			// top
			poly = new Polygon<T>();
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
		TVOID clip( const Plane<T>* frustumPlanes )
		{
			// clip the body with each plane
			for ( TU16 i = 0; i < 6; ++i )
			{
				// clip, but keep positive space this time since frustum planes are 
				// the opposite to other cases (facing inwards rather than outwards)
				clip(frustumPlanes[i], TFALSE);
			}
		}

		/** Clips the body with an AAB. The resulting holes
			are filled with new polygons.
		*/
		TVOID clip( const Vector3<T>& min, const Vector3<T>& max )
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
			Plane<T> p;


			// front
			p.setPlane(max,Vector3<T>(0,0,1));
			clip(p);

			// back
			p.setPlane(min,Vector3<T>(0,0,-1));
			clip(p);
		
			// left
			p.setPlane(min,Vector3<T>(-1,0,0));
			clip(p);
		
			// right
			p.setPlane(max,Vector3<T>(1,0,0));
			clip(p);
		
			// bottom
			p.setPlane(min,Vector3<T>(0,-1,0));
			clip(p);
		
			// top
			p.setPlane(max,Vector3<T>(0,1,0));
			clip(p);
		}

		/** Clips the body with another body.
		*/
		TVOID clip(const ConvexBody<T>& body)
		{
			if ( this == &body )
				return;

			// for each polygon; clip 'this' with each plane of 'body'
			// front vertex representation is ccw

			Plane<T> pl;

			for (TU32 iPoly = 0; iPoly < body.getPolygonCount(); ++iPoly )
			{
				const Polygon<T>& p = body.getPolygon( iPoly );

				assert( p.getVertexCount() >= 3 );

				// set up plane with first three vertices of the polygon (a polygon is always planar)
				pl.setPlane(p.getVertex( 0 ), p.getVertex( 1 ), p.getVertex( 2 ));

				clip(pl);
			}
		}

		/** Clips the object by the positive half space of a plane
		*/
		TVOID clip(const Plane<T>& pl, TBOOL keepNegative = TTRUE)
		{
			if ( getPolygonCount() == 0 )
				return;

			// current will be used as the reference body
			ConvexBody<T> current;
			current.moveDataFromBody(*this);
		
			assert( this->getPolygonCount() == 0 );
			assert( current.getPolygonCount() != 0 );

			// holds all intersection edges for the different polygons
			Map<Vector3<T>, Vector3<T>> intersectionEdges;

			// clip all polygons by the intersection plane
			// add only valid or intersected polygons to *this
			for (TU32 iPoly = 0; iPoly < current.getPolygonCount(); ++iPoly )
			{

				// fetch vertex count and ignore polygons with less than three vertices
				// the polygon is not valid and won't be added
				const TU32 vertexCount = current.getVertexCount( iPoly );
				if ( vertexCount < 3 )
					continue;

				// current polygon
				const Polygon<T>& p = current.getPolygon( iPoly );

				// the polygon to assemble
				Polygon<T> *pNew = new Polygon<T>();

				// the intersection polygon (indeed it's an edge or it's empty)
				Polygon<T> *pIntersect = new Polygon<T>();
			
				// check if polygons lie inside or outside (or on the plane)
				// for each vertex check where it is situated in regard to the plane
				// three possibilities appear:
				EIntersectionRelation3D clipSide = keepNegative ? EIntersectionRelation3D::ISREL3D_FRONT : EIntersectionRelation3D::ISREL3D_BACK;
				// - side is clipSide: vertex will be clipped
				// - side is !clipSide: vertex will be untouched
				// - side is NOSIDE:   vertex will be untouched
				EIntersectionRelation3D *side = new EIntersectionRelation3D[vertexCount];
				for (TU32 iVertex = 0; iVertex < vertexCount; ++iVertex )
				{
					side[ iVertex ] = pl.getSide( p.getVertex( iVertex ) );
				}

				// now we check the side combinations for the current and the next vertex
				// four different combinations exist:
				// - both points inside (or on the plane): keep the second (add it to the body)
				// - both points outside: discard both (don't add them to the body)
				// - first vertex is inside, second is outside: add the intersection point
				// - first vertex is outside, second is inside: add the intersection point, then the second
				for (TU32 iVertex = 0; iVertex < vertexCount; ++iVertex )
				{
					// determine the next vertex
					TU32 iNextVertex = ( iVertex + 1 ) % vertexCount;

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
						Vector3<T> vDirection = vCurrent - vNext;
						vDirection.normalise();
						Ray ray( vNext, vDirection );
						Vector3<T> outInterection;
						TF32 outTValue;
						TBOOL inter = intersects(pl, ray, outInterection, outTValue);

						// store intersection
						if ( inter )
						{
							// convert distance to vector
							//Vector3<T> vIntersect = ray.GetDest( outTValue );

							// store intersection
							pNew->insertVertex(outInterection);
							pIntersect->insertVertex(outInterection);
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
						Vector3<T> vDirection = vNext - vCurrent;
						vDirection.normalise();
						Vector3<T> vIntersect;
						TF32 intersectTValue;
						TBOOL bIntersect = pl.getIntersectionWithLine(vCurrent,vDirection,vIntersect, intersectTValue);

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
					intersectionEdges.push_back( pIntersect->getVertex( 0 ),pIntersect->getVertex( 1 ) );
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
				Polygon<T> *pClosing = new Polygon<T>();

				// Analyze the intersection list and insert the intersection points in ccw order
				// Each point is twice in the list because of the fact that we have a convex body
				// with convex polygons. All we have to do is order the edges (an even-odd pair)
				// in a ccw order. The plane normal shows us the direction.
				Map<Vector3<T>, Vector3<T>>::Iterator it = intersectionEdges.begin();

				// check the cross product of the first two edges
				Vector3<T> vFirst  = it->Key;
				Vector3<T> vSecond = it->Value;

				// remove inserted edge
				intersectionEdges.erase( it );

				Vector3<T> vNext;

				// find mating edge
				if (findAndEraseEdgePair(vSecond, intersectionEdges, vNext))
				{
					// detect the orientation
					// the polygon must have the same normal direction as the plane and then n
					Vector3<T> vCross = ( vFirst - vSecond ).crossProduct( vNext - vSecond );
					TBOOL frontside = ( pl.Normal ).directionEquals( vCross, (T)(DEGTORAD * 1) );

					// first inserted vertex
					Vector3<T> firstVertex;
					// currently inserted vertex
					Vector3<T> currentVertex;
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
		TVOID extend(const Vector3<T>& pt)
		{
			// Erase all polygons facing towards the point. For all edges that
			// are not removed twice (once in AB and once BA direction) build a
			// convex polygon (triangle) with the point.
			Map<Vector3<T>, Vector3<T>> edgeMap;

			for (TU32 i = 0; i < getPolygonCount(); ++i )
			{
				const Vector3<T>& normal = getNormal( i );
				// direction of the point in regard to the polygon
				// the polygon is planar so we can take an arbitrary vertex
				Vector3<T> ptDir  = pt - getVertex( i, 0 );
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

			Map<Vector3<T>, Vector3<T>>::Iterator it;
			// iterate from first to the element before the last one
			for (Map<Vector3<T>, Vector3<T>>::Iterator itStart = edgeMap.begin(); 
				itStart != edgeMap.end(); )
			{
				// compare with iterator + 1 to end
				// don't need to skip last entry in itStart since omitted in inner loop
				it = itStart;
				++it;

				TBOOL erased = TFALSE;
				// iterate from itStart+1 to the element before the last one
				for ( ; it != edgeMap.end(); ++it )
				{	
					if (itStart->Key.equals(it->Value) &&
						 itStart->Value.equals(it->Key))
					{
						edgeMap.erase(it);
						// increment itStart before deletion (iterator invalidation)
						Map<Vector3<T>, Vector3<T>>::Iterator delistart = itStart++;
						edgeMap.erase(delistart);
						erased = TTRUE;

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
				Map<Vector3<T>, Vector3<T>>::Iterator mapIt = edgeMap.begin();

				// build polygon it.first, it.second, point
				Polygon<T> *p = new Polygon<T>();

				p->insertVertex(mapIt->Key);
				p->insertVertex(mapIt->Value);

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
		TVOID reset( TVOID )
		{
			for (List<Polygon<T>*>::Iterator it = mPolygons.begin(); 
				it != mPolygons.end(); ++it)
			{
				delete *it;
			}
			mPolygons.clear();
		}

		/** Returns the current number of polygons.
		*/
		TU32 getPolygonCount( TVOID ) const
		{
			return (TU32)mPolygons.size();
		}

		/** Returns the number of vertices for a polygon
		*/
		TU32 getVertexCount(TU32 poly ) const
		{
			assert(poly < getPolygonCount());
		
			return (TU32)mPolygons[ poly ]->getVertexCount();
		}

		/** Returns a polygon.
		*/
		const Polygon<T>& getPolygon(TU32 poly ) const
		{
			assert(poly < getPolygonCount());

			return *mPolygons[poly];
		}

		/** Returns a specific vertex of a polygon.
		*/
		const Vector3<T>& getVertex(TU32 poly, TU32 vertex ) const
		{
			assert( poly < getPolygonCount() );
		
			return mPolygons[poly]->getVertex(vertex);
		}

		/** Returns the normal of a specified polygon.
		*/
		const Vector3<T>& getNormal(TU32 poly )
		{
			assert( poly < getPolygonCount() );
		
			return mPolygons[ poly ]->getNormal();
		}

		/** Returns an AABB representation.
		*/
		AABBox<T> getAABB( TVOID ) const;

		/** Checks if the body has a closed hull.
		*/
		TBOOL hasClosedHull( TVOID ) const
		{
			// if this map is returned empty, the body is closed
			Map<Vector3<T>, Vector3<T>> edgeMap = getSingleEdges();

			return edgeMap.empty();
		}

		/** Merges all neighboring polygons into one single polygon if they are
			lay in the same plane.
		*/
		TVOID mergePolygons( TVOID )
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
			TBOOL bDirty = TFALSE;

			for (TU32 iPolyA = 0; iPolyA < getPolygonCount(); ++iPolyA )
			{

				for (TU32 iPolyB = iPolyA+1; iPolyB < getPolygonCount(); ++iPolyB )
				{
					const Vector3<T>& n1 = getNormal( iPolyA );
					const Vector3<T>& n2 = getNormal( iPolyB );

					// if the normals point into the same direction
					if ( n1.directionEquals( n2, (T)(DEGTORAD * 0.00001) )  )
					{
						// indicates if a neighbor has been found and joined
						TBOOL bFound = TFALSE;

						// search the two fitting vertices (if there are any) for the common edge
						const TU32 numVerticesA = getVertexCount( iPolyA );
						for (TU32 iVertexA = 0; iVertexA < numVerticesA; ++iVertexA )
						{
							const TU32 numVerticesB = getVertexCount( iPolyB );
							for (TU32 iVertexB = 0; iVertexB < numVerticesB; ++iVertexB )
							{
								const Vector3<T>& aCurrent	= getVertex( iPolyA, iVertexA );
								const Vector3<T>& aNext		= getVertex( iPolyA, (iVertexA + 1) % getVertexCount( iPolyA ) );
								const Vector3<T>& bCurrent	= getVertex( iPolyB, iVertexB );
								const Vector3<T>& bNext		= getVertex( iPolyB, (iVertexB + 1) % getVertexCount( iPolyB ) );

								// if the edge is the same the current vertex of A has to be equal to the next of B and the other
								// way round
								if ( aCurrent.equals(bNext) &&
									 bCurrent.equals(aNext))
								{
									// polygons are neighbors, assemble new one
									Polygon<T> *pNew = new Polygon<T>();

									// insert all vertices of A up to the join (including the common vertex, ignoring
									// whether the first vertex of A may be a shared vertex)
									for (TU32 i = 0; i <= iVertexA; ++i )
									{
										pNew->insertVertex( getVertex( iPolyA, i%numVerticesA ) );
									}

									// insert all vertices of B _after_ the join to the end
									for (TU32 i = iVertexB + 2; i < numVerticesB; ++i )
									{
										pNew->insertVertex( getVertex( iPolyB, i ) );
									}

									// insert all vertices of B from the beginning up to the join (including the common vertex
									// and excluding the first vertex if the first is part of the shared edge)
									for (TU32 i = 0; i <= iVertexB; ++i )
									{
										pNew->insertVertex( getVertex( iPolyB, i%numVerticesB ) );
									}

									// insert all vertices of A _after_ the join to the end
									for (TU32 i = iVertexA + 2; i < numVerticesA; ++i )
									{
										pNew->insertVertex( getVertex( iPolyA, i ) );
									}

									// in case there are double vertices (in special cases), remove them
									for (TU32 i = 0; i < pNew->getVertexCount(); ++i )
									{
										const Vector3<T>& a = pNew->getVertex( i );
										const Vector3<T>& b = pNew->getVertex( (i + 1) % pNew->getVertexCount() );

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

									bFound = TTRUE;
									break;
								}
							}
						
							if ( bFound )
							{
								break;
							}
						}

						if ( bFound == TFALSE )
						{
							// there are two polygons available with the same normal direction, but they
							// could not be merged into one single because of no shared edge
							bDirty = TTRUE;
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
		TBOOL operator == ( const ConvexBody<T>& rhs ) const
		{
			if ( getPolygonCount() != rhs.getPolygonCount() )
				return TFALSE;

			// Compare the polygons. They may not be in correct order.
			// A correct convex body does not have identical polygons in its body.
			TBOOL *bChecked = new TBOOL[getPolygonCount()];
			for (TU32 i=0; i<getPolygonCount(); ++i )
			{
				bChecked[ i ] = TFALSE;
			}

			for (TU32 i=0; i<getPolygonCount(); ++i )
			{
				TBOOL bFound = TFALSE;

				for (TU32 j=0; j<getPolygonCount(); ++j )
				{
					const Polygon<T>& pA = getPolygon( i );
					const Polygon<T>& pB = rhs.getPolygon( j );

					if ( pA == pB )
					{
						bFound = TTRUE;
						bChecked[ i ] = TTRUE;
						break;
					}
				}

				if ( bFound == TFALSE )
				{
					delete [] bChecked;
					bChecked = 0;
					return TFALSE;
				}
			}

			for (TU32 i=0; i<getPolygonCount(); ++i )
			{
				if ( bChecked[ i ] != TTRUE )
				{
					delete [] bChecked;
					bChecked = 0;
					return TFALSE;
				}
			}

			delete [] bChecked;
			bChecked = 0;
			return TTRUE;
		}

		/** Determines if the current object is not equal to the compared one.
		*/
		TBOOL operator != ( const ConvexBody<T>& rhs ) const
		{ return !( *this == rhs ); }


	protected:
		/** Inserts a polygon at a particular point in the body.
		@note
			After this method is called, the ConvexBody 'owns' this Polygon
			and will be responsible for deleting it.
		*/
		TVOID insertPolygon(Polygon<T>* pdata, TU32 poly)
		{
			assert(poly <= getPolygonCount());
			assert( pdata != TNULL );

			List<Polygon<T>>::Iterator it = mPolygons.begin();
			it.advance(poly);
			//std::advance(it, poly);

			mPolygons.insert_after( it, pdata );
		}
		/** Inserts a polygon at the end.
		@note
			After this method is called, the ConvexBody 'owns' this Polygon
			and will be responsible for deleting it.
		*/
		TVOID insertPolygon(Polygon<T>* pdata)
		{
			assert( pdata != TNULL );

			mPolygons.push_back( pdata );
		}

		/** Inserts a vertex for a polygon at a particular point.
		@note
			No checks are done whether the assembled polygon is (still) planar, 
			the caller must ensure that this is the case.
		*/
		TVOID insertVertex(TU32 poly, const Vector3<T>& vdata, TU32 vertex)
		{
			assert(poly < getPolygonCount() );
		
			mPolygons[poly]->insertVertex(vdata, vertex);
		}
		/** Inserts a vertex for a polygon at the end.
		@note
			No checks are done whether the assembled polygon is (still) planar, 
			the caller must ensure that this is the case.
		*/
		TVOID insertVertex(TU32 poly, const Vector3<T>& vdata)
		{
			assert(poly < getPolygonCount() );

			mPolygons[poly]->insertVertex(vdata);
		}
		/** Deletes a specific polygon.
		*/
		TVOID deletePolygon(TU32 poly)
		{
			assert(poly < getPolygonCount() );

			List<Polygon<T>>::Iterator it = mPolygons.begin();
			it.advance(poly);
			//std::advance(it, poly);
		
			delete *it;
			mPolygons.erase(it);
		}

		/** Removes a specific polygon from the body without deleting it.
		@note
			The retrieved polygon needs to be deleted later by the caller.
		*/
		Polygon<T>* unlinkPolygon(TU32 poly)
		{
			assert( poly < getPolygonCount() );

			List<Polygon<T>>::Iterator it = mPolygons.begin();
			it.advance(poly);
			//std::advance(it, poly);

			// safe address
			Polygon<T> *pRet = *it;
		
			// delete entry
			mPolygons.erase(it);	

			// return polygon pointer

			return pRet;
		}

		/** Moves all polygons from the parameter body to this instance.
		@note Both the passed in object and this instance are modified
		*/
		TVOID moveDataFromBody(ConvexBody<T>& body)
		{
			List<Polygon<T>*> tmpPolygons = body.mPolygons;
			body.mPolygons.clone(this->mPolygons);
			this->mPolygons.clone(tmpPolygons);
		}

		/** Deletes a specific vertex of a specific polygon.
		*/
		TVOID deleteVertex(TU32 poly, TU32 vertex)
		{
			assert(poly < getPolygonCount() );

			mPolygons[poly]->deleteVertex(vertex);
		}

		/** Replace a polygon at a particular index.
		@note Again, the passed in polygon is owned by this object after this
			call returns, and this object is resonsible for deleting it.
		*/
		TVOID setPolygon(Polygon<T>* pdata, TU32 poly )
		{
			assert(poly < getPolygonCount() );
			assert(pdata != TNULL );

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
		TVOID setVertex(TU32 poly, const Vector3<T>& vdata, TU32 vertex )
		{
			assert(poly < getPolygonCount());
		
			mPolygons[poly]->setVertex(vdata, vertex);
		}

		/** Returns the single edges in an EdgeMap (= edges where one side is a vertex and the
			other is empty space (a hole in the body)).
		*/
		Map<Vector3<T>, Vector3<T>> getSingleEdges() const
		{
			Map<Vector3<T>, Vector3<T>> edgeMap;

			// put all edges of all polygons into a list every edge has to be
			// walked in each direction once	
			for (TU32 i = 0; i < getPolygonCount(); ++i )
			{
				const Polygon<T>& p = getPolygon( i );

				for (TU32 j = 0; j < p.getVertexCount(); ++j )
				{
					const Vector3<T>& a = p.getVertex( j );
					const Vector3<T>& b = p.getVertex( ( j + 1 ) % p.getVertexCount() );

					edgeMap.insert( a, b );
				}
			}

			// search corresponding parts
			Map<Vector3<T>, Vector3<T>>::Iterator it;
			Map<Vector3<T>, Vector3<T>>::Iterator itStart;
			Map<Vector3<T>, Vector3<T>>::Iterator itEnd;
			while( !edgeMap.empty() )
			{
				it = edgeMap.begin(); ++it;	// start one element after itStart
				itStart = edgeMap.begin();	// the element to be compared with the others
				itEnd = edgeMap.end();		// beyond the last element
			
				TBOOL bFound = TFALSE;

				for ( ; it != itEnd; ++it )
				{
					if (itStart->Key.equals(it->Value) &&
						 itStart->Value.equals(it->Key))
					{
						// erase itStart and it
						edgeMap.erase( it );
						edgeMap.erase( itStart );

						bFound = TTRUE;

						break; // found
					}
				}

				if ( bFound == TFALSE )
				{
					break;	// not all edges could be matched
							// body is not closed
				}
			}

			return edgeMap;
		}

		/** Stores the edges of a specific polygon in a passed in structure.
		*/
		TVOID storeEdgesOfPolygon(TU32 poly, Map<Vector3<T>, Vector3<T>> *edgeMap) const
		{
			assert(poly <= getPolygonCount() );
			assert( edgeMap != TNULL );

			mPolygons[poly]->storeEdges(edgeMap);
		}
			
		/** Allocates space for an specified amount of polygons with
			each of them having a specified number of vertices.
			@note
				Old data (if available) will be erased.
		*/
		TVOID allocateSpace(TU32 numPolygons, TU32 numVertices)
		{
			reset();

			// allocate numPolygons polygons with each numVertices vertices
			for (TU32 iPoly = 0; iPoly < numPolygons; ++iPoly )
			{
				Polygon<T> *poly = new Polygon<T>;

				for (TU32 iVertex = 0; iVertex < numVertices; ++iVertex )
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
		TBOOL findAndEraseEdgePair(const Vector3<T>& vec, 
			Map<Vector3<T>, Vector3<T>>& intersectionEdges, Vector3<T>& vNext ) const
		{
			for (Map<Vector3<T>, Vector3<T>>::Iterator it = intersectionEdges.begin(); 
				it != intersectionEdges.end(); ++it)
			{
				if (it->Key.equals(vec))
				{
					vNext = it->Value;

					// erase found edge
					intersectionEdges.erase( it );

					return TTRUE; // found!
				}
				else if (it->Value.equals(vec))
				{
					vNext = it->Key;

					// erase found edge
					intersectionEdges.erase( it );

					return TTRUE; // found!
				}
			}

			return TFALSE; // not found!
		}
	};
}

#endif