#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TURMAS 50
#define MAX_AULAS 1000
#define MAX_ALUNOS_POR_TURMA 30

typedef struct {
    char nome[100];
    int ano;
    int serie;
    char turno[20];
    int capacidade_maxima;
    int id;
} Turma;

typedef struct {
    char nome[100];
    char email[100];
    char senha[50];
    char data_nascimento[15];
    char genero;
    char cpf[15];
    int matricula;
} Aluno;

typedef struct {
    char nome[100];
    char email[100];
    char senha[50];
    char data_nascimento[15];
    char genero;
    char cpf[15];
    int matricula;
    char materia[50];
} Professor;

typedef struct {
    int professor_matricula;
    int turma_id;
    char dia[15];
    char hora[10];
    char materia[50];
    int id;
} Aula;

Turma turmas[MAX_TURMAS];
Aluno *alunos = NULL;
Professor *professores = NULL;
Aula aulas[MAX_AULAS];

int total_turmas = 0;
int total_alunos = 0;
int total_professores = 0;
int total_aulas = 0;
int proximo_id_turma = 1;
int proximo_id_aula = 1;
int proxima_matricula = 10000001;
int admin_logado = 0;
char admin_cpf[] = "12345678909";
char admin_senha[] = "admin@admin";

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int validar_cpf(char *cpf) {
    if (strlen(cpf) != 11) return 0;
    for (int i = 0; i < 11; i++) {
        if (cpf[i] < '0' || cpf[i] > '9') return 0;
    }
    return 1;
}

int validar_email(char *email) {
    if (strlen(email) < 5) return 0;
    int tem_arroba = 0, tem_ponto = 0;
    for (int i = 0; i < strlen(email); i++) {
        if (email[i] == '@') tem_arroba = 1;
        if (tem_arroba && email[i] == '.') tem_ponto = 1;
    }
    return tem_arroba && tem_ponto;
}

int validar_data(char *data) {
    if (strlen(data) != 10) return 0;
    if (data[2] != '/' || data[5] != '/') return 0;
    for (int i = 0; i < 10; i++) {
        if (i != 2 && i != 5 && (data[i] < '0' || data[i] > '9')) return 0;
    }
    int dia = (data[0] - '0') * 10 + (data[1] - '0');
    int mes = (data[3] - '0') * 10 + (data[4] - '0');
    if (dia < 1 || dia > 31 || mes < 1 || mes > 12) return 0;
    return 1;
}

void formatar_cpf(char *cpf, char *cpf_formatado) {
    sprintf(cpf_formatado, "%.3s.%.3s.%.3s-%.2s", cpf, cpf+3, cpf+6, cpf+9);
}

int cpf_ja_existe(char *cpf) {
    for (int i = 0; i < total_alunos; i++) {
        if (strcmp(alunos[i].cpf, cpf) == 0) return 1;
    }
    for (int i = 0; i < total_professores; i++) {
        if (strcmp(professores[i].cpf, cpf) == 0) return 1;
    }
    return 0;
}

void criar_turma() {
    if (!admin_logado) {
        printf("Acesso negado! Apenas administradores podem criar turmas.\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    if (total_turmas >= MAX_TURMAS) {
        printf("Limite de turmas atingido!\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    Turma nova_turma;
    nova_turma.id = proximo_id_turma++;
    
    printf("=== CRIAR TURMA ===\n");
    printf("(Digite '0' em qualquer campo para cancelar)\n\n");
    
    printf("Nome da turma: ");
    fgets(nova_turma.nome, 100, stdin);
    nova_turma.nome[strcspn(nova_turma.nome, "\n")] = 0;
    if (strcmp(nova_turma.nome, "0") == 0) return;
    
    printf("Ano (0 para cancelar): ");
    scanf("%d", &nova_turma.ano);
    if (nova_turma.ano == 0) return;
    
    printf("Serie (0 para cancelar): ");
    scanf("%d", &nova_turma.serie);
    limpar_buffer();
    if (nova_turma.serie == 0) return;
    
    printf("Turno: ");
    fgets(nova_turma.turno, 20, stdin);
    nova_turma.turno[strcspn(nova_turma.turno, "\n")] = 0;
    if (strcmp(nova_turma.turno, "0") == 0) return;
    
    nova_turma.capacidade_maxima = MAX_ALUNOS_POR_TURMA;
    printf("Capacidade maxima definida automaticamente: %d alunos\n", MAX_ALUNOS_POR_TURMA);
    
    turmas[total_turmas] = nova_turma;
    total_turmas++;
    
    printf("Turma criada com sucesso! ID: %d\n", nova_turma.id);
    printf("\nPressione Enter para voltar ao menu...");
    getchar();
}

void cadastrar_aluno() {
    if (!admin_logado) {
        printf("Acesso negado! Apenas administradores podem cadastrar alunos.\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    Aluno novo_aluno;
    
    printf("=== CADASTRAR ALUNO ===\n");
    printf("(Digite '0' em qualquer campo para cancelar)\n");
    printf("Matricula sera gerada automaticamente\n\n");
    
    printf("Nome: ");
    fgets(novo_aluno.nome, 100, stdin);
    novo_aluno.nome[strcspn(novo_aluno.nome, "\n")] = 0;
    if (strcmp(novo_aluno.nome, "0") == 0) return;
    
    while (1) {
        printf("Email: ");
        fgets(novo_aluno.email, 100, stdin);
        novo_aluno.email[strcspn(novo_aluno.email, "\n")] = 0;
        if (strcmp(novo_aluno.email, "0") == 0) return;
        
        if (!validar_email(novo_aluno.email)) {
            printf("Email invalido! Digite novamente.\n");
            continue;
        }
        break;
    }
    
    printf("Senha: ");
    fgets(novo_aluno.senha, 50, stdin);
    novo_aluno.senha[strcspn(novo_aluno.senha, "\n")] = 0;
    if (strcmp(novo_aluno.senha, "0") == 0) return;
    
    while (1) {
        printf("Data de nascimento (DD/MM/AAAA): ");
        fgets(novo_aluno.data_nascimento, 15, stdin);
        novo_aluno.data_nascimento[strcspn(novo_aluno.data_nascimento, "\n")] = 0;
        if (strcmp(novo_aluno.data_nascimento, "0") == 0) return;
        
        if (!validar_data(novo_aluno.data_nascimento)) {
            printf("Data invalida! Use o formato DD/MM/AAAA.\n");
            continue;
        }
        break;
    }
    
    while (1) {
        printf("Genero (M/F): ");
        scanf(" %c", &novo_aluno.genero);
        limpar_buffer();
        if (novo_aluno.genero == '0') return;
        
        if (novo_aluno.genero != 'M' && novo_aluno.genero != 'F' && novo_aluno.genero != 'm' && novo_aluno.genero != 'f') {
            printf("Genero invalido! Digite M para masculino ou F para feminino.\n");
            continue;
        }
        
        if (novo_aluno.genero == 'm') novo_aluno.genero = 'M';
        if (novo_aluno.genero == 'f') novo_aluno.genero = 'F';
        break;
    }
    
    while (1) {
        printf("Digite seu CPF (apenas numeros): ");
        fgets(novo_aluno.cpf, 15, stdin);
        novo_aluno.cpf[strcspn(novo_aluno.cpf, "\n")] = 0;
        if (strcmp(novo_aluno.cpf, "0") == 0) return;
        
        if (!validar_cpf(novo_aluno.cpf)) {
            printf("CPF invalido! Digite 11 numeros.\n");
            continue;
        }
        
        if (cpf_ja_existe(novo_aluno.cpf)) {
            printf("CPF ja cadastrado! Digite outro CPF.\n");
            continue;
        }
        
        char cpf_formatado[15];
        formatar_cpf(novo_aluno.cpf, cpf_formatado);
        printf("CPF cadastrado: %s\n", cpf_formatado);
        break;
    }
    
    novo_aluno.matricula = proxima_matricula++;
    printf("Matricula gerada automaticamente: %d\n", novo_aluno.matricula);
    
    alunos = realloc(alunos, (total_alunos + 1) * sizeof(Aluno));
    alunos[total_alunos] = novo_aluno;
    total_alunos++;
    
    printf("Aluno cadastrado com sucesso!\n");
    printf("\nPressione Enter para voltar ao menu...");
    getchar();
}

void cadastrar_professor() {
    if (!admin_logado) {
        printf("Acesso negado! Apenas administradores podem cadastrar professores.\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    Professor novo_professor;
    
    printf("=== CADASTRAR PROFESSOR ===\n");
    printf("(Digite '0' em qualquer campo para cancelar)\n");
    printf("Matricula sera gerada automaticamente\n\n");
    
    printf("Nome: ");
    fgets(novo_professor.nome, 100, stdin);
    novo_professor.nome[strcspn(novo_professor.nome, "\n")] = 0;
    if (strcmp(novo_professor.nome, "0") == 0) return;
    
    while (1) {
        printf("Email: ");
        fgets(novo_professor.email, 100, stdin);
        novo_professor.email[strcspn(novo_professor.email, "\n")] = 0;
        if (strcmp(novo_professor.email, "0") == 0) return;
        
        if (!validar_email(novo_professor.email)) {
            printf("Email invalido! Digite novamente.\n");
            continue;
        }
        break;
    }
    
    printf("Senha: ");
    fgets(novo_professor.senha, 50, stdin);
    novo_professor.senha[strcspn(novo_professor.senha, "\n")] = 0;
    if (strcmp(novo_professor.senha, "0") == 0) return;
    
    while (1) {
        printf("Data de nascimento (DD/MM/AAAA): ");
        fgets(novo_professor.data_nascimento, 15, stdin);
        novo_professor.data_nascimento[strcspn(novo_professor.data_nascimento, "\n")] = 0;
        if (strcmp(novo_professor.data_nascimento, "0") == 0) return;
        
        if (!validar_data(novo_professor.data_nascimento)) {
            printf("Data invalida! Use o formato DD/MM/AAAA.\n");
            continue;
        }
        break;
    }
    
    while (1) {
        printf("Genero (M/F): ");
        scanf(" %c", &novo_professor.genero);
        limpar_buffer();
        if (novo_professor.genero == '0') return;
        
        if (novo_professor.genero != 'M' && novo_professor.genero != 'F' && novo_professor.genero != 'm' && novo_professor.genero != 'f') {
            printf("Genero invalido! Digite M para masculino ou F para feminino.\n");
            continue;
        }
        
        if (novo_professor.genero == 'm') novo_professor.genero = 'M';
        if (novo_professor.genero == 'f') novo_professor.genero = 'F';
        break;
    }
    
    
    while (1) {
        printf("Digite seu CPF (apenas numeros): ");
        fgets(novo_professor.cpf, 15, stdin);
        novo_professor.cpf[strcspn(novo_professor.cpf, "\n")] = 0;
        if (strcmp(novo_professor.cpf, "0") == 0) return;
        
        if (!validar_cpf(novo_professor.cpf)) {
            printf("CPF invalido! Digite 11 numeros.\n");
            continue;
        }
        
        if (cpf_ja_existe(novo_professor.cpf)) {
            printf("CPF ja cadastrado! Digite outro CPF.\n");
            continue;
        }
        
        char cpf_formatado[15];
        formatar_cpf(novo_professor.cpf, cpf_formatado);
        printf("CPF cadastrado: %s\n", cpf_formatado);
        break;
    }
    
    novo_professor.matricula = proxima_matricula++;
    printf("Matricula gerada automaticamente: %d\n", novo_professor.matricula);
    
    printf("Materia (max 50 caracteres): ");
    fgets(novo_professor.materia, 50, stdin);
    novo_professor.materia[strcspn(novo_professor.materia, "\n")] = 0;
    if (strcmp(novo_professor.materia, "0") == 0) return;
    
    professores = realloc(professores, (total_professores + 1) * sizeof(Professor));
    professores[total_professores] = novo_professor;
    total_professores++;
    
    printf("Professor cadastrado com sucesso!\n");
    printf("\nPressione Enter para voltar ao menu...");
    getchar();
}

void registrar_aula() {
    if (!admin_logado) {
        printf("Acesso negado! Apenas administradores podem registrar aulas.\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    if (total_aulas >= MAX_AULAS) {
        printf("Limite de aulas atingido!\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    Aula nova_aula;
    nova_aula.id = proximo_id_aula++;
    
    printf("=== REGISTRAR AULA ===\n");
    printf("(Digite '0' em qualquer campo para cancelar)\n\n");
    
    printf("Matricula do professor (0 para cancelar): ");
    scanf("%d", &nova_aula.professor_matricula);
    if (nova_aula.professor_matricula == 0) return;
    
    printf("ID da turma (0 para cancelar): ");
    scanf("%d", &nova_aula.turma_id);
    limpar_buffer();
    if (nova_aula.turma_id == 0) return;
    
    printf("Dia da aula: ");
    fgets(nova_aula.dia, 15, stdin);
    nova_aula.dia[strcspn(nova_aula.dia, "\n")] = 0;
    if (strcmp(nova_aula.dia, "0") == 0) return;
    
    printf("Hora da aula (HH:MM): ");
    fgets(nova_aula.hora, 10, stdin);
    nova_aula.hora[strcspn(nova_aula.hora, "\n")] = 0;
    if (strcmp(nova_aula.hora, "0") == 0) return;
    
    printf("Materia: ");
    fgets(nova_aula.materia, 50, stdin);
    nova_aula.materia[strcspn(nova_aula.materia, "\n")] = 0;
    if (strcmp(nova_aula.materia, "0") == 0) return;
    
    aulas[total_aulas] = nova_aula;
    total_aulas++;
    
    printf("Aula registrada com sucesso! ID: %d\n", nova_aula.id);
    printf("\nPressione Enter para voltar ao menu...");
    getchar();
}

int fazer_login() {
    char cpf[15];
    char senha[50];
    
    printf("CPF: ");
    fgets(cpf, 15, stdin);
    cpf[strcspn(cpf, "\n")] = 0;
    
    printf("Senha: ");
    fgets(senha, 50, stdin);
    senha[strcspn(senha, "\n")] = 0;
    
    if (strcmp(cpf, admin_cpf) == 0 && strcmp(senha, admin_senha) == 0) {
        admin_logado = 1;
        printf("Login de administrador realizado com sucesso!\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return 1;
    }
    
    for (int i = 0; i < total_alunos; i++) {
        if (strcmp(alunos[i].cpf, cpf) == 0 && strcmp(alunos[i].senha, senha) == 0) {
            admin_logado = 0;
            printf("Login realizado com sucesso! Bem-vindo, %s\n", alunos[i].nome);
            printf("\nPressione Enter para voltar ao menu...");
            getchar();
            return 1;
        }
    }
    
    for (int i = 0; i < total_professores; i++) {
        if (strcmp(professores[i].cpf, cpf) == 0 && strcmp(professores[i].senha, senha) == 0) {
            admin_logado = 0;
            printf("Login realizado com sucesso! Bem-vindo, %s\n", professores[i].nome);
            printf("\nPressione Enter para voltar ao menu...");
            getchar();
            return 1;
        }
    }
    
    printf("CPF ou senha incorretos!\n");
    printf("\nPressione Enter para voltar ao menu...");
    getchar();
    return 0;
}

void excluir_aluno() {
    if (total_alunos == 0) {
        printf("Nenhum aluno cadastrado.\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    int matricula;
    printf("Digite a matricula do aluno a ser excluido (0 para cancelar): ");
    scanf("%d", &matricula);
    limpar_buffer();
    if (matricula == 0) return;
    
    for (int i = 0; i < total_alunos; i++) {
        if (alunos[i].matricula == matricula) {
            for (int j = i; j < total_alunos - 1; j++) {
                alunos[j] = alunos[j + 1];
            }
            total_alunos--;
            alunos = realloc(alunos, total_alunos * sizeof(Aluno));
            printf("Aluno excluido com sucesso!\n");
            printf("\nPressione Enter para voltar ao menu...");
            getchar();
            return;
        }
    }
    printf("Aluno nao encontrado!\n");
    printf("\nPressione Enter para voltar ao menu...");
    getchar();
}

void listar_alunos() {
    if (total_alunos == 0) {
        printf("Nenhum aluno cadastrado.\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    printf("\n=== ALUNOS CADASTRADOS ===\n");
    for (int i = 0; i < total_alunos; i++) {
        char cpf_formatado[15];
        formatar_cpf(alunos[i].cpf, cpf_formatado);
        printf("Matricula: %d | Nome: %s | Email: %s | CPF: %s | Nascimento: %s | Genero: %c\n",
               alunos[i].matricula, alunos[i].nome, alunos[i].email, 
               cpf_formatado, alunos[i].data_nascimento, alunos[i].genero);
    }
    
    char opcao;
    printf("\nDeseja excluir algum aluno? (S/N): ");
    scanf(" %c", &opcao);
    limpar_buffer();
    
    if (opcao == 'S' || opcao == 's') {
        excluir_aluno();
    }
}

void excluir_professor() {
    if (total_professores == 0) {
        printf("Nenhum professor cadastrado.\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    int matricula;
    printf("Digite a matricula do professor a ser excluido (0 para cancelar): ");
    scanf("%d", &matricula);
    limpar_buffer();
    if (matricula == 0) return;
    
    for (int i = 0; i < total_professores; i++) {
        if (professores[i].matricula == matricula) {
            for (int j = i; j < total_professores - 1; j++) {
                professores[j] = professores[j + 1];
            }
            total_professores--;
            professores = realloc(professores, total_professores * sizeof(Professor));
            printf("Professor excluido com sucesso!\n");
            printf("\nPressione Enter para voltar ao menu...");
            getchar();
            return;
        }
    }
    printf("Professor nao encontrado!\n");
    printf("\nPressione Enter para voltar ao menu...");
    getchar();
}

void listar_professores() {
    if (total_professores == 0) {
        printf("Nenhum professor cadastrado.\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    printf("\n=== PROFESSORES CADASTRADOS ===\n");
    for (int i = 0; i < total_professores; i++) {
        char cpf_formatado[15];
        formatar_cpf(professores[i].cpf, cpf_formatado);
        printf("Matricula: %d | Nome: %s | Email: %s | CPF: %s | Genero: %c | Materia: %s\n",
               professores[i].matricula, professores[i].nome, professores[i].email,
               cpf_formatado, professores[i].genero, professores[i].materia);
    }
    
    char opcao;
    printf("\nDeseja excluir algum professor? (S/N): ");
    scanf(" %c", &opcao);
    limpar_buffer();
    
    if (opcao == 'S' || opcao == 's') {
        excluir_professor();
    }
}

void excluir_turma() {
    if (total_turmas == 0) {
        printf("Nenhuma turma cadastrada.\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    int id;
    printf("Digite o ID da turma a ser excluida (0 para cancelar): ");
    scanf("%d", &id);
    limpar_buffer();
    if (id == 0) return;
    
    for (int i = 0; i < total_turmas; i++) {
        if (turmas[i].id == id) {
            for (int j = i; j < total_turmas - 1; j++) {
                turmas[j] = turmas[j + 1];
            }
            total_turmas--;
            printf("Turma excluida com sucesso!\n");
            printf("\nPressione Enter para voltar ao menu...");
            getchar();
            return;
        }
    }
    printf("Turma nao encontrada!\n");
    printf("\nPressione Enter para voltar ao menu...");
    getchar();
}

void excluir_aula() {
    if (total_aulas == 0) {
        printf("Nenhuma aula registrada.\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    int id;
    printf("Digite o ID da aula a ser excluida (0 para cancelar): ");
    scanf("%d", &id);
    limpar_buffer();
    if (id == 0) return;
    
    for (int i = 0; i < total_aulas; i++) {
        if (aulas[i].id == id) {
            for (int j = i; j < total_aulas - 1; j++) {
                aulas[j] = aulas[j + 1];
            }
            total_aulas--;
            printf("Aula excluida com sucesso!\n");
            printf("\nPressione Enter para voltar ao menu...");
            getchar();
            return;
        }
    }
    printf("Aula nao encontrada!\n");
    printf("\nPressione Enter para voltar ao menu...");
    getchar();
}

void listar_turmas() {
    if (total_turmas == 0) {
        printf("Nenhuma turma cadastrada.\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    printf("\n=== TURMAS CADASTRADAS ===\n");
    for (int i = 0; i < total_turmas; i++) {
        printf("ID: %d | Nome: %s | Ano: %d | Serie: %d | Turno: %s | Capacidade: %d\n",
               turmas[i].id, turmas[i].nome, turmas[i].ano, turmas[i].serie, 
               turmas[i].turno, turmas[i].capacidade_maxima);
    }
    
    char opcao;
    printf("\nDeseja excluir alguma turma? (S/N): ");
    scanf(" %c", &opcao);
    limpar_buffer();
    
    if (opcao == 'S' || opcao == 's') {
        excluir_turma();
    }
}

void listar_aulas() {
    if (total_aulas == 0) {
        printf("Nenhuma aula registrada.\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    printf("\n=== AULAS REGISTRADAS ===\n");
    for (int i = 0; i < total_aulas; i++) {
        printf("ID: %d | Professor: %d | Turma: %d | Dia: %s | Hora: %s | Materia: %s\n",
               aulas[i].id, aulas[i].professor_matricula, aulas[i].turma_id,
               aulas[i].dia, aulas[i].hora, aulas[i].materia);
    }
    
    char opcao;
    printf("\nDeseja excluir alguma aula? (S/N): ");
    scanf(" %c", &opcao);
    limpar_buffer();
    
    if (opcao == 'S' || opcao == 's') {
        excluir_aula();
    }
}

void menu_principal() {
    int opcao;
    
    while (1) {
        printf("\n=== SISTEMA ESCOLAR ===\n");
        printf("1. Login\n");
        if (admin_logado) {
            printf("2. Criar turma\n");
            printf("3. Cadastrar aluno\n");
            printf("4. Cadastrar professor\n");
            printf("5. Registrar aula\n");
        }
        printf("6. Listar turmas\n");
        printf("7. Listar alunos\n");
        printf("8. Listar professores\n");
        printf("9. Listar aulas\n");
        if (admin_logado) {
            printf("0. Logout\n");
            printf("\n[MODO ADMINISTRADOR ATIVO]\n");
        } else {
            printf("0. Sair\n");
        }
        printf("Escolha uma opcao: ");
        
        scanf("%d", &opcao);
        limpar_buffer();
        
        switch (opcao) {
            case 1:
                fazer_login();
                break;
            case 2:
                criar_turma();
                break;
            case 3:
                cadastrar_aluno();
                break;
            case 4:
                cadastrar_professor();
                break;
            case 5:
                registrar_aula();
                break;
            case 6:
                listar_turmas();
                break;
            case 7:
                listar_alunos();
                break;
            case 8:
                listar_professores();
                break;
            case 9:
                listar_aulas();
                break;
            case 0:
                if (admin_logado) {
                    admin_logado = 0;
                    printf("Logout realizado com sucesso!\n");
                    printf("\nPressione Enter para continuar...");
                    getchar();
                } else {
                    printf("Saindo do sistema...\n");
                    if (alunos) free(alunos);
                    if (professores) free(professores);
                    exit(0);
                }
                break;
            default:
                printf("Opcao invalida!\n");
        }
    }
}

int main() {
    printf("Bem-vindo ao Sistema Escolar!\n");
    printf("Horarios das aulas: 6 aulas por dia, 50 minutos cada\n");
    printf("Aula 1: 07:00-07:50 | Aula 2: 08:00-08:50 | Aula 3: 09:00-09:50\n");
    printf("Aula 4: 10:00-10:50 | Aula 5: 11:00-11:50 | Aula 6: 12:00-12:50\n");
    printf("\n=== CREDENCIAIS ADMINISTRADOR ===\n");
    printf("CPF: %s | Senha: %s\n\n", admin_cpf, admin_senha);
    
    menu_principal();
    
    return 0;
}