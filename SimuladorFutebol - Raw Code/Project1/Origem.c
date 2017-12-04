#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TRUE	1
#define FALSE	0

typedef struct node_jogos {
	int equipaA;
	int equipaB;
	struct node_jogos *next;
} node_jogos;

typedef struct lista_jogos {
	node_jogos *head;
} lista_jogos;

typedef struct node_jornadas {
	lista_jogos *jogos;
	struct node_jornadas *next;
} node_jornadas;

typedef struct lista_jornadas {
	node_jornadas *head;
} list_jornadas;

struct resultados {
	int n_empates;
	int n_derrotas;
	int n_vitorias;
	int golos_marcados;
	int golos_sofridos;
};

struct jogador {
	char nome[30]; //nome do jogador
	int def; //pontuação  do jogador como defesa
	int med; //pontuação do jogador como médio
	int atac; //pontuação do jogador como atacante
	int gr; //pontuação do jogador como guarda-redes
	int pos; //posição a definir para o jogador
};

typedef struct node {
	struct jogador jogador;
	struct node *next;
} node_t;

typedef struct lista {
	node_t *head;
} list_t;

struct formacao {
	int numdef; //número de defesas
	int nummed; //número de médios
	int numatac; //número de atacantes
};

struct equipa {
	int pont_geral;
	char nome[30];
	list_t *lista;
	struct formacao formacao;
	struct resultados resultados;
};

typedef struct node_equs {
	struct equipa equipa;
	struct node_equs *next;
} node_eqs;

typedef struct lista_equs {
	node_eqs *head;
} list_eqs;

//Funções de manipulação da lista ligada------------------------------------------------------------------
node_t * createNode() {
	node_t *node = NULL;
	node = (node_t *)malloc(sizeof(node_t));
	return (node);
}

node_eqs * createNode_eqs() {
	node_eqs *node = NULL;
	node = (node_eqs *)malloc(sizeof(node_eqs));
	return (node);
}

node_jogos * createNode_jogos() {
	node_jogos *node = NULL;
	node = (node_jogos *)malloc(sizeof(node_jogos));
	return (node);
}

node_jornadas * createNode_jornadas() {
	node_jornadas *node = NULL;
	node = (node_jornadas *)malloc(sizeof(node_jornadas));
	return (node);
}

void removeNode(node_t *node) {
	if (node)
		free(node);
}

list_eqs * newList_eqs() {
	list_eqs *list;
	list = (list_eqs *)malloc(sizeof(list_eqs));
	list->head = NULL;
	return (list);
}

list_jornadas * newList_jornadas() {
	list_jornadas *list;
	list = (list_jornadas *)malloc(sizeof(list_jornadas));
	list->head = NULL;
	return (list);
}

lista_jogos * newList_jogos() {
	lista_jogos *list;
	list = (lista_jogos *)malloc(sizeof(lista_jogos));
	list->head = NULL;
	return (list);
}

list_t * newList() {
	list_t *list;
	list = (list_t *)malloc(sizeof(list_t));
	list->head = NULL;
	return (list);
}

list_t * freeList(list_t *list) {
	if (list->head == NULL) {
		free(list);
		list = NULL;
	}
	return (list);
}

void insertListTop_eqs(list_eqs *list, node_eqs *node) {
		if (list->head != NULL)
			node->next = list->head;
		list->head = node;
}

void inserir_de_qualquer_lado(list_eqs *list, node_eqs *node) {
	node_eqs *node_atual = list->head;
	node_eqs *node_ant = list->head;

	if (list->head == node) return;
	while (node) {
		if (node_atual == node) {
			node_ant->next = node_atual->next;
			insertListTop_eqs(list, node);
			break;
		}
		
		else {
			node_ant = node_atual;
			node_atual = node_atual->next;
		}
	}
}

void insertListTop(list_t *list, node_t *node) {
	if (list->head != NULL)
		node->next = list->head;
	list->head = node;
}

void insertListTop_jornadas(list_jornadas *list, node_jornadas *node) {
	if (list->head != NULL)
		node->next = list->head;
	list->head = node;
}

void insertListTop_jogos(lista_jogos *list, node_jogos *node) {
	if (list->head != NULL)
		node->next = list->head;
	list->head = node;
}

//------------------------------------------------------------------------
//Funções de Recolha e mudança de jogadores

//Core Recolha dos dados dos ficheiros
list_t* recolher_dados_jogadores(char *nome_equipa)
{
	FILE *fptr;
	list_t *lista;
	node_t *node;
	char c[2];
	char nome[30];
	char txt[] = ".txt";

	lista = newList();

	strcpy(nome, nome_equipa);
	strcat(nome, txt);
	fptr = fopen(nome, "r");

	if (fptr == NULL) {
		printf("Impossivel abrir ficheiro!");
		return NULL;
	}

	while (fgets(c, sizeof(c), fptr)) {
		/*Cria um nó e insere os dados*/
		node = createNode();

		fgets(node->jogador.nome, sizeof(node->jogador.nome), fptr);
		fscanf(fptr, "%d %d %d %d %d", &node->jogador.gr, &node->jogador.def, 
				&node->jogador.med, &node->jogador.atac, &node->jogador.pos);
		node->next = NULL;
		/*Insere o no' na lista*/
		insertListTop(lista, node);
	}
	fclose(fptr);

	return lista;
}

//IO e Semi-Core Permite nos escolher a formação para a equipa
void escolher_form(node_eqs *node) {
	printf("Qual é a formacao que pretende?(Minimo de 1 em cada posicao)\n");
	printf("Quantos defesas, medios e atacantes quer?(Separado por espaco)");
	scanf("%d", &node->equipa.formacao.numdef);
	scanf("%d", &node->equipa.formacao.nummed);
	scanf("%d", &node->equipa.formacao.numatac);
	
	if (node->equipa.formacao.numdef < 1 || node->equipa.formacao.nummed < 1 || node->equipa.formacao.numatac < 1 ||
		node->equipa.formacao.numdef + node->equipa.formacao.nummed + node->equipa.formacao.numatac !=10) {
		
		while (node->equipa.formacao.numdef < 1 || node->equipa.formacao.nummed < 1 || node->equipa.formacao.numatac < 1 ||
			node->equipa.formacao.numdef + node->equipa.formacao.nummed + node->equipa.formacao.numatac != 10) {
			printf("Formacao invalida!\n");
			printf("Quantos defesas, medios e atacantes quer?(Separado por espaco)");
			scanf("%d", &node->equipa.formacao.numdef);
			scanf("%d", &node->equipa.formacao.nummed);
			scanf("%d", &node->equipa.formacao.numatac);
		}
	}
}

//Permite analisar as strings sem ser casesensitive(Nenhuma funçao em C o permite)
int casesensitive(char *nome, char *nomestr) {
	char nomes, nomestrs;
	int i=0;

	for(i; i < 30; i++) {
		nomes = *nome; 
		nomestrs = *nomestr;

		if (nomes != nomestrs) {
			if (nomes < 97 && nomestrs>=97) 
				nomes += 32;
			else if (nomes >= 97 && nomestrs<97) 
				nomes -= 32;
			else if (nomes == 32) 
				return 1;
			if (nomes != nomestrs) 
				return NULL;
		}
		if (nomes == 0) return 1;
		nome++; nomestr++;
	}
	return 0;
}

//Semi-core Permite procurar se o jogador escolhido existe
node_t * procura_jog(list_t *lista, char *nome) {
	node_t *node = lista->head;

	while (node) {
		if (casesensitive(node->jogador.nome, nome))
			break;
		else node = node->next;
	}
	return (node);
}

//IO e Semi-core Permite nos mudar a posiçao do jogador escolhido e mudar o parametro pos dele
void posicao_jog(node_t *node, struct formacao *equipa)
{
	static int gr = 0, def = 0, med = 0, atac = 0;
	int pos, erro;

	printf("Em que posicao quer colocar este jogador?");
	erro = scanf("%d", &pos);

	if (erro != 1) {
		printf("\n\n\n\n\n\nInsira um caracter valido");
		exit(-1);
	}

	if ((pos<0) || (pos > 4 || (def == equipa->numdef && pos == 2) || (med == equipa->nummed && pos == 3) ||
		(atac == equipa->numatac && pos == 4) || (gr == 1 && pos == 1))) {
		do {
			printf("Posicao Invalida! \n");
			printf("Em que posicao quer colocar este jogador?");
			scanf("%d", &pos);
		} while (pos > 4 || (def == equipa->numdef && pos == 2) || (med == equipa->nummed && pos == 3) ||
			(atac == equipa->numatac && pos == 4) || (gr == 1 && pos == 1));
	}

	else if ((node->jogador.pos != 0)) {
		switch (node->jogador.pos)
		{
		case 1:
			gr--;
			break;
		case 2:
			def--;
			break;
		case 3:
			med--;
			break;
		case 4:
			atac--;
			break;
		default:
			break;
		}
	}

	if (pos == 1) gr++;
	if (pos == 2) def++;
	if (pos == 3) med++;
	if (pos == 4) atac++;

	node->jogador.pos = pos;
}


//IO Função para imprimir os jogadores no ecrã
void printjogs(list_t *lista) {
	node_t *node = lista->head;

	while (node) {
		switch (node->jogador.pos)
		{
		case 1:
			printf("Guarda-Redes - ");
			break;
		case 2:
			printf("Defesa - ");
			break;
		case 3:
			printf("Medio - ");
			break;
		case 4:
			printf("Atacante - ");
			break;
		default:
			printf("Reserva - ");
			break;
		}
		printf("%s %d %d %d %d \n", node->jogador.nome, node->jogador.gr, node->jogador.def,
			node->jogador.med, node->jogador.atac);
		node = node->next;
	}
}

//IO Função para receber o nome do jogador desejado
void perguntjog(char *nome) {
	printf("Que jogador deseja? ");
	scanf(" %29[^\n]s", nome);
}

void escolher_jogadores(node_eqs *node_eqs) 
{
	node_t *node;
	int numjog = 0;
	char nome[30];

	while (numjog != 11) {
		printjogs(node_eqs->equipa.lista);
		perguntjog(nome);

		node = procura_jog(node_eqs->equipa.lista, nome);

		while (!node) {
			printf("Jogador inválido!");
			perguntjog(nome);
			node = procura_jog(node_eqs->equipa.lista, nome);
		}

		posicao_jog(node, &node_eqs->equipa.formacao);
		numjog++;

		system("cls");
	}
	system("cls");
	printf("Equipa definida!");
}

int mudar_jogadores(node_eqs *node_eqs) {
	escolher_form(node_eqs);
	escolher_jogadores(node_eqs);
	return TRUE;
}

//----------------------------------------------------------------------
//Funções de escolha de equipa

void recolhe_formacao(node_eqs *node_eqs);

//Preenche todos os dados das equipas
int recolher_dados_das_equipas(list_eqs *lista_eqs, int *nr_equipas)
{
	FILE *fptr;
	node_eqs *node;
	char c[2];

	fptr = fopen("equipas.txt", "r");

	if (fptr == NULL) {
		printf("Impossivel abrir ficheiro!");
		return FALSE;
	}

	while (fgets(c, sizeof(c), fptr)) {
		/*Cria um nó e insere os dados*/
		node = createNode_eqs();

		fscanf(fptr, " %29[^\n]s", node->equipa.nome);
		fscanf(fptr, "%d %d %d %d %d", &node->equipa.resultados.n_derrotas, 
			&node->equipa.resultados.n_empates ,&node->equipa.resultados.n_vitorias, 
			&node->equipa.resultados.golos_marcados, &node->equipa.resultados.golos_sofridos);
		node->next = NULL;

		if (!(node->equipa.lista = recolher_dados_jogadores(node->equipa.nome)))
			return FALSE;
		node->equipa.pont_geral = le_pont_geral(node);
		recolhe_formacao(node);
		/*Insere o nó na lista*/
		insertListTop_eqs(lista_eqs, node);
		(*nr_equipas)++;
	}
	fclose(fptr);

	return TRUE;
}

node_eqs * procura_equipa(list_eqs *lista, char *nome) {
	node_eqs *node = lista->head;

	while (node) {
		if (casesensitive(node->equipa.nome, nome))
			break;
		else node = node->next;
	}
	return (node);
}

void recolhe_eq_pret(list_eqs *lista_eqs, char *nomes) {
	node_eqs *node = lista_eqs->head;
	
	printf("Qual equipa pertende?\n");
	printf("Nome da equipa - Pontuacao geral - Formacao(Formacao afeta a pontuacao geral da equipa)\n\n");
	
	while (node) {
		printf("%22s - %d - %d %d %d\n", node->equipa.nome, node->equipa.pont_geral, node->equipa.formacao.numdef,
				node->equipa.formacao.nummed, node->equipa.formacao.numatac);
		node = node->next;
	}

	scanf(" %29[^\n]s", nomes);
}

void recolhe_formacao(node_eqs *node_eqs) {
	node_t *node = node_eqs->equipa.lista->head;
	node_eqs->equipa.formacao.numatac = 0;
	node_eqs->equipa.formacao.nummed = 0;
	node_eqs->equipa.formacao.numdef = 0;

	while (node) {
		if (node->jogador.pos < 5 && node->jogador.pos != 0) {
			switch (node->jogador.pos)
			{
			case 2:
				node_eqs->equipa.formacao.numdef += 1;
				break;
			case 3:
				node_eqs->equipa.formacao.nummed += 1;
				break;
			case 4:
				node_eqs->equipa.formacao.numatac += 1;
				break;
			default:
				break;
			}
		}
		node = node->next;
	}
}

void escolher_equipa(list_eqs *lista_eqs) {
	char nomes[30];
	char sim[] = "s ";
	char decisao[10];
	node_eqs *node;
	
	do {
		system("cls");
		recolhe_eq_pret(lista_eqs, nomes);

		if (!(node = procura_equipa(lista_eqs, nomes)))
			while (!(node = procura_equipa(lista_eqs, nomes))) {
				system("cls");
				printf("----------------------------Equipa invalida!----------------------------------\n");
				recolhe_eq_pret(lista_eqs, nomes);
			}
		printjogs(node->equipa.lista);
		printf("Tem a certeza que quer esta equipa? (s/n) ");
		scanf("%s", decisao);
	} while (!casesensitive(sim, decisao));

	inserir_de_qualquer_lado(lista_eqs, node);
}

int soma_pnt_geral(list_t *lista) {
	node_t *node = lista->head;
	int pnt_geral = 0;

	while (node) {
		if (node->jogador.pos < 5 && node->jogador.pos != 0) {
			switch (node->jogador.pos)
			{
			case 1:
				pnt_geral = pnt_geral + node->jogador.gr;
				break;
			case 2:
				pnt_geral = pnt_geral + node->jogador.def;
				break;
			case 3:
				pnt_geral = pnt_geral + node->jogador.med;
				break;
			case 4:
				pnt_geral = pnt_geral + node->jogador.atac;
				break;
			default:
				break;
			}
		}
		node = node->next;
	}
	return (pnt_geral / 11);
}

int le_pont_geral(node_eqs *equipaA) {
	equipaA->equipa.pont_geral = soma_pnt_geral(equipaA->equipa.lista);
}

//---------------------------------------------------------
//Bloco de simulação de jogo
void registar_resultados(node_eqs *equipaA, node_eqs *equipaB, int resultados, int gA, int gB) {
	
	if(resultados == 1){
		equipaA->equipa.resultados.n_vitorias += 1;
		equipaB->equipa.resultados.n_derrotas += 1;
	}
	
	else if (resultados == 2){
		equipaB->equipa.resultados.n_vitorias += 1;
		equipaA->equipa.resultados.n_derrotas += 1;
	}

	else {
		equipaB->equipa.resultados.n_empates += 1; 
		equipaA->equipa.resultados.n_empates += 1;
	}

	equipaB->equipa.resultados.golos_marcados += gB;
	equipaA->equipa.resultados.golos_marcados += gA;

	equipaB->equipa.resultados.golos_sofridos += gA;
	equipaA->equipa.resultados.golos_sofridos += gB;
	
}

void simulacao_user(node_eqs *equipaA, node_eqs *equipaB) {
	int n;
	float peqA = equipaA->equipa.pont_geral, peqB = equipaB->equipa.pont_geral, resultado = 0;
	double pA, pB, pgA, pgB, i, f; //pA = probabilidade da equipa A marcar de 0 a 100, pgA = probabilidade de golos real
	int gA = 0, gB = 0;

	pA = peqA / (peqA + peqB);
	pB = peqB / (peqA + peqB);
	pgA = pA * 0.025 * 100;
	pgB = pB * 0.025 * 100;


	for (n = 1; n < 90; n++) {
		i = rand() % 101;
		f = rand() % 99;
		f = f / 100;
		i = i + f;
		if (i >= 0 && i < pgA) {
			gA++;
			printf("A equipa %s marcou golo ao minuto %d\n", equipaA->equipa.nome,n);
		}
		else if (i >= pgA && i < 2.5) {
			gB++;
			printf("A equipa %s marcou golo ao minuto %d\n", equipaB->equipa.nome,n);
		}
	}


	if (gA > gB) {
		printf("A equipa %s ganhou a partida por %d-%d! \n", equipaA->equipa.nome, gA, gB);
		resultado = 1;
	}
	else if (gA < gB) {
		printf("A equipa %s ganhou a partida por %d-%d! \n", equipaB->equipa.nome, gA, gB);
		resultado = 2;
	}
	else printf("A equipa %s e a equipa %s empataram por %d-%d! \n", equipaA->equipa.nome, equipaB->equipa.nome, gA, gB);

	registar_resultados(equipaA, equipaB, resultado, gA, gB);
}

node_jogos* procura_jogo(list_jornadas *lista, int i, int j) {
	node_jornadas *node = lista->head;
	node_jogos *jogo;

	while (node) {
		jogo = node->jogos->head;
		while (jogo) {
			if (jogo->equipaA == i && jogo->equipaB == j || jogo->equipaA == j && jogo->equipaB == i)
					return(node);
			else jogo = jogo->next;
		}
		node = node->next;
	}
	return (node);
}

int calendario(int nr_equipas, list_jornadas *lista) {
	int i, j, nr_jogs, nr_jornadas = 0;
	int nr_desejado = ((nr_equipas * 2) - 2);
	nr_desejado = nr_desejado / 2;
	node_jogos *node;
	node_jornadas *jornada;

	do {
		jornada = createNode_jornadas();
		jornada->next = NULL;

		insertListTop_jornadas(lista, jornada);
		jornada->jogos = newList_jogos();
		nr_jogs = 0;
		
		do {
			i = rand() % (nr_equipas) + 1;
			j = rand() % (nr_equipas) + 1;

			if (i != j && !procura_jogo(lista, i, j)) {
				node = createNode_jogos();
				node->equipaA = i;
				node->equipaB = j;
				node->next = NULL;
				insertListTop_jogos(lista->head->jogos, node);
				nr_jogs++;
				}
			}while (nr_jogs != (nr_equipas / 2));
			
			nr_jornadas++;
	} while (nr_jornadas != nr_desejado);
	return nr_jornadas;
}

int conta_nr_nos(list_eqs *lista) {
	node_eqs *node = lista->head;
	int nr_nodes = 0;

	while (node) {
		nr_nodes++;
		node = node->next;
	}
	return nr_nodes;
}

node_eqs * getListItems(list_eqs *list, int num) {
	node_eqs *node = list->head;
	int i = 1;
	while (node) {
		if (i == num)
			break;
		else {
			i++;
			node = node->next;
		}
	}
	return (node);
}

void fazer_jornada(lista_jogos *lista_jogos, list_eqs *lista_eqs) {
	node_jogos *node = lista_jogos->head;
	node_eqs *equipaA;
	node_eqs *equipaB;
	
	while (node) {
		equipaA = getListItems(lista_eqs, node->equipaA);
		equipaB = getListItems(lista_eqs, node->equipaB);

		simulacao_user(equipaA, equipaB);
		node = node->next;
	}
}

void fazer_jornada_fase(lista_jogos *lista_jogos, list_eqs *lista_eqs) {
	node_jogos *node = lista_jogos->head;
	node_eqs *equipaA;
	node_eqs *equipaB;

	while (node) {
		equipaA = getListItems(lista_eqs, node->equipaA);
		equipaB = getListItems(lista_eqs, node->equipaB);

		simulacao_user(equipaB, equipaA);
		node = node->next;
	}
}

void print_resultados(list_eqs *lista) {
	node_eqs *node = lista->head;
	
	while (node) {
		printf("%s  Derrotas - %d, Empates - %d, Vitorias - %d, Gm - %d, Gs - %d\n", node->equipa.nome,
			node->equipa.resultados.n_derrotas, node->equipa.resultados.n_empates, node->equipa.resultados.n_vitorias
			, node->equipa.resultados.golos_marcados, node->equipa.resultados.golos_sofridos);
			node = node->next;
	}
}

int main(){
	srand(time(NULL));
	char sim[] = "s";
	char decis[2];
	int nr_jor = 0;
	int erro, nr_equipas = 0;
	list_eqs *lista_eqs = newList_eqs();
	list_jornadas *lista_jornadas = newList_jornadas();
	node_jornadas *jornada;

	erro = recolher_dados_das_equipas(lista_eqs, &nr_equipas);
	if (erro == FALSE) {
			return(-1);
		}

	escolher_equipa(lista_eqs);
	nr_equipas = conta_nr_nos(lista_eqs);
	int nr_jornadas = calendario(nr_equipas, lista_jornadas);
	
	jornada = lista_jornadas -> head;
	do{
		printf("Quer mudar o plantel? (s/n)");
		scanf("%s", decis);

		if (casesensitive(sim, decis))
			mudar_jogadores(lista_eqs->head);
		if (nr_jor == nr_jornadas) {
			jornada = lista_jornadas->head;
		}

		if (nr_jor >= nr_jornadas || jornada != NULL) {
			fazer_jornada_fase(jornada->jogos, lista_eqs);
			jornada = jornada->next;
			nr_jor++;
			printf("\n");
		}
		else if(jornada != NULL) {
			fazer_jornada(jornada->jogos, lista_eqs);
			jornada = jornada->next;
			nr_jor++;
			printf("\n");
		}
	} while (nr_jor != nr_jornadas * 2);
	printf("\n");

	print_resultados(lista_eqs);
	return 0;
}