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
			int i = CollisionComputation(static_cast<T*>(&first), static_cast<U*>(&second));
			std::cout << "box - box " << i << std::endl;
			return i;
		}
	private:
		
		static int CollisionComputation(BoxCollider* first, BoxCollider* second)
		{
			//Compute Collision on first axis system so first can be seen as an AABB
			
			const Vector3& FirstCenter=first->GetWorldPosition();
			const Vector3& SecondCenter=second->GetWorldPosition();
			Vector3 CentersDistance;
			CentersDistance = FirstCenter - SecondCenter;
			
			/*
			Vector3 radiiSum(first->GetHalfSize());
			radiiSum += second->GetHalfSize();

			//SphereTest for early fast reject
			bool SphereConsideration = (CentersDistance.getX()*CentersDistance.getX()) < (radiiSum.getX()*radiiSum.getX()) &&
				(CentersDistance.getY()*CentersDistance.getY()) < (radiiSum.getY()*radiiSum.getY()) &&
				(CentersDistance.getZ()*CentersDistance.getZ()) < (radiiSum.getZ()*radiiSum.getZ()) && (CentersDistance.sqrMagnitude()) < (radiiSum.sqrMagnitude());
			
			if (!SphereConsideration)
			{
				std::cout << "SphereRejection" << std::endl;
				return 0;
			}*/

			//Compute Min and Max for first box
			Vector3 Min(FirstCenter - first->GetHalfSize());
			Vector3 Max(FirstCenter + first->GetHalfSize());
			
			//second box in first obj space
			Vector3 CenterInOtherSystem(SecondCenter);
			Vector3 HalfSizeInOtherSystem(second->GetHalfSize());
		
			QuaternionRotate(first->GetRotation(), CenterInOtherSystem, CenterInOtherSystem);
			QuaternionRotate(second->GetRotation(), HalfSizeInOtherSystem, HalfSizeInOtherSystem);
			QuaternionRotate(first->GetRotation(), HalfSizeInOtherSystem, HalfSizeInOtherSystem);

			//Second vs first
			//Optimization TO DO
			//0 1 3 7 6 4 5 2
			Vector3 Vertex[8];
			Vertex[0] = (CenterInOtherSystem);
			Vertex[0][0] += HalfSizeInOtherSystem[0];
			Vertex[0][1] += HalfSizeInOtherSystem[1];
			Vertex[0][2] += HalfSizeInOtherSystem[2];

			Vertex[1] = (CenterInOtherSystem);
			Vertex[1][0] += HalfSizeInOtherSystem[0];
			Vertex[1][1] += HalfSizeInOtherSystem[1];
			Vertex[1][2] -= HalfSizeInOtherSystem[2];

			Vertex[2] = (CenterInOtherSystem);
			Vertex[2][0] += HalfSizeInOtherSystem[0];
			Vertex[2][1] -= HalfSizeInOtherSystem[1];
			Vertex[2][2] += HalfSizeInOtherSystem[2];
			
			Vertex[3] = (CenterInOtherSystem);
			Vertex[3][0] += HalfSizeInOtherSystem[0];
			Vertex[3][1] -= HalfSizeInOtherSystem[1];
			Vertex[3][2] -= HalfSizeInOtherSystem[2];

			Vertex[4] = (CenterInOtherSystem);
			Vertex[4][0] -= HalfSizeInOtherSystem[0];
			Vertex[4][1] += HalfSizeInOtherSystem[1];
			Vertex[4][2] += HalfSizeInOtherSystem[2];

			Vertex[5] = (CenterInOtherSystem);
			Vertex[5][0] -= HalfSizeInOtherSystem[0];
			Vertex[5][1] += HalfSizeInOtherSystem[1];
			Vertex[5][2] -= HalfSizeInOtherSystem[2];
		
			Vertex[6] = (CenterInOtherSystem);
			Vertex[6][0] -= HalfSizeInOtherSystem[0];
			Vertex[6][1] -= HalfSizeInOtherSystem[1];
			Vertex[6][2] += HalfSizeInOtherSystem[2];

			Vertex[7] = (CenterInOtherSystem);
			Vertex[7][0] -= HalfSizeInOtherSystem[0];
			Vertex[7][1] -= HalfSizeInOtherSystem[1];
			Vertex[7][2] -= HalfSizeInOtherSystem[2];
			
			//Compute points inside
			int indexes[8];
			float compenetration[8];
			unsigned int pointsInside = 0;
			for (unsigned int i = 0; i < 8; ++i)
			{
	
				if (Vertex[i][0] > Min[0] && Vertex[i][1] > Min[1] && Vertex[i][2] > Min[2] &&
					Vertex[i][0] < Max[0] && Vertex[i][1] < Max[1] && Vertex[i][2] < Max[2])
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

				//Compute normal by clamping on max value
				//function to do
				//Clamp(CentersDistance);
				CentersDistance.normalize();
				/*float max = CentersDistance[0];
				max = abs(max)>CentersDistance[1] ? max : CentersDistance[1];
				max = abs(max)>CentersDistance[2] ? max : CentersDistance[2];
	
				CentersDistance[0] = CentersDistance[0] == max ? max : 0;
				CentersDistance[1] = CentersDistance[1] == max ? max : 0;
				CentersDistance[2] = CentersDistance[2] == max ? max : 0;*/
				CentersDistance.clamp(Vector3::clamping::MAX);
				CentersDistance.normalize();
				std::cout << "Box Collision" << std::endl;
				Collision& collision = Constants::CollisionsCollection::GetSingleton().EditCollision();
				collision.Init(compenetration[0], Vertex[indexes[0]], CentersDistance);
				collision.SetBodies(first->EditOwner()->EditChild<RigidBody>(), second->EditOwner()->EditChild<RigidBody>());
				return 1;
			}
		
			//First Against Second
			//TO DO
			
			Min = SecondCenter - second->GetHalfSize();
			Max = SecondCenter - second->GetHalfSize();
			
			CenterInOtherSystem = FirstCenter;
			HalfSizeInOtherSystem = first->GetHalfSize();

			QuaternionRotate(second->GetRotation(), CenterInOtherSystem, CenterInOtherSystem);
			QuaternionRotate(first->GetRotation(), HalfSizeInOtherSystem, HalfSizeInOtherSystem);
			QuaternionRotate(second->GetRotation(), HalfSizeInOtherSystem, HalfSizeInOtherSystem);

			//TO DO
			//MatrixOp::Rotate<MatrixOp::ToWorldSpace>(first->GetRotation().ToMatrix(), SecondHalfSizeInFistSystem, SecondHalfSizeInFistSystem);
			//MatrixOp::Rotate<MatrixOp::ToObjSpace>(second->GetRotation().ToMatrix(), SecondHalfSizeInFistSystem, SecondHalfSizeInFistSystem);

			//first vs Second
			Vertex[0] = (CenterInOtherSystem);
			Vertex[0][0] += HalfSizeInOtherSystem[0];
			Vertex[0][1] += HalfSizeInOtherSystem[1];
			Vertex[0][2] += HalfSizeInOtherSystem[2];

			Vertex[1] = (CenterInOtherSystem);
			Vertex[1][0] += HalfSizeInOtherSystem[0];
			Vertex[1][1] += HalfSizeInOtherSystem[1];
			Vertex[1][2] -= HalfSizeInOtherSystem[2];

			Vertex[2] = (CenterInOtherSystem);
			Vertex[2][0] += HalfSizeInOtherSystem[0];
			Vertex[2][1] -= HalfSizeInOtherSystem[1];
			Vertex[2][2] += HalfSizeInOtherSystem[2];

			Vertex[3] = (CenterInOtherSystem);
			Vertex[3][0] += HalfSizeInOtherSystem[0];
			Vertex[3][1] -= HalfSizeInOtherSystem[1];
			Vertex[3][2] -= HalfSizeInOtherSystem[2];

			Vertex[4] = (CenterInOtherSystem);
			Vertex[4][0] -= HalfSizeInOtherSystem[0];
			Vertex[4][1] += HalfSizeInOtherSystem[1];
			Vertex[4][2] += HalfSizeInOtherSystem[2];

			Vertex[5] = (CenterInOtherSystem);
			Vertex[5][0] -= HalfSizeInOtherSystem[0];
			Vertex[5][1] += HalfSizeInOtherSystem[1];
			Vertex[5][2] -= HalfSizeInOtherSystem[2];

			Vertex[6] = (CenterInOtherSystem);
			Vertex[6][0] -= HalfSizeInOtherSystem[0];
			Vertex[6][1] -= HalfSizeInOtherSystem[1];
			Vertex[6][2] += HalfSizeInOtherSystem[2];

			Vertex[7] = (CenterInOtherSystem);
			Vertex[7][0] -= HalfSizeInOtherSystem[0];
			Vertex[7][1] -= HalfSizeInOtherSystem[1];
			Vertex[7][2] -= HalfSizeInOtherSystem[2];

			
			pointsInside = 0;
			for (unsigned int i = 0; i < 8; ++i)
			{
				if (Vertex[i][0] > Min[0] && Vertex[i][1] > Min[1] && Vertex[i][2] > Min[2] &&
					Vertex[i][0] < Max[0] && Vertex[i][1] < Max[1] && Vertex[i][2] < Max[2])
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

				//Compute normal by clamping on max value
				//function to do
				//Clamp(CentersDistance);
				CentersDistance.normalize();
				/*float max = CentersDistance[0];
				max = abs(max)>CentersDistance[1] ? max : CentersDistance[1];
				max = abs(max)>CentersDistance[2] ? max : CentersDistance[2];

				CentersDistance[0] = CentersDistance[0] == max ? max : 0;
				CentersDistance[1] = CentersDistance[1] == max ? max : 0;
				CentersDistance[2] = CentersDistance[2] == max ? max : 0;*/
				CentersDistance.clamp(Vector3::clamping::MAX);
				CentersDistance.normalize();
				std::cout << "Box Collision" << std::endl;

				//Compute Compenetration
				Collision& collision=Constants::CollisionsCollection::GetSingleton().EditCollision();
				collision.Init(-1.0f*compenetration[0], Vertex[indexes[0]], CentersDistance*-1.0f);
				collision.SetBodies(first->EditOwner()->EditChild<RigidBody>(), second->EditOwner()->EditChild<RigidBody>());
				return 1;
			}
			//If i'm here FUCK THAT
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
				std::cout << "Sphere-Sphere Detected" << std::endl;
				Collision& collision=Constants::CollisionsCollection::GetSingleton().EditCollision();
				collision.Init(compenetration, first->GetWorldPosition()/2.0f + second->GetWorldPosition()/2.0f, CentersDistance);
				collision.SetBodies(first->EditOwner()->EditChild<RigidBody>(), second->EditOwner()->EditChild<RigidBody>());
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
			/*
			Vector3 SphereCenterInBoxSystem(sphere->GetWorldPosition());
			Vector3 closestPoint(box->GetWorldPosition());

			QuaternionRotate(box->GetRotation(), SphereCenterInBoxSystem, SphereCenterInBoxSystem);
			Vector3 Min(closestPoint - box->GetHalfSize());
			Vector3 Max(closestPoint + box->GetHalfSize());
			//std::cout << "ScB: (" << SphereCenterInBoxSystem[0] << ", " << SphereCenterInBoxSystem[1] << ", " << SphereCenterInBoxSystem[2] << ")" << std::endl;
			
			for (int i = 0; i < 3; ++i)
			{
				if (SphereCenterInBoxSystem[i] <= Min[i])
					closestPoint[i] -= 1.0f;
				else
				{
					if (SphereCenterInBoxSystem[i] >= Max[i])
						closestPoint[i] += 1.0f;
				}
			}
			//std::cout << "Cp: (" << closestPoint[0] << ", " << closestPoint[1] << ", " << closestPoint[2] << ")" << std::endl;
			//std::cout << "Min: (" << Min[0] << ", " << Min[1] << ", " << Min[2] << ")" << std::endl;
			//std::cout << "Max: (" << Max[0] << ", " << Max[1] << ", " << Max[2] << ")" << std::endl;

		//	closestPoint[0] = ((Min.getX() - SphereCenterInBoxSystem.getX()> 0) ? Min.getX() - SphereCenterInBoxSystem.getX() : 0);
		//	closestPoint[1] = ((Min.getY() - SphereCenterInBoxSystem.getY()> 0) ? Min.getY() - SphereCenterInBoxSystem.getY() : 0);
		//	closestPoint[2] = ((Min.getZ() - SphereCenterInBoxSystem.getZ()> 0) ? Min.getZ() - SphereCenterInBoxSystem.getZ() : 0);
		//	closestPoint[0] += ((SphereCenterInBoxSystem.getX() - Max.getX() > 0) ? SphereCenterInBoxSystem.getX() - Max.getX() : 0);
		//	closestPoint[1] += ((SphereCenterInBoxSystem.getY() - Max.getY() > 0) ? SphereCenterInBoxSystem.getY() - Max.getY() : 0);
		//	closestPoint[2] += ((SphereCenterInBoxSystem.getZ() - Max.getZ() > 0) ? SphereCenterInBoxSystem.getZ() - Max.getZ() : 0);
	
			closestPoint -= SphereCenterInBoxSystem;
			float distance = closestPoint.sqrMagnitude();
			//std::cout << "d^2: " << distance << std::endl;
			if (distance < sphere->GetRadius()*sphere->GetRadius())
			{
				std::cout << "Sphere - Box" << std::endl;
				system("pause");
				Vector3 CentersDistance = box->GetWorldPosition() - SphereCenterInBoxSystem;
				CentersDistance.normalize();
				Collision& collision=Constants::CollisionsCollection::GetSingleton().EditCollision();

				collision.Init(sphere->GetRadius() - sqrt(distance), CentersDistance*sphere->GetRadius(), CentersDistance);
				collision.SetBodies(box->EditOwner()->EditChild<RigidBody>(), sphere->EditOwner()->EditChild<RigidBody>());
				return 1;
			}
			*/
			///*
			Vector3 CenterDiff(box->GetWorldPosition() - sphere->GetWorldPosition());
			Vector3 Normal(CenterDiff);
			Normal.normalize();
			float Distance = CenterDiff.dot(Normal);
			CenterDiff = Normal;
			Distance -= sphere->GetRadius();
		//	std::cout << "NormalB: (" << Normal[0] << ", " << Normal[1] << ", " << Normal[2] << ")" << std::endl;
		//	std::cout << "CenterDiff: (" << CenterDiff[0] << ", " << CenterDiff[1] << ", " << CenterDiff[2]<<")" << std::endl;
			QuaternionRotate(box->GetRotation(), Normal, Normal);
			const Vector3& HalfSize = box->GetHalfSize();
			float Radius = abs(HalfSize[0] * Normal[0]) + abs(HalfSize[1] * Normal[1]) + abs(HalfSize[2] * Normal[2]);
		//	std::cout << "distance: " << Distance << std::endl;
		//	std::cout << "Radius: " << Radius << std::endl;
			if (abs(Distance)<Radius)
			{
				Distance = Radius - abs(Distance);
				Vector3 CollisionPoint;
				//TO DO NEED to clamp if value diff is minor than threshold

				//	std::cout << "PlaneNormal: (";
				for (int i = 0; i < 3; ++i)
				{
					//	std::cout << PlaneNormal[i] << ", ";
					if (abs(Normal[i])>0.01f)
						CollisionPoint[i] = HalfSize[i] * !signbit(Normal[i]) - HalfSize[i] * signbit(Normal[i]);
					else
						CollisionPoint[i] = 0;
				}
			//	std::cout << "BSNormal: (" << Normal[0] << ", " << Normal[1] << ", " << Normal[2] << ")" << std::endl;
				QuaternionRotate(box->GetRotation(), CollisionPoint, CollisionPoint);
				CollisionPoint += box->GetWorldPosition();
				Collision& collision = Constants::CollisionsCollection::GetSingleton().EditCollision();

				collision.Init(Distance*1.50f, CollisionPoint,CenterDiff );
				collision.SetBodies(box->EditOwner()->EditChild<RigidBody>(), sphere->EditOwner()->EditChild<RigidBody>());
				return 1;
			}
			//*/
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

				//TO DO NEED to clamp if value diff is minor than threshold
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
				collision.Init(Distance*100.0f, CollisionPoint, second->GetPlaneNormal());
				collision.SetBodies(first->EditOwner()->EditChild<RigidBody>(), second->EditOwner()->EditChild<RigidBody>());
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
				collision.Init(distance*100.0f, SphereCenter - planeNormal*sphere->GetRadius(), planeNormal);
				collision.SetBodies(sphere->EditOwner()->EditChild<RigidBody>(), plane->EditOwner()->EditChild<RigidBody>());
				return 1;
			}
			return 0;
		};
	};
}