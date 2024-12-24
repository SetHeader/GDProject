#include "SimpleLogger.h"

DEFINE_LOG_CATEGORY(LogSimple)

TFunction<FString(const TCHAR*)> SimpleLogger::CreateNetwordInfoFormatter(AActor* Actor) {
	return [Actor](const TCHAR* Msg) {
		FString NetModeName;
		switch (Actor->GetNetMode()) {
		case NM_Standalone:
			NetModeName = "Standalone";
			break;
		case NM_DedicatedServer:
			NetModeName = "DedicatedServer";
			break;
		case NM_ListenServer:
			NetModeName = "ListenServer";
			break;
		case NM_Client:
			NetModeName = "Client";
			break;
		default:
			NetModeName = "Unknown";
		}

		FString RoleName;
		switch (Actor->GetLocalRole()) {
		case ROLE_None:
			RoleName = "None";
			break;
		case ROLE_SimulatedProxy:
			RoleName = "SimulatedProxy";
			break;
		case ROLE_AutonomousProxy:
			RoleName = "AutonomousProxy";
			break;
		case ROLE_Authority:
			RoleName = "Authority";
			break;
		default:
			RoleName = "Unknown";
		}
		
		return FString::Printf(TEXT("[%s:%s]\t%s"), *NetModeName, *RoleName, Msg);
	};
}

TFunction<FString(const TCHAR*)> SimpleLogger::CreateNetwordInfoFormatter(UActorComponent* ActorComponent) {
	return [ActorComponent](const TCHAR* Msg) {
		FString NetModeName;
		switch (ActorComponent->GetNetMode()) {
		case NM_Standalone:
			NetModeName = "Standalone";
			break;
		case NM_DedicatedServer:
			NetModeName = "DedicatedServer";
			break;
		case NM_ListenServer:
			NetModeName = "ListenServer";
			break;
		case NM_Client:
			NetModeName = "Client";
			break;
		default:
			NetModeName = "Unknown";
		}

		FString RoleName;
		switch (ActorComponent->GetOwnerRole()) {
		case ROLE_None:
			RoleName = "None";
			break;
		case ROLE_SimulatedProxy:
			RoleName = "SimulatedProxy";
			break;
		case ROLE_AutonomousProxy:
			RoleName = "AutonomousProxy";
			break;
		case ROLE_Authority:
			RoleName = "Authority";
			break;
		default:
			RoleName = "Unknown";
		}
		
		return FString::Printf(TEXT("[%s:%s]\t%s"), *NetModeName, *RoleName, Msg);
	};
}
