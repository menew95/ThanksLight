#include "ParsingData.h"

ASEParse::VertexInfo::VertexInfo()
	: m_pos()
	, m_normal()
	, m_IsNormalSet(false)
	, u(0.0f)
	, v(0.0f)
	, m_IsTextureSet(false)
	, m_bones()
	, m_weights()
	, m_indices(0)
	, m_matRef(0)
{

}


ASEParse::FaceInfo::FaceInfo()
{
	m_vertexindex[0] = 0;
	m_vertexindex[1] = 0;
	m_vertexindex[2] = 0;

	m_materialRef = 0;
	m_TFace = Vector3(0, 0, 0);
	m_normal = Vector3(0, 0, 0);
}

ASEParse::PosTrack::PosTrack(float frameRate, Vector3 pos)
: m_FrameRate(frameRate)
, m_Pos_Sample(pos)
{

}

ASEParse::RotTrack::RotTrack(float frameRate, Quaternion rot)
: m_FrameRate(frameRate)
, m_Rot_Sample(rot)
{

}

ASEParse::RotTrack::RotTrack(float frameRate, Quaternion rot, Vector3 eulerRot)
: m_FrameRate(frameRate)
, m_Rot_Sample(rot)
, m_eulerRot_Sample(eulerRot)
{

}

ASEParse::ScaleTrack::ScaleTrack(float frameRate, Vector3 scale)
: m_FrameRate(frameRate)
, m_Scale_Sample(scale)
{

}
