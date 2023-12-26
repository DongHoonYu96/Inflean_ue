// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SGameInstance.h"

#include "JsonObjectConverter.h"
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

	//===================직렬화 실습=================
	// FBirdData 구조체 인스턴스를 생성하고 로그를 출력합니다.
	FBirdData SrcRawData(TEXT("Pigeon17"), 17);
	UE_LOG(LogTemp, Log, TEXT("[SrcRawData] Name: %s, ID: %d"), *SrcRawData.Name, SrcRawData.ID);

	// 저장할 디렉토리 경로를 생성합니다.
	const FString SavedDir = FPaths::Combine(FPlatformMisc::ProjectDir(), TEXT("Saved"));
	UE_LOG(LogTemp, Log, TEXT("SavedDir: %s"), *SavedDir);

	// 원시 데이터 파일 이름을 설정하고 전체 경로를 생성합니다.
	const FString RawDataFileName(TEXT("RawData.bin"));
	FString AbsolutePathForRawData = FPaths::Combine(*SavedDir, *RawDataFileName);
	UE_LOG(LogTemp, Log, TEXT("Relative path for saved file: %s"), *AbsolutePathForRawData);
	FPaths::MakeStandardFilename(AbsolutePathForRawData);
	UE_LOG(LogTemp, Log, TEXT("Absolute path for saved file: %s"), *AbsolutePathForRawData);

	// 파일에 FBirdData를 쓰는 작업을 수행합니다.
	FArchive* RawFileWriterAr = IFileManager::Get().CreateFileWriter(*AbsolutePathForRawData);
	if (nullptr != RawFileWriterAr)
	{
	    *RawFileWriterAr << SrcRawData;
	    RawFileWriterAr->Close();
	    delete RawFileWriterAr;
	    RawFileWriterAr = nullptr;
	}

	// 파일에서 FBirdData를 읽어오는 작업을 수행합니다.
	FBirdData DstRawData;
	FArchive* RawFileReaderAr = IFileManager::Get().CreateFileReader(*AbsolutePathForRawData);
	if (nullptr != RawFileReaderAr)
	{
	    *RawFileReaderAr << DstRawData;
	    RawFileReaderAr->Close();
	    delete RawFileReaderAr;
	    RawFileReaderAr = nullptr;

	    UE_LOG(LogTemp, Log, TEXT("[DstRawData] Name: %s, ID: %d"), *DstRawData.Name, DstRawData.ID);
	}

	// USPigeon 객체를 생성하고 직렬화합니다.
	SerializedPigeon = NewObject<USPigeon>();
	SerializedPigeon->SetName(TEXT("Pigeon76"));
	SerializedPigeon->SetID(76);
	UE_LOG(LogTemp, Log, TEXT("[SerializedPigeon] Name: %s, ID: %d"), *SerializedPigeon->GetName(), SerializedPigeon->GetID());

	// 객체 데이터 파일 이름을 설정하고 전체 경로를 생성합니다.
	const FString ObjectDataFileName(TEXT("ObjectData.bin"));
	FString AbsolutePathForObjectData = FPaths::Combine(*SavedDir, *ObjectDataFileName);
	FPaths::MakeStandardFilename(AbsolutePathForObjectData);

	// 메모리 버퍼에 USPigeon 객체를 직렬화합니다.
	TArray<uint8> BufferArray;
	FMemoryWriter MemoryWriterAr(BufferArray);
	SerializedPigeon->Serialize(MemoryWriterAr);

	// 파일에 직렬화된 데이터를 씁니다.
	TUniquePtr<FArchive> ObjectDataFileWriterAr = TUniquePtr<FArchive>(IFileManager::Get().CreateFileWriter(*AbsolutePathForObjectData));
	if (nullptr != ObjectDataFileWriterAr)
	{
	    *ObjectDataFileWriterAr << BufferArray;
	    ObjectDataFileWriterAr->Close();
	    ObjectDataFileWriterAr = nullptr;
	}

	// 파일에서 직렬화된 데이터를 읽어옵니다.
	TArray<uint8> BufferArrayFromObjectDataFile;
	TUniquePtr<FArchive> ObjectDataFileReaderAr = TUniquePtr<FArchive>(IFileManager::Get().CreateFileReader(*AbsolutePathForObjectData));
	if (nullptr != ObjectDataFileReaderAr)
	{
	    *ObjectDataFileReaderAr << BufferArrayFromObjectDataFile;
	    ObjectDataFileReaderAr->Close();
	    ObjectDataFileReaderAr = nullptr;
	}

	// 읽어온 데이터로 USPigeon 객체를 역직렬화합니다.
	FMemoryReader MemoryReaderAr(BufferArrayFromObjectDataFile);
	USPigeon* Pigeon77 = NewObject<USPigeon>();
	Pigeon77->Serialize(MemoryReaderAr);
	UE_LOG(LogTemp, Log, TEXT("[Pigeon77] Name: %s, ID: %d"), *Pigeon77->GetName(), Pigeon77->GetID());

	//=====================JSON 실습=================
	// JSON 파일 이름을 설정하고 전체 경로를 생성합니다.
	const FString JsonDataFileName(TEXT("StudyJsonFile.txt"));
	FString AbsolutePathForJsonData = FPaths::Combine(*SavedDir, *JsonDataFileName);
	FPaths::MakeStandardFilename(AbsolutePathForJsonData);

	// 새로운 JSON 객체를 생성하고 USPigeon 객체를 JSON으로 변환합니다.
	TSharedRef<FJsonObject> SrcJsonObject = MakeShared<FJsonObject>();
	FJsonObjectConverter::UStructToJsonObject(SerializedPigeon->GetClass(), SerializedPigeon, SrcJsonObject);

	// JSON 문자열을 작성합니다.
	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR>> JsonWriterAr = TJsonWriterFactory<TCHAR>::Create(&JsonOutString);
	if (true == FJsonSerializer::Serialize(SrcJsonObject, JsonWriterAr))
	{
		// JSON 문자열을 파일에 저장합니다.
		FFileHelper::SaveStringToFile(JsonOutString, *AbsolutePathForJsonData);
	}

	// 파일에서 JSON 문자열을 읽어옵니다.
	FString JsonInString;
	FFileHelper::LoadFileToString(JsonInString, *AbsolutePathForJsonData);
	TSharedRef<TJsonReader<TCHAR>> JsonReaderAr = TJsonReaderFactory<TCHAR>::Create(JsonInString);

	// 읽어온 JSON 데이터를 사용하여 새로운 USPigeon 객체를 생성합니다.
	TSharedPtr<FJsonObject> DstJsonObject;
	if (true == FJsonSerializer::Deserialize(JsonReaderAr, DstJsonObject))
	{
		USPigeon* Pigeon78 = NewObject<USPigeon>();
		if (true == FJsonObjectConverter::JsonObjectToUStruct(DstJsonObject.ToSharedRef(), Pigeon78->GetClass(), Pigeon78))
		{
			// 역직렬화된 객체의 정보를 로그로 출력합니다.
			UE_LOG(LogTemp, Log, TEXT("[Pigeon78] Name: %s, ID: %d"), *Pigeon78->GetName(), Pigeon78->GetID());
		}
	}

}

void USGameInstance::Shutdown()
{
	Super::Shutdown();

	UKismetSystemLibrary::PrintString(GetWorld(),TEXT("종료될때 콜& 뷰포트에 찍힘"));
}

