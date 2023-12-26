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

	const FString& GetName() const { return Name; }

	void SetName(const FString& InName) { Name = InName; }

	int32 GetID() const { return ID; }

	void SetID(int32 InID) { ID = InID; }

	virtual void Serialize(FArchive& Ar) override;

	//TMAP 사용을 위한 연산자 오버로딩
	bool operator==(const FBirdData& InBirdData) const
	{
		return ID == InBirdData.ID;
	}

	friend uint32 GetTypeHash(const FBirdData& InBirdData)
	{
		return GetTypeHash(InBirdData.ID);
	}


private:
	UPROPERTY()
	FString Name;

	UPROPERTY()
	int32 ID;

};
