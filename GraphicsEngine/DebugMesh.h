#pragma once

#include "Mesh.h"

namespace TLGraphicsEngine
{
	class DebugMesh :
	    public Mesh
	{
	public:
		DebugMesh();
		DebugMesh(const DebugMesh&) = default;
		DebugMesh(DebugMesh&&) = default;
		DebugMesh& operator=(DebugMesh&&) = default;
		virtual ~DebugMesh();

	public:
		void MakeCameraMesh();
		void MakeGridMesh();
		void MakeAxisMesh();
		void MakeBoundingBoxMesh();
		void MakeBoxColliderMesh();
		//void MakeSkyBoxMesh();
		void MakePlane();
	};
}