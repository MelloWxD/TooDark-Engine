#pragma once

#include"Consts.h"
class GameObject;
class SDL_Renderer;

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
		v2 position = v2(0);
		v2 offset = v2(0);
		v2 centre = v2(0);
		v2 extents = v2(0);
		v2 t1, t2, t3;
		ColliderType _type = ColliderType::kNONE;
		std::vector<v2> _verts;
		virtual SDL_Rect getRect() = 0;
		bool _static = false;
		void setExtents(v2 e)
		{
			extents = e;
		}
		virtual std::vector<v2> getColliderVerts() = 0;
		virtual void update(v2 parent_pos) = 0;
		virtual void DrawGizmo(SDL_Renderer* pRender) = 0;
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
		void update(v2 Parent_Pos) override;

		void DrawGizmo(SDL_Renderer* pRender) override;
		SDL_Rect getRect() override;

		std::vector<v2> getGizmoColliderVerts();
		std::vector<v2> getColliderVerts() override;
	};
	
	class AABB : virtual public Collider
	{
	public:
		AABB();
		
		std::vector<v2> getColliderVerts() override;
		
		void update(v2 Parent_Pos) override;
		
		void DrawGizmo(SDL_Renderer* pRender) override;

		SDL_Rect getRect() override;
		

	};

	class Polygon : virtual public Collider
	{
	public:
		Polygon(std::vector<v2> Points);
		Polygon();
		void AddPoint(v2 point);

		void removePoint(int idx);
		void DrawGizmo(SDL_Renderer* pRender) override;

		void update(v2 Parent_Pos) override;

		SDL_Rect getRect() override;
		std::vector<v2> getColliderVerts() override;
	};
	bool isOverlappingSAT(Collider* lHB, Collider* rHB, CollisionInfo& colInfo);
	CollisionInfo CheckCollisions(GameObject* l_gobj, GameObject* r_gobj);
	void ResolveSAT(CollisionInfo info);
}
