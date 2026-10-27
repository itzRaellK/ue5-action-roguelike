
#include "RogueCharacter.h"
#include "Projectiles/RogueProjectileMagic.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

ARogueCharacter::ARogueCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Cria o braço de câmera como subobjeto padrão do personagem e configura sua ligação ao componente raiz.
	// O braço é criado primeiro porque será o componente pai da câmera na hierarquia de transformações.
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComponent->SetupAttachment(RootComponent);
	// Faz o braço usar a rotação de visão do Pawn, normalmente baseada na ControlRotation do Controller,
	// respeitando as opções de herança de rotação. Isso orienta a câmera ligada ao braço;
	// não significa, por si só, girar o corpo do personagem.
	SpringArmComponent->bUsePawnControlRotation = true;

	// Cria a câmera como subobjeto padrão. SetupAttachment configura a ligação para o registro
	// dos componentes e é apropriado no construtor, antes de eles estarem registrados no mundo.
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComponent->SetupAttachment(SpringArmComponent); // A câmera acompanha o braço, ligado à raiz do personagem.

	MuzzleSocketName = "Muzzle_01";
}

void ARogueCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARogueCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Cast verifica o tipo do objeto da Unreal e devolve um ponteiro para ele como UEnhancedInputComponent.
	// Não cria outro componente; se o objeto não for compatível, o resultado será nullptr.
	// As chamadas abaixo pressupõem que o componente recebido é desse tipo.
	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	// BindAction associa a ação e seu evento ao método que será executado neste objeto (this).
	// &ARogueCharacter::Move é um ponteiro para função-membro; a expressão não chama Move imediatamente.
	// Triggered depende dos gatilhos configurados na ação e pode ocorrer em vários quadros enquanto
	// suas condições forem satisfeitas; não significa necessariamente uma única chamada ao pressionar.
	EnhancedInput->BindAction(Input_Move, ETriggerEvent::Triggered, this, &ARogueCharacter::Move); // Look usa a mesma forma de associação.
	EnhancedInput->BindAction(Input_Look, ETriggerEvent::Triggered, this, &ARogueCharacter::Look); 

	// Associa a ação de ataque ao método PrimaryAttack, usando o mesmo evento Triggered.
	EnhancedInput->BindAction(Input_PrimaryAttack, ETriggerEvent::Triggered, this, &ARogueCharacter::PrimaryAttack); 
}

// Implementa o método de movimento declarado no cabeçalho, usando a orientação do Controller.
void ARogueCharacter::Move(const FInputActionValue& InValue)
{
	// Extrai dois eixos; a ação precisa estar configurada como Axis2D.
	// Neste código, X controla frente/trás e Y controla direita/esquerda; os mapeamentos devem corresponder a isso.
	FVector2D InputValue = InValue.Get<FVector2D>();
	
	// Remove a inclinação vertical da rotação usada para calcular as direções de movimento.
	// ControlRot é uma cópia local: alterar seu Pitch não modifica a rotação real do Controller.
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;

	// AddMovementInput recebe uma direção no mundo e uma escala de entrada. Uma escala negativa inverte
	// o sentido e sua magnitude representa a intensidade solicitada, não uma velocidade em cm/s.
	// A entrada é acumulada para o sistema de movimento do Character processar, em vez de mover o ator imediatamente.
	
	// Vector() obtém a direção para a frente dessa rotação; o sinal de X distingue avançar de recuar.
	AddMovementInput(ControlRot.Vector(), InputValue.X);
	
	// Converte o eixo local direito da Unreal (+Y) para uma direção no mundo usando ControlRot.
	// O sinal de Y determina se a entrada aponta nessa direção ou no sentido oposto.
	FVector RightDirection = ControlRot.RotateVector(FVector::RightVector);
	AddMovementInput(RightDirection, InputValue.Y);
}

// Implementa o método de visão: os dois eixos da ação alimentam a rotação do Controller.
void ARogueCharacter::Look(const FInputActionInstance& InValue)
{
	// Obtém o valor da instância da ação e o interpreta como vetor 2D, contendo os eixos X e Y.
	// Get<FVector2D>() pressupõe uma ação Axis2D; aqui os dois eixos serão usados.
	FVector2D InputValue = InValue.GetValue().Get<FVector2D>();

	// Pitch é a inclinação para cima/baixo (rotação em torno de Y); Yaw é o giro para os lados (em torno de Z).
	// Roll é a inclinação lateral (em torno de X), não "Row". Esses são ângulos de rotação,
	// não eixos próprios do mouse: o mapeamento abaixo associa o valor Y da ação ao Pitch e X ao Yaw.
	// O sentido e a sensibilidade finais também dependem dos modificadores e das configurações de entrada.
	AddControllerPitchInput(InputValue.Y);

	AddControllerYawInput(InputValue.X); // Usa o eixo horizontal da ação para o giro lateral.
}

// Implementação do método de ataque associado ao input. O corpo ainda está vazio;
// receber o evento de entrada, por si só, não cria nem dispara um projétil.
void ARogueCharacter::PrimaryAttack()
{
	FVector SpawnLocation = GetMesh()->GetSocketLocation(MuzzleSocketName);
	FRotator SpawnRotation = GetControlRotation();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
}

void ARogueCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

