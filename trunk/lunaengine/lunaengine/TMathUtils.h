#pragma once

#include "TRay.h"
#include "TLine.h"
#include "TTriangle.h"
#include "TPlane.h"
#include "TAABBbox.h"
#include "TSphere.h"
#include "TVector3.h"
#include "TVector2.h"
#include "TVector4.h"

namespace TLunaEngine{

	// 三角形与AABB的关系
	//! Determines if the triangle is totally inside a bounding box.
	/** \param box Box to check.
	//\return True if triangle is within the box, otherwise false. */
	template<typename T>
	bool TriIsTotalInsideBox(const TTriangle<T>& tri,const TAABBbox<T>& box)
	{
		return (box.isPointInside(tri.pointA)) &&
			(box.isPointInside(tri.pointB)) &&
			(box.isPointInside(tri.pointC));
	}

	// 三角形与球检测
	template<typename T>
	bool TriIsTotalInsideSphere(const TTriangle<T>& tri,const TSphere<T>& sphere)
	{
		return (sphere.Contains(tri.PointA)) && 
			(sphere.Contains(tri.pointB)) && 
			(sphere.Contains(tri.pointC));
	}

	// 三角形与线段检测
	//! Get an intersection with a 3d line.
	/** \param line Line to intersect with.
	\param outIntersection Place to store the intersection point, if there is one.
	\return True if there was an intersection, false if not. */
	template<typename T>
	bool TriGetIntersectionWithLine(const TTriangle<T>& tri,const TLine<T>& line,TVector3<T>& outIntersection)
	{
		return tri.getIntersectionWithLine(line.start,
			line.getVector(), outIntersection) &&
			outIntersection.isBetweenPoints(line.start, line.end);
	}

	// 三角形与射线检测
	template<typename T>
	bool TriGetInterscetionWithRay(const TTriangle<T>& tri,const TRay<T>& ray,TVector3<T>& outIntersection)
	{
		return tri.getIntersectionWithLine(ray.GetOrig(),ray.GetDir(),outIntersection);
	}

	// 平面与线段,射线检测可以直接用TPlane成员函数

	// AABB与线段检测
	//! Tests if the box intersects with a line
	/** \param line: Line to test intersection with.
	\return True if there is an intersection , else false. */
	template<typename T>
	bool AABBIntersectsWithLine(const TAABBbox<T>& aabb,const TLine<T>& line)
	{
		return aabb.intersectsWithLine(line.getMiddle(), line.getVector().normalize(),
				(T)(line.getLength() * 0.5));
	}

	// 球与线段检测可以使用TLine的成员函数

	// 球与射线的检测
	template<typename T>
	bool IntersectBall(const TVector3<T> &orig, const TVector3<T> &dir, const TVector3<T> &center, 
							  const T radius, TVector3<T> *pVRet, T *len,bool* pInside)
	{
		// 球心-起始点
		TVector3<T> distance = center - orig;
		// 距离
		T dis = distance.getLength()
		// 标准化方向
		TVector3<T> direction;
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
				(*pInside)=true;
			return true;
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
				(*pInside)=false;
			return true;
		}
		// 如果距离比半径小
		// 起始点在球内部
		if(dis<radius)
		{
			float flen = 0;
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
				(*pInside)=true;
			return true;
		}
		// 如果距离比半径大
		// 起始点在球内部
		// 只有投影>0时才可能相交
		if(dis>radius && dirLen>0.0f)
		{
			float af = dis*dis - dirLen*dirLen;
			float a = sqrt(af);
			// 这个值必须小于等于半径才相交
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
	bool IntersectBallSimple(const TVector3<T>& orig,const TVector3<T>& dir,const TVector3<T>& center,
			const T radius,TVector3<T>* pVRet,T* len,bool* pInside)
	{
		// 球心-起始点
		TVector3<T> distance = center - orig;
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
				(*pInside)=true;
			return true;
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
			float af = dis*dis - dirLen*dirLen;
			float a = sqrt(af);
			// 这个值必须小于等于半径才相交
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



	// AABB与射线的检测
	template<typename T>
	bool IntersectAABBSimple(const TVector3<T> &orig, const TVector3<T> &dir, const TVector3<T> &min, 
									const TVector3<T> &max, TVector3<T> *pVRet, T *len, bool *pInside)
	{
		bool inside = true;	// 假设在内部
		float xt=-1,yt=-1,zt=-1;	// 三个坐标的差,初始化为-1
		// 标准化方向
		TVector3<T> direction;
		direction = dir.normalize();

		// 分别比较三个坐标轴,计算出插值
		if(orig.X < min.X)
		{
			xt = min.X - orig.X;	// 计算差
			xt /= direction.X;			// 和方向位置的比值(是方向位置的倍数)
			inside = false;			// X轴不在内部
		}
		else if(orig.X > max.X)
		{
			xt = max.X - orig.X;
			xt /= direction.X;
			inside = false;
		}
		if(orig.Y < min.Y)
		{
			yt = min.Y - orig.Y;	// 计算差
			yt /= direction.Y;			// 和方向位置的比值(是方向位置的倍数)
			inside = false;			// X轴不在内部
		}
		else if(orig.Y > max.Y)
		{
			yt = max.Y - orig.Y;
			yt /= direction.Y;
			inside = false;
		}
		if(orig.Z < min.Z)
		{
			zt = min.Z - orig.Z;	// 计算差
			zt /= direction.Z;			// 和方向位置的比值(是方向位置的倍数)
			inside = false;			// X轴不在内部
		}
		else if(orig.Z > max.Z)
		{
			zt = max.Z - orig.Z;
			zt /= direction.Z;
			inside = false;
		}
		// 如果在内部
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
		// 选择适当的相交平面
		// 那个轴上的差的比值最大,说明和另外两个轴组成的平面先相交
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
		// 根据相交平面,判断是否在盒内
		bool result = true;	// 最后结果
		float x=0,y=0,z=0;	// 碰撞点坐标
		switch(which)
		{
		case 0:	// YZ平面
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
		case 1:	// XZ平面
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
		case 2:	// XY平面
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
				(*pVRet)=TVector3<T>(x,y,z);
			if(len)
				(*len)=(T)t;
			if(pInside)
				(*pInside)=false;
		}
		return result;
	}

	// 计算切线方向，用于法线贴图
	template<typename T>
	void CalculateTangentArray(long vertexCount, const TVector3<T> *vertex, const TVector3<T> *normal,
		const TVector2<T> *texcoord, long triangleCount, const long* indexArray, TVector4<T> *tangent)
	{
		TVector3<T> *tan1 = new TVector3<T>[vertexCount * 2];
		TVector3<T> *tan2 = tan1 + vertexCount;
		//::ZeroMemory(tan1, vertexCount * sizeof(Ogre::Vector3) * 2);
		memset(tan1,0,vertexCount * sizeof(TVector3<T>) * 2);
    
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
			TVector3<T> sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
					(t2 * z1 - t1 * z2) * r);
			TVector3<T> tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
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
			const TVector3<T>& n = normal[a];
			const TVector3<T>& t = tan1[a];
        
			// Gram-Schmidt orthogonalize
			tangent[a] = (t - n * n.dotProduct(t)).normalisedCopy();
        
			// Calculate handedness
			tangent[a].w = (n.crossProduct(t).dotProduct(tan2[a]) < 0.0F) ? -1.0F : 1.0F;
		}
    
		delete[] tan1;
	}
}