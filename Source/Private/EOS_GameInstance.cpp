// Fill out your copyright notice in the Description page of Project Settings.


#include "EOS_GameInstance.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystem.h"

#include "Interfaces/OnlineLeaderboardInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"

#include "Kismet/GameplayStatics.h"



void UEOS_GameInstance::LoginWithEOS(FString ID, FString Token, FString LoginType, FString NewNickname)
{
	IOnlineSubsystem *SubsystemRef = Online::GetSubsystem(this->GetWorld());
	if (SubsystemRef) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Subsystem Found : %s"), *SubsystemRef->GetOnlineServiceName().ToString());
		IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface();
		if (IdentityPointerRef) 
		{
			FOnlineAccountCredentials AccountDetails;
			AccountDetails.Id = ID;
			AccountDetails.Token = Token;
			AccountDetails.Type = LoginType;
			IdentityPointerRef->OnLoginCompleteDelegates->AddUObject(this, &UEOS_GameInstance::LoginWithEOS_Return);
			IdentityPointerRef->OnLoginStatusChangedDelegates->AddUObject(this, &UEOS_GameInstance::ChangeLoginStatus);
			IdentityPointerRef->Login(0, AccountDetails, NewNickname);
		}
	}
}

FString UEOS_GameInstance::GetPlayerUsername()
{	
	IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(this->GetWorld());
	if (SubsystemRef)
	{
		IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface();
		if (IdentityPointerRef)
		{
			if (IdentityPointerRef->GetLoginStatus(0) == ELoginStatus::LoggedIn)
			{
				UE_LOG(LogTemp, Warning, TEXT("Get Player Username Success : %s"), *IdentityPointerRef->GetPlayerNickname(0));
				return *IdentityPointerRef->GetPlayerNickname(0);
				/*
				FUniqueNetIdPtr UserId = IdentityPointerRef->GetUniquePlayerId(0);
				TSharedPtr<FUserOnlineAccount> UserOnlineAccount = IdentityPointerRef->GetUserAccount(*UserId); 
				if (UserOnlineAccount.IsValid())
				{
					FString ReturnValue;
					UserOnlineAccount->GetUserAttribute(TEXT("display_name"), ReturnValue);
					UE_LOG(LogTemp, Warning, TEXT("Get Player Username Success"));
					return ReturnValue;
				}
				*/
			} 
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Failed to Get Player Username"));
	return FString();
}

FString UEOS_GameInstance::UpdatePlayerUsername(FString NewDisplayName)
{
	IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(this->GetWorld());
	if (SubsystemRef)
	{
		IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface();
		if (IdentityPointerRef)
		{
			if (IdentityPointerRef->GetLoginStatus(0) == ELoginStatus::LoggedIn)
			{
				FUniqueNetIdPtr UserId = IdentityPointerRef->GetUniquePlayerId(0);
				TSharedPtr<FUserOnlineAccount> UserOnlineAccount = IdentityPointerRef->GetUserAccount(*UserId);
				
				if (UserOnlineAccount.IsValid())
				{
					bool bSuccess = UserOnlineAccount->SetUserAttribute(TEXT("display_name"), NewDisplayName);
					if (bSuccess)
					{
						UE_LOG(LogTemp, Warning, TEXT("Update Player Username Success"));
						return FString("Update Player Username Success");
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("Failed to Update Player Username"));
						return FString("Failed to Update Player Username");
					}
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("UserOnlineAccount is not valid"));
					return FString("UserOnlineAccount is not valid");
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Not Logged In"));
				return FString("Not Logged In");
			}
		}
	}
	UE_LOG(LogTemp, Error, TEXT("Subsystem or Interface not available"));
	return FString("Subsystem or Interface not available");
}


bool UEOS_GameInstance::IsPlayerLoggedIn()
{
	IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(this->GetWorld());
	if (SubsystemRef)
	{
		IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface();
		if (IdentityPointerRef)
		{
			if (IdentityPointerRef->GetLoginStatus(0) == ELoginStatus::LoggedIn)
			{
				UE_LOG(LogTemp, Warning, TEXT("Is Player Logged In Success"));
				return true;
			}
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Is Player Logged In Failed"));
	return false;
}



void UEOS_GameInstance::LoginWithEOS_Return(int32 LocalUserNum, bool bWasSuccess, const FUniqueNetId& UserId, const FString& Error)
{
	if (bWasSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("Login Success"));
		SubmitLoginInfo();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Login Fail - %s"), *Error);
	}
}

void UEOS_GameInstance::ChangeLoginStatus(int32 LocalUserNum, ELoginStatus::Type NotLoggedIn, ELoginStatus::Type LoggedIn,
	const FUniqueNetId& UserId)
{
}


void UEOS_GameInstance::GetAllEOSGameSessionsCompleted(bool bWasSuccess)
{
	if (bWasSuccess)
	{
		IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(this->GetWorld());
		if (SubsystemRef)
		{
			IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface();
			if (SessionPtrRef)
			{
				if (FindSessionSearch->SearchResults.Num() > 0)
				{
					TArray<FOnlineSessionSearchResult> SearchResults = FindSessionSearch->SearchResults;

					FindSessionInfo.Empty();

					for (const FOnlineSessionSearchResult& SearchResult : SearchResults)
					{
						FString SessionAddress = SearchResult.GetSessionIdStr();
						FString UserID = (SearchResult.Session.OwningUserId.IsValid()) ? SearchResult.Session.OwningUserId->ToString() : TEXT("Invalid");
						FString UserName = SearchResult.Session.OwningUserName;
						int32 CurrentPlayer = SearchResult.Session.SessionSettings.NumPublicConnections - SearchResult.Session.NumOpenPublicConnections;
						int32 MaxPlayer = SearchResult.Session.SessionSettings.NumPublicConnections;
						int32 RestPlayer = SearchResult.Session.NumOpenPublicConnections;
						FString SessionName;
						SearchResult.Session.SessionSettings.Get(SEARCH_KEYWORDS, SessionName);

						FSessionInfoStruct SessionInfo;

						SessionInfo.SessionAddress = SessionAddress;
						SessionInfo.SessionName = SessionName;
						SessionInfo.UserID = UserID;
						SessionInfo.UserName = UserName;
						SessionInfo.CurrentPlayer = CurrentPlayer;
						SessionInfo.MaxPlayer = MaxPlayer;
						SessionInfo.RestPlayer = RestPlayer;

						FindSessionInfo.Add(SessionInfo);

						CreateSessionInfo = SessionInfo;

						UE_LOG(LogTemp, Warning, TEXT("Session Name : %s"), *SessionName)
						UE_LOG(LogTemp, Warning, TEXT("Session Address: %s"), *SessionAddress)
						UE_LOG(LogTemp, Warning, TEXT("Owning User ID: %s"), *UserID)
						UE_LOG(LogTemp, Warning, TEXT("Owning User Name: %s"), *UserName)
						UE_LOG(LogTemp, Warning, TEXT("Number of Public Connections: %d"), MaxPlayer)
						UE_LOG(LogTemp, Warning, TEXT("Number of Open Public Connections: %d"), RestPlayer)
						UE_LOG(LogTemp, Warning, TEXT("Current Players: %d"), CurrentPlayer)
					}
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("No Sessions Found."));
				}
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to Find Sessions."));
	}
}

void UEOS_GameInstance::GetAllEOSGameSessions()
{
	IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(this->GetWorld());
	if (SubsystemRef)
	{
		IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface();
		if (SessionPtrRef)
		{

			FindSessionSearch = MakeShareable(new FOnlineSessionSearch());
			FindSessionSearch->bIsLanQuery = false;
			FindSessionSearch->MaxSearchResults = 50;
			FindSessionSearch->QuerySettings.SearchParams.Empty();

			SessionPtrRef->OnFindSessionsCompleteDelegates.RemoveAll(this);
			SessionPtrRef->OnFindSessionsCompleteDelegates.AddUObject(this, &UEOS_GameInstance::GetAllEOSGameSessionsCompleted);
			SessionPtrRef->FindSessions(0, FindSessionSearch.ToSharedRef());
		}
	}
}

bool UEOS_GameInstance::UnregisterPlayerFromSession(FString SessionName)
{
	IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(this->GetWorld());
	if (SubsystemRef)
	{
		IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface();
		if (SessionPtrRef)
		{
			IOnlineIdentityPtr IdentityInterface = SubsystemRef->GetIdentityInterface();
			if (IdentityInterface.IsValid())
			{
				FUniqueNetIdPtr PlayerId = IdentityInterface->GetUniquePlayerId(0); // 플레이어 인덱스를 지정합니다.
				if (PlayerId.IsValid())
				{
					if (SessionPtrRef->UnregisterPlayer(FName(SessionName), *PlayerId))
					{
						UE_LOG(LogTemp, Warning, TEXT("Player unregistered from session: %s"), *PlayerId->ToString());
						return true;
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("Player Unregistration Failed"));
					}
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Invalid Player ID. Player unregistration failed."));
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Invalid IdentityInterface."));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Invalid SessionPtrRef."));
		}
	}
	return false;
}


void UEOS_GameInstance::OnCreateSessionCompleted(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		GetWorld()->ServerTravel("/Game/GameSystem/InSession_BeforeGame/BeforeGameStartLevel?listen");

		FString JoinAddress;
		IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(this->GetWorld());
		if (SubsystemRef)
		{
			IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface();
			if (SessionPtrRef)
			{
				UE_LOG(LogTemp, Warning, TEXT("Created EOS Session Name is %s"), *SessionName.ToString());
				SessionPtrRef->GetResolvedConnectString(SessionName, JoinAddress);
				UE_LOG(LogTemp, Warning, TEXT("Created EOS Session Address is %s"), *JoinAddress);
				CreateSessionName = SessionName.ToString();
				CreateSessionID = JoinAddress;

				IOnlineIdentityPtr IdentityInterface = SubsystemRef->GetIdentityInterface();
				if (IdentityInterface.IsValid())
				{
					FUniqueNetIdPtr PlayerId = IdentityInterface->GetUniquePlayerId(0); // 플레이어 인덱스를 지정합니다.
					if (PlayerId.IsValid())
					{
						SessionPtrRef->RegisterPlayer(SessionName, *PlayerId, true);
						UE_LOG(LogTemp, Warning, TEXT("Player registered in session: %s"), *PlayerId->ToString());
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("Invalid Player ID. Player registration failed."));
					}
				}

			}
		}
	}
}

void UEOS_GameInstance::CreateEOSSession(bool bIsDedicatedServer, bool bIsLanServer, bool AllowInvites, bool bShouldAdvertise, FString SessionTitle, int32 NumberOfPublicConnections)
{
	IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(this->GetWorld());
	if (SubsystemRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Subsystem found : %s"), *SubsystemRef->GetOnlineServiceName().ToString());
		IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface();
		if (SessionPtrRef)
		{
			UE_LOG(LogTemp, Warning, TEXT("SessionPtr found"));

			FOnlineSessionSettings SessionCreationInfo;

			SessionCreationInfo.bIsDedicated = bIsDedicatedServer;
			SessionCreationInfo.bAllowInvites = AllowInvites;
			SessionCreationInfo.bIsLANMatch = bIsLanServer;
			SessionCreationInfo.bShouldAdvertise = bShouldAdvertise;
			SessionCreationInfo.NumPublicConnections = NumberOfPublicConnections;
			SessionCreationInfo.bUseLobbiesIfAvailable = false;
			SessionCreationInfo.bUsesPresence = false;
			SessionCreationInfo.bAllowJoinInProgress = true;
			SessionCreationInfo.bAllowJoinViaPresence = true;
			SessionCreationInfo.bUsesStats = true;

			SessionCreationInfo.Set(SEARCH_KEYWORDS, SessionTitle, EOnlineDataAdvertisementType::ViaOnlineService);

			SessionPtrRef->OnCreateSessionCompleteDelegates.AddUObject(this, &UEOS_GameInstance::OnCreateSessionCompleted);
			SessionPtrRef->CreateSession(0, FName(SessionTitle), SessionCreationInfo);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No SessionPtr Found"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Online Subsyetem failed"));
	}
}




void UEOS_GameInstance::OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	FString SessionTitle = SessionName.ToString();

	UE_LOG(LogTemp, Warning, TEXT("Current Session Name is %s"), *SessionTitle);

	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		if (APlayerController* PlayerControllerRef = UGameplayStatics::GetPlayerController(GetWorld(), 0))
		{
			FString JoinAddress;
			IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(this->GetWorld());
			if (SubsystemRef)
			{
				IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface();
				if (SessionPtrRef)
				{
					SessionPtrRef->GetResolvedConnectString(SessionName, JoinAddress);
					UE_LOG(LogTemp, Warning, TEXT("Join Address is %s"), *JoinAddress);
					if (!JoinAddress.IsEmpty())
					{
						PlayerControllerRef->ClientTravel(JoinAddress, TRAVEL_Absolute);
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("Empty Join Address is %s"), *JoinAddress);
					}
				}
			}
		}
	}
}


void UEOS_GameInstance::OnJoinSessionByIdCompleted(bool bWasSuccess)
{
	if (bWasSuccess)
	{
		IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(this->GetWorld());
		if (SubsystemRef)
		{
			IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface();
			if (SessionPtrRef)
			{
				if (JoinSessionSearch->SearchResults.Num() > 0)
				{
					TArray<FOnlineSessionSearchResult> SearchResults = JoinSessionSearch->SearchResults;

					for (const FOnlineSessionSearchResult& SearchResult : SearchResults)
					{
						FString SessionName;

						if (SearchResult.GetSessionIdStr() == TargetSessionID)
						{
							SearchResult.Session.SessionSettings.Get(SEARCH_KEYWORDS, SessionName);

							IOnlineIdentityPtr IdentityInterface = SubsystemRef->GetIdentityInterface();
							if (IdentityInterface.IsValid())
							{
								FUniqueNetIdPtr PlayerId = IdentityInterface->GetUniquePlayerId(0);
								if (PlayerId.IsValid())
								{
									if (SessionPtrRef->RegisterPlayer(FName(SessionName), *PlayerId, true))
									{
										UE_LOG(LogTemp, Warning, TEXT("Player registered from session: %s"), *PlayerId->ToString())
									}
									else
									{
										UE_LOG(LogTemp, Warning, TEXT("Player registration Failed"));
										return;
									}
								}
								else
								{
									UE_LOG(LogTemp, Error, TEXT("Invalid Player ID. Player registration failed."));
									return;
								}
							}
							else
							{
								UE_LOG(LogTemp, Error, TEXT("Invalid IdentityInterface."));
								return;
							}

							SessionPtrRef->OnJoinSessionCompleteDelegates.RemoveAll(this);
							SessionPtrRef->OnJoinSessionCompleteDelegates.AddUObject(this, &UEOS_GameInstance::OnJoinSessionCompleted);

							if (SessionPtrRef->JoinSession(0, FName(SessionName), SearchResult))
							{
								UE_LOG(LogTemp, Warning, TEXT("Joining session: %s"), *TargetSessionID);
								UE_LOG(LogTemp, Warning, TEXT("Session ID: %s"), *SessionName);
								UE_LOG(LogTemp, Warning, TEXT("Join Success"));
							}
							else
							{
								UE_LOG(LogTemp, Warning, TEXT("Failed to join Session - Session Address: %s"), *TargetSessionID);
								UE_LOG(LogTemp, Warning, TEXT("Failed to join Session - Session ID: %s"), *SessionName);
							}
							break;
						}
					}
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("No Session Found"));
				}
			}
		}
	}
}


void UEOS_GameInstance::JoinSessionById(const FString& SessionID)
{
	TargetSessionID = *SessionID;

	IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(this->GetWorld());
	if (SubsystemRef)
	{
		IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface();
		if (SessionPtrRef)
		{
			JoinSessionSearch = MakeShareable(new FOnlineSessionSearch());
			JoinSessionSearch->bIsLanQuery = false;
			JoinSessionSearch->MaxSearchResults = 50;
			JoinSessionSearch->QuerySettings.SearchParams.Empty();

			SessionPtrRef->OnFindSessionsCompleteDelegates.RemoveAll(this);
			SessionPtrRef->OnFindSessionsCompleteDelegates.AddUObject(this, &UEOS_GameInstance::OnJoinSessionByIdCompleted);
			SessionPtrRef->FindSessions(0, JoinSessionSearch.ToSharedRef());
		}
	}
}

void UEOS_GameInstance::OnStartOnlineSessionCompleted(FName SessionName, bool bWasSuccess)
{
	if (bWasSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("Update Online Session Success : %s"), *SessionName.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Update Online Session Failed : %s"), *SessionName.ToString());
	}
}

void UEOS_GameInstance::StartOnlineSession(FString SessionName)
{
	IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(this->GetWorld());
	if (SubsystemRef)
	{
		IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface();
		if (SessionPtrRef)
		{
			FNamedOnlineSession* Session = SessionPtrRef->GetNamedSession(FName(SessionName));
			if (Session)
			{
				SessionPtrRef->OnStartSessionCompleteDelegates.RemoveAll(this);
				SessionPtrRef->OnStartSessionCompleteDelegates.AddUObject(this, &UEOS_GameInstance::OnStartOnlineSessionCompleted);

				SessionPtrRef->StartSession(FName(SessionName));
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Session %s not found."), *SessionName);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Session interface is not valid."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Online subsystem not found."));
	}
}



void UEOS_GameInstance::OnUpdateOnlineSessionCompleted(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Update Online Session Complete : %s"), *SessionName.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Update Online Session Failed"));
	}
}


void UEOS_GameInstance::UpdateOnlineSession(FString SessionName)
{
	UE_LOG(LogTemp, Warning, TEXT("Update Online Session Execute"));

	IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(this->GetWorld());
	if (SubsystemRef)
	{
		IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface();
		if (SessionPtrRef)
		{
			FOnlineSessionSettings SessionCreationInfo;

			SessionCreationInfo.bAllowInvites = false;
			SessionCreationInfo.bShouldAdvertise = false;
			SessionCreationInfo.bAllowJoinInProgress = false;
			SessionCreationInfo.bAllowJoinViaPresence = false;
			SessionCreationInfo.bUsesPresence = false;

			SessionPtrRef->OnUpdateSessionCompleteDelegates.RemoveAll(this);
			SessionPtrRef->OnUpdateSessionCompleteDelegates.AddUObject(this, &UEOS_GameInstance::OnUpdateOnlineSessionCompleted);

			SessionPtrRef->UpdateSession(FName(SessionName), SessionCreationInfo);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UEOS_GameInstance::OnDestroySessionCompleted(FName SessionName, bool bWasSuccessful)
{

}

void UEOS_GameInstance::DestroySession(FString SessionName)
{
	IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(this->GetWorld());
	if (SubsystemRef)
	{
		IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface();
		if (SessionPtrRef)
		{
			SessionPtrRef->OnDestroySessionCompleteDelegates.AddUObject(this, &UEOS_GameInstance::OnDestroySessionCompleted);

			SessionPtrRef->DestroySession(FName(SessionName));

			UE_LOG(LogTemp, Warning, TEXT("Session Destroyed : %s"), *SessionName);
		}
	}
}


void UEOS_GameInstance::OnFindSessionCompleted(bool bWasSuccess)
{
	if (bWasSuccess)
	{
		IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(this->GetWorld());
		if (SubsystemRef)
		{
			IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface();
			if (SessionPtrRef)
			{
				if (AutoJoinSessionSearch->SearchResults.Num() > 0)
				{
					FString SessionName;
					AutoJoinSessionSearch->SearchResults[0].Session.SessionSettings.Get(SEARCH_KEYWORDS, SessionName);

					SessionPtrRef->OnJoinSessionCompleteDelegates.AddUObject(this, &UEOS_GameInstance::OnJoinSessionCompleted);
					SessionPtrRef->JoinSession(0, FName("MainSessionTest"), AutoJoinSessionSearch->SearchResults[0]);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Failed to Find Address"));
				}
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Error to Find Session"))
	}
}

void UEOS_GameInstance::SubmitLoginInfo() {
	IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(this->GetWorld());
	if (SubsystemRef)
	{
		IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface();
		if (IdentityPointerRef)
		{
			FString puid = IdentityPointerRef->GetUniquePlayerId(0)->ToString();
			FString username = *IdentityPointerRef->GetPlayerNickname(0);

			TSharedRef<IHttpRequest> Request = Http->CreateRequest();
			Request->OnProcessRequestComplete().BindUObject(this, &UEOS_GameInstance::OnSubmitLoginInfoComplete);

			Request->SetURL("http://j9a203.p.ssafy.io:8080/api/user/signup");
			Request->SetVerb("POST");

			TSharedRef<FJsonObject> JsonObject = MakeShared<FJsonObject>();
			JsonObject->SetStringField("puid", puid);
			JsonObject->SetStringField("signUpNickname", username);

			FString RequestBody;
			TSharedRef<TJsonWriter<TCHAR>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
			FJsonSerializer::Serialize(JsonObject, Writer);

			Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
			Request->SetHeader("Content-Type", TEXT("application/json"));
			Request->SetContentAsString(RequestBody);
			Request->ProcessRequest();
		}
	}
}

void UEOS_GameInstance::OnSubmitLoginInfoComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) {
	if (Response->GetResponseCode() == 201) {
		UE_LOG(LogTemp, Warning, TEXT("Success"));
	}
	else if (Response->GetResponseCode() == 400) {
		UE_LOG(LogTemp, Warning, TEXT("Already Signed up"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("FAILED"));
	}
}

