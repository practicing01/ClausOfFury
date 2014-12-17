/*
 * LoopInterface.h
 *
 *  Created on: Dec 13, 2014
 *      Author: practicing01
 */

#pragma once

class LoopInterface
{
public:
	virtual ~LoopInterface() {}
	virtual void Loop(float timeStep) = 0;
};
