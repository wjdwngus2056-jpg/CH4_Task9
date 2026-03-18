#include "Task9PlayerState.h"

#include "Net/UnrealNetwork.h"

ATask9PlayerState::ATask9PlayerState()
	:PlayerNameString(TEXT("None"))
{
	bReplicates = true;
}

void ATask9PlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, PlayerNameString);
}
