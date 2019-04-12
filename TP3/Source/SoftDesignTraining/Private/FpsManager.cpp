// Fill out your copyright notice in the Description page of Project Settings.

#include "SoftDesignTraining.h"
#include "FpsManager.h"

FpsManager::FpsManager()
{
}

FpsManager::~FpsManager()
{
}

FpsManager* FpsManager::GetInstance()
{
	if (!m_Instance)
	{
		m_Instance = new  FpsManager();
	}

	return m_Instance;
}

bool FpsManager::canExecute(uint8 taskId) {
	if (GFrameCounter != currentFrame) {
		currentFrame = GFrameCounter;
		timeLeft = 0.033f;
	}

	switch (taskId) {
		case 0:

			break;
		case 1:
			break;
		case 2:
			break;
	}

}
