#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DiverGameMode.generated.h"

UCLASS()
class VOIDDIVER_API ADiverGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ADiverGameMode();

	virtual void Tick(float DeltaTime) override;

	// 현재 점수 반환 (HUD에서 읽음)
	UFUNCTION(BlueprintPure, Category = "Game")
	int32 GetScore() const { return CurrentScore; }

	// 게임오버 처리. 장애물이 충돌 시 호출
	UFUNCTION(BlueprintCallable, Category = "Game")
	void OnGameOver();

	// 현재 게임오버 상태인지
	UFUNCTION(BlueprintPure, Category = "Game")
	bool IsGameOver() const { return bIsGameOver; }

	UFUNCTION(BlueprintImplementableEvent, Category = "Game")
	void OnGameOverBP();

protected:
	virtual void BeginPlay() override;

	// 게임오버 상태 플래그
	bool bIsGameOver = false;

	// 누적 생존 시간(초)
	float SurvivalTime = 0.0f;

	// 초당 점수. BP에서 튜닝
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	float ScorePerSecond = 10.0f;

	// 현재 점수(정수)
	int32 CurrentScore = 0;
};