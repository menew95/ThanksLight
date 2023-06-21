#include "YAMLToJsonParser.h"
#include <locale>
#include <codecvt>
#include <stdlib.h>
#include <cassert>

namespace YAMLToJsonParser
{
namespace fs = std::filesystem;
#define ToStringStream(str) '\"' << str << '\"'

	bool YAMLToJsonObject(std::stringstream& ss, std::string& line)
	{
		size_t posBegin = 0;
		size_t posEnd;
		if ((posEnd = line.find('{')) != std::string::npos)
		{
			if (line.length() == 4)
			{
				ss << line;
				return true;
			}
			ss << line.substr(posBegin, posEnd + 1 - posBegin);
			posBegin = posEnd + 1;
			while (true)
			{
				posEnd = line.find(':', posBegin);
				ss << ToStringStream(line.substr(posBegin, posEnd - posBegin));

				posBegin = posEnd;
				posEnd = line.find(' ', posBegin + 2);
				if (posEnd == std::string::npos)
				{
					ss << line.substr(posBegin);
					break;
				}

				size_t valuePos = line.find(' ', posBegin) + 1;
				for (size_t i = valuePos; i < posEnd; i++)
				{
					if ((line[i] >= 'a' && line[i] <= 'z') || (line[i] >= 'A' && line[i] <= 'Z'))
					{
						ss << line.substr(posBegin, valuePos - posBegin);
						ss << ToStringStream(line.substr(valuePos, posEnd - valuePos - 1));
						posBegin = posEnd - 1;
						break;
					}
				}

				ss << line.substr(posBegin, posEnd - posBegin);
				posBegin = posEnd + 1;
			}
			return true;
		}
		return false;
	}

	size_t GetDepth(std::string& line)
	{
		size_t newDepth = line.find_first_not_of(' ');
		if (newDepth == std::string::npos)
			newDepth = 0;
		else
		{
			if (line[0] != '-' && line[line.find_first_not_of(' ')] == '-')
			{
				newDepth += 2;
			}
			newDepth /= 2;
		}


		return newDepth;
	}
	void AddWhiteSpaceWithDepth(std::ostream& os, size_t depth)
	{
		for (size_t i = 0; i < depth * 2; i++)
		{
			os << ' ';
		}
	}

	void ParseToJson(fs::path fileName, fs::path sourceDirectory, fs::path outDirectory)
	{
		if (!fs::exists(sourceDirectory))
		{
			fs::create_directory(sourceDirectory);
		}
		if (!fs::exists(outDirectory))
		{
			fs::create_directory(outDirectory);
		}
		std::stringstream ss;
		ParseToJson(fileName, ss);

		fileName.replace_extension(".json");
		std::fstream fs = std::fstream(outDirectory / fileName.filename(), std::fstream::out);

		if (fs.is_open())
		{
			fs << ss.rdbuf();

			fs.close();
		}
	}

	std::string GetPrefabJson(fs::path fileName)
	{
		fileName.replace_extension(".json");
		std::fstream fs = std::fstream(fileName, std::fstream::in);

		std::stringstream ss;

		ss << fs.rdbuf();

		std::string result = ss.str();
		result = result.substr(2, result.length() - 4);
		return result;
	}

	void RemoveIndentation(std::string& str)
	{
		str.assign(&str[str.find_first_not_of(" -")]);
	}

	std::string GetKey(std::string& str)
	{
		return str.substr(0, str.find_first_of(':'));
	}

	struct Node
	{
		std::string key;
		std::string value;
		std::vector<Node> nodes;
		int depth = 0;
		bool isArray = false;
		bool isArrayElement = false;
	};

	std::vector<std::string> m_lines;
	int m_currentLineIndex;

	size_t GetDepth_(std::string& str)
	{
		size_t depth = str.find_first_not_of(' ');

		if (depth == str.npos)
			return 0;
		
		if (str[depth] == '-')
		{
			depth = str.find_first_not_of(' ', depth + 1);
		}

		return depth;
	}

	bool HasNode(std::string& str)
	{
		if (str.find_first_of('{') != str.npos
			|| str.find_first_of(':') == str.size() - 1)
		{
			return true;
		}

		return false;
	}

	void ParseLineMap(std::string& str, Node& node)
	{
		size_t startIndex = str.find_first_of('{') + 1;

		if (startIndex > str.size() || startIndex == str.npos + 1)
			return;

		size_t endIndex = str.find_first_of(':', startIndex);

		while (endIndex != str.npos)
		{
			Node n;
			n.depth = node.depth + 1;
			n.key = str.substr(startIndex, endIndex - startIndex);

			startIndex = endIndex + 2;
			endIndex = str.find_first_of(',', startIndex);

			if (endIndex == str.npos)
			{
				endIndex = str.find_first_of('}', startIndex);
				n.value = str.substr(startIndex, endIndex - startIndex);

				node.nodes.push_back(n);
				break;
			}

			n.value = str.substr(startIndex, endIndex - startIndex);

			node.nodes.push_back(n);

			startIndex = endIndex + 2;
			endIndex = str.find_first_of(':', startIndex);
		}
	}

	bool HasLineMap(std::string& str)
	{
		return str.find_first_of('{') != str.npos ? true : false;
	}

	void GetLine(std::string& line)
	{
		line = m_lines[m_currentLineIndex++];
	}

	void GetNextLine(std::string& line)
	{
		line = m_lines[m_currentLineIndex + 1];
	}

	void ParseToNode_(std::string& line, Node& node)
	{
		if (line == "") return;

		node.depth = GetDepth_(line);

		if (node.isArrayElement)
		{
			Node child;
			ParseToNode_(line, child);
			node.nodes.push_back(child);
			std::string nextLine;

			while (m_currentLineIndex < m_lines.size())
			{
				GetLine(nextLine);
				if (GetDepth_(nextLine) < node.depth || nextLine[nextLine.find_first_not_of(' ')] == '-')
				{
					m_currentLineIndex--;
					break;
				}
				Node child;
				ParseToNode_(nextLine, child);
				node.nodes.push_back(child);
			}
			return;
		}
		RemoveIndentation(line);

		node.key = GetKey(line);

		if (HasLineMap(line))
		{
			ParseLineMap(line, node);
			return;
		}

		if (HasNode(line))
		{
			while (m_currentLineIndex < m_lines.size())
			{
				std::string nextLine;
				GetLine(nextLine);
				Node child;
				if (GetDepth_(nextLine) <= node.depth)
				{
					m_currentLineIndex--;
					break;
				}

				if (nextLine[nextLine.find_first_not_of(' ')] == '-')
				{
					node.isArray = true;
				}
				if (node.isArray)
				{
					child.isArrayElement = true;
				}

				ParseToNode_(nextLine, child);
				node.nodes.push_back(child);
			}
		}
		else
		{
			int index = line.find_first_of(':') + 2;
			if (index < line.size())
			{
				node.value = line.substr(index);
			}
		}
	}

	void ParseToNode(Node& node)
	{
		std::string line;
		while (m_currentLineIndex < m_lines.size())
		{
			GetLine(line);
			if (GetDepth_(line) == 0)
			{
				break;
			}
			ParseToNode_(line, node);
		}
	}

	void AddIndentation(std::stringstream& ss, int depth)
	{
		for (int i = 0; i < depth; i++)
		{
			ss << ' ';
		}
	}

	struct Vector3Data
	{
		explicit Vector3Data(std::string name)
		{
			this->name = name;
		}
		std::string name;
		float x = -9999;
		float y = -9999;
		float z = -9999;

		friend std::stringstream& operator<<(std::stringstream& os, const Vector3Data& dt);
	};

	std::stringstream& operator<<(std::stringstream& os, const Vector3Data& dt)
	{
		assert(dt.name != "");

		AddIndentation(os, 2);
		os << ToStringStream(dt.name) << ": {\"x\": ";
		if (dt.x == -9999)
			os << "null";
		else
			os << dt.x;

		os << ", \"y\" : ";
		if (dt.y == -9999)
			os << "null";
		else
			os << dt.y;

		os << ", \"z\" : ";
		if (dt.z == -9999)
			os << "null";
		else
			os << dt.z; 

		os << " }";
		return os;
	}

	struct Vector4Data
	{
		explicit Vector4Data(std::string name)
		{
			this->name = name;
		}
		std::string name;
		float x = -9999;
		float y = -9999;
		float z = -9999;
		float w = -9999;

		friend std::stringstream& operator<<(std::stringstream& os, const Vector4Data& dt);
	};

	std::stringstream& operator<<(std::stringstream& os, const Vector4Data& dt)
	{
		assert(dt.name != "");

		AddIndentation(os, 2);
		os << ToStringStream(dt.name) << ": {\"x\": ";
		if (dt.x == -9999)
			os << "null";
		else
			os << dt.x;

		os << ", \"y\" : ";
		if (dt.y == -9999)
			os << "null";
		else
			os << dt.y;

		os << ", \"z\" : ";
		if (dt.z == -9999)
			os << "null";
		else
			os << dt.z;

		os << ", \"w\" : ";
		if (dt.w == -9999)
			os << "null";
		else
			os << dt.w;

		os << " }";
		return os;
	}

	Vector3Data m_LocalPosition{ "m_LocalPosition" };
	Vector4Data m_LocalRotation{ "m_LocalRotation" };
	Vector3Data m_LocalScale{ "m_LocalScale" };

	struct MaterialData
	{
		explicit MaterialData(std::string name)
		{
			this->name = name;
		}
		std::string name;
		int index = -9999;
		std::string guid;

		friend std::stringstream& operator<<(std::stringstream& os, const MaterialData& dt);
	};

	std::stringstream& operator<<(std::stringstream& os, const MaterialData& dt)
	{
		assert(dt.name != "");

		AddIndentation(os, 2);
		os << ToStringStream(dt.name) << ": {\n";
		AddIndentation(os, 3);
		os << "\"index\": " << dt.index << ",\n";

		AddIndentation(os, 3);
		os << "\"guid\": " << ToStringStream(dt.guid) << "\n";

		AddIndentation(os, 2);
		os << " }";

		return os;
	}
	std::vector<MaterialData> m_materialDatas;

	std::string m_prefabGUID;
	
	void PrefabNodeParsing(Node& node)
	{
		if (node.nodes.size() == 4)
		{
			if (node.nodes[0].key == "target")
			{
				m_prefabGUID = node.nodes[0].nodes[1].value;
			}

			if (node.nodes[1].value == "m_LocalPosition.x")
			{
				m_LocalPosition.x = std::stof(node.nodes[2].value);
				return;
			}
			else if (node.nodes[1].value == "m_LocalPosition.y")
			{
				m_LocalPosition.y = std::stof(node.nodes[2].value);
				return;
			}
			else if (node.nodes[1].value == "m_LocalPosition.z")
			{
				m_LocalPosition.z = std::stof(node.nodes[2].value);
				return;
			}

			if (node.nodes[1].value == "m_LocalRotation.x")
			{
				m_LocalRotation.x = std::stof(node.nodes[2].value);
				return;
			}
			else if (node.nodes[1].value == "m_LocalRotation.y")
			{
				m_LocalRotation.y = std::stof(node.nodes[2].value);
				return;
			}
			else if (node.nodes[1].value == "m_LocalRotation.z")
			{
				m_LocalRotation.z = std::stof(node.nodes[2].value);
				return;
			}
			else if (node.nodes[1].value == "m_LocalRotation.w")
			{
				m_LocalRotation.w = std::stof(node.nodes[2].value);
				return;
			}

			if (node.nodes[1].value == "m_LocalScale.x")
			{
				m_LocalScale.x = std::stof(node.nodes[2].value);
				return;
			}
			else if (node.nodes[1].value == "m_LocalScale.y")
			{
				m_LocalScale.y = std::stof(node.nodes[2].value);
				return;
			}
			else if (node.nodes[1].value == "m_LocalScale.z")
			{
				m_LocalScale.z = std::stof(node.nodes[2].value);
				return;
			}

			std::string materialPropertyPath = "m_Materials.Array.data[0]";
			for (int i = 0; i < 7; ++i)
			{
				materialPropertyPath[23] = i + '0';
				if (node.nodes[1].value == materialPropertyPath)
				{
					MaterialData data("m_Materials" + std::to_string(i));
					data.index = i;
					data.guid = node.nodes[3].nodes[1].value;
					m_materialDatas.push_back(data);
				}
			}
		}

		if (node.nodes.size() > 0)
		{
			for (int i = 0; i < node.nodes.size(); ++i)
			{
				PrefabNodeParsing(node.nodes[i]);
			}
		}
	}

	void PrefabNodeToStream(Node& node, std::stringstream& ss)
	{
		AddIndentation(ss, 2);
		ss << ToStringStream("guid") << ": " << ToStringStream(m_prefabGUID) << ",\n";

		ss << m_LocalPosition << ",\n";
		ss << m_LocalRotation << ",\n";
		ss << m_LocalScale;
		if (m_materialDatas.size() > 0)
		{
			for (auto& materialData : m_materialDatas)
			{
				ss << ",\n";
				ss << materialData;
			}
			ss << "\n";
		}
		else
		{
			ss << "\n";
		}

		//AddIndentation(ss, node.depth);
		//if(!node.isArrayElement)
		//	ss << ToStringStream(node.key) << ": ";

		//if (node.value != "")
		//{
		//	if (node.value.find_first_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ") != std::string::npos
		//		|| node.key.find("m_SceneGUID") != std::string::npos
		//		|| (node.key.find("m_Name") != std::string::npos && node.key.find_first_not_of(' ', node.key.find(": ") + 1) != std::string::npos)
		//		|| node.key.find("internalID") != std::string::npos
		//		|| node.key.find("fileID") != std::string::npos
		//		|| node.key.find("spriteID") != std::string::npos)guid
		//	{
		//		ss << ToStringStream(node.value);
		//	}
		//	else
		//	{
		//		ss << node.value;
		//	}
		//}

		//if (node.nodes.size() > 0)
		//{
		//	if (node.isArray)
		//	{
		//		ss << "[\n";
		//	}
		//	else
		//	{
		//		ss << "{\n";
		//	}

		//	for (int i = 0; i < node.nodes.size(); ++i)
		//	{
		//		PrefabNodeToStream(node.nodes[i], ss);
		//		if (i != node.nodes.size() - 1)
		//		{
		//			ss << ',';
		//		}
		//		ss << "\n";
		//	}
		//	AddIndentation(ss, node.depth);

		//	if (node.isArray)
		//	{
		//		ss << "]";
		//	}
		//	else
		//	{
		//		ss << "}";
		//	}
		//}
	}
	std::string UID = "";
	void TokenizeToLine(std::fstream& fs, std::vector<std::string>& lines)
	{
		std::string line;
		if (fs.is_open())
		{
			while (!fs.eof())
			{
				getline(fs, line);
				if (line[0] == '-')
				{
					UID = line.substr(line.find('&') + 1);
					break;
				}
				if (GetDepth_(line) == 0)
				{
					break;
				}
				lines.push_back(std::move(line));
			}
		}
	}

	void InitPrefab()
	{
		m_materialDatas.clear();
		m_LocalPosition = Vector3Data("m_LocalPosition");
		m_LocalRotation= Vector4Data("m_LocalRotation");
		m_LocalScale = Vector3Data("m_LocalScale");
		m_prefabGUID = "";
	}

	void ParsePrefab(std::fstream& fs, std::stringstream& ss)
	{
		InitPrefab();

		std::string line;
		Node node;
		m_lines.clear();
		m_currentLineIndex = 0;

		TokenizeToLine(fs, m_lines);

		ParseToNode(node);

		PrefabNodeParsing(node);

		PrefabNodeToStream(node, ss);

		ss << "}";
	}

	// 그지같넹!! 단순히 한줄 한줄 파싱하려고 했는데 
	// 자료구조로 끊어서 읽는게 더 나았을지도.. 
	// 하지만 돌아가긴 너무 귀찮아...
	void ParseToJson(fs::path path, std::stringstream& ss)
	{
		std::fstream fs = std::fstream(path);
		assert(fs.is_open());

		if (fs.is_open())
		{
			ss << "{\n";

			UID = "";
			size_t depth = 0;
			std::string line;
			bool isFirst = true;
			//std::stack<int> arrayObjectDepth;
			bool isText = false;

			while (!fs.eof())
			{
				getline(fs, line);

				//거지같은 유니티 Scene YAML 예외 사항
				if (line.length() > 1 && line[line.length() - 1] == ',')
				{
					std::string remainLine;
					getline(fs, remainLine);

					line.append(remainLine.substr(remainLine.find_first_not_of(' ') - 1));
				}

				size_t fileIDPos = line.find("fileID:");
				if (fileIDPos != std::string::npos)
				{
					fileIDPos = line.find_first_of(' ', fileIDPos + 6) + 1;
					line.insert(fileIDPos, 1, '\"');
					fileIDPos = line.find_first_of("}, ", fileIDPos + 1);
					line.insert(fileIDPos, 1, '\"');
				}

				if (line[0] == '%' || line.find_first_not_of(' ') == std::string::npos)
					continue;

				//새로운 문서
				if (line[0] == '-')
				{
					UID = line.substr(line.find('&') + 1);
					continue;
				}

				if (line[0] != ' ' && line.find("guid:") != std::string::npos)
				{
					UID = line.substr(line.find_first_of(' ') + 1);
				}
				else if (line[0] != '\t' && line[0] != ' ' && UID.length() > 0)
				{
					if (!isFirst) ss << ",\n"; isFirst = false;
					line.pop_back();

					ss << ToStringStream(UID) << " : {\n";
					ss << "\t" << ToStringStream("className") << ": " << ToStringStream(line);
					depth++;

					if (line == "PrefabInstance")
					{
						ss << ",\n";
						ParsePrefab(fs, ss);
						depth = 0;
					}
				}
				//멤버
				else if (line[0] == ' ')
				{
					bool isFirstArrayElement = false;
					std::string preLine;
					std::stack<int> arrayDepth;
					//한 문서가 끝날 때 까지 실행
					while (depth > 0)
					{
						{
							size_t newDepth = GetDepth(line);

							if (depth > newDepth)
							{
								size_t gap = depth - newDepth;
								if (!arrayDepth.empty() && preLine[0] != '{')
								{
									for (int i = gap; i > 0 && depth > arrayDepth.top(); i--)
									{
										depth--;
										ss << '\n';
										AddWhiteSpaceWithDepth(ss, depth);
										ss << "}";
										gap--;

										if (line.length() > 1 && line[line.find_first_not_of(' ')] == '-'
											&& !arrayDepth.empty() && arrayDepth.top() + 1 >= GetDepth(line))
										{
											if (line.find_last_of(':') == line.size() - 1)
											{
												ss << ",\n";
											}
										}
									}
								}

								if (!arrayDepth.empty() && arrayDepth.top() >= newDepth)
								{
									arrayDepth.pop();
									ss << '\n';
									AddWhiteSpaceWithDepth(ss, depth);
									ss << ']';

									if (preLine[0] == '{')
									{
										depth--;
										gap--;
									}
								}

								for (int i = gap; i > 0; i--)
								{
									depth--;
									ss << '\n';
									AddWhiteSpaceWithDepth(ss, depth);
									ss << "}";
								}

								if (newDepth == 0)
								{
									assert(depth == 0, "");

									break;
								}
							}

							if (depth == newDepth)
							{
								if (line[line.find_first_not_of(' ')] != '-')
								{
									ss << ",\n";
								}
							}

							if (depth < newDepth)
							{
								int pos = line.find_first_not_of(' ');
								if (line[pos] != '-' || line[pos + 2] != '{')
								{
									ss << '\n';
									AddWhiteSpaceWithDepth(ss, depth);
									ss << "{\n";
								}
							}

							isFirstArrayElement = false;
							depth += newDepth - depth;

							AddWhiteSpaceWithDepth(ss, depth);

							line.erase(line.begin(), line.begin() + depth * 2);
						}

						// 멤버 중에 스페이스가 들어가는게 있음......
						{
							size_t nameLength = line.find_first_of(':');
							if (nameLength != std::string::npos && line[0] != '-')
							{
								std::string memberName = line.substr(0, nameLength);
								size_t pos = memberName.find_first_of(' ');
								while (pos != std::string::npos)
								{
									line.replace(pos, 1, 1, '_');
									pos = memberName.find_first_of(' ', pos + 1);
								}
							}
						}

						//배열 요소일 경우
						if (line[0] == '-')
						{
							line.erase(0, 2);

							//if (arrayDepth.top() == depth && line[0] != '{' && line.find_first_of(':'))
							//{
							//	ss << "{ ";
							//	arrayObjectDepth.push((int)depth);
							//}
						}

						size_t nameLength = line.find_first_of(':');

						if (nameLength == line.npos)
						{
							preLine = line;
							getline(fs, line);
							continue;
						}

						std::string withoutName = line.substr(nameLength);

						if (line[0] != '{')
						{
							ss << ToStringStream(line.substr(0, nameLength));
						}

						//한줄 객체
						if (YAMLToJsonObject(ss, line[0] != '{' ? withoutName : line))
						{
						}
						else if (withoutName.length() > 2 && withoutName[2] == '\"')
						{
							ss << withoutName;
						}
						else if (withoutName.find_first_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ") != std::string::npos
							|| line.find("m_SceneGUID") != std::string::npos
							|| (line.find("m_Name") != std::string::npos && line.find_first_not_of(' ', line.find(": ") + 1) != std::string::npos)
							|| line.find("internalID:") != std::string::npos
							|| line.find("spriteID:") != std::string::npos)
						{
							ss << withoutName.substr(0, 2);
							ss << ToStringStream(withoutName.substr(2));
						}
						else
						{
							ss << withoutName;
						}
						
						//다음 라인인 읽고 어떤 타입인지에 따라 처리
						std::string nextLine;
						getline(fs, nextLine);

						//유니코드에 '\'추가
						{
							size_t unicodePos = nextLine.find("\\u");
							while (unicodePos != std::string::npos)
							{
								nextLine.insert(unicodePos, 1, '\\');
								unicodePos = nextLine.find("\\u", unicodePos + 3);
							}
						}

						{
							size_t fileIDPos = nextLine.find("fileID:");
							if (fileIDPos != std::string::npos)
							{
								fileIDPos = nextLine.find_first_of(' ', fileIDPos + 6) + 1;
								nextLine.insert(fileIDPos, 1, '\"');
								fileIDPos = nextLine.find_first_of("}, ", fileIDPos + 1);
								nextLine.insert(fileIDPos, 1, '\"');
							}
						}

						//텍스트가 '로 시작함...
						//그것도 영어만 있을떄! 장난하나 
						//너무 귀찮으니 그냥 한글이 있다는 가정으로.. 

						if (nextLine[0] == '-')
						{
							if (withoutName.find_first_not_of(' ', 1) == std::string::npos)
							{
								ss << " null";
							}
							UID = nextLine.substr(nextLine.find('&') + 1);
							continue;
						}

						//if (depth == GetDepth(nextLine) && (!arrayDepth.empty() && arrayDepth.top() >= GetDepth(nextLine))
						//	&& (!arrayObjectDepth.empty() && arrayObjectDepth.top() >= GetDepth(nextLine)))
						//{
						//	ss << " }";
						//	arrayObjectDepth.pop();
						//}
						//거지같은 유니티 Scene YAML 예외 사항
						if (nextLine.length() > 1 && nextLine[nextLine.length() - 1] == ',')
						{
							std::string remainLine;
							getline(fs, remainLine);

							nextLine.append(remainLine.substr(remainLine.find_first_not_of(' ') - 1));
						}

						size_t firstCharPos = nextLine.find_first_not_of(' ');

						if (nextLine.length() > 1 && nextLine[firstCharPos] == '-' 
							&& !arrayDepth.empty() && arrayDepth.top() + 1 >= GetDepth(nextLine))
						{
							if (nextLine[firstCharPos + 2] == '{')
							{
								ss << ",\n";
							}
							else if (nextLine.find_last_of(':') == nextLine.size() - 1)
							{
								//ss << ",\n";
								//AddWhiteSpaceWithDepth(ss, depth - 1);
								//ss << "}";
							}
							else
							{
								ss << "\n";
								AddWhiteSpaceWithDepth(ss, depth - 1);
								ss << "},\n";
								AddWhiteSpaceWithDepth(ss, depth - 1);
								ss << "{\n";
							}
							
						}

						preLine = line;
						line = nextLine;

						// : 이후 값을 없을 때 ( 없는 경우 - 배열, 객체, null )
						if (withoutName.find_first_not_of(' ', 1) == std::string::npos)
						{
							{
								// 파일 끝
								if (firstCharPos == std::string::npos || line[0] == '-')
								{
									ss << " null";
									continue;
								}
							}

							//배열 일 때
							if (nextLine[firstCharPos] == '-')
							{
								arrayDepth.push((int)depth);
								ss << "[";
								isFirstArrayElement = true;
								continue;
							}


							if (depth >= GetDepth(nextLine))
							{
								ss << " null";
							}
						}
					}
				}
			}
			ss << "\n}";
			fs.close();
		}
	}
}