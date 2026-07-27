// DiverCharacter.cpp
#include "DiverCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"

ADiverCharacter::ADiverCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ADiverCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 로컬 플레이어의 EIS 서브시스템에 기본 매핑 컨텍스트 등록
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}

void ADiverCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// EIS 전용 컴포넌트로 캐스팅
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// MoveAction이 Triggered 상태일 때 Move 함수 호출
		if (MoveAction)
		{
			EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADiverCharacter::Move);
		}
	}
}

void ADiverCharacter::Move(const FInputActionValue& Value)
{
	// Axis2D 입력을 Vector2D로 받음. X=좌우, Y=앞뒤
	const FVector2D Input = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// 월드 기준 단순 이동. 낙하 게임이라 카메라 회전과 무관하게 축 고정
		AddMovementInput(FVector::RightVector, Input.X * MoveScale);   // 좌우
		AddMovementInput(FVector::ForwardVector, Input.Y * MoveScale); // 앞뒤
	}
}