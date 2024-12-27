
#include "Tools/StringTool.h"


FString StringTool::Stringify(ENetMode NM) {
	switch (NM) {
	case NM_Standalone:
		return "Standalone";
	case NM_DedicatedServer:
		return "DedicatedServer";
	case NM_ListenServer:
		return "ListenServer";
	case NM_Client:
		return "Client";
	case NM_MAX:
		return "MAX";
	default:
		return "Unknown";
	}
}

FString StringTool::Stringify(ENetRole NR) {
	switch (NR) {
		case ROLE_None:
			return "None";
		case ROLE_SimulatedProxy:
			return "SimulatedProxy";
		case ROLE_AutonomousProxy:
			return "AutonomousProxy";
		case ROLE_Authority:
			return "Authority";
		case ROLE_MAX:
			return "MAX";
		default:
			return "Unknown";
	}
}
