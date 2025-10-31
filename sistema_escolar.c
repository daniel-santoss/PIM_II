#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

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
    char hora[20];
    char materia[50];
    int id;
} Aula;

typedef struct {
    int id;
    int professor_matricula;
    int turma_id;
    char nome[100];
    char tipo[20];
    char disciplina[50];
    int peso;
    char em_grupo;
    char observacao[500];
} Atividade;

typedef struct {
    int id;
    int atividade_id;
    char nome[50];
    int alunos[30];
    int num_alunos;
} Grupo;

int proximo_id_turma = 1;
int proximo_id_aula = 1;
int proxima_matricula = 10000001;
int proximo_id_atividade = 1;
int proximo_id_grupo = 1;
int admin_logado = 0;
int usuario_logado = 0;
int aluno_logado = 0;
int matricula_aluno_logado = 0;
int matricula_professor_logado = 0;
char admin_cpf[] = "12345678909";
char admin_senha[] = "admin";

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void carregar_contadores() {
    FILE *file = fopen("contadores.txt", "r");
    if (file != NULL) {
        fscanf(file, "%d %d %d %d %d", &proximo_id_turma, &proximo_id_aula, &proxima_matricula, &proximo_id_atividade, &proximo_id_grupo);
        fclose(file);
    }
}

void salvar_contadores() {
    FILE *file = fopen("contadores.txt", "w");
    if (file != NULL) {
        fprintf(file, "%d %d %d %d %d", proximo_id_turma, proximo_id_aula, proxima_matricula, proximo_id_atividade, proximo_id_grupo);
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

int validar_matricula_numerica(char *token) {
    // Verificar se a string contém apenas dígitos
    for (int i = 0; i < strlen(token); i++) {
        if (token[i] < '0' || token[i] > '9') {
            return 0;
        }
    }
    // Verificar se não é uma string vazia
    if (strlen(token) == 0) return 0;
    // Verificar se não é apenas zeros
    int mat = atoi(token);
    return mat > 0;
}

void formatar_cpf(char *cpf, char *cpf_formatado) {
    sprintf(cpf_formatado, "%.3s.%.3s.%.3s-%.2s", cpf, cpf+3, cpf+6, cpf+9);
}

int horario_ocupado(int turma_id, char *data, char *horario) {
    FILE *file = fopen("aulas.txt", "r");
    if (file == NULL) return 0;
    
    char linha[500];
    while (fgets(linha, sizeof(linha), file)) {
        int turma_arquivo;
        char data_arquivo[15], horario_arquivo[20];
        
        sscanf(linha, "%*d|%*d|%d|%[^|]|%[^|]|%*[^\n]", &turma_arquivo, data_arquivo, horario_arquivo);
        
        if (turma_arquivo == turma_id && strcmp(data_arquivo, data) == 0 && strcmp(horario_arquivo, horario) == 0) {
            fclose(file);
            return 1;
        }
    }
    
    fclose(file);
    return 0;
}

void exibir_horarios_disponiveis(char *turno, int turma_id, char *data) {
    char horarios[5][15];
    int total_horarios = 5;
    
    if (strcmp(turno, "Matutino") == 0) {
        strcpy(horarios[0], "07:00-07:50");
        strcpy(horarios[1], "07:50-08:40");
        strcpy(horarios[2], "08:40-09:30");
        strcpy(horarios[3], "10:00-10:50");
        strcpy(horarios[4], "10:50-11:40");
    } else if (strcmp(turno, "Vespertino") == 0) {
        strcpy(horarios[0], "13:00-13:50");
        strcpy(horarios[1], "13:50-14:40");
        strcpy(horarios[2], "14:40-15:30");
        strcpy(horarios[3], "16:00-16:50");
        strcpy(horarios[4], "16:50-17:40");
    } else if (strcmp(turno, "Noturno") == 0) {
        strcpy(horarios[0], "18:00-18:50");
        strcpy(horarios[1], "18:50-19:40");
        strcpy(horarios[2], "19:40-20:30");
        strcpy(horarios[3], "21:00-21:50");
        strcpy(horarios[4], "21:50-22:40");
    }
    
    printf("\n=== HORARIOS DISPONIVEIS (%s) ===\n", turno);
    int opcoes_disponiveis[5];
    int count_disponiveis = 0;
    
    for (int i = 0; i < total_horarios; i++) {
        if (!horario_ocupado(turma_id, data, horarios[i])) {
            printf("%d. %s\n", count_disponiveis + 1, horarios[i]);
            opcoes_disponiveis[count_disponiveis] = i;
            count_disponiveis++;
        }
    }
    
    if (count_disponiveis == 0) {
        printf("Nenhum horario disponivel para esta data.\n");
    }
}

int selecionar_horario_disponivel(char *turno, int turma_id, char *data, char *horario_selecionado) {
    char horarios[5][15];
    int total_horarios = 5;
    
    if (strcmp(turno, "Matutino") == 0) {
        strcpy(horarios[0], "07:00-07:50");
        strcpy(horarios[1], "07:50-08:40");
        strcpy(horarios[2], "08:40-09:30");
        strcpy(horarios[3], "10:00-10:50");
        strcpy(horarios[4], "10:50-11:40");
    } else if (strcmp(turno, "Vespertino") == 0) {
        strcpy(horarios[0], "13:00-13:50");
        strcpy(horarios[1], "13:50-14:40");
        strcpy(horarios[2], "14:40-15:30");
        strcpy(horarios[3], "16:00-16:50");
        strcpy(horarios[4], "16:50-17:40");
    } else if (strcmp(turno, "Noturno") == 0) {
        strcpy(horarios[0], "18:00-18:50");
        strcpy(horarios[1], "18:50-19:40");
        strcpy(horarios[2], "19:40-20:30");
        strcpy(horarios[3], "21:00-21:50");
        strcpy(horarios[4], "21:50-22:40");
    }
    
    int opcoes_disponiveis[5];
    int count_disponiveis = 0;
    
    for (int i = 0; i < total_horarios; i++) {
        if (!horario_ocupado(turma_id, data, horarios[i])) {
            opcoes_disponiveis[count_disponiveis] = i;
            count_disponiveis++;
        }
    }
    
    if (count_disponiveis == 0) {
        return 0;
    }
    
    int opcao;
    while (1) {
        printf("\nSelecione o horario (1-%d, 0 para cancelar): ", count_disponiveis);
        scanf("%d", &opcao);
        limpar_buffer();
        if (opcao == 0) return -1;
        
        if (opcao >= 1 && opcao <= count_disponiveis) {
            strcpy(horario_selecionado, horarios[opcoes_disponiveis[opcao-1]]);
            return 1;
        } else {
            printf("Opcao invalida! Digite um numero de 1 a %d.\n", count_disponiveis);
        }
    }
}

int cpf_ja_existe(char *cpf) {
    FILE *file;
    char linha[500];
    char cpf_arquivo[15];
    
    file = fopen("alunos.txt", "r");
    if (file != NULL) {
        while (fgets(linha, sizeof(linha), file)) {
            sscanf(linha, "%*d|%*[^|]|%*[^|]|%*[^|]|%*[^|]|%*c|%[^\n]", cpf_arquivo);
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
            sscanf(linha, "%*d|%*[^|]|%*[^|]|%*[^|]|%*[^|]|%*c|%[^|]|%*[^\n]", cpf_arquivo);
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

int aluno_ja_em_grupo(int matricula, int atividade_id) {
    FILE *file = fopen("grupos.txt", "r");
    if (file == NULL) return 0;
    
    char linha[500];
    while (fgets(linha, sizeof(linha), file)) {
        int id, ativ_id, num_alunos;
        char nome[50];
        sscanf(linha, "%d|%d|%[^|]|%d|", &id, &ativ_id, nome, &num_alunos);
        
        if (ativ_id == atividade_id) {
            char *resto = strchr(linha, '|');
            resto = strchr(resto + 1, '|');
            resto = strchr(resto + 1, '|');
            resto = strchr(resto + 1, '|');
            resto++;
            
            char *token = strtok(resto, ",");
            while (token != NULL) {
                int mat = atoi(token);
                if (mat == matricula) {
                    fclose(file);
                    return 1;
                }
                token = strtok(NULL, ",");
            }
        }
    }
    
    fclose(file);
    return 0;
}

int obter_proximo_numero_grupo(int atividade_id) {
    FILE *file = fopen("grupos.txt", "r");
    if (file == NULL) return 1;
    
    int numeros_usados[100] = {0};
    char linha[500];
    
    while (fgets(linha, sizeof(linha), file)) {
        int ativ_id;
        char nome_grupo[50];
        sscanf(linha, "%*d|%d|%[^|]|", &ativ_id, nome_grupo);
        
        if (ativ_id == atividade_id) {
            int num_grupo;
            if (sscanf(nome_grupo, "Grupo %d", &num_grupo) == 1) {
                if (num_grupo > 0 && num_grupo < 100) {
                    numeros_usados[num_grupo] = 1;
                }
            }
        }
    }
    
    fclose(file);
    
    for (int i = 1; i < 100; i++) {
        if (!numeros_usados[i]) {
            return i;
        }
    }
    
    return 1;
}

void criar_grupo(int atividade_id, int *matriculas, int num_alunos) {
    int num_grupo = obter_proximo_numero_grupo(atividade_id);
    char nome_grupo[50];
    sprintf(nome_grupo, "Grupo %d", num_grupo);
    
    FILE *file = fopen("grupos.txt", "a");
    if (file != NULL) {
        fprintf(file, "%d|%d|%s|%d|", proximo_id_grupo++, atividade_id, nome_grupo, num_alunos);
        
        for (int i = 0; i < num_alunos; i++) {
            fprintf(file, "%d", matriculas[i]);
            if (i < num_alunos - 1) {
                fprintf(file, ",");
            }
        }
        fprintf(file, "\n");
        fclose(file);
        salvar_contadores();
    }
}

void renomear_grupos_atividade(int atividade_id) {
    FILE *file = fopen("grupos.txt", "r");
    FILE *temp = fopen("temp_grupos.txt", "w");
    if (file == NULL || temp == NULL) return;
    
    typedef struct {
        int id;
        int atividade_id;
        char nome[50];
        int num_alunos;
        char alunos_str[200];
    } GrupoTemp;
    
    GrupoTemp grupos[100];
    int count_grupos = 0;
    
    char linha[500];
    while (fgets(linha, sizeof(linha), file)) {
        int id, ativ_id, num_alunos;
        char nome[50], alunos_str[200];
        
        sscanf(linha, "%d|%d|%[^|]|%d|%[^\n]", &id, &ativ_id, nome, &num_alunos, alunos_str);
        
        if (ativ_id == atividade_id) {
            grupos[count_grupos].id = id;
            grupos[count_grupos].atividade_id = ativ_id;
            strcpy(grupos[count_grupos].nome, nome);
            grupos[count_grupos].num_alunos = num_alunos;
            strcpy(grupos[count_grupos].alunos_str, alunos_str);
            count_grupos++;
        } else {
            fprintf(temp, "%s", linha);
        }
    }
    
    for (int i = 0; i < count_grupos; i++) {
        sprintf(grupos[i].nome, "Grupo %d", i + 1);
        fprintf(temp, "%d|%d|%s|%d|%s\n", grupos[i].id, grupos[i].atividade_id, 
                grupos[i].nome, grupos[i].num_alunos, grupos[i].alunos_str);
    }
    
    fclose(file);
    fclose(temp);
    
    remove("grupos.txt");
    rename("temp_grupos.txt", "grupos.txt");
}

void excluir_grupo(int grupo_id, int atividade_id) {
    FILE *file = fopen("grupos.txt", "r");
    FILE *temp = fopen("temp_grupos.txt", "w");
    if (file == NULL || temp == NULL) return;
    
    char linha[500];
    while (fgets(linha, sizeof(linha), file)) {
        int id;
        sscanf(linha, "%d|", &id);
        if (id != grupo_id) {
            fprintf(temp, "%s", linha);
        }
    }
    
    fclose(file);
    fclose(temp);
    
    remove("grupos.txt");
    rename("temp_grupos.txt", "grupos.txt");
    
    renomear_grupos_atividade(atividade_id);
}

void excluir_grupos_atividade(int atividade_id) {
    FILE *file = fopen("grupos.txt", "r");
    FILE *temp = fopen("temp_grupos.txt", "w");
    if (file == NULL || temp == NULL) return;
    
    char linha[500];
    while (fgets(linha, sizeof(linha), file)) {
        int ativ_id;
        sscanf(linha, "%*d|%d|", &ativ_id);
        if (ativ_id != atividade_id) {
            fprintf(temp, "%s", linha);
        }
    }
    
    fclose(file);
    fclose(temp);
    
    remove("grupos.txt");
    rename("temp_grupos.txt", "grupos.txt");
}

void incluir_aluno_turma(int matricula, int turma_id) {
    FILE *file = fopen("aluno_turma.txt", "a");
    if (file != NULL) {
        fprintf(file, "%d|%d\n", matricula, turma_id);
        fclose(file);
    }
}

void incluir_professor_turma(int matricula, int turma_id) {
    FILE *file = fopen("professor_turma.txt", "a");
    if (file != NULL) {
        fprintf(file, "%d|%d\n", matricula, turma_id);
        fclose(file);
    }
}

int professor_ja_tem_turma(int matricula, int turma_id) {
    FILE *file = fopen("professor_turma.txt", "r");
    if (file == NULL) return 0;
    
    char linha[100];
    while (fgets(linha, sizeof(linha), file)) {
        int mat_arquivo, turma_arquivo;
        sscanf(linha, "%d|%d", &mat_arquivo, &turma_arquivo);
        if (mat_arquivo == matricula && turma_arquivo == turma_id) {
            fclose(file);
            return 1;
        }
    }
    
    fclose(file);
    return 0;
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
    while (1) {
        printf("\n=== EDITAR TURMA ===\n");
        printf("1. Alterar dados da turma\n");
        printf("2. Incluir aluno\n");
        printf("3. Remover aluno\n");
        printf("4. Incluir professor\n");
        printf("0. Cancelar\n");
        printf("Escolha uma opcao: ");
        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida! Digite apenas numeros.\n");
            limpar_buffer();
            continue;
        }
        limpar_buffer();
        if (opcao >= 0 && opcao <= 4) {
            break;
        }
        printf("Opcao invalida! Digite 0, 1, 2, 3 ou 4.\n");
    }
    
    if (opcao == 0) {
        return;
    } else if (opcao == 1) {
        // Alterar dados da turma
        // Verificar se há aulas registradas na turma
        FILE *aulas_check = fopen("aulas.txt", "r");
        if (aulas_check != NULL) {
            char linha_aula[500];
            while (fgets(linha_aula, sizeof(linha_aula), aulas_check)) {
                int id_aula, prof_aula, turma_aula;
                sscanf(linha_aula, "%d|%d|%d|", &id_aula, &prof_aula, &turma_aula);
                if (turma_aula == turma_id) {
                    printf("Erro! A alteracao so e possivel caso nao haja registro de aula na turma.\n");
                    fclose(aulas_check);
                    printf("\nPressione Enter para voltar ao menu...");
                    getchar();
                    return;
                }
            }
            fclose(aulas_check);
        }
        
        do {
            printf("\nO que deseja alterar?\n");
            printf("1. Serie\n");
            printf("2. Letra\n");
            printf("3. Ano letivo\n");
            printf("4. Turno\n"); 
            
            int opcao_alterar;
            printf("Opcao: ");
            scanf("%d", &opcao_alterar);
            limpar_buffer();
            
            if (opcao_alterar == 0) return;
        
        // Buscar dados atuais da turma
        FILE *turma_atual = fopen("turmas.txt", "r");
        int serie_atual, ano_atual;
        char nome_atual[100], turno_atual[20];
        char letra_atual;
        
        if (turma_atual != NULL) {
            char linha_ta[500];
            while (fgets(linha_ta, sizeof(linha_ta), turma_atual)) {
                int id_ta, ano_ta, serie_ta, cap_ta;
                char nome_ta[100], turno_ta[20];
                sscanf(linha_ta, "%d|%[^|]|%d|%d|%[^|]|%d", &id_ta, nome_ta, &ano_ta, &serie_ta, turno_ta, &cap_ta);
                if (id_ta == turma_id) {
                    strcpy(nome_atual, nome_ta);
                    strcpy(turno_atual, turno_ta);
                    ano_atual = ano_ta;
                    serie_atual = serie_ta;
                    letra_atual = nome_ta[strlen(nome_ta) - 1];
                    break;
                }
            }
            fclose(turma_atual);
        }
        
        int nova_serie = serie_atual;
        char nova_letra = letra_atual;
        int novo_ano = ano_atual;
        char novo_turno[20];
        strcpy(novo_turno, turno_atual);
        
        if (opcao_alterar == 1) {
            printf("\nSelecione a nova serie:\n");
            printf("1 - 1 Ano\n2 - 2 Ano\n3 - 3 Ano\n0 - Cancelar\n");
            printf("Opcao: ");
            scanf("%d", &nova_serie);
            limpar_buffer();
            if (nova_serie == 0) return;
        } else if (opcao_alterar == 2) {
            printf("\nSelecione a nova letra (A-Z, 0 para cancelar): ");
            scanf(" %c", &nova_letra);
            limpar_buffer();
            if (nova_letra == '0') return;
            if (nova_letra >= 'a' && nova_letra <= 'z') {
                nova_letra = nova_letra - 'a' + 'A';
            }
        } else if (opcao_alterar == 3) {
            printf("\nSelecione o novo ano letivo:\n");
            printf("1 - 2025\n2 - 2026\n0 - Cancelar\n");
            printf("Opcao: ");
            int opcao_ano;
            scanf("%d", &opcao_ano);
            limpar_buffer();
            if (opcao_ano == 0) return;
            novo_ano = (opcao_ano == 1) ? 2025 : 2026;
        } else if (opcao_alterar == 4) {
            printf("\nSelecione o novo turno:\n");
            printf("1 - Matutino\n2 - Vespertino\n3 - Noturno\n0 - Cancelar\n");
            printf("Opcao: ");
            int opcao_turno;
            scanf("%d", &opcao_turno);
            limpar_buffer();
            if (opcao_turno == 0) return;
            if (opcao_turno == 1) strcpy(novo_turno, "Matutino");
            else if (opcao_turno == 2) strcpy(novo_turno, "Vespertino");
            else if (opcao_turno == 3) strcpy(novo_turno, "Noturno");
        }
        
        // Verificar se nova configuração já existe
        if (turma_ja_existe(nova_serie, nova_letra, novo_ano, novo_turno)) {
            printf("Erro: Ja existe uma turma %d Ano %c no ano %d no periodo %s!\n", nova_serie, nova_letra, novo_ano, novo_turno);
            printf("\nPressione Enter para voltar ao menu...");
            getchar();
            return;
        }
        
        // Atualizar arquivo
        FILE *turmas_file = fopen("turmas.txt", "r");
        FILE *temp_turmas = fopen("temp_turmas.txt", "w");
        
        if (turmas_file != NULL && temp_turmas != NULL) {
            char linha_turma[500];
            while (fgets(linha_turma, sizeof(linha_turma), turmas_file)) {
                int id_t, ano_t, serie_t, cap_t;
                char nome_t[100], turno_t[20];
                sscanf(linha_turma, "%d|%[^|]|%d|%d|%[^|]|%d", &id_t, nome_t, &ano_t, &serie_t, turno_t, &cap_t);
                
                if (id_t == turma_id) {
                    char novo_nome[100];
                    sprintf(novo_nome, "%d Ano %c", nova_serie, nova_letra);
                    fprintf(temp_turmas, "%d|%s|%d|%d|%s|%d\n", id_t, novo_nome, novo_ano, nova_serie, novo_turno, cap_t);
                } else {
                    fprintf(temp_turmas, "%s", linha_turma);
                }
            }
            fclose(turmas_file);
            fclose(temp_turmas);
            
            remove("turmas.txt");
            rename("temp_turmas.txt", "turmas.txt");
            printf("Dados da turma alterados com sucesso!\n");
        }
        
            char continuar;
            printf("\nDeseja efetuar outra alteracao? (S/N): ");
            scanf(" %c", &continuar);
            limpar_buffer();
            
            if (continuar != 'S' && continuar != 's') {
                printf("\nPressione Enter para voltar ao menu...");
                getchar();
                return;
            }
        } while (1);
    } else if (opcao == 2) {
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
            char nome[100], email[100], data[15];
            char genero;
            
            sscanf(linha_aluno, "%d|%[^|]|%[^|]|%*[^|]|%[^|]|%c|", &matricula, nome, email, data, &genero);
            
            if (!aluno_ja_tem_turma(matricula)) {
                printf("Matricula: %d | Nome: %s | Email: %s | Nascimento: %s | Genero: %c\n", matricula, nome, email, data, genero);
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
        
        // Validar entrada
        if (strlen(input) == 0) {
            printf("Entrada invalida! Digite pelo menos uma matricula.\n");
            printf("\nPressione Enter para voltar ao menu...");
            getchar();
            return;
        }
        
        int matriculas[50], count = 0;
        char *token = strtok(input, ",");
        while (token != NULL && count < 50) {
            // Remover espaços em branco
            while (*token == ' ') token++;
            int mat = atoi(token);
            if (mat <= 0) {
                printf("Matricula invalida ignorada: %s\n", token);
            } else {
                matriculas[count++] = mat;
            }
            token = strtok(NULL, ",");
        }
        
        if (count == 0) {
            printf("Nenhuma matricula valida foi informada!\n");
            printf("\nPressione Enter para voltar ao menu...");
            getchar();
            return;
        }
        
        // Verificar capacidade da turma
        int alunos_na_turma = 0;
        FILE *at_count = fopen("aluno_turma.txt", "r");
        if (at_count != NULL) {
            char linha_count[100];
            while (fgets(linha_count, sizeof(linha_count), at_count)) {
                int mat_count, turma_count;
                sscanf(linha_count, "%d|%d", &mat_count, &turma_count);
                if (turma_count == turma_id) {
                    alunos_na_turma++;
                }
            }
            fclose(at_count);
        }
        
        if (alunos_na_turma + count > MAX_ALUNOS_POR_TURMA) {
            printf("Erro: A turma ja tem %d alunos. Nao e possivel incluir %d alunos (limite: %d).\n", 
                   alunos_na_turma, count, MAX_ALUNOS_POR_TURMA);
            printf("\nPressione Enter para voltar ao menu...");
            getchar();
            return;
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
        
    } else if (opcao == 3) {
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
        
        // Validar entrada
        if (strlen(input) == 0) {
            printf("Entrada invalida! Digite pelo menos uma matricula.\n");
            printf("\nPressione Enter para voltar ao menu...");
            getchar();
            return;
        }
        
        int matriculas[50], count = 0;
        char *token = strtok(input, ",");
        while (token != NULL && count < 50) {
            // Remover espaços em branco
            while (*token == ' ') token++;
            int mat = atoi(token);
            if (mat <= 0) {
                printf("Matricula invalida ignorada: %s\n", token);
            } else {
                matriculas[count++] = mat;
            }
            token = strtok(NULL, ",");
        }
        
        if (count == 0) {
            printf("Nenhuma matricula valida foi informada!\n");
            printf("\nPressione Enter para voltar ao menu...");
            getchar();
            return;
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
    } else if (opcao == 4) {
        // Incluir professor
        FILE *prof_file = fopen("professores.txt", "r");
        if (prof_file == NULL) {
            printf("Nenhum professor cadastrado.\n");
            printf("\nPressione Enter para voltar ao menu...");
            getchar();
            return;
        }
        
        printf("\n=== PROFESSORES DISPONIVEIS ===\n");
        char linha_prof[500];
        int encontrou_prof = 0;
        
        while (fgets(linha_prof, sizeof(linha_prof), prof_file)) {
            int matricula;
            char nome[100], email[100], materia[200];
            
            sscanf(linha_prof, "%d|%[^|]|%[^|]|%*[^|]|%*[^|]|%*c|%*[^|]|%[^\n]", &matricula, nome, email, materia);
            printf("Matricula: %d | Nome: %s | Email: %s | Materia: %s\n", matricula, nome, email, materia);
            encontrou_prof = 1;
        }
        fclose(prof_file);
        
        if (!encontrou_prof) {
            printf("Nenhum professor cadastrado.\n");
            printf("\nPressione Enter para voltar ao menu...");
            getchar();
            return;
        }
        
        char input[200];
        printf("\nDigite as matriculas dos professores separadas por virgula, (0 para cancelar): ");
        fgets(input, 200, stdin);
        input[strcspn(input, "\n")] = 0;
        if (strcmp(input, "0") == 0) return;
        
        if (strlen(input) == 0) {
            printf("Entrada invalida! Digite pelo menos uma matricula.\n");
            printf("\nPressione Enter para voltar ao menu...");
            getchar();
            return;
        }
        
        int matriculas[50], count = 0;
        char *token = strtok(input, ",");
        while (token != NULL && count < 50) {
            while (*token == ' ') token++;
            int mat = atoi(token);
            if (mat <= 0) {
                printf("Matricula invalida ignorada: %s\n", token);
            } else {
                matriculas[count++] = mat;
            }
            token = strtok(NULL, ",");
        }
        
        if (count == 0) {
            printf("Nenhuma matricula valida foi informada!\n");
            printf("\nPressione Enter para voltar ao menu...");
            getchar();
            return;
        }
        
        int incluidos = 0;
        for (int i = 0; i < count; i++) {
            int matricula_prof = matriculas[i];
            
            if (professor_ja_tem_turma(matricula_prof, turma_id)) {
                printf("Professor com matricula %d ja esta nesta turma!\n", matricula_prof);
                continue;
            }
            
            FILE *check_file = fopen("professores.txt", "r");
            int prof_existe = 0;
            char nome_prof[100] = "";
            if (check_file != NULL) {
                char linha_check[500];
                while (fgets(linha_check, sizeof(linha_check), check_file)) {
                    int mat_check;
                    char nome_temp[100];
                    sscanf(linha_check, "%d|%[^|]|", &mat_check, nome_temp);
                    if (mat_check == matricula_prof) {
                        prof_existe = 1;
                        strcpy(nome_prof, nome_temp);
                        break;
                    }
                }
                fclose(check_file);
            }
            
            if (prof_existe) {
                incluir_professor_turma(matricula_prof, turma_id);
                printf("Professor %s incluido com sucesso!\n", nome_prof);
                incluidos++;
            } else {
                printf("Matricula %d nao encontrada!\n", matricula_prof);
            }
        }
        
        if (incluidos > 0) {
            printf("\n%d professor(es) incluido(s) na turma!\n", incluidos);
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
        printf("Erro ao salvar aluno! Verifique se a pasta output existe.\n");
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
        
        char continuar[10];
        while (1) {
            printf("\nDeseja selecionar mais uma materia? (S/N): ");
            fgets(continuar, 10, stdin);
            continuar[strcspn(continuar, "\n")] = 0;
            
            if (strcmp(continuar, "S") == 0 || strcmp(continuar, "s") == 0 || strcmp(continuar, "N") == 0 || strcmp(continuar, "n") == 0) {
                break;
            }
            printf("Opcao invalida! Digite S para Sim ou N para Nao.\n");
        }
        
        if (strcmp(continuar, "S") != 0 && strcmp(continuar, "s") != 0) {
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
        printf("Erro ao salvar professor! Verifique se a pasta output existe.\n");
    }
    
    printf("\nPressione Enter para voltar ao menu...");
    getchar();
}

void registrar_aula() {
    if (!admin_logado && !matricula_professor_logado) {
        printf("Acesso negado! Apenas administradores e professores podem registrar aulas.\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    Aula nova_aula;
    
    printf("=== REGISTRAR AULA ===\n");
    printf("(Digite '0' em qualquer campo para cancelar)\n");
    
    if (matricula_professor_logado) {
        nova_aula.professor_matricula = matricula_professor_logado;
        printf("Professor selecionado automaticamente: %d\n\n", matricula_professor_logado);
    } else {
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
        
        while (1) {
            printf("\nDigite a matricula do professor (0 para cancelar): ");
            scanf("%d", &nova_aula.professor_matricula);
            limpar_buffer();
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
            
            if (professor_existe) {
                break;
            } else {
                printf("Matricula incorreta. Por favor, digite novamente.\n");
            }
        }
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
        
        // Se professor logado, só mostrar turmas onde ele está registrado
        if (matricula_professor_logado) {
            if (professor_ja_tem_turma(matricula_professor_logado, id)) {
                printf("%d - %s (%do ano, %da serie, %s)\n", id, nome, ano, serie, turno);
                encontrou_turma = 1;
            }
        } else {
            printf("%d - %s (%do ano, %da serie, %s)\n", id, nome, ano, serie, turno);
            encontrou_turma = 1;
        }
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
    
    // Verificar se turma existe e se professor está registrado nela
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
    
    // Verificar se professor está registrado na turma
    if (!professor_ja_tem_turma(nova_aula.professor_matricula, nova_aula.turma_id)) {
        printf("Erro: O professor nao esta registrado nesta turma!\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    // Buscar ano letivo da turma
    int ano_turma = 0;
    FILE *turma_ano_file = fopen("turmas.txt", "r");
    if (turma_ano_file != NULL) {
        char linha_ano[500];
        while (fgets(linha_ano, sizeof(linha_ano), turma_ano_file)) {
            int id_ano, ano_arquivo;
            sscanf(linha_ano, "%d|%*[^|]|%d|", &id_ano, &ano_arquivo);
            if (id_ano == nova_aula.turma_id) {
                ano_turma = ano_arquivo;
                break;
            }
        }
        fclose(turma_ano_file);
    }
    
    while (1) {
        printf("Data da aula (DD/MM/AAAA): ");
        fgets(nova_aula.dia, 15, stdin);
        nova_aula.dia[strcspn(nova_aula.dia, "\n")] = 0;
        if (strcmp(nova_aula.dia, "0") == 0) return;
        
        if (!validar_data(nova_aula.dia)) {
            printf("Data invalida! Use o formato DD/MM/AAAA.\n");
            continue;
        }
        
        // Extrair ano da data inserida
        int ano_data = (nova_aula.dia[6] - '0') * 1000 + (nova_aula.dia[7] - '0') * 100 + (nova_aula.dia[8] - '0') * 10 + (nova_aula.dia[9] - '0');
        
        if (ano_data != ano_turma) {
            printf("Erro! O ano nao pode ser diferente do registrado na turma (%d).\n", ano_turma);
            continue;
        }
        
        break;
    }
    
    // Buscar turno da turma para mostrar horários disponíveis
    char turno_turma[20] = "";
    FILE *turma_file2 = fopen("turmas.txt", "r");
    if (turma_file2 != NULL) {
        char linha_turma2[500];
        while (fgets(linha_turma2, sizeof(linha_turma2), turma_file2)) {
            int id_arquivo;
            char turno_arquivo[20];
            sscanf(linha_turma2, "%d|%*[^|]|%*d|%*d|%[^|]|%*d", &id_arquivo, turno_arquivo);
            if (id_arquivo == nova_aula.turma_id) {
                strcpy(turno_turma, turno_arquivo);
                break;
            }
        }
        fclose(turma_file2);
    }
    
    exibir_horarios_disponiveis(turno_turma, nova_aula.turma_id, nova_aula.dia);
    
    int resultado = selecionar_horario_disponivel(turno_turma, nova_aula.turma_id, nova_aula.dia, nova_aula.hora);
    if (resultado == 0) {
        printf("Nenhum horario disponivel para esta data.\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    } else if (resultado == -1) {
        return;
    }
    
    // Buscar matérias do professor selecionado
    FILE *prof_file = fopen("professores.txt", "r");
    char materias_professor[200] = "";
    if (prof_file != NULL) {
        char linha_prof[500];
        while (fgets(linha_prof, sizeof(linha_prof), prof_file)) {
            int mat_arquivo;
            char materia_prof[200];
            sscanf(linha_prof, "%d|%*[^|]|%*[^|]|%*[^|]|%*[^|]|%*c|%*[^|]|%[^\n]", &mat_arquivo, materia_prof);
            if (mat_arquivo == nova_aula.professor_matricula) {
                strcpy(materias_professor, materia_prof);
                break;
            }
        }
        fclose(prof_file);
    }
    
    // Verificar se professor tem múltiplas matérias
    char materias_lista[10][50];
    int count_materias = 0;
    char *token = strtok(materias_professor, ", ");
    while (token != NULL && count_materias < 10) {
        strcpy(materias_lista[count_materias], token);
        count_materias++;
        token = strtok(NULL, ", ");
    }
    
    if (count_materias > 1) {
        printf("\nSelecione a materia da aula:\n");
        for (int i = 0; i < count_materias; i++) {
            printf("%d. %s\n", i + 1, materias_lista[i]);
        }
        
        int opcao_materia;
        while (1) {
            printf("\nOpcao (1-%d, 0 para cancelar): ", count_materias);
            scanf("%d", &opcao_materia);
            limpar_buffer();
            if (opcao_materia == 0) return;
            
            if (opcao_materia >= 1 && opcao_materia <= count_materias) {
                strcpy(nova_aula.materia, materias_lista[opcao_materia - 1]);
                break;
            } else {
                printf("Opcao invalida! Digite um numero de 1 a %d.\n", count_materias);
            }
        }
    } else {
        strcpy(nova_aula.materia, materias_lista[0]);
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
        usuario_logado = 1;
        printf("Login de administrador realizado com sucesso!\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return 1;
    }
    
    FILE *file = fopen("alunos.txt", "r");
    if (file != NULL) {
        char linha[500];
        while (fgets(linha, sizeof(linha), file)) {
            char nome[100], cpf_arquivo[15], senha_arquivo[50], email[100], data[15];
            int matricula;
            char genero;
            sscanf(linha, "%d|%[^|]|%[^|]|%[^|]|%[^|]|%c|%s", &matricula, nome, email, senha_arquivo, data, &genero, cpf_arquivo);
            
            if (strcmp(cpf_arquivo, cpf) == 0 && strcmp(senha_arquivo, senha) == 0) {
                admin_logado = 0;
                usuario_logado = 1;
                aluno_logado = 1;
                matricula_aluno_logado = matricula;
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
            int matricula;
            char email[100], data[15], materia[200];
            char genero;
            sscanf(linha, "%d|%[^|]|%[^|]|%[^|]|%[^|]|%c|%[^|]|%[^\n]", &matricula, nome, email, senha_arquivo, data, &genero, cpf_arquivo, materia);
            
            if (strcmp(cpf_arquivo, cpf) == 0 && strcmp(senha_arquivo, senha) == 0) {
                admin_logado = 0;
                usuario_logado = 1;
                aluno_logado = 0;
                matricula_professor_logado = matricula;
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
    
    char input[200];
    printf("\nDigite os IDs das turmas a serem excluidos (separados por virgula) ou digite 0 para cancelar): ");
    fgets(input, 200, stdin);
    input[strcspn(input, "\n")] = 0;
    if (strcmp(input, "0") == 0) return;
    
    int ids[50], count = 0;
    char *token = strtok(input, ",");
    while (token != NULL && count < 50) {
        ids[count++] = atoi(token);
        token = strtok(NULL, ",");
    }
    
    FILE *temp = fopen("temp.txt", "w");
    file = fopen("turmas.txt", "r");
    
    if (file == NULL || temp == NULL) {
        printf("Erro ao abrir arquivos!\n");
        return;
    }
    
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

void editar_aula() {
    if (!admin_logado && !matricula_professor_logado) {
        printf("Acesso negado! Apenas administradores e professores podem editar aulas.\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
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
        char dia[15], hora[20], materia[50];
        
        sscanf(linha, "%d|%d|%d|%[^|]|%[^|]|%[^\n]", &id, &prof_matricula, &turma_id, dia, hora, materia);
        
        if (matricula_professor_logado == 0 || prof_matricula == matricula_professor_logado) {
            char nome_turma[100] = "Turma nao encontrada";
            FILE *turma_file = fopen("turmas.txt", "r");
            if (turma_file != NULL) {
                char linha_turma[500];
                while (fgets(linha_turma, sizeof(linha_turma), turma_file)) {
                    int id_turma, ano_turma;
                    char nome_temp[100], turno_temp[20];
                    sscanf(linha_turma, "%d|%[^|]|%d|%*d|%[^|]|", &id_turma, nome_temp, &ano_turma, turno_temp);
                    if (id_turma == turma_id) {
                        sprintf(nome_turma, "%s (%d) - %s", nome_temp, ano_turma, turno_temp);
                        break;
                    }
                }
                fclose(turma_file);
            }
            
            char nome_professor[100] = "Professor nao encontrado";
            FILE *prof_file = fopen("professores.txt", "r");
            if (prof_file != NULL) {
                char linha_prof[500];
                while (fgets(linha_prof, sizeof(linha_prof), prof_file)) {
                    int mat_prof;
                    char nome_temp[100];
                    sscanf(linha_prof, "%d|%[^|]|", &mat_prof, nome_temp);
                    if (mat_prof == prof_matricula) {
                        strcpy(nome_professor, nome_temp);
                        break;
                    }
                }
                fclose(prof_file);
            }
            
            printf("ID: %d | Turma: %s | %s | %s | Data: %s | Horário: %s\n",
                   id, nome_turma, nome_professor, materia, dia, hora);
            encontrou = 1;
        }
    }
    fclose(file);
    
    if (!encontrou) {
        printf("Nenhuma aula registrada.\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    int aula_id;
    while (1) {
        printf("\nQual aula deseja alterar? (Digite o ID, 0 para cancelar): ");
        scanf("%d", &aula_id);
        limpar_buffer();
        if (aula_id == 0) return;
        
        int aula_existe = 0;
        FILE *check_aula = fopen("aulas.txt", "r");
        if (check_aula != NULL) {
            char linha_check[500];
            while (fgets(linha_check, sizeof(linha_check), check_aula)) {
                int id_check, prof_check;
                sscanf(linha_check, "%d|%d|", &id_check, &prof_check);
                if (id_check == aula_id && (matricula_professor_logado == 0 || prof_check == matricula_professor_logado)) {
                    aula_existe = 1;
                    break;
                }
            }
            fclose(check_aula);
        }
        
        if (aula_existe) {
            break;
        } else {
            printf("ID incorreto. Por favor, digite novamente.\n");
        }
    }
    
    do {
        printf("\nQuais os dados deseja alterar?\n");
        printf("1. Turma\n");
        if (admin_logado) printf("2. Professor\n");
        printf("%d. Materia\n", admin_logado ? 3 : 2);
        printf("%d. Data\n", admin_logado ? 4 : 3);
        printf("%d. Horario\n", admin_logado ? 5 : 4);
        printf("0. Cancelar\n");
        
        int opcao;
        printf("Opcao: ");
        scanf("%d", &opcao);
        limpar_buffer();
        
        if (opcao == 0) break;
        
        FILE *aulas_file = fopen("aulas.txt", "r");
        FILE *temp_file = fopen("temp_aulas.txt", "w");
        
        if (aulas_file == NULL || temp_file == NULL) {
            printf("Erro ao abrir arquivos!\n");
            return;
        }
        
        char linha_aula[500];
        int alterado = 0;
        
        while (fgets(linha_aula, sizeof(linha_aula), aulas_file)) {
            int id, prof_mat, turma_id;
            char dia[15], hora[20], mat[50];
            
            sscanf(linha_aula, "%d|%d|%d|%[^|]|%[^|]|%[^\n]", &id, &prof_mat, &turma_id, dia, hora, mat);
            
            if (id == aula_id && (matricula_professor_logado == 0 || prof_mat == matricula_professor_logado)) {
                if (opcao == 1) {
                    // Buscar turno da turma atual
                    char turno_atual[20] = "";
                    FILE *turma_atual = fopen("turmas.txt", "r");
                    if (turma_atual != NULL) {
                        char linha_ta[500];
                        while (fgets(linha_ta, sizeof(linha_ta), turma_atual)) {
                            int id_ta;
                            char turno_ta[20];
                            sscanf(linha_ta, "%d|%*[^|]|%*d|%*d|%[^|]|", &id_ta, turno_ta);
                            if (id_ta == turma_id) {
                                strcpy(turno_atual, turno_ta);
                                break;
                            }
                        }
                        fclose(turma_atual);
                    }
                    
                    printf("\n=== TURMAS DISPONIVEIS ===\n");
                    FILE *turmas = fopen("turmas.txt", "r");
                    if (turmas != NULL) {
                        char linha_t[500];
                        while (fgets(linha_t, sizeof(linha_t), turmas)) {
                            int id_t, ano, serie, cap;
                            char nome_t[100], turno_t[20];
                            sscanf(linha_t, "%d|%[^|]|%d|%d|%[^|]|%d", &id_t, nome_t, &ano, &serie, turno_t, &cap);
                            printf("%d - %s (%do ano, %da serie, %s)\n", id_t, nome_t, ano, serie, turno_t);
                        }
                        fclose(turmas);
                    }
                    int nova_turma_id;
                    while (1) {
                        printf("Nova turma (ID, 0 para cancelar): ");
                        scanf("%d", &nova_turma_id);
                        limpar_buffer();
                        if (nova_turma_id == 0) return;
                        
                        int turma_existe = 0;
                        FILE *check_turma = fopen("turmas.txt", "r");
                        if (check_turma != NULL) {
                            char linha_check[500];
                            while (fgets(linha_check, sizeof(linha_check), check_turma)) {
                                int id_check;
                                sscanf(linha_check, "%d|", &id_check);
                                if (id_check == nova_turma_id) {
                                    turma_existe = 1;
                                    break;
                                }
                            }
                            fclose(check_turma);
                        }
                        
                        if (turma_existe) {
                            break;
                        } else {
                            printf("ID incorreto. Por favor, digite novamente.\n");
                        }
                    }
                    
                    // Buscar turno da nova turma
                    char turno_novo[20] = "";
                    FILE *turma_nova = fopen("turmas.txt", "r");
                    if (turma_nova != NULL) {
                        char linha_tn[500];
                        while (fgets(linha_tn, sizeof(linha_tn), turma_nova)) {
                            int id_tn;
                            char turno_tn[20];
                            sscanf(linha_tn, "%d|%*[^|]|%*d|%*d|%[^|]|", &id_tn, turno_tn);
                            if (id_tn == nova_turma_id) {
                                strcpy(turno_novo, turno_tn);
                                break;
                            }
                        }
                        fclose(turma_nova);
                    }
                    
                    turma_id = nova_turma_id;
                    
                    // Verificar se o período mudou
                    if (strcmp(turno_atual, turno_novo) != 0) {
                        printf("\nA turma alterada tem o periodo diferente da anterior. Por favor, selecione um novo horario\n");
                        exibir_horarios_disponiveis(turno_novo, turma_id, dia);
                        
                        int resultado = selecionar_horario_disponivel(turno_novo, turma_id, dia, hora);
                        if (resultado == 0) {
                            printf("Nenhum horario disponivel para esta data.\n");
                            fclose(aulas_file);
                            fclose(temp_file);
                            remove("temp_aulas.txt");
                            printf("\nPressione Enter para voltar ao menu...");
                            getchar();
                            return;
                        } else if (resultado == -1) {
                            fclose(aulas_file);
                            fclose(temp_file);
                            remove("temp_aulas.txt");
                            return;
                        }
                    } else {
                        printf("Turma alterada com sucesso!\n");
                    }
                } else if (opcao == 2 && admin_logado) {
                    printf("\n=== PROFESSORES DISPONIVEIS ===\n");
                    FILE *profs = fopen("professores.txt", "r");
                    if (profs != NULL) {
                        char linha_p[500];
                        while (fgets(linha_p, sizeof(linha_p), profs)) {
                            int mat;
                            char nome_p[100], materia_p[200];
                            sscanf(linha_p, "%d|%[^|]|%*[^|]|%*[^|]|%*[^|]|%*c|%*[^|]|%[^\n]", &mat, nome_p, materia_p);
                            printf("%d - %s (%s)\n", mat, nome_p, materia_p);
                        }
                        fclose(profs);
                    }
                    while (1) {
                        printf("Novo professor (Matricula, 0 para cancelar): ");
                        scanf("%d", &prof_mat);
                        limpar_buffer();
                        if (prof_mat == 0) return;
                        
                        int professor_existe = 0;
                        FILE *check_prof = fopen("professores.txt", "r");
                        if (check_prof != NULL) {
                            char linha_check[500];
                            while (fgets(linha_check, sizeof(linha_check), check_prof)) {
                                int mat_check;
                                sscanf(linha_check, "%d|", &mat_check);
                                if (mat_check == prof_mat) {
                                    professor_existe = 1;
                                    break;
                                }
                            }
                            fclose(check_prof);
                        }
                        
                        if (professor_existe) {
                            break;
                        } else {
                            printf("Matricula incorreta. Por favor, digite novamente.\n");
                        }
                    }
                } else if ((opcao == 3 && admin_logado) || (opcao == 2 && !admin_logado)) {
                    // Buscar matérias do professor
                    char materias_professor[200] = "";
                    FILE *prof_mat_file = fopen("professores.txt", "r");
                    if (prof_mat_file != NULL) {
                        char linha_pm[500];
                        while (fgets(linha_pm, sizeof(linha_pm), prof_mat_file)) {
                            int mat_pm;
                            char materia_pm[200];
                            sscanf(linha_pm, "%d|%*[^|]|%*[^|]|%*[^|]|%*[^|]|%*c|%*[^|]|%[^\n]", &mat_pm, materia_pm);
                            if (mat_pm == prof_mat) {
                                strcpy(materias_professor, materia_pm);
                                break;
                            }
                        }
                        fclose(prof_mat_file);
                    }
                    
                    // Verificar se professor tem múltiplas matérias
                    char materias_lista[10][50];
                    int count_materias = 0;
                    char *token_mat = strtok(materias_professor, ", ");
                    while (token_mat != NULL && count_materias < 10) {
                        strcpy(materias_lista[count_materias], token_mat);
                        count_materias++;
                        token_mat = strtok(NULL, ", ");
                    }
                    
                    if (count_materias > 1) {
                        printf("\nSelecione a nova materia:\n");
                        for (int i = 0; i < count_materias; i++) {
                            printf("%d. %s\n", i + 1, materias_lista[i]);
                        }
                        
                        int opcao_materia;
                        while (1) {
                            printf("\nOpcao (1-%d, 0 para cancelar): ", count_materias);
                            scanf("%d", &opcao_materia);
                            limpar_buffer();
                            if (opcao_materia == 0) return;
                            
                            if (opcao_materia >= 1 && opcao_materia <= count_materias) {
                                strcpy(mat, materias_lista[opcao_materia - 1]);
                                break;
                            } else {
                                printf("Opcao invalida! Digite um numero de 1 a %d.\n", count_materias);
                            }
                        }
                    } else {
                        printf("Nao e possivel realizar a alteracao, pois o professor so possui uma materia registrada.\n");
                        fclose(aulas_file);
                        fclose(temp_file);
                        remove("temp_aulas.txt");
                        continue;
                    }
                } else if ((opcao == 4 && admin_logado) || (opcao == 3 && !admin_logado)) {
                    // Buscar ano letivo da turma
                    int ano_turma = 0;
                    FILE *turma_ano = fopen("turmas.txt", "r");
                    if (turma_ano != NULL) {
                        char linha_ta[500];
                        while (fgets(linha_ta, sizeof(linha_ta), turma_ano)) {
                            int id_ta, ano_ta;
                            sscanf(linha_ta, "%d|%*[^|]|%d|", &id_ta, &ano_ta);
                            if (id_ta == turma_id) {
                                ano_turma = ano_ta;
                                break;
                            }
                        }
                        fclose(turma_ano);
                    }
                    
                    while (1) {
                        printf("Nova data (DD/MM/AAAA, 0 para cancelar): ");
                        fgets(dia, 15, stdin);
                        dia[strcspn(dia, "\n")] = 0;
                        if (strcmp(dia, "0") == 0) return;
                        
                        if (!validar_data(dia)) {
                            printf("Data invalida! Use o formato DD/MM/AAAA.\n");
                            continue;
                        }
                        
                        // Extrair ano da data inserida
                        int ano_data = (dia[6] - '0') * 1000 + (dia[7] - '0') * 100 + (dia[8] - '0') * 10 + (dia[9] - '0');
                        
                        if (ano_data != ano_turma) {
                            printf("Erro! O ano nao pode ser diferente do registrado na turma.\n");
                            continue;
                        }
                        
                        break;
                    }
                } else if ((opcao == 5 && admin_logado) || (opcao == 4 && !admin_logado)) {
                    // Buscar turno da turma atual
                    char turno_turma[20] = "";
                    FILE *turma_horario = fopen("turmas.txt", "r");
                    if (turma_horario != NULL) {
                        char linha_th[500];
                        while (fgets(linha_th, sizeof(linha_th), turma_horario)) {
                            int id_th;
                            char turno_th[20];
                            sscanf(linha_th, "%d|%*[^|]|%*d|%*d|%[^|]|", &id_th, turno_th);
                            if (id_th == turma_id) {
                                strcpy(turno_turma, turno_th);
                                break;
                            }
                        }
                        fclose(turma_horario);
                    }
                    
                    exibir_horarios_disponiveis(turno_turma, turma_id, dia);
                    
                    int resultado = selecionar_horario_disponivel(turno_turma, turma_id, dia, hora);
                    if (resultado == 0) {
                        printf("Nenhum horario disponivel para esta data.\n");
                        fclose(aulas_file);
                        fclose(temp_file);
                        remove("temp_aulas.txt");
                        printf("\nPressione Enter para voltar ao menu...");
                        getchar();
                        return;
                    } else if (resultado == -1) {
                        fclose(aulas_file);
                        fclose(temp_file);
                        remove("temp_aulas.txt");
                        return;
                    }
                }
                alterado = 1;
            }
            
            fprintf(temp_file, "%d|%d|%d|%s|%s|%s\n", id, prof_mat, turma_id, dia, hora, mat);
        }
        
        fclose(aulas_file);
        fclose(temp_file);
        
        if (alterado) {
            remove("aulas.txt");
            rename("temp_aulas.txt", "aulas.txt");
            printf("Aula alterada com sucesso!\n");
        } else {
            remove("temp_aulas.txt");
        }
        
        char continuar;
        printf("\nDeseja realizar uma nova alteracao? (S/N): ");
        scanf(" %c", &continuar);
        limpar_buffer();
        
        if (continuar != 'S' && continuar != 's') break;
        
    } while (1);
    
    printf("\nPressione Enter para voltar ao menu...");
    getchar();
}

void excluir_aula() {
    if (!admin_logado && !matricula_professor_logado) {
        printf("Acesso negado! Apenas administradores e professores podem excluir aulas.\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
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
        char dia[15], hora[20], materia[50];
        
        sscanf(linha, "%d|%d|%d|%[^|]|%[^|]|%[^\n]", &id, &prof_matricula, &turma_id, dia, hora, materia);
        
        if (matricula_professor_logado == 0 || prof_matricula == matricula_professor_logado) {
            // Buscar nome da turma
            char nome_turma[100] = "Turma nao encontrada";
            FILE *turma_file = fopen("turmas.txt", "r");
            if (turma_file != NULL) {
                char linha_turma[500];
                while (fgets(linha_turma, sizeof(linha_turma), turma_file)) {
                    int id_turma, ano_turma;
                    char nome_temp[100], turno_temp[20];
                    sscanf(linha_turma, "%d|%[^|]|%d|%*d|%[^|]|", &id_turma, nome_temp, &ano_turma, turno_temp);
                    if (id_turma == turma_id) {
                        sprintf(nome_turma, "%s (%d) - %s", nome_temp, ano_turma, turno_temp);
                        break;
                    }
                }
                fclose(turma_file);
            }
            
            // Buscar nome do professor
            char nome_professor[100] = "Professor nao encontrado";
            FILE *prof_file = fopen("professores.txt", "r");
            if (prof_file != NULL) {
                char linha_prof[500];
                while (fgets(linha_prof, sizeof(linha_prof), prof_file)) {
                    int mat_prof;
                    char nome_temp[100];
                    sscanf(linha_prof, "%d|%[^|]|", &mat_prof, nome_temp);
                    if (mat_prof == prof_matricula) {
                        strcpy(nome_professor, nome_temp);
                        break;
                    }
                }
                fclose(prof_file);
            }
            
            printf("ID: %d | Turma: %s | %s | %s | Data: %s | Horario: %s\n",
                   id, nome_turma, nome_professor, materia, dia, hora);
            encontrou = 1;
        }
    }
    fclose(file);
    
    if (!encontrou) {
        printf("Nenhuma aula registrada.\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    char input[200];
    printf("\nDigite os IDs das aulas a serem excluidos separados por virgula, (0 para cancelar): ");
    fgets(input, 200, stdin);
    input[strcspn(input, "\n")] = 0;
    if (strcmp(input, "0") == 0) return;
    
    int ids[50], count = 0;
    char *token = strtok(input, ",");
    while (token != NULL && count < 50) {
        ids[count++] = atoi(token);
        token = strtok(NULL, ",");
    }
    
    FILE *temp_file = fopen("temp.txt", "w");
    file = fopen("aulas.txt", "r");
    
    if (file == NULL || temp_file == NULL) {
        printf("Erro ao abrir arquivos!\n");
        return;
    }
    
    int excluidos = 0;
    
    while (fgets(linha, sizeof(linha), file)) {
        int id_arquivo, prof_matricula;
        sscanf(linha, "%d|%d|", &id_arquivo, &prof_matricula);
        int deve_excluir = 0;
        
        for (int i = 0; i < count; i++) {
            if (id_arquivo == ids[i]) {
                if (matricula_professor_logado == 0 || prof_matricula == matricula_professor_logado) {
                    deve_excluir = 1;
                    excluidos++;
                } else {
                    printf("Voce nao pode excluir aulas de outros professores! ID: %d\n", id_arquivo);
                }
                break;
            }
        }
        
        if (!deve_excluir) {
            fprintf(temp_file, "%s", linha);
        }
    }
    
    fclose(file);
    fclose(temp_file);
    
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
    
    // Carregar turmas em array para ordenação
    typedef struct {
        int id, ano, serie, capacidade;
        char nome[100], turno[20];
    } TurmaTemp;
    
    TurmaTemp turmas[100];
    int count = 0;
    char linha[500];
    
    while (fgets(linha, sizeof(linha), file) && count < 100) {
        sscanf(linha, "%d|%[^|]|%d|%d|%[^|]|%d", &turmas[count].id, turmas[count].nome, 
               &turmas[count].ano, &turmas[count].serie, turmas[count].turno, &turmas[count].capacidade);
        count++;
    }
    fclose(file);
    
    if (count == 0) {
        printf("Nenhuma turma cadastrada.\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    // Ordenar: 1º por série, 2º por letra, 3º por turno
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            int trocar = 0;
            if (turmas[i].serie > turmas[j].serie) {
                trocar = 1;
            } else if (turmas[i].serie == turmas[j].serie) {
                char letra_i = turmas[i].nome[strlen(turmas[i].nome) - 1];
                char letra_j = turmas[j].nome[strlen(turmas[j].nome) - 1];
                if (letra_i > letra_j) {
                    trocar = 1;
                } else if (letra_i == letra_j) {
                    int ordem_i = (strcmp(turmas[i].turno, "Matutino") == 0) ? 1 : 
                                 (strcmp(turmas[i].turno, "Vespertino") == 0) ? 2 : 3;
                    int ordem_j = (strcmp(turmas[j].turno, "Matutino") == 0) ? 1 : 
                                 (strcmp(turmas[j].turno, "Vespertino") == 0) ? 2 : 3;
                    if (ordem_i > ordem_j) {
                        trocar = 1;
                    }
                }
            }
            
            if (trocar) {
                TurmaTemp temp = turmas[i];
                turmas[i] = turmas[j];
                turmas[j] = temp;
            }
        }
    }
    
    printf("\n=== TURMAS CADASTRADAS ===\n");
    for (int i = 0; i < count; i++) {
        // Contar alunos na turma
        int alunos_na_turma = 0;
        FILE *at_file = fopen("aluno_turma.txt", "r");
        if (at_file != NULL) {
            char linha_at[100];
            while (fgets(linha_at, sizeof(linha_at), at_file)) {
                int mat_at, turma_at;
                sscanf(linha_at, "%d|%d", &mat_at, &turma_at);
                if (turma_at == turmas[i].id) {
                    alunos_na_turma++;
                }
            }
            fclose(at_file);
        }
        
        printf("ID: %d | Nome: %s | Ano: %d | Serie: %d | Turno: %s | Capacidade: (%d/%d)\n",
               turmas[i].id, turmas[i].nome, turmas[i].ano, turmas[i].serie, turmas[i].turno, 
               alunos_na_turma, turmas[i].capacidade);
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
            
            printf("\nTurma selecionada: %s | Ano: %d | Serie: %d | Turno: %s\n", turma_nome, turma_ano, turma_serie, turma_turno);
            
            while (1) {
                printf("\nQual detalhe especifico deseja visualizar?\n");
                printf("1. Aulas registradas\n");
                printf("2. Alunos registrados\n");
                printf("3. Professores registrados\n");
                printf("0. Voltar\n");
                printf("Opcao: ");
                
                int opcao_detalhe;
                scanf("%d", &opcao_detalhe);
                limpar_buffer();
                
                if (opcao_detalhe == 0) break;
                
                if (opcao_detalhe == 1) {
                    // Listar aulas da turma
                    printf("\n=== AULAS REGISTRADAS ===\n");
            
            // Carregar todas as aulas da turma em um array
            typedef struct {
                int id;
                int prof_mat;
                char dia[15];
                char hora[20];
                char materia[50];
                char nome_prof[100];
            } AulaTurma;
            
            AulaTurma aulas_turma[100];
            int count_aulas = 0;
            
            FILE *aulas_file = fopen("aulas.txt", "r");
            if (aulas_file != NULL) {
                char linha_aula[500];
                while (fgets(linha_aula, sizeof(linha_aula), aulas_file)) {
                    int id_aula, prof_mat, turma_aula;
                    char dia_aula[15], hora_aula[20], materia_aula[50];
                    
                    sscanf(linha_aula, "%d|%d|%d|%[^|]|%[^|]|%[^\n]", &id_aula, &prof_mat, &turma_aula, dia_aula, hora_aula, materia_aula);
                    
                    if (turma_aula == turma_id && count_aulas < 100) {
                        aulas_turma[count_aulas].id = id_aula;
                        aulas_turma[count_aulas].prof_mat = prof_mat;
                        strcpy(aulas_turma[count_aulas].dia, dia_aula);
                        strcpy(aulas_turma[count_aulas].hora, hora_aula);
                        strcpy(aulas_turma[count_aulas].materia, materia_aula);
                        
                        // Buscar nome do professor
                        strcpy(aulas_turma[count_aulas].nome_prof, "Professor nao encontrado");
                        FILE *prof_file = fopen("professores.txt", "r");
                        if (prof_file != NULL) {
                            char linha_prof[500];
                            while (fgets(linha_prof, sizeof(linha_prof), prof_file)) {
                                int mat_prof;
                                char nome_temp[100];
                                sscanf(linha_prof, "%d|%[^|]|", &mat_prof, nome_temp);
                                if (mat_prof == prof_mat) {
                                    strcpy(aulas_turma[count_aulas].nome_prof, nome_temp);
                                    break;
                                }
                            }
                            fclose(prof_file);
                        }
                        count_aulas++;
                    }
                }
                fclose(aulas_file);
            }
            
            // Ordenar aulas por data e depois por horário
            for (int i = 0; i < count_aulas - 1; i++) {
                for (int j = i + 1; j < count_aulas; j++) {
                    int cmp_data = strcmp(aulas_turma[i].dia, aulas_turma[j].dia);
                    if (cmp_data > 0 || (cmp_data == 0 && strcmp(aulas_turma[i].hora, aulas_turma[j].hora) > 0)) {
                        AulaTurma temp = aulas_turma[i];
                        aulas_turma[i] = aulas_turma[j];
                        aulas_turma[j] = temp;
                    }
                }
            }
            
                    // Exibir aulas ordenadas
                    if (count_aulas > 0) {
                        for (int i = 0; i < count_aulas; i++) {
                            printf("ID: %d | %s | %s | %s | Data: %s\n", 
                                   aulas_turma[i].id, aulas_turma[i].nome_prof, aulas_turma[i].materia, 
                                   aulas_turma[i].hora, aulas_turma[i].dia);
                        }
                    } else {
                        printf("Nenhuma aula registrada nesta turma.\n");
                    }
                } else if (opcao_detalhe == 2) {
                    // Listar alunos da turma
                    printf("\n=== ALUNOS REGISTRADOS ===\n");
                    int alunos_encontrados = 0;
                    FILE *at_file = fopen("aluno_turma.txt", "r");
                    if (at_file != NULL) {
                        char linha_at[100];
                        while (fgets(linha_at, sizeof(linha_at), at_file)) {
                            int mat_at, turma_at;
                            sscanf(linha_at, "%d|%d", &mat_at, &turma_at);
                            
                            if (turma_at == turma_id) {
                                FILE *aluno_file = fopen("alunos.txt", "r");
                                if (aluno_file != NULL) {
                                    char linha_aluno[500];
                                    while (fgets(linha_aluno, sizeof(linha_aluno), aluno_file)) {
                                        int matricula;
                                        char nome[100], email[100], cpf[15], data[15], senha[50];
                                        char genero;
                                        
                                        sscanf(linha_aluno, "%d|%[^|]|%[^|]|%[^|]|%[^|]|%c|%s", &matricula, nome, email, senha, data, &genero, cpf);
                                        
                                        if (matricula == mat_at) {
                                            char cpf_formatado[15];
                                            formatar_cpf(cpf, cpf_formatado);
                                            
                                            if (admin_logado) {
                                                printf("Matricula: %d | Nome: %s | Email: %s | Senha: %s | CPF: %s | Nascimento: %s | Genero: %c\n",
                                                       matricula, nome, email, senha, cpf_formatado, data, genero);
                                            } else {
                                                printf("Matricula: %d | Nome: %s | Email: %s | Nascimento: %s | Genero: %c\n",
                                                       matricula, nome, email, data, genero);
                                            }
                                            alunos_encontrados = 1;
                                            break;
                                        }
                                    }
                                    fclose(aluno_file);
                                }
                            }
                        }
                        fclose(at_file);
                    }
                    if (!alunos_encontrados) {
                        printf("Nenhum aluno registrado nesta turma.\n");
                    }
                } else if (opcao_detalhe == 3) {
                    // Listar professores da turma
                    printf("\n=== PROFESSORES REGISTRADOS ===\n");
                    int professores_encontrados = 0;
                    FILE *pt_file = fopen("professor_turma.txt", "r");
                    if (pt_file != NULL) {
                        char linha_pt[100];
                        while (fgets(linha_pt, sizeof(linha_pt), pt_file)) {
                            int mat_pt, turma_pt;
                            sscanf(linha_pt, "%d|%d", &mat_pt, &turma_pt);
                            
                            if (turma_pt == turma_id) {
                                FILE *prof_file = fopen("professores.txt", "r");
                                if (prof_file != NULL) {
                                    char linha_prof[500];
                                    while (fgets(linha_prof, sizeof(linha_prof), prof_file)) {
                                        int matricula;
                                        char nome[100], email[100], cpf[15], materia[200], senha[50];
                                        char genero;
                                        
                                        sscanf(linha_prof, "%d|%[^|]|%[^|]|%[^|]|%*[^|]|%c|%[^|]|%[^\n]", &matricula, nome, email, senha, &genero, cpf, materia);
                                        
                                        if (matricula == mat_pt) {
                                            char cpf_formatado[15];
                                            formatar_cpf(cpf, cpf_formatado);
                                            
                                            if (admin_logado) {
                                                printf("Matricula: %d | Nome: %s | Email: %s | Senha: %s | CPF: %s | Genero: %c | Materia: %s\n",
                                                       matricula, nome, email, senha, cpf_formatado, genero, materia);
                                            } else {
                                                printf("Matricula: %d | Nome: %s | Email: %s | CPF: %s | Genero: %c | Materia: %s\n",
                                                       matricula, nome, email, cpf_formatado, genero, materia);
                                            }
                                            professores_encontrados = 1;
                                            break;
                                        }
                                    }
                                    fclose(prof_file);
                                }
                            }
                        }
                        fclose(pt_file);
                    }
                    if (!professores_encontrados) {
                        printf("Nenhum professor registrado nesta turma.\n");
                    }
                } else {
                    printf("Opcao invalida!\n");
                    continue;
                }
                
                char continuar;
                while (1) {
                    printf("\nDeseja visualizar outros detalhes? (S/N): ");
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
            
            // Listar turmas novamente
            printf("\n=== TURMAS CADASTRADAS ===\n");
            FILE *file_list = fopen("turmas.txt", "r");
            if (file_list != NULL) {
                char linha_list[500];
                while (fgets(linha_list, sizeof(linha_list), file_list)) {
                    int id, ano, serie, capacidade;
                    char nome[100], turno[20];
                    
                    sscanf(linha_list, "%d|%[^|]|%d|%d|%[^|]|%d", &id, nome, &ano, &serie, turno, &capacidade);
                    printf("ID: %d | Nome: %s | Ano: %d | Serie: %d | Turno: %s | Capacidade: %d\n",
                           id, nome, ano, serie, turno, capacidade);
                }
                fclose(file_list);
            }
        }
    }
    
    printf("\nPressione Enter para voltar ao menu...");
    getchar();
}

void minha_turma() {
    FILE *file = fopen("aluno_turma.txt", "r");
    if (file == NULL) {
        printf("Voce nao esta matriculado em nenhuma turma.\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    char linha[100];
    int turma_id = 0;
    
    while (fgets(linha, sizeof(linha), file)) {
        int mat_arquivo, turma_arquivo;
        sscanf(linha, "%d|%d", &mat_arquivo, &turma_arquivo);
        if (mat_arquivo == matricula_aluno_logado) {
            turma_id = turma_arquivo;
            break;
        }
    }
    fclose(file);
    
    if (turma_id == 0) {
        printf("Voce nao esta matriculado em nenhuma turma.\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    FILE *turma_file = fopen("turmas.txt", "r");
    if (turma_file != NULL) {
        char linha_turma[500];
        while (fgets(linha_turma, sizeof(linha_turma), turma_file)) {
            int id, ano, serie, capacidade;
            char nome[100], turno[20];
            
            sscanf(linha_turma, "%d|%[^|]|%d|%d|%[^|]|%d", &id, nome, &ano, &serie, turno, &capacidade);
            
            if (id == turma_id) {
                printf("\n=== MINHA TURMA ===\n");
                printf("Nome: %s\n", nome);
                printf("Ano: %d\n", ano);
                printf("Serie: %d\n", serie);
                printf("Turno: %s\n", turno);
                break;
            }
        }
        fclose(turma_file);
    }
    
    printf("\nPressione Enter para voltar ao menu...");
    getchar();
}

void lancar_atividade() {
    if (!admin_logado && !matricula_professor_logado) {
        printf("Acesso negado! Apenas administradores e professores podem lancar atividades.\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    Atividade nova_atividade;
    
    printf("=== LANCAR ATIVIDADE ===\n");
    printf("(Digite '0' em qualquer campo para cancelar)\n\n");
    
    // Tipo de atividade
    int tipo_opcao;
    while (1) {
        printf("Qual o tipo de atividade?\n");
        printf("1. Atividades diversas\n");
        printf("2. Trabalhos\n");
        printf("3. Avaliacoes\n");
        printf("0. Cancelar\n");
        printf("Opcao: ");
        scanf("%d", &tipo_opcao);
        limpar_buffer();
        
        if (tipo_opcao == 0) return;
        
        if (tipo_opcao == 1) {
            strcpy(nova_atividade.tipo, "Atividades diversas");
            break;
        } else if (tipo_opcao == 2) {
            strcpy(nova_atividade.tipo, "Trabalhos");
            break;
        } else if (tipo_opcao == 3) {
            strcpy(nova_atividade.tipo, "Avaliacoes");
            break;
        } else {
            printf("Opcao invalida! Digite 1, 2 ou 3.\n");
        }
    }
    
    // Nome da atividade
    printf("\nNome da atividade (maximo 100 caracteres): ");
    fgets(nova_atividade.nome, 100, stdin);
    nova_atividade.nome[strcspn(nova_atividade.nome, "\n")] = 0;
    if (strcmp(nova_atividade.nome, "0") == 0) return;
    
    // Definir professor
    if (matricula_professor_logado) {
        nova_atividade.professor_matricula = matricula_professor_logado;
    } else {
        printf("\nDigite a matricula do professor (0 para cancelar): ");
        scanf("%d", &nova_atividade.professor_matricula);
        limpar_buffer();
        if (nova_atividade.professor_matricula == 0) return;
    }
    
    // Listar turmas do professor
    printf("\n=== TURMAS DISPONIVEIS ===\n");
    FILE *turma_file = fopen("turmas.txt", "r");
    int encontrou_turma = 0;
    if (turma_file != NULL) {
        char linha_turma[500];
        while (fgets(linha_turma, sizeof(linha_turma), turma_file)) {
            int id, ano, serie;
            char nome[100], turno[20];
            
            sscanf(linha_turma, "%d|%[^|]|%d|%d|%[^|]|%*d", &id, nome, &ano, &serie, turno);
            
            if (professor_ja_tem_turma(nova_atividade.professor_matricula, id)) {
                printf("%d - %s (%do ano, %da serie, %s)\n", id, nome, ano, serie, turno);
                encontrou_turma = 1;
            }
        }
        fclose(turma_file);
    }
    
    if (!encontrou_turma) {
        printf("Professor nao esta registrado em nenhuma turma!\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    printf("\nPara qual turma sera aplicada (ID, 0 para cancelar): ");
    scanf("%d", &nova_atividade.turma_id);
    limpar_buffer();
    if (nova_atividade.turma_id == 0) return;
    
    // Verificar se professor esta na turma
    if (!professor_ja_tem_turma(nova_atividade.professor_matricula, nova_atividade.turma_id)) {
        printf("Erro: Professor nao esta registrado nesta turma!\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    // Listar disciplinas do professor
    FILE *prof_file = fopen("professores.txt", "r");
    char materias_professor[200] = "";
    if (prof_file != NULL) {
        char linha_prof[500];
        while (fgets(linha_prof, sizeof(linha_prof), prof_file)) {
            int mat_arquivo;
            char materia_prof[200];
            sscanf(linha_prof, "%d|%*[^|]|%*[^|]|%*[^|]|%*[^|]|%*c|%*[^|]|%[^\n]", &mat_arquivo, materia_prof);
            if (mat_arquivo == nova_atividade.professor_matricula) {
                strcpy(materias_professor, materia_prof);
                break;
            }
        }
        fclose(prof_file);
    }
    
    char materias_lista[10][50];
    int count_materias = 0;
    char *token = strtok(materias_professor, ", ");
    while (token != NULL && count_materias < 10) {
        strcpy(materias_lista[count_materias], token);
        count_materias++;
        token = strtok(NULL, ", ");
    }
    
    if (count_materias > 1) {
        printf("\nQual disciplina sera aplicada?\n");
        for (int i = 0; i < count_materias; i++) {
            printf("%d. %s\n", i + 1, materias_lista[i]);
        }
        
        int opcao_materia;
        while (1) {
            printf("\nOpcao (1-%d, 0 para cancelar): ", count_materias);
            scanf("%d", &opcao_materia);
            limpar_buffer();
            if (opcao_materia == 0) return;
            
            if (opcao_materia >= 1 && opcao_materia <= count_materias) {
                strcpy(nova_atividade.disciplina, materias_lista[opcao_materia - 1]);
                break;
            } else {
                printf("Opcao invalida! Digite um numero de 1 a %d.\n", count_materias);
            }
        }
    } else {
        strcpy(nova_atividade.disciplina, materias_lista[0]);
    }
    
    // Peso da atividade
    while (1) {
        printf("\nQual o peso da atividade? (7, 9 ou 10) 0 para cancelar: ");
        scanf("%d", &nova_atividade.peso);
        limpar_buffer();
        if (nova_atividade.peso == 0) return;
        
        if (nova_atividade.peso == 7 || nova_atividade.peso == 9 || nova_atividade.peso == 10) {
            break;
        } else {
            printf("Peso invalido! Digite apenas 7, 9 ou 10.\n");
        }
    }
    
    // Em grupo
    while (1) {
        printf("\nSera em grupo? (S/N, 0 para cancelar): ");
        char input[10];
        fgets(input, 10, stdin);
        input[strcspn(input, "\n")] = 0;
        if (strcmp(input, "0") == 0) return;
        
        if (strcmp(input, "S") == 0 || strcmp(input, "s") == 0) {
            nova_atividade.em_grupo = 'S';
            
            // Formar grupos imediatamente
            printf("\n=== FORMACAO DE GRUPOS ===\n");
            
            while (1) {
                // Listar alunos da turma que ainda não estão em grupo
                printf("\nAlunos disponiveis para formar grupo:\n");
                FILE *at_file = fopen("aluno_turma.txt", "r");
                int alunos_disponiveis[30];
                int count_disponiveis = 0;
                
                if (at_file != NULL) {
                    char linha_at[100];
                    while (fgets(linha_at, sizeof(linha_at), at_file)) {
                        int mat_at, turma_at;
                        sscanf(linha_at, "%d|%d", &mat_at, &turma_at);
                        
                        if (turma_at == nova_atividade.turma_id && !aluno_ja_em_grupo(mat_at, proximo_id_atividade)) {
                            // Buscar nome do aluno
                            FILE *aluno_file = fopen("alunos.txt", "r");
                            if (aluno_file != NULL) {
                                char linha_aluno[500];
                                while (fgets(linha_aluno, sizeof(linha_aluno), aluno_file)) {
                                    int matricula;
                                    char nome[100];
                                    sscanf(linha_aluno, "%d|%[^|]|", &matricula, nome);
                                    if (matricula == mat_at) {
                                        printf("%d - %s\n", matricula, nome);
                                        alunos_disponiveis[count_disponiveis++] = matricula;
                                        break;
                                    }
                                }
                                fclose(aluno_file);
                            }
                        }
                    }
                    fclose(at_file);
                }
                
                if (count_disponiveis == 0) {
                    printf("Nenhum aluno disponivel para inclusao.\n");
                    break;
                }
                
                char input_grupo[200];
                printf("\nDigite as matriculas dos alunos para formar o grupo (separadas por virgula, 0 para finalizar): ");
                fgets(input_grupo, 200, stdin);
                input_grupo[strcspn(input_grupo, "\n")] = 0;
                if (strcmp(input_grupo, "0") == 0) break;
                
                // Validar entrada
                if (strlen(input_grupo) == 0) {
                    printf("Matricula incorreta. Por favor, digite novamente.\n");
                    continue;
                }
                
                // Processar matriculas
                int matriculas_grupo[30], count_grupo = 0;
                int tem_erro = 0;
                char *token = strtok(input_grupo, ",");
                while (token != NULL && count_grupo < 30) {
                    // Remover espaços em branco
                    while (*token == ' ') token++;
                    
                    // Validar se é um número válido
                    if (!validar_matricula_numerica(token)) {
                        printf("Matricula incorreta. Por favor, digite novamente.\n");
                        tem_erro = 1;
                        break;
                    }
                    int mat = atoi(token);
                    
                    // Verificar se aluno existe
                    FILE *aluno_check = fopen("alunos.txt", "r");
                    int aluno_existe = 0;
                    if (aluno_check != NULL) {
                        char linha_check[500];
                        while (fgets(linha_check, sizeof(linha_check), aluno_check)) {
                            int mat_check;
                            sscanf(linha_check, "%d|", &mat_check);
                            if (mat_check == mat) {
                                aluno_existe = 1;
                                break;
                            }
                        }
                        fclose(aluno_check);
                    }
                    
                    if (!aluno_existe) {
                        printf("Matricula incorreta. Por favor, digite novamente.\n");
                        tem_erro = 1;
                        break;
                    }
                    
                    // Verificar se aluno está na turma
                    FILE *at_check = fopen("aluno_turma.txt", "r");
                    int aluno_na_turma = 0;
                    if (at_check != NULL) {
                        char linha_at[100];
                        while (fgets(linha_at, sizeof(linha_at), at_check)) {
                            int mat_at, turma_at;
                            sscanf(linha_at, "%d|%d", &mat_at, &turma_at);
                            if (mat_at == mat && turma_at == nova_atividade.turma_id) {
                                aluno_na_turma = 1;
                                break;
                            }
                        }
                        fclose(at_check);
                    }
                    
                    if (!aluno_na_turma) {
                        printf("Matricula incorreta. Por favor, digite novamente.\n");
                        tem_erro = 1;
                        break;
                    }
                    
                    // Verificar se aluno já está em grupo
                    if (aluno_ja_em_grupo(mat, proximo_id_atividade)) {
                        printf("Matricula incorreta. Por favor, digite novamente.\n");
                        tem_erro = 1;
                        break;
                    }
                    
                    // Verificar se aluno está disponível na lista
                    int valido = 0;
                    for (int i = 0; i < count_disponiveis; i++) {
                        if (alunos_disponiveis[i] == mat) {
                            valido = 1;
                            break;
                        }
                    }
                    
                    if (valido) {
                        matriculas_grupo[count_grupo++] = mat;
                    } else {
                        printf("Matricula incorreta. Por favor, digite novamente.\n");
                        tem_erro = 1;
                        break;
                    }
                    
                    token = strtok(NULL, ",");
                }
                
                if (tem_erro) {
                    continue;
                }
                
                if (count_grupo > 0) {
                    int num_grupo = obter_proximo_numero_grupo(proximo_id_atividade);
                    criar_grupo(proximo_id_atividade, matriculas_grupo, count_grupo);
                    printf("Grupo %d criado com %d aluno(s)!\n", num_grupo, count_grupo);
                }
            }
            
            break;
        } else if (strcmp(input, "N") == 0 || strcmp(input, "n") == 0) {
            nova_atividade.em_grupo = 'N';
            break;
        } else {
            printf("Opcao invalida! Digite S para Sim ou N para Nao.\n");
        }
    }
    
    // Observacao - inicializar como vazio
    strcpy(nova_atividade.observacao, "");
    while (1) {
        printf("\nDeseja adicionar alguma observacao? (S/N, 0 para cancelar): ");
        char input[10];
        fgets(input, 10, stdin);
        input[strcspn(input, "\n")] = 0;
        if (strcmp(input, "0") == 0) return;
        
        if (strcmp(input, "S") == 0 || strcmp(input, "s") == 0) {
            printf("\nObservacao (maximo 500 caracteres): ");
            fgets(nova_atividade.observacao, 500, stdin);
            nova_atividade.observacao[strcspn(nova_atividade.observacao, "\n")] = 0;
            if (strcmp(nova_atividade.observacao, "0") == 0) return;
            break;
        } else if (strcmp(input, "N") == 0 || strcmp(input, "n") == 0) {
            strcpy(nova_atividade.observacao, ""); // Garantir que fica vazio
            break;
        } else {
            printf("Opcao invalida! Digite S para Sim ou N para Nao.\n");
        }
    }
    
    nova_atividade.id = proximo_id_atividade++;
    
    FILE *file = fopen("atividades.txt", "a");
    if (file != NULL) {
        fprintf(file, "%d|%d|%d|%s|%s|%s|%d|%c|%s\n", 
                nova_atividade.id, nova_atividade.professor_matricula, nova_atividade.turma_id,
                nova_atividade.nome, nova_atividade.tipo, nova_atividade.disciplina,
                nova_atividade.peso, nova_atividade.em_grupo, nova_atividade.observacao);
        fclose(file);
        salvar_contadores();
        printf("\nAtividade lancada com sucesso! ID: %d\n", nova_atividade.id);
    } else {
        printf("Erro ao salvar atividade!\n");
    }
    
    printf("\nPressione Enter para voltar ao menu...");
    getchar();
}

void listar_atividades() {
    FILE *file = fopen("atividades.txt", "r");
    if (file == NULL) {
        printf("Nenhuma atividade registrada.\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    printf("\n=== ATIVIDADES REGISTRADAS ===\n");
    char linha[1000];
    int encontrou = 0;
    
    while (fgets(linha, sizeof(linha), file)) {
        int id, prof_matricula, turma_id, peso;
        char nome[100], tipo[20], disciplina[50], em_grupo, observacao[500];
        
        strcpy(observacao, "");
        
        sscanf(linha, "%d|%d|%d|%[^|]|%[^|]|%[^|]|%d|%c|%[^\n]", 
               &id, &prof_matricula, &turma_id, nome, tipo, disciplina, &peso, &em_grupo, observacao);
        
        // Filtrar por professor se logado
        if (matricula_professor_logado && prof_matricula != matricula_professor_logado) {
            continue;
        }
        
        // Buscar nome do professor
        char nome_professor[100] = "Professor nao encontrado";
        FILE *prof_file = fopen("professores.txt", "r");
        if (prof_file != NULL) {
            char linha_prof[500];
            while (fgets(linha_prof, sizeof(linha_prof), prof_file)) {
                int mat_prof;
                char nome_temp[100];
                sscanf(linha_prof, "%d|%[^|]|", &mat_prof, nome_temp);
                if (mat_prof == prof_matricula) {
                    strcpy(nome_professor, nome_temp);
                    break;
                }
            }
            fclose(prof_file);
        }
        
        // Buscar nome da turma e periodo
        char nome_turma[100] = "Turma nao encontrada";
        char periodo[20] = "";
        FILE *turma_file = fopen("turmas.txt", "r");
        if (turma_file != NULL) {
            char linha_turma[500];
            while (fgets(linha_turma, sizeof(linha_turma), turma_file)) {
                int id_turma;
                char nome_temp[100], turno_temp[20];
                sscanf(linha_turma, "%d|%[^|]|%*d|%*d|%[^|]|", &id_turma, nome_temp, turno_temp);
                if (id_turma == turma_id) {
                    strcpy(nome_turma, nome_temp);
                    strcpy(periodo, turno_temp);
                    break;
                }
            }
            fclose(turma_file);
        }
        
        printf("ID: %d | %s | %s | Peso: %d | %s - (%s) | %s | %s | Atividade em grupo: %c",
               id, tipo, nome, peso, nome_turma, periodo, nome_professor, disciplina, em_grupo);
        
        if (strlen(observacao) > 0) {
            printf(" | Observacao: %s", observacao);
        }
        
        printf("\n");
        encontrou = 1;
    }
    fclose(file);
    
    if (!encontrou) {
        printf("Nenhuma atividade encontrada.\n");
    }
    
    printf("\nPressione Enter para voltar ao menu...");
    getchar();
}

void excluir_atividade() {
    if (!admin_logado && !matricula_professor_logado) {
        printf("Acesso negado! Apenas administradores e professores podem excluir atividades.\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    FILE *file = fopen("atividades.txt", "r");
    if (file == NULL) {
        printf("Nenhuma atividade registrada.\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    printf("\n=== ATIVIDADES DISPONIVEIS ===\n");
    char linha[1000];
    int encontrou = 0;
    
    while (fgets(linha, sizeof(linha), file)) {
        int id, prof_matricula, turma_id, peso;
        char nome[100], tipo[20], disciplina[50], em_grupo, observacao[500];
        
        sscanf(linha, "%d|%d|%d|%[^|]|%[^|]|%[^|]|%d|%c|%[^\n]", 
               &id, &prof_matricula, &turma_id, nome, tipo, disciplina, &peso, &em_grupo, observacao);
        
        // Filtrar por professor se logado
        if (matricula_professor_logado && prof_matricula != matricula_professor_logado) {
            continue;
        }
        
        // Buscar nome da turma
        char nome_turma[100] = "Turma nao encontrada";
        FILE *turma_file = fopen("turmas.txt", "r");
        if (turma_file != NULL) {
            char linha_turma[500];
            while (fgets(linha_turma, sizeof(linha_turma), turma_file)) {
                int id_turma;
                char nome_temp[100];
                sscanf(linha_turma, "%d|%[^|]|", &id_turma, nome_temp);
                if (id_turma == turma_id) {
                    strcpy(nome_turma, nome_temp);
                    break;
                }
            }
            fclose(turma_file);
        }
        
        printf("ID: %d | %s | %s | Turma: %s | Disciplina: %s\n",
               id, nome, tipo, nome_turma, disciplina);
        encontrou = 1;
    }
    fclose(file);
    
    if (!encontrou) {
        printf("Nenhuma atividade encontrada.\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    char input[200];
    printf("\nDigite os IDs das atividades a serem excluidas separados por virgula (0 para cancelar): ");
    fgets(input, 200, stdin);
    input[strcspn(input, "\n")] = 0;
    if (strcmp(input, "0") == 0) return;
    
    int ids[50], count = 0;
    char *token = strtok(input, ",");
    while (token != NULL && count < 50) {
        while (*token == ' ') token++;
        int id = atoi(token);
        if (id <= 0) {
            printf("ID invalido ignorado: %s\n", token);
        } else {
            ids[count++] = id;
        }
        token = strtok(NULL, ",");
    }
    
    if (count == 0) {
        printf("Nenhum ID valido foi informado!\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    FILE *temp = fopen("temp_atividades.txt", "w");
    file = fopen("atividades.txt", "r");
    
    if (file == NULL || temp == NULL) {
        printf("Erro ao abrir arquivos!\n");
        return;
    }
    
    int excluidos = 0;
    
    while (fgets(linha, sizeof(linha), file)) {
        int id, prof_matricula;
        sscanf(linha, "%d|%d|", &id, &prof_matricula);
        int deve_excluir = 0;
        
        for (int i = 0; i < count; i++) {
            if (id == ids[i]) {
                if (matricula_professor_logado == 0 || prof_matricula == matricula_professor_logado) {
                    deve_excluir = 1;
                    excluidos++;
                } else {
                    printf("Voce nao pode excluir atividades de outros professores! ID: %d\n", id);
                }
                break;
            }
        }
        
        if (!deve_excluir) {
            fprintf(temp, "%s", linha);
        }
    }
    
    fclose(file);
    fclose(temp);
    
    remove("atividades.txt");
    rename("temp_atividades.txt", "atividades.txt");
    
    // Excluir grupos das atividades excluídas
    for (int i = 0; i < count; i++) {
        excluir_grupos_atividade(ids[i]);
    }
    
    if (excluidos > 0) {
        printf("%d atividade(s) excluida(s) com sucesso!\n", excluidos);
    } else {
        printf("Nenhuma atividade encontrada!\n");
    }
    
    printf("\nPressione Enter para voltar ao menu...");
    getchar();
}

void editar_atividade() {
    if (!admin_logado && !matricula_professor_logado) {
        printf("Acesso negado! Apenas administradores e professores podem editar atividades.\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    FILE *file = fopen("atividades.txt", "r");
    if (file == NULL) {
        printf("Nenhuma atividade registrada.\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    printf("\n=== ATIVIDADES DISPONIVEIS ===\n");
    char linha[1000];
    int encontrou = 0;
    
    while (fgets(linha, sizeof(linha), file)) {
        int id, prof_matricula, turma_id, peso;
        char nome[100], tipo[20], disciplina[50], em_grupo, observacao[500];
        
        sscanf(linha, "%d|%d|%d|%[^|]|%[^|]|%[^|]|%d|%c|%[^\n]", 
               &id, &prof_matricula, &turma_id, nome, tipo, disciplina, &peso, &em_grupo, observacao);
        
        if (matricula_professor_logado && prof_matricula != matricula_professor_logado) {
            continue;
        }
        
        char nome_turma[100] = "Turma nao encontrada";
        FILE *turma_file = fopen("turmas.txt", "r");
        if (turma_file != NULL) {
            char linha_turma[500];
            while (fgets(linha_turma, sizeof(linha_turma), turma_file)) {
                int id_turma;
                char nome_temp[100];
                sscanf(linha_turma, "%d|%[^|]|", &id_turma, nome_temp);
                if (id_turma == turma_id) {
                    strcpy(nome_turma, nome_temp);
                    break;
                }
            }
            fclose(turma_file);
        }
        
        printf("ID: %d | %s | %s | Turma: %s | Disciplina: %s\n",
               id, nome, tipo, nome_turma, disciplina);
        encontrou = 1;
    }
    fclose(file);
    
    if (!encontrou) {
        printf("Nenhuma atividade encontrada.\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    int atividade_id;
    printf("\nDigite o ID da atividade a ser editada (0 para cancelar): ");
    scanf("%d", &atividade_id);
    limpar_buffer();
    if (atividade_id == 0) return;
    
    printf("\nQual informacao deseja alterar?\n");
    printf("1. Tipo de atividade\n");
    printf("2. Nome da atividade\n");
    printf("3. Turma\n");
    printf("4. Disciplina\n");
    printf("5. Peso da atividade\n");
    printf("6. Integrantes do grupo\n");
    printf("7. Observacao\n");
    printf("0. Cancelar\n");
    printf("Opcao: ");
    
    int opcao;
    scanf("%d", &opcao);
    limpar_buffer();
    if (opcao == 0) return;
    
    FILE *temp = fopen("temp_atividades.txt", "w");
    file = fopen("atividades.txt", "r");
    
    if (file == NULL || temp == NULL) {
        printf("Erro ao abrir arquivos!\n");
        return;
    }
    
    int editado = 0;
    
    while (fgets(linha, sizeof(linha), file)) {
        int id, prof_matricula, turma_id, peso;
        char nome[100], tipo[20], disciplina[50], em_grupo, observacao[500];
        
        sscanf(linha, "%d|%d|%d|%[^|]|%[^|]|%[^|]|%d|%c|%[^\n]", 
               &id, &prof_matricula, &turma_id, nome, tipo, disciplina, &peso, &em_grupo, observacao);
        
        if (id == atividade_id && (matricula_professor_logado == 0 || prof_matricula == matricula_professor_logado)) {
            if (opcao == 1) {
                int tipo_opcao;
                while (1) {
                    printf("\nQual o tipo de atividade?\n");
                    printf("1. Atividades diversas\n");
                    printf("2. Trabalhos\n");
                    printf("3. Avaliacoes\n");
                    printf("0. Cancelar\n");
                    printf("Opcao: ");
                    scanf("%d", &tipo_opcao);
                    limpar_buffer();
                    
                    if (tipo_opcao == 0) return;
                    
                    if (tipo_opcao == 1) {
                        strcpy(tipo, "Atividades diversas");
                        break;
                    } else if (tipo_opcao == 2) {
                        strcpy(tipo, "Trabalhos");
                        break;
                    } else if (tipo_opcao == 3) {
                        strcpy(tipo, "Avaliacoes");
                        break;
                    } else {
                        printf("Opcao invalida! Digite 1, 2 ou 3.\n");
                    }
                }
            } else if (opcao == 2) {
                printf("\nNovo nome da atividade (0 para cancelar): ");
                fgets(nome, 100, stdin);
                nome[strcspn(nome, "\n")] = 0;
                if (strcmp(nome, "0") == 0) return;
            } else if (opcao == 3) {
                printf("\n=== TURMAS DISPONIVEIS ===\n");
                FILE *turma_file = fopen("turmas.txt", "r");
                if (turma_file != NULL) {
                    char linha_turma[500];
                    while (fgets(linha_turma, sizeof(linha_turma), turma_file)) {
                        int id_t, ano, serie;
                        char nome_t[100], turno[20];
                        
                        sscanf(linha_turma, "%d|%[^|]|%d|%d|%[^|]|%*d", &id_t, nome_t, &ano, &serie, turno);
                        
                        if (professor_ja_tem_turma(prof_matricula, id_t)) {
                            printf("%d - %s (%do ano, %da serie, %s)\n", id_t, nome_t, ano, serie, turno);
                        }
                    }
                    fclose(turma_file);
                }
                printf("\nNova turma (ID, 0 para cancelar): ");
                scanf("%d", &turma_id);
                limpar_buffer();
                if (turma_id == 0) return;
            } else if (opcao == 4) {
                FILE *prof_file = fopen("professores.txt", "r");
                char materias_professor[200] = "";
                if (prof_file != NULL) {
                    char linha_prof[500];
                    while (fgets(linha_prof, sizeof(linha_prof), prof_file)) {
                        int mat_arquivo;
                        char materia_prof[200];
                        sscanf(linha_prof, "%d|%*[^|]|%*[^|]|%*[^|]|%*[^|]|%*c|%*[^|]|%[^\n]", &mat_arquivo, materia_prof);
                        if (mat_arquivo == prof_matricula) {
                            strcpy(materias_professor, materia_prof);
                            break;
                        }
                    }
                    fclose(prof_file);
                }
                
                char materias_lista[10][50];
                int count_materias = 0;
                char *token = strtok(materias_professor, ", ");
                while (token != NULL && count_materias < 10) {
                    strcpy(materias_lista[count_materias], token);
                    count_materias++;
                    token = strtok(NULL, ", ");
                }
                
                if (count_materias <= 1) {
                    printf("Nao e possivel alterar a disciplina, pois o professor so possui uma materia registrada.\n");
                } else {
                    printf("\nQual disciplina?\n");
                    for (int i = 0; i < count_materias; i++) {
                        printf("%d. %s\n", i + 1, materias_lista[i]);
                    }
                    
                    int opcao_materia;
                    while (1) {
                        printf("\nOpcao (1-%d, 0 para cancelar): ", count_materias);
                        scanf("%d", &opcao_materia);
                        limpar_buffer();
                        if (opcao_materia == 0) return;
                        
                        if (opcao_materia >= 1 && opcao_materia <= count_materias) {
                            strcpy(disciplina, materias_lista[opcao_materia - 1]);
                            break;
                        } else {
                            printf("Opcao invalida! Digite um numero de 1 a %d.\n", count_materias);
                        }
                    }
                }
            } else if (opcao == 5) {
                while (1) {
                    printf("\nNovo peso da atividade (7, 9 ou 10, 0 para cancelar): ");
                    scanf("%d", &peso);
                    limpar_buffer();
                    if (peso == 0) return;
                    
                    if (peso == 7 || peso == 9 || peso == 10) {
                        break;
                    } else {
                        printf("Peso invalido! Digite apenas 7, 9 ou 10.\n");
                    }
                }
            } else if (opcao == 6) {
                // Verificar se atividade é em grupo
                if (em_grupo != 'S' && em_grupo != 's') {
                    printf("Esta atividade nao e em grupo!\n");
                    fclose(file);
                    fclose(temp);
                    remove("temp_atividades.txt");
                    printf("\nPressione Enter para voltar ao menu...");
                    getchar();
                    return;
                }
                
                // Listar grupos da atividade
                printf("\n=== GRUPOS DA ATIVIDADE ===\n");
                FILE *grupos_file = fopen("grupos.txt", "r");
                if (grupos_file != NULL) {
                    char linha_grupo[500];
                    while (fgets(linha_grupo, sizeof(linha_grupo), grupos_file)) {
                        int id_grupo, ativ_id, num_alunos;
                        char nome_grupo[50], alunos_str[200];
                        sscanf(linha_grupo, "%d|%d|%[^|]|%d|%[^\n]", &id_grupo, &ativ_id, nome_grupo, &num_alunos, alunos_str);
                        
                        if (ativ_id == atividade_id) {
                            printf("%s: ", nome_grupo);
                            char *token = strtok(alunos_str, ",");
                            while (token != NULL) {
                                int mat = atoi(token);
                                // Buscar nome do aluno
                                FILE *aluno_file = fopen("alunos.txt", "r");
                                if (aluno_file != NULL) {
                                    char linha_aluno[500];
                                    while (fgets(linha_aluno, sizeof(linha_aluno), aluno_file)) {
                                        int mat_aluno;
                                        char nome_aluno[100];
                                        sscanf(linha_aluno, "%d|%[^|]|", &mat_aluno, nome_aluno);
                                        if (mat_aluno == mat) {
                                            printf("%s (%d) ", nome_aluno, mat);
                                            break;
                                        }
                                    }
                                    fclose(aluno_file);
                                }
                                token = strtok(NULL, ",");
                            }
                            printf("\n");
                        }
                    }
                    fclose(grupos_file);
                }
                
                printf("\n1. Incluir integrante\n");
                printf("2. Remover integrante\n");
                printf("0. Cancelar\n");
                printf("Opcao: ");
                int opcao_grupo;
                scanf("%d", &opcao_grupo);
                limpar_buffer();
                
                if (opcao_grupo == 1) {
                    // Listar alunos disponíveis
                    printf("\nAlunos disponiveis:\n");
                    int alunos_encontrados = 0;
                    FILE *at_file = fopen("aluno_turma.txt", "r");
                    if (at_file != NULL) {
                        char linha_at[100];
                        while (fgets(linha_at, sizeof(linha_at), at_file)) {
                            int mat_at, turma_at;
                            sscanf(linha_at, "%d|%d", &mat_at, &turma_at);
                            
                            if (turma_at == turma_id && !aluno_ja_em_grupo(mat_at, atividade_id)) {
                                FILE *aluno_file = fopen("alunos.txt", "r");
                                if (aluno_file != NULL) {
                                    char linha_aluno[500];
                                    while (fgets(linha_aluno, sizeof(linha_aluno), aluno_file)) {
                                        int matricula;
                                        char nome[100];
                                        sscanf(linha_aluno, "%d|%[^|]|", &matricula, nome);
                                        if (matricula == mat_at) {
                                            printf("%d - %s\n", matricula, nome);
                                            alunos_encontrados = 1;
                                            break;
                                        }
                                    }
                                    fclose(aluno_file);
                                }
                            }
                        }
                        fclose(at_file);
                    }
                    
                    if (!alunos_encontrados) {
                        printf("Nenhum aluno disponivel para inclusao.\n");
                        fclose(file);
                        fclose(temp);
                        remove("temp_atividades.txt");
                        printf("\nPressione Enter para voltar ao menu...");
                        getchar();
                        return;
                    }
                    
                    printf("\nDigite as matriculas separadas por virgula (0 para cancelar): ");
                    char input[200];
                    fgets(input, 200, stdin);
                    input[strcspn(input, "\n")] = 0;
                    if (strcmp(input, "0") != 0) {
                        // Validar entrada
                        if (strlen(input) == 0) {
                            printf("Matricula incorreta. Por favor, digite novamente.\n");
                            continue;
                        }
                        
                        int matriculas[30], count = 0;
                        int tem_erro = 0;
                        char *token = strtok(input, ",");
                        while (token != NULL && count < 30) {
                            // Remover espaços em branco
                            while (*token == ' ') token++;
                            
                            // Validar se é um número válido
                            if (!validar_matricula_numerica(token)) {
                                printf("Matricula incorreta. Por favor, digite novamente.\n");
                                tem_erro = 1;
                                break;
                            }
                            int mat = atoi(token);
                            
                            // Verificar se aluno existe
                            FILE *aluno_check = fopen("alunos.txt", "r");
                            int aluno_existe = 0;
                            if (aluno_check != NULL) {
                                char linha_check[500];
                                while (fgets(linha_check, sizeof(linha_check), aluno_check)) {
                                    int mat_check;
                                    sscanf(linha_check, "%d|", &mat_check);
                                    if (mat_check == mat) {
                                        aluno_existe = 1;
                                        break;
                                    }
                                }
                                fclose(aluno_check);
                            }
                            
                            if (!aluno_existe) {
                                printf("Matricula incorreta. Por favor, digite novamente.\n");
                                tem_erro = 1;
                                break;
                            }
                            
                            // Verificar se aluno está na turma
                            FILE *at_check = fopen("aluno_turma.txt", "r");
                            int aluno_na_turma = 0;
                            if (at_check != NULL) {
                                char linha_at[100];
                                while (fgets(linha_at, sizeof(linha_at), at_check)) {
                                    int mat_at, turma_at;
                                    sscanf(linha_at, "%d|%d", &mat_at, &turma_at);
                                    if (mat_at == mat && turma_at == turma_id) {
                                        aluno_na_turma = 1;
                                        break;
                                    }
                                }
                                fclose(at_check);
                            }
                            
                            if (!aluno_na_turma) {
                                printf("Matricula incorreta. Por favor, digite novamente.\n");
                                tem_erro = 1;
                                break;
                            }
                            
                            // Verificar se aluno já está em grupo
                            if (aluno_ja_em_grupo(mat, atividade_id)) {
                                printf("Matricula incorreta. Por favor, digite novamente.\n");
                                tem_erro = 1;
                                break;
                            }
                            
                            matriculas[count++] = mat;
                            token = strtok(NULL, ",");
                        }
                        
                        if (tem_erro) {
                            continue;
                        }
                        
                        if (count > 0) {
                            criar_grupo(atividade_id, matriculas, count);
                            printf("%d aluno(s) incluido(s) em novo grupo!\n", count);
                        }
                    }
                } else if (opcao_grupo == 2) {
                    // Listar alunos nos grupos
                    printf("\nAlunos nos grupos:\n");
                    int alunos_em_grupos = 0;
                    FILE *grupos_list = fopen("grupos.txt", "r");
                    if (grupos_list != NULL) {
                        char linha_grupo[500];
                        while (fgets(linha_grupo, sizeof(linha_grupo), grupos_list)) {
                            int id_grupo, ativ_id, num_alunos;
                            char nome_grupo[50], alunos_str[200];
                            sscanf(linha_grupo, "%d|%d|%[^|]|%d|%[^\n]", &id_grupo, &ativ_id, nome_grupo, &num_alunos, alunos_str);
                            
                            if (ativ_id == atividade_id) {
                                printf("%s: ", nome_grupo);
                                char *token = strtok(alunos_str, ",");
                                while (token != NULL) {
                                    int mat = atoi(token);
                                    FILE *aluno_file = fopen("alunos.txt", "r");
                                    if (aluno_file != NULL) {
                                        char linha_aluno[500];
                                        while (fgets(linha_aluno, sizeof(linha_aluno), aluno_file)) {
                                            int mat_aluno;
                                            char nome_aluno[100];
                                            sscanf(linha_aluno, "%d|%[^|]|", &mat_aluno, nome_aluno);
                                            if (mat_aluno == mat) {
                                                printf("%s (%d) ", nome_aluno, mat);
                                                alunos_em_grupos = 1;
                                                break;
                                            }
                                        }
                                        fclose(aluno_file);
                                    }
                                    token = strtok(NULL, ",");
                                }
                                printf("\n");
                            }
                        }
                        fclose(grupos_list);
                    }
                    
                    if (!alunos_em_grupos) {
                        printf("Nenhum aluno disponivel para remocao.\n");
                        continue;
                    }
                    
                    while (1) {
                        printf("\nDigite as matriculas a remover separadas por virgula (0 para cancelar): ");
                        char input[200];
                        fgets(input, 200, stdin);
                        input[strcspn(input, "\n")] = 0;
                        if (strcmp(input, "0") == 0) break;
                        
                        // Validar entrada
                        if (strlen(input) == 0) {
                            printf("Matricula incorreta. Por favor, digite novamente.\n");
                            continue;
                        }
                        
                        // Validar matrículas antes de processar
                        char *input_copy = malloc(strlen(input) + 1);
                        strcpy(input_copy, input);
                        char *token_val = strtok(input_copy, ",");
                        int tem_erro = 0;
                        
                        while (token_val != NULL) {
                            while (*token_val == ' ') token_val++;
                            
                            if (!validar_matricula_numerica(token_val)) {
                                printf("Matricula incorreta. Por favor, digite novamente.\n");
                                tem_erro = 1;
                                break;
                            }
                            
                            int mat = atoi(token_val);
                            
                            // Verificar se aluno existe
                            FILE *aluno_check = fopen("alunos.txt", "r");
                            int aluno_existe = 0;
                            if (aluno_check != NULL) {
                                char linha_check[500];
                                while (fgets(linha_check, sizeof(linha_check), aluno_check)) {
                                    int mat_check;
                                    sscanf(linha_check, "%d|", &mat_check);
                                    if (mat_check == mat) {
                                        aluno_existe = 1;
                                        break;
                                    }
                                }
                                fclose(aluno_check);
                            }
                            
                            if (!aluno_existe) {
                                printf("Matricula incorreta. Por favor, digite novamente.\n");
                                tem_erro = 1;
                                break;
                            }
                            
                            if (!aluno_ja_em_grupo(mat, atividade_id)) {
                                printf("Matricula incorreta. Por favor, digite novamente.\n");
                                tem_erro = 1;
                                break;
                            }
                            
                            token_val = strtok(NULL, ",");
                        }
                        free(input_copy);
                        
                        if (tem_erro) {
                            continue;
                        }
                        
                        // Se chegou aqui, processar remoção
                        // Remover alunos dos grupos
                        FILE *grupos_file = fopen("grupos.txt", "r");
                        FILE *temp_grupos = fopen("temp_grupos.txt", "w");
                        if (grupos_file != NULL && temp_grupos != NULL) {
                            char linha_grupo[500];
                            while (fgets(linha_grupo, sizeof(linha_grupo), grupos_file)) {
                                int id_grupo, ativ_id, num_alunos;
                                char nome_grupo[50], alunos_str[200];
                                sscanf(linha_grupo, "%d|%d|%[^|]|%d|%[^\n]", &id_grupo, &ativ_id, nome_grupo, &num_alunos, alunos_str);
                                
                                if (ativ_id == atividade_id) {
                                    // Processar remoções
                                    char nova_lista[200] = "";
                                    int novo_count = 0;
                                    char *token = strtok(alunos_str, ",");
                                    while (token != NULL) {
                                        int mat = atoi(token);
                                        int remover = 0;
                                        
                                        char *input_copy = malloc(strlen(input) + 1);
                                        strcpy(input_copy, input);
                                        char *rem_token = strtok(input_copy, ",");
                                        while (rem_token != NULL) {
                                            while (*rem_token == ' ') rem_token++;
                                            if (atoi(rem_token) == mat) {
                                                remover = 1;
                                                break;
                                            }
                                            rem_token = strtok(NULL, ",");
                                        }
                                        free(input_copy);
                                        
                                        if (!remover) {
                                            if (novo_count > 0) strcat(nova_lista, ",");
                                            char mat_str[20];
                                            sprintf(mat_str, "%d", mat);
                                            strcat(nova_lista, mat_str);
                                            novo_count++;
                                        }
                                        token = strtok(NULL, ",");
                                    }
                                    
                                    if (novo_count > 0) {
                                        fprintf(temp_grupos, "%d|%d|%s|%d|%s\n", id_grupo, ativ_id, nome_grupo, novo_count, nova_lista);
                                    }
                                } else {
                                    fprintf(temp_grupos, "%s", linha_grupo);
                                }
                            }
                            fclose(grupos_file);
                            fclose(temp_grupos);
                            
                            remove("grupos.txt");
                            rename("temp_grupos.txt", "grupos.txt");
                            renomear_grupos_atividade(atividade_id);
                            printf("Integrantes removidos com sucesso!\n");
                        }
                        break;
                    }
                }
            } else if (opcao == 7) {
                printf("\nNova observacao (0 para cancelar): ");
                fgets(observacao, 500, stdin);
                observacao[strcspn(observacao, "\n")] = 0;
                if (strcmp(observacao, "0") == 0) return;
            }
            editado = 1;
        }
        
        fprintf(temp, "%d|%d|%d|%s|%s|%s|%d|%c|%s\n", 
                id, prof_matricula, turma_id, nome, tipo, disciplina, peso, em_grupo, observacao);
    }
    
    fclose(file);
    fclose(temp);
    
    remove("atividades.txt");
    rename("temp_atividades.txt", "atividades.txt");
    
    if (editado) {
        printf("Atividade editada com sucesso!\n");
    } else {
        printf("Atividade nao encontrada!\n");
    }
    
    printf("\nPressione Enter para voltar ao menu...");
    getchar();
}

void listar_atividades_aluno() {
    // Buscar turma do aluno
    FILE *at_file = fopen("aluno_turma.txt", "r");
    int turma_aluno = 0;
    if (at_file != NULL) {
        char linha_at[100];
        while (fgets(linha_at, sizeof(linha_at), at_file)) {
            int mat_at, turma_at;
            sscanf(linha_at, "%d|%d", &mat_at, &turma_at);
            if (mat_at == matricula_aluno_logado) {
                turma_aluno = turma_at;
                break;
            }
        }
        fclose(at_file);
    }
    
    if (turma_aluno == 0) {
        printf("Voce nao esta matriculado em nenhuma turma.\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    FILE *file = fopen("atividades.txt", "r");
    if (file == NULL) {
        printf("Nenhuma atividade registrada.\n");
        printf("\nPressione Enter para voltar ao menu...");
        getchar();
        return;
    }
    
    printf("\n=== ATIVIDADES DA TURMA ===\n");
    char linha[1000];
    int encontrou = 0;
    
    while (fgets(linha, sizeof(linha), file)) {
        int id, prof_matricula, turma_id, peso;
        char nome[100], tipo[20], disciplina[50], em_grupo, observacao[500];
        
        strcpy(observacao, "");
        
        sscanf(linha, "%d|%d|%d|%[^|]|%[^|]|%[^|]|%d|%c|%[^\n]", 
               &id, &prof_matricula, &turma_id, nome, tipo, disciplina, &peso, &em_grupo, observacao);
        
        if (turma_id != turma_aluno) {
            continue;
        }
        
        // Buscar nome do professor
        char nome_professor[100] = "Professor nao encontrado";
        FILE *prof_file = fopen("professores.txt", "r");
        if (prof_file != NULL) {
            char linha_prof[500];
            while (fgets(linha_prof, sizeof(linha_prof), prof_file)) {
                int mat_prof;
                char nome_temp[100];
                sscanf(linha_prof, "%d|%[^|]|", &mat_prof, nome_temp);
                if (mat_prof == prof_matricula) {
                    strcpy(nome_professor, nome_temp);
                    break;
                }
            }
            fclose(prof_file);
        }
        
        // Buscar nome da turma e periodo
        char nome_turma[100] = "Turma nao encontrada";
        char periodo[20] = "";
        FILE *turma_file = fopen("turmas.txt", "r");
        if (turma_file != NULL) {
            char linha_turma[500];
            while (fgets(linha_turma, sizeof(linha_turma), turma_file)) {
                int id_turma;
                char nome_temp[100], turno_temp[20];
                sscanf(linha_turma, "%d|%[^|]|%*d|%*d|%[^|]|", &id_turma, nome_temp, turno_temp);
                if (id_turma == turma_id) {
                    strcpy(nome_turma, nome_temp);
                    strcpy(periodo, turno_temp);
                    break;
                }
            }
            fclose(turma_file);
        }
        
        printf("ID: %d | %s | %s | Peso: %d | %s - (%s) | %s | %s | Atividade em grupo: %c",
               id, tipo, nome, peso, nome_turma, periodo, nome_professor, disciplina, em_grupo);
        
        if (strlen(observacao) > 0) {
            printf(" | Observacao: %s", observacao);
        }
        
        printf("\n");
        encontrou = 1;
    }
    fclose(file);
    
    if (!encontrou) {
        printf("Nenhuma atividade encontrada para sua turma.\n");
    }
    
    printf("\nPressione Enter para voltar ao menu...");
    getchar();
}

void minhas_notas() {
    printf("\n=== MINHAS NOTAS ===\n");
    printf("Funcionalidade em desenvolvimento.\n");
    printf("\nPressione Enter para voltar ao menu...");
    getchar();
}

void menu_atividades_aluno() {
    int opcao;
    
    while (1) {
        printf("\n=== ATIVIDADES E NOTAS ===\n");
        printf("1. Listar atividades\n");
        printf("2. Minhas notas\n");
        printf("0. Voltar ao menu principal\n");
        printf("Escolha uma opcao: ");
        
        scanf("%d", &opcao);
        limpar_buffer();
        
        switch (opcao) {
            case 1:
                listar_atividades_aluno();
                break;
            case 2:
                minhas_notas();
                break;
            case 0:
                return;
            default:
                printf("Opcao invalida!\n");
        }
    }
}

void menu_atividades() {
    int opcao;
    
    while (1) {
        printf("\n=== ATIVIDADES E CONTROLE DE NOTA ===\n");
        printf("1. Lancar atividade\n");
        printf("2. Editar atividade\n");
        printf("3. Excluir atividade\n");
        printf("4. Listar atividades\n");
        printf("0. Voltar ao menu principal\n");
        printf("Escolha uma opcao: ");
        
        scanf("%d", &opcao);
        limpar_buffer();
        
        switch (opcao) {
            case 1:
                lancar_atividade();
                break;
            case 2:
                editar_atividade();
                break;
            case 3:
                excluir_atividade();
                break;
            case 4:
                listar_atividades();
                break;
            case 0:
                return;
            default:
                printf("Opcao invalida!\n");
        }
    }
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
    int turma_aluno = 0;
    
    // Se aluno logado, buscar sua turma
    if (aluno_logado) {
        FILE *at_file = fopen("aluno_turma.txt", "r");
        if (at_file != NULL) {
            char linha_at[100];
            while (fgets(linha_at, sizeof(linha_at), at_file)) {
                int mat_at, turma_at;
                sscanf(linha_at, "%d|%d", &mat_at, &turma_at);
                if (mat_at == matricula_aluno_logado) {
                    turma_aluno = turma_at;
                    break;
                }
            }
            fclose(at_file);
        }
        
        if (turma_aluno == 0) {
            printf("Voce nao esta matriculado em nenhuma turma.\n");
            printf("\nPressione Enter para voltar ao menu...");
            getchar();
            return;
        }
    }
    
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
        char dia[15], hora[20], materia[50];
        
        sscanf(linha, "%d|%d|%d|%[^|]|%[^|]|%[^\n]", &id, &prof_matricula, &turma_id, dia, hora, materia);
        
        // Filtrar por turma do aluno se logado
        if (aluno_logado && turma_id != turma_aluno) {
            continue;
        }
        
        // Buscar nome do professor
        char nome_professor[100] = "Professor nao encontrado";
        FILE *prof_file = fopen("professores.txt", "r");
        if (prof_file != NULL) {
            char linha_prof[500];
            while (fgets(linha_prof, sizeof(linha_prof), prof_file)) {
                int mat_prof;
                char nome_temp[100];
                sscanf(linha_prof, "%d|%[^|]|", &mat_prof, nome_temp);
                if (mat_prof == prof_matricula) {
                    strcpy(nome_professor, nome_temp);
                    break;
                }
            }
            fclose(prof_file);
        }
        
        // Buscar nome da turma
        char nome_turma[100] = "Turma nao encontrada";
        FILE *turma_file = fopen("turmas.txt", "r");
        if (turma_file != NULL) {
            char linha_turma[500];
            while (fgets(linha_turma, sizeof(linha_turma), turma_file)) {
                int id_turma, ano_turma;
                char nome_temp[100], turno_temp[20];
                sscanf(linha_turma, "%d|%[^|]|%d|%*d|%[^|]|", &id_turma, nome_temp, &ano_turma, turno_temp);
                if (id_turma == turma_id) {
                    sprintf(nome_turma, "%s (%d) - %s", nome_temp, ano_turma, turno_temp);
                    break;
                }
            }
            fclose(turma_file);
        }
        
        printf("ID: %d | Turma: %s | %s | %s | Data: %s | Horario: %s\n", id, nome_turma, nome_professor, materia, dia, hora);
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
            printf("2. Editar aula\n");
            printf("3. Excluir aula\n");
            printf("4. Listar aulas\n");
        } else {
            printf("1. Registrar aula\n");
            printf("2. Editar aula\n");
            printf("3. Excluir aula\n");
            printf("4. Listar aulas\n");
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
                    editar_aula();
                    break;
                case 3:
                    excluir_aula();
                    break;
                case 4:
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
                    registrar_aula();
                    break;
                case 2:
                    editar_aula();
                    break;
                case 3:
                    excluir_aula();
                    break;
                case 4:
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
        if (!usuario_logado) {
            printf("1. Login\n");
            printf("2. Consultar horario de funcionamento\n");
        } else if (aluno_logado) {
            printf("1. Minha turma\n");
            printf("2. Listar aulas\n");
            printf("3. Atividades e notas\n");
            printf("4. Consultar horario de funcionamento\n");
        } else if (admin_logado) {
            printf("1. Atividades e controle de nota\n");
            printf("2. Alunos\n");
            printf("3. Professores\n");
            printf("4. Turmas\n");
            printf("5. Aulas\n");
            printf("6. Consultar horario de funcionamento\n");
        } else {
            printf("1. Turmas\n");
            printf("2. Aulas\n");
            printf("3. Atividades e controle de nota\n");
            printf("4. Consultar horario de funcionamento\n");
        }
        if (usuario_logado) {
            printf("0. Logout\n");
            if (admin_logado) {
                printf("\n[MODO ADMINISTRADOR ATIVO]\n");
            }
        } else {
            printf("0. Sair\n");
        }
        printf("Escolha uma opcao: ");
        
        scanf("%d", &opcao);
        limpar_buffer();
        
        if (opcao == 0) {
            if (usuario_logado) {
                admin_logado = 0;
                usuario_logado = 0;
                aluno_logado = 0;
                matricula_aluno_logado = 0;
                matricula_professor_logado = 0;
                printf("Logout realizado com sucesso!\n");
                printf("\nPressione Enter para continuar...");
                getchar();
            } else {
                printf("Saindo do sistema...\n");
                exit(0);
            }
        } else if (!usuario_logado) {
            switch (opcao) {
                case 1:
                    fazer_login();
                    break;
                case 2:
                    consultar_horarios();
                    break;
                default:
                    printf("Opcao invalida!\n");
            }
        } else if (aluno_logado) {
            switch (opcao) {
                case 1:
                    minha_turma();
                    break;
                case 2:
                    listar_aulas();
                    break;
                case 3:
                    menu_atividades_aluno();
                    break;
                case 4:
                    consultar_horarios();
                    break;
                default:
                    printf("Opcao invalida!\n");
            }
        } else if (admin_logado) {
            switch (opcao) {
                case 1:
                    menu_atividades();
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
                default:
                    printf("Opcao invalida!\n");
            }
        } else {
            switch (opcao) {
                case 1:
                    menu_turmas();
                    break;
                case 2:
                    menu_aulas();
                    break;
                case 3:
                    menu_atividades();
                    break;
                case 4:
                    consultar_horarios();
                    break;
                default:
                    printf("Opcao invalida!\n");
            }
        }
    }
}

int main() {
    setlocale(LC_ALL, "Portuguese");
    
    carregar_contadores();
    
    printf("Bem-vindo ao Sistema Escolar!\n");
    
    menu_principal();
    
    return 0;
}
