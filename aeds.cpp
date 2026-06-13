#include <iostream>
#include <string>
#include <cstdlib> //Permite o sistema de sorteios. rand () % 10 escolhe aleatoriamente uma posição do vetor de jogadores
                   //Srand faz com que o rand mude em cada partida, sem o srand todas as partidas seriam iguais
#include <ctime> // Relaciona o horário atual do pc com o tempo do jogo
using namespace std;


// ================= ENUMS =================

enum Personagem { // Dafny(D): Mudei o nome de "Função" para "Personagem" para não haver nomes repetidos durante o código. 
    ASSASSINO,
    MEDICO,
    VIDENTE,
    CIDADAO
};

enum EstadoJogo { // Controla em qual etapa da partida o sistema está.
    MENU,
    DISTRIBUICAO,
    NOITE,
    DIA,
    VOTACAO,
    FIM
};

// ================= STRUCTS =================

struct Jogador{ // Armazena todas as informações de cada jogador.
    string nome;
    Personagem funcao;
    bool vivo;
    int votosRecebidos;
    bool protegido;
    int suspeita;
};

struct HistoricoRodada{ // Serve para registrar tudo o que aconteceu em cada rodada.
    int rodada;
    string noite;
    string dia;
    string votacao;
};

//Possíveis funções que vamos utilizar e desenvolver (PROTÓTIPOS)

void menuPrincipal(); // (D): Feito! 

void cadastrarJogadores();

void iniciarPartida();

void distribuirFuncoes();

void mostrarFuncaoJogadores();

void executarNoite();

void executarDia();

void realizarVotacao();

bool verificarVitoria();

void mostrarHistorico();

void mostrarRegras();

void relatorioFinal();

void limparPartida();

// ================= MAIN =================

int main() {

    srand(time(NULL)); // Permite a mudança de funções entre as partidas

    HistoricoRodada historico[30];
int quantidadeJogadores;
int quantidadeRodadas;

    menuPrincipal();

return 0;
}

// ================= MENU =================
/*
OBS: A implementação da função menuPrincipal() está após a main() porque seu
protótipo foi declarado anteriormente. Isso melhora a organização do código
e mantém a função principal mais limpa e fácil de entender.
*/

void menuPrincipal(){
    int opcao; 

    do {

        cout << "\n====== CIDADE DORME ======\n";
        cout << "1 - Cadastrar jogadores\n"; 
        cout << "2 - Iniciar partida\n";
        cout << "3 - Regras\n";
        cout << "4 - Historico da ultima partida\n";
        cout << "5 - Sair\n";

        cout << "\nEscolha uma opcao: ";
        cin >> opcao;

        switch(opcao){

            case 1:
                cadastrarJogadores(); 
                break;
            
            case 2: 
                iniciarPartida();
                break;
            
            case 3: 
                mostrarRegras();
                break; 
            
            case 4: 
                mostrarHistorico();
                break;
                
            case 5:
                cout << "\nEncerrando programa...\n"; 
                break; 
            
            default:
                cout << "\nOpcao invalida!\n"; 
        }
    
    } while (opcao != 5);
};


// ================= FUNÇÕES TEMPORÁRIAS =================
/*
(D)OBS: As funções abaixo foram criadas temporariamente para permitir a compilação
e os testes iniciais do menu principal.

Durante o desenvolvimento do projeto, essas implementações simples serão
substituídas pelas versões completas, responsáveis por executar as regras,
ações e mecânicas do jogo "Cidade Dorme".

A utilização de funções temporárias facilita a construção do programa por
etapas, permitindo testar cada parte do sistema antes da implementação final.
*/


void cadastrarJogadores() {
    cout << "\n[Cadastro de jogadores ainda nao implementado]\n";
}

void iniciarPartida() {
    cout << "\n[Partida ainda nao implementada]\n";
}

void mostrarRegras() {
    cout << "\n[Regras ainda nao implementadas]\n";
}

void mostrarHistorico() {
    cout << "\n[Historico ainda nao implementado]\n";
}
