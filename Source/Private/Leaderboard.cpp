// Fill out your copyright notice in the Description page of Project Settings.


#include "Leaderboard.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineLeaderboardInterface.h"

ULeaderboard* ULeaderboard::GetLeaderboard(FString Statname, UGameInstance* Instance) {
	ULeaderboard* NewGetLeaderboard = NewObject<ULeaderboard>();
	NewGetLeaderboard->CurrentStatName = Statname;
	NewGetLeaderboard->GameInstance = Instance;
	NewGetLeaderboard->Http = &FHttpModule::Get();
	return NewGetLeaderboard;
}

void ULeaderboard::Activate() {
	IOnlineSubsystem* SubSystemRef = Online::GetSubsystem(GameInstance->GetWorld());

	if (SubSystemRef) {

		IOnlineIdentityPtr IdentityPointerRef = SubSystemRef->GetIdentityInterface();

		if (IdentityPointerRef) {
			IOnlineLeaderboardsPtr LeaderboardRef = SubSystemRef->GetLeaderboardsInterface();
				
				FString puid = IdentityPointerRef->GetUniquePlayerId(0)->ToString();
				TSharedRef<IHttpRequest> Request = Http->CreateRequest();
				Request->OnProcessRequestComplete().BindUObject(this, &ULeaderboard::ExecuteOnReadLeaderboard);

				Request->SetURL("http://j9a203.p.ssafy.io:8080/api/user/rank?stat=" + CurrentStatName);
				Request->SetVerb("GET");

				Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
				Request->SetHeader("Content-Type", TEXT("application/json"));
				Request->ProcessRequest();

				/*
				ReadObject = MakeShareable(new FOnlineLeaderboardRead());
				ReadObject->LeaderboardName = FName(CurrentBoardName);
				ReadObject->SortedColumn = FName(CurrentStatName);

				ReadObject->ColumnMetadata.Empty();
				FColumnMetaData Metadata(FName(CurrentStatName), EOnlineKeyValuePairDataType::Int32);
				ReadObject->ColumnMetadata.Add(Metadata);
				UE_LOG(LogTemp, Warning, TEXT("Metadata Count: %s"), *FString::FromInt(ReadObject->ColumnMetadata.Num()));

				FOnlineLeaderboardReadRef ReadObjectRef = ReadObject.ToSharedRef();

				LeaderboardReadCompleteDelegateHandle = LeaderboardRef->AddOnLeaderboardReadCompleteDelegate_Handle(FOnLeaderboardReadCompleteDelegate::CreateUObject(this, &ULeaderboard::ExecuteOnReadLeaderboard));
				LeaderboardRef->ReadLeaderboardsAroundRank(0, 20, ReadObjectRef);
				*/
		}
	}
}

void ULeaderboard::ExecuteOnReadLeaderboard(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	LeaderboardReadResult.Empty();

	if (Response->GetResponseCode() == 200) {
		UE_LOG(LogTemp, Warning, TEXT("Success"));

		//ÆÄ½Ì
		FString JsonString = Response->GetContentAsString();
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(*JsonString);
		TArray<TSharedPtr<FJsonValue>> JsonArray;
		if (FJsonSerializer::Deserialize(Reader, JsonArray))
		{
			for (auto Row : JsonArray) {
				auto RowObj = Row->AsObject();

				int32 Rank = RowObj->GetIntegerField("rank");
				FString Nickname = RowObj->GetStringField("nickname");
				int32 Val = RowObj->GetIntegerField("score");

				FCustomLeaderboardRow Result;
				Result.Nickname = Nickname;
				Result.Rank = Rank;
				Result.Value = Val;

				LeaderboardReadResult.Add(Result);
			}
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Fail"));
	}

	OnReadLeaderboard.Broadcast(bWasSuccessful, LeaderboardReadResult);
}
