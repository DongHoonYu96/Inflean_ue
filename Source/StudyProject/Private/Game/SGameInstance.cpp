// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SGameInstance.h"
#include "SUnrealObjectClass.h"
#include "Example/SPigeon.h"
#include "Kismet/KismetSystemLibrary.h"

USGameInstance::USGameInstance()
{
	Name = TEXT("USGameInstance Class Default Object");
	//CDO의 Name 속성에 저장됨.
	//에디터 실행전에 호출됨
}

void USGameInstance::Init()
{
	Super::Init();

	UKismetSystemLibrary::PrintString(GetWorld(),TEXT("초기화 콜 & 뷰포트에 찍힘"));

	UClass* RuntimeClassInfo = GetClass(); //런타임일때 클래스정보 조회
	UClass* CompiletimeClassInfo  = StaticClass(); // 컴파일일때 클래스정보 조회

	UE_LOG(LogTemp, Log, TEXT("Class Name: %s"), *RuntimeClassInfo->GetName());

	Name = TEXT("USGameInstance Object");
	// CDO를 통해 생성된 개체의 Name 속성에 새롭게 대입되는 값. (CDO 아님!)

	//CDO를 가져오고, 형변환, 이름가져와라. 결과 : CDO의 이름
	UE_LOG(LogTemp, Log, TEXT("USGameInstance::Name: %s"), *(GetClass()->GetDefaultObject<USGameInstance>()->Name));
	//결과 : 새 객체의 이름
	UE_LOG(LogTemp, Log, TEXT("USGameInstance::Name: %s"), *Name);


	//=====================UFUNCTION 실습===================================
	
	USUnrealObjectClass* USObject1 = NewObject<USUnrealObjectClass>();
	// 언리얼은 이런식으로 new 키워드를 안쓰고 NewObject<>() API를 사용해야 함.
	//<type>의 객체 생성

	UE_LOG(LogTemp, Log, TEXT("USObject1's Name: %s"), *USObject1->GetName());                      
	// 우리가 정의한 Getter()

	FProperty* NameProperty = USUnrealObjectClass::StaticClass()->FindPropertyByName(TEXT("Name"));
	// 프로퍼티 시스템을 활용한 Getter()

	FString CompiletimeUSObjectName;
	if (nullptr != NameProperty)
	{
		//(클래스정보, 저장할변수 == out변수!)
		NameProperty->GetValue_InContainer(USObject1, &CompiletimeUSObjectName);
		UE_LOG(LogTemp, Log, TEXT("CompiletimeUSObjectName: %s"), *CompiletimeUSObjectName);
	}

	USObject1->HelloUnreal();

	UFunction* HelloUnrealFunction = USObject1->GetClass()->FindFunctionByName(TEXT("HelloUnreal"));
	if (nullptr != HelloUnrealFunction)
	{
		//함수실행해봐
		USObject1->ProcessEvent(HelloUnrealFunction, nullptr);
	}

	//===========인터페이스 실습=======================
	USPigeon* Pigeon1 = NewObject<USPigeon>(); //생성자
	ISFlyable* Bird1 = Cast<ISFlyable>(Pigeon1); //피존을 <Type>으로 업캐스팅
	// 현업에서 인터페이스 개념은 대부분 이런식으로 업캐스팅 하기 위함. => 배열에 담아 관리하기위해

	if (nullptr != Bird1)
	{
		Bird1->Fly();
	}
}

void USGameInstance::Shutdown()
{
	Super::Shutdown();

	UKismetSystemLibrary::PrintString(GetWorld(),TEXT("종료될때 콜& 뷰포트에 찍힘"));
}

