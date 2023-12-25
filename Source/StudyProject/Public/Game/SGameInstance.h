// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class STUDYPROJECT_API USGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	USGameInstance();
	virtual void Init() override;

	virtual void Shutdown() override;

private:
	UPROPERTY() //언리언이 관리하는속성으로설정 => 조회가능하도록!
	FString Name;
};
