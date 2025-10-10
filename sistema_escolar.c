#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

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

void carregar_contadores() {
    FILE *file = fopen("contadores.txt", "r");
    if (file != NULL) {
        fscanf(file, "%d %d %d", &proximo_id_turma, &proximo_id_aula, &proxima_matricula);
        fclose(file);
    }
}

void salvar_contadores() {
    FILE *file = fopen("contadores.txt", "w");
    if (file != NULL) {
        fprintf(file, "%d %d %d", proximo_id_turma, proximo_id_aula, proxima_matricula);
        fclose(file);
    }
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

void criptografar_senha(char *senha, char *senha_criptografada) {
    int chave = 123;
    for (int i = 0; i < strlen(senha); i++) {
        senha_criptografada[i] = senha[i] ^ chave;
    }
    senha_criptografada[strlen(senha)] = '\0';
}

void descriptografar_senha(char *senha_criptografada, char *senha_original) {
    int chave = 123;
    for (int i = 0; i < strlen(senha_criptografada); i++) {
        senha_original[i] = senha_criptografada[i] ^ chave;
    }
    senha_original[strlen(senha_criptografada)] = '\0';
}

int cpf_ja_existe(char *cpf) {
    FILE *file;
    char linha[500];
    char cpf_arquivo[15];
    
    file = fopen("alunos.txt", "r");
    if (file != NULL) {
        while (fgets(linha, sizeof(linha), file)) {
            sscanf(linha, "%*d|%*[^|]|%*[^|]|%*[^|]|%*[^|]|%*c|%s", cpf_arquivo);
            if (strcmp(cpf_arquivo, cpf) == 0) {
                fclose(file);
                return 1;
            }
        }
        fclose(file);
    }
    
    file = fopen("professores.txt", "r");
    if (file != NULL) {
        while (fgets(linha, sizeof(linha), file)) {
            sscanf(linha, "%*d|%*[^|]|%*[^|]|%*[^|]|%*[^|]|%*c|%s", cpf_arquivo);
            if (strcmp(cpf_arquivo, cpf) == 0) {
                fclose(file);
                return 1;
            }
        }
        fclose(file);
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
    
    Turma nova_turma;
    nova_turma.id = proximo_id_turma++;
    
    printf("=== CRIAR TURMA ===\n");
    printf("(Digite '0' em qualquer campo para cancelar)\n\n");
    
    printf("Nome da turma: ");
    fgets(nova_turma.nome, 100, stdin);
    nova_turma.nome[strcspn(nova_turma.nome, "\n")] = 0;
    if (strcmp(nova_turma.nome, "0") == 0) return;
    
    printf("Ano letivo (0 para cancelar): ");
    scanf("%d", &nova_turma.ano);
    if (nova_turma.ano == 0) return;
    
    printf("Serie (1° até o 3° ano) (0 para cancelar): ");
    scanf("%d", &nova_turma.serie);
    limpar_buffer();
    if (nova_turma.serie == 0) return;
    
    int opcao_turno;
    while (1) {
        printf("Selecione o periodo:\n");
        printf("1 - Matutino\n");
        printf("2 - Vespertino\n");
        printf("3 - Noturno\n");
        printf("0 - Cancelar\n");
        printf("Opcao: ");
        scanf("%d", &opcao_turno);
        limpar_buffer();
        
        if (opcao_turno == 0) return;
        
        if (opcao_turno == 1) {
            strcpy(nova_turma.turno, "Matutino");
            break;
        } else if (opcao_turno == 2) {
            strcpy(nova_turma.turno, "Vespertino");
            break;
        } else if (opcao_turno == 3) {
            strcpy(nova_turma.turno, "Noturno");
            break;
        } else {
            printf("Opcao invalida! Digite 1, 2 ou 3.\n");
        }
    }
    
    nova_turma.capacidade_maxima = MAX_ALUNOS_POR_TURMA;
    printf("Capacidade maxima definida automaticamente: %d alunos\n", MAX_ALUNOS_POR_TURMA);
    
    FILE *file = fopen("turmas.txt", "a");
    if (file != NULL) {
        fprintf(file, "%d|%s|%d|%d|%s|%d\n", nova_turma.id, nova_turma.nome, nova_turma.ano, nova_turma.serie, nova_turma.turno, nova_turma.capacidade_maxima);
        fclose(file);
        salvar_contadores();
        printf("Turma criada com sucesso! ID: %d\n", nova_turma.id);
    } else {
        printf("Erro ao salvar turma!\n");
    }
    
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
    
    char senha_criptografada[50];
    criptografar_senha(novo_aluno.senha, senha_criptografada);
    
    FILE *file = fopen("alunos.txt", "a");
    if (file != NULL) {
        fprintf(file, "%d|%s|%s|%s|%s|%c|%s\n", novo_aluno.matricula, novo_aluno.nome, novo_aluno.email, senha_criptografada, novo_aluno.data_nascimento, novo_aluno.genero, novo_aluno.cpf);
        fclose(file);
        salvar_contadores();
        printf("Aluno cadastrado com sucesso!\n");
    } else {
        printf("Erro ao salvar aluno!\n");
    }
    
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
    
    char senha_criptografada[50];
    criptografar_senha(novo_professor.senha, senha_criptografada);
    
    FILE *file = fopen("professores.txt", "a");
    if (file != NULL) {
        fprintf(file, "%d|%s|%s|%s|%s|%c|%s|%s\n", novo_professor.matricula, novo_professor.nome, novo_professor.email, senha_criptografada, novo_professor.data_nascimento, novo_professor.genero, novo_professor.cpf, novo_professor.materia);
        fclose(file);
        salvar_contadores();
        printf("Professor cadastrado com sucesso!\n");
    } else {
        printf("Erro ao salvar professor!\n");
    }
    
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
    
    Aula nova_aula;
    nova_aula.id = proximo_id_aula++;
    
    printf("=== REGISTRAR AULA ===\n");
    printf("(Digite '0' em qualquer campo para cancelar)\n");
    printf("A materia sera definida automaticamente pelo professor\n\n");
    
    // Listar professores disponíveis
    FILE *prof_file = fopen("professores.txt", "r");
    if (prof_file == NULL) {
        printf("Nenhum professor cadastrado!\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    printf("=== PROFESSORES DISPONIVEIS ===\n");
    char linha_prof[500];
    int encontrou_prof = 0;
    
    while (fgets(linha_prof, sizeof(linha_prof), prof_file)) {
        int matricula;
        char nome[100], materia[50];
        
        sscanf(linha_prof, "%d|%[^|]|%*[^|]|%*[^|]|%*[^|]|%*c|%*[^|]|%s", &matricula, nome, materia);
        printf("%d - %s (%s)\n", matricula, nome, materia);
        encontrou_prof = 1;
    }
    fclose(prof_file);
    
    if (!encontrou_prof) {
        printf("Nenhum professor cadastrado!\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    printf("\nDigite a matricula do professor (0 para cancelar): ");
    scanf("%d", &nova_aula.professor_matricula);
    if (nova_aula.professor_matricula == 0) return;
    
    // Verificar se professor existe
    int professor_existe = 0;
    prof_file = fopen("professores.txt", "r");
    if (prof_file != NULL) {
        while (fgets(linha_prof, sizeof(linha_prof), prof_file)) {
            int mat_arquivo;
            sscanf(linha_prof, "%d|", &mat_arquivo);
            if (mat_arquivo == nova_aula.professor_matricula) {
                professor_existe = 1;
                break;
            }
        }
        fclose(prof_file);
    }
    
    if (!professor_existe) {
        printf("Professor nao encontrado!\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    // Listar turmas disponíveis
    FILE *turma_file = fopen("turmas.txt", "r");
    if (turma_file == NULL) {
        printf("Nenhuma turma cadastrada!\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    printf("\n=== TURMAS DISPONIVEIS ===\n");
    char linha_turma[500];
    int encontrou_turma = 0;
    
    while (fgets(linha_turma, sizeof(linha_turma), turma_file)) {
        int id, ano, serie;
        char nome[100], turno[20];
        
        sscanf(linha_turma, "%d|%[^|]|%d|%d|%[^|]|%*d", &id, nome, &ano, &serie, turno);
        printf("%d - %s (%dº ano, %dª série, %s)\n", id, nome, ano, serie, turno);
        encontrou_turma = 1;
    }
    fclose(turma_file);
    
    if (!encontrou_turma) {
        printf("Nenhuma turma cadastrada!\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    printf("\nDigite o ID da turma (0 para cancelar): ");
    scanf("%d", &nova_aula.turma_id);
    limpar_buffer();
    if (nova_aula.turma_id == 0) return;
    
    // Verificar se turma existe
    int turma_existe = 0;
    turma_file = fopen("turmas.txt", "r");
    if (turma_file != NULL) {
        while (fgets(linha_turma, sizeof(linha_turma), turma_file)) {
            int id_arquivo;
            sscanf(linha_turma, "%d|", &id_arquivo);
            if (id_arquivo == nova_aula.turma_id) {
                turma_existe = 1;
                break;
            }
        }
        fclose(turma_file);
    }
    
    if (!turma_existe) {
        printf("Turma nao encontrada!\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    printf("Dia da aula: ");
    fgets(nova_aula.dia, 15, stdin);
    nova_aula.dia[strcspn(nova_aula.dia, "\n")] = 0;
    if (strcmp(nova_aula.dia, "0") == 0) return;
    
    printf("Hora da aula (HH:MM): ");
    fgets(nova_aula.hora, 10, stdin);
    nova_aula.hora[strcspn(nova_aula.hora, "\n")] = 0;
    if (strcmp(nova_aula.hora, "0") == 0) return;
    
    // Buscar matéria do professor selecionado
    prof_file = fopen("professores.txt", "r");
    if (prof_file != NULL) {
        while (fgets(linha_prof, sizeof(linha_prof), prof_file)) {
            int mat_arquivo;
            char materia_prof[50];
            sscanf(linha_prof, "%d|%*[^|]|%*[^|]|%*[^|]|%*[^|]|%*c|%*[^|]|%s", &mat_arquivo, materia_prof);
            if (mat_arquivo == nova_aula.professor_matricula) {
                strcpy(nova_aula.materia, materia_prof);
                break;
            }
        }
        fclose(prof_file);
    }
    
    printf("Materia definida automaticamente: %s\n", nova_aula.materia);
    
    FILE *file = fopen("aulas.txt", "a");
    if (file != NULL) {
        fprintf(file, "%d|%d|%d|%s|%s|%s\n", nova_aula.id, nova_aula.professor_matricula, nova_aula.turma_id, nova_aula.dia, nova_aula.hora, nova_aula.materia);
        fclose(file);
        salvar_contadores();
        printf("Aula registrada com sucesso! ID: %d\n", nova_aula.id);
    } else {
        printf("Erro ao salvar aula!\n");
    }
    
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
    
    FILE *file = fopen("alunos.txt", "r");
    if (file != NULL) {
        char linha[500];
        while (fgets(linha, sizeof(linha), file)) {
            char nome[100], cpf_arquivo[15], senha_criptografada[50];
            sscanf(linha, "%*d|%[^|]|%*[^|]|%[^|]|%*[^|]|%*c|%s", nome, senha_criptografada, cpf_arquivo);
            
            char senha_descriptografada[50];
            descriptografar_senha(senha_criptografada, senha_descriptografada);
            
            if (strcmp(cpf_arquivo, cpf) == 0 && strcmp(senha_descriptografada, senha) == 0) {
                admin_logado = 0;
                printf("Login realizado com sucesso! Bem-vindo, %s\n", nome);
                fclose(file);
                printf("\nPressione Enter para voltar ao menu...");
                getchar();
                return 1;
            }
        }
        fclose(file);
    }
    
    file = fopen("professores.txt", "r");
    if (file != NULL) {
        char linha[500];
        while (fgets(linha, sizeof(linha), file)) {
            char nome[100], cpf_arquivo[15], senha_criptografada[50];
            sscanf(linha, "%*d|%[^|]|%*[^|]|%[^|]|%*[^|]|%*c|%s", nome, senha_criptografada, cpf_arquivo);
            
            char senha_descriptografada[50];
            descriptografar_senha(senha_criptografada, senha_descriptografada);
            
            if (strcmp(cpf_arquivo, cpf) == 0 && strcmp(senha_descriptografada, senha) == 0) {
                admin_logado = 0;
                printf("Login realizado com sucesso! Bem-vindo, %s\n", nome);
                fclose(file);
                printf("\nPressione Enter para voltar ao menu...");
                getchar();
                return 1;
            }
        }
        fclose(file);
    }
    
    printf("CPF ou senha incorretos!\n");
    printf("\nPressione Enter para voltar ao menu...");
    getchar();
    return 0;
}

void excluir_aluno() {
    int matricula;
    printf("Digite a matricula do aluno a ser excluido (0 para cancelar): ");
    scanf("%d", &matricula);
    limpar_buffer();
    if (matricula == 0) return;
    
    FILE *file = fopen("alunos.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    
    if (file == NULL || temp == NULL) {
        printf("Erro ao abrir arquivos!\n");
        return;
    }
    
    char linha[500];
    int encontrou = 0;
    
    while (fgets(linha, sizeof(linha), file)) {
        int mat_arquivo;
        sscanf(linha, "%d|", &mat_arquivo);
        if (mat_arquivo != matricula) {
            fprintf(temp, "%s", linha);
        } else {
            encontrou = 1;
        }
    }
    
    fclose(file);
    fclose(temp);
    
    remove("alunos.txt");
    rename("temp.txt", "alunos.txt");
    
    if (encontrou) {
        printf("Aluno excluido com sucesso!\n");
    } else {
        printf("Aluno nao encontrado!\n");
    }
    
    printf("\nPressione Enter para voltar ao menu...");
    getchar();
}

void listar_alunos() {
    FILE *file = fopen("alunos.txt", "r");
    if (file == NULL) {
        printf("Nenhum aluno cadastrado.\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    printf("\n=== ALUNOS CADASTRADOS ===\n");
    char linha[500];
    int encontrou = 0;
    
    while (fgets(linha, sizeof(linha), file)) {
        int matricula;
        char nome[100], email[100], cpf[15], data[15];
        char genero;
        
        sscanf(linha, "%d|%[^|]|%[^|]|%*[^|]|%[^|]|%c|%s", &matricula, nome, email, data, &genero, cpf);
        
        char cpf_formatado[15];
        formatar_cpf(cpf, cpf_formatado);
        printf("Matricula: %d | Nome: %s | Email: %s | CPF: %s | Nascimento: %s | Genero: %c\n",
               matricula, nome, email, cpf_formatado, data, genero);
        encontrou = 1;
    }
    fclose(file);
    
    if (!encontrou) {
        printf("Nenhum aluno cadastrado.\n");
    } else {
        char opcao;
        printf("\nDeseja excluir algum aluno? (S/N): ");
        scanf(" %c", &opcao);
        limpar_buffer();
        
        if (opcao == 'S' || opcao == 's') {
            excluir_aluno();
            return;
        }
    }
    
    printf("\nPressione Enter para voltar ao menu...");
    getchar();
}

void excluir_professor() {
    int matricula;
    printf("Digite a matricula do professor a ser excluido (0 para cancelar): ");
    scanf("%d", &matricula);
    limpar_buffer();
    if (matricula == 0) return;
    
    FILE *file = fopen("professores.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    
    if (file == NULL || temp == NULL) {
        printf("Erro ao abrir arquivos!\n");
        return;
    }
    
    char linha[500];
    int encontrou = 0;
    
    while (fgets(linha, sizeof(linha), file)) {
        int mat_arquivo;
        sscanf(linha, "%d|", &mat_arquivo);
        if (mat_arquivo != matricula) {
            fprintf(temp, "%s", linha);
        } else {
            encontrou = 1;
        }
    }
    
    fclose(file);
    fclose(temp);
    
    remove("professores.txt");
    rename("temp.txt", "professores.txt");
    
    if (encontrou) {
        printf("Professor excluido com sucesso!\n");
    } else {
        printf("Professor nao encontrado!\n");
    }
    
    printf("\nPressione Enter para voltar ao menu...");
    getchar();
}

void excluir_turma() {
    int id;
    printf("Digite o ID da turma a ser excluida (0 para cancelar): ");
    scanf("%d", &id);
    limpar_buffer();
    if (id == 0) return;
    
    FILE *file = fopen("turmas.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    
    if (file == NULL || temp == NULL) {
        printf("Erro ao abrir arquivos!\n");
        return;
    }
    
    char linha[500];
    int encontrou = 0;
    
    while (fgets(linha, sizeof(linha), file)) {
        int id_arquivo;
        sscanf(linha, "%d|", &id_arquivo);
        if (id_arquivo != id) {
            fprintf(temp, "%s", linha);
        } else {
            encontrou = 1;
        }
    }
    
    fclose(file);
    fclose(temp);
    
    remove("turmas.txt");
    rename("temp.txt", "turmas.txt");
    
    if (encontrou) {
        printf("Turma excluida com sucesso!\n");
    } else {
        printf("Turma nao encontrada!\n");
    }
    
    printf("\nPressione Enter para voltar ao menu...");
    getchar();
}

void excluir_aula() {
    int id;
    printf("Digite o ID da aula a ser excluida (0 para cancelar): ");
    scanf("%d", &id);
    limpar_buffer();
    if (id == 0) return;
    
    FILE *file = fopen("aulas.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    
    if (file == NULL || temp == NULL) {
        printf("Erro ao abrir arquivos!\n");
        return;
    }
    
    char linha[500];
    int encontrou = 0;
    
    while (fgets(linha, sizeof(linha), file)) {
        int id_arquivo;
        sscanf(linha, "%d|", &id_arquivo);
        if (id_arquivo != id) {
            fprintf(temp, "%s", linha);
        } else {
            encontrou = 1;
        }
    }
    
    fclose(file);
    fclose(temp);
    
    remove("aulas.txt");
    rename("temp.txt", "aulas.txt");
    
    if (encontrou) {
        printf("Aula excluida com sucesso!\n");
    } else {
        printf("Aula nao encontrada!\n");
    }
    
    printf("\nPressione Enter para voltar ao menu...");
    getchar();
}

void listar_professores() {
    FILE *file = fopen("professores.txt", "r");
    if (file == NULL) {
        printf("Nenhum professor cadastrado.\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    printf("\n=== PROFESSORES CADASTRADOS ===\n");
    char linha[500];
    int encontrou = 0;
    
    while (fgets(linha, sizeof(linha), file)) {
        int matricula;
        char nome[100], email[100], cpf[15], materia[50];
        char genero;
        
        sscanf(linha, "%d|%[^|]|%[^|]|%*[^|]|%*[^|]|%c|%[^|]|%s", &matricula, nome, email, &genero, cpf, materia);
        
        char cpf_formatado[15];
        formatar_cpf(cpf, cpf_formatado);
        printf("Matricula: %d | Nome: %s | Email: %s | CPF: %s | Genero: %c | Materia: %s\n",
               matricula, nome, email, cpf_formatado, genero, materia);
        encontrou = 1;
    }
    fclose(file);
    
    if (!encontrou) {
        printf("Nenhum professor cadastrado.\n");
    } else {
        char opcao;
        printf("\nDeseja excluir algum professor? (S/N): ");
        scanf(" %c", &opcao);
        limpar_buffer();
        
        if (opcao == 'S' || opcao == 's') {
            excluir_professor();
            return;
        }
    }
    
    printf("\nPressione Enter para voltar ao menu...");
    getchar();
}

void listar_turmas() {
    FILE *file = fopen("turmas.txt", "r");
    if (file == NULL) {
        printf("Nenhuma turma cadastrada.\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    printf("\n=== TURMAS CADASTRADAS ===\n");
    char linha[500];
    int encontrou = 0;
    
    while (fgets(linha, sizeof(linha), file)) {
        int id, ano, serie, capacidade;
        char nome[100], turno[20];
        
        sscanf(linha, "%d|%[^|]|%d|%d|%[^|]|%d", &id, nome, &ano, &serie, turno, &capacidade);
        
        printf("ID: %d | Nome: %s | Ano: %d | Serie: %d | Turno: %s | Capacidade: %d\n",
               id, nome, ano, serie, turno, capacidade);
        encontrou = 1;
    }
    fclose(file);
    
    if (!encontrou) {
        printf("Nenhuma turma cadastrada.\n");
    } else {
        char opcao;
        printf("\nDeseja excluir alguma turma? (S/N): ");
        scanf(" %c", &opcao);
        limpar_buffer();
        
        if (opcao == 'S' || opcao == 's') {
            excluir_turma();
            return;
        }
    }
    
    printf("\nPressione Enter para voltar ao menu...");
    getchar();
}

void consultar_horarios() {
    printf("\n=== HORARIOS DE FUNCIONAMENTO ===\n");
    printf("MATUTINO: 07h00-11h40 | VESPERTINO: 13h00-17h40 | NOTURNO: 18h00-22h40\n");
    printf("\nMATUTINO:\n");
    printf("1a Aula: 07h00-07h50 | 2a Aula: 07h50-08h40 | 3a Aula: 08h40-09h30\n");
    printf("Intervalo: 09h30-10h00 (30min)\n");
    printf("4a Aula: 10h00-10h50 | 5a Aula: 10h50-11h40\n");
    printf("\nVESPERTINO:\n");
    printf("1a Aula: 13h00-13h50 | 2a Aula: 13h50-14h40 | 3a Aula: 14h40-15h30\n");
    printf("Intervalo: 15h30-16h00 (30min)\n");
    printf("4a Aula: 16h00-16h50 | 5a Aula: 16h50-17h40\n");
    printf("\nNOTURNO:\n");
    printf("1a Aula: 18h00-18h50 | 2a Aula: 18h50-19h40 | 3a Aula: 19h40-20h30\n");
    printf("Intervalo: 20h30-21h00 (30min)\n");
    printf("4a Aula: 21h00-21h50 | 5a Aula: 21h50-22h40\n");
    
    printf("\nPressione Enter para voltar ao menu...");
    getchar();
}

void listar_aulas() {
    FILE *file = fopen("aulas.txt", "r");
    if (file == NULL) {
        printf("Nenhuma aula registrada.\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    printf("\n=== AULAS REGISTRADAS ===\n");
    char linha[500];
    int encontrou = 0;
    
    while (fgets(linha, sizeof(linha), file)) {
        int id, prof_matricula, turma_id;
        char dia[15], hora[10], materia[50];
        
        sscanf(linha, "%d|%d|%d|%[^|]|%[^|]|%s", &id, &prof_matricula, &turma_id, dia, hora, materia);
        
        printf("ID: %d | Professor: %d | Turma: %d | Dia: %s | Hora: %s | Materia: %s\n",
               id, prof_matricula, turma_id, dia, hora, materia);
        encontrou = 1;
    }
    fclose(file);
    
    if (!encontrou) {
        printf("Nenhuma aula registrada.\n");
    } else {
        char opcao;
        printf("\nDeseja excluir alguma aula? (S/N): ");
        scanf(" %c", &opcao);
        limpar_buffer();
        
        if (opcao == 'S' || opcao == 's') {
            excluir_aula();
            return;
        }
    }
    
    printf("\nPressione Enter para voltar ao menu...");
    getchar();
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
        printf("10. Consultar horario de funcionamento\n");
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
            case 10:
                consultar_horarios();
                break;
            case 0:
                if (admin_logado) {
                    admin_logado = 0;
                    printf("Logout realizado com sucesso!\n");
                    printf("\nPressione Enter para continuar...");
                    getchar();
                } else {
                    printf("Saindo do sistema...\n");
                    exit(0);
                }
                break;
            default:
                printf("Opcao invalida!\n");
        }
    }
}

int main() {
    setlocale(LC_ALL, "Portuguese");
    
    carregar_contadores();
    
    printf("Bem-vindo ao Sistema Escolar!\n");
    printf("\n=== CREDENCIAIS ADMINISTRADOR ===\n");
    printf("CPF: %s | Senha: %s\n\n", admin_cpf, admin_senha);
    
    menu_principal();
    
    return 0;
}