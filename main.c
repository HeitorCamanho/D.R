#include "raylib.h"
#include <stdio.h>
#include <math.h>

/* DICIONÁRIO DAS VARIÁVEIS
 * ----------------------
 *                  |Tela|
 * telaLargura (const int) -> Responsável por definir a largura da tela;
 * telaAltura (const int) -> Responsável por definir o comprimento da tela;
 *                  |Tela|
 *
 *                  |Colisão|
 * col_mouse_caixa (bool) -> Verifica se há colisão entre mouse e caixa de papéis;
 *                  |Colisão|
 *
 *                  |Controladoras|
 * qnt_cont_frames (int) -> Define durante quantos segundos algo deve durar;
 * fase_delay (int) -> Tempo inicial de delay antes da fase começar;
 * fase_delay_val (char[4][2]) -> Guarda os valores que serão exibidos na contagem do delay;
 * fase_delay_fim (bool) -> Controla se o fim do delay já chegou ou não;
 * fase_nivel (int) -> Controla qual o nível atual;
 * fase_vida (int) -> Controla a quantidade de vidas do jogador
 * upg_escolha (int) -> Guarda o id do upgrade para a execução das suas instruções;
 * telaAtual (telaJogo) -> Define a fase atual;
 * menu_novoJogo (bool) -> Define se o fundo atual do Menu será o do "Novo Jogo"
 * menu_sair (bool) -> Define se o fundo atual do Menu será o do "Sair"
 *                  |Controladoras|
 *
 *                  |JOGO|
 * func_tempo (int) -> Função que calcula a quantidade de tempo por fase;
 * func_qnt_tempo (int) -> Armazena quantidade de tempo;
 * func_papel (int) -> Função que calcula a quantidade de papéis por fase;
 * func_qnt_papel (int) -> Armazena a quantidade de papéis;
 * potencia (double) -> Armazena o valor da potência para ser aplicada na função;
 * qnt_papel (int) -> Define a quantidade de papéis;
 * qnt_tempo (double) -> Define a quantidade de tempo;
 * telaJogo (enum {MENU, UPGRADE, DELAY, FASE}) -> Define quais serão as telas;
 * val_desc_papel (int) -> Define a quantidade de papel descontado a cada clique do jogador;
 * qnt_bonus_papel(int) -> Define a quantidade total de papel a ser descontado como bônus do upgrade;
 * qnt_bonus_tempo(double) -> Define a quantidade total de tempo a ser acrescentado como bônus do upgrade;
 * Upgrade (Struct {int id, Rectangle tamanho, Color cor}) -> Armazena os atributos dos upgrades;
 * upgrade (Vetor (struct Upgrade)) -> Armazena os upgrades em um vetor;
 * Fase (Struct {int papel, double tempo}) -> Armazena os atributos das fases;
 * fase (Vetor (struct Fase)) -> Armazena as fases em um vetor;
 *                  |JOGO|
 *
 *                  |Objeto|
 * caixaPapeis (Rectangle) -> Define as dimensões da caixa de papéis;
 * text_papel (Texture2D) -> Carrega o sprite dos papéis;
 * text_menu (Texture2D) -> Carrega o sprite do fundo do Menu;
 * text_mouse (Texture2D) -> Carrega o sprite do ícone do mouse;
 * botaoJogar (Rectangle) -> Define as dimensões do botão "NOVO JOGO";
 * botaoSair (Rectangle) -> Define as dimensões do botão "SAIR;
 *                  |Objeto|
 * */

// Criação Variáveis e Funções
//--------------------------------------------------------------------------------------
#pragma region Variáveis Criação                                                                                                       //Criação das Variáveis
const int telaLargura = 1280;                                                                                           //Define a largura da tela
const int telaAltura = 720;                                                                                             //Define o comprimento da tela

int qnt_papel;                                                                                                          //Quantidade de papel necessário para vencer
int qnt_cont_frames = 0;                                                                                                //Definindo uma variável contadora (vantagem em relação ao Wait é que com ela podemos encerrar a aplicação a qualquer momento)
int fase_delay;                                                                                                         //Tempo de delay inicial da fase
int fase_nivel;                                                                                                         //Definindo o nível
int fase_vida;                                                                                                          //Definindo a quantidade de vidas
int upg_escolha;                                                                                                        //Guarda o id do upgrade escolhido
int val_desc_papel = 1;                                                                                                 //Quantidade de papel descontado a cada clique
int qnt_bonus_papel = 0;                                                                                                //Quantidade a ser diminuida de papel

double qnt_tempo;                                                                                                       //Definindo o tempo da fase
double qnt_bonus_tempo = 0;                                                                                             //Quantidade a ser somanda de tempo

bool col_mouse_caixa = false;                                                                                           //Criando a variável para a colisão
bool fase_delay_fim = false;                                                                                            //Variável que controla o fim do delay inicial da fase
bool menu_novoJogo = false;                                                                                             //Definindo o fundo do Menu
bool menu_sair = false;                                                                                                 //Definindo o fundo do Menu


char fase_delay_val [4][2] = {"", "1","2","3"};                                                         //Lista que expõe a contagem do delay inicial

typedef struct {                                                                                                        //Struct dos Upgrades
    int id;
    Rectangle tamanho;
    Color cor;
} Upgrade;

typedef struct {                                                                                                        //Struct das Fases
    int papel;
    double tempo;
} Fase;

enum telaJogo {MENU, UPGRADE, DELAY, FASE};                                                                             //Definindo as opções de Telas

#pragma endregion Variáveis

#pragma region Funções Criação                                                                                                         //Criação das Funções
int func_tempo()                                                                                                        //Função para calcular o tempo
{
    int func_qnt_tempo = (12 * fase_nivel) + 15;
    return func_qnt_tempo;
}

int func_papel()                                                                                                        //Função para calcular a quantidade de papel
{
    double potencia = pow(4, fase_nivel);
    int func_qnt_papel =  potencia + 20;
    return func_qnt_papel;
}
#pragma endregion Funções
//--------------------------------------------------------------------------------------

int main(void)
{
    // Inicialização
    //--------------------------------------------------------------------------------------
    Upgrade upgrade[3];                                                                                                 //Vetor de structs que armazena os upgrades
#pragma region Upgrade Valores                                                                                                         //Criação dos upgrades
    upgrade[0].tamanho.x = 299;
    upgrade[0].tamanho.y = 182;
    upgrade[0].tamanho.width = (64*3)-20;
    upgrade[0].tamanho.height = 64*3;
    upgrade[0].id = 1;
    upgrade[0].cor = GOLD;

    upgrade[1].tamanho.x = 575;
    upgrade[1].tamanho.y = 182;
    upgrade[1].tamanho.width = (64*3)-20;
    upgrade[1].tamanho.height = 64*3;
    upgrade[1].id = 2;
    upgrade[1].cor = RED;

    upgrade[2].tamanho.x = 842;
    upgrade[2].tamanho.y = 182;
    upgrade[2].tamanho.width = (64*3)-20;
    upgrade[2].tamanho.height = 64*3;
    upgrade[2].id = 3;
    upgrade[2].cor = GREEN;
#pragma endregion Upgrade

    Fase fase[6];
#pragma region Fase Valores                                                                                                            //Criação das fases
    fase[0].papel = 0;
    fase[0].tempo = 0;

    fase[1].papel = 20;
    fase[1].tempo = 30;

    fase[2].papel = 80;
    fase[2].tempo = 40;

    fase[3].papel = 200;
    fase[3].tempo = 50;

    fase[4].papel = 300;
    fase[4].tempo = 60;

    fase[5].papel = 500;
    fase[5].tempo = 70;
#pragma endregion Fase

    enum telaJogo telaAtual = MENU;                                                                                     //Define a fase atual

    InitWindow(telaLargura, telaAltura, "D.R:Demon's Resources");                                      //Criando a tela de Jogo
    SetTargetFPS(60);                                                                                               //Definindo a quantidade de frames por segundo

#pragma region Load Retângulos
    Rectangle caixaPapeis = { 20, 15, 150, 180 };                                                   //Criando os valores da caixa de papéis
    Rectangle botaoJogar = {616, 338, 50, 50};                                                      //Criando os valores do botão "NOVO JOGO"
    Rectangle botaoSair = {616, 436, 50, 50};                                                       //Criando os valores do botão "SAIR"
    Rectangle molduraPapeis = {15,10,160,190};                                                      //Criando os valores da moldura do papel
    Rectangle molduraBaseQntPapeis = {60, 165, 70, 45,};                                            //Criando os valores da base da moldura da quantidade de papéis
    Rectangle molduraQntPapeis = {65, 170, 60, 35,};                                                //Criando os valores da moldura da quantidade de papéis
    Rectangle molduraBaseQntTempo = {(telaLargura / 2) - 5, (telaAltura / 2) - 350, 90, 50};        //Criando os valores da base da moldura da quantidade de tempo
    Rectangle molduraQntTempo = {(telaLargura / 2), (telaAltura / 2) - 350, 80, 50};                //Criando os valores da moldura da quantidade de tempo
    Rectangle molduraTelaFase = {0,0,1280,720};                                                     //Criando os valores da moldura da fase
#pragma endregion Retângulos

#pragma region Load Texturas
    Texture2D text_papel = LoadTexture("Assets/Sprites/papel.png");                                             //Carrega o sprite do papel
    Texture2D text_menu = LoadTexture("Assets/Sprites/menu.png");                                               //Carrega o sprite do fundo do Menu
    Texture2D text_menu_novoJogo = LoadTexture("Assets/Sprites/menu_novo_jogo.png");                            //Carrega o sprite do fundo "Novo Jogo" do Menu
    Texture2D text_menu_sair = LoadTexture("Assets/Sprites/menu_sair.png");                                     //Carrega o sprite do fundo "Sair" do Menu
    Texture2D text_mouse = LoadTexture("Assets/Sprites/mouse.png");                                             //Carrega o sprite do ícone do mouse
    Texture2D text_mouse_2 = LoadTexture("Assets/Sprites/mouse-clique.png");                                    //Carrega o sprite do ícone pressionado do mouse
    Texture2D text_tela_delay = LoadTexture("Assets/Sprites/tela-delay.png");                                   //Carrega o sprite do fundo do Delay
    Texture2D text_escritorio_1 = LoadTexture("Assets/Sprites/escritorio-1.png");                               //Carrega o sprite do fundo 1 da Fase
    Texture2D text_escritorio_2 = LoadTexture("Assets/Sprites/escritorio-2.png");                               //Carrega o sprite do fundo 2 da Fase
    Texture2D text_upgrades = LoadTexture("Assets/Sprites/quadro de upgrades.png");                             //Carrega o sprite do fundo do Upgrade
    Image text_icon = LoadImage("icone-jogo.png");                                                              //Carrega o sprite do ícone do jogo
#pragma endregion Texturas

    SetWindowIcon(text_icon);                                                                                     //Inicia o ícone do jogo
    HideCursor();                                                                                                       //Esconde o cursor padrão
    //--------------------------------------------------------------------------------------



    // Início Jogo
    //--------------------------------------------------------------------------------------
    while (!WindowShouldClose()) {
        // Início Código
        //--------------------------------------------------------------------------------------
        Vector2 spriteMouse = GetMousePosition();                                                                       //Captura as posições do mouse para substituir pelo ícone

        if(IsKeyPressed(KEY_F)){
            ToggleFullscreen();
        }

        switch (telaAtual) {                                                                                            //Fluxo de Telas do Jogo
            case MENU:
            {
                if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))                                                       //Detectando o botão esquerdo do mouse
                {
                    Vector2 posMouse = GetMousePosition();                                                              //Capturando os valores de x e y do mouse

                    if(CheckCollisionPointRec(posMouse, botaoJogar))                                          //Clicando no botão "NOVO JOGO"
                    {
                        menu_novoJogo = true;                                                                           //Para desenhar o fundo do "Novo Jogo"
                        fase_vida = 3;                                                                                  //Definindo a quantidade de vida
                        fase_nivel = 1;                                                                                 //Definindo o nível padrão
                        qnt_bonus_tempo = 0;                                                                            //Zerando o tempo bônus
                        qnt_bonus_papel = 0;                                                                            //Zerando o papel bônus
                        val_desc_papel = 1;                                                                             //Definindo o desconto de cada clique do mouse
                    }
                    else if(CheckCollisionPointRec(posMouse, botaoSair))                                      //Clicando no botão "SAIR"
                    {
                        menu_sair = true;                                                                               //Para desenhar o fundo do "Sair"
                    }
                }

            }break;

            case UPGRADE:
            {
                upg_escolha = 0;                                                                                        //Definindo para a escolha inicial ser sempre nula

                if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))                                                      //Detectando o botão esquerdo do mouse
                {
                    Vector2 posMouse = GetMousePosition();                                                              //Capturando os valores de x e y do mouse

                    for(int i = 0; i <= 2; i++)                                                                         //Lendo as posições das opções de upgrade
                    {
                        if(CheckCollisionPointRec(posMouse, upgrade[i].tamanho))                              //Checando se a posição do mouse "colide" com algum upgrade
                        {
                            upg_escolha = upgrade[i].id;                                                                //Atribuindo o id do upgrade para futura execução
                        }
                    }
                }

                switch (upg_escolha) {                                                                                  //Com base na escolha do upgrade um conjunto de tarefas será executado
                    case 1:{
                        printf("O escolhido foi o upgrade 1!\n");
                        qnt_bonus_tempo += 15.00;
                        telaAtual = DELAY;
                    }break;

                    case 2: {
                        printf("O escolhido foi o upgrade 2!\n");
                        val_desc_papel += 1;
                        telaAtual = DELAY;
                    }break;

                    case 3: {
                        printf("O escolhido foi o upgrade 3!\n");
                        qnt_bonus_papel += 5;
                        telaAtual = DELAY;
                    }

                    default:{

                    }break;
                }

            }break;

            case DELAY:
            {
                qnt_cont_frames++;

                if(fase_delay_fim)                                                                                      //Variável atualizada no segundo DELAY switch
                {
                    qnt_cont_frames = 0;                                                                                //Resetando a contagem de frames para ser reutilizada
                    qnt_tempo = func_tempo() + qnt_bonus_tempo;                                                         //Definindo a quantidade de tempo da fase

                    if (fase_nivel == 5)                                                                                //Limitando o valor total de papéis para a fase 5
                    {
                        qnt_papel = 500;
                    }
                    else
                    {
                        qnt_papel = func_papel() - qnt_bonus_papel;                                                     //Definindo a quantidade de papel total da fase
                    }

                    if(val_desc_papel > 5)                                                                              //Limitando o bônus do clique para 5
                    {
                        val_desc_papel = 5;
                    }

                    telaAtual = FASE;                                                                                   //Redirecionando para a tela Fase
                }

                fase_delay_fim = false;                                                                                 //Atualizando a variável do delay para poder ser executado em outra chamada

            }break;

            case FASE:
            {
                if (qnt_tempo > 0.1)                                                                                    //Verificando se ainda há tempo na fase para habilitar a colisão com os papéis
                {
                    qnt_tempo -= 0.1;

                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))                                                 //Detectando se o botão esquerdo foi pressionado
                    {
                        Vector2 posMouse = GetMousePosition();                                                          //Capturando as posições de X e Y do mouse
                        col_mouse_caixa = CheckCollisionPointRec(posMouse, caixaPapeis);                      //Verificando se há colisão do mouse e caixa de papel
                    }

                    if(col_mouse_caixa)                                                                                 //Contando ponto caso a colisão com a caixa exista
                    {
                        qnt_papel -= val_desc_papel;                                                                    //Descontando um do total de papéis
                        col_mouse_caixa = false;                                                                        //Tornando falsa para evitar um bug no loop

                        if(qnt_papel <= 0){                                                                             //Impedindo um bug visual da quantidade de papéis
                            qnt_papel = 0;
                        }
                    }

                    if(qnt_tempo < 0.1){                                                                                //Impedindo um bug visual da quantidade de tempo
                        qnt_tempo = 0;
                    }

                }
            }break;
        }
        //--------------------------------------------------------------------------------------


        // Início Gráfico
        //--------------------------------------------------------------------------------------
        BeginDrawing();

        switch (telaAtual) {                                                                                            //Fluxo de Desenho das Telas do Jogo
            case MENU:
            {
                DrawRectangleRec(botaoJogar, WHITE);                                                          //Desenhando o botão de "NOVO JOGO"
                DrawRectangleRec(botaoSair, LIGHTGRAY);                                                       //Desenhando o botão de "SAIR"

                if(menu_novoJogo)                                                                                       //Verificando se "Novo Jogo" foi selecionado
                {
                    menu_novoJogo = false;                                                                              //Altera para falso para evitar um erro futuro
                    BeginDrawing();
                    DrawTexture(text_menu_novoJogo, 0, 0, LIGHTGRAY);                            //Desenha o novo fundo
                    EndDrawing();
                    telaAtual = DELAY;                                                                                  //Redirecionando para a tela de DELAY
                    WaitTime(1);
                }
                else if(menu_sair)                                                                                      //Verificando se "Sair" foi selecionado
                {
                    menu_sair = false;                                                                                  //Altera para falso para evitar um erro futuro
                    BeginDrawing();
                    DrawTexture(text_menu_sair, 0, 0, LIGHTGRAY);                                //Desenha o novo fundo
                    EndDrawing();
                    WaitTime(1);

                    UnloadTexture(text_papel);                                                                   //Descarregando o papel
                    UnloadTexture(text_menu);                                                                    //Descarregando o fundo do Menu
                    UnloadTexture(text_menu_novoJogo);                                                           //Descarregando o fundo "Novo Jogo" do Menu
                    UnloadTexture(text_menu_sair);                                                               //Descarregando o fundo "Sair" do Menu
                    UnloadTexture(text_mouse);                                                                   //Descarregando o Mouse 1
                    UnloadTexture(text_mouse_2);                                                                 //Descarregando o Mouse 2
                    UnloadTexture(text_tela_delay);                                                              //Descarregando o fundo da Delay
                    UnloadTexture(text_escritorio_1);                                                            //Descarregando o fundo 1 da Fase
                    UnloadTexture(text_escritorio_2);                                                            //Descarregando o fundo 2 da Fase
                    UnloadTexture(text_upgrades);                                                                //Descarregando o fundo do Upgrade
                    UnloadImage(text_icon);                                                                       //Descarregando o ícone do Jogo
                    CloseWindow();                                                                                      //Fechando o jogo
                }
                else
                {
                    DrawTexture(text_menu, 0, 0, LIGHTGRAY);                                      //Desenha o fundo padrão
                }
            }break;

            case UPGRADE:
            {
                DrawText("Tela de Upgrade", (telaLargura / 2 + 80), (telaAltura / 2) + 130, 30, LIGHTGRAY);                                     //Título da tela de Upgrades

                for(int i = 0; i <= 2; i++)                                                                                                                                   //Expondo os upgrades disponíveis
                {
                    DrawRectangleRec(upgrade[i].tamanho, upgrade[i].cor);                                                                                           //Desenhando os upgrades
                }

                DrawTexture(text_upgrades,0,0,WHITE);                                                                                                   //Desenhando o fundo
                DrawText(TextFormat("Tempo: %.2f   |", qnt_bonus_tempo),280,440,30,WHITE);                                                  //Expondo os valores do tempo
                DrawText(TextFormat("Clique: %d   |", val_desc_papel),590,440,30,WHITE);                                                    //Expondo os valores do clique
                DrawText(TextFormat("Papel: %d   |", qnt_bonus_papel),850,440,30,WHITE);                                                    //Expondo os valores do papel

            }break;

            case DELAY:
            {
                DrawTexture(text_tela_delay,0, 0, WHITE);
                DrawText(TextFormat("Fase %d", fase_nivel), (telaLargura / 2) - 40, (telaAltura / 2) - 200, 30, YELLOW);                     //Desenhando o nível atual
                DrawText(TextFormat("Vidas: %d", fase_vida), (telaLargura / 2) - 40, (telaAltura / 2) + 200, 30, YELLOW);                    //Desenhando as vidas

                if(qnt_cont_frames <= 60)                                                                                                                                      //Atualiza com base na contagem de frames (qnt_cont_frames)
                {
                    fase_delay = 3;                                                                                                                                            //Puxa o quarto valor da lista fase_delay_val("3")
                }
                else if(qnt_cont_frames <= 120)
                {
                    fase_delay = 2;                                                                                                                                            //Puxa o terceiro valor da lista fase_delay_val("2")
                }
                else if(qnt_cont_frames <= 180)
                {
                    fase_delay = 1;                                                                                                                                            //Puxa o segundo valor da lista fase_delay_val("1")
                }
                else if(qnt_cont_frames <= 300)
                {
                    fase_delay = 0;                                                                                                                                          //Puxa o primeiro valor da lista fase_delay_val("")
                    DrawText("Clique no Papel!!!", (telaLargura / 2) - 120, (telaAltura / 2) + 40, 30, YELLOW);                                //Desenhando a mensagem final da contagem
                }
                else
                {
                    fase_delay_fim = true;                                                                                                                                  //Encerra o loop do delay
                }

                DrawText(TextFormat("%s...", fase_delay_val[fase_delay]), (telaLargura / 2), (telaAltura / 2) + 40, 30, YELLOW);          //Desenhando a contagem

            }break;

            case FASE:
            {
                DrawRectangleRec(molduraTelaFase,BROWN);                                                                                                            //Desenhando a borda da tela

                if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(spriteMouse, caixaPapeis))
                {
                    DrawTexture(text_escritorio_2,6,6,WHITE);                                                                                           //Desenhando o fundo 2

                }
                else
                {
                    DrawTexture(text_escritorio_1,6,6,WHITE);                                                                                           //Desenhando o fundo 1
                }

                DrawRectangleRec(molduraPapeis,BLACK);                                                                                                              //Desenhando a moldura do papel
                DrawTexture(text_papel, 20,15, WHITE);                                                                                                  //Desenhando o papel
                DrawRectangleRec(molduraBaseQntPapeis, BLACK);
                DrawRectangleRec(molduraQntPapeis, LIGHTGRAY);                                                                                                       //Desenhando a moldura da quantidade de papel
                DrawRectangleRec(molduraBaseQntTempo, GOLD);                                                                                                         //Desenhando a primeira parte da moldura do tempo
                DrawRectangleRec(molduraQntTempo, BLACK);                                                                                                            //Desenhando a segunda parte da moldura do tempo
                DrawText(TextFormat("%.2f", qnt_tempo), (telaLargura / 2) + 15, 20 + 5, 20, GOLD);                                           //Desenhando o tempo
                DrawText(TextFormat("%d",qnt_papel), 80, 175, 25, BLACK);                                                                    //Desenhanto a quantidade de papel


                if((qnt_tempo < 0.1) && (qnt_papel > 0)){                                                                                                                     //Desenhando a Derrota
                    fase_vida -= 1;                                                                                                                                           //Diminuindo a vida em um

                    if(fase_vida == 0)                                                                                                                                       //Limite de vidas atingido
                    {
                        telaAtual = MENU;
                    }
                    else                                                                                                                                                     //Limite de vidas não atingido
                    {
                        telaAtual = UPGRADE;
                    }

                    BeginDrawing();
                    DrawRectangle((telaLargura / 2) - 70, (telaAltura / 2) - 10, 220, 100, RED);
                    DrawRectangle((telaLargura / 2) - 60, (telaAltura / 2), 200, 80, BLACK);
                    DrawText("PERDEU...", (telaLargura / 2) - 30, (telaAltura / 2) + 25, 30, RED);
                    EndDrawing();
                    WaitTime(2);
                }
                else if ((qnt_tempo > 0.1) && (qnt_papel <= 0))                                                                                                             //Desenhando a Vitória
                {
                    fase_nivel += 1;                                                                                                                                        //Aumentando o nível

                    if(fase_nivel > 5)                                                                                                                                      //Atingindo o nível máximo
                    {
                        telaAtual = MENU;
                    }
                    else                                                                                                                                                    //Não atingindo o nível máximo
                    {
                        telaAtual = UPGRADE;
                    }

                    BeginDrawing();
                    DrawRectangle((telaLargura / 2) - 70, (telaAltura / 2) - 10, 220, 100, GREEN);
                    DrawRectangle((telaLargura / 2) - 60, (telaAltura / 2), 200, 80, BLACK);
                    DrawText("GANHOU!!!", (telaLargura / 2) - 30, (telaAltura / 2) + 25, 30, GREEN);
                    EndDrawing();
                    WaitTime(2);
                }

            }break;
        }

        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            DrawTexture(text_mouse_2, spriteMouse.x - 10, spriteMouse.y - 10, WHITE);                  //Desenhando o ícone na atual posição do mouse

        }
        else
        {
            DrawTexture(text_mouse, spriteMouse.x - 10, spriteMouse.y - 10, WHITE);                    //Desenhando o ícone na atual posição do mouse
        }

        EndDrawing();
        //--------------------------------------------------------------------------------------
    }
    //--------------------------------------------------------------------------------------

    // Fechamento
    //--------------------------------------------------------------------------------------
    UnloadTexture(text_papel);                                                                                      //Descarregando o papel
    UnloadTexture(text_menu);                                                                                       //Descarregando o fundo do Menu
    UnloadTexture(text_menu_novoJogo);                                                                              //Descarregando o fundo "Novo Jogo" do Menu
    UnloadTexture(text_menu_sair);                                                                                  //Descarregando o fundo "Sair" do Menu
    UnloadTexture(text_mouse);                                                                                      //Descarregando o Mouse 1
    UnloadTexture(text_mouse_2);                                                                                    //Descarregando o Mouse 2
    UnloadTexture(text_tela_delay);                                                                                 //Descarregando o fundo da Delay
    UnloadTexture(text_escritorio_1);                                                                               //Descarregando o fundo 1 da Fase
    UnloadTexture(text_escritorio_2);                                                                               //Descarregando o fundo 2 da Fase
    UnloadTexture(text_upgrades);                                                                                   //Descarregando o fundo do Upgrade
    UnloadImage(text_icon);                                                                                         //Descarregando o ícone do Jogo
    CloseWindow();                                                                                                        //Fechando o jogo
    //--------------------------------------------------------------------------------------

    return 0;
}


/* CÓDIGOS QUE AINDA PODEM SER ÚTEIS
 *
 * --------|Uma outra forma de fazer o delay inicial|--------
 *
 *   while(fase_delay >= 0){
 *       BeginDrawing();
 *       WaitTime(1);
 *       ClearBackground(RAYWHITE);
 *       DrawText(TextFormat("%d", fase_delay), GetScreenWidth() / 2, GetScreenHeight() / 2, 30, LIGHTGRAY);
 *       fase_delay--;
 *       EndDrawing();
 *   }
 *   BeginDrawing();
 *   ClearBackground(RAYWHITE);
 *   DrawText("Clique nos Papéis!!!", GetScreenWidth() / 2 - 120, GetScreenHeight() / 2, 30, LIGHTGRAY);
 *   EndDrawing();
 *   WaitTime(1);
 *
 * !Desvantagens!: -Há a necessidade de utlizar as funções de desenho novamente;
 *                 -Uso do Wait na contagem;
 *
 * !Vantagens!: -Mais otimizado;
 *              -Possibilidade de escrever uma mensagem de "disparo" personalizada;
 *
 * --------|Uma outra forma de fazer o delay inicial|--------
 *
 *
 * --------|Outra forma de fazer o delay inicial|--------
 * for(fase_delay = 3; fase_delay >= 0; fase_delay--){
 *                  BeginDrawing();
 *                  ClearBackground(RAYWHITE);
 *                  printf("%d",fase_delay);
 *                  DrawText(TextFormat("%d !", fase_delay), GetScreenWidth() / 2 - 20, GetScreenHeight() / 2, 30, LIGHTGRAY);
 *                  WaitTime(1);
 *                  EndDrawing();
 *              }
 *              ClearBackground(RAYWHITE);
 *              DrawText("Clique nos Papéis!!!", GetScreenWidth() / 2 - 120, GetScreenHeight() / 2, 30, LIGHTGRAY);
 *              fase_delay_fim = true;
 *
 * !Desvantagens!: -Problema principal, falta de entendimento do porquê funciona somente com "0" e não com "1";
 *               -Há a necessidade de utlizar as funções de desenho novamente;
 *
 * !Vantagens!: -Mais ou menos otimizado;
 *            -Possibilidade de escrever uma mensagem de "disparo" personalizada;
 *
 *  --------|Outra forma de fazer o delay inicial|--------
 *
 *
 *  --------|Outra forma de fazer o delay inicial|--------
 *
 * for(fase_delay = 3; fase_delay >= -1; fase_delay--){
 *                  BeginDrawing();
 *                  ClearBackground(RAYWHITE);
 *                  printf("%d",fase_delay);
 *                  DrawText(TextFormat("%d !", fase_delay_val [fase_delay]), GetScreenWidth() / 2 - 20, GetScreenHeight() / 2, 30, LIGHTGRAY);
 *                  WaitTime(1);
 *                  EndDrawing();
 *              }
 *              fase_delay_fim = true;
 *
 * !Desvantagens!: -Problema principal, falta de entendimento do porquê funciona somente com "-1" e não com "0";
 *               -Há a necessidade de utlizar as funções de desenho novamente;
 *               -A contagem e mensagem de "disparo" precisam ter a mesma formatação;
 *
 * !Vantagens!: -Mais otimizado;
 *
 * --------|Outra forma de fazer o delay inicial|--------
 *
 *
 * --------|Outra forma de fazer a checagem do upgrade|--------
 * while(upg_escolha == 0){
                    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    {
                        Vector2 GetMouse = GetMousePosition();

                        for(int i = 0; i <= 2; i++)
                        {
                            if(CheckCollisionPointRec(GetMouse, upgrade[i].tamanho))
                            {
                                printf("%d \n", upgrade[i].id);
                                upg_escolha = upgrade[i].id;
                                printf("%d", upg_escolha);
                            }
                        }
                    }

                    switch (upg_escolha) {
                        case 1:{
                            printf("O escolhido foi o upgrade 1!");
                            //telaAtual = DELAY;
                        }break;

                        case 2: {
                            printf("O escolhido foi o upgrade 2!");
                            //telaAtual = DELAY;
                        }break;

                        default:{
                            //telaAtual = UPGRADE;
                        }break;
                    }
                }
 *
 *--------|Outra forma de fazer a checagem do upgrade|--------
 *
 *
 * --------|Lendo o tempo e papéis com base na struct|---------
 * case DELAY:
            {
                qnt_cont_frames++;

                if(fase_delay_fim)                                                                                      //Variável atualizada no segundo DELAY switch
                {
                    qnt_cont_frames = 0;                                                                                //Resetando a contagem de frames para ser reutilizada


                    qnt_tempo = fase[fase_nivel].tempo + qnt_bonus_tempo;                                               //Definindo a quantidade de tempo da fase
                    qnt_papel = fase[fase_nivel].papel - qnt_bonus_papel;                                               //Definindo a quantidade de papel total da fase


                    if(val_desc_papel > 5)                                                                              //Limitando o bônus do clique para 5
                    {
                        val_desc_papel = 5;
                    }


                    telaAtual = FASE;                                                                                   //Redirecionando para a tela Fase
                }

                fase_delay_fim = false;                                                                                 //Atualizando a variável do delay para poder ser executado em outra chamada
                }break;

--------------|Fazer com que o jogador retorne para a fase anterior quando perder|--------------------------
                case FASE:
            {
                DrawRectangleRec(molduraTelaFase,BROWN);                                                                                                            //Desenhando a borda da tela

                if(CheckCollisionPointRec(spriteMouse, caixaPapeis) && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                {
                    DrawTexture(text_escritorio_2,6,6,WHITE);                                                                                           //Desenhando o fundo 2

                }
                else
                {
                    DrawTexture(text_escritorio_1,6,6,WHITE);                                                                                           //Desenhando o fundo 1
                }

                DrawRectangleRec(molduraPapeis,BLACK);                                                                                                              //Desenhando a moldura do papel
                DrawTexture(text_papel, 20,15, WHITE);                                                                                                  //Desenhando o papel
                DrawRectangleRec(molduraBaseQntPapeis, BLACK);
                DrawRectangleRec(molduraQntPapeis, LIGHTGRAY);                                                                                                       //Desenhando a moldura da quantidade de papel
                DrawRectangleRec(molduraBaseQntTempo, GOLD);                                                                                                         //Desenhando a primeira parte da moldura do tempo
                DrawRectangleRec(molduraQntTempo, BLACK);                                                                                                            //Desenhando a segunda parte da moldura do tempo
                DrawText(TextFormat("%.2f", qnt_tempo), (telaLargura / 2) + 15, 20 + 5, 20, GOLD);                                           //Desenhando o tempo
                DrawText(TextFormat("%d",qnt_papel), 80, 175, 25, BLACK);                                                                    //Desenhanto a quantidade de papel


                if((qnt_tempo < 0.1) && (qnt_papel > 0)){                                                                                                                     //Desenhando a Derrota
                    fase_vida -= 1;                                                                                                                                           //Diminuindo a vida em um

                    if(fase_nivel == 1)                                                                                                                                     //Impedindo a fase 0
                    {
                        fase_nivel = 1;
                    }
                    else                                                                                                                                                      //Retornando para a fase anterior
                    {
                        fase_nivel -= 1;


                    if(fase_vida == 0)                                                                                                                                       //Limite de vidas atingido
                    {
                    telaAtual = MENU;
                    }
                    else                                                                                                                                                     //Limite de vidas não atingido
                    {
                    telaAtual = UPGRADE;
                    }

                    BeginDrawing();
                    DrawRectangle((telaLargura / 2) - 70, (telaAltura / 2) - 10, 220, 100, RED);
                    DrawRectangle((telaLargura / 2) - 60, (telaAltura / 2), 200, 80, BLACK);
                    DrawText("PERDEU...", (telaLargura / 2) - 30, (telaAltura / 2) + 25, 30, RED);
                    EndDrawing();
                    WaitTime(2);
                    }
                    else if ((qnt_tempo > 0.1) && (qnt_papel <= 0))                                                                                                             //Desenhando a Vitória
                    {
                    fase_nivel += 1;                                                                                                                                        //Aumentando o nível

                    if(fase_nivel > 5)                                                                                                                                      //Atingindo o nível máximo
                    {
                    telaAtual = MENU;
                    }
                    else                                                                                                                                                    //Não atingindo o nível máximo
                    {
                    telaAtual = UPGRADE;
                    }

                    BeginDrawing();
                    DrawRectangle((telaLargura / 2) - 70, (telaAltura / 2) - 10, 220, 100, GREEN);
                    DrawRectangle((telaLargura / 2) - 60, (telaAltura / 2), 200, 80, BLACK);
                    DrawText("GANHOU!!!", (telaLargura / 2) - 30, (telaAltura / 2) + 25, 30, GREEN);
                    EndDrawing();
                    WaitTime(2);
                    }

                    }break;
 */