// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueCharacter.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ARogueCharacter::ARogueCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Vamos inicializar o SpringArmComponent antes da Camera, pois queremos que a Camera fique presa a ele.
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComponent->SetupAttachment(RootComponent);
	// Vamos ativar o pawn rotation via C++
	SpringArmComponent->bUsePawnControlRotation = true;

	// No Construtor vamos iniciar a camera
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComponent->SetupAttachment(SpringArmComponent); // Aqui trocamos para o SpringArmComponent, assim ele fica preso ao SpringArm e não ao Root do Character
}

// Called when the game starts or when spawned
void ARogueCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Módulo Move do Character
void ARogueCharacter::Move(const FInputActionValue& InValue)
{
	FVector2D InputValue = InValue.Get<FVector2D>();
	
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;

	// Aqui acontece uma lógica interessanten, como o AddMovementInput tem alguns parametros que podemos alterar, podemos aplicar
	// uma lógica de movimentação para frente e para a trás utilizando 'scale'. Isso acontece pois já recebemos esses valores (positivos ou negativos) do próprio input
	
	// Forward/Back
	AddMovementInput(ControlRot.Vector(), InputValue.X);
	
	// Right/Left
	FVector RightDirection = ControlRot.RotateVector(FVector::RightVector);
	AddMovementInput(RightDirection, InputValue.Y);
}

// Módulo Look do Character
void ARogueCharacter::Look(const FInputActionInstance& InValue)
{
	// Guardamos o valor do Input Y para usar no Pitch (Up - Down)
	FVector2D InputValue = InValue.GetValue().Get<FVector2D>();

	// Em tese seria o mesmo código para utilizar os valores que X ou Z receberem (caso esteja utilizando os movimentos laterais e rotacionais)
	// Dentro dos Inputs de mouse, existem o Pitch (pra cima / pra baixo), Yaw (direita / esquerda), e Row (rotação)
	AddControllerPitchInput(InputValue.Y);

	AddControllerYawInput(InputValue.X); // Acredito que seja isso
}

// Called every frame
void ARogueCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARogueCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInput->BindAction(Input_Move, ETriggerEvent::Triggered, this, &ARogueCharacter::Move); // Vamos duplicar essa linha pois funcionará da mesma forma para o Input Look
	EnhancedInput->BindAction(Input_Look, ETriggerEvent::Triggered, this, &ARogueCharacter::Look); 
}

