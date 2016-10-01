#ifndef _TLMATHUTILS_H_
#define _TLMATHUTILS_H_

#include "TLRay.h"
#include "TLLine.h"
#include "TLTriangle.h"
#include "TLPlane.h"
#include "TLAABBox.h"
#include "TLSphere.h"
#include "TLVector3.h"
#include "TLVector2.h"
#include "TLVector4.h"
#include "TLQuaternion.h"

namespace TLunaEngine{

	// 三角形与AABB的关系
	//! Determines if the triangle is totally inside a bounding box.
	/** \param box Box to check.
	//\return True if triangle is within the box, otherwise TFALSE. */
	template<typename T>
	TBOOL TriIsTotalInsideBox(const Triangle<T>& tri,const AABBox<T>& box)
	{
		return (box.isPointInside(tri.pointA)) &&
			(box.isPointInside(tri.pointB)) &&
			(box.isPointInside(tri.pointC));
	}

	// 三角形与球检测
	template<typename T>
	TBOOL TriIsTotalInsideSphere(const Triangle<T>& tri,const Sphere<T>& sphere)
	{
		return (sphere.Contains(tri.PointA)) && 
			(sphere.Contains(tri.pointB)) && 
			(sphere.Contains(tri.pointC));
	}

	// 三角形与线段检测
	//! Get an intersection with a 3d line.
	/** \param line Line to intersect with.
	\param outIntersection Place to store the intersection point, if there is one.
	\return True if there was an intersection, TFALSE if not. */
	template<typename T>
	TBOOL TriGetIntersectionWithLine(const Triangle<T>& tri,const Line<T>& line,Vector3<T>& outIntersection,TF32& outTValue)
	{
		return tri.getIntersectionWithLine(line.start,
			line.getVector(), outIntersection, outTValue) &&
			outIntersection.isBetweenPoints(line.start, line.end);
	}

	// 三角形与射线检测
	template<typename T>
	TBOOL TriGetInterscetionWithRay(const Triangle<T>& tri,const Ray<T>& ray,Vector3<T>& outIntersection, TF32& outTValue)
	{
		return tri.getIntersectionWithLine(ray.GetOrig(),ray.GetDir(),outIntersection, outTValue);
	}

	// 平面与线段,射线检测可以直接用Plane成员函数

	// AABB与线段检测
	//! Tests if the box intersects with a line
	/** \param line: Line to test intersection with.
	\return True if there is an intersection , else TFALSE. */
	template<typename T>
	TBOOL AABBIntersectsWithLine(const AABBox<T>& aabb,const Line<T>& line)
	{
		return aabb.intersectsWithLine(line.getMiddle(), line.getVector().normalize(),
				(T)(line.getLength() * 0.5));
	}

	// 球与线段检测可以使用Line的成员函数

	// 球与射线的检测
	template<typename T>
	TBOOL IntersectBall(const Vector3<T> &orig, const Vector3<T> &dir, const Vector3<T> &center, 
							  const T radius, Vector3<T> *pVRet, T *len,TBOOL* pInside)
	{
		// 球心-起始点
		Vector3<T> distance = center - orig;
		// 距离
		T dis = distance.getLength()
		// 标准化方向
		Vector3<T> direction;
		direction = dir.normalize();
		// 方向上的长度
		T dirLen = distance.dotProduct(direction);
		// 如果距离是0
		// 起始点在圆心
		// 碰撞点为从圆心向射线走半径长度的点
		if(abs(dis)<=0.1f)
		{
			if(pVRet)
				(*pVRet)=center + direction * radius;
			if(len)
				(*len)=radius;
			if(pInside)
				(*pInside)=TTRUE;
			return TTRUE;
		}
		// 如果距离和半径一样
		// 起始点在球上
		if(abs(dis-radius)<=0.1f)
		{
			if(pVRet)
				(*pVRet)=orig;
			if(len)
				(*len)=(T)0;
			if(pInside)
				(*pInside)=TFALSE;
			return TTRUE;
		}
		// 如果距离比半径小
		// 起始点在球内部
		if(dis<radius)
		{
			TF32 flen = 0;
			// 如果投影是0
			// 碰撞点平方=半径平方-距离平方
			if(abs(dirLen)<=0.1f)
			{
				flen = sqrt(radius*radius - dis*dis);
			}
			// 如果投影大于或者小于0
			else
			{
				flen = sqrt(radius*radius - dis*dis + dirLen*dirLen) + dirLen;
			}
			if(pVRet)
				(*pVRet)=orig + direction * flen;
			if(len)
				(*len)=(T)flen;
			if(pInside)
				(*pInside)=TTRUE;
			return TTRUE;
		}
		// 如果距离比半径大
		// 起始点在球内部
		// 只有投影>0时才可能相交
		if(dis>radius && dirLen>0.0f)
		{
			TF32 af = dis*dis - dirLen*dirLen;
			TF32 a = sqrt(af);
			// 这个值必须小于等于半径才相交
			if(abs(a-radius)<=0.1f)
			{
				if(pVRet)
					(*pVRet)=orig + direction * dirLen;
				if(len)
					(*len)=(T)dirLen;
				if(pInside)
					(*pInside)=TFALSE;
				return TTRUE;
			}
			else if(a<=radius)
			{
				TF32 flen = dirLen - sqrt(radius*radius - af);
				if(pVRet)
					(*pVRet)=orig + direction * flen;
				if(len)
					(*len)=(T)flen;
				if(pInside)
					(*pInside)=TFALSE;
				return TTRUE;
			}
		}
		return TFALSE;
	}

	template<typename T>
	TBOOL IntersectBallSimple(const Vector3<T>& orig,const Vector3<T>& dir,const Vector3<T>& center,
			const T radius,Vector3<T>* pVRet,T* len,TBOOL* pInside)
	{
		// 球心-起始点
		Vector3<T> distance = center - orig;
		// 距离
		T dis = distance.getLength();
		// 如果距离比半径小
		// 起始点在球内部
		if(dis<=radius+0.1f)
		{
			if(pVRet)
				(*pVRet)=orig;
			if(len)
				(*len)=(T)0;
			if(pInside)
				(*pInside)=TTRUE;
			return TTRUE;
		}
		// 标准化方向
		D3DXVECTOR3 direction;
		direction = dir.normalize();
		// 方向上的长度
		T dirLen = distance.dotProduct(direction);
		// 如果距离比半径大
		// 起始点在球内部
		// 只有投影>0时才可能相交
		if(dis>radius && dirLen>0.0f)
		{
			TF32 af = dis*dis - dirLen*dirLen;
			TF32 a = sqrt(af);
			// 这个值必须小于等于半径才相交
			if(abs(a-radius)<=0.1f)
			{
				if(pVRet)
					(*pVRet)=orig + direction * dirLen;
				if(len)
					(*len)=(T)dirLen;
				if(pInside)
					(*pInside)=TFALSE;
				return TTRUE;
			}
			else if(a<=radius)
			{
				TF32 flen = dirLen - sqrt(radius*radius - af);
				if(pVRet)
					(*pVRet)=orig + direction * flen;
				if(len)
					(*len)=(T)flen;
				if(pInside)
					(*pInside)=TFALSE;
				return TTRUE;
			}
		}
		return TFALSE;
	}



	// AABB与射线的检测
	template<typename T>
	TBOOL IntersectAABBSimple(const Vector3<T> &orig, const Vector3<T> &dir, const Vector3<T> &min, 
									const Vector3<T> &max, Vector3<T> *pVRet, T *len, TBOOL *pInside)
	{
		TBOOL inside = TTRUE;	// 假设在内部
		TF32 xt=-1,yt=-1,zt=-1;	// 三个坐标的差,初始化为-1
		// 标准化方向
		Vector3<T> direction;
		direction = dir.normalize();

		// 分别比较三个坐标轴,计算出插值
		if(orig.X < min.X)
		{
			xt = min.X - orig.X;	// 计算差
			xt /= direction.X;			// 和方向位置的比值(是方向位置的倍数)
			inside = TFALSE;			// X轴不在内部
		}
		else if(orig.X > max.X)
		{
			xt = max.X - orig.X;
			xt /= direction.X;
			inside = TFALSE;
		}
		if(orig.Y < min.Y)
		{
			yt = min.Y - orig.Y;	// 计算差
			yt /= direction.Y;			// 和方向位置的比值(是方向位置的倍数)
			inside = TFALSE;			// X轴不在内部
		}
		else if(orig.Y > max.Y)
		{
			yt = max.Y - orig.Y;
			yt /= direction.Y;
			inside = TFALSE;
		}
		if(orig.Z < min.Z)
		{
			zt = min.Z - orig.Z;	// 计算差
			zt /= direction.Z;			// 和方向位置的比值(是方向位置的倍数)
			inside = TFALSE;			// X轴不在内部
		}
		else if(orig.Z > max.Z)
		{
			zt = max.Z - orig.Z;
			zt /= direction.Z;
			inside = TFALSE;
		}
		// 如果在内部
		if(inside)
		{
			if(pVRet)
				(*pVRet)=orig;
			if(len)
				(*len)=(T)0;
			if(pInside)
				(*pInside)=TTRUE;
			return TTRUE;
		}
		// 选择适当的相交平面
		// 那个轴上的差的比值最大,说明和另外两个轴组成的平面先相交
		TS32 which = 0;
		TF32 t = xt;
		if(yt>t)
		{
			which = 1;
			t = yt;
		}
		if(zt>t)
		{
			which = 2;
			t = zt;
		}
		// 根据相交平面,判断是否在盒内
		TBOOL result = TTRUE;	// 最后结果
		TF32 x=0,y=0,z=0;	// 碰撞点坐标
		switch(which)
		{
		case 0:	// YZ平面
			{
				y = orig.Y + direction.Y * t;
				if(y<min.Y || y>max.Y)
				{
					result = TFALSE;
					break;
				}
				z = orig.Z + direction.Z * t;
				if(z<min.Z || z>max.Z)
				{
					result = TFALSE;
					break;
				}
				x = orig.X + direction.X * t;
			}
			break;
		case 1:	// XZ平面
			{
				x = orig.X + direction.X * t;
				if(x<min.X || x>max.X)
				{
					result = TFALSE;
					break;
				}
				z = orig.Z + direction.Z * t;
				if(z<min.Z || z>max.Z)
				{
					result = TFALSE;
					break;
				}
				y = orig.Y + direction.Y * t;
			}
			break;
		case 2:	// XY平面
			{
				x = orig.X + direction.X * t;
				if(x<min.X || x>max.X)
				{
					result = TFALSE;
					break;
				}
				y = orig.Y + direction.Y * t;
				if(y<min.Y || y>max.Y)
				{
					result = TFALSE;
					break;
				}
				z = orig.Z + direction.Z * t;
			}
			break;
		}
		if(result)
		{
			if(pVRet)
				(*pVRet)=Vector3<T>(x,y,z);
			if(len)
				(*len)=(T)t;
			if(pInside)
				(*pInside)=TFALSE;
		}
		return result;
	}

	// 计算切线方向，用于法线贴图
	template<typename T>
	TVOID CalculateTangentArray(TS32 vertexCount, const Vector3<T> *vertex, const Vector3<T> *normal,
		const Vector2<T> *texcoord, TS32 triangleCount, const TS32* indexArray, Vector4<T> *tangent)
	{
		Vector3<T> *tan1 = new Vector3<T>[vertexCount * 2];
		Vector3<T> *tan2 = tan1 + vertexCount;
		//::ZeroMemory(tan1, vertexCount * sizeof(Ogre::Vector3) * 2);
		memset(tan1,0,vertexCount * sizeof(Vector3<T>) * 2);
    
		for (TS32 a = 0; a < triangleCount; a++)
		{
			TS32 i1 = indexArray[a*3+0];
			TS32 i2 = indexArray[a*3+1];
			TS32 i3 = indexArray[a*3+2];
        
			const Ogre::Vector3& v1 = vertex[i1];
			const Ogre::Vector3& v2 = vertex[i2];
			const Ogre::Vector3& v3 = vertex[i3];
        
			const Ogre::Vector2& w1 = texcoord[i1];
			const Ogre::Vector2& w2 = texcoord[i2];
			const Ogre::Vector2& w3 = texcoord[i3];
        
			TF32 x1 = v2.x - v1.x;
			TF32 x2 = v3.x - v1.x;
			TF32 y1 = v2.y - v1.y;
			TF32 y2 = v3.y - v1.y;
			TF32 z1 = v2.z - v1.z;
			TF32 z2 = v3.z - v1.z;
        
			TF32 s1 = w2.x - w1.x;
			TF32 s2 = w3.x - w1.x;
			TF32 t1 = w2.y - w1.y;
			TF32 t2 = w3.y - w1.y;
        
			TF32 r = 1.0F / (s1 * t2 - s2 * t1);
			Vector3<T> sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
					(t2 * z1 - t1 * z2) * r);
			Vector3<T> tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
					(s1 * z2 - s2 * z1) * r);
        
			tan1[i1] += sdir;
			tan1[i2] += sdir;
			tan1[i3] += sdir;
        
			tan2[i1] += tdir;
			tan2[i2] += tdir;
			tan2[i3] += tdir;
		}
    
		for (TS32 a = 0; a < vertexCount; a++)
		{
			const Vector3<T>& n = normal[a];
			const Vector3<T>& t = tan1[a];
        
			// Gram-Schmidt orthogonalize
			tangent[a] = (t - n * n.dotProduct(t)).normalisedCopy();
        
			// Calculate handedness
			tangent[a].w = (n.crossProduct(t).dotProduct(tan2[a]) < 0.0F) ? -1.0F : 1.0F;
		}
    
		delete[] tan1;
	}

	/** Gets the shortest arc quaternion to rotate this vector to the destination
	vector.
	@remarks
	If you call this with a dest vector that is close to the inverse
	of this vector, we will rotate 180 degrees around the 'fallbackAxis'
	(if specified, or a generated axis if not) since in this case
	ANY axis of rotation is valid.
	*/
	template<typename T>
	Quaternion<T> getRotationBetweenVectors(const Vector3<T>& src, const Vector3<T>& dest,
		const Vector3<T>& fallbackAxis = Vector3<T>(0,0,0))
	{
		// Based on Stan Melax's article in Game Programming Gems
		Quaternion<T> q;
		// Copy, since cannot modify local
		Vector3<T> v0 = src;
		Vector3<T> v1 = dest;
		v0.normalise();
		v1.normalise();

		T d = v0.dotProduct(v1);
		// If dot == 1, vectors are the same
		if (d >= (T)1.0f)
		{
			q.makeIdentity();
			return q;
		}
		if (d < (T)(1e-6f - 1.0f))
		{
			if (fallbackAxis.isZero()==TFALSE)
			{
				// rotate 180 degrees about the fallback axis
				q.FromAngleAxis(PI, fallbackAxis);
			}
			else
			{
				// Generate an axis
				Vector3<T> axis = Vector3<T>(1,0,0).crossProduct(src);
				if (axis.isZeroLength()) // pick another if colinear
					axis = Vector3<T>(0, 1, 0).crossProduct(src);
				axis.normalize();
				q.FromAngleAxis(PI, axis);
			}
		}
		else
		{
			TF32 s = sqrtf((1 + (TF32)d) * 2);
			TF32 invs = 1 / s;

			Vector3<T> c = v0.crossProduct(v1);

			q.x = (T)(c.x * invs);
			q.y = (T)(c.y * invs);
			q.z = (T)(c.z * invs);
			q.w = (T)(s * 0.5f);
			q.normalize();
		}
		return q;
	}

	// plane / aabbox intersection test
	template<typename T>
	TBOOL intersects(const Plane<T>& plane, const AABBox<T>& box)
	{
		if (plane.classifyPointRelation(box.getCenter(), box.getHalfExtent()) == ISREL3D_SPANNING)
			return TTRUE;
		return TFALSE;
	}

	// plane / ray intersection test
	template<typename T>
	TBOOL intersects(const Plane<T>& plane, const Ray<T>& ray, Vector3<T>& outInterection, TF32& outTValue)
	{
		if (plane.getIntersectionWithLine(ray.m_Orig, ray.m_Dir, outInterection, outTValue) == TTRUE)
		{
			if (outTValue >= 0.0f)
			{
				return TTRUE;
			}
		}
		return TFALSE;
	}

	// aabbox edge cross test
	template<typename T>
	TBOOL TEST_CROSS_EDGE_BOX_MCR(const Vector3<T>& edge, const Vector3<T>& absolute_edge, const Vector3<T>& pointa,
		const Vector3<T>& pointb, const Vector3<T>& _extend, TS32 i_dir_0, TS32 i_dir_1, TS32 i_comp_0, TS32 i_comp_1)
	{
		const T dir0 = -edge[i_dir_0];
		const T dir1 = edge[i_dir_1];
		T pmin = pointa[i_comp_0] * dir0 + pointa[i_comp_1] * dir1;
		T pmax = pointb[i_comp_0] * dir0 + pointb[i_comp_1] * dir1;

		if (pmin>pmax)
		{
			pmin = pmin + pmax;
			pmax = pmin - pmax;
			pmin = pmin - pmax;
		}

		const T abs_dir0 = absolute_edge[i_dir_0];
		const T abs_dir1 = absolute_edge[i_dir_1];
		const T rad = _extend[i_comp_0] * abs_dir0 + _extend[i_comp_1] * abs_dir1;

		if (pmin>rad || -rad>pmax)
			return TFALSE;

		return TTRUE;
	}

	// triangle / aabbox intersection test
	// only use the first 3 verticies in poly
	template<typename T>
	TBOOL intersects(const Polygon<T>& poly, const AABBox<T>& box)
	{
		T minX, maxX, minY, maxY, minZ, maxZ, x, y, z;

		minX = maxX = poly.getVertex(0).X;
		minY = maxY = poly.getVertex(0).Y;
		minZ = maxZ = poly.getVertex(0).Z;

		for (TS32 i = 1; i < 3; ++i)
		{
			x = poly.getVertex(i).x;
			y = poly.getVertex(i).y;
			z = poly.getVertex(i).z;

			if (x < minX)
				minX = x;

			if (x > maxX)
				maxX = x;

			if (y < minY)
				minY = y;

			if (y > maxY)
				maxY = y;

			if (z < minZ)
				minZ = z;

			if (z > maxZ)
				maxZ = z;
		}

		if (minX > box.getMaximum().x || maxX < box.getMinimum().x || minY > box.getMaximum().y ||
			maxY < box.getMinimum().y || minZ > box.getMaximum().z || maxZ < box.getMinimum().z)
		{
			return TFALSE;
		}

		Plane<T> triPlane(poly.getVertex(0), poly.getVertex(1), poly.getVertex(2));
		if (!intersects(triPlane, box))
			return TFALSE;
		const Vector3<T> v1(poly.getVertex(0) - box.getCenter());
		const Vector3<T> v2(poly.getVertex(1) - box.getCenter());
		const Vector3<T> v3(poly.getVertex(2) - box.getCenter());
		Vector3<T> extends = box.getMaximum() - box.getCenter();
		//First axis
		Vector3<T> diff(v2 - v1);
		Vector3<T> abs_diff = Vector3<T>(fabs(diff.x), fabs(diff.y), fabs(diff.z)); //diff.absolute();
		//Test With X axis
		if (!TEST_CROSS_EDGE_BOX_MCR(diff, abs_diff, v1, v3, extends, 2, 1, 1, 2))
			return TFALSE;
		//Test With Y axis
		if (!TEST_CROSS_EDGE_BOX_MCR(diff, abs_diff, v1, v3, extends, 0, 2, 2, 0))
			return TFALSE;

		//Test With Z axis
		if (!TEST_CROSS_EDGE_BOX_MCR(diff, abs_diff, v1, v3, extends, 1, 0, 0, 1))
			return TFALSE;


		diff = v3 - v2;
		abs_diff = Vector3<T>(fabs(diff.x), fabs(diff.y), fabs(diff.z)); //diff.absolute();
		//Test With X axis
		if (!TEST_CROSS_EDGE_BOX_MCR(diff, abs_diff, v2, v1, extends, 2, 1, 1, 2))
			return TFALSE;

		//Test With Y axis
		if (!TEST_CROSS_EDGE_BOX_MCR(diff, abs_diff, v2, v1, extends, 0, 2, 2, 0))
			return TFALSE;
		//Test With Z axis
		if (!TEST_CROSS_EDGE_BOX_MCR(diff, abs_diff, v2, v1, extends, 1, 0, 0, 1))
			return TFALSE;

		diff = v1 - v3;
		abs_diff = Vector3<T>(fabs(diff.x), fabs(diff.y), fabs(diff.z)); //diff.absolute();
		//Test With X axis
		if (!TEST_CROSS_EDGE_BOX_MCR(diff, abs_diff, v3, v2, extends, 2, 1, 1, 2))
			return TFALSE;
		//Test With Y axis
		if (!TEST_CROSS_EDGE_BOX_MCR(diff, abs_diff, v3, v2, extends, 0, 2, 2, 0))
			return TFALSE;
		//Test With Z axis
		if (!TEST_CROSS_EDGE_BOX_MCR(diff, abs_diff, v3, v2, extends, 1, 0, 0, 1))
			return TFALSE;

		return TTRUE;
	}

	// plane clip two points of polygon
	template<typename T>
	TVOID planeClipPolygonLine(const Vector3<T>& point0, const Vector3<T>& point1, T dist0, T dist1, Polygon<T>& polyClipped)
	{
		TBOOL _prevclassif = (dist0>(T)DBL_EPSILON);
		TBOOL _classif = (dist1>(T)DBL_EPSILON);
		if (_classif != _prevclassif)
		{
			T blendfactor = (T)(-dist0 / (dist1 - dist0));

			//assert(clipped_count < MAX_CLIPPED_POINTS);
			polyClipped.insertVertex((1 - blendfactor)*point0 + blendfactor * point1);
		}
		if (!_classif)
		{
			//assert(clipped_count < MAX_CLIPPED_POINTS);
			polyClipped.insertVertex(point1);
		}
	}

	// plane clip polygon points
	template<typename T>
	TVOID planeClipPolygon(const Plane<T>& plane, const Polygon<T>& polyOrig, Polygon<T>& polyClipped, TBOOL positive)
	{
		//clip first point
		T firstdist = plane.getDistanceTo(polyOrig.getVertex(0));
		if (positive)
		{
			firstdist *= -1.0;
		}
		if (!(firstdist>(T)DBL_EPSILON))
		{
			//assert(clipped_count < MAX_CLIPPED_POINTS);
			polyClipped.insertVertex(polyOrig.getVertex(0));
		}
		T olddist = firstdist;
		for (TS32 i = 1; i<(TS32)polyOrig.getVertexCount(); i++)
		{
			T dist = plane.getDistanceTo(polyOrig.getVertex(i));
			if (positive)
			{
				dist *= (T)-1.0;
			}
			planeClipPolygonLine(polyOrig.getVertex(i - 1), polyOrig.getVertex(i), olddist, dist, polyClipped);
			olddist = dist;
		}
		//RETURN TO FIRST  point
		planeClipPolygonLine(polyOrig.getVertex(polyOrig.getVertexCount() - 1), polyOrig.getVertex(0), olddist, firstdist, polyClipped);
	}
}

#endif