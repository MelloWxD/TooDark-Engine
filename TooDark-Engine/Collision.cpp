#include "Collision.h"
#include"GameObject.h"

v2 Collisions::tripleProduct(v2 a, v2 b, v2 c)
{
	v3 A = v3(a.x, a.y, 0);
	v3 B = v3(b.x, b.y, 0);
	v3 C = v3(c.x, c.y, 0);

	v3 second = glm::cross(glm::cross(A, B), C);

	return v2(second.x, second.y);
}

bool Collisions::isOverlapping(Collider* lHB, Collider* rHB, CollisionInfo& colInfo)
{
	auto Lverts = lHB->getColliderVerts();
	auto Rverts = rHB->getColliderVerts();
	v2 Collision_Normal = v2(0);

	bool col = false;
	for (auto Lpoint : Lverts) // Check diagonals of Polygon
	{
		v2 line_r1s = lHB->centre;
		v2 line_r1e = Lpoint;

		for (int x = 0; x < Rverts.size();++x) // against edges of another
		{
			v2 line_r2s = rHB->centre;
			v2 line_r2e = Rverts[(x+1 ) % Rverts.size()];

			float h = (line_r2e.x - line_r2s.x) * (line_r1s.y - line_r1e.y) - (line_r1s.x - line_r1e.x) * (line_r2e.y - line_r2s.y);
			float t1 = ((line_r2s.y - line_r2e.y) * (line_r1s.x - line_r2s.x) + (line_r2e.x - line_r2s.x) * (line_r1s.y - line_r2s.y)) / h;
			float t2 = ((line_r1s.y - line_r1e.y) * (line_r1s.x - line_r2s.x) + (line_r1e.x - line_r1s.x) * (line_r1s.y - line_r2s.y)) / h;

			if (t1 >= 0.0f && t1 < 1.0f && t2 >= 0.0f && t2 < 1.0f)
			{
				// If trigger return true immediately
				col = true;

				// Accumulate all normals
				Collision_Normal.x += (1.0f - t1) * (line_r1e.x - line_r1s.x);
				Collision_Normal.y += (1.0f - t1) * (line_r1e.y - line_r1s.y);
			}
		}
	}
	if (col)
	{
		if (Collision_Normal.x > Collision_Normal.y)
		{
			Collision_Normal.y = 0;
		}
		else
		{
			Collision_Normal.x = 0;
		}
		colInfo.normal = Collision_Normal;
		return true;
	}
	return false;
}
bool Collisions::isOverlappingSAT(Collider* pLHB, Collider* pRHB, CollisionInfo& colInfo)
{

	auto* lHB = pLHB;
	auto* rHB = pRHB;

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
	colInfo.Depth = pen_depth;
	return true;
}
Collisions::CollisionInfo Collisions::CheckCollisions(GameObject* l_gobj, GameObject* r_gobj)
{
	CollisionInfo info;
	info.pGameObj_l = l_gobj;
	info.pGameObj_r = r_gobj;
	
	if (isOverlappingSAT(&l_gobj->_hitbox, &r_gobj->_hitbox, info))
	{
		info.HasCollision = true;
		
	}
	return info;
}

void Collisions::Resolve(CollisionInfo info)
{
	auto* obj1 = info.pGameObj_l;
	auto* obj2 = info.pGameObj_r;

	if (!obj1->_static)
	{
		obj1->position.x -= (info.normal.x * 0.5f);
		obj1->position.y -= (info.normal.y * 0.5f);
	}
	if (!obj2->_static)
	{
		obj2->position.x += (info.normal.x * 0.5f);
		obj2->position.y += (info.normal.y * 0.5f);
	}
	obj1->_hitbox.update(obj1->position);
	obj2->_hitbox.update(obj2->position);
}
void Collisions::ResolveSAT(CollisionInfo info)
{
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
	if (!obj1->_static)
	{
		if (obj1->position.y > obj2->position.y) // i.e. ob1 is below obj2
		{
			obj1->position.y += (fabs(info.normal.y) * 0.5f); // move it down
		}
		else // obj1 above obj 2
		{
			obj1->position.y -= (fabs(info.normal.y) * 0.5f); //// move ut up

		}
		obj1->position.x += (info.normal.x * info.Depth) * 0.5f;
	}
	if (!obj2->_static)
	{
		if (obj2->position.y < obj1->position.y) // i.e. ob1 is below obj2
		{
			obj2->position.y -= (fabs(info.normal.y) * 0.5f); // move it down
		}
		else
		{
			obj2->position.y += (fabs(info.normal.y) * 0.5f); //// move ut up

		}
		obj2->position.x -= (info.normal.x * info.Depth) * 0.5f;
	}
	obj1->_hitbox.update(obj1->position);
	obj2->_hitbox.update(obj2->position);
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



SDL_Rect Collisions::AABB::getRect()
{
	int x = position.x + offset.x - extents.x;
	int y = position.y + offset.y - extents.y;
	int w = extents.x * 2;
	int h = extents.y * 2;

	return SDL_Rect{ .x = x, .y = y, .w = w, .h = h };
}

Collisions::Triangle::Triangle()
{
	t1 = _verts[0];
	t2 = _verts[1];
	t3 = _verts[2];
}

Collisions::Triangle::Triangle(v2 a, v2 b, v2 c)
{
	_verts[0] = a;
	_verts[1] = b;
	_verts[2] = c;
	t1 = _verts[0];
	t2 = _verts[1];
	t3 = _verts[2];
}

void Collisions::Triangle::update(v2 Parent_Pos)
{
	position = Parent_Pos;
	centre = Parent_Pos + offset;
}

std::vector<v2> Collisions::Triangle::getColliderVerts()
{
	return _verts;
}
