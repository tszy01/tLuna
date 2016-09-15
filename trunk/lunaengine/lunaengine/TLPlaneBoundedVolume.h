#ifndef _TLPLANEBOUNDEDVOLUME_H_
#define _TLPLANEBOUNDEDVOLUME_H_
#include "TLPlane.h"
#include <assert.h>
#include <vector>

namespace TLunaEngine
{
	template<typename T>
	class PlaneBoundedVolume
	{
	public:
        /// Publicly accessible plane list, you can modify this direct
		std::vector<Plane<T>> planes;
        EIntersectionRelation3D outside;

		PlaneBoundedVolume() :outside(ISREL3D_BACK) {}
        /** Constructor, determines which side is deemed to be 'outside' */
        PlaneBoundedVolume(EIntersectionRelation3D theOutside) 
            : outside(theOutside) {}

        /** Intersection test with AABB
        @remarks May return false positives but will never miss an intersection.
        */
        inline bool intersects(const Vector3<T>& center, const Vector3<T>& halfSize) const
        {
			std::vector<Plane<T>>::iterator itr = planes.begin();
			for (;itr != planes.end(); ++itr)
            {
                const Plane<T>& plane = *itr;

                EIntersectionRelation3D side = plane.classifyPointRelation(center, halfSize);
                if (side == outside)
                {
                    // Found a splitting plane therefore return not intersecting
                    return false;
                }
            }

            // couldn't find a splitting plane, assume intersecting
            return true;

        }
        /** Intersection test with Sphere
        @remarks May return false positives but will never miss an intersection.
        */
        inline bool intersects(const Vector3<T>& center, T radius) const
        {
            std::vector<Plane<T>>::iterator itr = planes.begin();
			for (;itr != planes.end(); ++itr)
            {
                const Plane<T>& plane = *itr;

                // Test which side of the plane the sphere is
                T d = plane.getDistanceTo(center);
                // Negate d if planes point inwards
                if (outside == ISREL3D_BACK) d = -d;

                if ( (d - radius) > 0)
                    return false;
            }

            return true;

        }

        /** Intersection test with a Ray
        @return std::pair of hit (bool) and distance
        @remarks May return false positives but will never miss an intersection.
        */
        inline std::pair<bool, TF32> intersects(const Vector3<T>& point, const Vector3<T>& dir)
        {
            //list<Plane>::type::const_iterator planeit, planeitend;
			//planeitend = planes.end();
			bool allInside = true;
			std::pair<bool, TF32> ret;
			std::pair<bool, TF32> end;
			ret.first = false;
			ret.second = 0.0f;
			end.first = false;
			end.second = 0;


			// derive side
			// NB we don't pass directly since that would require Plane::Side in 
			// interface, which results in recursive includes since Math is so fundamental
			//Plane::Side outside = normalIsOutside ? Plane::POSITIVE_SIDE : Plane::NEGATIVE_SIDE;

			std::vector<Plane<T>>::iterator itr = planes.begin();
			for (;itr != planes.end(); ++itr)
            {
                const Plane<T>& plane = *itr;
				// is origin outside?
				if (plane.classifyPointRelation(point) == outside)
				{
					allInside = false;
					// Test single plane
					Vector3<T> intersection;
					bool bInter = plane.getIntersectionWithLine(point,dir,intersection);
					if (bInter)
					{
						// Ok, we intersected
						ret.first = true;
						// Use the most distant result since convex volume
						ret.second = std::max(ret.second, intersection);
					}
					else
					{
						ret.first =false;
						ret.second=0.0f;
						return ret;
					}
				}
				else
				{
					Vector3<T> intersection;
					bool bInter = plane.getIntersectionWithLine(point,dir,intersection);
					if (bInter)
					{
						if( !end.first )
						{
							end.first = true;
							end.second = intersection;
						}
						else
						{
							end.second = std::min( intersection, end.second );
						}

					}

				}
			}

			if (allInside)
			{
				// Intersecting at 0 distance since inside the volume!
				ret.first = true;
				ret.second = 0.0f;
				return ret;
			}

			if( end.first )
			{
				if( end.second < ret.second )
				{
					ret.first = false;
					return ret;
				}
			}
			return ret;
        }
	};
}

#endif