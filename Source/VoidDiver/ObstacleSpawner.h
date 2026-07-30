#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObstacleSpawner.generated.h"

UCLASS()
class VOIDDIVER_API AObstacleSpawner : public AActor
{
	GENERATED_BODY()

public:
	AObstacleSpawner();

protected:
	virtual void BeginPlay() override;

	// 스폰할 장애물 클래스. BP에서 BP_Obstacle 지정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TSubclassOf<class AObstacle> ObstacleClass;

	// 스폰 간격(초). BP에서 튜닝
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	float SpawnInterval = 1.0f;

	// 스폰 높이(Z). 플레이어보다 위
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	float SpawnHeight = 2000.0f;

	// 좌우 랜덤 범위(X). -Range ~ +Range 사이에서 스폰
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	float SpawnRangeX = 500.0f;

	// 풀 크기. 미리 만들어둘 장애물 개수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	int32 PoolSize = 20;

private:
	// 타이머 핸들
	FTimerHandle SpawnTimerHandle;

	// 실제 스폰 함수
	void SpawnObstacle();

	// 장애물 풀(창고)
	UPROPERTY()
	TArray<class AObstacle*> ObstaclePool;

	// 풀 초기화 함수
	void InitializePool();

	// 풀에서 비활성 장애물 하나 반환. 없으면 nullptr
	AObstacle* GetPooledObstacle();
};