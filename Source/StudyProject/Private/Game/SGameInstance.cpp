// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SGameInstance.h"

#include "Kismet/KismetSystemLibrary.h"

void USGameInstance::Init()
{
	Super::Init();

	UKismetSystemLibrary::PrintString(GetWorld(),TEXT("초기화 콜 & 뷰포트에 찍힘"));
}

void USGameInstance::Shutdown()
{
	Super::Shutdown();

	UKismetSystemLibrary::PrintString(GetWorld(),TEXT("종료될때 콜& 뷰포트에 찍힘"));
}

