#include "LookAt.h"
#include "../EDGameCore/GameObject.h"
#include "../EDGameCore/Transform.h"
#include "../EDGameCore/RigidBody.h"
#include "../EDGameCore/Collider.h"
#include "../EDGameCore/Game.h"


#include <list>
#include "../EDGameCore/GameObject.h"

using namespace std;
using namespace EDGameCore;

LookAt::LookAt()
{
	target = nullptr;
	targetId = 0;
}

void LookAt::LateUpdate()
{
	Transform* looker_transform = GetGameObject()->GetTransform();
	Transform* target_transform = nullptr;

	// Finds the target based on its instance id each update 
	// to avoid having a dangling pointer
	// to an object that might have been destroyed.
	target = GameObject::GetGameObjectInstance(targetId);
		

	// TODO-STUDENT - comment this out and write your own solution
	//LookAtSolution();
	target_transform = target->GetGameObject()->GetTransform();
	Float3 Zvector = target_transform->GetWorldMatrix().WAxis - looker_transform->GetWorldMatrix().WAxis;
	Zvector = Zvector.normalize();
	
	Float3 Xvector;
	CrossProduct(Xvector, Float3(0.0f, 1.0f, 0.0f), Zvector);
	Xvector = Xvector.normalize();

	Float3 Yvector;
	CrossProduct(Yvector, Zvector, Xvector);
	Yvector = Yvector.normalize();

	Float4x4 helper = helper.makeIdentity();
	helper.XAxis = Xvector;
	helper.YAxis = Yvector;
	helper.ZAxis = Zvector;
	helper.WAxis = looker_transform->GetWorldMatrix().WAxis;

	looker_transform->SetLocalMatrix(helper);
}

void LookAt::LoadState(TiXmlElement* xmlElement)
{
	// Get the name of the target to look-at
	TiXmlElement* targetEle = xmlElement->FirstChildElement("TargetName");
	
	if( targetEle != 0 &&
		targetEle->GetText() != 0 &&
		strlen( targetEle->GetText() ) != 0 )
		targetName = targetEle->GetText();
}

void LookAt::Awake(void)
{
	// Find the first GameObject named targetName.
	GameObject* obj = GameObject::Find( targetName.c_str() );

	if( obj != 0 )
		targetId = obj->GetInstanceId();
}
