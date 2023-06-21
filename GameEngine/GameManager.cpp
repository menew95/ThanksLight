#include "pch.h"
#include "GameManager.h"

TLGameEngine::GameManager::GameManager()
{

}

TLGameEngine::GameManager::~GameManager()
{

}

void TLGameEngine::GameManager::InitSetting()
{
	m_keybordInput = true;
	m_canConvert2D = true;
	m_canConvert3D = true;
	m_gamePause = false;
}