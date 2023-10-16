// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineLeaderboardInterface.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Leaderboard.generated.h"

/**
 *
 */

USTRUCT(BlueprintType)
struct FCustomLeaderboardRow
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "EOS Leaderboard")
	int32 Rank = 0;

	UPROPERTY(BlueprintReadOnly, Category = "EOS Leaderboard")
	FString Nickname = "";

	UPROPERTY(BlueprintReadOnly, Category = "EOS Leaderboard")
	int32 Value = 0;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FResponseDelegate, bool, bWasSuccessful, const TArray<FCustomLeaderboardRow>&, ReadResult);

UCLASS()
class MAIN_MAP_API ULeaderboard : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable);
	FResponseDelegate OnReadLeaderboard;

	UFUNCTION(BlueprintCallable, Category = "EOS Leaderboard", meta = (BlueprintInternalUseOnly = "true"))
	static ULeaderboard* GetLeaderboard(FString StatName, UGameInstance *Instance);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS Leaderboard")
	TArray<FCustomLeaderboardRow> LeaderboardReadResult;

	virtual void Activate() override;

private:
	void ExecuteOnReadLeaderboard(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	
	FString CurrentStatName;

	UGameInstance* GameInstance;

	FHttpModule* Http;
};
