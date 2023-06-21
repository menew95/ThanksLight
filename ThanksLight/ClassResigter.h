#pragma once
#include "Factory.h"

#include "AudioSource.h"
#include "BoxCollider.h"
#include "Button.h"
#include "ButtonTrigger.h"
#include "ButtonTrigger2.h"
#include "Camera.h"
#include "CameraController.h"
#include "Canvas.h"
#include "CapsuleCollider.h"
#include "Door.h"
#include "DoorTrigger.h"
#include "DoorTriggerManager.h"
#include "EndingEvent.h"
#include "FileID.h"
#include "FillTrigger.h"
#include "FlashLight.h"
#include "GameObject.h"
#include "Image.h"
#include "InteractiveObject.h"
#include "Level10Event.h"
#include "Level11Event.h"
#include "Level2Event.h"
#include "Level3Event.h"
#include "Level5ButtonTrigger.h"
#include "Level6DoorEvent.h"
#include "Level7Event.h"
#include "Level8ButtonTrigger.h"
#include "Level8Event.h"
#include "Level9Event.h"
#include "Light.h"
#include "MainMenuProcess.h"
#include "MeshCollider.h"
#include "MeshFilter.h"
#include "MeshRenderer.h"
#include "Narration.h"
#include "NarrationTrigger.h"
#include "NextSceneTrigger.h"
#include "OutLine.h"
#include "PlayerController.h"
#include "PostProcess.h"
#include "ProjectedCtrlTest.h"
#include "ProjectedRenderer.h"
#include "ProjectorObject.h"
#include "PrologueEvent.h"
#include "RectTransform.h"
#include "Rigidbody.h"
#include "SceneLoader.h"
#include "SphereCollider.h"
#include "TableSnapTrigger.h"
#include "Text.h"
#include "Transform.h"
#include "TranslateTrigger.h"

namespace ClassFactory {
using namespace TLGameEngine;
void RegistClasses() {
	RegistFactoryFromJson(AudioSource); 
	RegistFactoryFromJson(BoxCollider); 
	RegistFactoryFromJson(Button); 
	RegistFactoryFromJson(ButtonTrigger); 
	RegistFactoryFromJson(ButtonTrigger2); 
	RegistFactoryFromJson(Camera); 
	RegistFactoryFromJson(CameraController); 
	RegistFactoryFromJson(Canvas); 
	RegistFactoryFromJson(CapsuleCollider); 
	RegistFactoryFromJson(Door); 
	RegistFactoryFromJson(DoorTrigger); 
	RegistFactoryFromJson(DoorTriggerManager); 
	RegistFactoryFromJson(EndingEvent); 
	RegistFactoryFromJson(FileID); 
	RegistFactoryFromJson(FillTrigger); 
	RegistFactoryFromJson(FlashLight); 
	RegistFactoryFromJson(GameObject); 
	RegistFactoryFromJson(Image); 
	RegistFactoryFromJson(InteractiveObject); 
	RegistFactoryFromJson(Level10Event); 
	RegistFactoryFromJson(Level11Event); 
	RegistFactoryFromJson(Level2Event); 
	RegistFactoryFromJson(Level3Event); 
	RegistFactoryFromJson(Level5ButtonTrigger); 
	RegistFactoryFromJson(Level6DoorEvent); 
	RegistFactoryFromJson(Level7Event); 
	RegistFactoryFromJson(Level8ButtonTrigger); 
	RegistFactoryFromJson(Level8Event); 
	RegistFactoryFromJson(Level9Event); 
	RegistFactoryFromJson(Light); 
	RegistFactoryFromJson(MainMenuProcess); 
	RegistFactoryFromJson(MeshCollider); 
	RegistFactoryFromJson(MeshFilter); 
	RegistFactoryFromJson(MeshRenderer); 
	RegistFactoryFromJson(Narration); 
	RegistFactoryFromJson(NarrationTrigger); 
	RegistFactoryFromJson(NextSceneTrigger); 
	RegistFactoryFromJson(OutLine); 
	RegistFactoryFromJson(PlayerController); 
	RegistFactoryFromJson(PostProcess); 
	RegistFactoryFromJson(ProjectedCtrlTest); 
	RegistFactoryFromJson(ProjectedRenderer); 
	RegistFactoryFromJson(ProjectorObject); 
	RegistFactoryFromJson(PrologueEvent); 
	RegistFactoryFromJson(RectTransform); 
	RegistFactoryFromJson(Rigidbody); 
	RegistFactoryFromJson(SceneLoader); 
	RegistFactoryFromJson(SphereCollider); 
	RegistFactoryFromJson(TableSnapTrigger); 
	RegistFactoryFromJson(Text); 
	RegistFactoryFromJson(Transform); 
	RegistFactoryFromJson(TranslateTrigger); 
}}
