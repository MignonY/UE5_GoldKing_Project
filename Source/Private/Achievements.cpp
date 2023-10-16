// Fill out your copyright notice in the Description page of Project Settings.


#include "Achievements.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineAchievementsInterface.h"

UAchievements* UAchievements::ReadAchievements(UGameInstance* Instance) {
	UAchievements* NewReadAchievements = NewObject<UAchievements>();
	NewReadAchievements->GameInstance = Instance;
	return NewReadAchievements;
}

void UAchievements::Activate() {
	IOnlineSubsystem* SubSystemRef = Online::GetSubsystem(GameInstance->GetWorld());

	if (SubSystemRef) {

		IOnlineIdentityPtr IdentityPointerRef = SubSystemRef->GetIdentityInterface();

		if (IdentityPointerRef.IsValid()) {
			IOnlineAchievementsPtr AchievementsRef = SubSystemRef->GetAchievementsInterface();
			
			if (AchievementsRef.IsValid()) {
				AchievementsRef->QueryAchievementDescriptions(*IdentityPointerRef->GetUniquePlayerId(0), FOnQueryAchievementsCompleteDelegate::CreateUObject(this, &UAchievements::QueryAchievements));
			}
		}
	}
}

void UAchievements::QueryAchievements(const FUniqueNetId& UserId, const bool bWasSuccessful) {
	if (bWasSuccessful) {
		IOnlineSubsystem* SubSystemRef = Online::GetSubsystem(GameInstance->GetWorld());

		if (SubSystemRef) {

			IOnlineIdentityPtr IdentityPointerRef = SubSystemRef->GetIdentityInterface();

			if (IdentityPointerRef.IsValid()) {
				IOnlineAchievementsPtr AchievementsRef = SubSystemRef->GetAchievementsInterface();

				if (AchievementsRef.IsValid()) {
					AchievementsRef->QueryAchievements(*IdentityPointerRef->GetUniquePlayerId(0), FOnQueryAchievementsCompleteDelegate::CreateUObject(this, &UAchievements::RetrieveAchievements));
				}
			}
		}
	}
}


void UAchievements::RetrieveAchievements(const FUniqueNetId& UserId, const bool bWasSuccessful) {
	if (bWasSuccessful) {
		IOnlineSubsystem* SubSystemRef = Online::GetSubsystem(GameInstance->GetWorld());

		if (SubSystemRef) {

			IOnlineIdentityPtr IdentityPointerRef = SubSystemRef->GetIdentityInterface();

			if (IdentityPointerRef.IsValid()) {
				IOnlineAchievementsPtr AchievementsRef = SubSystemRef->GetAchievementsInterface();

				if (AchievementsRef.IsValid()) {
					TArray<FOnlineAchievement> Achievements;
					TArray<FCustomAchievement> ReadResult = TArray<FCustomAchievement>();

					AchievementsRef->GetCachedAchievements(*IdentityPointerRef->GetUniquePlayerId(0), Achievements);

					for (auto Data : Achievements) {
						FOnlineAchievementDesc AchievementsDesc;
						AchievementsRef->GetCachedAchievementDescription(Data.Id, AchievementsDesc);
						FCustomAchievement AchievementStruct;
						AchievementStruct.Title = AchievementsDesc.Title;
						AchievementStruct.Desc = AchievementsDesc.UnlockedDesc;
						ReadResult.Add(AchievementStruct);
					}

					UAchievements:: OnReadAchievements.Broadcast(ReadResult);
				}
			}
		}
	}

	RemoveFromRoot();
}
	
