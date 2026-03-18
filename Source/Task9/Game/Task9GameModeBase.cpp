#include "Task9GameModeBase.h"

#include "EngineUtils.h"
#include "Task9GameStateBase.h"
#include "Player/Task9PlayerController.h"

void ATask9GameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SecretNumberString = GenerateSecretNumber();
	UE_LOG(LogTemp, Warning, TEXT("SecretNumberString = %s"), *SecretNumberString);
}

void ATask9GameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	ATask9GameStateBase* Task9GameStateBase = GetGameState<ATask9GameStateBase>();
	if (IsValid(Task9GameStateBase))
	{
		Task9GameStateBase->MulticastRPCBroadcastLoginMessage(TEXT("Player"));
	}

	ATask9PlayerController* Task9PlayerController = Cast<ATask9PlayerController>(NewPlayer);
	if (IsValid(Task9PlayerController))
	{
		AllPlayerControllers.Add(Task9PlayerController);
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

	return FString::Printf(TEXT("%dS%dB"), StrikeCount, BallCount);
}

void ATask9GameModeBase::PrintChatMessageString(ATask9PlayerController* InChattingPlayerController,
                                                const FString& InChatMessageString)
{
	FString ChatMessageString = InChatMessageString;
	int Index = InChatMessageString.Len() - 3;
	FString GuessNumberString = InChatMessageString.RightChop(Index);
	if (IsGuessNumberString(GuessNumberString) == true)
	{
		FString JudgeResultString = JudgeResult(SecretNumberString, GuessNumberString);
		for (TActorIterator<ATask9PlayerController> It(GetWorld()); It; ++It)
		{
			ATask9PlayerController* Task9PlayerController = *It;
			if (IsValid(Task9PlayerController) == true)
			{
				FString CombinedMessageString = InChatMessageString + TEXT(" -> ") + JudgeResultString;
				Task9PlayerController->ClientRPCPrintChatMessageString(CombinedMessageString);
			}
		}
	}
	else
	{
		for (TActorIterator<ATask9PlayerController> It(GetWorld()); It; ++It)
		{
			ATask9PlayerController* Task9PlayerController = *It;
			if (IsValid(Task9PlayerController) == true)
			{
				Task9PlayerController->ClientRPCPrintChatMessageString(InChatMessageString);
			}
		}
	}
}
