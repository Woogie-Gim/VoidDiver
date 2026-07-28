// DiverCharacter.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DiverCharacter.generated.h"

// 전방 선언 - 헤더 의존성 최소화
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class VOIDDIVER_API ADiverCharacter : public ACharacter
{
	GENERATED_BODY()

	public:
	ADiverCharacter();

protected:
	virtual void BeginPlay() override;

	// 입력 바인딩 설정
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// BP에서 지정할 입력 에셋들. EditDefaultsOnly로 CDO에서만 설정
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	// 이동 속도. BP에서 튜닝 가능하게 노출
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveScale = 1.0f;

	// IA_Move 입력 시 호출될 함수
	void Move(const FInputActionValue& Value);

	// 카메라 붐(팔). 캐릭터 뒤/위에서 따라옴
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent* CameraBoom;

	// 실제 시점 카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* FollowCamera;

	// 카메라 거리. BP에서 튜닝 가능
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float BoomLength = 800.0f;

	// 아래를 내려다보는 각도(음수 = 아래로). 낙하 시점용
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float CameraPitch = -30.0f;
};