// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "EOS_Stats.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FCustomStatInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "EOS Stats")
	FString StatName;

	UPROPERTY(BlueprintReadOnly, Category = "EOS Stats")
	int32 StatValue;

};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStatResponseDelegate, const TArray<FCustomStatInfo>&, Stats);

UCLASS()
class MAIN_MAP_API UEOS_Stats : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public : 
	UPROPERTY(BlueprintAssignable);
	FStatResponseDelegate OnReadStats;

	UFUNCTION(BlueprintCallable, Category = "EOS Stats", meta = (BlueprintInternalUseOnly = "true"))
	static UEOS_Stats* ReadStats(TArray<FString> InStatName, UGameInstance* Instance);

	virtual void Activate() override;

private: 

	void OnGetStatsComplete(const FOnlineError& Result, const TArray<TSharedRef<const FOnlineStatsUserStats>>& UsersStatsResult);

	TArray<FString> StatName;

	UGameInstance* GameInstance;
};
