/*
 * SceneObjectMoveTo.cpp
 *
 *  Created on: Dec 9, 2014
 *      Author: practicing01
 */

#include "ProcessUtils.h"
#include "SceneObjectMoveTo.h"
#include "Scene.h"
#include "SceneEvents.h"

#include "DebugNew.h"
#include "Log.h"

SceneObjectMoveTo::SceneObjectMoveTo(Context* context) :
		LogicComponent(context)
{
	// Only the scene update event is needed: unsubscribe from the rest for optimization
	SetUpdateEventMask(USE_UPDATE);
}

void SceneObjectMoveTo::OnMoveToComplete()
{
	//OnMoveToCompleteCallback(node_);
	SendEvent(E_MOVETOCOMPLETE);
}

void SceneObjectMoveTo::MoveTo(Vector3 dest, float speed, bool stopOnCompletion)
{
	moveToSpeed_ = speed;
	moveToDest_ = dest;
	moveToLoc_ = node_->GetWorldPosition();
	moveToDir_ = dest - moveToLoc_;
	moveToDir_.Normalize();
	moveToTravelTime_ = (moveToDest_ - moveToLoc_).Length() / speed;
	moveToElapsedTime_ = 0;
	moveToStopOnTime_ = stopOnCompletion;
	isMoving_ = true;
}

void SceneObjectMoveTo::Update(float timeStep)
{
	// Components have their scene node as a member variable for convenient access.
	if (isMoving_ == true)
	{
		inderp_ = timeStep * moveToSpeed_;
		remainingDist_ = (node_->GetWorldPosition() - moveToDest_).Length();
		node_->SetWorldPosition(node_->GetWorldPosition().Lerp(moveToDest_, inderp_ / remainingDist_));
		moveToElapsedTime_ += timeStep;
		if (moveToElapsedTime_ >= moveToTravelTime_)
		{
			isMoving_ = false;
			if (moveToStopOnTime_ == true)
			{
			}
			OnMoveToComplete();
		}
		//LOGERRORF("%f %f",moveToTravelTime_,moveToElapsedTime_);
	}
}
