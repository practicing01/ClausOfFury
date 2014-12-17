//
// Copyright (c) 2008-2014 the Urho3D project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

/*******************************************************************
 *
 *
 *
 * This app was quickly and carelessly hacked together to learn
 * both c++ and urho3d. ~practicing01
 *
 *
 *
 */

#include "AnimatedModel.h"
#include "Camera.h"
#include "CollisionShape.h"
#include "CoreEvents.h"
#include "DebugRenderer.h"
#include "Engine.h"
#include "Font.h"
#include "Graphics.h"
#include "Input.h"
#include "Light.h"
#include "Log.h"
#include "Main.h"
#include "Material.h"
#include "Model.h"
#include "Node.h"
#include "ObjectAnimation.h"
#include "Octree.h"
#include "PhysicsEvents.h"
#include "PhysicsWorld.h"
#include "ProcessUtils.h"
#include "Renderer.h"
#include "ResourceCache.h"
#include "ResourceEvents.h"
#include "RigidBody.h"
#include "Scene.h"
#include "SceneObjectMoveTo.h"
#include "Sprite.h"
#include "Text.h"
#include "Texture2D.h"
#include "UI.h"
#include "Zone.h"
#include "ValueAnimation.h"
#include "Viewport.h"

#include "Urho3DPlayer.h"

#include "DebugNew.h"
/*
void ResetHeli(Node* neod)
{
	neod->SetWorldPosition(Vector3(-10.0f, 19.0f, 10.0f));
}
 */

void DummyLoop(Urho3DPlayer* gameState, float timeStep)
{
	// Move the camera, scale movement with time step
	gameState->MoveCamera(timeStep);
	gameState->ResetScheduler(timeStep);
}

void HackSplash(Urho3DPlayer* gameState, float timeStep)
{
	if (gameState->elapsedSplashTime_ == 0.0f)//hacks, hacks everywhere
	{
		gameState->scene_ = new Scene(gameState->GetContext());
		gameState->scene_->CreateComponent<Octree>();

		gameState->cameraNode_ = new Node(gameState->GetContext());
		Camera* camera = gameState->cameraNode_->CreateComponent<Camera>();
		camera->SetOrthographic(true);
		camera->SetAspectRatio(1.67f);
		camera->SetFov(45.0f);
		//camera->SetOrthoSize(Vector2(12.5f, 7.5f));
		camera->SetFarClip(300.0f);

		// Set an initial position for the camera scene node above the floor
		gameState->cameraNode_->SetPosition(Vector3(0.0f, 10.0f, 0.0f));
		gameState->cameraNode_->LookAt(Vector3(0.0f, 10.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f));

		Renderer* renderer = gameState->GetSubsystem<Renderer>();
		Graphics* graphics = gameState->GetSubsystem<Graphics>();
		UI* ui = gameState->GetSubsystem<UI>();

		// Set up a viewport to the Renderer subsystem so that the 3D scene can be seen
		gameState->viewport_ = new Viewport(gameState->GetContext(), gameState->scene_, gameState->cameraNode_->GetComponent<Camera>());
		renderer->SetViewport(0, gameState->viewport_);

		ResourceCache* cache = gameState->GetSubsystem<ResourceCache>();

		Texture2D* decalTex = cache->GetResource<Texture2D>("Textures/fich.png");
		SharedPtr<Sprite> sprite(new Sprite(gameState->GetContext()));
		sprite->SetTexture(decalTex);
		sprite->SetPosition(Vector2(((float)graphics->GetWidth())*0.5f, ((float)graphics->GetHeight())*0.5f));
        sprite->SetSize(IntVector2(128, 128));
        sprite->SetHotSpot(IntVector2(64, 64));
        sprite->SetColor(Color(Random(0.5f) + 0.5f, Random(0.5f) + 0.5f, Random(0.5f) + 0.5f));
        ui->GetRoot()->AddChild(sprite);

		Texture2D* mdsdecalTex = cache->GetResource<Texture2D>("Textures/mds.png");
		SharedPtr<Sprite> mdssprite(new Sprite(gameState->GetContext()));
		mdssprite->SetTexture(mdsdecalTex);
		mdssprite->SetPosition(Vector2(128.0f, ((float)graphics->GetHeight())*0.5f));
		mdssprite->SetSize(IntVector2(256, 256));
		mdssprite->SetHotSpot(IntVector2(128, 128));
		ui->GetRoot()->AddChild(mdssprite);

		Texture2D* ogadecalTex = cache->GetResource<Texture2D>("Textures/oga.png");
		SharedPtr<Sprite> ogasprite(new Sprite(gameState->GetContext()));
		ogasprite->SetTexture(ogadecalTex);
		ogasprite->SetPosition(Vector2(((float)graphics->GetWidth())-128.0f, ((float)graphics->GetHeight())*0.5f));
		ogasprite->SetSize(IntVector2(256, 256));
		ogasprite->SetHotSpot(IntVector2(128, 128));
		ui->GetRoot()->AddChild(ogasprite);

		Text* text_ = new Text(gameState->GetContext());
		text_->SetText("Urho3D");
	    // Set font and text color
		text_->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 12);
		text_->SetColor(Color(0.0f, 1.0f, 0.5f));
		text_->SetPosition(0,(graphics->GetHeight()/2)+64);
	    // Align Text center-screen
		text_->SetHorizontalAlignment(HA_CENTER);
		text_->SetVerticalAlignment(VA_TOP);
	    // Add Text instance to the UI root element
		gameState->GetSubsystem<UI>()->GetRoot()->AddChild(text_);


		Text* mdstext_ = new Text(gameState->GetContext());
		mdstext_->SetText("MourningDoveSoft");
	    // Set font and text color
		mdstext_->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 12);
		mdstext_->SetColor(Color(0.0f, 0.5f, 0.5f));
		mdstext_->SetPosition(0,(graphics->GetHeight()/2)+128);
	    // Align Text center-screen
		mdstext_->SetHorizontalAlignment(HA_LEFT);
		mdstext_->SetVerticalAlignment(VA_TOP);
	    // Add Text instance to the UI root element
		gameState->GetSubsystem<UI>()->GetRoot()->AddChild(mdstext_);

	}

	gameState->elapsedSplashTime_ += timeStep;

	if (gameState->elapsedSplashTime_ >= 2.0f)
	{
		/*gameState->cameraNode_->RemoveAllChildren();
		gameState->cameraNode_->RemoveAllComponents();
		gameState->cameraNode_->Remove();
		gameState->scene_->RemoveAllChildren();
		gameState->scene_->RemoveAllComponents();
		gameState->scene_->Remove();*/
		gameState->CreateScene();
		gameState->SetupViewport();
		gameState->pLoop = DummyLoop;
	}

}

DEFINE_APPLICATION_MAIN(Urho3DPlayer);

Urho3DPlayer::Urho3DPlayer(Context* context) :
								Application(context)
{
	// Register an object factory for our custom SceneObjectMoveTo component so that we can attach them to scene nodes.
	context->RegisterFactory<SceneObjectMoveTo>();
	heliSpeed_ = 10.0f;
	maxHouseWidth_ = 5;
	maxHouseHeight_ = 10;
	clausCollided_ = false;
	friction_ = 1.0f;
	resetClaus_ = false;
	claus_ = NULL;
	heliXSpawn_ = -25.0f;
	color_ = 0.0f;
	score_ = 0;
	splashPhase_ = 0;
	elapsedSplashTime_ = 0.0f;
	pLoop = HackSplash;
}

void Urho3DPlayer::Setup()
{
	engineParameters_["WindowWidth"] = 800;
	engineParameters_["WindowHeight"] = 480;
	engineParameters_["FullScreen"] = false;
	engineParameters_["VSync"] = true;
	engineParameters_["TextureFilterMode"] = 1;
}

void Urho3DPlayer::Start()
{
	Application::Start();
	SetRandomSeed(GetSubsystem<Time>()->GetSystemTime());
	AssLoad();
	SubscribeToEvents();
}

void Urho3DPlayer::Stop()
{
}

void Urho3DPlayer::AssLoad()
{
}

void Urho3DPlayer::SetupViewport()
{
	Renderer* renderer = GetSubsystem<Renderer>();

	// Set up a viewport to the Renderer subsystem so that the 3D scene can be seen
	viewport_ = new Viewport(context_, scene_, cameraNode_->GetComponent<Camera>());
	renderer->SetViewport(0, viewport_);

	Input* input = GetSubsystem<Input>();
	input->SetMouseVisible(true);
	input->SetTouchEmulation(true);
}

void Urho3DPlayer::CreateScene()
{

	ResourceCache* cache = GetSubsystem<ResourceCache>();

	//scene_ = new Scene(context_);

	// Create octree, use default volume (-1000, -1000, -1000) to (1000, 1000, 1000)
	// Create a physics simulation world with default parameters, which will update at 60fps. Like the Octree must
	// exist before creating drawable components, the PhysicsWorld must exist before creating physics components.
	// Finally, create a DebugRenderer component so that we can draw physics debug geometry
	//scene_->CreateComponent<Octree>();
	scene_->RemoveAllChildren();
	scene_->CreateComponent<PhysicsWorld>();
	scene_->CreateComponent<DebugRenderer>();

	// Create a Zone component for ambient lighting & fog control
	Node* zoneNode = scene_->CreateChild("Zone");
	Zone* zone = zoneNode->CreateComponent<Zone>();
	zone->SetBoundingBox(BoundingBox(-1000.0f, 1000.0f));
	//zone->SetAmbientColor(Color(0.15f, 0.15f, 0.15f));
	zone->SetAmbientColor(Color(0.0f, 0.0f, 0.0f));
	//zone->SetFogColor(Color(0.5f, 0.5f, 0.7f));
	zone->SetFogColor(Color(0.0f, 0.0f, 0.0f));
	zone->SetFogStart(100.0f);
	zone->SetFogEnd(300.0f);

	// Create the camera. Limit far clip distance to match the fog. Note: now we actually create the camera node outside
	// the scene, because we want it to be unaffected by scene load / save
	/*cameraNode_ = new Node(context_);
	Camera* camera = cameraNode_->CreateComponent<Camera>();
	camera->SetOrthographic(true);
	camera->SetAspectRatio(1.67f);
	camera->SetFov(45.0f);
	//camera->SetOrthoSize(Vector2(12.5f, 7.5f));
	camera->SetFarClip(300.0f);*/

	// Set an initial position for the camera scene node above the floor
	cameraNode_->SetPosition(Vector3(0.0f, 10.0f, 0.0f));
	cameraNode_->LookAt(Vector3(0.0f, 10.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f));

	// Create a directional light to the world. Enable cascaded shadows on it
	Node* lightNode = scene_->CreateChild("DirectionalLight");
	lightNode->SetDirection(Vector3(-1.0f, 0.0f, 0.0f));
	//lightNode->SetDirection(Vector3(0.5f, -1.0f, 0.5f));
	Light* light = lightNode->CreateComponent<Light>();
	light->SetLightType(LIGHT_DIRECTIONAL);
	light->SetColor(Color(0.8f, 0.8f, 0.8f));
	//light->SetCastShadows(true);
	//light->SetShadowBias(BiasParameters(0.00025f, 0.5f));
	// Set cascade splits at 10, 50 and 200 world units, fade shadows out at 80% of maximum shadow distance
	//light->SetShadowCascade(CascadeParameters(10.0f, 50.0f, 200.0f, 0.0f, 0.8f));

	{
		// Create a floor object, 500 x 500 world units. Adjust position so that the ground is at zero Y
		Node* floorNode = scene_->CreateChild("Floor");
		floorNode->SetPosition(Vector3(0.0f, -0.5f, 0.0f));
		floorNode->SetScale(Vector3(500.0f, 1.0f, 500.0f));
		StaticModel* floorObject = floorNode->CreateComponent<StaticModel>();
		floorObject->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
		floorObject->SetMaterial(
				cache->GetResource<Material>("Materials/StoneTiled.xml"));

		// Make the floor physical by adding RigidBody and CollisionShape components
		RigidBody* body = floorNode->CreateComponent<RigidBody>();
		// We will be spawning spherical objects in this sample. The ground also needs non-zero rolling friction so that
		// the spheres will eventually come to rest
		body->SetRollingFriction(friction_);
		CollisionShape* shape = floorNode->CreateComponent<CollisionShape>();
		// Set a box shape of size 1 x 1 x 1 for collision. The shape will be scaled with the scene node scale, so the
		// rendering and physics representation sizes should match (the box model is also 1 x 1 x 1.)
		shape->SetBox(Vector3::ONE);
	}

	heliNode_ = scene_->CreateChild("helicopter");
	heliNode_->SetScale(0.5f);
	heliNode_->SetPosition(Vector3(-20.0f, 19.0f, 10.0f));
	StaticModel* heliObject = heliNode_->CreateComponent<StaticModel>();
	heliObject->SetModel(cache->GetResource<Model>("Models/Helicopter.mdl"));
	heliObject->ApplyMaterialList("Models/Helicopter.txt");
	SceneObjectMoveTo* heliSceneObject = heliNode_->CreateComponent<SceneObjectMoveTo>();
	//heliSceneObject->OnMoveToCompleteCallback = ResetHeli;
	SubscribeToEvent(E_MOVETOCOMPLETE, HANDLER(Urho3DPlayer, ResetHeli));
	heliSceneObject->MoveTo(Vector3(15.0f, 19.0f, 10.0f), heliSpeed_, true);

	SpawnClaus();

	SpawnHouse();
	//CreateBoxes();

	text_ = new Text(context_);
	text_->SetText("Children Saved: " + String(score_));
    // Set font and text color
	text_->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 12);
	text_->SetColor(Color(0.0f, 0.5f, 0.5f));

	Graphics* graphics = GetSubsystem<Graphics>();
	text_->SetPosition(0,graphics->GetHeight()-15);

    // Align Text center-screen
	text_->SetHorizontalAlignment(HA_LEFT);
	text_->SetVerticalAlignment(VA_TOP);

	GetSubsystem<UI>()->GetRoot()->RemoveAllChildren();
    // Add Text instance to the UI root element
    GetSubsystem<UI>()->GetRoot()->AddChild(text_);

    // Create a point light to the world so that we can see something.
    Node* pointLightNode = scene_->CreateChild("PointLight");
    Light* pointlight = pointLightNode->CreateComponent<Light>();
    pointlight->SetLightType(LIGHT_POINT);//LIGHT_POINT LIGHT_SPOT
    pointlight->SetRange(1000.0f);

    pointLightNode->SetPosition(Vector3(0.0f, 5.0f, 5.0f));
    pointLightNode->LookAt(Vector3(0.0f, 5.0f, 10.0f));
    //pointLightNode->SetDirection(Vector3(Sin(0.0f), -1.5f, Cos(0.0f)));

    // Create light animation
    SharedPtr<ObjectAnimation> lightAnimation(new ObjectAnimation(context_));

    // Create light color animation
    SharedPtr<ValueAnimation> colorAnimation(new ValueAnimation(context_));
    colorAnimation->SetKeyFrame(0.0f, Color::WHITE);
    colorAnimation->SetKeyFrame(1.0f, Color::RED);
    colorAnimation->SetKeyFrame(2.0f, Color::YELLOW);
    colorAnimation->SetKeyFrame(3.0f, Color::GREEN);
    colorAnimation->SetKeyFrame(4.0f, Color::WHITE);
    // Set Light component's color animation
    lightAnimation->AddAttributeAnimation("@Light/Color", colorAnimation);

    // Apply light animation to light node
    pointLightNode->SetObjectAnimation(lightAnimation);

}

void Urho3DPlayer::MoveCamera(float timeStep)
{
	// Do not move if the UI has a focused element (the console)
	//if (GetSubsystem<UI>()->GetFocusElement())
		//return;

	Input* input = GetSubsystem<Input>();
	/*
	 // Movement speed as world units per second
	 const float MOVE_SPEED = 5.0f;
	 // Mouse sensitivity as degrees per pixel
	 const float MOUSE_SENSITIVITY = 0.1f;

	 // Use this frame's mouse motion to adjust camera node yaw and pitch. Clamp the pitch between -90 and 90 degrees
	 IntVector2 mouseMove = input->GetMouseMove();
	 yaw_ += MOUSE_SENSITIVITY * mouseMove.x_;
	 pitch_ += MOUSE_SENSITIVITY * mouseMove.y_;
	 pitch_ = Clamp(pitch_, -90.0f, 90.0f);

	 // Construct new orientation for the camera scene node from yaw and pitch. Roll is fixed to zero
	 cameraNode_->SetRotation(Quaternion(pitch_, yaw_, 0.0f));

	 // Read WASD keys and move the camera scene node to the corresponding direction if they are pressed
	 if (input->GetKeyDown('W'))
	 cameraNode_->Translate(Vector3::FORWARD * MOVE_SPEED * timeStep);
	 if (input->GetKeyDown('S'))
	 cameraNode_->Translate(Vector3::BACK * MOVE_SPEED * timeStep);
	 if (input->GetKeyDown('A'))
	 cameraNode_->Translate(Vector3::LEFT * MOVE_SPEED * timeStep);
	 if (input->GetKeyDown('D'))
	 cameraNode_->Translate(Vector3::RIGHT * MOVE_SPEED * timeStep);*/

	if (input->GetKeyDown('Q'))
		engine_->Exit();

}

void Urho3DPlayer::SubscribeToEvents()
{
	// Subscribe HandleUpdate() function for processing update events
	SubscribeToEvent(E_UPDATE, HANDLER(Urho3DPlayer, HandleUpdate));

	SubscribeToEvent(E_TOUCHBEGIN, HANDLER(Urho3DPlayer, TouchDown));

	// Subscribe HandlePostRenderUpdate() function for processing the post-render update event, during which we request
	// debug geometry
	//SubscribeToEvent(E_POSTRENDERUPDATE,
	//	HANDLER(Urho3DPlayer, HandlePostRenderUpdate));
}

void Urho3DPlayer::HandleUpdate(StringHash eventType, VariantMap& eventData)
{
	using namespace Update;

	// Take the frame time step, which is stored as a float
	float timeStep = eventData[P_TIMESTEP].GetFloat();

	pLoop(this,timeStep);
}

void Urho3DPlayer::HandlePostRenderUpdate(StringHash eventType,
		VariantMap& eventData)
{
	//scene_->GetComponent<PhysicsWorld>()->DrawDebugGeometry(true);
}

void Urho3DPlayer::CreateRagdollBone(Node* noed, const String& boneName,
		ShapeType type, const Vector3& size, const Vector3& position,
		const Quaternion& rotation)
{
	// Find the correct child scene node recursively
	Node* boneNode = noed->GetChild(boneName, true);
	if (!boneNode)
	{
		LOGWARNING(
				"Could not find bone " + boneName + " for creating ragdoll physics components");
		return;
	}

	RigidBody* body = boneNode->CreateComponent<RigidBody>();
	if (boneName == "pelvis")
	{
		pelvis_ = body;
	}
	// Set mass to make movable
	body->SetMass(1.0f);
	// Set damping parameters to smooth out the motion
	body->SetLinearDamping(0.05f);
	body->SetAngularDamping(0.85f);
	// Set rest thresholds to ensure the ragdoll rigid bodies come to rest to not consume CPU endlessly
	body->SetLinearRestThreshold(1.5f);
	body->SetAngularRestThreshold(2.5f);

	//body->SetUseGravity(false);

	CollisionShape* shape = boneNode->CreateComponent<CollisionShape>();
	// We use either a box or a capsule shape for all of the bones
	if (type == SHAPE_BOX)
		//shape->SetBox(size, position, rotation);
		shape->SetBox(size, boneNode->GetPosition(), rotation);
	else
		//shape->SetCapsule(size.x_, size.y_, position, rotation);
		shape->SetCapsule(size.x_, size.y_, boneNode->GetPosition(), rotation);

	SubscribeToEvent(boneNode, E_NODECOLLISION, HANDLER(Urho3DPlayer, HandleNodeCollision));
}

void Urho3DPlayer::CreateRagdollConstraint(Node* noed, const String& boneName,
		const String& parentName, ConstraintType type, const Vector3& axis,
		const Vector3& parentAxis, const Vector2& highLimit,
		const Vector2& lowLimit, bool disableCollision)
{
	Node* boneNode = noed->GetChild(boneName, true);
	Node* parentNode = noed->GetChild(parentName, true);
	if (!boneNode)
	{
		LOGWARNING(
				"Could not find bone " + boneName + " for creating ragdoll constraint");
		return;
	}
	if (!parentNode)
	{
		LOGWARNING(
				"Could not find bone " + parentName + " for creating ragdoll constraint");
		return;
	}

	Constraint* constraint = boneNode->CreateComponent<Constraint>();
	constraint->SetConstraintType(type);
	// Most of the constraints in the ragdoll will work better when the connected bodies don't collide against each other
	constraint->SetDisableCollision(disableCollision);
	// The connected body must be specified before setting the world position
	constraint->SetOtherBody(parentNode->GetComponent<RigidBody>());
	// Position the constraint at the child bone we are connecting
	constraint->SetWorldPosition(boneNode->GetWorldPosition());
	// Configure axes and limits
	constraint->SetAxis(axis);
	constraint->SetOtherAxis(parentAxis);
	constraint->SetHighLimit(highLimit);
	constraint->SetLowLimit(lowLimit);
}

void Urho3DPlayer::CreateBoxes()
{
	ResourceCache* cache = GetSubsystem<ResourceCache>();

	///Left side.
	for (int y = 0; y < maxHouseHeight_; y++)
	{
		for (int x = -maxHouseWidth_; x < maxHouseWidth_+1; x++)
		{
			Node* boxNode = new Node(context_);
			scene_->AddChild(boxNode);
			boxNode->SetPosition( Vector3( (float)x, (float)y + 0.5f, 10.0f) );
			boxNode->SetScale(1.0f);
			StaticModel* boxObject = boxNode->CreateComponent<StaticModel>();
			boxObject->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
			if (x != 0 || y != 0)
			{
				boxObject->SetMaterial(
						cache->GetResource<Material>("Materials/brick.xml"));
			}
			else
			{
				boxObject->SetMaterial(
						cache->GetResource<Material>("Materials/door.xml"));
			}

			//boxObject->SetCastShadows(true);
			RigidBody* body = boxNode->CreateComponent<RigidBody>();
			body->SetMass(0.1f);
			body->SetRollingFriction(friction_);
			CollisionShape* shape = boxNode->CreateComponent<CollisionShape>();
			shape->SetBox(Vector3(1.0f, 1.0f, 1.0f));

			victor_.Push(SharedPtr<Node>(boxNode));
			//boxNode->SetEnabledRecursive(false);

		}
	}

	///Chimney
	chimney_ = scene_->CreateChild("Block");
	chimney_->SetPosition(
			Vector3(0.0f, 10.5f, 10.0f));
	chimney_->SetScale(1.0f);
	StaticModel* chimneyObject = chimney_->CreateComponent<StaticModel>();
	chimneyObject->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
	chimneyObject->SetMaterial(
			cache->GetResource<Material>("Materials/cobblestone.xml"));
	//chimneyObject->SetCastShadows(true);

	RigidBody* chimneybody = chimney_->CreateComponent<RigidBody>();
	chimneybody->SetMass(0.1f);
	chimneybody->SetRollingFriction(friction_);
	CollisionShape* chimneyshape =
			chimney_->CreateComponent<CollisionShape>();
	chimneyshape->SetBox(Vector3(1.0f, 1.0f, 1.0f));
	chimney_->SetEnabledRecursive(false);
}

void Urho3DPlayer::SpawnHouse()
{
	ResourceCache* cache = GetSubsystem<ResourceCache>();

	int leftCount, rightCount, heightCount, chimneySpot;

	leftCount = Random(0, maxHouseWidth_);
	rightCount = Random(0, maxHouseWidth_);
	heightCount = Random(1, maxHouseHeight_);
	chimneySpot = Random(-leftCount, rightCount);
	//LOGERRORF("left: %d right: %d height: %d chim: %d", leftCount, rightCount, heightCount, chimneySpot);

	///Left side.
	for (int y = 1; y < heightCount + 1; y++)
	{
		for (int x = 1; x < leftCount + 1; x++)
		{
			Node* boxNode = scene_->CreateChild("Block");
			boxNode->SetPosition(Vector3((float) (0 - x), ((float) y) - 0.5f, 10.0f));
			boxNode->SetScale(1.0f);
			StaticModel* boxObject = boxNode->CreateComponent<StaticModel>();
			boxObject->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
			boxObject->SetMaterial(
					cache->GetResource<Material>("Materials/brick.xml"));
			//boxObject->SetCastShadows(true);

			RigidBody* body = boxNode->CreateComponent<RigidBody>();
			body->SetMass(0.1f);
			body->SetRollingFriction(friction_);
			CollisionShape* shape = boxNode->CreateComponent<CollisionShape>();
			shape->SetBox(Vector3(1.0f, 1.0f, 1.0f));

			victor_.Push(SharedPtr<Node>(boxNode));
		}
	}

	///Door and middle.
	Node* boxNode = scene_->CreateChild("Block");
	boxNode->SetPosition(Vector3(0.0f, 0.5f, 10.0f));
	boxNode->SetScale(1.0f);
	StaticModel* boxObject = boxNode->CreateComponent<StaticModel>();
	boxObject->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
	boxObject->SetMaterial(cache->GetResource<Material>("Materials/door.xml"));
	//boxObject->SetCastShadows(true);

	RigidBody* body = boxNode->CreateComponent<RigidBody>();
	body->SetMass(0.1f);
	//body->SetRollingFriction(friction_);
	body->SetRollingFriction(friction_);
	CollisionShape* shape = boxNode->CreateComponent<CollisionShape>();
	shape->SetBox(Vector3(1.0f, 1.0f, 1.0f));

	victor_.Push(SharedPtr<Node>(boxNode));

	for (int y = 2; y < heightCount + 1; y++)
	{
		Node* boxNode = scene_->CreateChild("Block");
		boxNode->SetPosition(Vector3(0.0f, ((float) y) - 0.5f, 10.0f));
		boxNode->SetScale(1.0f);
		StaticModel* boxObject = boxNode->CreateComponent<StaticModel>();
		boxObject->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
		boxObject->SetMaterial(
				cache->GetResource<Material>("Materials/brick.xml"));
		//boxObject->SetCastShadows(true);

		RigidBody* body = boxNode->CreateComponent<RigidBody>();
		body->SetMass(0.1f);
		body->SetRollingFriction(friction_);
		CollisionShape* shape = boxNode->CreateComponent<CollisionShape>();
		shape->SetBox(Vector3(1.0f, 1.0f, 1.0f));

		victor_.Push(SharedPtr<Node>(boxNode));
	}

	///Right side.
	for (int y = 1; y < heightCount + 1; y++)
	{
		for (int x = 1; x < rightCount + 1; x++)
		{
			Node* boxNode = scene_->CreateChild("Block");
			boxNode->SetPosition(Vector3((float) x, ((float) y) - 0.5f, 10.0f));
			boxNode->SetScale(1.0f);
			StaticModel* boxObject = boxNode->CreateComponent<StaticModel>();
			boxObject->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
			boxObject->SetMaterial(
					cache->GetResource<Material>("Materials/brick.xml"));
			//boxObject->SetCastShadows(true);

			RigidBody* body = boxNode->CreateComponent<RigidBody>();
			body->SetMass(0.1f);
			body->SetRollingFriction(friction_);
			CollisionShape* shape = boxNode->CreateComponent<CollisionShape>();
			shape->SetBox(Vector3(1.0f, 1.0f, 1.0f));

			victor_.Push(SharedPtr<Node>(boxNode));
		}
	}

	///Chimney
	Node* chimneyNode = scene_->CreateChild("Chimney");
	chimneyNode->SetPosition(
			Vector3((float) chimneySpot, ((float) (heightCount + 1)) - 0.5f, 10.0f));
	chimneyNode->SetScale(1.0f);
	StaticModel* chimneyObject = chimneyNode->CreateComponent<StaticModel>();
	chimneyObject->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
	chimneyObject->SetMaterial(
			cache->GetResource<Material>("Materials/cobblestone.xml"));
	//chimneyObject->SetCastShadows(true);

	RigidBody* chimneybody = chimneyNode->CreateComponent<RigidBody>();
	chimneybody->SetMass(0.1f);
	chimneybody->SetRollingFriction(friction_);
	CollisionShape* chimneyshape =
			chimneyNode->CreateComponent<CollisionShape>();
	chimneyshape->SetBox(Vector3(1.0f, 1.0f, 1.0f));

	victor_.Push(SharedPtr<Node>(chimneyNode));

}

void Urho3DPlayer::ResetHeli(StringHash eventType, VariantMap& eventData)
{
	heliSpeed_ = Random(5, 10);
	heliNode_->SetWorldPosition(Vector3(heliXSpawn_, 19.0f, 10.0f));
	heliNode_->GetComponent<SceneObjectMoveTo>()->MoveTo(Vector3(15.0f, 19.0f, 10.0f), heliSpeed_, true);
}

void Urho3DPlayer::TouchDown(StringHash eventType, VariantMap& eventData)
{

	if (heliNode_->GetNumChildren() != 1)//hackery to check if claus is attached
	{
		return;
	}
	//pelvis_->ApplyImpulse(Vector3(0.0f, 100.0f, 0.0f));
	Vector3 pos = claus_->GetWorldPosition();
	heliNode_->RemoveChild(claus_);
	//heliNode_->RemoveChild(cameraNode_);
	//claus_->AddChild(cameraNode_);
	claus_->SetWorldPosition(pos);
	claus_->SetScale(0.5f);
	ToggleRigidBodies(claus_,true);

}

void Urho3DPlayer::ToggleRigidBodies(Node* noed, bool enable)
{

	for (int x = 0; x < noed->GetNumChildren(); x++)
	{
		if (noed->GetChild(x)->HasComponent<RigidBody>() == true)
		{
			noed->GetChild(x)->GetComponent<RigidBody>()->SetKinematic(!enable);
			noed->GetChild(x)->GetComponent<RigidBody>()->ResetForces();
			//noed->GetChild(x)->GetComponent<RigidBody>()->SetEnabled(enable);//segfault
			//noed->GetChild(x)->GetComponent<RigidBody>()->SetUseGravity(enable);
		}
		ToggleRigidBodies(noed->GetChild(x),enable);
	}

}

void Urho3DPlayer::HandleNodeCollision(StringHash eventType, VariantMap& eventData)
{
	using namespace NodeCollision;

	Node* OtherNode = static_cast<Node*>(eventData[P_OTHERNODE].GetPtr());
	//LOGERRORF("something collided with: %s",OtherNode->GetName().CString());

	if (OtherNode->GetName() == "Chimney")
	{
		if (clausCollided_ == false)
		{
			clausCollided_ = true;
			elapsedTime_ = 0.0f;
			resetDelay_ = 0.0f;
			resetClaus_ = true;
			score_++;
			text_->SetText("Children Saved: " + String(score_));
			return;
		}
	}

	if (OtherNode->GetName() == "Block" || OtherNode->GetName() == "Floor")
	{
		if (clausCollided_ == false)
		{
			clausCollided_ = true;
			elapsedTime_ = 0.0f;
			resetDelay_ = 2.0f;
			resetClaus_ = true;
		}
	}

}

void Urho3DPlayer::ResetClaus()
{
	SpawnClaus();
	clausCollided_ = false;
	return;
	ToggleRigidBodies(claus_,false);
	heliNode_->AddChild(claus_);
	claus_->SetPosition(Vector3(0.0f, -4.0f, 0.0f));
	claus_->SetScale(1.0f);
	clausCollided_ = false;
}

void Urho3DPlayer::SpawnClaus()
{

	ResourceCache* cache = GetSubsystem<ResourceCache>();

	if (claus_ != NULL)
	{
		claus_->RemoveAllChildren();
		claus_->RemoveAllComponents();
		claus_->Remove();
	}

	claus_ = scene_->CreateChild("claus");
	AnimatedModel* modelObject = claus_->CreateComponent<AnimatedModel>();
	modelObject->SetModel(cache->GetResource<Model>("Models/claus.mdl"));
	modelObject->SetMaterial(
			cache->GetResource<Material>("Materials/clausMat.xml"));
	modelObject->SetUpdateInvisible(true);

	// Create RigidBody & CollisionShape components to bones
	CreateRagdollBone(claus_, "pelvis", SHAPE_BOX,
			Vector3(0.25f, 0.10f, 0.25f), Vector3(0.0f, 0.0f, 0.0f),
			Quaternion(0.0f, 0.0f, 0.0f));
	CreateRagdollBone(claus_, "neck", SHAPE_BOX, Vector3(0.5f, 0.5f, 0.5f),
			Vector3(0.0f, 0.0f, 0.0f), Quaternion(0.0f, 0.0f, 0.0f));
	CreateRagdollBone(claus_, "head", SHAPE_BOX, Vector3(1.0f, 1.5f, 1.0f),
			Vector3(0.0f, 0.0f, 0.0f), Quaternion(0.0f, 0.0f, 0.0f));
	CreateRagdollBone(claus_, "thighL", SHAPE_CAPSULE,
			Vector3(0.0625f, 0.1f, 0.0625f), Vector3(0.0f, 0.0f, 0.0f),
			Quaternion(0.0f, 0.0f, 0.0f));
	CreateRagdollBone(claus_, "thighR", SHAPE_CAPSULE,
			Vector3(0.0625f, 0.1f, 0.0625f), Vector3(0.0f, 0.0f, 0.0f),
			Quaternion(0.0f, 0.0f, 0.0f));
	CreateRagdollBone(claus_, "kneeL", SHAPE_CAPSULE,
			Vector3(0.175f, 0.2f, 0.175f), Vector3(0.0f, 0.0f, 0.0f),
			Quaternion(0.0f, 0.0f, 0.0f));
	CreateRagdollBone(claus_, "kneeR", SHAPE_CAPSULE,
			Vector3(0.175f, 0.2f, 0.175f), Vector3(0.0f, 0.0f, 0.0f),
			Quaternion(0.0f, 0.0f, 0.0f));
	CreateRagdollBone(claus_, "footL", SHAPE_CAPSULE,
			Vector3(0.1f, 0.1f, 0.1f), Vector3(0.0f, 0.0f, 0.0f),
			Quaternion(0.0f, 0.0f, 0.0f));
	CreateRagdollBone(claus_, "footR", SHAPE_CAPSULE,
			Vector3(0.1f, 0.1f, 0.1f), Vector3(0.0f, 0.0f, 0.0f),
			Quaternion(0.0f, 0.0f, 0.0f));
	CreateRagdollBone(claus_, "armL", SHAPE_CAPSULE,
			Vector3(0.125f, 0.1f, 0.125f), Vector3(0.0f, 0.0f, 0.0f),
			Quaternion(0.0f, 0.0f, 0.0f));
	CreateRagdollBone(claus_, "armR", SHAPE_CAPSULE,
			Vector3(0.125f, 0.1f, 0.125f), Vector3(0.0f, 0.0f, 0.0f),
			Quaternion(0.0f, 0.0f, 0.0f));
	CreateRagdollBone(claus_, "elbowL", SHAPE_CAPSULE,
			Vector3(0.15f, 0.2f, 0.15f), Vector3(0.0f, 0.0f, 0.0f),
			Quaternion(0.0f, 0.0f, 0.0f));
	CreateRagdollBone(claus_, "elbowR", SHAPE_CAPSULE,
			Vector3(0.15f, 0.2f, 0.15f), Vector3(0.0f, 0.0f, 0.0f),
			Quaternion(0.0f, 0.0f, 0.0f));
	CreateRagdollBone(claus_, "handL", SHAPE_CAPSULE,
			Vector3(0.125f, 0.1f, 0.125f), Vector3(0.0f, 0.0f, 0.0f),
			Quaternion(0.0f, 0.0f, 0.0f));
	CreateRagdollBone(claus_, "handR", SHAPE_CAPSULE,
			Vector3(0.125f, 0.1f, 0.125f), Vector3(0.0f, 0.0f, 0.0f),
			Quaternion(0.0f, 0.0f, 0.0f));

	// Create Constraints between bones
	CreateRagdollConstraint(claus_, "kneeL", "thighL", CONSTRAINT_HINGE,
			Vector3::BACK, Vector3::BACK, Vector2(45.0f, 0.0f), Vector2::ZERO);
	CreateRagdollConstraint(claus_, "kneeR", "thighR", CONSTRAINT_HINGE,
			Vector3::BACK, Vector3::BACK, Vector2(45.0f, 0.0f), Vector2::ZERO);
	CreateRagdollConstraint(claus_, "footL", "kneeL", CONSTRAINT_HINGE,
			Vector3::BACK, Vector3::BACK, Vector2(45.0f, 0.0f), Vector2::ZERO);
	CreateRagdollConstraint(claus_, "footR", "kneeR", CONSTRAINT_HINGE,
			Vector3::BACK, Vector3::BACK, Vector2(45.0f, 0.0f), Vector2::ZERO);
	CreateRagdollConstraint(claus_, "neck", "pelvis", CONSTRAINT_HINGE,
			Vector3::FORWARD, Vector3::FORWARD, Vector2(0.0f, 0.0f),
			Vector2::ZERO);
	CreateRagdollConstraint(claus_, "head", "neck", CONSTRAINT_CONETWIST,
			Vector3::LEFT, Vector3::LEFT, Vector2(0.0f, 45.0f), Vector2::ZERO);
	CreateRagdollConstraint(claus_, "elbowL", "armL", CONSTRAINT_HINGE,
			Vector3::BACK, Vector3::BACK, Vector2(45.0f, 0.0f), Vector2::ZERO);
	CreateRagdollConstraint(claus_, "elbowR", "armR", CONSTRAINT_HINGE,
			Vector3::BACK, Vector3::BACK, Vector2(45.0f, 0.0f), Vector2::ZERO);
	CreateRagdollConstraint(claus_, "handL", "elbowL", CONSTRAINT_HINGE,
			Vector3::BACK, Vector3::BACK, Vector2(45.0f, 0.0f), Vector2::ZERO);
	CreateRagdollConstraint(claus_, "handR", "elbowR", CONSTRAINT_HINGE,
			Vector3::BACK, Vector3::BACK, Vector2(45.0f, 0.0f), Vector2::ZERO);
	CreateRagdollConstraint(claus_, "thighL", "pelvis", CONSTRAINT_POINT,
			Vector3::BACK, Vector3::FORWARD, Vector2(0.0f, 0.0f),
			Vector2::ZERO);
	CreateRagdollConstraint(claus_, "thighR", "pelvis", CONSTRAINT_POINT,
			Vector3::BACK, Vector3::FORWARD, Vector2(0.0f, 0.0f),
			Vector2::ZERO);
	CreateRagdollConstraint(claus_, "armL", "neck", CONSTRAINT_POINT,
			Vector3::DOWN, Vector3::UP, Vector2(0.0f, 0.0f), Vector2::ZERO);
	CreateRagdollConstraint(claus_, "armR", "neck", CONSTRAINT_POINT,
			Vector3::DOWN, Vector3::UP, Vector2(0.0f, 0.0f), Vector2::ZERO);

	ToggleRigidBodies(claus_,false);
	heliNode_->AddChild(claus_);
	claus_->SetPosition(Vector3(0.0f, -4.0f, 0.0f));
}

void Urho3DPlayer::ResetScheduler(float timeStep)
{
	if (resetClaus_ == true)
	{
		elapsedTime_ += timeStep;
		if (elapsedTime_ >= resetDelay_)
		{
			resetClaus_ = false;
			for (int x = 0; x < victor_.Size(); x++)
			{
				Node* noed = victor_[x];
				noed->Remove();
			}
			victor_.Clear();

			ResetClaus();
			SpawnHouse();
		}
	}
}

void Urho3DPlayer::Loop(float timeStep)
{
	pLoop(this, timeStep);
}
