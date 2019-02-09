// Fill out your copyright notice in the Description page of Project Settings.

#include "SoftDesignTraining.h"
#include "SoftDesignTrainingMainCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

USoundWave* GetSoundWaveFromFile(const FString& filePath)
{
	USoundWave* sw = NewObject<USoundWave>(USoundWave::StaticClass());

	if (!sw)
		return nullptr;

	TArray < uint8 > rawFile;
	FFileHelper::LoadFileToArray(rawFile, filePath.GetCharArray().GetData());

	FWaveModInfo WaveInfo;

	if (WaveInfo.ReadWaveInfo(rawFile.GetData(), rawFile.Num()))
	{
		sw->InvalidateCompressedData();

		sw->RawData.Lock(LOCK_READ_WRITE);
		void* LockedData = sw->RawData.Realloc(rawFile.Num());
		FMemory::Memcpy(LockedData, rawFile.GetData(), rawFile.Num());
		sw->RawData.Unlock();

		int32 DurationDiv = *WaveInfo.pChannels * *WaveInfo.pBitsPerSample * *WaveInfo.pSamplesPerSec;
		if (DurationDiv)
		{
			sw->Duration = *WaveInfo.pWaveDataSize * 8.0f / DurationDiv;
		}
		else
		{
			sw->Duration = 0.0f;
		}
		sw->SampleRate = *WaveInfo.pSamplesPerSec;
		sw->NumChannels = *WaveInfo.pChannels;
		sw->RawPCMDataSize = WaveInfo.SampleDataSize;
		sw->SoundGroup = ESoundGroup::SOUNDGROUP_Default;
	}
	else {
		return nullptr;
	}

	return sw;
}

ASoftDesignTrainingMainCharacter::ASoftDesignTrainingMainCharacter()
{
	FString path = FString("Source/SoftDesignTraining/Sound/mario-sound.wav");
	soundToPlay = GetSoundWaveFromFile(FPaths::GameDir() + path);

    m_IsPoweredUp = false;

    // Create a camera boom...
    m_CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    m_CameraBoom->SetupAttachment(RootComponent);
    m_CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
    m_CameraBoom->TargetArmLength = 800.f;
    m_CameraBoom->RelativeRotation = FRotator(-60.f, 0.f, 0.f);
    m_CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

                                            // Create a camera...
    m_TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
    m_TopDownCameraComponent->SetupAttachment(m_CameraBoom, USpringArmComponent::SocketName);
    m_TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm;
}

void ASoftDesignTrainingMainCharacter::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    Super::OnBeginOverlap(OverlappedComponent, OtherActor, OtherComponent, OtherBodyIndex, bFromSweep, SweepResult);

    if (ASoftDesignTrainingCharacter* character = Cast<ASoftDesignTrainingCharacter>(OtherActor))
    {
        if (!IsPoweredUp())
            SetActorLocation(m_StartingPosition);
    }
}

void ASoftDesignTrainingMainCharacter::OnCollectPowerUp()
{
	
	UGameplayStatics::PlaySound2D(GetWorld(), soundToPlay);

    m_IsPoweredUp = true;

    GetMesh()->SetMaterial(0, m_PoweredUpMaterial);

    GetWorld()->GetTimerManager().SetTimer(m_PowerUpTimer, this, &ASoftDesignTrainingMainCharacter::OnPowerUpDone, m_PowerUpDuration, false);
}

void ASoftDesignTrainingMainCharacter::OnPowerUpDone()
{
    m_IsPoweredUp = false;

    GetMesh()->SetMaterial(0, nullptr);

    GetWorld()->GetTimerManager().ClearTimer(m_PowerUpTimer);
}
