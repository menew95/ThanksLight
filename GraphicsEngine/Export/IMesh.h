#pragma once

#include "ResourceBase.h"

namespace TLGraphicsEngine
{
	struct Buffer;

	class IMesh : 
		public ResourceBase
	{
	public:
		IMesh() {}
		IMesh(const char* name) : ResourceBase(eResourceType::Mesh, name) {}

		virtual ~IMesh() {}
		
		virtual bool GetVertexBufferInfo(Buffer& output) abstract;
		virtual bool GetIndexBufferInfo(Buffer& output) abstract;

	};

	struct Buffer
	{
		void* buffer;
		UINT32 bufferCnt;
		UINT32 bufferSize;
	};

}