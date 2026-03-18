#include "Task9GameModeBase.h"

#include "EngineUtils.h"
#include "TimerManager.h"
#include "Task9GameStateBase.h"
#include "Player/Task9PlayerController.h"
#include "Player/Task9PlayerState.h"

void ATask9GameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SecretNumberString = GenerateSecretNumber();
	UE_LOG(LogTemp, Warning, TEXT("SecretNumberString = %s"), *SecretNumberString);

	ATask9GameStateBase* Task9GameStateBase = GetGameState<ATask9GameStateBase>();
	if (IsValid(Task9GameStateBase))
	{
		Task9GameStateBase->RemainingTurnTime = TurnDuration;
		Task9GameStateBase->MaxTurnTime = TurnDuration;
		Task9GameStateBase->MaxTurnGuessCount = 3;
		Task9GameStateBase->ResultMessage = TEXT("");
	}
}

void ATask9GameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	
	ATask9PlayerController* Task9PlayerController = Cast<ATask9PlayerController>(NewPlayer);
	if (IsValid(Task9PlayerController))
	{
		AllPlayerControllers.Add(Task9PlayerController);

		ATask9PlayerState* CXPS = Task9PlayerController->GetPlayerState<ATask9PlayerState>();
		if (IsValid(CXPS) == true)
		{
			CXPS->PlayerNameString = TEXT("Player") + FString::FromInt(AllPlayerControllers.Num());
		}

		ATask9GameStateBase* Task9GameStateBase =  GetGameState<ATask9GameStateBase>();
		if (IsValid(Task9GameStateBase) == true)
		{
			Task9GameStateBase->MulticastRPCBroadcastLoginMessage(CXPS->PlayerNameString);
		}

		if (CurrentTurnIndex == INDEX_NONE)
		{
			StartTurn(0);
		}
	}
}

FString ATask9GameModeBase::GenerateSecretNumber()
{
	TArray<int32> Numbers;
	for (int32 i = 1; i <= 9; ++i)
	{
		Numbers.Add(i);
	}

	FMath::RandInit(FDateTime::Now().GetTicks());
	Numbers = Numbers.FilterByPredicate([](int32 Num) { return Num > 0; });

	FString Result;
	for (int32 i = 0; i < 3; ++i)
	{
		int32 Index = FMath::RandRange(0, Numbers.Num() - 1);
		Result.Append(FString::FromInt(Numbers[Index]));
		Numbers.RemoveAt(Index);
	}

	return Result;
}

bool ATask9GameModeBase::IsGuessNumberString(const FString& InNumberString)
{
	bool bCanPlay = false;

	do
	{
		if (InNumberString.Len() != 3)
		{
			break;
		}

		bool bIsUnique = true;
		TSet<TCHAR> UniqueDigits;
		for (TCHAR C : InNumberString)
		{
			if (FChar::IsDigit(C) == false || C == '0')
			{
				bIsUnique = false;
				break;
			}

			UniqueDigits.Add(C);
		}

		if (!bIsUnique)
		{
			break;
		}

		bCanPlay = true;
	}
	while (false);

	return bCanPlay;
}

FString ATask9GameModeBase::JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString)
{
	int32 StrikeCount = 0, BallCount = 0;

	for (int32 i = 0; i < 3; i++)
	{
		if (InSecretNumberString[i] == InGuessNumberString[i])
		{
			StrikeCount++;
		}
		else
		{
			FString PlayerGuessChar = FString::Printf(TEXT("%c"), InGuessNumberString[i]);
			if (InSecretNumberString.Contains(PlayerGuessChar))
			{
				BallCount++;
			}
		}
	}

	if (StrikeCount == 0 && BallCount == 0)
	{
		return TEXT("OUT");
	}

	return FString::Printf(TEXT("%dS %dB"), StrikeCount, BallCount);
}

void ATask9GameModeBase::PrintChatMessageString(ATask9PlayerController* InChattingPlayerController,
                                                const FString& InChatMessageString)
{
	if (!IsCurrentTurnPlayer(InChattingPlayerController))
	{
		if (IsValid(InChattingPlayerController))
		{
			InChattingPlayerController->ClientRPCPrintChatMessageString(TEXT("당신의 차례가 아닙니다."));
		}
		return;
	}

	FString ChatMessageString = InChatMessageString;
	int Index = InChatMessageString.Len() - 3;
	FString GuessNumberString = InChatMessageString.RightChop(Index);
	if (IsGuessNumberString(GuessNumberString))
	{
		FString JudgeResultString = JudgeResult(SecretNumberString, GuessNumberString);
		
		IncreaseGuessCount(InChattingPlayerController);
		UpdateTurnUI();
		
		for (TActorIterator<ATask9PlayerController> It(GetWorld()); It; ++It)
		{
			ATask9PlayerController* Task9PlayerController = *It;
			if (IsValid(Task9PlayerController))
			{
				FString CombinedMessageString = InChatMessageString + TEXT(" -> ") + JudgeResultString;
				Task9PlayerController->ClientRPCPrintChatMessageString(CombinedMessageString);
			}
		}

		int32 StrikeCount = FCString::Atoi(*JudgeResultString.Left(1));
		JudgeGame(InChattingPlayerController, StrikeCount);

		ATask9PlayerState* Task9PS = InChattingPlayerController->GetPlayerState<ATask9PlayerState>();
		if (IsValid(Task9PS) && Task9PS->CurrentGuessCount >= Task9PS->MaxGuessCount)
		{
			EndTurn();
		}
	}
	else
	{
		InChattingPlayerController->ClientRPCPrintChatMessageString(TEXT("세 자리 숫자를 입력하세요."));
	}
}

void ATask9GameModeBase::IncreaseGuessCount(ATask9PlayerController* InChattingPlayerController)
{
	ATask9PlayerState* Task9PS = InChattingPlayerController->GetPlayerState<ATask9PlayerState>();
	if (IsValid(Task9PS))
	{
		Task9PS->CurrentGuessCount++;
	}
}

void ATask9GameModeBase::ResetGame()
{
	SecretNumberString = GenerateSecretNumber();
	GetWorldTimerManager().ClearTimer(TurnTimerHandle);
	GetWorldTimerManager().ClearTimer(ResetTimerHandle);

	BroadcastSystemMessage(TEXT("게임을 다시 시작합니다."));
	
	for (const auto& Task9PlayerController : AllPlayerControllers)
	{
		ATask9PlayerState* Task9PS = Task9PlayerController->GetPlayerState<ATask9PlayerState>();
		if (IsValid(Task9PS))
		{
			Task9PS->CurrentGuessCount = 0;
		}
	}

	ATask9GameStateBase* Task9GameStateBase = GetGameState<ATask9GameStateBase>();
	if (IsValid(Task9GameStateBase))
	{
		Task9GameStateBase->ResultMessage = TEXT("");
	}

	if (AllPlayerControllers.Num() > 0)
	{
		StartTurn(0);
	}
}

void ATask9GameModeBase::JudgeGame(ATask9PlayerController* InChattingPlayerController, int InStrikeCount)
{
	if (InStrikeCount == 3)
	{
		ATask9PlayerState* Task9PS = InChattingPlayerController->GetPlayerState<ATask9PlayerState>();
		if (IsValid(Task9PS))
		{
			const FString CombinedMessageString = Task9PS->PlayerNameString + TEXT("의 승리입니다.");
			BroadcastSystemMessage(CombinedMessageString);

			ATask9GameStateBase* Task9GameStateBase = GetGameState<ATask9GameStateBase>();
			if (IsValid(Task9GameStateBase))
			{
				Task9GameStateBase->ResultMessage = FString::Printf(TEXT("%s"), *Task9PS->PlayerNameString);
			}

			GetWorldTimerManager().ClearTimer(TurnTimerHandle);
			GetWorldTimerManager().SetTimer(ResetTimerHandle, this, &ThisClass::HandleDelayedReset, ResetDelay, false);
		}
	}
}

void ATask9GameModeBase::StartTurn(int32 InTurnIndex)
{
	if (AllPlayerControllers.Num() == 0)
	{
		CurrentTurnIndex = INDEX_NONE;
		return;
	}

	CurrentTurnIndex = InTurnIndex % AllPlayerControllers.Num();
	RemainingTurnTime = TurnDuration;

	ATask9PlayerController* CurrentTurnPlayerController = AllPlayerControllers[CurrentTurnIndex];
	if (IsValid(CurrentTurnPlayerController))
	{
		ATask9PlayerState* Task9PS = CurrentTurnPlayerController->GetPlayerState<ATask9PlayerState>();
		if (IsValid(Task9PS))
		{
			Task9PS->CurrentGuessCount = 0;
		}
	}

	UpdateTurnUI();
	GetWorldTimerManager().SetTimer(TurnTimerHandle, this, &ThisClass::HandleTurnTimerTick, 1.0f, true);
}

void ATask9GameModeBase::EndTurn()
{
	GetWorldTimerManager().ClearTimer(TurnTimerHandle);
	AdvanceTurn();
}

void ATask9GameModeBase::AdvanceTurn()
{
	if (AllPlayerControllers.Num() == 0)
	{
		CurrentTurnIndex = INDEX_NONE;
		return;
	}

	const int32 NextTurnIndex = (CurrentTurnIndex + 1) % AllPlayerControllers.Num();
	StartTurn(NextTurnIndex);
}

void ATask9GameModeBase::HandleTurnTimerTick()
{
	RemainingTurnTime = FMath::Max(0, RemainingTurnTime - 1);
	UpdateTurnUI();

	if (RemainingTurnTime <= 0)
	{
		BroadcastSystemMessage(TEXT("시간 초과"));
		EndTurn();
	}
}

void ATask9GameModeBase::HandleDelayedReset()
{
	ResetGame();
}

bool ATask9GameModeBase::IsCurrentTurnPlayer(const ATask9PlayerController* InPlayerController) const
{
	return CurrentTurnIndex != INDEX_NONE
		&& AllPlayerControllers.IsValidIndex(CurrentTurnIndex)
		&& AllPlayerControllers[CurrentTurnIndex] == InPlayerController;
}

void ATask9GameModeBase::BroadcastSystemMessage(const FString& InMessageString)
{
	for (const auto& Task9PlayerController : AllPlayerControllers)
	{
		if (IsValid(Task9PlayerController))
		{
			Task9PlayerController->ClientRPCPrintChatMessageString(InMessageString);
		}
	}
}

void ATask9GameModeBase::UpdateTurnUI() const
{
	ATask9GameStateBase* Task9GameStateBase = GetGameState<ATask9GameStateBase>();
	if (!IsValid(Task9GameStateBase))
	{
		return;
	}

	Task9GameStateBase->RemainingTurnTime = RemainingTurnTime;

	if (!AllPlayerControllers.IsValidIndex(CurrentTurnIndex))
	{
		Task9GameStateBase->CurrentTurnPlayerName = TEXT("None");
		Task9GameStateBase->CurrentTurnGuessCount = 0;
		return;
	}

	ATask9PlayerController* CurrentTurnPlayerController = AllPlayerControllers[CurrentTurnIndex];
	ATask9PlayerState* Task9PS = IsValid(CurrentTurnPlayerController)
		? CurrentTurnPlayerController->GetPlayerState<ATask9PlayerState>()
		: nullptr;

	Task9GameStateBase->CurrentTurnPlayerName = IsValid(Task9PS) ? Task9PS->PlayerNameString : TEXT("None");
	Task9GameStateBase->CurrentTurnGuessCount = IsValid(Task9PS) ? Task9PS->CurrentGuessCount : 0;
	Task9GameStateBase->MaxTurnGuessCount = IsValid(Task9PS) ? Task9PS->MaxGuessCount : 3;
}
