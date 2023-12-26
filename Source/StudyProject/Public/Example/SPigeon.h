// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SFlyable.h"
#include "UObject/NoExportTypes.h"
#include "SPigeon.generated.h"

/**
 * 
 */
UCLASS()
class STUDYPROJECT_API USPigeon : public UObject, public ISFlyable
{
	GENERATED_BODY()

public:
	USPigeon();

	virtual void Fly() override;

private:
	UPROPERTY()
	FString Name;

};
