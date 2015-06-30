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
				float max = CentersDistance[0];
				max = abs(max)>CentersDistance[1] ? max : CentersDistance[1];
				max = abs(max)>CentersDistance[2] ? max : CentersDistance[2];
	
				CentersDistance[0] = CentersDistance[0] == max ? max : 0;
				CentersDistance[1] = CentersDistance[1] == max ? max : 0;
				CentersDistance[2] = CentersDistance[2] == max ? max : 0;
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
				float max = CentersDistance[0];
				max = abs(max)>CentersDistance[1] ? max : CentersDistance[1];
				max = abs(max)>CentersDistance[2] ? max : CentersDistance[2];

				CentersDistance[0] = CentersDistance[0] == max ? max : 0;
				CentersDistance[1] = CentersDistance[1] == max ? max : 0;
				CentersDistance[2] = CentersDistance[2] == max ? max : 0;
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
				Collision& collision=Constants::CollisionsCollection::GetSingleton().EditCollision();
				collision.Init(compenetration, CentersDistance*first->GetRadius(), CentersDistance);
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
		static int CollisionComputation(BoxCollider* first, SphereCollider* second)
		{
	
			Vector3 SphereCenterInBoxSystem(second->GetWorldPosition());
			QuaternionRotate(first->GetRotation(), SphereCenterInBoxSystem, SphereCenterInBoxSystem);
			Vector3 Min(first->GetWorldPosition() - first->GetHalfSize());
			Vector3 Max(first->GetWorldPosition() + first->GetHalfSize());

			Vector3 closestPoint;
			
			closestPoint[0] = ((Min.getX() - SphereCenterInBoxSystem.getX()> 0) ? Min.getX() - SphereCenterInBoxSystem.getX() : 0);
			closestPoint[1] = ((Min.getY() - SphereCenterInBoxSystem.getY()> 0) ? Min.getY() - SphereCenterInBoxSystem.getY() : 0);
			closestPoint[2] = ((Min.getZ() - SphereCenterInBoxSystem.getZ()> 0) ? Min.getZ() - SphereCenterInBoxSystem.getZ() : 0);
			closestPoint[0] += ((SphereCenterInBoxSystem.getX() - Max.getX() > 0) ? SphereCenterInBoxSystem.getX() - Max.getX() : 0);
			closestPoint[1] += ((SphereCenterInBoxSystem.getY() - Max.getY() > 0) ? SphereCenterInBoxSystem.getY() - Max.getY() : 0);
			closestPoint[2] += ((SphereCenterInBoxSystem.getZ() - Max.getZ() > 0) ? SphereCenterInBoxSystem.getZ() - Max.getZ() : 0);
	
			float distance = closestPoint.dot(closestPoint);

			if (distance < second->GetRadius()*second->GetRadius())
			{
				
				Vector3 CentersDistance = first->GetWorldPosition() - SphereCenterInBoxSystem;
				CentersDistance.normalize();
				Collision& collision=Constants::CollisionsCollection::GetSingleton().EditCollision();

				collision.Init(second->GetRadius() - sqrt(distance), CentersDistance*second->GetRadius(), CentersDistance);
				collision.SetBodies(first->EditOwner()->EditChild<RigidBody>(), second->EditOwner()->EditChild<RigidBody>());
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
				Distance -= Radius;
				Distance *= -1.0f;
				Collision& collision = Constants::CollisionsCollection::GetSingleton().EditCollision();
				collision.Init(Distance, (first->GetWorldPosition() - second->GetPlaneNormal()*Radius), second->GetPlaneNormal());
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
		static int Fire<SphereCollider, PlaneCollider>(Collider& first, Collider& second)
		{
			return CollisionComputation(static_cast<PlaneCollider*>(&second), static_cast<SphereCollider*>(&first));
		}
	private:
		static int CollisionComputation(PlaneCollider* first, SphereCollider* second )
		{
			const Vector3& planeNormal = first->GetPlaneNormal();
			const Vector3& SphereCenter = second->GetWorldPosition();
			Vector3 d(SphereCenter - first->GetWorldPosition());
			float distance = d.dot(planeNormal);;
			if (distance< second->GetRadius()*second->GetRadius()){
				distance = sqrt(distance) - second->GetRadius();
				Collision& collision = Constants::CollisionsCollection::GetSingleton().EditCollision();
				collision.Init(distance, SphereCenter*second->GetRadius(), planeNormal);
				collision.SetBodies(first->EditOwner()->EditChild<RigidBody>(), second->EditOwner()->EditChild<RigidBody>());
				return 1;
			}
			return 0;
		};
	};
}

/*
Box Box SAT for lol sake
Vector3 const& CenterOfFirstBox = first->GetWorldPosition();
Vector3 const* AxisesOfFirstBox[3];
AxisesOfFirstBox[0] = &first->GetAxis(0);
Vector3 const& FirstBoxHalfSize = first->GetHalfSize();
Vector3 const& CenterOfSecondBox = second->GetWorldPosition();
Vector3 const* AxisesOfSecondBox[3];
AxisesOfSecondBox[0] = &second->GetAxis(0);
AxisesOfSecondBox[1] = &second->GetAxis(1);
AxisesOfSecondBox[2] = &second->GetAxis(2);
Vector3 const& SecondBoxHalfSize = second->GetHalfSize();

//Optimization TO DO
//for numerical stability 1-epsilon checking for parallelism
//const float cutoff = 1.0f - 0.00001f;
//bool existsParallelPair = false;

Vector3 CentersDistance(CenterOfSecondBox - CenterOfFirstBox);

float MatrixOfDotProduct[9];
float AbsOfDotProduct[9];
float DotProductCentersAndAxisesOfFirstBox[3];
float r0, r1, r;
float r01;

for (int i = 0; i < 3; ++i)
{
MatrixOfDotProduct[i] = VectorOp::DotProduct(*(AxisesOfFirstBox[0]), *(AxisesOfSecondBox[i]));
AbsOfDotProduct[i] = std::abs(MatrixOfDotProduct[i]);
//	existsParallelPair = AbsOfDotProduct[i] > cutoff;
}
DotProductCentersAndAxisesOfFirstBox[0] = VectorOp::DotProduct(CentersDistance, *(AxisesOfFirstBox[0]));
r = std::abs(DotProductCentersAndAxisesOfFirstBox[0]);
r1 = SecondBoxHalfSize.getX() * AbsOfDotProduct[0] + SecondBoxHalfSize.getY() * AbsOfDotProduct[1] + SecondBoxHalfSize.getZ() * AbsOfDotProduct[2];
r01 = FirstBoxHalfSize.getX() + r1;
if (r > r01)
{
return nullptr;
}
AxisesOfFirstBox[1]= &first->GetAxis(1);
for (int i = 0; i < 3; ++i)
{
MatrixOfDotProduct[3 + i] = VectorOp::DotProduct(*(AxisesOfFirstBox[1]), *(AxisesOfSecondBox[i]));
AbsOfDotProduct[3 + i] = std::abs(MatrixOfDotProduct[3 + i]);
//	existsParallelPair = AbsOfDotProduct[3 + i] > cutoff;
}
DotProductCentersAndAxisesOfFirstBox[1] = VectorOp::DotProduct(CentersDistance, *(AxisesOfFirstBox[1]));
r = std::abs(DotProductCentersAndAxisesOfFirstBox[1]);
r1 = SecondBoxHalfSize.getX() * AbsOfDotProduct[3] + SecondBoxHalfSize.getY() * AbsOfDotProduct[4] + SecondBoxHalfSize.getZ() * AbsOfDotProduct[5];
r01 = FirstBoxHalfSize.getY() + r1;
if (r > r01)
{
return nullptr;
}
AxisesOfFirstBox[2]= & first->GetAxis(2);

for (int i = 0; i < 3; ++i)
{
MatrixOfDotProduct[6 + i] = VectorOp::DotProduct(*(AxisesOfFirstBox[2]), *(AxisesOfSecondBox[i]));
AbsOfDotProduct[6 + i] = std::abs(MatrixOfDotProduct[6 + i]);
//	existsParallelPair = AbsOfDotProduct[6 + i] > cutoff;
}
DotProductCentersAndAxisesOfFirstBox[2] = VectorOp::DotProduct(CentersDistance, *(AxisesOfFirstBox[2]));
r = std::abs(DotProductCentersAndAxisesOfFirstBox[2]);
r1 = SecondBoxHalfSize.getX() * AbsOfDotProduct[6] + SecondBoxHalfSize.getY() * AbsOfDotProduct[7] + SecondBoxHalfSize.getZ() * AbsOfDotProduct[8];
r01 = FirstBoxHalfSize.getZ() + r1;
if (r > r01)
{
return nullptr;
}

r = std::abs(VectorOp::DotProduct(CentersDistance, *(AxisesOfSecondBox[0])));
r0 = FirstBoxHalfSize.getX() * AbsOfDotProduct[0] + FirstBoxHalfSize.getY() * AbsOfDotProduct[3] + FirstBoxHalfSize.getZ() * AbsOfDotProduct[6];
r01 = r0 + SecondBoxHalfSize.getX();
if (r > r01)
{
return nullptr;
}

r = std::abs(VectorOp::DotProduct(CentersDistance, *(AxisesOfSecondBox[1])));
r0 = FirstBoxHalfSize.getX() * AbsOfDotProduct[1] + FirstBoxHalfSize.getY() * AbsOfDotProduct[4] + FirstBoxHalfSize.getZ() * AbsOfDotProduct[7];
r01 = r0 + SecondBoxHalfSize.getY();
if (r > r01)
{
return nullptr;
}

r = std::abs(VectorOp::DotProduct(CentersDistance, *(AxisesOfSecondBox[2])));
r0 = FirstBoxHalfSize.getX() * AbsOfDotProduct[2] + FirstBoxHalfSize.getY() * AbsOfDotProduct[5] + FirstBoxHalfSize.getZ() * AbsOfDotProduct[8];
r01 = r0 + SecondBoxHalfSize.getZ();
if (r > r01)
{
return nullptr;
}

r = std::abs(DotProductCentersAndAxisesOfFirstBox[2] * MatrixOfDotProduct[3] - DotProductCentersAndAxisesOfFirstBox[1] * MatrixOfDotProduct[6]);
r0 = FirstBoxHalfSize.getY() * AbsOfDotProduct[6] + FirstBoxHalfSize.getZ() * AbsOfDotProduct[3];
r1 = SecondBoxHalfSize.getY() * AbsOfDotProduct[2] + SecondBoxHalfSize.getZ() * AbsOfDotProduct[1];
r01 = r0 + r1;
if (r > r01)
{
return nullptr;
}

r = std::abs(DotProductCentersAndAxisesOfFirstBox[2] * MatrixOfDotProduct[4] - DotProductCentersAndAxisesOfFirstBox[1] * MatrixOfDotProduct[7]);
r0 = FirstBoxHalfSize.getY() * AbsOfDotProduct[7] + FirstBoxHalfSize.getZ() * AbsOfDotProduct[4];
r1 = SecondBoxHalfSize.getX() * AbsOfDotProduct[2] + SecondBoxHalfSize.getZ() * AbsOfDotProduct[0];
r01 = r0 + r1;
if (r > r01)
{
return nullptr;
}

r = std::abs(DotProductCentersAndAxisesOfFirstBox[2] * MatrixOfDotProduct[5] - DotProductCentersAndAxisesOfFirstBox[1] * MatrixOfDotProduct[8]);
r0 = FirstBoxHalfSize.getY() * AbsOfDotProduct[8] + FirstBoxHalfSize.getZ() * AbsOfDotProduct[5];
r1 = SecondBoxHalfSize.getX() * AbsOfDotProduct[1] + SecondBoxHalfSize.getY() * AbsOfDotProduct[0];
r01 = r0 + r1;
if (r > r01)
{
return nullptr;
}

r = std::abs(DotProductCentersAndAxisesOfFirstBox[0] * MatrixOfDotProduct[6] - DotProductCentersAndAxisesOfFirstBox[2] * MatrixOfDotProduct[0]);
r0 = FirstBoxHalfSize.getX() * AbsOfDotProduct[6] + FirstBoxHalfSize.getZ() * AbsOfDotProduct[0];
r1 = SecondBoxHalfSize.getY() * AbsOfDotProduct[5] + SecondBoxHalfSize.getZ() * AbsOfDotProduct[4];
r01 = r0 + r1;
if (r > r01)
{
return nullptr;
}

r = std::abs(DotProductCentersAndAxisesOfFirstBox[0] * MatrixOfDotProduct[7] - DotProductCentersAndAxisesOfFirstBox[2] * MatrixOfDotProduct[1]);
r0 = FirstBoxHalfSize.getX() * AbsOfDotProduct[7] + FirstBoxHalfSize.getZ() * AbsOfDotProduct[1];
r1 = SecondBoxHalfSize.getX() * AbsOfDotProduct[5] + SecondBoxHalfSize.getZ() * AbsOfDotProduct[3];
r01 = r0 + r1;
if (r > r01)
{
return nullptr;
}

r = std::abs(DotProductCentersAndAxisesOfFirstBox[0] * MatrixOfDotProduct[8] - DotProductCentersAndAxisesOfFirstBox[2] * MatrixOfDotProduct[2]);
r0 = FirstBoxHalfSize.getX() * AbsOfDotProduct[8] + FirstBoxHalfSize.getZ() * AbsOfDotProduct[2];
r1 = SecondBoxHalfSize.getX() * AbsOfDotProduct[4] + SecondBoxHalfSize.getY() * AbsOfDotProduct[3];
r01 = r0 + r1;
if (r > r01)
{
return nullptr;
}

r = std::abs(DotProductCentersAndAxisesOfFirstBox[1] * MatrixOfDotProduct[0] - DotProductCentersAndAxisesOfFirstBox[0] * MatrixOfDotProduct[3]);
r0 = FirstBoxHalfSize.getX() * AbsOfDotProduct[3] + FirstBoxHalfSize.getY() * AbsOfDotProduct[0];
r1 = SecondBoxHalfSize.getY() * AbsOfDotProduct[8] + SecondBoxHalfSize.getZ() * AbsOfDotProduct[7];
r01 = r0 + r1;
if (r > r01)
{
return nullptr;
}

r = std::abs(DotProductCentersAndAxisesOfFirstBox[1] * MatrixOfDotProduct[1] - DotProductCentersAndAxisesOfFirstBox[0] * MatrixOfDotProduct[4]);
r0 = FirstBoxHalfSize.getX() * AbsOfDotProduct[4] + FirstBoxHalfSize.getY() * AbsOfDotProduct[1];
r1 = SecondBoxHalfSize.getX() * AbsOfDotProduct[8] + SecondBoxHalfSize.getZ() * AbsOfDotProduct[6];
r01 = r0 + r1;
if (r > r01)
{
return nullptr;
}

r = std::abs(DotProductCentersAndAxisesOfFirstBox[1] * MatrixOfDotProduct[2] - DotProductCentersAndAxisesOfFirstBox[0] * MatrixOfDotProduct[5]);
r0 = FirstBoxHalfSize.getX() * AbsOfDotProduct[5] + FirstBoxHalfSize.getY() * AbsOfDotProduct[2];
r1 = SecondBoxHalfSize.getX() * AbsOfDotProduct[7] + SecondBoxHalfSize.getY() * AbsOfDotProduct[6];
r01 = r0 + r1;
if (r > r01)
{
return nullptr;
}

return new Collision(???,???,???);
*/