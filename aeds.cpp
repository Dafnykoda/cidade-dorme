#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
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

struct Jogador { // Armazena todas as informações de cada jogador.
    string nome;
    Personagem funcao;
    bool vivo;
    int votosRecebidos;
    bool protegido;
    int suspeita;
};

struct HistoricoRodada { // Serve para registrar tudo o que aconteceu em cada rodada.
    int rodada;
    string noite;
    string dia;
    string votacao;
};

// ================= VARIÁVEIS GLOBAIS =================

const int MAX_JOGADORES = 10;

Jogador jogadores[MAX_JOGADORES];
HistoricoRodada historico[30];

int quantidadeJogadores = 0;
int quantidadeRodadas = 0;

// ================= PROTÓTIPOS =================

void menuPrincipal();        // (D): Feito!
void cadastrarJogadores();   // (D): Feito!
void iniciarPartida();       // Marcelo/Tiago
void distribuirFuncoes();    // Marcelo: Feito!
void mostrarFuncaoJogadores(); // Marcelo: Feito!
string nomeFuncao(Personagem funcao); // Marcelo: Feito!
int lerInteiro();            // Marcelo: trata entrada inválida (letras etc)
void limparTela();
void executarNoite();        // Tiago
void executarDia();          // Tiago
void realizarVotacao();      // Tiago
bool verificarVitoria();     // (D): Feito!
void mostrarHistorico();
void mostrarRegras();        // (A): Feito!
void relatorioFinal();
void limparPartida();        // (A): Feito!
void limparhistorico();      //Hugo

// ================= MAIN =================

int main() {
    srand(time(NULL));
    menuPrincipal();
    return 0;
}

// ================= MENU =================
/*
OBS: A implementação da função menuPrincipal() está após a main() porque seu
protótipo foi declarado anteriormente. Isso melhora a organização do código
e mantém a função principal mais limpa e fácil de entender.
*/

void menuPrincipal() {
    int opcao;

    do {
        cout << "\n====== CIDADE DORME ======\n";
        cout << "1 - Cadastrar jogadores\n";
        cout << "2 - Iniciar partida\n";
        cout << "3 - Regras\n";
        cout << "4 - Historico da ultima partida\n";
        cout << "5 - Sair\n";
        cout << "\nEscolha uma opcao: ";
        opcao = lerInteiro();

        switch (opcao) {
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
                } else {
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
}

// ================= CADASTRO =================

void cadastrarJogadores() {
    /*
    OBS (D): Esta função realiza o cadastro dos participantes da partida.
    O sistema solicita a quantidade de jogadores, valida se o valor está
    dentro do limite permitido (entre 6 e 10 jogadores) e registra os nomes.
    Além disso, inicializa os atributos de cada jogador para garantir que
    todos iniciem a partida com os mesmos estados padrão.
    */

    cout << "\n====== CADASTRO DE JOGADORES ======\n";

    do {
        cout << "Quantidade de jogadores (6 a 10): ";
        quantidadeJogadores = lerInteiro();
        if (quantidadeJogadores < 6 || quantidadeJogadores > 10)
            cout << "\nQuantidade invalida! Digite entre 6 a 10 jogadores.\n";
    } while (quantidadeJogadores < 6 || quantidadeJogadores > 10);

    cin.ignore(1000, '\n');

    for (int i = 0; i < quantidadeJogadores; i++) {
        cout << "\nNome do jogador " << i + 1 << ": ";
        getline(cin, jogadores[i].nome);
        jogadores[i].vivo = true;
        jogadores[i].votosRecebidos = 0;
        jogadores[i].protegido = false;
        jogadores[i].suspeita = 0;
    }

    cout << "\nJogadores cadastrados com sucesso!\n";
    cout << "\n===== JOGADORES CADASTRADOS =====\n";
    for (int i = 0; i < quantidadeJogadores; i++)
        cout << i + 1 << " - " << jogadores[i].nome << endl;
}

// ================= UTILITÁRIOS =================

string nomeFuncao(Personagem funcao) { // Marcelo: converte enum em texto
    switch (funcao) {
        case ASSASSINO: return "Assassino";
        case MEDICO:    return "Medico";
        case VIDENTE:   return "Vidente";
        case CIDADAO:   return "Cidadao";
        default:        return "Desconhecido";
    }
}

void limparTela() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Lê um inteiro com segurança — rejeita letras e entradas inválidas
int lerInteiro() {
    int valor;
    while (!(cin >> valor)) {
        cin.clear();            // limpa o estado de erro do cin
        cin.ignore(1000, '\n'); // descarta o lixo do buffer
        cout << "Entrada invalida. Digite um numero: ";
    }
    return valor;
}

// ================= DISTRIBUIÇÃO =================

void distribuirFuncoes() { // Marcelo: Cria vetor temporario c/ todas funções -> Embaralha -> Copia para jogadores
    Personagem funcoes[MAX_JOGADORES];
    int pos = 0;

    // Funcoes obrigatorias
    funcoes[pos++] = ASSASSINO;
    funcoes[pos++] = ASSASSINO;
    funcoes[pos++] = MEDICO;
    funcoes[pos++] = MEDICO;
    funcoes[pos++] = VIDENTE;

    // Completa com cidadaos
    while (pos < quantidadeJogadores)
        funcoes[pos++] = CIDADAO;

    // Embaralhamento (Fisher-Yates — sem viés estatístico)
    for (int i = quantidadeJogadores - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Personagem temp = funcoes[i];
        funcoes[i] = funcoes[j];
        funcoes[j] = temp;
    }

    // Distribui para os jogadores
    for (int i = 0; i < quantidadeJogadores; i++)
        jogadores[i].funcao = funcoes[i];
}

void mostrarFuncaoJogadores() { // Marcelo: Cada jogador vê sua própria função.
    cin.ignore(1000, '\n'); //Hugo: caso não exista nada no buffer

    for (int i = 0; i < quantidadeJogadores; i++) {
        limparTela();
        cout << "\n=================================\n";
        cout << "Jogador: " << jogadores[i].nome << "\n";
        cout << "Pressione ENTER para ver sua funcao...";
        cin.get();

        cout << "\nSua funcao e: " << nomeFuncao(jogadores[i].funcao) << "\n";

        if (jogadores[i].funcao == ASSASSINO)
            cout << "[Dica] Voce e um Assassino. Na noite, escolha uma vitima.\n";
        else if (jogadores[i].funcao == MEDICO)
            cout << "[Dica] Voce e o Medico. Na noite, escolha alguem para proteger.\n";
        else if (jogadores[i].funcao == VIDENTE)
            cout << "[Dica] Voce e o Vidente. Na noite, investigue a funcao de alguem.\n";
        else
            cout << "[Dica] Voce e um Cidadao. Use bem seu voto durante o dia!\n";

        cout << "\nPressione ENTER para esconder e passar para o proximo...";
        cin.get();
    }

    limparTela();
}

// ================= NOITE =================

void executarNoite() {

    // Zera estados da rodada
    for (int i = 0; i < quantidadeJogadores; i++) {
        jogadores[i].protegido = false;
        jogadores[i].votosRecebidos = 0;
    }

    historico[quantidadeRodadas].rodada = quantidadeRodadas;
    historico[quantidadeRodadas].noite.clear();

    cout << "\n====== NOITE " << quantidadeRodadas << " ======\n";
    cout << "A cidade adormece...\n";
    cout << "Pressione ENTER...";
    cin.ignore(1000, '\n');
    cin.get();

    int alvosAssassinos[2] = {-1, -1};
    int contAssassinos = 0;

    // ORDEM DE CADASTRO DOS JOGADORES
    for (int i = 0; i < quantidadeJogadores; i++) {

        if (!jogadores[i].vivo)
            continue;

        limparTela();

        cout << "\n=================================\n";
        cout << "VEZ DE " << jogadores[i].nome << "\n";
        cout << "=================================\n";
        cout << "Pressione ENTER para agir...";
        cin.get();

        switch (jogadores[i].funcao) {

            case MEDICO:
            {
                cout << "\n[ MEDICO ]\n";
                cout << "\nJogadores vivos:\n";

                for (int j = 0; j < quantidadeJogadores; j++) {
                    if (jogadores[j].vivo)
                        cout << j + 1 << " - " << jogadores[j].nome << "\n";
                }

                int escolha;

                do {
                    cout << "\nQuem deseja proteger? ";
                    escolha = lerInteiro() - 1;

                    if (escolha < 0 ||
                        escolha >= quantidadeJogadores ||
                        !jogadores[escolha].vivo)
                    {
                        cout << "Escolha invalida.\n";
                    } else {
                        break;
                    }

                } while (true);

                jogadores[escolha].protegido = true;

                historico[quantidadeRodadas].noite +=
                    "Medico protegeu " +
                    jogadores[escolha].nome + ".\n";

                break;
            }

            case ASSASSINO:
            {
                cout << "\n[ ASSASSINO ]\n";
                cout << "\nJogadores vivos:\n";

                for (int j = 0; j < quantidadeJogadores; j++) {
                    if (jogadores[j].vivo && j != i)
                        cout << j + 1 << " - " << jogadores[j].nome << "\n";
                }

                int escolha;

                do {
                    cout << "\nQuem deseja atacar? ";
                    escolha = lerInteiro() - 1;

                    if (escolha < 0 ||
                        escolha >= quantidadeJogadores ||
                        !jogadores[escolha].vivo ||
                        escolha == i)
                    {
                        cout << "Escolha invalida.\n";
                    } else {
                        break;
                    }

                } while (true);

                if (contAssassinos < 2)
                    alvosAssassinos[contAssassinos++] = escolha;

                break;
            }

            case VIDENTE:
            {
                cout << "\n[ VIDENTE ]\n";
                cout << "\nJogadores vivos:\n";

                for (int j = 0; j < quantidadeJogadores; j++) {
                    if (jogadores[j].vivo && j != i)
                        cout << j + 1 << " - " << jogadores[j].nome << "\n";
                }

                int escolha;

                do {
                    cout << "\nQuem deseja investigar? ";
                    escolha = lerInteiro() - 1;

                    if (escolha < 0 ||
                        escolha >= quantidadeJogadores ||
                        !jogadores[escolha].vivo ||
                        escolha == i)
                    {
                        cout << "Escolha invalida.\n";
                    } else {
                        break;
                    }

                } while (true);

                cout << "\n>>> "
                     << jogadores[escolha].nome
                     << " e "
                     << nomeFuncao(jogadores[escolha].funcao)
                     << " <<<\n";

                historico[quantidadeRodadas].noite +=
                    "Vidente investigou " +
                    jogadores[escolha].nome + ".\n";

                cout << "\nPressione ENTER...";
                cin.ignore(1000, '\n');
                cin.get();

                break;
            }

            case CIDADAO:
            {
                cout << "\n[ CIDADAO ]\n";
                cout << "Voce nao possui acao noturna.\n";
                cout << "Pressione ENTER...";
                cin.get();

                break;
            }
        }

        limparTela();
    }

    // RESOLUCAO DOS ATAQUES

    int eliminado = -1;

    if (contAssassinos == 1) {

        if (!jogadores[alvosAssassinos[0]].protegido)
            eliminado = alvosAssassinos[0];

    }
    else if (contAssassinos == 2) {

        int alvo1 = alvosAssassinos[0];
        int alvo2 = alvosAssassinos[1];

        if (alvo1 == alvo2) {

            if (!jogadores[alvo1].protegido)
                eliminado = alvo1;
        }
        else {

            bool prot1 = jogadores[alvo1].protegido;
            bool prot2 = jogadores[alvo2].protegido;

            if (!prot1 && !prot2) {
                eliminado = alvosAssassinos[rand() % 2];
            }
            else if (!prot1) {
                eliminado = alvo1;
            }
            else if (!prot2) {
                eliminado = alvo2;
            }
        }
    }

    if (eliminado != -1) {

        jogadores[eliminado].vivo = false;

        historico[quantidadeRodadas].noite +=
            "Assassinos eliminaram " +
            jogadores[eliminado].nome + ".\n";
    }
    else {

        historico[quantidadeRodadas].noite +=
            "Nenhum jogador foi eliminado nesta noite.\n";
    }
}
// ================= DIA =================

void executarDia() {
    historico[quantidadeRodadas].dia.clear();

    limparTela();
    cout << "\n====== DIA " << quantidadeRodadas << " ======\n";
    cout << "A cidade acorda...\n\n";

    // Narra resultado da noite
    cout << "[ RESULTADO DA NOITE ]\n";
    cout << historico[quantidadeRodadas].noite;

    // Mostra jogadores vivos
    int vivos = 0;
    cout << "\n[ JOGADORES VIVOS ]\n";
    for (int i = 0; i < quantidadeJogadores; i++) {
        if (jogadores[i].vivo) {
            cout << i + 1 << " - " << jogadores[i].nome << "\n";
            vivos++;
        }
    }
    cout << "\nTotal: " << vivos << " jogadores vivos.\n";

    // Mostra eliminados com função revelada
    cout << "\n[ JOGADORES ELIMINADOS ]\n";
    bool algum = false;
    for (int i = 0; i < quantidadeJogadores; i++) {
        if (!jogadores[i].vivo) {
            cout << "- " << jogadores[i].nome
                 << " (" << nomeFuncao(jogadores[i].funcao) << ")\n";
            algum = true;
        }
    }
    if (!algum) cout << "Nenhum ate agora.\n";

    historico[quantidadeRodadas].dia =
        "Dia: " + to_string(vivos) + " jogadores vivos.";

    cout << "\nPressione ENTER para iniciar a votacao...";
    cin.get();
}

// ================= VOTAÇÃO =================

void realizarVotacao() {
    /*
    Cada jogador vivo vota manualmente em outro jogador vivo.
    O mais votado é eliminado. Em empate, sorteio entre os empatados.
    */
    historico[quantidadeRodadas].votacao.clear();

    limparTela();
    cout << "\n====== VOTACAO " << quantidadeRodadas << " ======\n";

    // Reset votos
    for (int i = 0; i < quantidadeJogadores; i++)
        jogadores[i].votosRecebidos = 0;

    // Cada jogador vivo vota
    for (int i = 0; i < quantidadeJogadores; i++) {
        if (!jogadores[i].vivo) continue;

        limparTela();
        cout << "\n[ VOTACAO ] Vez de " << jogadores[i].nome << "\n";
        cout << "Pressione ENTER para votar...";
        cin.get();

        cout << "\nJogadores disponiveis para votar:\n";
        for (int j = 0; j < quantidadeJogadores; j++)
            if (jogadores[j].vivo && j != i)
                cout << j + 1 << " - " << jogadores[j].nome << "\n";

        int escolha;
        do {
            cout << "\nVote em (numero): ";
            escolha = lerInteiro() - 1;
            if (escolha < 0 || escolha >= quantidadeJogadores || !jogadores[escolha].vivo || escolha == i)
                cout << "Voto invalido. Tente novamente.\n";
            else
                break;
        } while (true);

        jogadores[escolha].votosRecebidos++;

        cout << "\nVoto registrado! Pressione ENTER para ocultar...";
        cin.get();
        limparTela();
    }

    // Apura resultado
    cout << "\n[ RESULTADO DA VOTACAO ]\n";

    int maxVotos = 0;
    for (int i = 0; i < quantidadeJogadores; i++)
        if (jogadores[i].vivo && jogadores[i].votosRecebidos > maxVotos)
            maxVotos = jogadores[i].votosRecebidos;

    // Coleta empatados
    int empatados[MAX_JOGADORES];
    int contEmpatados = 0;
    for (int i = 0; i < quantidadeJogadores; i++)
        if (jogadores[i].vivo && jogadores[i].votosRecebidos == maxVotos)
            empatados[contEmpatados++] = i;

    // Placar
    for (int i = 0; i < quantidadeJogadores; i++)
        if (jogadores[i].vivo)
            cout << jogadores[i].nome << ": " << jogadores[i].votosRecebidos << " voto(s)\n";

    int eliminado;
    string registroVotacao = "";

    if (contEmpatados > 1) {
        cout << "\nEmpate! Sorteando entre os mais votados...\n";
        eliminado = empatados[rand() % contEmpatados];
        registroVotacao = "Empate. ";
    } else {
        eliminado = empatados[0];
    }

    jogadores[eliminado].vivo = false;
    registroVotacao += jogadores[eliminado].nome +
        " eliminado na votacao (" + nomeFuncao(jogadores[eliminado].funcao) + ").";

    cout << "\n>>> " << jogadores[eliminado].nome
         << " foi eliminado! Funcao revelada: "
         << nomeFuncao(jogadores[eliminado].funcao) << " <<<\n";

    historico[quantidadeRodadas].votacao = registroVotacao;

    cout << "\nPressione ENTER para continuar...";
    cin.get();
}

// ================= VERIFICAR VITÓRIA =================
 
bool verificarVitoria() {
    /*
    Vitória dos cidadãos: todos os assassinos eliminados.
    Vitória dos assassinos: assassinos >= demais jogadores vivos.
    Retorna true se a partida acabou.
    */
    int assassinosVivos = 0;
    int outrosVivos = 0;
 
    for (int i = 0; i < quantidadeJogadores; i++) {
        if (jogadores[i].vivo) {
            if (jogadores[i].funcao == ASSASSINO)
                assassinosVivos++;
            else
                outrosVivos++;
        }
    }
 
    if (assassinosVivos == 0) {
        cout << "\n=================================\n";
        cout << "VITORIA DOS CIDADAOS!\n";
        cout << "Todos os assassinos foram eliminados.\n";
        cout << "=================================\n";
        return true;
    }
 
    if (assassinosVivos >= outrosVivos) {
        cout << "\n=================================\n";
        cout << "VITORIA DOS ASSASSINOS!\n";
        cout << "Os assassinos dominaram a cidade.\n";
        cout << "=================================\n";
        return true;
    }
 
    return false;
}
 
// ================= INICIAR PARTIDA =================
 
void iniciarPartida() {
    cout << "\n===== INICIANDO PARTIDA =====\n";
 
    limparPartida();
    distribuirFuncoes();
    mostrarFuncaoJogadores();
 
    cout << "Todas as funcoes foram distribuidas!\n";
    cout << "Pressione ENTER para comecar a partida...";
    cin.get();
 
    while (true) {
        quantidadeRodadas++;
 
        if (quantidadeRodadas > 30) {
            cout << "\n[Limite de 30 rodadas atingido]\n";
            break;
        }
 
        executarNoite();
        if (verificarVitoria()) break;
 
        executarDia();
        realizarVotacao();
        if (verificarVitoria()) break;
 
        cout << "\n>>> Fim da Rodada " << quantidadeRodadas << " <<<\n";
        cout << "Pressione ENTER para continuar...";
        cin.get();
    }
 
    relatorioFinal();
}
 
// ================= RELATÓRIO FINAL =================
 
void relatorioFinal() {
    limparTela();
    cout << "\n======== RELATORIO FINAL ========\n";
    cout << "Total de rodadas: " << quantidadeRodadas << "\n\n";
 
    cout << "[ JOGADORES ]\n";
    for (int i = 0; i < quantidadeJogadores; i++) {
        cout << jogadores[i].nome
             << " | " << nomeFuncao(jogadores[i].funcao)
             << " | " << (jogadores[i].vivo ? "Vivo" : "Eliminado")
             << "\n";
    }
 
    cout << "\nPressione ENTER para voltar ao menu...";
    cin.get();
 
    limparPartida();
    cout << "\nVoltando ao menu principal...\n";
}
 
// ================= HISTÓRICO =================
 
void mostrarHistorico() {
    if (quantidadeRodadas == 0) {
        cout << "\nNenhuma partida foi jogada ainda.\n";
        return;
    }
 
    cout << "\n======== HISTORICO DA PARTIDA ========\n";
    for (int i = 1; i <= quantidadeRodadas; i++) {
        cout << "\n--- Rodada " << historico[i].rodada << " ---\n";
        cout << "Noite   : " << historico[i].noite;
        cout << "Dia     : " << historico[i].dia << "\n";
        cout << "Votacao : " << historico[i].votacao << "\n";
    }
}
 
// ================= REGRAS =================
 
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
    cout << "    ate que fiquem em numero igual ou menor que eles.\n\n";
 
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

        cout << "--------------------------------------------------\n";

    // Hugo: Sistema de suspeita

    cout << "  SISTEMA DE SUSPEITA:\n";

    cout << "  Cada jogador possuira um indice de suspeita/desempenho.\n";
    cout << "  Esse valor sera alterado confome:\n";
    cout << "  votos corretos;\n";
    cout << "  votos incorretos;\n";
    cout << "  apoio a assassinos;\n";
    cout << "  eliminacao de cidadaos;\n";
    cout << "  eliminacao de assassinos.\n";
    cout << "  Essa pontuacao sera mostrada no final da partida.\n\n";
 
    cout << "==================================================\n";
 
    cout << "Pressione ENTER para voltar ao menu principal.";
    cin.ignore(1000, '\n');
    cin.get();
}
 
// ================= LIMPAR PARTIDA =================
 
void limparPartida() {
    // (A): Limpa os dados da partida anterior.
    quantidadeRodadas = 0;
 
    for (int i = 0; i < quantidadeJogadores; i++) {
        jogadores[i].vivo = true;
        jogadores[i].votosRecebidos = 0;
        jogadores[i].protegido = false;
        jogadores[i].suspeita = 0;
    }
}

//Hugo: Limpar histórico

void limparhistorico(){
    // Hugo: Limpar histórico

        for(int i = 0; i < 30; i++){
    historico[i].rodada = 0;
    historico[i].noite = "";
    historico[i].dia = "";
    historico[i].votacao = "";
        }
}