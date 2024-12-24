#pragma once

#include "CoreMinimal.h"

TOOLMODULE_API DECLARE_LOG_CATEGORY_EXTERN(LogSimple, Log, All)

TOOLMODULE_API class SimpleLogger
{
public:
	SimpleLogger(const TCHAR* Prefix)
		: Prefix(Prefix) {}
	
	SimpleLogger(const TFunction<FString(const TCHAR*)>& FinalFormatter)
		: FinalFormatter(FinalFormatter) {}

	SimpleLogger(const TCHAR* Prefix, const TFunction<FString(const TCHAR*)>& FinalFormatter)
			: Prefix(Prefix)
			, FinalFormatter(FinalFormatter) {}
	
	template<typename FmtType, typename... ArgTypes>
	void Log(const FmtType& Format, ArgTypes... Args) {
		UE_LOG(LogSimple, Log, TEXT("%s\t%s"), *Prefix,
			*FinalFormatter(*FString::Printf(Format, Forward<ArgTypes>(Args)...)));
	}
	
	static TFunction<FString(const TCHAR*)> CreateNetwordInfoFormatter(AActor* Actor);
	static TFunction<FString(const TCHAR*)> CreateNetwordInfoFormatter(UActorComponent* ActorComponent);
private:
	const FString Prefix;
	const TFunction<FString(const TCHAR*)> FinalFormatter = [](const TCHAR* Format) {return FString(Format);};
};
