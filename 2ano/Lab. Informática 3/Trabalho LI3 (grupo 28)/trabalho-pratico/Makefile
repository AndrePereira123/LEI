CC=gcc

CFLAGS=-Wall -Iincludes -Ipath/to/glib/include `pkg-config --cflags glib-2.0`

LDFLAGS += $(shell pkg-config --libs glib-2.0) -lncurses

all : main teste

main:	   src/main.o interativo/modo_interativo.o src/utilidade_tempo.o interativo/menu.o src/gestor_utilizador.o src/struct_utilizador.o src/gestor_reserva.o src/struct_reserva.o src/gestor_voos.o src/struct_voos.o src/gestor_passageiro.o src/erros.o src/parser.o  src/interpreter.o   src/queries.o interativo/queries_interativas.o testes/queries_testes.o    
	gcc -g src/main.o interativo/modo_interativo.o src/utilidade_tempo.o interativo/menu.o src/gestor_utilizador.o src/struct_utilizador.o src/gestor_reserva.o src/struct_reserva.o src/gestor_voos.o src/struct_voos.o src/gestor_passageiro.o src/erros.o src/parser.o  src/interpreter.o   src/queries.o interativo/queries_interativas.o testes/queries_testes.o  $(LDFLAGS) -o programa-principal

teste:     testes/programa-testes.o src/utilidade_tempo.o src/gestor_utilizador.o src/struct_utilizador.o src/gestor_reserva.o src/struct_reserva.o src/gestor_voos.o src/struct_voos.o src/gestor_passageiro.o src/erros.o src/parser.o  src/interpreter.o   src/queries.o testes/queries_testes.o    
	gcc -g testes/programa-testes.o src/utilidade_tempo.o src/gestor_utilizador.o src/struct_utilizador.o src/gestor_reserva.o src/struct_reserva.o src/gestor_voos.o src/struct_voos.o src/gestor_passageiro.o src/erros.o src/parser.o  src/interpreter.o   src/queries.o testes/queries_testes.o  $(LDFLAGS) -o programa-testes

src/main.o: src/main.c

interativo/modo_interativo.o: interativo/modo_interativo.c

src/utilidade_tempo.o: src/utilidade_tempo.c

interativo/menu.o: interativo/menu.c

src/gestor_utilizador.o: src/gestor_utilizador.c

src/struct_utilizador.o: src/struct_utilizador.c

src/gestor_reserva.o: src/struct_reserva.c

src/struct_reserva.o: src/struct_reserva.c

src/gestor_voos.o: src/gestor_voos.c

src/struct_voos.o: src/struct_voos.c

src/gestor_passageiro.o: src/gestor_passageiro.c

src/erros.o: src/erros.c

src/parser.o: src/parser.c

src/interpreter.o: src/interpreter.c

src/queries.o: src/queries.c

interativo/queries_interativas.o: interativo/queries_interativas.c

testes/queries_testes.o: testes/queries_testes.c

testes/programa-testes.o: testes/programa-testes.c


clean:
	rm -f *.o programa-principal
	rm -f *.o programa-testes
	rm -f src/*.o $(objects)
	rm -f testes/*.o $(objects) 
	rm -f interativo/*.o $(objects) 
	rm -rf Resultados/*


