#pragma once

#include <memory>
#include <string>

using int8 = int8_t;
using uint8 = uint8_t;
using int16 = int16_t;
using uint16 = uint16_t;
using int32 = int32_t;
using uint32 = uint32_t;
using int64 = int64_t;
using uint64 = uint64_t;

using UID = std::string;

namespace Common
{
class Object
{
public:
	Object();
	virtual ~Object();

	UID GetUID() { return m_uid; }
	void SetUID(UID uid) {
		m_uid = uid; 
	}
private:
	UID m_uid = "Empty";
};
}

