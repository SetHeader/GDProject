#pragma once

#include "CoreMinimal.h"
#include "Logging\LogVerbosity.h"

DEFINE_LOG_CATEGORY_STATIC(LogSimple, Log, All)

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
		InnerLog(ELogVerbosity::Type::Log, Prefix, *FinalFormatter(*FString::Printf(Format, Forward<ArgTypes>(Args)...)));
	}
	template<typename FmtType, typename... ArgTypes>
	void Warning(const FmtType& Format, ArgTypes... Args) {
		InnerLog(ELogVerbosity::Type::Warning, Prefix, *FinalFormatter(*FString::Printf(Format, Forward<ArgTypes>(Args)...)));
	}
	template<typename FmtType, typename... ArgTypes>
	void Error(const FmtType& Format, ArgTypes... Args) {
		InnerLog(ELogVerbosity::Type::Error, Prefix, *FinalFormatter(*FString::Printf(Format, Forward<ArgTypes>(Args)...)));
	}
	template<typename PrefixType, typename FmtType, typename... ArgTypes>
	static void LogStatic(const PrefixType& InPrefix, const FmtType& Format, ArgTypes... Args) {
		InnerLog(ELogVerbosity::Type::Log, InPrefix, *FString::Printf(Format, Forward<ArgTypes>(Args)...));
	}
	template<typename PrefixType, typename FmtType, typename... ArgTypes>
	static void WarningStatic(const PrefixType& InPrefix, const FmtType& Format, ArgTypes... Args) {
		InnerLog(ELogVerbosity::Type::Warning, InPrefix, *FString::Printf(Format, Forward<ArgTypes>(Args)...));
	}
	template<typename PrefixType, typename FmtType, typename... ArgTypes>
	static void ErrorStatic(const PrefixType& InPrefix, const FmtType& Format, ArgTypes... Args) {
		InnerLog(ELogVerbosity::Type::Error, InPrefix, *FString::Printf(Format, Forward<ArgTypes>(Args)...));
	}
	
	TOOLMODULE_API static TFunction<FString(const TCHAR*)> CreateNetworkInfoFormatter(AActor* Actor);
	TOOLMODULE_API static TFunction<FString(const TCHAR*)> CreateNetworkInfoFormatter(UActorComponent* ActorComponent);
	
private:
	const TCHAR* Prefix;
	const TFunction<FString(const TCHAR*)> FinalFormatter = [](const TCHAR* Format) {return FString(Format);};
	
	template<typename PrefixType, typename FmtType>
	static void InnerLog(ELogVerbosity::Type InVerbosity, const PrefixType& InPrefix, const FmtType& InContent);
};

template <typename PrefixType, typename FmtType>
void SimpleLogger::InnerLog(ELogVerbosity::Type InVerbosity, const PrefixType& InPrefix, const FmtType& InContent) {
	switch (InVerbosity) {
	case ELogVerbosity::Type::Error:
		UE_LOG(LogSimple, Error, TEXT("%s\t%s"), InPrefix, InContent);
		break;
	case ELogVerbosity::Type::Warning:
		UE_LOG(LogSimple, Warning, TEXT("%s\t%s"), InPrefix, InContent);
		break;
	case ELogVerbosity::Type::Display:
		UE_LOG(LogSimple, Display, TEXT("%s\t%s"), InPrefix, InContent);
		break;
	case ELogVerbosity::Type::Log:
		UE_LOG(LogSimple, Log, TEXT("%s\t%s"), InPrefix, InContent);
		break;
	default:
		break;;
	}
}
