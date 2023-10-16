// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Achievements.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FCustomAchievement
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "EOS Achievements")
	FText Title;

	UPROPERTY(BlueprintReadOnly, Category = "EOS Achievements")
	FText Desc;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAchievementDelegate, const TArray<FCustomAchievement>&, ReadResult);

UCLASS()
class MAIN_MAP_API UAchievements : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public: 
	UPROPERTY(BlueprintAssignable);
	FAchievementDelegate OnReadAchievements;

	UFUNCTION(BlueprintCallable, Category = "EOS Achievements", meta = (BlueprintInternalUseOnly = "true"))
	static UAchievements* ReadAchievements(UGameInstance* Instance);
	
	virtual void Activate() override;

private:
	UGameInstance* GameInstance;
	void QueryAchievements(const FUniqueNetId& UserId, const bool bWasSuccessful);
	void RetrieveAchievements(const FUniqueNetId& UserId, const bool bWasSuccessful);
};
