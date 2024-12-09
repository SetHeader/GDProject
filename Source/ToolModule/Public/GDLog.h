#pragma once

#include "CoreMinimal.h"

TOOLMODULE_API DECLARE_LOG_CATEGORY_EXTERN(LogGD, Log, All)

#define GDLOG(Category, Format, ...) GDLog::Log(Category, Format, ##__VA_ARGS__);
#define GDLOG_E(Category, Format, ...) GDLog::Error(Category, Format, ##__VA_ARGS__);
#define GDLOG_W(Category, Format, ...) GDLog::Warning(Category, Format, ##__VA_ARGS__);

TOOLMODULE_API class GDLog
{
public:
	template <typename T1, size_t N1, typename T2, size_t N2, typename... T>
	static void Log(const T1 (&Category)[N1], const T2 (&Format)[N2], T&&... Args)
	{
		UE_LOG(LogGD, Log, TEXT("%s\t%s"), Category, *FString::Printf(Format, Forward<T>(Args)...));
	}

	template <typename T1, size_t N1, typename T2, size_t N2, typename ...T>
	static void Error(const T1 (&Category)[N1], const T2 (&Format)[N2], T&&... Args)
	{
		UE_LOG(LogGD, Error, TEXT("%s\t%s"), Category, *FString::Printf(Format, Forward<T>(Args)...));
	}
	
	template <typename T1, size_t N1, typename T2, size_t N2, typename ...T>
	static void Warning(const T1 (&Category)[N1], const T2 (&Format)[N2], T&&... Args)
	{
		UE_LOG(LogGD, Warning, TEXT("%s\t%s"), Category, *FString::Printf(Format, Forward<T>(Args)...));
	}
};