// Fill out your copyright notice in the Description page of Project Settings.


#include "EOS_Stats.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineStatsInterface.h" 

UEOS_Stats* UEOS_Stats::ReadStats(TArray<FString> InStatName, UGameInstance* Instance) {
	UEOS_Stats* NewReadStats= NewObject<UEOS_Stats>();
	NewReadStats->StatName= InStatName;
	NewReadStats->GameInstance = Instance;
	return NewReadStats;
}

void UEOS_Stats::Activate() {
	IOnlineSubsystem* SubSystemRef = Online::GetSubsystem(GameInstance->GetWorld());

	if (SubSystemRef) {

		IOnlineIdentityPtr IdentityPointerRef = SubSystemRef->GetIdentityInterface();

		if (IdentityPointerRef) {

			IOnlineStatsPtr StatsPointerRef = SubSystemRef->GetStatsInterface();

			if (StatsPointerRef) {
				TArray<TSharedRef<const FUniqueNetId>> UsersVar;
				UsersVar.Add(IdentityPointerRef->GetUniquePlayerId(0).ToSharedRef());
				StatsPointerRef->QueryStats(IdentityPointerRef->GetUniquePlayerId(0).ToSharedRef(),
					UsersVar, StatName, FOnlineStatsQueryUsersStatsComplete::CreateUObject(this, &UEOS_Stats::OnGetStatsComplete));
			}
		}
	}
}

void UEOS_Stats::OnGetStatsComplete(const FOnlineError& Result, const TArray<TSharedRef<const FOnlineStatsUserStats>>& UsersStatsResult)
{
	TArray<FCustomStatInfo> Ret;
	if (Result.WasSuccessful()) {
		for (auto StatsVar : UsersStatsResult) {
			for (auto StoredValueRef : StatsVar->Stats) {
				FString KeyName = StoredValueRef.Key;
				int32 Value;
				StoredValueRef.Value.GetValue(Value);
				Ret.Add({ KeyName, Value });
				UE_LOG(LogTemp, Warning, TEXT("Stat with name %s is %s"), *KeyName, *FString::FromInt(Value));
			}
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Fail - %s"), *Result.ToLogString());
	}
	OnReadStats.Broadcast(Ret);
	RemoveFromRoot();
}


