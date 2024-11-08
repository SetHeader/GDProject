// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/MyTestActor.h"

#include "GDProject/GDLog.h"


AMyTestActor::AMyTestActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMyTestActor::BeginPlay()
{
	Super::BeginPlay();
}

void AMyTestActor::TestGenerate()
{
	MyActor1 = GetWorld()->SpawnActor<AMyTestActor>(AMyTestActor::StaticClass());
	MyActor2 = GetWorld()->SpawnActor<AMyTestActor>(AMyTestActor::StaticClass());
	MyActor3 = GetWorld()->SpawnActor<AMyTestActor>(AMyTestActor::StaticClass());
	MyActor4 = GetWorld()->SpawnActor<AMyTestActor>(AMyTestActor::StaticClass());
}

void AMyTestActor::TestDestroy()
{
	MyActor1->Destroy();
	MyActor2->Destroy();
	MyActor3->Destroy();
	MyActor4->Destroy();
}

void AMyTestActor::TestFun()
{
	if (MyActor1)
	{
		GDLOG(TEXT("MyTestActor"), TEXT("%s operator bool() = true"), TEXT("MyActor1"));
	}
	else
	{
		GDLOG(TEXT("MyTestActor"), TEXT("%s operator bool() = false"), TEXT("MyActor1"));
	}
	if (MyActor2)
	{
		GDLOG(TEXT("MyTestActor"), TEXT("%s operator bool() = true"), TEXT("MyActor2"));
	}
	else
	{
		GDLOG(TEXT("MyTestActor"), TEXT("%s operator bool() = false"), TEXT("MyActor2"));
	}
	
	if (MyActor3)
	{
		GDLOG(TEXT("MyTestActor"), TEXT("%s operator bool() = true"), TEXT("MyActor3"));
	}
	else
	{
		GDLOG(TEXT("MyTestActor"), TEXT("%s operator bool() = false"), "MyActor3");
	}
	
	if (MyActor4)
	{
		GDLOG(TEXT("MyTestActor"), TEXT("%s operator bool() = true"), TEXT("MyActor4"));
	}
	else
	{
		GDLOG(TEXT("MyTestActor"), TEXT("%s operator bool() = false"), TEXT("MyActor4"));
	}



	if (IsValid(MyActor1))
	{
		GDLOG(TEXT("MyTestActor"), TEXT("%s IsValid = true"), TEXT("MyActor1"));
	}
	else
	{
		GDLOG(TEXT("MyTestActor"), TEXT("%s IsValid = false"), TEXT("MyActor1"));
	}
	if (IsValid(MyActor2))
	{
		GDLOG(TEXT("MyTestActor"), TEXT("%s IsValid = true"), TEXT("MyActor2"));
	}
	else
	{
		GDLOG(TEXT("MyTestActor"), TEXT("%s IsValid = false"), TEXT("MyActor2"));
	}
	
	if (IsValid(MyActor3))
	{
		GDLOG(TEXT("MyTestActor"), TEXT("%s IsValid = true"), TEXT("MyActor3"));
	}
	else
	{
		GDLOG(TEXT("MyTestActor"), TEXT("%s IsValid = false"), TEXT("MyActor3"));
	}
	
	if (IsValid(MyActor4))
	{
		GDLOG(TEXT("MyTestActor"), TEXT("%s IsValid = true"), TEXT("MyActor4"));
	}
	else
	{
		GDLOG(TEXT("MyTestActor"), TEXT("%s IsValid = false"), TEXT("MyActor4"));
	}
	
}

