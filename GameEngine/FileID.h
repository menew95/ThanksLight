#pragma once

#include <string>
#include "boost/describe/class.hpp"

struct FileID
{
	std::string fileID;
};
BOOST_DESCRIBE_STRUCT(FileID, (), (fileID))

struct FileInfo
{
	std::string fileID;
	std::string guid;
};
BOOST_DESCRIBE_STRUCT(FileInfo, (), (fileID, guid))

struct ComponentFile
{
	FileID component;
};
BOOST_DESCRIBE_STRUCT(ComponentFile, (), (component))

struct ParsingColor
{
	ParsingColor() {}
	ParsingColor(float r, float g, float b, float a)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
	float r, g, b, a;
};
BOOST_DESCRIBE_STRUCT(ParsingColor, (), (r, g, b, a));
