// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "UpdateStats.generated.h"

/**
 * 
 */

UCLASS()
class MAIN_MAP_API UUpdateStats : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "EOS Stats", meta = (BlueprintInternalUseOnly = "true"))
	static UUpdateStats* UpdateStats(FString InStatName, int32 InStatValue, UGameInstance* Instance);

	virtual void Activate() override;

private:

	FHttpModule* Http;

	void UpdateStatsHTTP(FString Puid);

	void OnUpdateHttpComplete(FHttpRequestPtr Request, FHttpResponsePtr Resonse, bool bWasSuccessful);

	FString StatName;

	int32 StatValue;

	UGameInstance* GameInstance;
};
