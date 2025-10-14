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

int turma_ja_existe(int serie, char letra, int ano, char *turno) {
    FILE *file = fopen("turmas.txt", "r");
    if (file == NULL) return 0;
    
    char linha[500];
    while (fgets(linha, sizeof(linha), file)) {
        int ano_arquivo, serie_arquivo;
        char nome_arquivo[100], turno_arquivo[20];
        
        sscanf(linha, "%*d|%[^|]|%d|%d|%[^|]|%*d", nome_arquivo, &ano_arquivo, &serie_arquivo, turno_arquivo);
        
        char letra_arquivo = nome_arquivo[strlen(nome_arquivo) - 1];
        
        if (serie_arquivo == serie && letra_arquivo == letra && ano_arquivo == ano && strcmp(turno_arquivo, turno) == 0) {
            fclose(file);
            return 1;
        }
    }
    
    fclose(file);
    return 0;
}

int aluno_ja_tem_turma(int matricula) {
    FILE *file = fopen("aluno_turma.txt", "r");
    if (file == NULL) return 0;
    
    char linha[100];
    while (fgets(linha, sizeof(linha), file)) {
        int mat_arquivo;
        sscanf(linha, "%d|", &mat_arquivo);
        if (mat_arquivo == matricula) {
            fclose(file);
            return 1;
        }
    }
    
    fclose(file);
    return 0;
}

void incluir_aluno_turma(int matricula, int turma_id) {
    FILE *file = fopen("aluno_turma.txt", "a");
    if (file != NULL) {
        fprintf(file, "%d|%d\n", matricula, turma_id);
        fclose(file);
    }
}

void remover_aluno_turma(int matricula) {
    FILE *file = fopen("aluno_turma.txt", "r");
    FILE *temp = fopen("temp_aluno_turma.txt", "w");
    
    if (file == NULL || temp == NULL) return;
    
    char linha[100];
    while (fgets(linha, sizeof(linha), file)) {
        int mat_arquivo;
        sscanf(linha, "%d|", &mat_arquivo);
        if (mat_arquivo != matricula) {
            fprintf(temp, "%s", linha);
        }
    }
    
    fclose(file);
    fclose(temp);
    
    remove("aluno_turma.txt");
    rename("temp_aluno_turma.txt", "aluno_turma.txt");
}

void editar_turmas() {
    if (!admin_logado) {
        printf("Acesso negado! Apenas administradores podem editar turmas.\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    FILE *file = fopen("turmas.txt", "r");
    if (file == NULL) {
        printf("Nenhuma turma cadastrada.\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    printf("\n=== TURMAS DISPONIVEIS ===\n");
    char linha[500];
    int encontrou = 0;
    
    while (fgets(linha, sizeof(linha), file)) {
        int id, ano, serie, capacidade;
        char nome[100], turno[20];
        
        sscanf(linha, "%d|%[^|]|%d|%d|%[^|]|%d", &id, nome, &ano, &serie, turno, &capacidade);
        
        printf("ID: %d | Nome: %s | Ano: %d | Serie: %d | Turno: %s\n",
               id, nome, ano, serie, turno);
        encontrou = 1;
    }
    fclose(file);
    
    if (!encontrou) {
        printf("Nenhuma turma cadastrada.\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    int turma_id;
    printf("\nDigite o ID da turma que deseja editar (0 para cancelar): ");
    scanf("%d", &turma_id);
    limpar_buffer();
    if (turma_id == 0) return;
    
    // Buscar e exibir informações da turma selecionada
    FILE *turma_file = fopen("turmas.txt", "r");
    char turma_nome[100], turma_turno[20];
    int turma_ano = 0, turma_encontrada = 0;
    
    if (turma_file != NULL) {
        char linha_turma[500];
        while (fgets(linha_turma, sizeof(linha_turma), turma_file)) {
            int id, ano, serie, capacidade;
            char nome[100], turno[20];
            
            sscanf(linha_turma, "%d|%[^|]|%d|%d|%[^|]|%d", &id, nome, &ano, &serie, turno, &capacidade);
            
            if (id == turma_id) {
                strcpy(turma_nome, nome);
                strcpy(turma_turno, turno);
                turma_ano = ano;
                turma_encontrada = 1;
                break;
            }
        }
        fclose(turma_file);
    }
    
    if (!turma_encontrada) {
        printf("Turma nao encontrada!\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    printf("\nTurma selecionada: %s | Ano: %d | Turno: %s\n", turma_nome, turma_ano, turma_turno);
    
    int opcao;
    printf("\n=== EDITAR TURMA ===\n");
    printf("1. Incluir aluno\n");
    printf("2. Remover aluno\n");
    printf("0. Cancelar\n");
    printf("Escolha uma opcao: ");
    scanf("%d", &opcao);
    limpar_buffer();
    
    if (opcao == 1) {
        // Incluir aluno
        FILE *aluno_file = fopen("alunos.txt", "r");
        if (aluno_file == NULL) {
            printf("Nenhum aluno cadastrado.\n");
            printf("\nPressione Enter para voltar ao menu...");
            getchar();
            return;
        }
        
        printf("\n=== ALUNOS DISPONIVEIS ===\n");
        char linha_aluno[500];
        int encontrou_aluno = 0;
        
        while (fgets(linha_aluno, sizeof(linha_aluno), aluno_file)) {
            int matricula;
            char nome[100];
            
            sscanf(linha_aluno, "%d|%[^|]|", &matricula, nome);
            
            if (!aluno_ja_tem_turma(matricula)) {
                printf("Matricula: %d | Nome: %s\n", matricula, nome);
                encontrou_aluno = 1;
            }
        }
        fclose(aluno_file);
        
        if (!encontrou_aluno) {
            printf("Nenhum aluno disponivel para inclusao.\n");
            printf("\nPressione Enter para voltar ao menu...");
            getchar();
            return;
        }
        
        char input[200];
        printf("\nDigite as matriculas dos alunos separadas por virgula, (0 para cancelar): ");
        fgets(input, 200, stdin);
        input[strcspn(input, "\n")] = 0;
        if (strcmp(input, "0") == 0) return;
        
        int matriculas[50], count = 0;
        char *token = strtok(input, ",");
        while (token != NULL && count < 50) {
            matriculas[count++] = atoi(token);
            token = strtok(NULL, ",");
        }
        
        int incluidos = 0;
        for (int i = 0; i < count; i++) {
            int matricula_aluno = matriculas[i];
            
            if (aluno_ja_tem_turma(matricula_aluno)) {
                printf("Matricula %d ja esta em uma turma!\n", matricula_aluno);
                continue;
            }
            
            // Verificar se aluno existe
            FILE *check_file = fopen("alunos.txt", "r");
            int aluno_existe = 0;
            char nome_aluno[100] = "";
            if (check_file != NULL) {
                char linha_check[500];
                while (fgets(linha_check, sizeof(linha_check), check_file)) {
                    int mat_check;
                    char nome_temp[100];
                    sscanf(linha_check, "%d|%[^|]|", &mat_check, nome_temp);
                    if (mat_check == matricula_aluno) {
                        aluno_existe = 1;
                        strcpy(nome_aluno, nome_temp);
                        break;
                    }
                }
                fclose(check_file);
            }
            
            if (aluno_existe) {
                incluir_aluno_turma(matricula_aluno, turma_id);
                printf("Aluno %s incluido com sucesso!\n", nome_aluno);
                incluidos++;
            } else {
                printf("Matricula %d nao encontrada!\n", matricula_aluno);
            }
        }
        
        if (incluidos > 0) {
            printf("\n%d aluno(s) incluido(s) na turma!\n", incluidos);
        }
        
    } else if (opcao == 2) {
        // Remover aluno
        FILE *at_file = fopen("aluno_turma.txt", "r");
        if (at_file == NULL) {
            printf("Nenhum aluno registrado em turmas.\n");
            printf("\nPressione Enter para voltar ao menu...");
            getchar();
            return;
        }
        
        printf("\n=== ALUNOS NA TURMA ===\n");
        char linha_at[100];
        int encontrou_na_turma = 0;
        
        while (fgets(linha_at, sizeof(linha_at), at_file)) {
            int mat_at, turma_at;
            sscanf(linha_at, "%d|%d", &mat_at, &turma_at);
            
            if (turma_at == turma_id) {
                // Buscar nome do aluno
                FILE *aluno_file = fopen("alunos.txt", "r");
                if (aluno_file != NULL) {
                    char linha_aluno[500];
                    while (fgets(linha_aluno, sizeof(linha_aluno), aluno_file)) {
                        int mat_aluno;
                        char nome_aluno[100];
                        sscanf(linha_aluno, "%d|%[^|]|", &mat_aluno, nome_aluno);
                        if (mat_aluno == mat_at) {
                            printf("Matricula: %d | Nome: %s\n", mat_at, nome_aluno);
                            encontrou_na_turma = 1;
                            break;
                        }
                    }
                    fclose(aluno_file);
                }
            }
        }
        fclose(at_file);
        
        if (!encontrou_na_turma) {
            printf("Nenhum aluno registrado nesta turma.\n");
            printf("\nPressione Enter para voltar ao menu...");
            getchar();
            return;
        }
        
        char input[200];
        printf("\nDigite as matriculas dos alunos a serem removidos separadas por virgula, (0 para cancelar): ");
        fgets(input, 200, stdin);
        input[strcspn(input, "\n")] = 0;
        if (strcmp(input, "0") == 0) return;
        
        int matriculas[50], count = 0;
        char *token = strtok(input, ",");
        while (token != NULL && count < 50) {
            matriculas[count++] = atoi(token);
            token = strtok(NULL, ",");
        }
        
        int removidos = 0;
        for (int i = 0; i < count; i++) {
            int matricula_aluno = matriculas[i];
            
            // Verificar se aluno esta na turma
            FILE *check_file = fopen("aluno_turma.txt", "r");
            int aluno_na_turma = 0;
            if (check_file != NULL) {
                char linha_check[100];
                while (fgets(linha_check, sizeof(linha_check), check_file)) {
                    int mat_check, turma_check;
                    sscanf(linha_check, "%d|%d", &mat_check, &turma_check);
                    if (mat_check == matricula_aluno && turma_check == turma_id) {
                        aluno_na_turma = 1;
                        break;
                    }
                }
                fclose(check_file);
            }
            
            if (aluno_na_turma) {
                // Buscar nome do aluno
                char nome_aluno[100] = "";
                FILE *nome_file = fopen("alunos.txt", "r");
                if (nome_file != NULL) {
                    char linha_nome[500];
                    while (fgets(linha_nome, sizeof(linha_nome), nome_file)) {
                        int mat_nome;
                        char nome_temp[100];
                        sscanf(linha_nome, "%d|%[^|]|", &mat_nome, nome_temp);
                        if (mat_nome == matricula_aluno) {
                            strcpy(nome_aluno, nome_temp);
                            break;
                        }
                    }
                    fclose(nome_file);
                }
                
                remover_aluno_turma(matricula_aluno);
                printf("Aluno %s removido com sucesso!\n", nome_aluno);
                removidos++;
            } else {
                printf("Matricula %d nao existe ou nao esta nesta turma!\n", matricula_aluno);
            }
        }
        
        if (removidos > 0) {
            printf("\n%d aluno(s) removido(s) da turma!\n", removidos);
        }
    }
    
    printf("\nPressione Enter para voltar ao menu...");
    getchar();
}

void criar_turma() {
    if (!admin_logado) {
        printf("Acesso negado! Apenas administradores podem criar turmas.\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    Turma nova_turma;
    
    printf("=== CRIAR TURMA ===\n");
    printf("(Digite '0' em qualquer campo para cancelar)\n\n");
    
    // Selecionar série
    int opcao_serie;
    while (1) {
        printf("Selecione a serie:\n");
        printf("1 - 1 Ano\n");
        printf("2 - 2 Ano\n");
        printf("3 - 3 Ano\n");
        printf("0 - Cancelar\n");
        printf("Opcao: ");
        scanf("%d", &opcao_serie);
        limpar_buffer();
        
        if (opcao_serie == 0) return;
        
        if (opcao_serie >= 1 && opcao_serie <= 3) {
            nova_turma.serie = opcao_serie;
            break;
        } else {
            printf("Opcao invalida! Digite 1, 2 ou 3.\n");
        }
    }
    
    // Selecionar letra da turma
    char letra_turma;
    while (1) {
        printf("\nSelecione a letra da turma (A-Z): ");
        scanf(" %c", &letra_turma);
        limpar_buffer();
        
        if (letra_turma == '0') return;
        
        if ((letra_turma >= 'A' && letra_turma <= 'Z') || (letra_turma >= 'a' && letra_turma <= 'z')) {
            if (letra_turma >= 'a' && letra_turma <= 'z') {
                letra_turma = letra_turma - 'a' + 'A'; // Converter para maiúscula
            }
            break;
        } else {
            printf("Letra invalida! Digite uma letra de A a Z.\n");
        }
    }
    
    // Selecionar ano letivo
    int opcao_ano;
    while (1) {
        printf("\nSelecione o ano letivo:\n");
        printf("1 - 2025\n");
        printf("2 - 2026\n");
        printf("0 - Cancelar\n");
        printf("Opcao: ");
        scanf("%d", &opcao_ano);
        limpar_buffer();
        
        if (opcao_ano == 0) return;
        
        if (opcao_ano == 1) {
            nova_turma.ano = 2025;
            break;
        } else if (opcao_ano == 2) {
            nova_turma.ano = 2026;
            break;
        } else {
            printf("Opcao invalida! Digite 1 ou 2.\n");
        }
    }
    
    // Gerar nome da turma automaticamente
    sprintf(nova_turma.nome, "%d Ano %c", nova_turma.serie, letra_turma);
    
    int opcao_turno;
    while (1) {
        printf("\nSelecione o periodo:\n");
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
    
    // Verificar se turma já existe
    if (turma_ja_existe(nova_turma.serie, letra_turma, nova_turma.ano, nova_turma.turno)) {
        printf("Erro: Ja existe uma turma %s no ano %d no periodo %s!\n", nova_turma.nome, nova_turma.ano, nova_turma.turno);
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    // Só incrementa o ID se a turma for válida
    nova_turma.id = proximo_id_turma++;
    nova_turma.capacidade_maxima = MAX_ALUNOS_POR_TURMA;
    
    FILE *file = fopen("turmas.txt", "a");
    if (file != NULL) {
        fprintf(file, "%d|%s|%d|%d|%s|%d\n", nova_turma.id, nova_turma.nome, nova_turma.ano, nova_turma.serie, nova_turma.turno, nova_turma.capacidade_maxima);
        fclose(file);
        salvar_contadores();
        printf("Turma criada com sucesso! O nome da turma e: %s.\n", nova_turma.nome);
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
    
    FILE *file = fopen("alunos.txt", "a");
    if (file != NULL) {
        fprintf(file, "%d|%s|%s|%s|%s|%c|%s\n", novo_aluno.matricula, novo_aluno.nome, novo_aluno.email, novo_aluno.senha, novo_aluno.data_nascimento, novo_aluno.genero, novo_aluno.cpf);
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
    
    char materias[200] = "";
    int primeira_materia = 1;
    
    while (1) {
        int opcao_materia;
        printf("\nSelecione a materia:\n");
        printf("1 - Matematica\n");
        printf("2 - Portugues\n");
        printf("3 - Ciencias\n");
        printf("4 - Geografia\n");
        printf("5 - Historia\n");
        printf("6 - Ingles\n");
        printf("7 - Filosofia\n");
        printf("8 - Sociologia\n");
        printf("0 - Cancelar\n");
        printf("Opcao: ");
        scanf("%d", &opcao_materia);
        limpar_buffer();
        
        if (opcao_materia == 0) return;
        
        char materia_selecionada[20];
        switch (opcao_materia) {
            case 1: strcpy(materia_selecionada, "Matematica"); break;
            case 2: strcpy(materia_selecionada, "Portugues"); break;
            case 3: strcpy(materia_selecionada, "Ciencias"); break;
            case 4: strcpy(materia_selecionada, "Geografia"); break;
            case 5: strcpy(materia_selecionada, "Historia"); break;
            case 6: strcpy(materia_selecionada, "Ingles"); break;
            case 7: strcpy(materia_selecionada, "Filosofia"); break;
            case 8: strcpy(materia_selecionada, "Sociologia"); break;
            default:
                printf("Opcao invalida!\n");
                continue;
        }
        
        if (primeira_materia) {
            strcpy(materias, materia_selecionada);
            primeira_materia = 0;
        } else {
            strcat(materias, ", ");
            strcat(materias, materia_selecionada);
        }
        
        printf("Materia adicionada: %s\n", materia_selecionada);
        printf("Materias selecionadas: %s\n", materias);
        
        char continuar;
        while (1) {
            printf("\nDeseja selecionar mais uma materia? (S/N): ");
            scanf(" %c", &continuar);
            limpar_buffer();
            
            if (continuar == 'S' || continuar == 's' || continuar == 'N' || continuar == 'n') {
                break;
            }
            printf("Opcao invalida! Digite S para Sim ou N para Nao.\n");
        }
        
        if (continuar != 'S' && continuar != 's') {
            break;
        }
    }
    
    strcpy(novo_professor.materia, materias);
    
    FILE *file = fopen("professores.txt", "a");
    if (file != NULL) {
        fprintf(file, "%d|%s|%s|%s|%s|%c|%s|%s\n", novo_professor.matricula, novo_professor.nome, novo_professor.email, novo_professor.senha, novo_professor.data_nascimento, novo_professor.genero, novo_professor.cpf, novo_professor.materia);
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
        
        sscanf(linha_prof, "%d|%[^|]|%*[^|]|%*[^|]|%*[^|]|%*c|%*[^|]|%[^\n]", &matricula, nome, materia);
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
        printf("%d - %s (%do ano, %da serie, %s)\n", id, nome, ano, serie, turno);
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
            sscanf(linha_prof, "%d|%*[^|]|%*[^|]|%*[^|]|%*[^|]|%*c|%*[^|]|%[^\n]", &mat_arquivo, materia_prof);
            if (mat_arquivo == nova_aula.professor_matricula) {
                strcpy(nova_aula.materia, materia_prof);
                break;
            }
        }
        fclose(prof_file);
    }
    
    printf("Materia definida automaticamente: %s\n", nova_aula.materia);
    
    // Só incrementa o ID se a aula for válida
    nova_aula.id = proximo_id_aula++;
    
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
            char nome[100], cpf_arquivo[15], senha_arquivo[50];
            sscanf(linha, "%*d|%[^|]|%*[^|]|%[^|]|%*[^|]|%*c|%s", nome, senha_arquivo, cpf_arquivo);
            
            if (strcmp(cpf_arquivo, cpf) == 0 && strcmp(senha_arquivo, senha) == 0) {
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
            char nome[100], cpf_arquivo[15], senha_arquivo[50];
            sscanf(linha, "%*d|%[^|]|%*[^|]|%[^|]|%*[^|]|%*c|%s", nome, senha_arquivo, cpf_arquivo);
            
            if (strcmp(cpf_arquivo, cpf) == 0 && strcmp(senha_arquivo, senha) == 0) {
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
    if (!admin_logado) {
        printf("Acesso negado! Apenas administradores podem excluir alunos.\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    char input[200];
    printf("Digite as matriculas dos alunos a serem excluidos separadas por virgula, (0 para cancelar): ");
    fgets(input, 200, stdin);
    input[strcspn(input, "\n")] = 0;
    if (strcmp(input, "0") == 0) return;
    
    int matriculas[50], count = 0;
    char *token = strtok(input, ",");
    while (token != NULL && count < 50) {
        matriculas[count++] = atoi(token);
        token = strtok(NULL, ",");
    }
    
    FILE *file = fopen("alunos.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    
    if (file == NULL || temp == NULL) {
        printf("Erro ao abrir arquivos!\n");
        return;
    }
    
    char linha[500];
    int excluidos = 0;
    
    while (fgets(linha, sizeof(linha), file)) {
        int mat_arquivo;
        sscanf(linha, "%d|", &mat_arquivo);
        int deve_excluir = 0;
        
        for (int i = 0; i < count; i++) {
            if (mat_arquivo == matriculas[i]) {
                deve_excluir = 1;
                excluidos++;
                break;
            }
        }
        
        if (!deve_excluir) {
            fprintf(temp, "%s", linha);
        }
    }
    
    fclose(file);
    fclose(temp);
    
    remove("alunos.txt");
    rename("temp.txt", "alunos.txt");
    
    if (excluidos > 0) {
        printf("%d aluno(s) excluido(s) com sucesso!\n", excluidos);
    } else {
        printf("Nenhum aluno encontrado!\n");
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
        char nome[100], email[100], cpf[15], data[15], senha[50];
        char genero;
        
        sscanf(linha, "%d|%[^|]|%[^|]|%[^|]|%[^|]|%c|%s", &matricula, nome, email, senha, data, &genero, cpf);
        
        char cpf_formatado[15];
        formatar_cpf(cpf, cpf_formatado);
        
        if (admin_logado) {
            printf("Matricula: %d | Nome: %s | Email: %s | Senha: %s | CPF: %s | Nascimento: %s | Genero: %c\n",
                   matricula, nome, email, senha, cpf_formatado, data, genero);
        } else {
            printf("Matricula: %d | Nome: %s | Email: %s | CPF: %s | Nascimento: %s | Genero: %c\n",
                   matricula, nome, email, cpf_formatado, data, genero);
        }
        encontrou = 1;
    }
    fclose(file);
    
    if (!encontrou) {
        printf("Nenhum aluno cadastrado.\n");
    }
    
    printf("\nPressione Enter para voltar ao menu...");
    getchar();
}

void excluir_professor() {
    if (!admin_logado) {
        printf("Acesso negado! Apenas administradores podem excluir professores.\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    char input[200];
    printf("Digite as matriculas dos professores a serem excluidos separadas por virgula, (0 para cancelar): ");
    fgets(input, 200, stdin);
    input[strcspn(input, "\n")] = 0;
    if (strcmp(input, "0") == 0) return;
    
    int matriculas[50], count = 0;
    char *token = strtok(input, ",");
    while (token != NULL && count < 50) {
        matriculas[count++] = atoi(token);
        token = strtok(NULL, ",");
    }
    
    FILE *file = fopen("professores.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    
    if (file == NULL || temp == NULL) {
        printf("Erro ao abrir arquivos!\n");
        return;
    }
    
    char linha[500];
    int excluidos = 0;
    
    while (fgets(linha, sizeof(linha), file)) {
        int mat_arquivo;
        sscanf(linha, "%d|", &mat_arquivo);
        int deve_excluir = 0;
        
        for (int i = 0; i < count; i++) {
            if (mat_arquivo == matriculas[i]) {
                deve_excluir = 1;
                excluidos++;
                break;
            }
        }
        
        if (!deve_excluir) {
            fprintf(temp, "%s", linha);
        }
    }
    
    fclose(file);
    fclose(temp);
    
    remove("professores.txt");
    rename("temp.txt", "professores.txt");
    
    if (excluidos > 0) {
        printf("%d professor(es) excluido(s) com sucesso!\n", excluidos);
    } else {
        printf("Nenhum professor encontrado!\n");
    }
    
    printf("\nPressione Enter para voltar ao menu...");
    getchar();
}

void excluir_turma() {
    if (!admin_logado) {
        printf("Acesso negado! Apenas administradores podem excluir turmas.\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    char input[200];
    printf("Digite os IDs das turmas a serem excluidos (separados por virgula) ou digite 0 para cancelar): ");
    fgets(input, 200, stdin);
    input[strcspn(input, "\n")] = 0;
    if (strcmp(input, "0") == 0) return;
    
    int ids[50], count = 0;
    char *token = strtok(input, ",");
    while (token != NULL && count < 50) {
        ids[count++] = atoi(token);
        token = strtok(NULL, ",");
    }
    
    FILE *file = fopen("turmas.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    
    if (file == NULL || temp == NULL) {
        printf("Erro ao abrir arquivos!\n");
        return;
    }
    
    char linha[500];
    int excluidos = 0;
    
    while (fgets(linha, sizeof(linha), file)) {
        int id_arquivo;
        sscanf(linha, "%d|", &id_arquivo);
        int deve_excluir = 0;
        
        for (int i = 0; i < count; i++) {
            if (id_arquivo == ids[i]) {
                deve_excluir = 1;
                excluidos++;
                break;
            }
        }
        
        if (!deve_excluir) {
            fprintf(temp, "%s", linha);
        }
    }
    
    fclose(file);
    fclose(temp);
    
    remove("turmas.txt");
    rename("temp.txt", "turmas.txt");
    
    if (excluidos > 0) {
        printf("%d turma(s) excluida(s) com sucesso!\n", excluidos);
    } else {
        printf("Nenhuma turma encontrada!\n");
    }
    
    printf("\nPressione Enter para voltar ao menu...");
    getchar();
}

void excluir_aula() {
    if (!admin_logado) {
        printf("Acesso negado! Apenas administradores podem excluir aulas.\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    char input[200];
    printf("Digite os IDs das aulas a serem excluidos separados por virgula, (0 para cancelar): ");
    fgets(input, 200, stdin);
    input[strcspn(input, "\n")] = 0;
    if (strcmp(input, "0") == 0) return;
    
    int ids[50], count = 0;
    char *token = strtok(input, ",");
    while (token != NULL && count < 50) {
        ids[count++] = atoi(token);
        token = strtok(NULL, ",");
    }
    
    FILE *file = fopen("aulas.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    
    if (file == NULL || temp == NULL) {
        printf("Erro ao abrir arquivos!\n");
        return;
    }
    
    char linha[500];
    int excluidos = 0;
    
    while (fgets(linha, sizeof(linha), file)) {
        int id_arquivo;
        sscanf(linha, "%d|", &id_arquivo);
        int deve_excluir = 0;
        
        for (int i = 0; i < count; i++) {
            if (id_arquivo == ids[i]) {
                deve_excluir = 1;
                excluidos++;
                break;
            }
        }
        
        if (!deve_excluir) {
            fprintf(temp, "%s", linha);
        }
    }
    
    fclose(file);
    fclose(temp);
    
    remove("aulas.txt");
    rename("temp.txt", "aulas.txt");
    
    if (excluidos > 0) {
        printf("%d aula(s) excluida(s) com sucesso!\n", excluidos);
    } else {
        printf("Nenhuma aula encontrada!\n");
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
        char nome[100], email[100], cpf[15], materia[50], senha[50];
        char genero;
        
        sscanf(linha, "%d|%[^|]|%[^|]|%[^|]|%*[^|]|%c|%[^|]|%[^\n]", &matricula, nome, email, senha, &genero, cpf, materia);
        
        char cpf_formatado[15];
        formatar_cpf(cpf, cpf_formatado);
        
        if (admin_logado) {
            printf("Matricula: %d | Nome: %s | Email: %s | Senha: %s | CPF: %s | Genero: %c | Materia: %s\n",
                   matricula, nome, email, senha, cpf_formatado, genero, materia);
        } else {
            printf("Matricula: %d | Nome: %s | Email: %s | CPF: %s | Genero: %c | Materia: %s\n",
                   matricula, nome, email, cpf_formatado, genero, materia);
        }
        encontrou = 1;
    }
    fclose(file);
    
    if (!encontrou) {
        printf("Nenhum professor cadastrado.\n");
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
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    char opcao;
    while (1) {
        printf("\nDeseja ver detalhes de turmas especificas? (S/N): ");
        scanf(" %c", &opcao);
        limpar_buffer();
        
        if (opcao == 'S' || opcao == 's' || opcao == 'N' || opcao == 'n') {
            break;
        }
        printf("Opcao invalida! Digite S para Sim ou N para Nao.\n");
    }
    
    if (opcao == 'S' || opcao == 's') {
        while (1) {
            int turma_id;
            printf("\nDigite o ID da turma (0 para voltar ao menu): ");
            scanf("%d", &turma_id);
            limpar_buffer();
            if (turma_id == 0) return;
            
            // Buscar informações da turma
            FILE *turma_file = fopen("turmas.txt", "r");
            int turma_encontrada = 0;
            char turma_nome[100], turma_turno[20];
            int turma_ano, turma_serie, turma_capacidade;
            
            if (turma_file != NULL) {
                char linha_turma[500];
                while (fgets(linha_turma, sizeof(linha_turma), turma_file)) {
                    int id, ano, serie, capacidade;
                    char nome[100], turno[20];
                    
                    sscanf(linha_turma, "%d|%[^|]|%d|%d|%[^|]|%d", &id, nome, &ano, &serie, turno, &capacidade);
                    
                    if (id == turma_id) {
                        strcpy(turma_nome, nome);
                        strcpy(turma_turno, turno);
                        turma_ano = ano;
                        turma_serie = serie;
                        turma_capacidade = capacidade;
                        turma_encontrada = 1;
                        break;
                    }
                }
                fclose(turma_file);
            }
            
            if (!turma_encontrada) {
                printf("Turma nao encontrada!\n");
                continue;
            }
            
            // Contar alunos na turma
            int alunos_na_turma = 0;
            FILE *at_file = fopen("aluno_turma.txt", "r");
            if (at_file != NULL) {
                char linha_at[100];
                while (fgets(linha_at, sizeof(linha_at), at_file)) {
                    int mat_at, turma_at;
                    sscanf(linha_at, "%d|%d", &mat_at, &turma_at);
                    if (turma_at == turma_id) {
                        alunos_na_turma++;
                    }
                }
                fclose(at_file);
            }
            
            // Exibir detalhes da turma
            printf("\n=== DETALHES DA TURMA ===\n");
            printf("ID: %d | Nome: %s | Ano: %d | Serie: %d | Turno: %s | Capacidade: (%d/%d)\n",
                   turma_id, turma_nome, turma_ano, turma_serie, turma_turno, alunos_na_turma, turma_capacidade);
            
            // Listar alunos da turma
            if (alunos_na_turma > 0) {
                printf("\n=== ALUNOS REGISTRADOS ===\n");
                FILE *at_file2 = fopen("aluno_turma.txt", "r");
                if (at_file2 != NULL) {
                    char linha_at2[100];
                    while (fgets(linha_at2, sizeof(linha_at2), at_file2)) {
                        int mat_at2, turma_at2;
                        sscanf(linha_at2, "%d|%d", &mat_at2, &turma_at2);
                        
                        if (turma_at2 == turma_id) {
                            // Buscar informações do aluno
                            FILE *aluno_file = fopen("alunos.txt", "r");
                            if (aluno_file != NULL) {
                                char linha_aluno[500];
                                while (fgets(linha_aluno, sizeof(linha_aluno), aluno_file)) {
                                    int matricula;
                                    char nome[100], email[100], cpf[15], data[15], senha[50];
                                    char genero;
                                    
                                    sscanf(linha_aluno, "%d|%[^|]|%[^|]|%[^|]|%[^|]|%c|%s", &matricula, nome, email, senha, data, &genero, cpf);
                                    
                                    if (matricula == mat_at2) {
                                        char cpf_formatado[15];
                                        formatar_cpf(cpf, cpf_formatado);
                                        
                                        if (admin_logado) {
                                            printf("Matricula: %d | Nome: %s | Email: %s | Senha: %s | CPF: %s | Nascimento: %s | Genero: %c\n",
                                                   matricula, nome, email, senha, cpf_formatado, data, genero);
                                        } else {
                                            printf("Matricula: %d | Nome: %s | Email: %s | CPF: %s | Nascimento: %s | Genero: %c\n",
                                                   matricula, nome, email, cpf_formatado, data, genero);
                                        }
                                        break;
                                    }
                                }
                                fclose(aluno_file);
                            }
                        }
                    }
                    fclose(at_file2);
                }
            } else {
                printf("\nNenhum aluno registrado nesta turma.\n");
            }
            
            char continuar;
            while (1) {
                printf("\nDeseja ver detalhes de outra turma? (S/N): ");
                scanf(" %c", &continuar);
                limpar_buffer();
                
                if (continuar == 'S' || continuar == 's' || continuar == 'N' || continuar == 'n') {
                    break;
                }
                printf("Opcao invalida! Digite S para Sim ou N para Nao.\n");
            }
            
            if (continuar != 'S' && continuar != 's') {
                return;
            }
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
    }
    
    printf("\nPressione Enter para voltar ao menu...");
    getchar();
}

void menu_alunos() {
    int opcao;
    
    while (1) {
        printf("\n=== MENU ALUNOS ===\n");
        if (admin_logado) {
            printf("1. Cadastrar aluno\n");
            printf("2. Excluir aluno\n");
            printf("3. Listar alunos\n");
        } else {
            printf("1. Listar alunos\n");
        }
        printf("0. Voltar ao menu principal\n");
        printf("Escolha uma opcao: ");
        
        scanf("%d", &opcao);
        limpar_buffer();
        
        if (admin_logado) {
            switch (opcao) {
                case 1:
                    cadastrar_aluno();
                    break;
                case 2:
                    excluir_aluno();
                    break;
                case 3:
                    listar_alunos();
                    break;
                case 0:
                    return;
                default:
                    printf("Opcao invalida!\n");
            }
        } else {
            switch (opcao) {
                case 1:
                    listar_alunos();
                    break;
                case 0:
                    return;
                default:
                    printf("Opcao invalida!\n");
            }
        }
    }
}

void menu_professores() {
    int opcao;
    
    while (1) {
        printf("\n=== MENU PROFESSORES ===\n");
        if (admin_logado) {
            printf("1. Cadastrar professor\n");
            printf("2. Excluir professor\n");
            printf("3. Listar professores\n");
        } else {
            printf("1. Listar professores\n");
        }
        printf("0. Voltar ao menu principal\n");
        printf("Escolha uma opcao: ");
        
        scanf("%d", &opcao);
        limpar_buffer();
        
        if (admin_logado) {
            switch (opcao) {
                case 1:
                    cadastrar_professor();
                    break;
                case 2:
                    excluir_professor();
                    break;
                case 3:
                    listar_professores();
                    break;
                case 0:
                    return;
                default:
                    printf("Opcao invalida!\n");
            }
        } else {
            switch (opcao) {
                case 1:
                    listar_professores();
                    break;
                case 0:
                    return;
                default:
                    printf("Opcao invalida!\n");
            }
        }
    }
}

void menu_turmas() {
    int opcao;
    
    while (1) {
        printf("\n=== MENU TURMAS ===\n");
        if (admin_logado) {
            printf("1. Criar turma\n");
            printf("2. Excluir turma\n");
            printf("3. Editar turmas\n");
            printf("4. Listar turmas\n");
        } else {
            printf("1. Listar turmas\n");
        }
        printf("0. Voltar ao menu principal\n");
        printf("Escolha uma opcao: ");
        
        scanf("%d", &opcao);
        limpar_buffer();
        
        if (admin_logado) {
            switch (opcao) {
                case 1:
                    criar_turma();
                    break;
                case 2:
                    excluir_turma();
                    break;
                case 3:
                    editar_turmas();
                    break;
                case 4:
                    listar_turmas();
                    break;
                case 0:
                    return;
                default:
                    printf("Opcao invalida!\n");
            }
        } else {
            switch (opcao) {
                case 1:
                    listar_turmas();
                    break;
                case 0:
                    return;
                default:
                    printf("Opcao invalida!\n");
            }
        }
    }
}

void menu_aulas() {
    int opcao;
    
    while (1) {
        printf("\n=== MENU AULAS ===\n");
        if (admin_logado) {
            printf("1. Registrar aula\n");
            printf("2. Excluir aula\n");
            printf("3. Listar aulas\n");
        } else {
            printf("1. Listar aulas\n");
        }
        printf("0. Voltar ao menu principal\n");
        printf("Escolha uma opcao: ");
        
        scanf("%d", &opcao);
        limpar_buffer();
        
        if (admin_logado) {
            switch (opcao) {
                case 1:
                    registrar_aula();
                    break;
                case 2:
                    excluir_aula();
                    break;
                case 3:
                    listar_aulas();
                    break;
                case 0:
                    return;
                default:
                    printf("Opcao invalida!\n");
            }
        } else {
            switch (opcao) {
                case 1:
                    listar_aulas();
                    break;
                case 0:
                    return;
                default:
                    printf("Opcao invalida!\n");
            }
        }
    }
}

void menu_principal() {
    int opcao;
    
    while (1) {
        printf("\n=== SISTEMA ESCOLAR ===\n");
        if (!admin_logado) {
            printf("1. Login\n");
        }
        printf("2. Alunos\n");
        printf("3. Professores\n");
        printf("4. Turmas\n");
        printf("5. Aulas\n");
        printf("6. Consultar horario de funcionamento\n");
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
                if (!admin_logado) {
                    fazer_login();
                } else {
                    printf("Opcao invalida!\n");
                }
                break;
            case 2:
                menu_alunos();
                break;
            case 3:
                menu_professores();
                break;
            case 4:
                menu_turmas();
                break;
            case 5:
                menu_aulas();
                break;
            case 6:
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