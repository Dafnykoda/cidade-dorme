#include <iostream>
#include <string>
#include <cstdlib> //Permite o sistema de sorteios. rand () % 10 escolhe aleatoriamente uma posição do vetor de jogadores
                   //Srand faz com que o rand mude em cada partida, sem o srand todas as partidas seriam iguais
#include <ctime> // Relaciona o horário atual do pc com o tempo do jogo
using namespace std;

enum Funcao {
    ASSASSINO,
    MEDICO,
    VIDENTE,
    CIDADAO
};

struct Jogador{
    string nome;
    Funcao funcao;
    bool vivo;
    int votosRecebidos;
    bool protegido;
    int suspeita;
};

struct HistoricoRodada{
    int rodada;
    string noite;
    string dia;
    string votacao;
};

//Possíveis funções que vamos utilizar e desenvolver

void menuPrincipal();

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

int main() {

    srand(time(NULL)); // Permite a mudança de funções entre as partidas

    HistoricoRodada historico[30];
int quantidadeJogadores;
int quantidadeRodadas;

void menuPrincipal(){
    
    int opcao;
    do{

    cout << "\n ===== CIDADE DORME =====\n";
    cout << " 1 - Cadastrar jogadores \n";
    cout << " 2 - Iniciar partida \n";
    cout << " 3 - Regras \n";
    cout << " 4 - Histórico da última partida\n";
    cout << " 5 - Sair \n";

    cout << "\n Opção: ";
    cin >> opcao;

    switch(opcao){ //switch serve para executar diferentes cenários que dependem do valor informado da variável
        case 1: //caso definido pelo valor da variável 
        cadastrarJogadores(); //encaixar...
        break; //break encerra o bloco, impedindo o algoritmo de passar automaticamente para os outros casos

        case 2:
        iniciarPartida(); //encaixar...
        break; 

        case 3:
        mostrarRegras(); //encaixar...
        break;

        case 4:
        mostrarHistorico(); //encaixar...
        break;

        case 5:
        cout<<"\nEncerrando...\n";
        break;

        defalt: //executa se nenhum caso for verdadeiro
        cout<<"\nOpção inválida!\n";
    }

}while (opcao != 5);
}
return 0;
}
