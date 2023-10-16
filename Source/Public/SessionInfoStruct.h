// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UObject/NoExportTypes.h"
#include "SessionInfoStruct.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FSessionInfoStruct
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS Variables")
    FString SessionName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS Variables")
    FString SessionAddress;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS Variables")
    FString UserID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS Variables")
    FString UserName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS Variables")
    int32 CurrentPlayer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS Variables")
    int32 MaxPlayer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS Variables")
    int32 RestPlayer;
};