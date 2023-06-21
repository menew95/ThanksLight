#pragma once


#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <stack>
#include <fstream>
#include <filesystem>
#include <unordered_map>

namespace YAMLToJsonParser
{
	bool YAMLToJsonObject(std::stringstream& ss, std::string& line);

	size_t GetDepth(std::string& line);

	void AddWhiteSpaceWithDepth(std::ostream& os, size_t depth);

	void ParseToJson(std::filesystem::path fileName, std::filesystem::path sourceDirectory, std::filesystem::path outDirectory);
	void ParseToJson(std::filesystem::path path, std::stringstream& ss);
	std::string GetPrefabJson(std::filesystem::path fileName);
}