#pragma once
#include "Vector3.h"
#include "Component.h"

#define DoubleDispatchMethod public: static const int getType(); private: static const int _ColliderType;
#define DoubleDispatchEnumaration(T) const int T::_ColliderType = TL::IndexOf<ColliderType::ColliderTypeList, T>::value;const int  T::getType(){return T::_ColliderType; };
#define ColliderInit SetType(_ColliderType);

class Collider abstract :public Component
{
public:
	int GetType() const;
	const Vector3 GetWorldPosition() const;
	
protected:
	virtual ~Collider(){};
	void SetType(int type);
	Collider( const Vector3& displ);

private:
	int m_Type;
	Vector3 m_Displacement;

};