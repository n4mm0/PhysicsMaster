#pragma once
#include "Utilities\TypeList.h"
#include "Collision\CollisionAlgorithms.h"

//CUSTOM FOR COLLISIONDISPATCHER 
//NOW LESS CUSTOM BUT STILL CUSTOM T.T

template<typename Class, typename L1, typename L2, template<class, class> class Callee>
struct AutomaticInsert
{
	static void Init(Class& d)
	{
	//	d.Add<L1::Head, L2::Head>(CollisionAlgorithm::CollisionDetectionAlgorithm<L1::Head, L2::Head>::Fire<L1::Head, L2::Head>);
	//	d.Add<L2::Head, L1::Head>(CollisionAlgorithm::CollisionDetectionAlgorithm<L1::Head, L2::Head>::Fire<L2::Head, L1::Head>);
		d.Add<L1::Head, L2::Head>(Callee<L1::Head, L2::Head>::Fire<L1::Head, L2::Head>);
		d.Add<L2::Head, L1::Head>(Callee<L1::Head, L2::Head>::Fire<L2::Head, L1::Head>);

		AutomaticInsert<Class, L1, L2::Tail, Callee>::Init(d);
	};
};

template<typename Class, typename L1, template<typename, typename> class Callee >
struct AutomaticInsert<Class, L1, L1, Callee>
{
	static void Init(Class& d)
	{
	//	d.Add<L1::Head, L1::Head>(CollisionAlgorithm::CollisionDetectionAlgorithm<L1::Head, L1::Head>::Fire<L1::Head, L1::Head>);
		d.Add<L1::Head, L1::Head>(Callee<L1::Head, L1::Head>::Fire<L1::Head, L1::Head>);

		AutomaticInsert<Class, L1, L1::Tail, Callee >::Init(d);
		AutomaticInsert<Class, L1::Tail, L1::Tail, Callee>::Init(d);
	};
};

//BASE CASE
template<typename Class, typename L1, template<class, class> class Callee  >
struct AutomaticInsert<Class, L1, NullType, Callee >
{
	static void Init(Class& d)
	{
		return;
	};
};

template<typename Class, typename L1, template<class, class> class Callee >
struct AutomaticInsert<Class, NullType, L1, Callee>
{
	static void Init(Class& d)
	{
		return;
	};
};

template<typename Class, template<class, class> class Callee >
struct AutomaticInsert<Class, NullType, NullType, Callee >
{
	static void Init(Class& d)
	{
		return;
	};
};



