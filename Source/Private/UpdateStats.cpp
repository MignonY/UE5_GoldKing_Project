// Fill out your copyright notice in the Description page of Project Settings.


#include "UpdateStats.h"

UUpdateStats* UUpdateStats::UpdateStats(FString InStatName, int32 InStatValue, UGameInstance* Instance) {
	UUpdateStats* NewUpdateStats= NewObject<UUpdateStats>();
	NewUpdateStats->StatName= InStatName;
	NewUpdateStats->GameInstance = Instance;
	NewUpdateStats->StatValue= InStatValue;
	NewUpdateStats->Http = &FHttpModule::Get();
	return NewUpdateStats;

}


void UUpdateStats::Activate() {
	IOnlineSubsystem* SubSystemRef = Online::GetSubsystem(GameInstance->GetWorld());

	if (SubSystemRef) {
		IOnlineIdentityPtr IdentityPointerRef = SubSystemRef->GetIdentityInterface();

		if (IdentityPointerRef) {
			IOnlineStatsPtr StatsPointerRef = SubSystemRef->GetStatsInterface();

			if (StatsPointerRef) {
				FOnlineStatsUserUpdatedStats StatVar = FOnlineStatsUserUpdatedStats(IdentityPointerRef->GetUniquePlayerId(0).ToSharedRef());
				FString puid = IdentityPointerRef->GetUniquePlayerId(0)->ToString();

				StatVar.Stats.Add(StatName, FOnlineStatUpdate(StatValue, FOnlineStatUpdate::EOnlineStatModificationType::Sum));
				TArray<FOnlineStatsUserUpdatedStats> StatArray;
				StatArray.Add(StatVar);
				UE_LOG(LogTemp, Warning, TEXT("%s"), *IdentityPointerRef->GetUniquePlayerId(0)->ToString());
				StatsPointerRef->UpdateStats(IdentityPointerRef->GetUniquePlayerId(0).ToSharedRef(), StatArray,
					FOnlineStatsUpdateStatsComplete::CreateLambda([this, puid](const FOnlineError& Result) {
						if (Result == FOnlineError::Success()) {
							UE_LOG(LogTemp, Warning, TEXT("Success"));
							UpdateStatsHTTP(puid);
						}
						else {
							UE_LOG(LogTemp, Error, TEXT("Fail"));
						}}));
			}
		}
	}
}


void UUpdateStats::UpdateStatsHTTP(FString puid) {
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UUpdateStats::OnUpdateHttpComplete);

	Request->SetURL("http://j9a203.p.ssafy.io:8080/api/user/update");
	Request->SetVerb("POST");

	TSharedRef<FJsonObject> JsonObject = MakeShared<FJsonObject>();
	JsonObject->SetStringField("puid", puid);

	if (StatName.Equals("GOLD")) JsonObject->SetNumberField("gold", StatValue);
	else JsonObject->SetNumberField("gold", 0); 

	if (StatName.Equals("KILL")) JsonObject->SetNumberField("kill", StatValue);
	else JsonObject->SetNumberField("kill", 0); 

	if (StatName.Equals("PLAYCOUNT")) JsonObject->SetNumberField("playCount", StatValue);
	else JsonObject->SetNumberField("playCount", 0); 


	FString RequestBody;
	TSharedRef<TJsonWriter<TCHAR>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(JsonObject, Writer);

	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->SetContentAsString(RequestBody);
	Request->ProcessRequest();
}


void UUpdateStats::OnUpdateHttpComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) {
	if (Response->GetResponseCode() == 200) {
		UE_LOG(LogTemp, Warning, TEXT("Success"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Fail"));
	}
}
