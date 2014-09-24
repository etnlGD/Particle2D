#pragma once
#include "vector3d.h"

namespace math3d
{
	class plane
	{
	public:
		enum IntersectionRelation
		{
			FRONT, BACK, PLANNAR, 
		};

		plane() : normal(0, 1, 0), distance(0) { }
		plane(const plane& other);
		plane(float a, float b, float c, float d);
		plane(const vector3df& normal, float distance);
		plane(const vector3df& normal, const vector3df& point);
		plane(const vector3df& point1, const vector3df& point2, const vector3df& point3);

		~plane();

		bool operator==(const plane& other) const;
		bool operator!=(const plane& other) const;
		plane& operator=(const plane& other);

		plane& set(float a, float b, float c, float d);
		plane& set(const vector3df& normal, float distance);
		plane& set(const vector3df& normal, const vector3df& point);
		plane& set(const vector3df& point1, const vector3df& point2, const vector3df& point3);		 

		bool getIntersection(const plane& other, vector3df* outLinePoint = 0, vector3df* outLineVec = 0)
		{
			float cos = normal.dot(other.normal);

			if (cos == 1.0f || cos == -1.0f) return false;

			// if exists intersection.
			if (outLineVec != 0)
			{
				*outLineVec = normal.cross(other.normal);
			}

			if (outLinePoint != 0)
			{				
				float sin2 = 1 - cos*cos;
				const float invsin2 = 1.0 / sin2;
				const float fc0 = (-this->distance + cos*other.distance) * invsin2;
				const float fc1 = (-other.distance + cos*this->distance) * invsin2;

				*outLinePoint = normal*fc0 + other.normal*fc1;
			}
			
		}
	private:
		vector3df normal;	// normal of this plane.
		float distance;		// from plane to origin. The sign of distance is positive if normal is pointing to origin, otherwise negative.
	};

}