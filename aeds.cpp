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

// ================= VARIÁVEIS GLOBAIS =================

/*
OBS: Variáveis globais utilizadas para armazenar informações que serão
compartilhadas entre várias funções do programa, como jogadores,
histórico de rodadas e controle da partida.
*/

const int MAX_JOGADORES = 10;

Jogador jogadores[MAX_JOGADORES];

HistoricoRodada historico[30];

int quantidadeJogadores = 0;
int quantidadeRodadas = 0;

//Possíveis funções que vamos utilizar e desenvolver (PROTÓTIPOS)

 void menuPrincipal(); // (D): Feito! 

void cadastrarJogadores(); // (D): Feito!

void iniciarPartida(); //Marcelo/Tiago: Temporario

void distribuirFuncoes();  // Marcelo: Feito!

void mostrarFuncaoJogadores();  // Marcelo: Feito!

void executarNoite(); // Tiago: Temporario

void executarDia(); // Tiago: Temporario

void realizarVotacao(); // Tiago: Temporario

bool verificarVitoria(); // (D): Feito!

void mostrarHistorico();

void mostrarRegras(); // (A): Feito!

void relatorioFinal();

void limparPartida(); // (A): Feito!

// ================= MAIN =================

int main() {

    srand(time(NULL)); // Permite a mudança de funções entre as partidas

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
                /*
                OBS (D): Validação para impedir que uma partida seja iniciada sem que os
                jogadores tenham sido cadastrados previamente.
                */
                if (quantidadeJogadores == 0) {
                    cout << "\nCadastre os jogadores antes de iniciar a partida!\n";
                }
                else {
                    iniciarPartida();
                }
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

void cadastrarJogadores() {
/*
OBS (D): Esta função realiza o cadastro dos participantes da partida.

O sistema solicita a quantidade de jogadores, valida se o valor está
dentro do limite permitido (entre 6 e 10 jogadores) e registra os nomes
informados.

Além disso, inicializa os atributos de cada jogador para garantir que
todos iniciem a partida com os mesmos estados padrão.
*/

    cout << "\n====== CADASTRO DE JOGADORES ======\n";

    do
    {
        cout<<"Quantidade de jogadores (6 a 10): ";
        cin>> quantidadeJogadores; 

        if (quantidadeJogadores<6||quantidadeJogadores>10) {
            cout<<"\nQuantidade invalida! Digite entre 6 a 10 jogadores.\n";
        }
        
    } while (quantidadeJogadores<6||quantidadeJogadores>10);

    cin.ignore(); 

    for (int i=0; i<quantidadeJogadores; i++) {
        
        cout<<"\nNome do jogador "<<i+1<<": "; 
        getline(cin, jogadores[i].nome); 

        jogadores[i].vivo=true;
        jogadores[i].votosRecebidos=0;
        jogadores[i].protegido=false; 
        jogadores[i].suspeita=0; 

    }
    
    cout<<"\nJogadores cadastrados com sucesso!\n";

    cout << "\n===== JOGADORES CADASTRADOS =====\n";

    for(int i = 0; i < quantidadeJogadores; i++) {
    
        cout << i + 1 << " - " << jogadores[i].nome << endl;
    }
    
}

    string nomeFuncao(Personagem funcao) { //Marcelo: Funcao string criada para converter enum em texto

        switch(funcao) {

            case ASSASSINO:
                return "Assassino";

            case MEDICO:
                return "Medico";

            case VIDENTE:
                return "Vidente";

            case CIDADAO:
                return "Cidadao";

            default:
                return "Desconhecido";
        }
    }

    void distribuirFuncoes() { //Marcelo: Cria vetor temporario c/ todas funções -> Embaralha -> Copia para jogadores

        Personagem funcoes[MAX_JOGADORES];

        int pos = 0;

        // Funcoes obrigatorias
        funcoes[pos++] = ASSASSINO;
        funcoes[pos++] = ASSASSINO;

        funcoes[pos++] = MEDICO;
        funcoes[pos++] = MEDICO;

        funcoes[pos++] = VIDENTE;

        // Completa com cidadaos
        while(pos < quantidadeJogadores) {
            funcoes[pos++] = CIDADAO;
        }

        // Embaralhamento
        for(int i = 0; i < quantidadeJogadores; i++) {

            int sorteio = rand() % quantidadeJogadores;

            Personagem temp = funcoes[i];
            funcoes[i] = funcoes[sorteio];
            funcoes[sorteio] = temp;
        }

        // Distribui para os jogadores
        for(int i = 0; i < quantidadeJogadores; i++) {
            jogadores[i].funcao = funcoes[i];
        }
}

void mostrarFuncaoJogadores() { //Marcelo: Cada jogador vê sua própia função.

    cin.ignore();

    for(int i = 0; i < quantidadeJogadores; i++) {

        cout << "\n=================================\n";
        cout << "Jogador: " << jogadores[i].nome << endl;

        cout << "Pressione ENTER para ver sua funcao...";
        cin.get();

        cout << "\nSua funcao e: "
             << nomeFuncao(jogadores[i].funcao)
             << endl;

        cout << "\nPressione ENTER para esconder...";
        cin.get();

       #ifdef _WIN32 //limpa tela para o prox jogador não ver funcao do jogador anterior.
        system("cls");
    #else
        system("clear");
    #endif
    }
}

void executarNoite() {

    // Zera estados da rodada
    for (int i = 0; i < quantidadeJogadores; i++) {
        jogadores[i].protegido = false;
        jogadores[i].votosRecebidos = 0; // votos serão usados na votação
        // suspeita pode ser mantida/alterada se você quiser
    }

    // Estrutura de texto para histórico
    historico[quantidadeRodadas].rodada = quantidadeRodadas;
    historico[quantidadeRodadas].noite.clear();

    // MEDICO: protege alguém (se vivo)

    int medicoIndex = -1;
    for (int i = 0; i < quantidadeJogadores; i++) {
        if (jogadores[i].vivo && jogadores[i].funcao == MEDICO) {
            medicoIndex = i;
            break;
        }
    }

    if (medicoIndex != -1) {
        // Sem interface: sorteia automaticamente um alvo vivo (projeto pode evoluir para entrada manual)
        int alvo = -1;
        while (alvo == -1 || !jogadores[alvo].vivo || alvo == medicoIndex) {
            alvo = rand() % quantidadeJogadores;
        }
        jogadores[alvo].protegido = true;

        historico[quantidadeRodadas].noite +=
            "Noite: Medico protegeu " + jogadores[alvo].nome + ".\n";
    } else {
        historico[quantidadeRodadas].noite += "Noite: Medico ja nao existe.\n";
    }

    // ASSASSINO: escolhe alvo

    int assassinoIndex = -1; // pode haver mais de um; aqui escolhemos um "representante" para escolher alvo
    for (int i = 0; i < quantidadeJogadores; i++) {
        if (jogadores[i].vivo && jogadores[i].funcao == ASSASSINO) {
            assassinoIndex = i;
            break;
        }
    }

    if (assassinoIndex != -1) {
        int alvo = -1;
        // alvo não pode ser assassino em si (e preferimos que seja vivo)
        while (alvo == -1 || !jogadores[alvo].vivo || jogadores[alvo].funcao == ASSASSINO) {
            alvo = rand() % quantidadeJogadores;
        }

        // Se alvo estiver protegido, assassino falha
        if (jogadores[alvo].protegido) {
            historico[quantidadeRodadas].noite +=
                "Noite: Assassinos tentaram matar " + jogadores[alvo].nome + ", mas o Medico protegeu!\n";
        } else {
            jogadores[alvo].vivo = false;
            historico[quantidadeRodadas].noite +=
                "Noite: Assassinos mataram " + jogadores[alvo].nome + ".\n";
        }
    } else {
        historico[quantidadeRodadas].noite += "Noite: Nao ha assassinos vivos.\n";
    }


    // VIDENTE:

    int videnteIndex = -1;
    for (int i = 0; i < quantidadeJogadores; i++) {
        if (jogadores[i].vivo && jogadores[i].funcao == VIDENTE) {
            videnteIndex = i;
            break;
        }
    }

    if (videnteIndex != -1) {
        int alvo = -1;
        while (alvo == -1 || !jogadores[alvo].vivo || alvo == videnteIndex) {
            alvo = rand() % quantidadeJogadores;
        }
        bool éAssassino = (jogadores[alvo].funcao == ASSASSINO);

        historico[quantidadeRodadas].noite +=
            "Noite: Vidente olhou " + jogadores[alvo].nome + " e viu que era " +
            (éAssassino ? "ASSASSINO.\n" : "CIDADAO.\n");
    }

    cout << "\n====== NOITE " << quantidadeRodadas << " ======\n";
    cout << historico[quantidadeRodadas].noite;
}

void executarDia() {
    historico[quantidadeRodadas].dia.clear();

    cout << "\n====== DIA " << quantidadeRodadas << " ======\n";

    // A morte da noite já aconteceu; agora coletamos mortos nesta rodada
    // (para simplificar, varremos os que estão vivos vs antes; como não temos antes, apenas descrevemos o estado atual)
    int vivos = 0;
    for (int i = 0; i < quantidadeJogadores; i++) {
        if (jogadores[i].vivo) vivos++;
    }

    historico[quantidadeRodadas].dia +=
        "Dia: Existem " + to_string(vivos) + " jogadores vivos no momento.\n";

    cout << historico[quantidadeRodadas].dia;

    cout << "Pressione ENTER para votar.\n";
    cin.ignore();
    cin.get();
}

void realizarVotacao() {
    historico[quantidadeRodadas].votacao.clear();

    cout << "\n====== VOTACAO " << quantidadeRodadas << " ======\n";

    // Reset votos
    for (int i = 0; i < quantidadeJogadores; i++) {
        jogadores[i].votosRecebidos = 0;
    }

    // Cada jogador vivo vota automaticamente em outro vivo aleatório
    for (int votante = 0; votante < quantidadeJogadores; votante++) {
        if (!jogadores[votante].vivo) continue;

        int alvo = -1;
        while (alvo == -1 || !jogadores[alvo].vivo || alvo == votante) {
            alvo = rand() % quantidadeJogadores;
        }
        jogadores[alvo].votosRecebidos++;
    }

    // Encontrar maior votação
    int maior = -1;
    for (int i = 0; i < quantidadeJogadores; i++) {
        if (jogadores[i].vivo && jogadores[i].votosRecebidos > maior) {
            maior = jogadores[i].votosRecebidos;
        }
    }

    // Possíveis empatados
    int empateIndices[MAX_JOGADORES];
    int qtdEmpate = 0;
    for (int i = 0; i < quantidadeJogadores; i++) {
        if (jogadores[i].vivo && jogadores[i].votosRecebidos == maior) {
            empateIndices[qtdEmpate++] = i;
        }
    }

    // Desempate aleatório
    int escolhido = empateIndices[rand() % qtdEmpate];
    jogadores[escolhido].vivo = false;

    // Montar histórico
    historico[quantidadeRodadas].votacao += "Resultado da votacao:\n";
    for (int i = 0; i < quantidadeJogadores; i++) {
        if (jogadores[i].votosRecebidos > 0) {
            historico[quantidadeRodadas].votacao +=
                " - " + jogadores[i].nome + ": " + to_string(jogadores[i].votosRecebidos) + " voto(s)\n";
        }
    }
    historico[quantidadeRodadas].votacao +=
        "Eliminado no dia: " + jogadores[escolhido].nome + ".\n";

    cout << historico[quantidadeRodadas].votacao;
}

bool verificarVitoria() {

    /*
    Esta função verifica se alguma das condições de vitória do jogo foi atingida.

    Vitória dos cidadãos:
    - Ocorre quando todos os assassinos são eliminados.

    Vitória dos assassinos:
    - Ocorre quando a quantidade de assassinos vivos é igual ou superior à quantidade dos demais jogadores vivos.

    A função retorna:
    true  -> partida encerrada.
    false -> partida continua.
    */

    int assassinosVivos = 0;
    int outrosVivos = 0;

    for(int i = 0; i < quantidadeJogadores; i++) {

        if(jogadores[i].vivo) {

            if(jogadores[i].funcao == ASSASSINO) {
                assassinosVivos++;
            }
            else {
                outrosVivos++;
            }
        }
    }

    // Vitória dos cidadãos
    if(assassinosVivos == 0) {

        cout << "\n=================================\n";
        cout << "VITORIA DOS CIDADAOS!\n";
        cout << "Todos os assassinos foram eliminados.\n";
        cout << "=================================\n";

        return true;
    }

    // Vitória dos assassinos
    if(assassinosVivos >= outrosVivos) {

        cout << "\n=================================\n";
        cout << "VITORIA DOS ASSASSINOS!\n";
        cout << "Os assassinos dominaram a cidade.\n";
        cout << "=================================\n";

        return true;
    }

    return false;
}

void mostrarRegras() { 
    
    #ifdef _WIN32 
        system("cls");
    #else
        system("clear");
    #endif

    // (A): Regras e Explicacao.
    
    cout << "======== REGRAS DO JOGO: CIDADE DORME ======== \n\n";
    
    cout << "  OBJETIVO:\n";
    cout << "  - Cidadaos, Medicos e Videntes: Devem descobrir\n";
    cout << "    quem sao os assassinos e elimina-los na votacao.\n";
    cout << "  - Assassinos: Devem eliminar os outros jogadores\n";
    cout << "    ate que fiquem em numero igual ou maior que eles.\n\n";

    cout << "--------------------------------------------------\n";
    
    cout << "  PERSONAGENS:\n";
    cout << "  * ASSASSINO: Acorda na noite e escolhe alguem para matar.\n";
    cout << "  * MEDICO: Acorda na noite e escolhe alguem para salvar.\n";
    cout << "  * VIDENTE: Acorda na noite e descobre a funcao de alguem.\n";
    cout << "  * CIDADAO: Nao acorda a noite, apenas vota durante o dia.\n\n";

    cout << "--------------------------------------------------\n";

    cout << "  A RODADA:\n";
    cout << "  1o - NOITE: Todos fecham os olhos. O sistema chama as\n";
    cout << "       funcoes especiais uma por uma para agirem em segredo.\n";
    cout << "  2o - DIA: A cidade acorda e descobre se houve alguma morte.\n";
    cout << "  3o - VOTACAO: Os jogadores discutem e votam para eliminar\n";
    cout << "       um suspeito. O mais votado sai do jogo.\n\n";

    cout << "==================================================\n";
    
    cout << "Pressione ENTER para voltar ao menu principal.";
    cin.ignore(); 
    cin.get();    
}
void limparPartida() {
    
    // (A): Limpa os dados da partida anterior.
    
    quantidadeRodadas = 0;

    for(int i = 0; i < quantidadeJogadores; i++) {
        jogadores[i].vivo = true;
        jogadores[i].votosRecebidos = 0;
        jogadores[i].protegido = false;
        jogadores[i].suspeita = 0;
    }
}

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

void iniciarPartida() {

    cout << "\n===== INICIANDO PARTIDA =====\n";

    quantidadeRodadas = 0;

    // Reset de estados de jogadores caso inicie nova partida sem cadastrar de novo
    for (int i = 0; i < quantidadeJogadores; i++) {
        jogadores[i].vivo = true;
        jogadores[i].votosRecebidos = 0;
        jogadores[i].protegido = false;
        jogadores[i].suspeita = 0;
    }

    distribuirFuncoes();

    mostrarFuncaoJogadores();

    cout << "\nTodas as funcoes foram distribuidas!\n";
    cout << "A partida esta pronta para comecar.\n";

   // Loop principal do jogo
    while (true) {
        quantidadeRodadas++;
        if (quantidadeRodadas > 30) {
            // Segurança: evita estourar o histórico
            cout << "\n[Limite de rodadas atingido]\n";
            break;
        }

        executarNoite();
        if (verificarVitoria()) break;

        executarDia();
        realizarVotacao();
        if (verificarVitoria()) break;

        // Exibir um pequeno resumo da rodada
        cout << "\n\n>>> Fim da Rodada " << quantidadeRodadas << " <<<\n";
        cout << "Pressione ENTER para continuar...";
        cin.ignore();
        cin.get();
    }

    // Relatório final

    // Limpa histórico e permite nova partida
    limparPartida();
    cout << "\nVoltando ao menu principal...\n";
}

void mostrarHistorico() {
    cout << "\n[Historico ainda nao implementado]\n";
}