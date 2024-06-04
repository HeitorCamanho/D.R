#include "raylib.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

/* DICIONÁRIO DAS VARIÁVEIS
 * ----------------------
 *                  |Tela|
 * telaLargura (const int) -> Responsável por definir a largura da tela;
 * telaAltura (const int) -> Responsável por definir o comprimento da tela;
 * cor_up (Color) -> Carrega um valor alfa para efeito na tela de Upgrade;
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


char fase_delay_val [4][2] = {"", "1","2","3"};                                                         //Lista que expõe a contagem do delay inicial

typedef struct {                                                                                                        //Struct dos Upgrades
    int id;
    char* descricao;
    Rectangle tamanho;
    Color cor;
} Upgrade;

typedef struct {
    int papel;
    double tempo;
} Fase;

enum telaJogo {MENU, UPGRADE, DELAY, FASE};                                                                             //Definindo as opções de Telas

Color cor_up = (Color){0,0,0,128};                                                                         //Criando efeito da tela de Upgrade
#pragma endregion Variáveis

#pragma region Funções Criação                                                                                                         //Criação das Funções
int func_tempo()                                                                                                        //Função para calcular o tempo
{
    int func_qnt_tempo = (12 * fase_nivel) + 15;
    return func_qnt_tempo;
    if (func_qnt_tempo < 0 || func_qnt_tempo == 0.10)
    {
        func_qnt_tempo = 0;
    }
}

int func_papel()                                                                                                        //Função para calcular a quantidade de papel
{
    double potencia = pow(4, fase_nivel);
    int func_qnt_papel =  potencia + 20;
    if (func_qnt_papel < 0)
    {
        func_qnt_papel = 0;
    }
    return func_qnt_papel;
}
#pragma endregion Funções
//--------------------------------------------------------------------------------------

int main(void)
{
    // Inicialização
    //--------------------------------------------------------------------------------------
    Upgrade upgrade[3];                                                                                                 //Vetor de structs que armazena os upgrades
#pragma region Upgrade Valores                                                                                                     //Criação dos upgrades
    upgrade[0].tamanho.x = 20;
    upgrade[0].tamanho.y = 120;
    upgrade[0].tamanho.width = 150;
    upgrade[0].tamanho.height = 225;
    upgrade[0].id = 1;
    upgrade[0].cor = GOLD;
    upgrade[0].descricao = "+1 Clique";

    upgrade[1].tamanho.x = 200;
    upgrade[1].tamanho.y = 120;
    upgrade[1].tamanho.width = 150;
    upgrade[1].tamanho.height = 225;
    upgrade[1].id = 2;
    upgrade[1].cor = RED;
    upgrade[1].descricao = "Mais Tempo";

    upgrade[2].tamanho.x = 380;
    upgrade[2].tamanho.y = 120;
    upgrade[2].tamanho.width = 150;
    upgrade[2].tamanho.height = 225;
    upgrade[2].id = 3;
    upgrade[2].cor = GREEN;
    upgrade[2].descricao = "Redução Papel";
#pragma endregion Upgrade

    Fase fase[6];
#pragma region Fase Valores                                                                                                        //Criação das fases
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

    Rectangle caixaPapeis = { 20, 15, 150, 180 };                                                   //Criando os valores da caixa de papéis
    Rectangle botaoJogar = {(telaLargura / 2) - 100, (telaAltura / 2), 250, 60};                    //Criando os valores do botão "NOVO JOGO"
    Rectangle botaoSair = {(telaLargura / 2) - 100, (telaAltura / 2) + 100, 250, 60};               //Criando os valores do botão "SAIR"

    InitWindow(telaLargura, telaAltura, "D.R:Demon's Resources");                                      //Criando a tela de Jogo
    SetWindowIcon(LoadImage("icone jogo.png"));                                                          //Carregando o ícone do arquivo
    SetTargetFPS(60);                                                                                               //Definindo a quantidade de frames por segundo

#pragma region Load Texturas
    Texture2D text_papel = LoadTexture("Assets/Sprites/papel3.0.png");                                          //Carrega o sprite dos papéis
    Texture2D text_menu = LoadTexture("Assets/Sprites/menu.png");                                               //Carrega o sprite do fundo do Menu
    Texture2D text_mouse = LoadTexture("Assets/Sprites/mouse.png");                                             //Carrega o sprite do ícone do mouse
    Texture2D text_mouse_2 = LoadTexture("Assets/Sprites/mouse-clique.png");
    Texture2D text_escritorio = LoadTexture("Assets/Sprites/escritorio.png");
    Texture2D text_tela_delay = LoadTexture("Assets/Sprites/tela-delay.png");
    Texture2D text_escritorio_1 = LoadTexture("Assets/Sprites/escritorio ortografico 1.png");
    Image text_icon = LoadImage("icone-jogo.png");
#pragma endregion Load

    SetWindowIcon(text_icon);
    //--------------------------------------------------------------------------------------



    // Início Jogo
    //--------------------------------------------------------------------------------------
    while (!WindowShouldClose()) {
        // Início Código
        //--------------------------------------------------------------------------------------
        Vector2 spriteMouse = GetMousePosition();                                                                       //Captura as posições do mouse para substituir pelo ícone
        HideCursor();                                                                                                   //Esconde o cursor padrão


        switch (telaAtual) {                                                                                            //Fluxo de Telas do Jogo
            case MENU:
            {
                if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))                                                       //Detectando o botão esquerdo do mouse
                {
                    Vector2 posMouse = GetMousePosition();                                                              //Capturando os valores de x e y do mouse

                    if(CheckCollisionPointRec(posMouse, botaoJogar))                                          //Clicando no botão "NOVO JOGO"
                    {
                        fase_vida = 3;                                                                                  //Definindo a quantidade de vida
                        fase_nivel = 1;                                                                                 //Definindo o nível padrão
                        qnt_bonus_tempo = 0;                                                                            //Zerando o tempo bônus
                        qnt_bonus_papel = 0;                                                                            //Zerando o papel bônus
                        val_desc_papel = 1;                                                                             //Definindo o desconto de cada clique do mouse
                        telaAtual = DELAY;                                                                              //Redirecionando para a tela de DELAY
                    }
                    else if(CheckCollisionPointRec(posMouse, botaoSair))                                      //Clicando no botão "SAIR"
                    {
                        UnloadTexture(text_papel);                                                               //Descarregando o papel
                        UnloadTexture(text_menu);                                                                //Descarregando o fundo Menu
                        UnloadTexture(text_mouse);                                                               //Descarregando o ícone mouse
                        UnloadTexture(text_mouse_2);                                                             //Descarregando o ícone mouse pressionado
                        UnloadImage(text_icon);                                                                   //Descarregnado o ícone do jogo
                        UnloadTexture(text_escritorio);                                                           //Descarregando o fundo da Fase
                        UnloadTexture(text_tela_delay);
                        UnloadTexture(text_escritorio_1);
                        CloseWindow();                                                                                  //Fechando o jogo
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

                        val_desc_papel += 1;
                        telaAtual = DELAY;
                    }break;

                    case 2: {
                        printf("O escolhido foi o upgrade 2!\n");

                        printf("%f", qnt_tempo);
                        qnt_bonus_tempo += 15.00;
                        telaAtual = DELAY;
                    }break;

                    case 3: {
                        printf("O escolhido foi o upgrade 3!\n");

                        qnt_bonus_papel += 10;
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

                    /*
                    qnt_tempo = fase[fase_nivel].tempo + qnt_bonus_tempo;                                               //Definindo a quantidade de tempo da fase
                    qnt_papel = fase[fase_nivel].papel - qnt_bonus_papel;                                               //Definindo a quantidade de papel total da fase
                    */

                    if(val_desc_papel > 5)                                                                              //Limitando o bônus do clique para 5
                    {
                        val_desc_papel = 5;
                    }

                    printf("Clique: %d\n", val_desc_papel);             //Teste Clique
                    printf("Papel: %d\n", qnt_papel);                   //Teste Papel
                    printf("Tempo: %.2f\n", qnt_tempo);                 //Teste Tempo

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
                        DrawText("FUNCIONOU !", posMouse.x, posMouse.y, 20, GOLD);        //Confirmação da captura (Apenas para teste)
                        col_mouse_caixa = CheckCollisionPointRec(posMouse, caixaPapeis);                      //Verificando se há colisão do mouse e caixa de papel
                    }

                    if(col_mouse_caixa)                                                                                 //Contando ponto caso a colisão com a caixa exista
                    {
                        qnt_papel -= val_desc_papel;                                                                    //Descontando um do total de papéis
                        col_mouse_caixa = false;                                                                        //Tonando falsa para evitar um bug no loop
                    }

                }
            }break;
        }
        //--------------------------------------------------------------------------------------


        // Início Gráfico
        //--------------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(RAYWHITE);                                                                                //Desenhando o fundo

        switch (telaAtual) {                                                                                            //Fluxo de Desenho das Telas do Jogo
            case MENU:
            {
                DrawTexture(text_menu, 0, 0, WHITE);
                DrawRectangleRec(botaoJogar, LIGHTGRAY);                                                                                                                     //Desenhando o botão de "NOVO JOGO"
                DrawRectangleRec(botaoSair, LIGHTGRAY);                                                                                                                      //Desenhando o botão de "SAIR"

                DrawText("NOVO JOGO", (telaLargura / 2) - 55, (telaAltura / 2) + 15, 30, RAYWHITE);
                DrawText("SAIR", (telaLargura / 2) - 15, (telaAltura / 2) + 115, 30, RAYWHITE);

            }break;

            case UPGRADE:
            {
                DrawText("Tela de Upgrade", (telaLargura / 2 + 80), (telaAltura / 2) + 130, 30, LIGHTGRAY);                                     //Título da tela de Upgrades

                for(int i = 0; i <= 2; i++)                                                                                                                                   //Expondo os upgrades disponíveis
                {
                    DrawRectangleRec(upgrade[i].tamanho, upgrade[i].cor);                                                                                           //Desenhando os upgrades
                    DrawText(TextFormat("%s", upgrade[i].descricao), upgrade[i].tamanho.x + 25, upgrade[i].tamanho.y + 80, 15, WHITE);
                }
                DrawRectangle(0,0,telaLargura,telaAltura,cor_up);

            }break;

            case DELAY:
            {
                DrawTexture(text_tela_delay,0, 0, WHITE);
                DrawText(TextFormat("Fase %d", fase_nivel), (telaLargura / 2) - 40, (telaAltura / 2) - 200, 30, YELLOW);             //Desenhando o nível atual
                DrawText(TextFormat("Vidas: %d", fase_vida), (telaLargura / 2) - 40, (telaAltura / 2) + 200, 30, YELLOW);            //Desenhando as vidas

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
                    fase_delay = 0;                                                                                                                                       //Puxa o primeiro valor da lista fase_delay_val("")
                    DrawText("Clique nos Papéis!!!", (telaLargura / 2) - 120, (telaAltura / 2), 30, YELLOW);                             //Desenhando a mensagem final da contagem
                }
                else
                {
                    fase_delay_fim = true;                                                                                                                                  //Encerra o loop do delay
                }

                DrawText(TextFormat("%s...", fase_delay_val[fase_delay]), (telaLargura / 2), (telaAltura / 2), 30, YELLOW);          //Desenhando a contagem

            }break;

            case FASE:
            {
                DrawRectangle(0,0,1280,720,BROWN);
                DrawTexture(text_escritorio_1,3,3, WHITE);
                DrawRectangle(15,10,160,190,BLACK);
                DrawTexture(text_papel, 20,15, WHITE);                                                                                                  //Desenhando a caixa de Papéis
                DrawText(TextFormat("%.2f", qnt_tempo), (telaLargura / 2), 20, 20, LIGHTGRAY);                                              //Desenhando o tempo
                DrawText(TextFormat("Papéis: %d",qnt_papel), (telaLargura / 2) + 200, 20, 20, LIGHTGRAY);                                   //Desenhanto a quantidade de papel


                if((qnt_tempo < 0.1) && (qnt_papel > 0)){                                                                                                                     //Desenhando a Derrota
                    fase_vida -= 1;                                                                                                                                           //Diminuindo a vida em um

                    /*if(fase_nivel == 1)                                                                                                                                     //Impedindo a fase 0
                    {
                        fase_nivel = 1;
                    }
                    else                                                                                                                                                      //Retornando para a fase anterior
                    {
                        fase_nivel -= 1;
                    }*/

                    if(fase_vida == 0)                                                                                                                                       //Limite de vidas atingido
                    {
                        telaAtual = MENU;
                    }
                    else                                                                                                                                                     //Limite de vidas não atingido
                    {
                        telaAtual = UPGRADE;
                    }

                    BeginDrawing();
                    DrawText("Perdeu!!!", (telaLargura / 2) - 60, (telaAltura / 2), 30, LIGHTGRAY);
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
                    DrawText("Ganhou!!!", (telaLargura / 2) - 60, (telaAltura / 2), 30, LIGHTGRAY);
                    EndDrawing();
                    WaitTime(2);
                }

            }break;
        }


        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
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
    UnloadTexture(text_papel);                                                                                   //Descarregando o papel
    UnloadTexture(text_menu);                                                                                    //Descarregando o fundo Menu
    UnloadTexture(text_mouse);                                                                                   //Descarregando o ícone mouse
    UnloadTexture(text_mouse_2);                                                                                 //Descarregando o ícone mouse pressionado
    UnloadImage(text_icon);                                                                                       //Descarregnado o ícone do jogo
    UnloadTexture(text_escritorio);                                                                               //Descarregando o fundo da Fase
    UnloadTexture(text_tela_delay);
    UnloadTexture(text_escritorio_1);
    CloseWindow();                                                                                                      //Fechando o jogo
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
 */