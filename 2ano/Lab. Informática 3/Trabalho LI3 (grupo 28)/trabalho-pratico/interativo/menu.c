#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>

#include "../include/gestor_utilizador.h"
#include "../include/struct_utilizador.h"
#include "../include/gestor_reserva.h"
#include "../include/struct_reserva.h"
#include "../include/gestor_voos.h"
#include "../include/struct_voos.h"
#include "../include/gestor_passageiro.h"
#include "../include/queries.h"
#include "../include/parser.h"
#include "../include/interpreter.h"
#include "../include/modo_interativo.h"
#include "../include/queries_interativas.h"
#include "../include/utilidade_tempo.h"



#define TAMANHO 5000
#define TAMANHO_HASH 10000
#define HASH_PASSAGEIROS 2000
#define HASH_VOOS_ERRADOS 2000
#define HASH_ASCII 300

#define RESERVA 0
#define VOO 1
#define PASSAGEIROS 2
#define UTILIZADORES 3


#define BLACK 0
#define RED 1
#define GREEN 2
#define YELLOW 3
#define BLUE 4
#define AMARELO 5
#define VERDE 6
#define AZUL 8
#define BRANCO 20

int calcular_pagina_final (char* resposta_original,int querie,int y_max)
{
    int espaco = 1;
    if (querie == 2) espaco = 5;
    if (querie == 9) espaco = 3;
    switch (querie)
    {
        case 1:
        {
        return 1;
        }
        case 3: 
        {
        return 1;
        }

        default:
        {
        char* resposta = strdup (resposta_original);
        char *token = strtok(resposta, "\n");
        int y = y_max /10 - 3;
        int pag_incompleta = 0;
        int numero_paginas = 1;

        for (numero_paginas = 1;pag_incompleta != 1;numero_paginas++)
        {
          y = y_max/10 - 3;
          while (y + espaco != y_max - 3 && token != NULL) 
          {
              token = strtok(NULL, "\n");
              if (token == NULL) pag_incompleta = 1;
              y++;
          }
        }
        return numero_paginas - 1;
        }
    }
    return 1;
}

void print_pagina (char* resposta_original,int querie,int pagina,WINDOW* caminho_win,int y_max,int x_max)
{
    int espaco = 1;
    if (querie == 2) espaco = 5;
    if (querie == 9) espaco = 3;
    char* resposta = strdup (resposta_original);
    switch (querie)
    {
        case 1:  
       {
         char* save_tok;
         char *token = strtok_r(resposta, "\n",&save_tok);
         int y = y_max /2 - 3;
       
        wclear (caminho_win);
        while (y + 1 != y_max - 3 && token != NULL) 
        {
             mvwprintw(caminho_win, y++, x_max/10 - 3, "%s", token);
             token = strtok_r(NULL, "\n",&save_tok);
         }
         }

        case 3:  //PRINT DA QUERIE 3 EM PAGINAS 
        {
            char* save_tok;
         char *token = strtok_r(resposta, "\n",&save_tok);
         int y = y_max /2 - 3;
      
     
         while (y + 1 != y_max - 3 && token != NULL) 
         {
             mvwprintw(caminho_win, y++, x_max/10 - 3, "%s", token);
             token = strtok_r(NULL, "\n",&save_tok);
         }
     
         wrefresh(caminho_win);
         break;
        }

        case 8:
        {
            char* save_tok;
         char *token = strtok_r(resposta, "\n",&save_tok);
         int y = y_max /2 - 3;
      
     
         while (y + 1 != y_max - 3 && token != NULL) 
         {
             mvwprintw(caminho_win, y++, x_max/10 - 3, "%s", token);
             token = strtok_r(NULL, "\n",&save_tok);
         }
     
         wrefresh(caminho_win);
         break;
        }

        default:  //PRINT DA QUERIE 9 // 2  EM PAGINAS 
        {
         char *token = strtok(resposta, "\n");
         int y = y_max /10 - 3;
      
       for (int percorrer_pag = 1;percorrer_pag < pagina;percorrer_pag++)
       {
         y = y_max/10 - 3;
         while (y + espaco != y_max - 3 && token != NULL) 
         {
             y++;
             token = strtok(NULL, "\n");
         }
       }
       y = y_max/10 - 3;
       
       wclear (caminho_win);
       while (y + espaco != y_max - 3 && token != NULL) 
       {
            if ((y == y_max/10 - 3 && strlen (token) < 3))  token = strtok(NULL, "\n");
            mvwprintw(caminho_win, y++, x_max/5, "%s", token);
            token = strtok(NULL, "\n");
        }

        wrefresh(caminho_win);
        break;
        }
    }
}

int select_querie()
{
    initscr();
    noecho();
    curs_set(0);
    start_color();
   int max_row, max_col;
    getmaxyx(stdscr, max_row, max_col);

    WINDOW *querie_win = newwin(max_row, max_col, 0, 0);

    wclear(querie_win);

    wattron(querie_win, A_BOLD);
    mvwprintw(querie_win, max_row/2 - 10, max_col/6, "Selecione uma Querie!");
    wattroff(querie_win, A_BOLD);
  
    keypad(querie_win,TRUE);

    int key;
    int selected = 0;

    do
    {
        for(int i = 0; i < 10; i++)
        {
            wattron(querie_win, COLOR_PAIR(10)| A_DIM);
            if(i == selected)
            {
                wattroff(querie_win, COLOR_PAIR(10) | A_DIM);
                wattron(querie_win, A_BOLD);	//Caso esteja selecionado ativamos os atributos
				wattron(querie_win, COLOR_PAIR(4));
            }
       
            switch(i)
            {
            case 0:
                mvwprintw(querie_win, max_row/2 - 8, max_col/6, "Querie 1            Nesta querie pode ver o resume de um utilizador,voo ou reserva");
                break;
            case 1:
                 mvwprintw(querie_win, max_row/2 - 6, max_col/6, "Querie 2            Nesta querie pode listar os voos e reservas de um utilizador");
                 break;
            case 2:
                 mvwprintw(querie_win, max_row/2 - 4, max_col/6, "Querie 3            Nesta querie pode ver a classificacao media de um hotel");
                 break;
            case 3:
                 mvwprintw(querie_win, max_row/2 - 2, max_col/6, "Querie 4            Nesta querie pode listar as reservas de um hotel");
                 break;
            case 4:
                 mvwprintw(querie_win,max_row/2, max_col/6, "Querie 5            Nesta querie pode listar voos de um aeroporto entre 2 datas");
                 break;
            case 5:
	             mvwprintw(querie_win, max_row/2 +2, max_col/6, "Querie 6            Nesta querie pode listar aeroportos com mais passageiro num certo ano");
                 break;
            case 6:
                 mvwprintw(querie_win, max_row/2 + 4, max_col/6, "Querie 7            Nesta querie pode listar aeroportos com maior mediana de atrasos");
                 break;
            case 7:
                 mvwprintw(querie_win, max_row/2 +6, max_col/6, "Querie 8            Nesta querie pode ver a receita total de um hotel entre 2 datas");
                 break;
            case 8:
	             mvwprintw(querie_win, max_row/2 +8, max_col/6, "Querie 9            Nesta querie pode listar todos os nomes cujo nome começa com um certo prefixo");
                 break;
            case 9:
                 mvwprintw(querie_win, max_row/2 +10, max_col/6, "Querie 10           Nesta querie pode ver varias metricas gerais da aplicacao");
                 break;
            }
            wattroff(querie_win, A_BOLD);	//Desliga o atributo quando trocamos de opção
			wattroff(querie_win, COLOR_PAIR(4));
        }

        wrefresh(querie_win);
        key = wgetch(querie_win); //Input do utilizador

        if (key == '8' || key == KEY_UP) 
		{
            selected--;
            if (selected < 0)
			{
				selected = 9;	//Caso tentemos exceder o topo dá wormhole para baixo
			} 
        }
        else if (key == '2' || key == KEY_DOWN) 
		{
            selected++;
            if (selected > 9)
			{
				selected = 0; //Caso tentemos exceder o minimo dá wormhole para cima
			} 
        }
        
    } while (key != '\n');
    
    endwin();
    return selected;
}

int escolha_output(int querie)
{
      int selected = 0;
      int exit_flag = -1;
    while (true)
    {
        initscr();
        noecho();
        curs_set(0);
        start_color();
        

       int max_row, max_col;
       getmaxyx(stdscr, max_row, max_col);

        WINDOW *escolha_output_win = newwin(max_row, max_col, 0, 0);

        keypad(escolha_output_win, TRUE);

        wclear(escolha_output_win);
        int i;

        wattron(escolha_output_win, A_BOLD );
        mvwprintw(escolha_output_win,2,30,"Pressiona q para voltar");
        wattroff(escolha_output_win, A_BOLD );

        mvwprintw(escolha_output_win,30,20,"Tipo 1");
        mvwprintw(escolha_output_win,32,20,"Tipo 2");
        wrefresh(escolha_output_win);
        
    
        switch (querie)
        {
            case 0:
                i = 10;
                
                wattron(escolha_output_win, A_BOLD);
                wattron(escolha_output_win, COLOR_PAIR(1));
                mvwprintw(escolha_output_win,i - 1,40,"Input: LGarcia1208");
                mvwprintw(escolha_output_win,i+1,40,"    tipo 1                                                                                 tipo 2(F)");
                wattroff(escolha_output_win, A_BOLD);
                wattroff(escolha_output_win, COLOR_PAIR(1));
                mvwprintw(escolha_output_win,i+2, 40,"                                                                                           --- 1 ---");
                mvwprintw(escolha_output_win,i+3, 40,"                                                                                           name: Lia Garcia");
                mvwprintw(escolha_output_win,i+4, 40,"Lia Garcia;F;27;PT;RQ468696;11;6;3034.800                                                  sex: F");
                mvwprintw(escolha_output_win,i+5, 40,"                                                                                           age: 27");
                mvwprintw(escolha_output_win,i+6, 40,"                                                                                           country_code: PT");
                mvwprintw(escolha_output_win,i+7, 40,"                                                                                           passport: RQ468696");
                mvwprintw(escolha_output_win,i+8, 40,"                                                                                           number_of_flights: 11");
                mvwprintw(escolha_output_win,i+9, 40,"                                                                                           number_of_reservations: 6");
                mvwprintw(escolha_output_win,i+10,40,"                                                                                           total_spent: 3034.800");
                mvwprintw(escolha_output_win,i+11,40,"                                                                                             (etc.)");
                
                
                wattron(escolha_output_win, A_BOLD);
                wattron(escolha_output_win, COLOR_PAIR(2));
                mvwprintw(escolha_output_win,i+13,40,"Input: 946");
                mvwprintw(escolha_output_win,i+15,40,"    tipo 1                                                                                tipo 2(F)");
                wattroff(escolha_output_win, A_BOLD);
                wattroff(escolha_output_win, COLOR_PAIR(2));
                mvwprintw(escolha_output_win,i+16,40,"                                                                                          --- 1 ---");
                mvwprintw(escolha_output_win,i+17,40,"                                                                                          airline: Transavia");
                mvwprintw(escolha_output_win,i+18,40,"Transavia;Airbus A320;AMS;NCE;2023/05/06 17:15:55;2023/05/06 19:35:55;83;0                plane_model: Airbus A320            ");
                mvwprintw(escolha_output_win,i+19,40,"                                                                                          origin: AMS");
                mvwprintw(escolha_output_win,i+20,40,"                                                                                          destination: NCE");
                mvwprintw(escolha_output_win,i+21,40,"                                                                                          schedule_departure_date: 2023/05/06 17:15:55");
                mvwprintw(escolha_output_win,i+22,40,"                                                                                          schedule_arrival_date: 2023/05/06 19:35:55");
                mvwprintw(escolha_output_win,i+23,40,"                                                                                          passengers: 83");
                mvwprintw(escolha_output_win,i+24,40,"                                                                                          delay: 0");
                mvwprintw(escolha_output_win,i+25,40,"                                                                                            (etc.)");

                
                wattron(escolha_output_win, A_BOLD);
                wattron(escolha_output_win, COLOR_PAIR(3));
                mvwprintw(escolha_output_win,i+27,40,"Input: Book156");
                mvwprintw(escolha_output_win,i+29,40,"    tipo 1                                                                                tipo 2(F)");
                wattroff(escolha_output_win, A_BOLD);
                wattroff(escolha_output_win, COLOR_PAIR(3));
                mvwprintw(escolha_output_win,i+30,40,"                                                                                          --- 1 ---");
                mvwprintw(escolha_output_win,i+31,40,"                                                                                          hotel_id: HTL904");
                mvwprintw(escolha_output_win,i+32,40,"HTL904;Chiado Plaza Hotel;3;2023/09/01;2023/09/03;False;2;306.000                         hotel_name: Chiado Plaza Hotel");
                mvwprintw(escolha_output_win,i+33,40,"                                                                                          hotel_stars: 3");
                mvwprintw(escolha_output_win,i+34,40,"                                                                                          begin_date: 2023/09/01");
                mvwprintw(escolha_output_win,i+35,40,"                                                                                          end_date: 2023/09/03");
                mvwprintw(escolha_output_win,i+36,40,"                                                                                          includes_breakfast: False");
                mvwprintw(escolha_output_win,i+37,40,"                                                                                          nights: 2");
                mvwprintw(escolha_output_win,i+38,40,"                                                                                          total_price: 306.000");
                mvwprintw(escolha_output_win,i+39,40,"                                                                                             (etc.)");
                break;
            case 1:
                i = 5;
                 wattron(escolha_output_win, A_BOLD);
                wattron(escolha_output_win, COLOR_PAIR(1));
                mvwprintw(escolha_output_win,i-1,40,"Input: JéssiTavares910");
                mvwprintw(escolha_output_win,i+1,40,"tipo 1                                                      tipo 2(F)");
                wattroff(escolha_output_win, A_BOLD);
                wattroff(escolha_output_win, COLOR_PAIR(1));
                mvwprintw(escolha_output_win,i+2, 40,"0000000922;2023/07/19;flight                                --- 1 ---");
                mvwprintw(escolha_output_win,i+3, 40,"Book0000037981;2023/07/19;reservation                       id: 0000000922");
                mvwprintw(escolha_output_win,i+4, 40,"0000000796;2023/06/11;flight                                date: 2023/07/19");
                mvwprintw(escolha_output_win,i+5, 40,"0000000163;2023/02/10;flight                                type: flight");
                mvwprintw(escolha_output_win,i+6, 40,"Book0000006454;2023/02/10;reservation");
                mvwprintw(escolha_output_win,i+7, 40,"0000000582;2022/12/20;flight                                --- 2 ---");
                mvwprintw(escolha_output_win,i+8, 40,"0000000115;2022/10/21;flight                                id: Book0000037981");
                mvwprintw(escolha_output_win,i+9, 40,"0000000447;2022/10/11;flight                                date: 2023/07/19");
                mvwprintw(escolha_output_win,i+10,40,"Book0000018300;2022/10/11;reservation                       type: reservation");
                mvwprintw(escolha_output_win,i+11,40,"0000000022;2022/10/06;flight");
                mvwprintw(escolha_output_win,i+12,40,"0000000769;2022/01/14;flight                                --- 3 ---");
                mvwprintw(escolha_output_win,i+13,40,"Book0000031687;2022/01/14;reservation                       id: 0000000796");      
                mvwprintw(escolha_output_win,i+14,40,"0000000764;2021/12/11;flight                                date: 2023/06/11");
                mvwprintw(escolha_output_win,i+15,40,"Book0000031542;2021/12/11;reservation                       type: flight");
                mvwprintw(escolha_output_win,i+16,40,"0000000534;2021/10/04;flight");
                mvwprintw(escolha_output_win,i+17,40,"0000000459;2021/08/18;flight                                --- 4 --- ");
                mvwprintw(escolha_output_win,i+18,40,"0000000459;2021/08/18;flight                                   (etc.) ");

                i = 26;
                 wattron(escolha_output_win, A_BOLD);
                wattron(escolha_output_win, COLOR_PAIR(2));
                mvwprintw(escolha_output_win,i-1,40,"Input: JéssiTavares910 flights");
                mvwprintw(escolha_output_win,i+1,40,"tipo 1                                                      tipo 2(F)");
                 wattroff(escolha_output_win, A_BOLD);
                wattroff(escolha_output_win, COLOR_PAIR(2));
                mvwprintw(escolha_output_win,i+2, 40,"0000000922;2023/07/19                                       --- 1 ---");
                mvwprintw(escolha_output_win,i+3, 40,"0000000796;2023/06/11                                       id: 0000000922");
                mvwprintw(escolha_output_win,i+4, 40,"0000000163;2023/02/10                                       date: 2023/07/19");
                mvwprintw(escolha_output_win,i+5, 40,"0000000582;2022/12/20");                    
                mvwprintw(escolha_output_win,i+6, 40,"0000000115;2022/10/21                                       --- 2 ---");
                mvwprintw(escolha_output_win,i+7, 40,"0000000447;2022/10/11                                       id: 0000000796");
                mvwprintw(escolha_output_win,i+8, 40,"0000000769;2022/01/14");                    
                mvwprintw(escolha_output_win,i+9, 40,"0000000764;2021/12/11                                       --- 3 ---");
                mvwprintw(escolha_output_win,i+10,40,"0000000534;2021/10/04                                       id: 0000000163");
                mvwprintw(escolha_output_win,i+11,40,"0000000459;2021/08/18                                       date: 2023/02/10");
                mvwprintw(escolha_output_win,i+12,40,"                                                              (etc.)");

                i = 42;
                 wattron(escolha_output_win, A_BOLD);
                wattron(escolha_output_win, COLOR_PAIR(3));
                mvwprintw(escolha_output_win,i-1,40,"Input: JéssiTavares910 reservations");  
                mvwprintw(escolha_output_win,i+1,40,"tipo 1                                                      tipo 2(F)");    
                 wattroff(escolha_output_win, A_BOLD);
                wattroff(escolha_output_win, COLOR_PAIR(3));
                mvwprintw(escolha_output_win,i+2,40,"Book0000037981;2023/07/19                                   --- 1 ---");
                mvwprintw(escolha_output_win,i+3,40,"Book0000006454;2023/02/10                                   id: Book0000037981");
                mvwprintw(escolha_output_win,i+4,40,"Book0000018300;2022/10/11                                   date: 2023/07/19");
                mvwprintw(escolha_output_win,i+5,40,"Book0000031687;2022/01/14");                    
                mvwprintw(escolha_output_win,i+6,40,"Book0000031542;2021/12/11                                   --- 2 ---");
                mvwprintw(escolha_output_win,i+7,40,"                                                            id: Book0000006454");
                mvwprintw(escolha_output_win,i+8,40,"                                                            date: 2023/02/10");
                mvwprintw(escolha_output_win,i+9,40,"                                                               (etc.) ");
                break;
            case 2:
            i = 20;
                wattron(escolha_output_win, A_BOLD);
                wattron(escolha_output_win, COLOR_PAIR(1));
                mvwprintw(escolha_output_win,i-1,40,"Input: HTL1003");
                mvwprintw(escolha_output_win,i+1,40,"tipo 1                                                      tipo 2(F)");
                wattroff(escolha_output_win, A_BOLD);
                wattroff(escolha_output_win, COLOR_PAIR(1));
                mvwprintw(escolha_output_win,i+3,40,"                                                            --- 1 ---");
                mvwprintw(escolha_output_win,i+4,40,"2.449                                                       rating: 2.449");
                break;
            case 3:
                i = 20;
                wattron(escolha_output_win, A_BOLD);
                wattron(escolha_output_win, COLOR_PAIR(1));
                mvwprintw(escolha_output_win,i-1,40,"Input: HTL1003");
                mvwprintw(escolha_output_win,i+1,40,"tipo 1                                                                                          tipo 2(F)");
                wattroff(escolha_output_win, A_BOLD);
                wattroff(escolha_output_win, COLOR_PAIR(1));
                mvwprintw(escolha_output_win,i+2, 40,"Book0000030182;2023/07/21;2023/07/24;LuíPereira;2;489.600                                       --- 1 ---");
                mvwprintw(escolha_output_win,i+3, 40,"Book0000030183;2023/07/21;2023/07/24;DanielPinto643;2;489.600                                   id: Book0000030182");
                mvwprintw(escolha_output_win,i+4, 40,"Book0000030188;2023/07/21;2023/07/23;CGonçalves829;3;326.400                                    begin_date: 2023/07/21");
                mvwprintw(escolha_output_win,i+5, 40,"Book0000030189;2023/07/21;2023/07/23;MarCardoso1978;2;326.400                                   end_date: 2023/07/24");
                mvwprintw(escolha_output_win,i+6, 40,"Book0000030196;2023/07/21;2023/07/25;NoCampos;2;652.800                                         user_id: LuíPereira");
                mvwprintw(escolha_output_win,i+7, 40,"Book0000030201;2023/07/21;2023/07/24;Violeta-MaBarbosa;1;489.600                                rating: 2");
                mvwprintw(escolha_output_win,i+8, 40,"Book0000030203;2023/07/21;2023/07/25;PeFerreira;1;652.800                                       total_price: 489.600");
                mvwprintw(escolha_output_win,i+9, 40,"Book0000030205;2023/07/21;2023/07/24;FrMoura1715;3;489.600   ");
                mvwprintw(escolha_output_win,i+10,40,"Book0000030206;2023/07/21;2023/07/22;GabSá468;4;163.200                                         --- 2 ---");
                mvwprintw(escolha_output_win,i+11,40,"Book0000030219;2023/07/21;2023/07/23;IsaOliveira562;4;326.400                                   id: Book0000030183");
                mvwprintw(escolha_output_win,i+12,40,"Book0000030220;2023/07/21;2023/07/22;YaAnjos;4;163.200                                          begin_date: 2023/07/21");
                mvwprintw(escolha_output_win,i+13,40,"Book0000030221;2023/07/21;2023/07/25;ÂngRocha1874;3;652.800                                     end_date: 2023/07/24");     
                mvwprintw(escolha_output_win,i+14,40,"Book0000027513;2023/07/18;2023/07/19;LeandrGuerreiro;1;163.200                                  user_id: DanielPinto643");
                mvwprintw(escolha_output_win,i+15,40,"Book0000027531;2023/07/18;2023/07/19;MatVicente318;1;163.200                                    rating: 2");
                mvwprintw(escolha_output_win,i+16,40,"Book0000027538;2023/07/18;2023/07/22;HuMendes1216;1;652.800                                     total_price: 489.600");
                mvwprintw(escolha_output_win,i+17,40,"Book0000027546;2023/07/18;2023/07/19;LuciaPaiva657;2;163.200");
                mvwprintw(escolha_output_win,i+18,40,"Book0000027550;2023/07/18;2023/07/21;RafaBaptista873;2;489.600                                  --- 3 ---");
                mvwprintw(escolha_output_win,i+18,40,"Book0000027555;2023/07/18;2023/07/19;Helena-BFreitas;3;163.200                                  id: Book0000030188");
                mvwprintw(escolha_output_win,i+19,40,"Book0000027558;2023/07/18;2023/07/20;JBranco;4;326.400                                          begin_date: 2023/07/21");
                mvwprintw(escolha_output_win,i+20,40,"                                                                                                (etc.)");
                break;
            case 4:
                i = 20;
                wattron(escolha_output_win, A_BOLD);
                wattron(escolha_output_win, COLOR_PAIR(1));
                mvwprintw(escolha_output_win,i-1,40,"Input: LIS \"2023/01/01 00:00:00\" \"2023/05/31 23:59:59\" ");
                mvwprintw(escolha_output_win,i+1,40,"tipo 1                                                                            tipo 2(F)");
                wattroff(escolha_output_win, A_BOLD);
                wattroff(escolha_output_win, COLOR_PAIR(1));
                mvwprintw(escolha_output_win,i+2, 40,"0000000938;2023/04/22 00:24:24;MAD;TAP Air Portugal;Airbus A320                   --- 1 ---");
                mvwprintw(escolha_output_win,i+3, 40,"0000000070;2023/03/18 02:24:30;MAD;TAP Air Portugal;Airbus A320                   id: 0000000938");
                mvwprintw(escolha_output_win,i+4, 40,"0000000487;2023/02/20 06:54:54;MAD;TAP Air Portugal;Airbus A320                   schedule_departure_date: 2023/04/22 00:24:24");
                mvwprintw(escolha_output_win,i+5, 40,"0000000398;2023/01/15 11:00:17;MAD;TAP Air Portugal;Airbus A320                   destination: MAD   ");
                mvwprintw(escolha_output_win,i+6, 40,"                                                                                  airline: TAP Air Portugal");                    
                mvwprintw(escolha_output_win,i+7, 40,"                                                                                  plane_model: Airbus A320");                    
                mvwprintw(escolha_output_win,i+8, 40,"                                                                                  --- 2 ---");
                mvwprintw(escolha_output_win,i+9, 40,"                                                                                  id: 0000000070");
                mvwprintw(escolha_output_win,i+10,40,"                                                                                  schedule_departure_date: 2023/03/18 02:24:30");
                mvwprintw(escolha_output_win,i+11,40,"                                                                                  destination: MAD");
                mvwprintw(escolha_output_win,i+12,40,"                                                                                  (etc.)");
                break;
            case 5:
                i = 20;
                wattron(escolha_output_win, A_BOLD);
                wattron(escolha_output_win, COLOR_PAIR(1));
                mvwprintw(escolha_output_win,i-1,40,"Input: 2021 10");
                mvwprintw(escolha_output_win,i+1,40,"tipo 1                    tipo 2(F)");
                wattroff(escolha_output_win, A_BOLD);
                wattroff(escolha_output_win, COLOR_PAIR(1));
                mvwprintw(escolha_output_win,i+2, 40,"LIS;3816                  name: MAD");
                mvwprintw(escolha_output_win,i+3, 40,"FCO;3233                  passengers: 4572");
                mvwprintw(escolha_output_win,i+4, 40,"AMS;3054");
                mvwprintw(escolha_output_win,i+5, 40,"PRG;2980                  --- 2 ---");
                mvwprintw(escolha_output_win,i+6, 40,"CDG;2867                  name: LIS");                    
                mvwprintw(escolha_output_win,i+7, 40,"DUB;2763                  passengers: 3816");                    
                mvwprintw(escolha_output_win,i+8, 40,"LHR;2439");
                mvwprintw(escolha_output_win,i+9, 40,"CPH;2423                  --- 3 ---");
                mvwprintw(escolha_output_win,i+10,40,"OSL;2423                  name: FCO");
                mvwprintw(escolha_output_win,i+11,40,"                          (etc.)");
                break;
            case 6:
                i = 20;
                wattron(escolha_output_win, A_BOLD);
                wattron(escolha_output_win, COLOR_PAIR(1));
                mvwprintw(escolha_output_win,i-1,40,"Input: 10");
                mvwprintw(escolha_output_win,i+1,40,"tipo 1                    tipo 2(F)");
                wattroff(escolha_output_win, A_BOLD);
                wattroff(escolha_output_win, COLOR_PAIR(1));
                mvwprintw(escolha_output_win,i+2, 40,"IST;900                 --- 1 ---   ");
                mvwprintw(escolha_output_win,i+3, 40,"LIS;900                 name: IST");
                mvwprintw(escolha_output_win,i+4, 40,"VIE;900                 median: 900");
                mvwprintw(escolha_output_win,i+5, 40,"WAW;450");
                mvwprintw(escolha_output_win,i+6, 40,"AMS;300                 --- 2 ---");         
                mvwprintw(escolha_output_win,i+7, 40,"ARN;300                 name: LIS");               
                mvwprintw(escolha_output_win,i+8, 40,"CDG;300                 median: 900");
                mvwprintw(escolha_output_win,i+9, 40,"DUB;300");
                mvwprintw(escolha_output_win,i+10,40,"FCO;300                 --- 3 ---");
                mvwprintw(escolha_output_win,i+11,40,"FRA;300                 name: VIE");
                mvwprintw(escolha_output_win,i+12,40,"FRA;300                 etc.)");
                break;
            case 7:
                i = 28;
                wattron(escolha_output_win, A_BOLD);
                wattron(escolha_output_win, COLOR_PAIR(1));
                mvwprintw(escolha_output_win,i-1,40,"Input: HTL1001 2023/05/01 2023/06/01");
                mvwprintw(escolha_output_win,i+1,40,"tipo 1                    tipo 2(F)");
                wattroff(escolha_output_win, A_BOLD);
                wattroff(escolha_output_win, COLOR_PAIR(1));
                mvwprintw(escolha_output_win,i+2,40,"13640                     --- 1 ---");
                mvwprintw(escolha_output_win,i+3,40,"                          revenue: 13640");
                break;
            case 8:
                i = 20;
                wattron(escolha_output_win, A_BOLD);
                wattron(escolha_output_win, COLOR_PAIR(1));
                mvwprintw(escolha_output_win,i-1,40,"Input: Julia");
                mvwprintw(escolha_output_win,i+1,40,"tipo 1                                                  tipo 2(F)");
                wattroff(escolha_output_win, A_BOLD);
                wattroff(escolha_output_win, COLOR_PAIR(1));
                mvwprintw(escolha_output_win,i+2, 40,"JulAlves1633;Juliana Alves                              --- 1 ---");
                mvwprintw(escolha_output_win,i+3, 40,"JAndrade;Juliana Andrade                                id: JulAlves1633");
                mvwprintw(escolha_output_win,i+4, 40,"JulAnjos;Juliana Anjos                                  name: Juliana Alves");
                mvwprintw(escolha_output_win,i+5, 40,"JulAntunes931;Juliana Antunes");
                mvwprintw(escolha_output_win,i+6, 40,"JBatista;Juliana Batista                                --- 2 ---");         
                mvwprintw(escolha_output_win,i+7, 40,"JuliaBatista;Juliana Batista                            id: JAndrade");    
                mvwprintw(escolha_output_win,i+8, 40,"JuliBatista1204;Juliana Batista                         name: Juliana Andrade");
                mvwprintw(escolha_output_win,i+9, 40,"Juliana-Branco;Juliana-Bianca Branco");
                mvwprintw(escolha_output_win,i+10,40,"JuCarvalho;Juliana Carvalho                             --- 3 ---");
                mvwprintw(escolha_output_win,i+11,40,"JuCastro1012;Juliana Castro                             id: JulAnjos");
                mvwprintw(escolha_output_win,i+12,40,"Juliana-CHenriques;Juliana-Catarina Henriques           name: Juliana Anjos");  
                mvwprintw(escolha_output_win,i+13,40,"JuAlmeida;Juliana da Almeida");    
                mvwprintw(escolha_output_win,i+14,40,"JuliaAndrade1578;Juliana da Andrade                     --- 4 ---");
                mvwprintw(escolha_output_win,i+15,40,"(etc.)                                                  (etc.)");
                break;
            case 9: 
                i = 20;
                wattron(escolha_output_win, A_BOLD);
                wattron(escolha_output_win, COLOR_PAIR(1));
                mvwprintw(escolha_output_win,i-1,40,"Input: 2023");
                mvwprintw(escolha_output_win,i+1,40,"tipo 1                                                  tipo 2(F)");
                wattroff(escolha_output_win, A_BOLD);
                wattroff(escolha_output_win, COLOR_PAIR(1));
                mvwprintw(escolha_output_win,i+2, 40,"1;0;37;2867;2487;1512                                   --- 1 ---");
                mvwprintw(escolha_output_win,i+3, 40,"2;0;25;1919;1741;1067                                   month: 1");
                mvwprintw(escolha_output_win,i+4, 40,"3;0;30;2247;2011;1154                                   users: 0");
                mvwprintw(escolha_output_win,i+5, 40,"4;0;29;2158;1952;1029                                   flights: 37");
                mvwprintw(escolha_output_win,i+6, 40,"5;0;32;2592;2305;1364                                   passengers: 2867");     
                mvwprintw(escolha_output_win,i+7, 40,"6;0;39;3009;2590;1501                                   unique_passengers: 2487");   
                mvwprintw(escolha_output_win,i+8, 40,"7;0;25;2228;1995;1200                                   reservations: 1512");
                mvwprintw(escolha_output_win,i+9, 40,"8;0;27;2074;1866;1043");
                mvwprintw(escolha_output_win,i+10,40,"9;0;1;81;81;34                                          --- 2 ---"); 
                mvwprintw(escolha_output_win,i+11,40,"                                                        (etc.)");
                break;
        }
        wrefresh(escolha_output_win);

        for (int i = 0; i < 2; i++) {
            if (i == selected) {
                wattron(escolha_output_win, A_BOLD);
                wattron(escolha_output_win, COLOR_PAIR(4));
            }

            switch (i) {
                case 0:
                    mvwprintw(escolha_output_win, 30, 20, "Tipo 1");
                    break;
                case 1:
                    mvwprintw(escolha_output_win, 32, 20, "Tipo 2");
                    break;
            }

            wattroff(escolha_output_win, A_BOLD);
            wattroff(escolha_output_win, COLOR_PAIR(4));
        }

        wrefresh(escolha_output_win);
        int ch = wgetch(escolha_output_win);

        if (ch == 'q') 
        {
            return exit_flag;
        }

        if (ch == '\n') 
        {
            endwin();
            return selected;
        }

        if (ch == '8' || ch == KEY_UP) {
            selected--;
            if (selected < 0) {
                selected = 1;
            }
        } else if (ch == '2' || ch == KEY_DOWN) {
            selected++;
            if (selected > 1) {
                selected = 0;
            }
        }
    }
}

char* write_querie(int querie,int n_res,int n_voos,int n_utili)
{
    setlocale(LC_COLLATE, "en_US.UTF-8");
    initscr();
    noecho();
    curs_set(0);
    start_color();
    
    int max_row, max_col;
    getmaxyx(stdscr, max_row, max_col);
    WINDOW *write_querie_win = newwin(max_row, max_col, 0, 0);
    wclear(write_querie_win);

    int i = 32;

    mvwprintw(write_querie_win,i-2,40,"Estes são os formatos do input para esta querie!");

    switch (querie)
    {
    case 0:
        mvwprintw(write_querie_win,i,40,  "Input: LGarcia1208          (Username // Há %d utilizadores validos)",n_utili);
        mvwprintw(write_querie_win,i+2,40,"Input: 946                  (Id de um voo // Há %d voos validos)",n_voos);
        mvwprintw(write_querie_win,i+4,40,"Input: Book156              (Id de uma reserva // Há %d reservas validas)",n_res);

        wattron(write_querie_win, A_BOLD);
        wattron (write_querie_win,COLOR_PAIR(4));
        mvwprintw(write_querie_win,i,47,  "LGarcia1208");
        mvwprintw(write_querie_win,i+2,47,"946");
        mvwprintw(write_querie_win,i+4,47,"Book156");
        wattroff (write_querie_win,COLOR_PAIR(4));
        mvwprintw(write_querie_win,i+6,40,"Os ids inferiores ao valor maximo podem nao existir e podem existir ids superiores ao valor de validos!");
        wattroff(write_querie_win, A_BOLD);
        break;
    case 1: 
        mvwprintw(write_querie_win,i,40,"Input: JéssiTavares910");
        mvwprintw(write_querie_win,i+2,40,"Input: JéssiTavares910 flights");
        mvwprintw(write_querie_win,i+4,40,"Input: JéssiTavares910 reservations");
        wattron(write_querie_win, A_BOLD);
        wattron (write_querie_win,COLOR_PAIR(4));
        mvwprintw(write_querie_win,i,47,"JéssiTavares910");
        mvwprintw(write_querie_win,i+2,47,"JéssiTavares910 flights");
        mvwprintw(write_querie_win,i+4,47,"JéssiTavares910 reservations");
        wattroff (write_querie_win,COLOR_PAIR(4));
        wattroff(write_querie_win, A_BOLD);
        break;
    case 2:
        mvwprintw(write_querie_win,i,40,"Input: HTL1003");
        wattron(write_querie_win, A_BOLD);
        wattron (write_querie_win,COLOR_PAIR(4));
        mvwprintw(write_querie_win,i,47,"HTL1003");
        wattroff (write_querie_win,COLOR_PAIR(4));
        mvwprintw(write_querie_win,i+3,40,"Nem todos os ids possiveis (0000 a 9999) de hoteis existem!");
        wattroff(write_querie_win, A_BOLD);
        break;
    case 3:
        mvwprintw(write_querie_win,i,40,"Input: HTL1003");
        wattron(write_querie_win, A_BOLD);
        wattron (write_querie_win,COLOR_PAIR(4));
        mvwprintw(write_querie_win,i,47,"HTL1003");
        wattroff (write_querie_win,COLOR_PAIR(4));
        mvwprintw(write_querie_win,i+3,40,"Nem todos os ids possiveis (0000 a 9999) de hoteis existem!");
        wattroff(write_querie_win, A_BOLD);
        break;
    case 4:
        mvwprintw(write_querie_win,i,40,"Input: LIS \"2023/01/01 00:00:00\" \"2023/05/31 23:59:59\" ");
        wattron(write_querie_win, A_BOLD);
        wattron (write_querie_win,COLOR_PAIR(4));
        mvwprintw(write_querie_win,i,47,"LIS \"2023/01/01 00:00:00\" \"2023/05/31 23:59:59\" ");
        wattroff (write_querie_win,COLOR_PAIR(4));
        wattroff(write_querie_win, A_BOLD);
        break;
    case 5:
        mvwprintw(write_querie_win,i,40,"Input: 2021 10");
        wattron(write_querie_win, A_BOLD);
        wattron (write_querie_win,COLOR_PAIR(4));
        mvwprintw(write_querie_win,i,47,"2021 10");
        wattroff(write_querie_win, A_BOLD);
        wattroff (write_querie_win,COLOR_PAIR(4));
        break;
    case 6:
        mvwprintw(write_querie_win,i-1,40,"Input: 10");
        wattron(write_querie_win, A_BOLD);
        wattron (write_querie_win,COLOR_PAIR(4));
        mvwprintw(write_querie_win,i-1,47,"10");
        wattroff(write_querie_win, A_BOLD);
        wattroff (write_querie_win,COLOR_PAIR(4));
        break;
    case 7:
        mvwprintw(write_querie_win,i-1,40,"Input: HTL1001 2023/05/01 2023/06/01");
        wattron(write_querie_win, A_BOLD);
        wattron (write_querie_win,COLOR_PAIR(4));
        mvwprintw(write_querie_win,i-1,47,"HTL1001 2023/05/01 2023/06/01");
        wattroff(write_querie_win, A_BOLD);
        wattroff (write_querie_win,COLOR_PAIR(4));
        break;
    case 8:
        mvwprintw(write_querie_win,i-1,40,"Input: Julia");
        wattron(write_querie_win, A_BOLD);
        wattron (write_querie_win,COLOR_PAIR(4));
        mvwprintw(write_querie_win,i-1,47,"Julia");
        wattroff(write_querie_win, A_BOLD);
        wattroff (write_querie_win,COLOR_PAIR(4));
        break;
    case 9:
        mvwprintw(write_querie_win,i-1,40,"Input: 2023 ");
        wattron(write_querie_win, A_BOLD);
        wattron (write_querie_win,COLOR_PAIR(4));
        mvwprintw(write_querie_win,i-1,47,"2023");
        wattroff(write_querie_win, A_BOLD);
        mvwprintw(write_querie_win,i+1,40,"Insira \"v\" para um resumo por ano");
        wattroff (write_querie_win,COLOR_PAIR(4));
        break;
    }
        

    wrefresh(write_querie_win);

    char *input = malloc(sizeof(char) * 100);
    int repeat = 0;
    int break_cycle = 0;

    do
    {
        char buffer[100];
        int buffer_index = 0;
        if(repeat == 0)
        {
            wattron(write_querie_win, A_BOLD);
            wattron (write_querie_win,COLOR_PAIR(4));
            mvwprintw(write_querie_win, 18, 40, "Insira o seu Input!\n");
            wattroff(write_querie_win, A_BOLD);
            wattroff (write_querie_win,COLOR_PAIR(4));
        }
        else
        {
            wclear(write_querie_win);
            wattron(write_querie_win, A_BOLD);
            wattron (write_querie_win,COLOR_PAIR(3));
            mvwprintw(write_querie_win, 18, 40, "Insira um input válido!\n");
            wattroff(write_querie_win, A_BOLD);
            wattroff (write_querie_win,COLOR_PAIR(3));
            mvwprintw(write_querie_win,i-2,40,"Estes são os formatos do input para esta querie!");
            switch (querie)
             {
               case 0:
                   mvwprintw(write_querie_win,i,40,  "Input: LGarcia1208          (Username // Há %d utilizadores validos)",n_utili);
                   mvwprintw(write_querie_win,i+2,40,"Input: 946                  (Id de um voo // Há %d voos validos)",n_voos);
                   mvwprintw(write_querie_win,i+4,40,"Input: Book156              (Id de uma reserva // Há %d reservas validas)",n_res);
           
                   wattron(write_querie_win, A_BOLD);
                   wattron (write_querie_win,COLOR_PAIR(3));
                   mvwprintw(write_querie_win,i,47,  "LGarcia1208");
                   mvwprintw(write_querie_win,i+2,47,"946");
                   mvwprintw(write_querie_win,i+4,47,"Book156");
                   wattroff (write_querie_win,COLOR_PAIR(3));
                   mvwprintw(write_querie_win,i+6,40,"Os ids inferiores ao valor maximo podem nao existir e podem existir ids superiores ao valor de validos!");
                   wattroff(write_querie_win, A_BOLD);
                   break;
               case 1: 
                   mvwprintw(write_querie_win,i,40,"Input: JéssiTavares910");
                   mvwprintw(write_querie_win,i+2,40,"Input: JéssiTavares910 flights");
                   mvwprintw(write_querie_win,i+4,40,"Input: JéssiTavares910 reservations");
                   wattron(write_querie_win, A_BOLD);
                   wattron (write_querie_win,COLOR_PAIR(3));
                   mvwprintw(write_querie_win,i,47,"JéssiTavares910");
                   mvwprintw(write_querie_win,i+2,47,"JéssiTavares910 flights");
                   mvwprintw(write_querie_win,i+4,47,"JéssiTavares910 reservations");
                   wattroff (write_querie_win,COLOR_PAIR(3));
                   wattroff(write_querie_win, A_BOLD);
                   break;
               case 2:
                   mvwprintw(write_querie_win,i,40,"Input: HTL1003");
                   wattron(write_querie_win, A_BOLD);
                   wattron (write_querie_win,COLOR_PAIR(3));
                   mvwprintw(write_querie_win,i,47,"HTL1003");
                   wattroff (write_querie_win,COLOR_PAIR(3));
                   mvwprintw(write_querie_win,i+3,40,"Nem todos os ids possiveis (0000 a 9999) de hoteis existem!");
                   wattroff(write_querie_win, A_BOLD);
                   break;
               case 3:
                   mvwprintw(write_querie_win,i,40,"Input: HTL1003");
                   wattron(write_querie_win, A_BOLD);
                   wattron (write_querie_win,COLOR_PAIR(3));
                   mvwprintw(write_querie_win,i,47,"HTL1003");
                   wattroff (write_querie_win,COLOR_PAIR(3));
                   mvwprintw(write_querie_win,i+3,40,"Nem todos os ids possiveis (0000 a 9999) de hoteis existem!");
                   wattroff(write_querie_win, A_BOLD);
                   break;
               case 4:
                   mvwprintw(write_querie_win,i,40,"Input: LIS \"2023/01/01 00:00:00\" \"2023/05/31 23:59:59\" ");
                   wattron(write_querie_win, A_BOLD);
                   wattron (write_querie_win,COLOR_PAIR(3));
                   mvwprintw(write_querie_win,i,47,"LIS \"2023/01/01 00:00:00\" \"2023/05/31 23:59:59\" ");
                   wattroff (write_querie_win,COLOR_PAIR(3));
                   mvwprintw(write_querie_win,i+3,40,"Todas as datas impossiveis serao descartadas (incluindo um 29 de fevereiro inexistente, data final menor q inicial etc.");
                   mvwprintw(write_querie_win,i+4,40,"Inserir dias menores que 10 como 2 algarismos é essencial (01;02;...).");
                   wattroff(write_querie_win, A_BOLD);
                   break;
               case 5:
                   mvwprintw(write_querie_win,i,40,"Input: 2021 10");
                   wattron(write_querie_win, A_BOLD);
                   wattron (write_querie_win,COLOR_PAIR(3));
                   mvwprintw(write_querie_win,i,47,"2021 10");
                   wattroff(write_querie_win, A_BOLD);
                   wattroff (write_querie_win,COLOR_PAIR(3));
                   break;
               case 6:
                   mvwprintw(write_querie_win,i-1,40,"Input: 10");
                   wattron(write_querie_win, A_BOLD);
                   wattron (write_querie_win,COLOR_PAIR(3));
                   mvwprintw(write_querie_win,i-1,47,"10");
                   wattroff(write_querie_win, A_BOLD);
                   wattroff (write_querie_win,COLOR_PAIR(3));
                   break;
               case 7:
                   mvwprintw(write_querie_win,i-1,40,"Input: HTL1001 2023/05/01 2023/06/01");
                   wattron(write_querie_win, A_BOLD);
                   wattron (write_querie_win,COLOR_PAIR(3));
                   mvwprintw(write_querie_win,i-1,47,"HTL1001 2023/05/01 2023/06/01");
                   wattroff(write_querie_win, A_BOLD);
                   wattroff (write_querie_win,COLOR_PAIR(3));
                   break;
               case 8:
                   mvwprintw(write_querie_win,i-1,40,"Input: Julia");
                   wattron(write_querie_win, A_BOLD);
                   wattron (write_querie_win,COLOR_PAIR(3));
                   mvwprintw(write_querie_win,i-1,47,"Julia");
                   wattroff(write_querie_win, A_BOLD);
                   wattroff (write_querie_win,COLOR_PAIR(3));
                   break;
               case 9:
                   mvwprintw(write_querie_win,i-1,40,"Input: 2023 ");
                   wattron(write_querie_win, A_BOLD);
                   wattron (write_querie_win,COLOR_PAIR(3));
                   mvwprintw(write_querie_win,i-1,47,"2023");
                   wattroff(write_querie_win, A_BOLD);
                   mvwprintw(write_querie_win,i+1,40,"Insira \"v\" para um resumo por ano");
                   wattroff (write_querie_win,COLOR_PAIR(3));
                   break;
               }
        }
        
        wrefresh(write_querie_win);

        int ch;
        while ((ch = wgetch(write_querie_win)) != '\n')  // Loop até pressionar Enter
        {
            if((ch == KEY_BACKSPACE || ch == 127) && buffer_index >= 1)
            {
                buffer[--buffer_index] = '\0';
                mvwprintw(write_querie_win, 20, 40, "%s ", buffer);
                wrefresh(write_querie_win);
            }
            else
            {
                buffer[buffer_index++] = ch;
                buffer[buffer_index] = '\0';  // Garante que o buffer seja uma string válida
                mvwprintw(write_querie_win, 20, 40, "%s", buffer);
                wrefresh(write_querie_win);
            }
            
        }
        strcpy(input,buffer);
    
       int check = 0;
       switch (querie)
       {
          case 0:  //Querie 1
          break;

          case 1: // Query 2
          check = 0;
          int u = 0;
        
          if(!isalpha(input[u])) {check = 1; break;}

          while (!isspace(input[u]) && input[u] != '\0') 
          {
              u++;
          }

          if(input[u] == '\0')
          {
            break;
          }
      
          while (isspace(input[u])) 
          {
              u++;
          }
      
          if (strcmp(input + u, "flights\0") != 0 && strcmp(input + u, "reservations\0") != 0) 
          {
              check = 1;
          }
          break;


           case 2:  //Querie 3
           check = 0;
           if(input[0] != 'H') {check = 1;}
           else if(input[1] != 'T') {check = 1;}
           else if(input[2] != 'L') {check = 1;}
           else 
           {
               for(int j = 3; input[j+1] != '\0'; j++)
               {
                  if(!isdigit(input[j]))
                  {
                     check = 1;
                     break;
                  }
               }
           }
          break;

          case 3:   //Querie 4
           check = 0;
           if(input[0] != 'H') {check = 1;}
           else if(input[1] != 'T') {check = 1;}
           else if(input[2] != 'L') {check = 1;}
           else 
           {
               for(int j = 3; input[j+1] != '\0'; j++)
               {
                  if(!isdigit(input[j]))
                  {
                     check = 1;
                     break;
                  }
               }
           }
          break;

          case 4:   //Querie 5

         if(strlen(input) != 47) {check = 1; break;}

          int q5 = 0;
          while (q5 < 3)
          {
            if(!isalpha(input[q5]))
            {
                check = 1;
                break;
            }
            q5++;
          }
    
          if(!isspace(input[q5])) {check = 1; break;}
            

          
          int data[6];   
          int data2[6]; 
          
          data [0] = atoi (input + 5);
          if (data[0] < 2010 || data[0] > 2023) {check = 1;break;}
          data [1] = atoi (input + 10);
          if (data[1] < 1 || data[1] > 12) {check = 1;break;}
          data [2] = atoi (input + 13);
          if (data[2] < 1 || data[2] > 31 || (data[2] > 29 && data[1] == 2) || (data[2] > 30 && data[1] == 4) || (data[2] > 30 && data[1] == 6)
                                          || (data[2] > 30 && data[1] == 9) || (data[2] > 30 && data[1] == 11) ) {check = 1;break;}
          data [3] = atoi (input + 16);
         if (data[3] < 0 || data[3] > 23) {check = 1;break;}
          data [4] = atoi (input + 19);
          if (data [4] < 0 || data[4] > 59) {check = 1;break;}
          data [5] = atoi (input + 22);
          if (data[5] < 0 || data[5] > 59) {check = 1;break;}

          data2 [0] = atoi (input + 27);
          if (data2[0] < 2010 || data2[0] > 2023) {check = 1;break;}
          data2 [1] = atoi (input + 32);
           if (data2[1] < 1 || data2[1] > 12) {check = 1;break;}
          data2 [2] = atoi (input + 35);
          if (data2[2] < 1 || data2[2] > 31 || (data2[2] > 29 && data2[1] == 2) || (data2[2] > 30 && data2[1] == 4) || (data2[2] > 30 && data2[1] == 6)
                                            || (data2[2] > 30 && data2[1] == 9) || (data2[2] > 30 && data2[1] == 11) ) {check = 1;break;}
          data2 [3] = atoi (input + 38);
          if (data2[3] < 0 || data2[3] > 23) {check = 1;break;}
          data2 [4] = atoi (input + 41);
          if (data2 [4] < 0 || data2[4] > 59) {check = 1;break;}
          data2 [5] = atoi (input + 44);
          if (data2[5] < 0 || data2[5] > 59) {check = 1;break;}
          
          if (data[2] == 29 && (!  ((data[0] %4 == 0 && data[0] %100 != 0) || data[0] % 400 == 0)   ) ) // se for 29 de janeiro e negacao de ano bissesto
            {
             check = 1;
             break;
            }
           if (data2[2] == 29 && (!  ((data2[0] %4 == 0 && data2[0] %100 != 0) || data2[0] % 400 == 0)   ) ) // se for 29 de janeiro e negacao de ano bissesto
            {
             check = 1;
             break;
            }


          if(compara_data_tempo(data,data2) == 0)
          {
            check = 1;
          }

          break;


          case 5:  //Querie 6

          check = 0;
          int j = 4;
          int n = atoi (input);
          if (n == 0 || n > 2023 || n < 1900) {check = 1; break;}
          if(!isspace(input[j])) {check = 1; break;}
          j++;
          n = atoi(input + j);
          if(n == 0) {check = 1;}
          break;

          case 6:   //Querie 7
          check = 0;
          if(atoi(input) == 0) {check = 1;}
          break;
          
      case 7: // Querie 8
          check = 0;
          if (input[0] != 'H') {
              check = 1;
              break;
          }
          if (input[1] != 'T') {
              check = 1;
              break;
          }
          if (input[2] != 'L') {
              check = 1;
              break;
          }
      
          if (isspace(input[3])) {
              check = 1;
              break;
          }
      
          int j8;
          for (j8 = 3; input[j8] != '\0' && (!isspace(input[j8])); j8++) {
              if (!isdigit(input[j8])) {
                  check = 1;
                  break;
              }
          }
      
          // 2021/01/02 2022/04/05
      
          if (strlen(input + j8 + 1) != 21) {
              check = 1;
              break;
          }
      
          if (!isspace(input[j8])) {
              check = 1;
              break;
          }
          j8++;
      
          char copia8_1[100];
          int data_8_1[3];
          int itera_data;
          for (itera_data = 0; !isspace(input[j8]) && input[j8] != '\0'; itera_data++) {
              copia8_1[itera_data] = input[j8];
              j8++;
          }
      
          if (check_data(copia8_1, data_8_1) == 0) {
              check = 1;
              break;
          }
      
          if (!isspace(input[j8])) {
              check = 1;
              break;
          }
          j8++;
      
          char copia_8_2[100];
          int data_8_2[3];
          int itera_data2;
          for (itera_data2 = 0; input[itera_data2] != '\0'; itera_data2++) {
              copia_8_2[itera_data2] = input[j8];
              j8++;
          }
      
          if (check_data(copia_8_2, data_8_2) == 0) {
              check = 1;
              break;
          }
      
           if(data_8_1[0] > data_8_2[0] || (data_8_1[0] == data_8_2[0] && (data_8_1[1] > data_8_2[1])) 
           || (data_8_1[0] == data_8_2[0] && data_8_1[1] == data_8_2[1] && (data_8_1[2] > data_8_2[2])))
           {
            check = 1;
           }

          break;

          case 8: //Querie 9
          for(int j = 0; input[j] != '\0'; j++)
          {
            if(!isalpha(input[j]))
            {
                check = 1;
                break;
            }
          }
          break;

          case 9: //Querie 10
            if (input [0] == 'v') break;
            if (strlen (input) != 0 )
            {       
                if (atoi (input) > 2023 || atoi (input) < 1980) 
                {
                    check = 1;
                    break;
                }
                if ((strlen (input) > 4 && atoi (input + 5) > 12) ||  (strlen (input) > 4 && atoi (input + 5) < 1))
                {
                    
                        check = 1;
                        break;
                }
                
            }

          break;  
       }    
       repeat = 1;
       if(check == 0) break_cycle = 1;
    } while (break_cycle != 1);

    endwin();
    return input;
}



void processar(char *caminho,WINDOW *caminho_win,int y_max,int x_max)
{
    setlocale(LC_COLLATE, "en_US.UTF-8");
    wclear (caminho_win);
    mvwprintw(caminho_win, y_max/2 - 2, x_max/3 - 15, "EM PROCESSAMENTO\n");
    mvwprintw(caminho_win, y_max/2, x_max/3 - 15, "|              |\n");
    wrefresh(caminho_win);

    

    char pasta [100];

    void *apontador_verificar_hotel_passageiros = NULL;
    GHashTable *verificar_hotel = g_hash_table_new(g_str_hash,g_str_equal);
    apontador_verificar_hotel_passageiros = verificar_hotel;

    void *apontador_verificar_utilizador = NULL;
    GHashTable *verificar_utilizador = g_hash_table_new(g_str_hash,g_str_equal);
    apontador_verificar_utilizador = verificar_utilizador;

    int verificar_inicial_utilizador [HASH_ASCII];
    for (int i = 0; i < HASH_ASCII; i++) verificar_inicial_utilizador [i] = 1;

    void* anos_registos_utilizadores = criar_lista_anos_registos();   //anos desde 2010 de registos por ano
    void* anos_registos_voos = criar_lista_registos_voos();
    aeroporto_arvore* arvore_todos_voos = NULL;
    void* anos_registos_reservas  = criar_lista_registos_reservas();


    char** total_dados_utilizadores = malloc(TAMANHO*sizeof(char*));
    char** total_dados_reservas = malloc (TAMANHO*sizeof(char*));          //matrizes para parsing generico dos ficheiros
    char** total_dados_voos = malloc(TAMANHO*sizeof(char*));
    char** total_dados_passageiros = malloc(TAMANHO*sizeof(char*));


    void* hash_todos_utilizadores = NULL;   //hashing por id de utilizador para todos os utilizadores (com arvores dos voos e reservas dele)
    GHashTable* tabela_todos_utilizadores = NULL;

    void* dados_voos = NULL;                  //hashing por aeroporto (com arvore de voos partidos dele)
    GHashTable* tabela_voos = NULL;

    void* erros_utilizadores_void = NULL;       //hashing para identificar erros de utilizadores facilmente
    GHashTable* tabela_erros = NULL;

   
    btree_utilizadores** apontador_arvore_por_iniciais = calloc (sizeof (btree_utilizadores*),HASH_ASCII);   //hashing de todos os utilizadores em arvores por inicial

    reservas_arvore **dados_reservas = calloc(sizeof(reservas_arvore *),TAMANHO_HASH);     //hashing de reservas atraves dos seus ids de hotel (com arvore de reservas)

  
    int* erros_voos = malloc (sizeof(int) * HASH_VOOS_ERRADOS);
    // hash para verificar se voo é errado 


    int n_voos = 0;
    int n_voos_validos = 0;
    int numero_utilizadores = 0;
    int n_passageiros = 0;
    int n_reservas = 0;
    int maior_id_reserva = 0;


    sprintf(pasta, "%s/users.csv",caminho);             
    FILE* parse_u = fopen(pasta,"r");

    sprintf(pasta, "%s/reservations.csv",caminho);
    FILE* parse_r = fopen(pasta,"r");

    sprintf(pasta, "%s/flights.csv",caminho);
    FILE* parse_v = fopen(pasta,"r");

    sprintf(pasta, "%s/passengers.csv",caminho);
    FILE* parse_p = fopen(pasta,"r");

    if(parse_r == NULL || parse_u == NULL || parse_v == NULL || parse_p == NULL)
    {
        mvwprintw(caminho_win,y_max/2, x_max/3 - 15,"Não foi possível abrir o ficheiro \n");
        return;
    }

    mvwprintw(caminho_win, y_max/2 - 2, x_max/3 - 15, "|--            |\n");
    mvwprintw(caminho_win, y_max/2, x_max/3 - 15, "Espaço alocado e ficheiros abertos \n");
    wattron (caminho_win,COLOR_PAIR (4));
    mvwprintw(caminho_win, y_max/2, x_max/3 + 43, "X\n");
    wattroff (caminho_win,COLOR_PAIR (4));
    wrefresh(caminho_win);
    
    char* ficheiro_utilizador_erro = "Resultados/users_errors.csv";
    char* header_utilizadores = "id;name;email;phone_number;birth_date;sex;passport;country_code;address;account_creation;pay_method;account_status";
    erros_utilizadores_void = parser_generico_utilizadores(parse_u,&total_dados_utilizadores,ficheiro_utilizador_erro,tabela_erros,&erros_voos,header_utilizadores,&numero_utilizadores);
    
    char* ficheiro_reserva_erro = "Resultados/reservations_errors.csv";
    char* header_reservas = "id;user_id;hotel_id;hotel_name;hotel_stars;city_tax;address;begin_date;end_date;price_per_night;includes_breakfast;room_details;rating;comment";
    parser_generico_reservas(parse_r, &total_dados_reservas, ficheiro_reserva_erro, erros_utilizadores_void
                            ,header_reservas, &n_reservas,anos_registos_reservas,apontador_verificar_hotel_passageiros,&maior_id_reserva,1);

    reserva **hash_reservas = calloc(sizeof(reserva *),maior_id_reserva); 

    char* ficheiro_voos_erro = "Resultados/flights_errors.csv";
    char* header_voos = "id;airline;plane_model;total_seats;origin;destination;schedule_departure_date;schedule_arrival_date;real_departure_date;real_arrival_date;pilot;copilot;notes";
    parser_generico_voos(parse_v, &total_dados_voos, ficheiro_voos_erro, &erros_voos, header_voos,&n_voos, &n_voos_validos);

    voo **hash_voos = calloc(sizeof(voo *),n_voos);                              
    // hashing de todos os voos pelo seu id individual
    
    mvwprintw(caminho_win, y_max/2 - 2, x_max/3 - 15, "|----          |\n");
    mvwprintw(caminho_win, y_max/2 + 1, x_max/3 - 15, "Ficheiros de voos,utilizadores e reservas filtrados \n");
    wattron (caminho_win,COLOR_PAIR (4));
    mvwprintw(caminho_win, y_max/2 + 1, x_max/3 + 43, "X\n");
    wattroff (caminho_win,COLOR_PAIR (4));
    wrefresh(caminho_win);
    
    int passageiros_voo[n_voos];
    for (int i = 0; i < n_voos;i++) passageiros_voo [i] = 0;

      
    hash_todos_utilizadores = parser_utilizadores(total_dados_utilizadores, tabela_todos_utilizadores, apontador_arvore_por_iniciais, numero_utilizadores,anos_registos_utilizadores,verificar_inicial_utilizador,apontador_verificar_utilizador,1); // parsings nao genericos para Btrees etc.
    free_matriz(total_dados_utilizadores, numero_utilizadores);


   
   char *passageiros_erro = "Resultados/passengers_errors.csv";
    char *header_passageiros = "flight_id;user_id";
    parser_generico_passageiros(parse_p, &total_dados_passageiros, passageiros_erro, erros_utilizadores_void, passageiros_voo, &erros_voos, header_passageiros, hash_todos_utilizadores, &n_passageiros);
    free_erros(erros_utilizadores_void);
    free(erros_voos);
    
    mvwprintw(caminho_win, y_max/2 - 2, x_max/3 - 15, "|------        |\n");
    mvwprintw(caminho_win, y_max/2 + 2, x_max/3 - 15, "Parsing de utilizadores e filtragem de passageiros \n");
    wattron (caminho_win,COLOR_PAIR (4));
    mvwprintw(caminho_win, y_max/2 + 2, x_max/3 + 43, "X\n");
    wattroff (caminho_win,COLOR_PAIR (4));
    wrefresh(caminho_win);

    dados_reservas = parser_reserva(total_dados_reservas,dados_reservas,&hash_reservas,hash_todos_utilizadores,apontador_verificar_hotel_passageiros,n_reservas,anos_registos_reservas,1);
    free_matriz (total_dados_reservas,n_reservas);

    mvwprintw(caminho_win, y_max/2 - 2, x_max/3 - 15, "|----------    |\n");
    mvwprintw(caminho_win, y_max/2 +3, x_max/3 - 15, "Parsing de reservas \n");
    wattron (caminho_win,COLOR_PAIR (4));
    mvwprintw(caminho_win, y_max/2 + 3, x_max/3 + 43, "X\n");
    wattroff (caminho_win,COLOR_PAIR (4));
    wrefresh(caminho_win);

    dados_voos = parser_voos(total_dados_voos, tabela_voos, passageiros_voo, &hash_voos, n_voos_validos,anos_registos_voos,&arvore_todos_voos);
    free_matriz(total_dados_voos, n_voos_validos);

    mvwprintw(caminho_win, y_max/2 - 2, x_max/3 - 15, "|------------  |\n");
    mvwprintw(caminho_win, y_max/2+4, x_max/3 - 15, "Parsing de voos \n");
    wattron (caminho_win,COLOR_PAIR (4));
    mvwprintw(caminho_win, y_max/2 + 4, x_max/3 + 43, "X\n");
    wattroff (caminho_win,COLOR_PAIR (4));
    wrefresh(caminho_win);
    
    parser_passageiros (total_dados_passageiros,hash_todos_utilizadores,hash_voos,n_passageiros,apontador_verificar_hotel_passageiros,1);
            
    mvwprintw(caminho_win, y_max/2 - 2, x_max/3 - 15, "|--------------|\n");
    mvwprintw(caminho_win, y_max/2 +5, x_max/3 - 15, "Parsing de passageiros \n");
    wattron (caminho_win,COLOR_PAIR (4));
    mvwprintw(caminho_win, y_max/2 + 5, x_max/3 + 43, "X\n");
    wattroff (caminho_win,COLOR_PAIR (4));
    wrefresh(caminho_win);


    int sair_definitivo = 0;
    do  /////////////////////////////////////////////////////////////CICLO DE REALIAZACAO DE QUERIES//////////////////////////////
    {
    int querie = select_querie();
    int tipo_print = escolha_output(querie);
    while (tipo_print == -1)
    {
        querie = select_querie() ;          
        tipo_print = escolha_output(querie);  // 0 para normal e 1 para F
    }

    char* input = write_querie(querie,n_reservas,n_voos_validos,numero_utilizadores);
    char* resposta = NULL;

    switch (querie + 1)
    {
        case (1):
        {
        resposta = calloc (sizeof(char),1000);
        int select = identificar_id (input);
        
        if (select == RESERVA)
        {
            querie1_R_interativa  (hash_reservas,input,resposta,tipo_print); //comecar com id-1
        }
        else if (select == VOO)
        {
            querie1_V_interativa (hash_voos,passageiros_voo,input,resposta,tipo_print);
        }
        else if (select == UTILIZADORES)
        {
            querie1_U_interativa (hash_todos_utilizadores,input,resposta,tipo_print);
        }
        break;
        }
        case (2):
        {        
            resposta = calloc (sizeof(char),1000000);
            char* temp = strdup(input);
            char* token;
            char* id;
            char* save_tok;
            token = strtok_r(temp," ",&save_tok);
            id = strdup (token);
    
             token = strtok_r (NULL,"\n",&save_tok);
             if (token == NULL) 
             {
                 querie_2_Total_interativo (id,hash_todos_utilizadores,resposta,tipo_print);
             }                  
             else if (strcmp ("flights",token) == 0) 
             {
                 querie_2_Voos_interativo (id,hash_todos_utilizadores,resposta,tipo_print);
             }
             else if (strcmp ("reservations",token) == 0) 
             {
                 querie_2_Reservas_interativo (id,hash_todos_utilizadores,resposta,tipo_print);
             }
             free(id);
             free(temp);
             break;
        }
        case (3):
        {
             resposta = calloc (sizeof(char), 1000);
             int hash = atoi(input + 3);        //fazemos a hash para pesquisar apenas na arvore necessaria

             if (dados_reservas[hash] == NULL)
             {  
                sprintf (resposta,"Nao existem registos desse hotel! (HTL%d)\n",hash);
             }
             else 
             {
                if (tipo_print == 1)  
             {
                 sprintf(resposta + strlen (resposta),"--- 1 ---\n");
                 sprintf(resposta + strlen (resposta),"rating: ");        //imprimimos o header se o F estiver incluido na chamada da querie
             }

              double res;
              double Rating_total = 0.000;
              double Numero_nodos = 0.000;
      
              Rating_total = Somar_ratings (dados_reservas[hash],&Numero_nodos); //a funcao somar ratings retorna o rating total 
                                                                  //e guarda o numero de nodos no endereço passado como argumento
          
              if (Rating_total == 0) res = 0;
              else  res = (Rating_total/Numero_nodos); // media de ratings
      
                sprintf(resposta + strlen (resposta),"%.3f\n",res);
             }
            break;
        }
        case (4):
        {
            int hash = atoi (input + 3);
            if (dados_reservas[hash] == NULL) 
            {
                resposta = calloc (sizeof(char), 100);
                strcpy (resposta,"Esse hotel nao existe.\n");
            }
            else
            {
            int contador = 0;
            resposta = calloc (sizeof(char), 100000000); //outputs 4000+ linhas
            querie4_interativa (dados_reservas[hash],resposta,&contador,tipo_print);
            }
            break;
        }
        case (5):
        {
             //FUNCIONA MAS TESTE DE INPUT DA QUERIE 5 NAO FUNCIONA
             char* token;
             char* save_token;
             token = strtok_r(input," ",&save_token);

            if (!g_hash_table_contains (dados_voos,token)) 
            {
                resposta = calloc (sizeof(char), 100); //outputs 1500+ linhas
                sprintf(resposta,"Nao existem dados relativos a esse aeroporto! \n");
            }
            else 
            {
             resposta = calloc (sizeof(char), 100000000); //outputs 1500+ linhas
             aeroporto_arvore* arvore_de_voos = g_hash_table_lookup (dados_voos,token);

             const char* delimiter = "\"";
             token = strtok_r(NULL,delimiter,&save_token);
             int data_inicio[6];
             converter_schedule(token,data_inicio);     //convertemos as datas de strings para inteiros 
      
             token = strtok_r(NULL,delimiter,&save_token);
             token = strtok_r(NULL,delimiter,&save_token);
             int data_fim[6];
             converter_schedule(token,data_fim);
             
             int contador = 1;


             querie5_interativa (arvore_de_voos,resposta,data_inicio,data_fim,&contador,tipo_print);
            
            }
            break;
        }
        case (6):
        {
            resposta = calloc (sizeof(char), 3000); // outputs pequenos (lista de aeroportos com numeros)
            char* token;
            char* save_token;
            token = strtok_r(input," ",&save_token);
    
            int ano = atoi (token);
            token = strtok_r(NULL," ",&save_token);     //guardamos o ano e o N numero de paises a considerar como inteiros
            int N = atoi (token);

            querie6_interativa (dados_voos,ano,N,resposta,tipo_print);
            break;
             
        }
        case (7):
        {
            resposta = calloc (sizeof(char), 1000); //3 letra por aeroporto mais a mediana (30 aeroportos maximo)

            int numero_aeroportos = atoi (input);
            
            querie7_interativa (dados_voos,numero_aeroportos,resposta,n_voos,tipo_print);
            
            break;
        }
        case (8):
        {
            resposta = calloc (sizeof(char), 3000);
        
            char* token;
            char* save_token;
            token = strtok_r(input," ",&save_token);
            
            int hash = atoi(token + 3);               //calculamos a hash para selecionar apenas a arvore necessaria
            
            token = strtok_r(NULL," ",&save_token);
            int data_inicio[3];
            converter_data(token,data_inicio);          //convertemos as datas em inteiros
            token = strtok_r(NULL," ",&save_token);
            int data_fim[3];
            converter_data(token,data_fim);
            
            if (tipo_print == 1)  
            {
                sprintf(resposta + strlen(resposta),"--- 1 ---\n"); //imprimimos o cabecalho em caso de chamada "8F"
                sprintf(resposta + strlen(resposta),"revenue: ");
            }
            querie8_interativa(dados_reservas[hash],resposta,data_inicio,data_fim);
            break;
        }
        case (9):
        {
            resposta = calloc (sizeof(char),10000000); 
              int contador = 1;
              int ASCII = 0; 
              ASCII =  (int)input[0];
              if (ASCII < 0) ASCII += 128;
              
              if (apontador_arvore_por_iniciais[ASCII] == NULL) 
              {
                strcpy (resposta,"Nao existe ninguem com esse prefixo! \n");
              }
              else 
              {
              querie_9_interativa (apontador_arvore_por_iniciais[ASCII],input,resposta,&contador,tipo_print); 
              }
              break;
        }
        case (10):
        {
            resposta = calloc (sizeof(char),10000);

            int data[2] = {0};
            if (input[0] != 'v')
            {
            char* token;
            char* save_token;
            token = strtok_r(input," ",&save_token);
    
            if (token != NULL)
            {
                data[0] = atoi (token);
                token = strtok_r(NULL," ",&save_token);
                if (token != NULL)
                {
                    data[1] = atoi (token);
                    token = strtok_r(NULL," ",&save_token);
                }
            }
            }
          
            querie10_interativa (data,total_dados_passageiros,n_passageiros,arvore_todos_voos,resposta,anos_registos_utilizadores,anos_registos_reservas,anos_registos_voos,n_voos,tipo_print);

            break;
        }
     }
    wclear(caminho_win);
    wrefresh (caminho_win);
    keypad (caminho_win,TRUE);
    int pagina = 1;
    int selected = 0;
    int key;
    int pagina_final = calcular_pagina_final (resposta,querie + 1,y_max);
    do
    {

        print_pagina (resposta,querie + 1,pagina,caminho_win,y_max,x_max);
        
        wattron(caminho_win, A_BOLD );
        mvwprintw(caminho_win,10,10 ,"Pressiona q para sair");
        wattroff(caminho_win, A_BOLD );
        mvwprintw(caminho_win, y_max - 2, x_max/2 - 9,"Pag: %d/%d",pagina,pagina_final);
        
        for(int i = 0; i < 6; i++)
        {
            if(i == selected)
            {
                wattron(caminho_win, A_BOLD);	//Caso esteja selecionado ativamos os atributos
				wattron(caminho_win, COLOR_PAIR(4));
            }
       
            switch(i)
            {
            case 0:
                mvwprintw(caminho_win, y_max - 3 ,x_max/2 - 23, "INICIO");
                break;
            case 1:
                mvwprintw(caminho_win, y_max - 3 ,x_max/2 - 15, "<==");
                break;
            case 2:
                mvwprintw(caminho_win, y_max - 3 ,x_max/2 - 10, "<--");
                break;
                
            case 3:
                 mvwprintw(caminho_win, y_max - 3, x_max/2 - 5,"-->");
                 break;
    
            case 4:
                mvwprintw(caminho_win, y_max - 3, x_max/2 ,"==>");
                break;
            case 5:
                 mvwprintw(caminho_win, y_max - 3, x_max/2 + 5,"FINAL");
                break;
            }
            wattroff(caminho_win, A_BOLD);	//Desliga o atributo quando trocamos de opção
			wattroff(caminho_win, COLOR_PAIR(4));
        }

        wrefresh(caminho_win);
        key = wgetch(caminho_win); //Input do utilizador

        if (key == '4' || key == KEY_LEFT) 
		{
            if (selected > 0)
            {
            selected--;
            }
        }
        else if (key == '6' || key == KEY_RIGHT) 
		{
            if (selected < 5)
            {
            selected++;
            }
        }
        
        if (key == '\n')
        {
            if (selected == 0) pagina = 1;
            if (selected == 1) pagina -= 10;
            if (selected == 2) pagina--;
            if (selected == 3) pagina++;
            if (selected == 4) pagina+= 10;
            if (selected == 5) pagina = pagina_final;
        }
        if (pagina < 1) pagina = 1;
        else if (pagina > pagina_final) pagina = pagina_final;


     } while (key != 'q');

        wclear (caminho_win);
        selected = 0;
        do
        {
            for(int i = 0; i < 2; i++)
            {
                if(i == selected)
                {
                    wattron(caminho_win, A_BOLD);	//Caso esteja selecionado ativamos os atributos
                    if (i == 0) wattron(caminho_win, COLOR_PAIR(4));
                    if (i == 1) wattron(caminho_win, COLOR_PAIR(3));
                }
           
                switch(i)
                {
                case 0:
                    mvwprintw(caminho_win, y_max/2 - 5, x_max/2 - 16,"Realizar outra Querie");
                    break;
                    
                case 1:
                    mvwprintw(caminho_win, y_max/2,x_max/2 - 8, "Sair");
                    break;
                }
                wattroff(caminho_win, COLOR_PAIR(4));
                wattroff(caminho_win, COLOR_PAIR(3));
                wattroff(caminho_win, A_BOLD);	//Desliga o atributo quando trocamos de opção
	    		
            }
    
            wrefresh(caminho_win);
            key = wgetch(caminho_win); //Input do utilizador
    
            if (key == '4' || key == KEY_UP) 
	    	{
                selected = 0;
            }
            else if (key == '6' || key == KEY_DOWN) 
	    	{
                selected = 1;
            }
            
    
         } while (key != '\n');


         if (selected == 1) sair_definitivo = 1;

    } while (sair_definitivo != 1);


    free_matriz(total_dados_passageiros, n_passageiros);

    free_g_hash_utilizadores(hash_todos_utilizadores);
    free_hash_utilizadores_inicial(apontador_arvore_por_iniciais);
    free_hash_reservas_essenciais(apontador_verificar_hotel_passageiros);

    free_registos_utilizadores(anos_registos_utilizadores);
    free_registos_reservas(anos_registos_reservas);
    free_registos_voos(anos_registos_voos);
    g_hash_table_destroy (apontador_verificar_utilizador);

    free_tree_reservas(dados_reservas, hash_reservas);
    free_aeroporto (arvore_todos_voos);
    free_voos(dados_voos, hash_voos);

    fclose(parse_r);
    fclose(parse_u);
    fclose(parse_v);
    fclose(parse_p);
    
    
    return;
}

void path()
{
    initscr();
    noecho();
    curs_set(0);
    start_color();
    int max_row, max_col;
    getmaxyx(stdscr, max_row, max_col);


    mousemask(0, NULL); 


    WINDOW *caminho_win = newwin(max_row, max_col, 0, 0);

    wclear(caminho_win);
    char* caminho;
    int repeat = 0;

    FILE* dataset = NULL;
    do
    { 
        char buffer[100];
        int buffer_index = 0;
        caminho = NULL;
        if(repeat == 0)
        {
            wattron (caminho_win,A_BOLD);
            mvwprintw(caminho_win, max_row/2 -2, max_col/3 - 15, "Qual o caminho para o Dataset?\n");
             wattroff (caminho_win,A_BOLD);
        }
        else
        {
            wclear(caminho_win);
            wattron (caminho_win,A_BOLD);
            mvwprintw(caminho_win, max_row/2 - 2, max_col/3 - 15, "Insira um caminho válido!\n");
            wattroff (caminho_win,A_BOLD);
        }
        
        wrefresh(caminho_win);

        int ch;
        while ((ch = wgetch(caminho_win)) != '\n' && ch != KEY_MOUSE)  // Loop até pressionar Enter
        { 
         
        if ((ch == KEY_BACKSPACE || ch == 127)) {
            if (buffer_index >= 1)
            {
            buffer[--buffer_index] = '\0';
            mvwprintw(caminho_win, max_row/2 , max_col/3 - 15, "%s ", buffer);
            wrefresh(caminho_win);
            }
        } else 
        {
            buffer[buffer_index++] = ch;
            buffer[buffer_index] = '\0';
            mvwprintw(caminho_win, max_row/2 , max_col/3 - 15, "%s", buffer);
            wrefresh(caminho_win);
        }
        }
        caminho = strdup(buffer);
        dataset = fopen(caminho,"r");
        repeat = 1;
    } while (dataset == NULL);

    return processar(caminho,caminho_win,max_row,max_col);
}


int menu()
{
    initscr();
    noecho();
    curs_set(0);
    start_color();
    flushinp(); //Limpa buffer teclado

    init_color(AMARELO, 900, 900, 0);
    init_color(VERDE, 0, 900, 400);
    init_color(AZUL, 300, 400, 900);
    init_color(BRANCO, 400, 400, 400);
    init_pair(4, AMARELO, BLACK);
	init_pair(1, VERDE, BLACK);  // amarelo em terminal e magenta em vscode 
    init_pair(2, AZUL, BLACK); 
    init_pair(3, COLOR_RED, BLACK);
    init_pair(10, BRANCO, BLACK);
    int max_row, max_col;
    getmaxyx(stdscr, max_row, max_col);

    WINDOW *menu = newwin(max_row, max_col, 0, 0);

    if (max_row < 35 || max_col < 120) 
    {
        wattron (menu,COLOR_PAIR (1));
        mvwprintw(menu, max_row/2,max_col/2 - 10,"Por favor abra numa janela maior!\n");
        mvwprintw(menu, max_row/2 + 1,max_col/2 - 10,"O programa vai encerrar.\n");
        wattroff (menu,COLOR_PAIR (1));
        wrefresh(menu);
        napms (4000);
        endwin();
        return 1;
    }

    wclear(menu);

     wattron(menu, A_BOLD);
      wattron(menu, COLOR_PAIR (4));
    mvwprintw(menu, max_row/2 - 7,max_col/5,"  __  __           _         _____       _                 _   _            \n");
    mvwprintw(menu, max_row/2 - 6,max_col/5," |  \\/  |         | |       |_   _|     | |               | | (_)           \n");
    mvwprintw(menu, max_row/2 - 5,max_col/5," | \\  / | ___   __| | ___     | |  _ __ | |_ ___ _ __ __ _| |_ ___   _____  \n");
    mvwprintw(menu, max_row/2 -4,max_col/5," | |\\/| |/ _ \\ / _` |/ _ \\    | | | '_ \\| __/ _ \\ '__/ _` | __| \\ \\ / / _ \\ \n");
    mvwprintw(menu, max_row/2- 3,max_col/5," | |  | | (_) | (_| | (_) |  _| |_| | | | ||  __/ | | (_| | |_| |\\ V / (_) | \n");
    mvwprintw(menu, max_row/2 - 2,max_col/5," |_|  |_|\\___/ \\__,_|\\___/  |_____|_| |_|\\__\\___|_|  \\__,_|\\__|_| \\_/ \\___/ \n");
     wattroff(menu, A_BOLD);
      wattroff(menu, COLOR_PAIR (4));

    mvwprintw(menu, max_row/2 + 1, max_col/3, "Passar Queries");
    mvwprintw(menu, max_row/2 + 2, max_col/3, "Sair");


     wattron(menu, A_BOLD);
	mvwprintw(menu, max_row/2 + 4, max_col/3 - 12, "Pressiona ENTER para selecionar uma opção!");
    wattroff(menu, A_BOLD);

    keypad(menu, TRUE);

    int key;
    int selected = 0;

    do
    {
        for(int i = 0; i < 2; i++)
        {
            if(i == selected)
            {
                wattron(menu, A_BOLD);	//Caso esteja selecionado ativamos os atributos
				wattron(menu, COLOR_PAIR(4));
            }
       
            switch(i)
            {
            case 0:
                mvwprintw(menu, max_row/2 + 1, max_col/3, "Passar Queries");
                break;
            case 1:
                mvwprintw(menu, max_row/2 + 2, max_col/3, "Sair");
                break;
            }
            wattroff(menu, A_BOLD);	//Desliga o atributo quando trocamos de opção
			wattroff(menu, COLOR_PAIR(4));
        }

        wrefresh(menu);
        key = wgetch(menu); //Input do utilizador

        if (key == '8' || key == KEY_UP) 
		{
            selected--;
            if (selected < 0)
			{
				selected = 1;	//Caso tentemos exceder o topo dá wormhole para baixo
			} 
        }
        else if (key == '2' || key == KEY_DOWN) 
		{
            selected++;
            if (selected > 1)
			{
				selected = 0; //Caso tentemos exceder o minimo dá wormhole para cima
			} 
        }

 } while (key != '\n');

    endwin();
    return selected;
    
}