#pragma once

namespace TLGameEngine
{
struct Vertex
{
	Vertex(Vector3 pos, Vector4 color)
		: Pos(pos), Color(color), UV(0, 0), Normal(0, 1, 0)
		, Tangent(0, 0, 0), Bone(0, 0, 0, 0), Weight(0, 0, 0, 0)
	{ }
	Vertex() : Pos(0, 0, 0), Color(1, 1, 1, 1), UV(0, 0)
		, Normal(1, 0, 0), Tangent(0, 0, 0), Bone(0, 0, 0, 0)
		, Weight(0, 0, 0, 0)
	{ }
	Vertex(Vector3 pos) : Pos(pos), Color(1, 1, 1, 1), UV(0, 0)
		, Normal(1, 0, 0), Tangent(0, 0, 0), Bone(0, 0, 0, 0)
		, Weight(0, 0, 0, 0)
	{ }
	Vector3 Pos;
	Vector4 Color;
	Vector2 UV;
	Vector3 Normal;
	Vector3 Tangent;

	Vector4 Bone; // 스킨 매쉬용 => 일단 지금은 본이 네개 이하인 것만 고려한다.
	Vector4 Weight; // 스킨 매쉬용 => 일단 지금은 본이 네개 이하인 것만 고려한다.
};
}
