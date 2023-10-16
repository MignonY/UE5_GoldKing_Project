// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"

#include "Runtime/Online/HTTP/Public/Http.h"

#include "ThirdParty/EOSSDK/SDK/Include/eos_connect_types.h"
#include "ThirdParty/EOSSDK/SDK/Include/eos_sdk.h"
#include "ThirdParty/EOSSDK/SDK/Include/eos_connect.h"

#include "Interfaces/OnlineSessionInterface.h"
#include "SessionInfoStruct.h"

#include "EOS_GameInstance.generated.h"

UCLASS()
class MAIN_MAP_API UEOS_GameInstance : public UGameInstance
{
	GENERATED_BODY()

	
public :
	
	TSharedPtr<FOnlineSessionSearch> AutoJoinSessionSearch;
	TSharedPtr<FOnlineSessionSearch> JoinSessionSearch;
	TSharedPtr<FOnlineSessionSearch> FindSessionSearch;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS Variables")
	FSessionInfoStruct CreateSessionInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS Variables")
	TArray<FSessionInfoStruct> FindSessionInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS Variables")
	FString TargetSessionID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS Variables")
	FString TargetSessionName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS Variables")
		FString CreateSessionName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS Variables")
		FString CreateSessionID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS Variables")
	FString OpenLevelText;

	///

	UFUNCTION(BlueprintCallable, Category = "EOS Authentication")
	void LoginWithEOS(FString ID, FString Token, FString LoginType, FString NewNickname);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOS Authentication")
	FString GetPlayerUsername();

	UFUNCTION(BlueprintCallable, Category = "EOS Authentication")
	FString UpdatePlayerUsername(FString NewDisplayName);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOS Authentication")
	bool IsPlayerLoggedIn();
	
	///

	UFUNCTION(BlueprintCallable, Category = "EOS_Function")
	void CreateEOSSession(bool bIsDedicatedServer, bool bIsLanServer, bool AllowInvites, bool bShouldAdvertise, FString SessionTitle, int32 NumberOfPublicConnections);

	UFUNCTION(BlueprintCallable, Category = "EOS_Function")
	void DestroySession(FString SessionTitle);

	UFUNCTION(BlueprintCallable, Category = "EOS_Function")
	void JoinSessionById(const FString& SessionID);

	UFUNCTION(BlueprintCallable, Category = "EOS_Function")
	void StartOnlineSession(FString SessionTitle);

	UFUNCTION(BlueprintCallable, Category = "EOS_Function")
	void GetAllEOSGameSessions();

	UFUNCTION(BlueprintCallable, Category = "EOS_Function")
	bool UnregisterPlayerFromSession(FString SessionName);

	UFUNCTION(BlueprintCallable, Category = "EOS_Function")
	void UpdateOnlineSession(FString SessionTitle);

	void GetAllEOSGameSessionsCompleted(bool bWasSuccess);
	void OnCreateSessionCompleted(FName SessionName, bool bWasSuccessful);
	void OnDestroySessionCompleted(FName SessionName, bool bWasSuccessful);
	void OnFindSessionCompleted(bool bWasSuccess);
	void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnJoinSessionByIdCompleted(bool bWasSuccess);
	void OnStartOnlineSessionCompleted(FName SessionName, bool bWasSuccess);
	void OnUpdateOnlineSessionCompleted(FName SessionName, bool bWasSuccessful);

private :
	void LoginWithEOS_Return(int32 LocalUserNum, bool bWasSuccess, const FUniqueNetId& UserId, const FString& Error);
	void ChangeLoginStatus(int32 LocalUserNum, ELoginStatus::Type OldStatus, ELoginStatus::Type NewStatus, const FUniqueNetId& UserId);

	void SubmitLoginInfo();
	void OnSubmitLoginInfoComplete(FHttpRequestPtr Request, FHttpResponsePtr Resonse, bool bWasSuccessful);

	FHttpModule* Http = &FHttpModule::Get();

};
