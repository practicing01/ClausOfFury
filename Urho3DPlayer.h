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

#pragma once

#include "Application.h"
#include "LoopInterface.h"

#include "CollisionShape.h"
#include "Constraint.h"

namespace Urho3D
{

class Node;
class Scene;

}

using namespace Urho3D;

/// Urho3DPlayer application runs a script specified on the command line.
class Urho3DPlayer : public Application, LoopInterface
{
	OBJECT(Urho3DPlayer);

public:
	/// Construct.
	Urho3DPlayer(Context* context);

	/// Setup before engine initialization. Verify that a script file has been specified.
	virtual void Setup();
	/// Setup after engine initialization. Load the script and execute its start function.
	virtual void Start();
	/// Cleanup after the main loop. Run the script's stop function if it exists.
	virtual void Stop();
	void Loop(float timeStep);
	SharedPtr<Node> heliNode_;
	SharedPtr<Node> claus_;
	SharedPtr<Node> chimney_;
	SharedPtr<Text> text_;
	float heliSpeed_;
	float heliXSpawn_;
	Vector<SharedPtr<Node> > victor_;
	int maxHouseWidth_;
	int maxHouseHeight_;
	bool clausCollided_;
	float friction_;
	bool resetClaus_;
	float elapsedTime_;
	float resetDelay_;
	float color_;
	int score_;
	int splashPhase_;
	float elapsedSplashTime_;
	void (*pLoop)(Urho3DPlayer* gameState, float timeStep);

//protected:
	SharedPtr<Viewport> viewport_;
	/// Scene.
	SharedPtr<Scene> scene_;
	/// Camera scene node.
	SharedPtr<Node> cameraNode_;
	/// Camera yaw angle.
	float yaw_;
	/// Camera pitch angle.
	float pitch_;
	RigidBody* pelvis_;

//private:
	/// Load ass.
	void AssLoad();
	/// Subscribe to application-wide logic update events.
	void SubscribeToEvents();
	/// Construct the scene content.
	void CreateScene();
	/// Set up a viewport for displaying the scene.
	void SetupViewport();
	/// Read input and moves the camera.
	void MoveCamera(float timeStep);
	/// Handle the logic update event.
	void HandleUpdate(StringHash eventType, VariantMap& eventData);
	/// Handle the post-render update event.
	void HandlePostRenderUpdate(StringHash eventType, VariantMap& eventData);
	/// Make a bone physical by adding RigidBody and CollisionShape components.
	void CreateRagdollBone(Node* noed, const String& boneName, ShapeType type, const Vector3& size, const Vector3& position, const Quaternion& rotation);
	/// Join two bones with a Constraint component.
	void CreateRagdollConstraint(Node* noed, const String& boneName, const String& parentName, ConstraintType type, const Vector3& axis, const Vector3& parentAxis, const Vector2& highLimit, const Vector2& lowLimit, bool disableCollision = false);
	void CreateBoxes();
	void SpawnHouse();
	void ResetHeli(StringHash eventType, VariantMap& eventData);
	void TouchDown(StringHash eventType, VariantMap& eventData);
	void ToggleRigidBodies(Node* noed, bool enable);
	/// Handle scene node's physics collision.
	void HandleNodeCollision(StringHash eventType, VariantMap& eventData);
	void ResetClaus();
	void SpawnClaus();
	void ResetScheduler(float timeStep);
};
