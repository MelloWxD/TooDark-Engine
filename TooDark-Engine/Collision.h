#pragma once

#include"Consts.h"
class GameObject;


namespace Collisions
{

	struct CollisionInfo
	{
		GameObject* pGameObj_r = nullptr;
		GameObject* pGameObj_l = nullptr;
		bool HasCollision = false;
		v2 normal = v2(0);
		float Depth = 0.f;
	};
	enum ColliderType : uint16_t
	{
		kNONE = 0u,
		kCircle = 1u,
		kTriangle = 2u,
		kAABB = 3u,
		kPolygon = 4u,
		kOBB = 4u
	};

	class Collider // Abstract base class
	{
	public:
		v2 position;
		v2 offset;
		v2 centre;
		ColliderType _type = ColliderType::kNONE;
		std::vector<v2> _verts;
		virtual std::vector<v2> getColliderVerts() = 0;
		
	};

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
		
		
		SDL_Rect getRect();
		

		v2 extents;
	};

	class Polygon : virtual public Collider
	{
		Polygon(std::vector<v2> Points);
		Polygon();
		void AddPoint(v2 point);
		void removePoint(int idx);
		std::vector<v2> getColliderVerts();
	};
	bool isOverlappingSAT(Collider* lHB, Collider* rHB, CollisionInfo& colInfo);
	CollisionInfo CheckCollisions(GameObject* l_gobj, GameObject* r_gobj);
	void ResolveSAT(CollisionInfo info);
}
