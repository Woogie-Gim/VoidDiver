#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Obstacle.generated.h"

UCLASS()
class VOIDDIVER_API AObstacle : public AActor
{
	GENERATED_BODY()

public:
	AObstacle();

protected:
	virtual void Tick(float DeltaTime) override;

	// 장애물 외형. BP에서 메시 지정
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Obstacle")
	class UStaticMeshComponent* MeshComp;

	// 아래로 흐르는 속도(cm/s). BP에서 튜닝
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Obstacle")
	float FallSpeed = 500.0f;

	// 이 높이(Z)보다 아래로 내려가면 스스로 제거
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Obstacle")
	float KillZ = -2000.0f;
};