// Fill out your copyright notice in the Description page of Project Settings.

#include "SoftDesignTraining.h"
#include "FpsManager.h"
#include "SDTUtils.h"

FpsManager* FpsManager::m_FPS_Instance;


FpsManager::FpsManager()
{
}


void FpsManager::Destroy()
{
	delete m_FPS_Instance;
	m_FPS_Instance = nullptr;
}

FpsManager* FpsManager::GetInstance()
{
	if (!m_FPS_Instance)
	{
		m_FPS_Instance = new  FpsManager();
	}
	return m_FPS_Instance;
}

bool FpsManager::canExecute() {

	if (GFrameCounter != currentFrame) {
		currentFrame = GFrameCounter;
		timeLeft = BUDGET;
	}

	if (timeLeft - AVERAGE_EXEC_TIME > 0) {
		timeLeft -= AVERAGE_EXEC_TIME;
		return true;
	}
	return false;

}
