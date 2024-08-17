#pragma once

#include"Consts.h"
class GameObject;

#define GJK_MAX_ITER 50

namespace Collisions
{
	enum ColliderType : uint16_t
	{
		kNONE = 0u,
		kCircle = 1u,
		kTriangle = 2u,
		kAABB = 3u,
		kOBB = 4u
	};

	

	class Collider
	{
	public:
		v2 position;
		v2 offset;
		v2 centre;
		ColliderType _type = ColliderType::kNONE;
		std::vector<v2> _verts;
		virtual std::vector<v2> getColliderVerts() = 0;
		
	};

	v2 tripleProduct(v2 a, v2 b, v2 c);
	
	class Circle : virtual public Collider
	{
	public:
		Circle();
		
		Circle(float r);
		
		float Radius;
	};

	class Triangle : virtual public Collider
	{
	public:
		Triangle();
		Triangle(v2 t1, v2 t2,v2 t3);
		v2 t1;
		v2 t2;
		v2 t3;
		void update(v2 Parent_Pos);

		std::vector<v2> getColliderVerts() override;
	private:
		std::vector<v2> _verts = { v2(1, 0), v2(1, 1), v2(0, 1) };
	};
	
	class AABB : virtual public Collider
	{
	public:
		AABB();
		
		std::vector<v2> getColliderVerts() override;
		
		void update(v2 Parent_Pos);
		
		v2 FurthestPoint(v2 Direction); // find the furthest point in a given direction
		
		SDL_Rect getRect();
		

		v2 extents;
	};
	
	

	struct CollisionInfo
	{
		GameObject* pGameObj_r = nullptr;
		GameObject* pGameObj_l = nullptr;
		bool HasCollision = false;
		v2 normal = v2(0);
		float Depth = 0.f;
	};

	bool isOverlapping(Collider* l_aabb, Collider* r_aabb, CollisionInfo& colInfo);
	bool isOverlappingSAT(Collider* lHB, Collider* rHB, CollisionInfo& colInfo);
	CollisionInfo CheckCollisions(GameObject* l_gobj, GameObject* r_gobj);
	void Resolve(CollisionInfo info);
	void ResolveSAT(CollisionInfo info);
}
