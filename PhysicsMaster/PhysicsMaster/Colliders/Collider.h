#pragma once

class AABB;

class Collider abstract
{
public:
	virtual void Collide(Collider&) = 0;
	virtual void Collide(AABB&) = 0;
};