#include "Collision.h"
#include"GameObject.h"
#include"Consts.h"

constexpr float MAX_COL_DEPTH_CUTOFF = 10.f;

bool Collisions::isOverlappingSAT(Collider* pLHB, Collider* pRHB, CollisionInfo& colInfo)
{

	auto* lHB = pLHB;
	auto* rHB = pRHB;

	if (lHB->_static && rHB->_static) // Skip unneccessary checks
	{
		return false;
	}
	auto Lverts = lHB->getColliderVerts();
	auto Rverts = rHB->getColliderVerts();
	v2 Collision_Normal = v2(0);

	float pen_depth = FLT_MAX;
	for (int x = 0; x < 2; ++x)
	{
		if (x == 1)
		{
			lHB = pRHB;
			rHB = pLHB;
		}
		
		for (int i = 0; i < Lverts.size(); ++i)
		{
			int b = (i + 1) % Lverts.size(); // wrap around all verts
			v2 ProjAxis = v2(-(Lverts[b].y - Lverts[i].y), (Lverts[b].x - Lverts[i].x)); // create normal to edge from flipping x, y components

			float min_r1 = FLT_MAX;
			float max_r1 = -FLT_MAX;

			// work out min and max poitns on L shape
			for (int j = 0; j < Lverts.size(); ++j)
			{
				float q = (Lverts[j].x * ProjAxis.x + Lverts[j].y * ProjAxis.y);
				min_r1 = std::min(min_r1, q);
				max_r1 = std::max(max_r1, q);
			}

			float min_r2 = FLT_MAX;
			float max_r2 = -FLT_MAX;
			// work out min and max poitns on R shape
			for (int j = 0; j < Rverts.size(); ++j)
			{
				float q = (Rverts[j].x * ProjAxis.x + Rverts[j].y * ProjAxis.y);
				min_r2 = std::min(min_r2, q);
				max_r2 = std::max(max_r2, q);
			}
			// Find the minimum overlap distance between the two shapes
			pen_depth = std::min(std::min(max_r1, max_r2) - std::max(min_r1, min_r2), pen_depth);
			if (!(max_r2 >= min_r1 && max_r1 >= min_r2))
			{
				return false;
			}
		}
	}
	v2 normal = glm::normalize(v2(rHB->centre.x - lHB->centre.x, rHB->centre.y - lHB->centre.y));
	colInfo.normal = normal;
	
		if (lHB->_type == kTriangle || rHB->_type == kTriangle)
		{
			colInfo.Depth = fabs(pen_depth) * 0.00001f;

		}
		else
		{
			colInfo.Depth = fabs(pen_depth);
		}
	if (fabs(pen_depth) >= MAX_COL_DEPTH_CUTOFF)
	{
		colInfo.Depth = 1.0f;
	}
	return true;
}
Collisions::CollisionInfo Collisions::CheckCollisions(GameObject* l_gobj, GameObject* r_gobj)
{
	CollisionInfo info;
	
	
	if (isOverlappingSAT(l_gobj->_hitbox, r_gobj->_hitbox, info))
	{
		info.pGameObj_l = l_gobj;
		info.pGameObj_r = r_gobj;
		info.HasCollision = true;
		
	}
	return info;
}


void Collisions::ResolveSAT(CollisionInfo info)
{
	if (info.Depth <= 0)
	{
		return;
	}
	auto* obj1 = info.pGameObj_l;
	auto* obj2 = info.pGameObj_r;
	if (fabs(info.normal.y) < 0.25f)
	{
		info.normal.y = 0;
	}
	if (fabs(info.normal.x) < 0.25f)
	{
		info.normal.x = 0;
	}
	if (!obj1->_hitbox->_static)
	{
		//if (obj1->position.y > obj2->position.y) // i.e. ob1 is below obj2
		//{
		//	obj1->position.y += (fabs(info.normal.y) * 0.5f); // move it down
		//}
		//else // obj1 above obj 2
		//{
		//	obj1->position.y -= (fabs(info.normal.y) * 0.5f); //// move ut up

		//}
		obj1->position.x += (info.normal.x * info.Depth) * 0.5f;
		obj1->position.y += (info.normal.y * info.Depth) * 0.5f;	
		
		obj1->velocity.x *= 0.5f;
		obj1->velocity.y *= 0.5f;
	}
	if (!obj2->_hitbox->_static)
	{
		//if (obj2->position.y < obj1->position.y) // i.e. ob1 is below obj2
		//{
		//	obj2->position.y -= (fabs(info.normal.y) * 0.5f); // move it down
		//}
		//else
		//{
		//	obj2->position.y += (fabs(info.normal.y) * 0.5f); //// move ut up

		//}
		obj2->position.x += (-info.normal.x * info.Depth) * 0.5f;
		obj2->position.y += (-info.normal.y * info.Depth) * 0.5f;
		obj2->velocity.x *= 0.5f;
		obj2->velocity.y *= 0.5f;
	}

}

Collisions::Circle::Circle()
{
	Radius = 0.f;
	_type = ColliderType::kCircle;
}

Collisions::Circle::Circle(float r)
{
	Radius = r;
	_type = ColliderType::kCircle;
}
v2 Cross2D(v2 A, v2 B)
{
	return v2(B.y, -A.x);
}



Collisions::AABB::AABB()
{
	_type = ColliderType::kAABB;
	extents = v2(11, 49);
	offset = v2(57, 152);
	auto r = this->getRect();

	_verts.push_back(v2(r.x, r.y)); // Bot left
	_verts.push_back(v2(r.x, r.y + r.h)); // Top left
	_verts.push_back(v2(r.x + r.w, r.y + r.h)); // Top Right
	_verts.push_back(v2(r.x + r.w, r.y)); // Bottom Right

	
}

std::vector<v2> Collisions::AABB::getColliderVerts()
{
	std::vector<v2> v;
	for (auto& vert : _verts)
	{
		v.push_back(vert - centre);
	}
	return _verts;
}

void Collisions::AABB::update(v2 Parent_Pos)
{
	position = Parent_Pos;
	centre = Parent_Pos + offset;
	auto r = getRect();

	_verts[0] = (v2(centre.x - extents.x, centre.y - extents.y)); // Bot left
	_verts[1] = (v2(centre.x - extents.x, centre.y + extents.y)); // top left
	_verts[2] = (v2(centre.x + extents.x, centre.y + extents.y)); // Top Right
	_verts[3] = (v2(centre.x + extents.x, centre.y - extents.y)); // Bottom Right

}

void Collisions::Polygon::update(v2 parent_pos)
{
	position = parent_pos;
	centre = parent_pos + offset;
}

SDL_FRect Collisions::AABB::getRect()
{
	float x = centre.x - extents.x;
	float y = centre.y - extents.y;
	float w = (extents.x * 2);
	float h = (extents.y * 2);

	return SDL_FRect{ .x = x, .y = y, .w = w, .h = h };
}

void Collisions::AABB::DrawGizmo(SDL_Renderer* pRender)
{
	auto rect = this->getRect();
	Uint8 r;
	Uint8 g;
	Uint8 b;
	Uint8 a;
	SDL_GetRenderDrawColor(pRender, &r, &g, &b, &a);
	SDL_SetRenderDrawColor(pRender, 255, 255, 255, 255);
	SDL_RenderDrawRectF(pRender, &rect);
	SDL_SetRenderDrawColor(pRender, r, g, b, a);

}

Collisions::Triangle::Triangle()
{
	_verts = { v2(1, 0), v2(1, 1), v2(0, 1) };
	t1 = _verts[0];
	t2 = _verts[1];
	t3 = _verts[2];
	_type = ColliderType::kTriangle;
}

Collisions::Triangle::Triangle(v2 a, v2 b, v2 c)
{
	_verts = { a, b, c };
	
	t1 = _verts[0];
	t2 = _verts[1];
	t3 = _verts[2];
	_type = ColliderType::kTriangle;
}

void Collisions::Triangle::update(v2 Parent_Pos)
{
	position = Parent_Pos;
	centre = Parent_Pos + offset;
	_verts = { t1, t2, t3 };
}

SDL_FRect Collisions::Triangle::getRect()
{
	return SDL_FRect();
}

void Collisions::Triangle::DrawGizmo(SDL_Renderer* pRender)
{
	Uint8 r;
	Uint8 g;
	Uint8 b;
	Uint8 a;
	SDL_GetRenderDrawColor(pRender, &r, &g, &b, &a);
	SDL_SetRenderDrawColor(pRender, 0, 255, 0, 255);
	SDL_RenderDrawPointF(pRender, centre.x, centre.y);
	SDL_SetRenderDrawColor(pRender, 255, 255, 255, 255);

	SDL_RenderDrawLine(pRender, centre.x + t1.x, centre.y + t1.y, centre.x + t2.x, centre.y + t2.y);
	SDL_RenderDrawLine(pRender, centre.x + t2.x, centre.y + t2.y, centre.x + t3.x, centre.y + t3.y);
	SDL_RenderDrawLine(pRender, centre.x + t3.x, centre.y + t3.y, centre.x + t1.x, centre.y + t1.y);

	SDL_SetRenderDrawColor(pRender, r, g, b, a);


}

std::vector<v2> Collisions::Triangle::getColliderVerts()
{
	std::vector<v2> verts = _verts;

	for (v2& v : verts)
	{
		v = v + centre;
	}

	return verts;
}

Collisions::Polygon::Polygon(std::vector<v2> Points)
{
	_type = kPolygon;
	for (auto& v : Points)
	{
		AddPoint(v);
	}
}

Collisions::Polygon::Polygon()
{
	_type = kPolygon;

}

void Collisions::Polygon::AddPoint(v2 point)
{
	_verts.push_back(point);
}

void Collisions::Polygon::removePoint(int idx)
{
	_verts.erase(_verts.begin() + idx);
}

std::vector<v2> Collisions::Polygon::getColliderVerts()
{
	return _verts;
}

SDL_FRect Collisions::Polygon::getRect()
{
	return SDL_FRect();
}

void Collisions::Polygon::DrawGizmo(SDL_Renderer* pRender)
{
}
