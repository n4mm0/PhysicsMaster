#pragma once
#include "ColliderUtil.h"
#include "Collision.h"
#include "Quaternion.h"
#include "Constants.h"
#include "GameObject.h"
//debug
#include <iostream>
#include <algorithm>

namespace CollisionAlgorithm
{

	template<class T, class U> class CollisionDetectionAlgorithm;

	//Dummy needed for automatic
	template<> 
	class	CollisionDetectionAlgorithm<PlaneCollider,PlaneCollider> 
	{
	public:
		
		template<typename T, typename U>
		static int Fire(Collider& first, Collider& second)
		{
			return CollisionComputation(static_cast<T*>(&first), static_cast<U*>(&second));
		}
	private:
		static int CollisionComputation(PlaneCollider* first, PlaneCollider* second)
		{
			return 0;
		}
	};

	template<>
	class CollisionDetectionAlgorithm<BoxCollider, BoxCollider>
	{
	public:
		template<typename T, typename U>
		static int Fire(Collider& first, Collider& second)
		{
			return CollisionComputation(static_cast<T*>(&first), static_cast<U*>(&second));
		}

	private:
		
		static int CollisionComputation(BoxCollider* first, BoxCollider* second)
		{
			//Compute Collision on first axis system so first can be seen as an AABB
			
			const Vector3& FirstCenter=first->GetWorldPosition();
			const Vector3& SecondCenter=second->GetWorldPosition();
			Vector3 CentersDistance;
			CentersDistance = FirstCenter - SecondCenter;
			
			
			Vector3 radiiSum(first->GetHalfSize());
			radiiSum += second->GetHalfSize();

			//SphereTest for early fast reject
			bool SphereConsideration = (CentersDistance.getX()*CentersDistance.getX()) < (radiiSum.getX()*radiiSum.getX()) &&
				(CentersDistance.getY()*CentersDistance.getY()) < (radiiSum.getY()*radiiSum.getY()) &&
				(CentersDistance.getZ()*CentersDistance.getZ()) < (radiiSum.getZ()*radiiSum.getZ()) && (CentersDistance.sqrMagnitude()) < (radiiSum.sqrMagnitude());
			
			if (!SphereConsideration)
			{
				return 0;
			}

			//Compute Min and Max for first box
			Vector3 Min(FirstCenter - first->GetHalfSize());
			Vector3 Max(FirstCenter + first->GetHalfSize());
			
			//second box in first obj space
			Vector3 CenterInOtherSystem(SecondCenter);
			Vector3 Vertex[8];
			
			first->GetVertex(Vertex);
			for (int i = 0; i < 8; ++i)
			{
				QuaternionRotate(second->GetRotation(), Vertex[i], Vertex[i]);
				Vertex[i] += SecondCenter;
				QuaternionRotateT(first->GetRotation(), Vertex[i], Vertex[i]);			
			}
			
			//Compute points inside
			int indexes[8];
			float compenetration[8];
			unsigned int pointsInside = 0;
			for (unsigned int i = 0; i < 8; ++i)
			{
				if (Vertex[i][0] >= Min[0] && Vertex[i][1] >= Min[1] && Vertex[i][2] >= Min[2] &&
					Vertex[i][0] <= Max[0] && Vertex[i][1] <= Max[1] && Vertex[i][2] <= Max[2])
				{
					indexes[pointsInside] = i;
					compenetration[pointsInside] = (Vertex[i] - FirstCenter).sqrMagnitude();
					++pointsInside;
				}
			}

			//Compute Collision Data iff there is a collision
			if (pointsInside)
			{
				float CompenetrationSum = compenetration[0];
				Vertex[indexes[0]][0] *= compenetration[0];
				Vertex[indexes[0]][1] *= compenetration[0];
				Vertex[indexes[0]][2] *= compenetration[0];
				//centroid of points which is the point of collision impact
				for (unsigned int i = 1; i < pointsInside; ++i)
				{
					Vertex[indexes[i]] *= compenetration[i];
					Vertex[indexes[0]][0] += Vertex[indexes[i]][0];
					Vertex[indexes[0]][1] += Vertex[indexes[i]][1];
					Vertex[indexes[0]][2] += Vertex[indexes[i]][2];
					CompenetrationSum += compenetration[i];
					compenetration[0] = std::fmaxf(compenetration[0], compenetration[i]);
				}
				Vertex[indexes[0]][0] /= CompenetrationSum;
				Vertex[indexes[0]][1] /= CompenetrationSum;
				Vertex[indexes[0]][2] /= CompenetrationSum;

				CentersDistance.normalize();

				Collision& collision = Constants::CollisionsCollection::GetSingleton().EditCollision();
				collision.Init(compenetration[0], Vertex[indexes[0]], CentersDistance, first->EditOwner()->EditChild<RigidBody>(), second->EditOwner()->EditChild<RigidBody>());
				return 1;
			}
		
			//First Against Second
			Min = SecondCenter - second->GetHalfSize();
			Max = SecondCenter + second->GetHalfSize();
			
			CenterInOtherSystem = FirstCenter;

			second->GetVertex(Vertex);

			for (int i = 0; i < 8; ++i)
			{
				QuaternionRotate(first->GetRotation(), Vertex[i], Vertex[i]);
				Vertex[i] += FirstCenter;
				QuaternionRotateT(second->GetRotation(), Vertex[i], Vertex[i]);
			}
			
			pointsInside = 0;
			for (unsigned int i = 0; i < 8; ++i)
			{
				if (Vertex[i][0] >= Min[0] && Vertex[i][1] >= Min[1] && Vertex[i][2] >= Min[2] &&
					Vertex[i][0] <= Max[0] && Vertex[i][1] <= Max[1] && Vertex[i][2] <= Max[2])
				{
					indexes[pointsInside] = i;
					compenetration[pointsInside] = (Vertex[i] - second->GetWorldPosition()).sqrMagnitude();
					++pointsInside;
				}
			}

			//Compute Collision Data iff there is a collision
			if (pointsInside)
			{
				float CompenetrationSum = compenetration[0];
				Vertex[indexes[0]][0] *= compenetration[0];
				Vertex[indexes[0]][1] *= compenetration[0];
				Vertex[indexes[0]][2] *= compenetration[0];
				//centroid of points which is the point of collision impact
				for (unsigned int i = 1; i < pointsInside; ++i)
				{
					Vertex[indexes[0]][0] += Vertex[indexes[i]][0];
					Vertex[indexes[0]][1] += Vertex[indexes[i]][1];
					Vertex[indexes[0]][2] += Vertex[indexes[i]][2];
					CompenetrationSum += compenetration[i];
					compenetration[0] = std::fmaxf(compenetration[0], compenetration[i]);
				}
				Vertex[indexes[0]][0] /= CompenetrationSum;
				Vertex[indexes[0]][1] /= CompenetrationSum;
				Vertex[indexes[0]][2] /= CompenetrationSum;

				CentersDistance *= -1.0f;
				CentersDistance.normalize();

				//Compute Compenetration
				Collision& collision=Constants::CollisionsCollection::GetSingleton().EditCollision();
				collision.Init(compenetration[0], Vertex[indexes[0]], CentersDistance, second->EditOwner()->EditChild<RigidBody>(), first->EditOwner()->EditChild<RigidBody>());
				return 1;
			}
			return 0;
		};
	};

	template<>
	class CollisionDetectionAlgorithm<SphereCollider, SphereCollider>
	{
	public:
		template<typename T, typename U>
		static int Fire(Collider& first, Collider& second)
		{
			return CollisionComputation(static_cast<T*>(&first), static_cast<U*>(&second));
		}

	private:
		
		static int CollisionComputation(SphereCollider* first, SphereCollider* second)
		{
			Vector3 CentersDistance = first->GetWorldPosition() -  second->GetWorldPosition();
			float distance = CentersDistance.dot(CentersDistance);
			float radiiSum = first->GetRadius() + second->GetRadius();
			if (distance < radiiSum*radiiSum)
			{
				float compenetration = radiiSum - sqrtf(distance);
				CentersDistance.normalize();
				Collision& collision=Constants::CollisionsCollection::GetSingleton().EditCollision();
				collision.Init(compenetration, (first->GetWorldPosition()  + second->GetWorldPosition() )* 0.5f, CentersDistance,first->EditOwner()->EditChild<RigidBody>(), second->EditOwner()->EditChild<RigidBody>());
				return 1;
			}
			return 0;
		};
	};

	template<>
	class CollisionDetectionAlgorithm<BoxCollider, SphereCollider>
	{
	public:
		template<typename T, typename U>
		static int Fire(Collider& first, Collider& second)
		{
			return CollisionComputation(static_cast<T*>(&first), static_cast<U*>(&second));
		};
		//for symmetry sake
		template<>
		static int Fire<SphereCollider, BoxCollider>(Collider& first, Collider& second)
		{
			return CollisionComputation(static_cast<BoxCollider*>(&second), static_cast<SphereCollider*>(&first));
		};
	private:
		static int CollisionComputation(BoxCollider* box, SphereCollider* sphere)
		{
			Vector3 CenterDiff(box->GetWorldPosition() - sphere->GetWorldPosition());
			Vector3 Normal(CenterDiff);
			Normal.normalize();
			float Distance = CenterDiff.dot(Normal);
			CenterDiff = Normal;
			Distance -= sphere->GetRadius();
			QuaternionRotateT(box->GetRotation(), Normal, Normal);
			const Vector3& HalfSize = box->GetHalfSize();
			float Radius = abs(HalfSize[0] * Normal[0]) + abs(HalfSize[1] * Normal[1]) + abs(HalfSize[2] * Normal[2]);
			if (abs(Distance)<Radius)
			{
				Distance = Radius - abs(Distance);
				Vector3 CollisionPoint;
				for (int i = 0; i < 3; ++i)
				{
					if (abs(Normal[i])>0.01f)
						CollisionPoint[i] = HalfSize[i] * !signbit(Normal[i]) - HalfSize[i] * signbit(Normal[i]);
					else
						CollisionPoint[i] = 0;
				}
				QuaternionRotate(box->GetRotation(), CollisionPoint, CollisionPoint);
				CollisionPoint += box->GetWorldPosition();
				Collision& collision = Constants::CollisionsCollection::GetSingleton().EditCollision();

				collision.Init(Distance, CollisionPoint, CenterDiff, box->EditOwner()->EditChild<RigidBody>(), sphere->EditOwner()->EditChild<RigidBody>());
				return 1;
			}
			return 0;
		};
	};

	template<>
	class CollisionDetectionAlgorithm<BoxCollider, PlaneCollider>
	{
	public:
		template<typename T, typename U>
		static int Fire(Collider& first, Collider& second)
		{
			return CollisionComputation(static_cast<T*>(&first), static_cast<U*>(&second));
		}
		//for symmetry sake
		template<>
		static int Fire<PlaneCollider, BoxCollider>(Collider& first, Collider& second)
		{
			
			return CollisionComputation(static_cast<BoxCollider*>(&second), static_cast<PlaneCollider*>(&first));
		}
	private:
		static int CollisionComputation(BoxCollider* first, PlaneCollider* second)
		{
			Vector3 PlaneNormal(second->GetPlaneNormal());		
			float Distance = (first->GetWorldPosition()-second->GetWorldPosition()).dot(PlaneNormal);
			QuaternionRotate(first->GetRotation(), PlaneNormal, PlaneNormal);
			const Vector3& HalfSize = first->GetHalfSize();
			float Radius = abs(HalfSize[0] * PlaneNormal[0]) + abs(HalfSize[1] * PlaneNormal[1]) + abs(HalfSize[2] * PlaneNormal[2]);
			if (abs(Distance)<Radius)
			{
				Distance = Radius - sqrt(abs(Distance));
				Vector3 CollisionPoint;

				for (int i = 0; i < 3; ++i)
				{
					if (abs(PlaneNormal[i])>0.01f)
						CollisionPoint[i] = HalfSize[i] * !signbit(PlaneNormal[i]) - HalfSize[i] * signbit(PlaneNormal[i]);
					else
						CollisionPoint[i] = 0;
				}

				QuaternionRotate(first->GetRotation(), CollisionPoint, CollisionPoint);
				CollisionPoint += first->GetWorldPosition();
				Collision& collision = Constants::CollisionsCollection::GetSingleton().EditCollision();
				collision.Init(Distance, CollisionPoint, second->GetPlaneNormal(), first->EditOwner()->EditChild<RigidBody>(), second->EditOwner()->EditChild<RigidBody>());
				return 1;
			}
			return 0;
		};
	};

	template<>
	class CollisionDetectionAlgorithm<SphereCollider, PlaneCollider>
	{
	public:
		template<typename T, typename U>
		static int Fire(Collider& first, Collider& second)
		{
			return CollisionComputation(static_cast<T*>(&first), static_cast<U*>(&second));
		}
		//for symmetry sake
		template<>
		static int Fire<PlaneCollider,SphereCollider>(Collider& first, Collider& second)
		{
			return CollisionComputation(static_cast<SphereCollider*>(&second), static_cast<PlaneCollider*>(&first));
		}
	private:
		static int CollisionComputation(SphereCollider* sphere ,PlaneCollider* plane)
		{
			const Vector3& planeNormal = plane->GetPlaneNormal();
			const Vector3& SphereCenter = sphere->GetWorldPosition();
			Vector3 d(SphereCenter - plane->GetWorldPosition());
			float distance = d.dot(planeNormal);
			if (distance< sphere->GetRadius()*sphere->GetRadius()){
				distance = sphere->GetRadius() - sqrt(distance);
				Collision& collision = Constants::CollisionsCollection::GetSingleton().EditCollision();
				collision.Init(distance, SphereCenter - planeNormal*sphere->GetRadius(), planeNormal, sphere->EditOwner()->EditChild<RigidBody>(), plane->EditOwner()->EditChild<RigidBody>());
				return 1;
			}
			return 0;
		};
	};
}