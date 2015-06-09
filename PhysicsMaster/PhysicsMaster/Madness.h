#pragma once
#include "Utilities\TypeList.h"
#include "Collision\CollisionAlgorithms.h"

//CUSTOM FOR COLLISIONDISPATCHER

template<typename Class,typename L1, typename L2>
struct AutomaticInsert
{
	static void Init(Class& d)
	{
		d.Add<L1::Head, L2::Head>(CollisionAlgorithm::CollisionDetectionAlgorithm<L1::Head, L2::Head>::Fire<L1::Head, L2::Head>);
		d.Add<L2::Head, L1::Head>(CollisionAlgorithm::CollisionDetectionAlgorithm<L1::Head, L2::Head>::Fire<L2::Head, L1::Head>);
		AutomaticInsert<Class, L1, L2::Tail>::Init(d);
	};
};

template<typename Class, typename L1>
struct AutomaticInsert<Class,L1,L1>
{
	static void Init(Class& d)
	{
		d.Add<L1::Head, L1::Head>(CollisionAlgorithm::CollisionDetectionAlgorithm<L1::Head, L1::Head>::Fire<L1::Head, L1::Head>);
		AutomaticInsert<Class, L1, L1::Tail>::Init(d);
		AutomaticInsert<Class, L1::Tail, L1::Tail>::Init(d);
	};
};

//BASE CASE
template<typename Class, typename L1> 
struct AutomaticInsert<Class, L1, NullType>
{
	static void Init(Class& d)
	{
		return;
	};
};

template<typename Class, typename L1>
struct AutomaticInsert<Class, NullType, L1>
{
	static void Init(Class& d)
	{
		return;
	};
};

template<typename Class> 
struct AutomaticInsert<Class, NullType, NullType>
{
	static void Init(Class& d)
	{
		return;
	};
};



