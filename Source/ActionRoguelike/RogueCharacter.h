// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RogueCharacter.generated.h"

struct FInputActionInstance;
struct FInputActionValue;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class ACTIONROGUELIKE_API ARogueCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARogueCharacter();

protected:

	// Vamos adicionar o primeiro componente em C++ para o PlayerCharacter e vamos fazer isso aqui, no 'protected'
	// 'protected' significa que não pode ser acessado diretamente de outra classe.
	// já o 'public' significa que pode ser acessado diretamente de outra classe.

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_Move;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_Look;

	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UCameraComponent> CameraComponent;
	// Interessante! Tom mencionou que essencialmente poderiamos utilizar o ponteiro para relacionar o UCameraCompenent a nosso objeto
	// Porém, temos uma nova convenção que será utilizada, utilizaremos o 'TObjectPtr<UCameraComponent>'

	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Criando uma função para utilizar no Input de Move
	void Move(const FInputActionValue& InValue);

	// Criar a mesma forma da nossa função para Look
	void Look(const FInputActionInstance& InValue); // Da mesma forma, declaração forward

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
