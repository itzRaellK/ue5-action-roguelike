#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
// O cabeçalho gerado pelo Unreal Header Tool deve ser o último #include deste arquivo.
#include "RogueCharacter.generated.h"

// Declarações antecipadas (forward declarations): informam que estes tipos existem sem incluir
// suas definições completas aqui. São suficientes para os ponteiros e as referências usados abaixo;
// para acessar os membros desses tipos no .cpp, é necessário incluir os cabeçalhos correspondentes.
class ARogueProjectileMagic;
struct FInputActionInstance;
struct FInputActionValue;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;

UCLASS(Abstract)
class ACTIONROGUELIKE_API ARogueCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ARogueCharacter();

protected:

	UPROPERTY(EditDefaultsOnly, Category="PrimaryAttack")
	TSubclassOf<ARogueProjectileMagic> ProjectileClass;

	UPROPERTY(VisibleAnywhere, Category="PrimaryAttack")
	FName MuzzleSocketName;

	// Em C++, membros protected podem ser acessados pela própria classe, por classes derivadas
	// e por seus amigos (friend), respeitando as regras de acesso da linguagem.
	// Membros public também podem ser acessados por código externo com acesso ao objeto.
	// A exposição ao editor e aos Blueprints é definida separadamente pelos especificadores da Unreal.

	// Estas propriedades guardam referências aos assets UInputAction, não a componentes do personagem.
	// EditDefaultsOnly permite configurar os valores padrão da classe no editor, inclusive em um
	// Blueprint derivado, mas não sobrescrevê-los em cada instância colocada no nível.
	// Category apenas organiza a exibição; o mapeamento entre teclas/eixos e ações fica no Input Mapping Context.
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_Move;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_Look;

	// Referência à ação que será associada a PrimaryAttack em SetupPlayerInputComponent.
	// Declarar a propriedade não cria o asset nem faz a associação com uma tecla automaticamente.
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_PrimaryAttack;

	// TObjectPtr<T> armazena uma referência a um UObject do tipo T, não uma cópia do objeto.
	// Em uma UPROPERTY, essa referência forte é rastreada pelo coletor de lixo da Unreal enquanto
	// o objeto que a contém estiver alcançável. Ela não cria o componente nem impede Destroy() explícito.
	// Os componentes são criados com CreateDefaultSubobject no construtor, sem usar new/delete manualmente.
	// VisibleAnywhere exibe a referência no editor sem permitir sua substituição nessa propriedade;
	// as propriedades internas do componente continuam sujeitas às próprias regras de edição.
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	virtual void BeginPlay() override;

	// Declara o método que recebe o valor da ação de movimento. const T& passa uma referência
	// sem copiar a estrutura e impede sua alteração por esse parâmetro dentro do método.
	void Move(const FInputActionValue& InValue);

	// FInputActionInstance fornece o valor da ação e informações como evento de gatilho e tempos.
	// Esta é a declaração de um método; as declarações antecipadas dos tipos estão no início do arquivo.
	void Look(const FInputActionInstance& InValue); // A implementação do método fica no .cpp.

	// Método de ataque, não um módulo da Unreal. Um módulo agrupa código e dependências de compilação,
	// como ActionRoguelike e EnhancedInput; suas regras são definidas em arquivos .Build.cs.
	// O Enhanced Input aceita esta assinatura sem parâmetros quando não precisamos consultar o valor da ação.
	void PrimaryAttack();

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
