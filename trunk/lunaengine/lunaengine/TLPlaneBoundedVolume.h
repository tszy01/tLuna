#ifndef _TLPLANEBOUNDEDVOLUME_H_
#define _TLPLANEBOUNDEDVOLUME_H_
#include "TLPlane.h"
#include <assert.h>
#include "TLList.h"
#include "TLMap.h"

namespace TLunaEngine
{
	template<typename T>
	class PlaneBoundedVolume
	{
	public:
        /// Publicly accessible plane list, you can modify this direct
		List<Plane<T>> planes;
        EIntersectionRelation3D outside;

		PlaneBoundedVolume() :outside(ISREL3D_BACK) {}
        /** Constructor, determines which side is deemed to be 'outside' */
        PlaneBoundedVolume(EIntersectionRelation3D theOutside) 
            : outside(theOutside) {}

        /** Intersection test with AABB
        @remarks May return TFALSE positives but will never miss an intersection.
        */
        inline TBOOL intersects(const Vector3<T>& center, const Vector3<T>& halfSize) const
        {
			List<Plane<T>>::Iterator itr = planes.begin();
			for (;itr != planes.end(); ++itr)
            {
                const Plane<T>& plane = *itr;

                EIntersectionRelation3D side = plane.classifyPointRelation(center, halfSize);
                if (side == outside)
                {
                    // Found a splitting plane therefore return not intersecting
                    return TFALSE;
                }
            }

            // couldn't find a splitting plane, assume intersecting
            return TTRUE;

        }
        /** Intersection test with Sphere
        @remarks May return TFALSE positives but will never miss an intersection.
        */
        inline TBOOL intersects(const Vector3<T>& center, T radius) const
        {
			List<Plane<T>>::Iterator itr = planes.begin();
			for (;itr != planes.end(); ++itr)
            {
                const Plane<T>& plane = *itr;

                // Test which side of the plane the sphere is
                T d = plane.getDistanceTo(center);
                // Negate d if planes point inwards
                if (outside == ISREL3D_BACK) d = -d;

                if ( (d - radius) > 0)
                    return TFALSE;
            }

            return TTRUE;

        }

        /** Intersection test with a Ray
        @return std::pair of hit (TBOOL) and distance
        @remarks May return TFALSE positives but will never miss an intersection.
        */
        inline Pair<TBOOL, TF32> intersects(const Vector3<T>& point, const Vector3<T>& dir)
        {
            //list<Plane>::type::const_iterator planeit, planeitend;
			//planeitend = planes.end();
			TBOOL allInside = TTRUE;
			Pair<TBOOL, TF32> ret;
			Pair<TBOOL, TF32> end;
			ret.key() = TFALSE;
			ret.value() = 0.0f;
			end.key() = TFALSE;
			end.value() = 0;


			// derive side
			// NB we don't pass directly since that would require Plane::Side in 
			// interface, which results in recursive includes since Math is so fundamental
			//Plane::Side outside = normalIsOutside ? Plane::POSITIVE_SIDE : Plane::NEGATIVE_SIDE;

			List<Plane<T>>::Iterator itr = planes.begin();
			for (;itr != planes.end(); ++itr)
            {
                const Plane<T>& plane = *itr;
				// is origin outside?
				if (plane.classifyPointRelation(point) == outside)
				{
					allInside = TFALSE;
					// Test single plane
					Vector3<T> intersection;
					TF32 intersectTValue;
					TBOOL bInter = plane.getIntersectionWithLine(point,dir,intersection, intersectTValue);
					if (bInter)
					{
						// Ok, we intersected
						ret.key() = TTRUE;
						// Use the most distant result since convex volume
						ret.value() = max_(ret.value(), intersectTValue);
					}
					else
					{
						ret.key() = TFALSE;
						ret.value() = 0.0f;
						return ret;
					}
				}
				else
				{
					Vector3<T> intersection;
					TF32 intersectTValue;
					TBOOL bInter = plane.getIntersectionWithLine(point,dir,intersection, intersectTValue);
					if (bInter)
					{
						if( !end.key() )
						{
							end.key() = TTRUE;
							end.value() = intersectTValue;
						}
						else
						{
							end.value() = min_(intersectTValue, end.value() );
						}

					}

				}
			}

			if (allInside)
			{
				// Intersecting at 0 distance since inside the volume!
				ret.key() = TTRUE;
				ret.value() = 0.0f;
				return ret;
			}

			if( end.key() )
			{
				if( end.value() < ret.value() )
				{
					ret.key() = TFALSE;
					return ret;
				}
			}
			return ret;
        }
	};
}

#endif