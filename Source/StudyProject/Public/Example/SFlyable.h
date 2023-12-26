// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SFlyable.generated.h"

USTRUCT() //구조체 => 데이터 전달
struct FBirdData
{
	GENERATED_BODY() // Unreal Engine의 구조체 초기화 매크로

public:
	FBirdData() {}

	FBirdData(const FString& InName, int32 InID)
		: Name(InName)
		, ID(InID)
	{
	}

	// 직렬화 연산자 오버로딩
	// friend 키워드 : (private, protected) 변수 접근허용
	friend FArchive& operator<<(FArchive& Ar, FBirdData& InBirdData)
	{
		Ar << InBirdData.Name; // Name 멤버 변수 직렬화
		Ar << InBirdData.ID;   // ID 멤버 변수 직렬화
		return Ar;
	}

	UPROPERTY()
	FString Name = TEXT("DefaultBirdName");

	UPROPERTY()
	int32 ID = 0;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USFlyable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class STUDYPROJECT_API ISFlyable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Fly() = 0;
};
