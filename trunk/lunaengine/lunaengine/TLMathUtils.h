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

	// ��������AABB�Ĺ�ϵ
	//! Determines if the triangle is totally inside a bounding box.
	/** \param box Box to check.
	//\return True if triangle is within the box, otherwise false. */
	template<typename T>
	bool TriIsTotalInsideBox(const Triangle<T>& tri,const AABBox<T>& box)
	{
		return (box.isPointInside(tri.pointA)) &&
			(box.isPointInside(tri.pointB)) &&
			(box.isPointInside(tri.pointC));
	}

	// ������������
	template<typename T>
	bool TriIsTotalInsideSphere(const Triangle<T>& tri,const Sphere<T>& sphere)
	{
		return (sphere.Contains(tri.PointA)) && 
			(sphere.Contains(tri.pointB)) && 
			(sphere.Contains(tri.pointC));
	}

	// ���������߶μ��
	//! Get an intersection with a 3d line.
	/** \param line Line to intersect with.
	\param outIntersection Place to store the intersection point, if there is one.
	\return True if there was an intersection, false if not. */
	template<typename T>
	bool TriGetIntersectionWithLine(const Triangle<T>& tri,const Line<T>& line,Vector3<T>& outIntersection)
	{
		return tri.getIntersectionWithLine(line.start,
			line.getVector(), outIntersection) &&
			outIntersection.isBetweenPoints(line.start, line.end);
	}

	// �����������߼��
	template<typename T>
	bool TriGetInterscetionWithRay(const Triangle<T>& tri,const Ray<T>& ray,Vector3<T>& outIntersection)
	{
		return tri.getIntersectionWithLine(ray.GetOrig(),ray.GetDir(),outIntersection);
	}

	// ƽ�����߶�,���߼�����ֱ����Plane��Ա����

	// AABB���߶μ��
	//! Tests if the box intersects with a line
	/** \param line: Line to test intersection with.
	\return True if there is an intersection , else false. */
	template<typename T>
	bool AABBIntersectsWithLine(const AABBox<T>& aabb,const Line<T>& line)
	{
		return aabb.intersectsWithLine(line.getMiddle(), line.getVector().normalize(),
				(T)(line.getLength() * 0.5));
	}

	// �����߶μ�����ʹ��Line�ĳ�Ա����

	// �������ߵļ��
	template<typename T>
	bool IntersectBall(const Vector3<T> &orig, const Vector3<T> &dir, const Vector3<T> &center, 
							  const T radius, Vector3<T> *pVRet, T *len,bool* pInside)
	{
		// ����-��ʼ��
		Vector3<T> distance = center - orig;
		// ����
		T dis = distance.getLength()
		// ��׼������
		Vector3<T> direction;
		direction = dir.normalize();
		// �����ϵĳ���
		T dirLen = distance.dotProduct(direction);
		// ���������0
		// ��ʼ����Բ��
		// ��ײ��Ϊ��Բ���������߰뾶���ȵĵ�
		if(abs(dis)<=0.1f)
		{
			if(pVRet)
				(*pVRet)=center + direction * radius;
			if(len)
				(*len)=radius;
			if(pInside)
				(*pInside)=true;
			return true;
		}
		// �������Ͱ뾶һ��
		// ��ʼ��������
		if(abs(dis-radius)<=0.1f)
		{
			if(pVRet)
				(*pVRet)=orig;
			if(len)
				(*len)=(T)0;
			if(pInside)
				(*pInside)=false;
			return true;
		}
		// �������Ȱ뾶С
		// ��ʼ�������ڲ�
		if(dis<radius)
		{
			float flen = 0;
			// ���ͶӰ��0
			// ��ײ��ƽ��=�뾶ƽ��-����ƽ��
			if(abs(dirLen)<=0.1f)
			{
				flen = sqrt(radius*radius - dis*dis);
			}
			// ���ͶӰ���ڻ���С��0
			else
			{
				flen = sqrt(radius*radius - dis*dis + dirLen*dirLen) + dirLen;
			}
			if(pVRet)
				(*pVRet)=orig + direction * flen;
			if(len)
				(*len)=(T)flen;
			if(pInside)
				(*pInside)=true;
			return true;
		}
		// �������Ȱ뾶��
		// ��ʼ�������ڲ�
		// ֻ��ͶӰ>0ʱ�ſ����ཻ
		if(dis>radius && dirLen>0.0f)
		{
			float af = dis*dis - dirLen*dirLen;
			float a = sqrt(af);
			// ���ֵ����С�ڵ��ڰ뾶���ཻ
			if(abs(a-radius)<=0.1f)
			{
				if(pVRet)
					(*pVRet)=orig + direction * dirLen;
				if(len)
					(*len)=(T)dirLen;
				if(pInside)
					(*pInside)=false;
				return true;
			}
			else if(a<=radius)
			{
				float flen = dirLen - sqrt(radius*radius - af);
				if(pVRet)
					(*pVRet)=orig + direction * flen;
				if(len)
					(*len)=(T)flen;
				if(pInside)
					(*pInside)=false;
				return true;
			}
		}
		return false;
	}

	template<typename T>
	bool IntersectBallSimple(const Vector3<T>& orig,const Vector3<T>& dir,const Vector3<T>& center,
			const T radius,Vector3<T>* pVRet,T* len,bool* pInside)
	{
		// ����-��ʼ��
		Vector3<T> distance = center - orig;
		// ����
		T dis = distance.getLength();
		// �������Ȱ뾶С
		// ��ʼ�������ڲ�
		if(dis<=radius+0.1f)
		{
			if(pVRet)
				(*pVRet)=orig;
			if(len)
				(*len)=(T)0;
			if(pInside)
				(*pInside)=true;
			return true;
		}
		// ��׼������
		D3DXVECTOR3 direction;
		direction = dir.normalize();
		// �����ϵĳ���
		T dirLen = distance.dotProduct(direction);
		// �������Ȱ뾶��
		// ��ʼ�������ڲ�
		// ֻ��ͶӰ>0ʱ�ſ����ཻ
		if(dis>radius && dirLen>0.0f)
		{
			float af = dis*dis - dirLen*dirLen;
			float a = sqrt(af);
			// ���ֵ����С�ڵ��ڰ뾶���ཻ
			if(abs(a-radius)<=0.1f)
			{
				if(pVRet)
					(*pVRet)=orig + direction * dirLen;
				if(len)
					(*len)=(T)dirLen;
				if(pInside)
					(*pInside)=false;
				return true;
			}
			else if(a<=radius)
			{
				float flen = dirLen - sqrt(radius*radius - af);
				if(pVRet)
					(*pVRet)=orig + direction * flen;
				if(len)
					(*len)=(T)flen;
				if(pInside)
					(*pInside)=false;
				return true;
			}
		}
		return false;
	}



	// AABB�����ߵļ��
	template<typename T>
	bool IntersectAABBSimple(const Vector3<T> &orig, const Vector3<T> &dir, const Vector3<T> &min, 
									const Vector3<T> &max, Vector3<T> *pVRet, T *len, bool *pInside)
	{
		bool inside = true;	// �������ڲ�
		float xt=-1,yt=-1,zt=-1;	// ��������Ĳ�,��ʼ��Ϊ-1
		// ��׼������
		Vector3<T> direction;
		direction = dir.normalize();

		// �ֱ�Ƚ�����������,�������ֵ
		if(orig.X < min.X)
		{
			xt = min.X - orig.X;	// �����
			xt /= direction.X;			// �ͷ���λ�õı�ֵ(�Ƿ���λ�õı���)
			inside = false;			// X�᲻���ڲ�
		}
		else if(orig.X > max.X)
		{
			xt = max.X - orig.X;
			xt /= direction.X;
			inside = false;
		}
		if(orig.Y < min.Y)
		{
			yt = min.Y - orig.Y;	// �����
			yt /= direction.Y;			// �ͷ���λ�õı�ֵ(�Ƿ���λ�õı���)
			inside = false;			// X�᲻���ڲ�
		}
		else if(orig.Y > max.Y)
		{
			yt = max.Y - orig.Y;
			yt /= direction.Y;
			inside = false;
		}
		if(orig.Z < min.Z)
		{
			zt = min.Z - orig.Z;	// �����
			zt /= direction.Z;			// �ͷ���λ�õı�ֵ(�Ƿ���λ�õı���)
			inside = false;			// X�᲻���ڲ�
		}
		else if(orig.Z > max.Z)
		{
			zt = max.Z - orig.Z;
			zt /= direction.Z;
			inside = false;
		}
		// ������ڲ�
		if(inside)
		{
			if(pVRet)
				(*pVRet)=orig;
			if(len)
				(*len)=(T)0;
			if(pInside)
				(*pInside)=true;
			return true;
		}
		// ѡ���ʵ����ཻƽ��
		// �Ǹ����ϵĲ�ı�ֵ���,˵����������������ɵ�ƽ�����ཻ
		int which = 0;
		float t = xt;
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
		// �����ཻƽ��,�ж��Ƿ��ں���
		bool result = true;	// �����
		float x=0,y=0,z=0;	// ��ײ������
		switch(which)
		{
		case 0:	// YZƽ��
			{
				y = orig.Y + direction.Y * t;
				if(y<min.Y || y>max.Y)
				{
					result = false;
					break;
				}
				z = orig.Z + direction.Z * t;
				if(z<min.Z || z>max.Z)
				{
					result = false;
					break;
				}
				x = orig.X + direction.X * t;
			}
			break;
		case 1:	// XZƽ��
			{
				x = orig.X + direction.X * t;
				if(x<min.X || x>max.X)
				{
					result = false;
					break;
				}
				z = orig.Z + direction.Z * t;
				if(z<min.Z || z>max.Z)
				{
					result = false;
					break;
				}
				y = orig.Y + direction.Y * t;
			}
			break;
		case 2:	// XYƽ��
			{
				x = orig.X + direction.X * t;
				if(x<min.X || x>max.X)
				{
					result = false;
					break;
				}
				y = orig.Y + direction.Y * t;
				if(y<min.Y || y>max.Y)
				{
					result = false;
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
				(*pInside)=false;
		}
		return result;
	}

	// �������߷������ڷ�����ͼ
	template<typename T>
	void CalculateTangentArray(long vertexCount, const Vector3<T> *vertex, const Vector3<T> *normal,
		const Vector2<T> *texcoord, long triangleCount, const long* indexArray, Vector4<T> *tangent)
	{
		Vector3<T> *tan1 = new Vector3<T>[vertexCount * 2];
		Vector3<T> *tan2 = tan1 + vertexCount;
		//::ZeroMemory(tan1, vertexCount * sizeof(Ogre::Vector3) * 2);
		memset(tan1,0,vertexCount * sizeof(Vector3<T>) * 2);
    
		for (long a = 0; a < triangleCount; a++)
		{
			long i1 = indexArray[a*3+0];
			long i2 = indexArray[a*3+1];
			long i3 = indexArray[a*3+2];
        
			const Ogre::Vector3& v1 = vertex[i1];
			const Ogre::Vector3& v2 = vertex[i2];
			const Ogre::Vector3& v3 = vertex[i3];
        
			const Ogre::Vector2& w1 = texcoord[i1];
			const Ogre::Vector2& w2 = texcoord[i2];
			const Ogre::Vector2& w3 = texcoord[i3];
        
			float x1 = v2.x - v1.x;
			float x2 = v3.x - v1.x;
			float y1 = v2.y - v1.y;
			float y2 = v3.y - v1.y;
			float z1 = v2.z - v1.z;
			float z2 = v3.z - v1.z;
        
			float s1 = w2.x - w1.x;
			float s2 = w3.x - w1.x;
			float t1 = w2.y - w1.y;
			float t2 = w3.y - w1.y;
        
			float r = 1.0F / (s1 * t2 - s2 * t1);
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
    
		for (long a = 0; a < vertexCount; a++)
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
			if (fallbackAxis.isZero()==false)
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
	bool intersects(const Plane<T>& plane, const AABBox<T>& box)
	{
		if (plane.classifyPointRelation(box.getCenter(), box.getHalfExtent()) == ISREL3D_SPANNING)
			return true;
		return false;
	}

	// aabbox edge cross test
	template<typename T>
	bool TEST_CROSS_EDGE_BOX_MCR(const Vector3<T>& edge, const Vector3<T>& absolute_edge, const Vector3<T>& pointa,
		const Vector3<T>& pointb, const Vector3<T>& _extend, int i_dir_0, int i_dir_1, int i_comp_0, int i_comp_1)
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
			return false;

		return true;
	}

	// triangle / aabbox intersection test
	// only use the first 3 verticies in poly
	template<typename T>
	bool intersects(const Polygon<T>& poly, const AABBox<T>& box)
	{
		T minX, maxX, minY, maxY, minZ, maxZ, x, y, z;

		minX = maxX = poly.getVertex(0).X;
		minY = maxY = poly.getVertex(0).Y;
		minZ = maxZ = poly.getVertex(0).Z;

		for (int i = 1; i < 3; ++i)
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
			return false;
		}

		Plane<T> triPlane(poly.getVertex(0), poly.getVertex(1), poly.getVertex(2));
		if (!intersects(triPlane, box))
			return false;
		const Vector3<T> v1(poly.getVertex(0) - box.getCenter());
		const Vector3<T> v2(poly.getVertex(1) - box.getCenter());
		const Vector3<T> v3(poly.getVertex(2) - box.getCenter());
		Vector3<T> extends = box.getMaximum() - box.getCenter();
		//First axis
		Vector3<T> diff(v2 - v1);
		Vector3<T> abs_diff = Vector3<T>(fabs(diff.x), fabs(diff.y), fabs(diff.z)); //diff.absolute();
		//Test With X axis
		if (!TEST_CROSS_EDGE_BOX_MCR(diff, abs_diff, v1, v3, extends, 2, 1, 1, 2))
			return false;
		//Test With Y axis
		if (!TEST_CROSS_EDGE_BOX_MCR(diff, abs_diff, v1, v3, extends, 0, 2, 2, 0))
			return false;

		//Test With Z axis
		if (!TEST_CROSS_EDGE_BOX_MCR(diff, abs_diff, v1, v3, extends, 1, 0, 0, 1))
			return false;


		diff = v3 - v2;
		abs_diff = Vector3<T>(fabs(diff.x), fabs(diff.y), fabs(diff.z)); //diff.absolute();
		//Test With X axis
		if (!TEST_CROSS_EDGE_BOX_MCR(diff, abs_diff, v2, v1, extends, 2, 1, 1, 2))
			return false;

		//Test With Y axis
		if (!TEST_CROSS_EDGE_BOX_MCR(diff, abs_diff, v2, v1, extends, 0, 2, 2, 0))
			return false;
		//Test With Z axis
		if (!TEST_CROSS_EDGE_BOX_MCR(diff, abs_diff, v2, v1, extends, 1, 0, 0, 1))
			return false;

		diff = v1 - v3;
		abs_diff = Vector3<T>(fabs(diff.x), fabs(diff.y), fabs(diff.z)); //diff.absolute();
		//Test With X axis
		if (!TEST_CROSS_EDGE_BOX_MCR(diff, abs_diff, v3, v2, extends, 2, 1, 1, 2))
			return false;
		//Test With Y axis
		if (!TEST_CROSS_EDGE_BOX_MCR(diff, abs_diff, v3, v2, extends, 0, 2, 2, 0))
			return false;
		//Test With Z axis
		if (!TEST_CROSS_EDGE_BOX_MCR(diff, abs_diff, v3, v2, extends, 1, 0, 0, 1))
			return false;

		return true;
	}

	// plane clip two points of polygon
	template<typename T>
	void planeClipPolygonLine(const Vector3<T>& point0, const Vector3<T>& point1, T dist0, T dist1, Polygon<T>& polyClipped)
	{
		bool _prevclassif = (dist0>(T)DBL_EPSILON);
		bool _classif = (dist1>(T)DBL_EPSILON);
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
	void planeClipPolygon(const Plane<T>& plane, const Polygon<T>& polyOrig, Polygon<T>& polyClipped, bool positive)
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
		for (int i = 1; i<(int)polyOrig.getVertexCount(); i++)
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