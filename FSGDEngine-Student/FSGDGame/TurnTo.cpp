#include <list>
#include "TurnTo.h"
#include "../EDGameCore/GameObject.h"
#include "../EDGameCore/Transform.h"
#include "../EDGameCore/RigidBody.h"
#include "../EDGameCore/Collider.h"
#include "../EDGameCore/Game.h"
#include "../EDGameCore/GameObject.h"
#include "WorldData.h"

using namespace std;
using namespace EDGameCore;

TurnTo::TurnTo()
{
	target = nullptr;
	targetId = 0;
	turn_speed = 0.0f;
}

void TurnTo::LateUpdate()
{
	Transform* looker_transform = GetGameObject()->GetTransform();
	Transform* target_transform = nullptr;

	// Finds the target based on its instance id each update 
	// to avoid having a dangling pointer to an object that might have been destroyed.
	target = GameObject::GetGameObjectInstance(targetId);
	Float4x4 helper = looker_transform->GetLocalMatrix();
	// TODO-STUDENT - comment this out and write your own solution
	//TurnToSolution();
	target_transform = target->GetGameObject()->GetTransform();
	Float3 toTargetVector = target_transform->GetWorldMatrix().WAxis - looker_transform->GetWorldMatrix().WAxis;
	toTargetVector = toTargetVector.normalize();

	float XSpace = DotProduct(toTargetVector, looker_transform->GetWorldMatrix().XAxis);

	helper.rotateLocalY(-XSpace* WorldData::GetDeltaTime());

	float YSpace = DotProduct(toTargetVector, looker_transform->GetWorldMatrix().YAxis);

	helper.rotateLocalX(YSpace * WorldData::GetDeltaTime());

	//lookat

	CrossProduct(helper.XAxis, Float3(0.0f, 1.0f, 0.0f), helper.ZAxis);
	helper.XAxis = helper.XAxis.normalize();

	CrossProduct(helper.YAxis, helper.ZAxis, helper.XAxis);
	helper.YAxis = helper.YAxis.normalize();

	looker_transform->SetLocalMatrix(helper);
}

void TurnTo::LoadState(TiXmlElement* xmlElement)
{
	// Get the name of the target to look-at
	TiXmlElement* targetEle = xmlElement->FirstChildElement("TargetName");
	if( targetEle != 0 &&
		targetEle->GetText() != 0 &&
		strlen( targetEle->GetText() ) != 0 )
		targetName = targetEle->GetText();

	targetEle = xmlElement->FirstChildElement("TurnSpeed");
	if( targetEle != 0 &&
		targetEle->GetText() != 0 &&
		strlen( targetEle->GetText() ) != 0 )
		turn_speed = static_cast<float>(std::atof(targetEle->GetText()));
}

void TurnTo::Awake(void)
{
	// Find the first GameObject named targetName.
	GameObject* obj = GameObject::Find( targetName.c_str() );

	if( obj != 0 )
		targetId = obj->GetInstanceId();
}
