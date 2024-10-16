// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IHttpRequest.h"
#include "NewHttpActor.generated.h"

UENUM()
enum class EServerType : uint8
{
	Pkh = 0,
	Lsh,
	Local
};

UCLASS()
class VICTROYSHIP_API ANewHttpActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ANewHttpActor();

protected:
	virtual void BeginPlay() override;

// NPC conversation
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class AFarmLifeGameMode> MyGameMode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString BaseURL = TEXT("");

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EServerType ChooseServer = EServerType::Pkh;

	FString PkhURL = TEXT("http://192.168.240.91:3172");
	FString LshURL = TEXT("http://192.168.240.81:3172");
	FString LocalURL = TEXT("http://127.0.0.1:8000");

	// STT
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString EndPoint_SendSpeech = TEXT("/post-speech");

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString EndPoint_GetSpeech = TEXT("/get-speech");

	// ChatBot response
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString EndPoint_SendConv = TEXT("/post-conv");

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString EndPoint_GetConv = TEXT("/get-conv");

	// Direct text
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString EndPoint_SendText = TEXT("/post-text");

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString EndPoint_GetText = TEXT("/get-text");

	// TTS
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString EndPoint_GetTTS = TEXT("/get-tts");

	// Greeting
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString EndPoint_InitGreeting = TEXT("/init-greeting");

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString EndPoint_PostGreeting = TEXT("/post-greeting");

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString EndPoint_GetGreetingData = TEXT("/get-greeting-data");

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString EndPoint_GetGreetingTTS = TEXT("/get-greeting-tts");

	// Present
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString EndPoint_PostPresent = TEXT("/post-present");

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString EndPoint_GetPresentData = TEXT("/get-present-data");

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString EndPoint_GetPresentTTS = TEXT("/get-present-tts");

	// End Chat
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString EndPoint_EndChat = TEXT("/end-chat");

	// Extra Folder Directory
	UPROPERTY(EditDefaultsOnly)
	FString ExtraFolder = TEXT("Extras/");

	UPROPERTY(EditDefaultsOnly)
	FString ExtraPath;

public:
	// STT
	void SendSpeech(const FString& FilePath);
	void SendSpeechComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	void ReqTextFromSpeech();
	void ReqTextFromSpeechComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	// ChatBot response
	void SendConv(const FString& NPCName, int32 Preference, int32 Hour);
	void SendConvComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	void GetConv();
	void GetConvComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	// Text
	void SendText(const FString& NPCName, const FString& InputText, int32 Preference, int32 Hour);
	void SendTextComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	void GetText();
	void GetTextComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	// TTS
	void GetTTS();
	void GetTTSComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	// Greeting
	void InitGreeting(const FString& NPCName, int32 Likeability);
	void InitGreetingComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	void RequestGreeting(const FString& NPCName);
	void RequestGreetingComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	void GetGreetingData();
	void GetGreetingDataComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	void GetGreetingTTS();
	void GetGreetingTTSComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	// Present
	void RequestPresent(const FString& NPCName, const FString& ItemName, int32 Likeability, bool IsPrefer);
	void RequestPresentComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	void GetPresentData();
	void GetPresentDataComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	void GetPresentTTS();
	void GetPresentTTSComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	// End Chat
	void EndChat(const FString& NPCName);
	void EndChatComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

// Talk to plant
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString EndPoint_TalkToPlant = TEXT("/post-talk2plant");

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString EndPoint_GetSpeechScore = TEXT("/get-talk2plant");

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString EndPoint_TextToPlant = TEXT("/post-text2plant");

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString EndPoint_GetTextScore = TEXT("/get-text2plant");

public:
	// STT
	void TalkToPlant(const  FString& FilePath);
	void TalkToPlantComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	void ReqScore();
	void ReqScoreComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	// Text
	void TalkToPlantWithText(const FString& InputText);
	void TalkToPlantWithTextComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	void ReqScoreWithText();
	void ReqScoreWithTextComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

// EndGame
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString EndPoint_EndGame = TEXT("/get-clearHistory");

public:
	void EndGame();
};
