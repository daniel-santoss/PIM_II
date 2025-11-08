// Bibliotecas necessárias para o sistema
#include <stdio.h>    // Entrada/saída padrão (printf, scanf, FILE)
#include <stdlib.h>   // Funções gerais (malloc, free, exit)
#include <string.h>   // Manipulação de strings (strcpy, strcmp, strlen)
#include <ctype.h>    // Funções de caracteres (isdigit, toupper)

// Estruturas de dados do sistema

// Estrutura para armazenar dados de uma turma
typedef struct {
    int id;           // Identificador único da turma
    char nome[100];   // Nome da turma (ex: "1º A")
    int serie;        // Série/ano escolar (1, 2 ou 3)
    char turno[20];   // Turno (Matutino, Vespertino, Noturno)
    int ano;          // Ano letivo
} Turma;

// Estrutura para armazenar dados de um aluno
typedef struct {
    int matricula;    // Número de matrícula único
    char nome[100];   // Nome completo do aluno
    char cpf[12];     // CPF (11 dígitos + \0)
    char senha[20];   // Senha para login
    int turma_id;     // ID da turma que o aluno pertence
} Aluno;

// Estrutura para armazenar dados de um professor
typedef struct {
    int matricula;      // Número de matrícula único
    char nome[100];     // Nome completo do professor
    char cpf[12];       // CPF (11 dígitos + \0)
    char senha[20];     // Senha para login
    char materias[200]; // Matérias que o professor leciona (separadas por vírgula)
} Professor;

// Estrutura para armazenar dados de uma aula
typedef struct {
    int id;                   // Identificador único da aula
    int turma_id;             // ID da turma que terá a aula
    int professor_matricula;  // Matrícula do professor responsável
    char disciplina[50];      // Matéria da aula
    char data[11];            // Data da aula (DD/MM/AAAA)
    char horario[15];         // Horário da aula (HH:MM-HH:MM)
} Aula;

// Estrutura para armazenar dados de uma atividade avaliativa
typedef struct {
    int id;                   // Identificador único da atividade
    char tipo[50];            // Tipo (Prova, Trabalho, Exercício, etc.)
    char nome[100];           // Nome/descrição da atividade
    int turma_id;             // ID da turma que fará a atividade
    int professor_matricula;  // Matrícula do professor responsável
    char disciplina[50];      // Matéria da atividade
    char data[11];            // Data de aplicação (DD/MM/AAAA)
} Atividade;

// Estrutura para armazenar notas dos alunos
typedef struct {
    int id;                // Identificador único da nota
    int atividade_id;      // ID da atividade avaliada
    int aluno_matricula;   // Matrícula do aluno
    float nota;            // Nota obtida (0.0 a 10.0)
} Nota;

typedef struct {
    int id;
    int aula_id;
    int aluno_matricula;
    char presenca;
} Presenca;

// Variáveis globais para controle de sessão
int admin_logado = 0;      // Flag: 1 se admin está logado, 0 caso contrário
int aluno_logado = 0;      // Flag: 1 se aluno está logado, 0 caso contrário
int professor_logado = 0;  // Flag: 1 se professor está logado, 0 caso contrário
int matricula_atual = 0;   // Armazena a matrícula do usuário logado

// Declaração de funções (protótipos)

// Funções utilitárias
void limpar_buffer();                                              // Limpa buffer do teclado
void pausar();                                                     // Pausa execução até Enter
char* obter_caminho_arquivo(char *nome_arquivo);                   // Cria caminho completo do arquivo
int fazer_login();                                                 // Realiza login no sistema
void menu_admin();                                                 // Menu do administrador
void menu_aluno();                                                 // Menu do aluno
void menu_professor();
void menu_diario_eletronico();                                             // Menu do professor
int validar_cpf(char *cpf);                                        // Valida formato do CPF
int gerar_id(char *arquivo);                                       // Gera próximo ID disponível
int confirmar_operacao(char *mensagem);                            // Confirma operação crítica
int validar_id_positivo(int id);                                   // Valida se ID é positivo
int verificar_arquivo_existe(char *arquivo, char *mensagem_erro);
int selecionar_turma();
int selecionar_aula_professor(int turma_id);
void listar_alunos_turma(int turma_id);  // Verifica se arquivo existe

// Funções de gerenciamento de turmas
void cadastrar_turma();  // Cadastra nova turma
void listar_turmas();    // Lista todas as turmas
void excluir_turma();    // Remove turma do sistema

// Funções de gerenciamento de alunos
void cadastrar_aluno();  // Cadastra novo aluno
void listar_alunos();    // Lista todos os alunos
void excluir_aluno();    // Remove aluno do sistema
void minha_turma();      // Mostra turma do aluno logado

// Funções de gerenciamento de professores
void cadastrar_professor();  // Cadastra novo professor
void listar_professores();   // Lista todos os professores
void excluir_professor();    // Remove professor do sistema

// Funções de gerenciamento de aulas
void registrar_aula();  // Registra nova aula
void listar_aulas();    // Lista todas as aulas
void excluir_aula();    // Remove aula do sistema
void minhas_aulas();    // Mostra aulas do professor logado

// Funções de gerenciamento de atividades
void lancar_atividade();      // Lança nova atividade avaliativa
void consultar_atividades();  // Consulta atividades cadastradas
void excluir_atividade();     // Remove atividade do sistema

// Funções de gerenciamento de notas
void lancar_notas();     // Lança notas para atividades
void consultar_notas();  // Consulta notas cadastradas
void excluir_nota();     // Remove nota do sistema
void minhas_notas();
void registrar_presenca();
void consultar_presencas();     // Mostra notas do aluno logado

// IMPLEMENTAÇÃO DAS FUNÇÕES

// Limpa o buffer de entrada do teclado
// Remove caracteres extras após scanf
void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);  // Lê até encontrar \n ou EOF
}

// Pausa a execução até o usuário pressionar Enter
// Usado para dar tempo de ler mensagens na tela
void pausar() {
    printf("\nPressione Enter para continuar...");
    getchar();  // Aguarda Enter
}

// Valida se o CPF tem formato correto (11 dígitos numéricos)
// Retorna 1 se válido, 0 se inválido
int validar_cpf(char *cpf) {
    if (strlen(cpf) != 11) return 0;  // Deve ter exatamente 11 caracteres
    for (int i = 0; i < 11; i++) {
        if (!isdigit(cpf[i])) return 0;  // Todos devem ser dígitos
    }
    return 1;  // CPF válido
}

// Valida se a disciplina informada existe no sistema
// Padroniza o nome da disciplina e retorna 1 se válida, 0 se inválida
int validar_disciplina(char *disciplina) {
    // Lista de disciplinas aceitas pelo sistema
    char disciplinas_validas[][20] = {
        "Matematica", "Ciencias", "Geografia", "Historia", 
        "Filosofia", "Sociologia", "Portugues", "Ingles"
    };
    int total_disciplinas = 8;
    
    // Verifica se a disciplina está na lista (ignora maiúsculas/minúsculas)
    for (int i = 0; i < total_disciplinas; i++) {
        if (strcasecmp(disciplina, disciplinas_validas[i]) == 0) {
            strcpy(disciplina, disciplinas_validas[i]); // Padroniza o nome
            return 1;  // Disciplina válida
        }
    }
    return 0;  // Disciplina não encontrada
}

// Verifica se um professor leciona determinada disciplina
// Retorna 1 se leciona, 0 se não leciona
int professor_leciona_disciplina(int matricula_prof, char *disciplina) {
    FILE *file = fopen("professores.txt", "r");
    if (file == NULL) return 0;  // Arquivo não existe
    
    char linha[400];
    while (fgets(linha, sizeof(linha), file)) {
        int mat;
        char materias[200];
        // Lê matrícula e lista de matérias do professor
        sscanf(linha, "%d|%*[^|]|%*[^|]|%*[^|]|%[^\n]", &mat, materias);
        
        if (mat == matricula_prof) {
            // Divide a string de matérias por vírgula e espaço
            char *token = strtok(materias, ", ");
            while (token != NULL) {
                if (strcasecmp(token, disciplina) == 0) {
                    fclose(file);
                    return 1;  // Professor leciona a disciplina
                }
                token = strtok(NULL, ", ");  // Próxima matéria
            }
            break;  // Professor encontrado, mas não leciona a disciplina
        }
    }
    fclose(file);
    return 0;  // Professor não leciona a disciplina
}

// Obtém o turno de uma turma específica pelo ID
// Preenche a string turno com o resultado
void obter_turno_turma(int turma_id, char *turno) {
    FILE *file = fopen("turmas.txt", "r");
    strcpy(turno, "");  // Inicializa como vazio
    if (file != NULL) {
        char linha[200];
        while (fgets(linha, sizeof(linha), file)) {
            int id;
            char turno_arquivo[20];
            // Lê ID e turno da linha
            sscanf(linha, "%d|%*[^|]|%*d|%[^|]", &id, turno_arquivo);
            if (id == turma_id) {
                strcpy(turno, turno_arquivo);  // Copia o turno encontrado
                break;  // Para a busca
            }
        }
        fclose(file);
    }
}

// Obtém o nome de uma turma específica pelo ID
// Preenche a string nome com o resultado
void obter_nome_turma(int turma_id, char *nome) {
    FILE *file = fopen("turmas.txt", "r");
    strcpy(nome, "Turma nao encontrada");  // Valor padrão se não encontrar
    if (file != NULL) {
        char linha[200];
        while (fgets(linha, sizeof(linha), file)) {
            int id;
            char nome_arquivo[100];
            // Lê ID e nome da turma
            sscanf(linha, "%d|%[^|]", &id, nome_arquivo);
            if (id == turma_id) {
                strcpy(nome, nome_arquivo);  // Copia o nome encontrado
                break;  // Para a busca
            }
        }
        fclose(file);
    }
}

// Obtém dados completos de uma turma (nome, turno, ano) pelo ID
// Formata os dados em uma string legível
void obter_dados_completos_turma(int turma_id, char *dados_completos) {
    FILE *file = fopen("turmas.txt", "r");
    strcpy(dados_completos, "Turma nao encontrada");  // Valor padrão
    if (file != NULL) {
        char linha[200];
        while (fgets(linha, sizeof(linha), file)) {
            int id, serie, ano;
            char nome[100], turno[20];
            // Lê todos os dados da turma
            sscanf(linha, "%d|%[^|]|%d|%[^|]|%d", &id, nome, &serie, turno, &ano);
            if (id == turma_id) {
                // Formata os dados principais
                sprintf(dados_completos, "%s | %s | %d", nome, turno, ano);
                break;  // Para a busca
            }
        }
        fclose(file);
    }
}

// Obtém o nome de um professor pela matrícula
// Preenche a string nome com o resultado
void obter_nome_professor(int matricula_prof, char *nome) {
    FILE *file = fopen("professores.txt", "r");
    strcpy(nome, "Professor nao encontrado");  // Valor padrão
    if (file != NULL) {
        char linha[400];
        while (fgets(linha, sizeof(linha), file)) {
            int mat;
            char nome_arquivo[100];
            // Lê matrícula e nome do professor
            sscanf(linha, "%d|%[^|]", &mat, nome_arquivo);
            if (mat == matricula_prof) {
                strcpy(nome, nome_arquivo);  // Copia o nome encontrado
                break;  // Para a busca
            }
        }
        fclose(file);
    }
}

// Gera um novo ID único para inserção em arquivo
// Lê o arquivo e retorna o próximo ID disponível (maior ID + 1)
int gerar_id(char *arquivo) {
    FILE *file = fopen(arquivo, "r");
    int maior_id = 0;
    char linha[500];
    
    if (file != NULL) {
        while (fgets(linha, sizeof(linha), file)) {
            int id;
            sscanf(linha, "%d", &id);  // Lê o primeiro número da linha (ID)
            if (id > maior_id) maior_id = id;  // Atualiza o maior ID encontrado
        }
        fclose(file);
    }
    return maior_id + 1;  // Retorna o próximo ID disponível
}

// Solicita confirmação do usuário para operações críticas
// Retorna 1 se confirmado (S), 0 se cancelado (N)
int confirmar_operacao(char *mensagem) {
    char confirmacao;
    do {
        printf("%s (S/N): ", mensagem);  // Exibe a mensagem de confirmação
        scanf(" %c", &confirmacao);
        limpar_buffer();
        confirmacao = toupper(confirmacao);  // Converte para maiúscula
        
        // Valida se a resposta é S ou N
        if (confirmacao != 'S' && confirmacao != 'N') {
            printf("Erro: Digite S para Sim ou N para Nao!\n");
        }
    } while (confirmacao != 'S' && confirmacao != 'N');  // Repete até resposta válida
    
    return confirmacao == 'S';  // Retorna 1 se confirmado, 0 se cancelado
}

// Valida se um ID é positivo e diferente de zero
// ID = 0 significa cancelamento, ID < 1 é inválido
int validar_id_positivo(int id) {
    if (id == 0) {
        printf("Operacao cancelada.\n");  // Zero significa cancelar
        return 0;
    }
    if (id < 1) {
        printf("Erro: ID deve ser um numero positivo!\n");  // Negativo é inválido
        return 0;
    }
    return 1;  // ID válido
}

// Verifica se um arquivo existe e pode ser aberto para leitura
// Exibe mensagem de erro personalizada se não existir
int verificar_arquivo_existe(char *arquivo, char *mensagem_erro) {
    FILE *file = fopen(arquivo, "r");
    if (file == NULL) {
        printf("%s\n", mensagem_erro);  // Exibe mensagem de erro
        pausar();
        return 0;  // Arquivo não existe
    }
    fclose(file);
    return 1;  // Arquivo existe
}

// Função principal de autenticação do sistema
// Verifica credenciais e define o tipo de usuário logado
int fazer_login() {
    char cpf[12], senha[20];
    
    printf("=== LOGIN ===\n");
    printf("CPF (11 digitos): ");
    scanf("%s", cpf);
    limpar_buffer();
    
    // Valida formato do CPF antes de prosseguir
    if (!validar_cpf(cpf)) {
        printf("CPF invalido!\n");
        pausar();
        return 0;
    }
    
    printf("Senha: ");
    scanf("%s", senha);
    limpar_buffer();
    
    // Verifica login do administrador (credenciais fixas)
    if (strcmp(cpf, "12345678909") == 0 && strcmp(senha, "admin") == 0) {
        admin_logado = 1;
        printf("Login admin realizado com sucesso!\n");
        pausar();
        return 1;
    }
    
    // Verifica login de aluno (busca no arquivo alunos.txt)
    FILE *file_aluno = fopen("alunos.txt", "r");
    if (file_aluno != NULL) {
        char linha[300];
        while (fgets(linha, sizeof(linha), file_aluno)) {
            int mat;
            char nome[100], cpf_arquivo[12], senha_arquivo[20];
            // Lê dados do aluno da linha
            sscanf(linha, "%d|%[^|]|%[^|]|%[^|]|%*s", &mat, nome, cpf_arquivo, senha_arquivo);
            
            // Compara CPF e senha
            if (strcmp(cpf, cpf_arquivo) == 0 && strcmp(senha, senha_arquivo) == 0) {
                aluno_logado = 1;
                matricula_atual = mat;  // Armazena matrícula para uso posterior
                printf("Login aluno realizado com sucesso! Bem-vindo, %s!\n", nome);
                fclose(file_aluno);
                pausar();
                return 1;
            }
        }
        fclose(file_aluno);
    }
    
    // Verifica login de professor (busca no arquivo professores.txt)
    FILE *file_prof = fopen("professores.txt", "r");
    if (file_prof != NULL) {
        char linha[400];
        while (fgets(linha, sizeof(linha), file_prof)) {
            int mat;
            char nome[100], cpf_arquivo[12], senha_arquivo[20];
            sscanf(linha, "%d|%[^|]|%[^|]|%[^|]|%*s", &mat, nome, cpf_arquivo, senha_arquivo);
            
            if (strcmp(cpf, cpf_arquivo) == 0 && strcmp(senha, senha_arquivo) == 0) {
                professor_logado = 1;
                matricula_atual = mat;
                printf("Login professor realizado com sucesso! Bem-vindo, %s!\n", nome);
                fclose(file_prof);
                pausar();
                return 1;
            }
        }
        fclose(file_prof);
    }
    
    printf("CPF ou senha incorretos!\n");
    pausar();
    return 0;
}

int selecionar_turma() {
    printf("=== TURMAS DISPONIVEIS ===\n");
    FILE *file = fopen("turmas.txt", "r");
    if (file == NULL) {
        printf("Nenhuma turma cadastrada!\n");
        pausar();
        return -1;
    }
    
    char linha[200];
    while (fgets(linha, sizeof(linha), file)) {
        int id, serie, ano;
        char nome[100], turno[20];
        sscanf(linha, "%d|%[^|]|%d|%[^|]|%d", &id, nome, &serie, turno, &ano);
        printf("ID: %d | Nome: %s | Turno: %s | Ano: %d\n", id, nome, turno, ano);
    }
    fclose(file);
    
    int turma_id;
    printf("\nID da turma (0 para cancelar): ");
    scanf("%d", &turma_id);
    limpar_buffer();
    
    if (turma_id == 0) {
        printf("Operacao cancelada.\n");
        pausar();
    }
    return turma_id;
}

int selecionar_aula_professor(int turma_id) {
    printf("\n=== MINHAS AULAS ===\n");
    FILE *file = fopen("aulas.txt", "r");
    if (file == NULL) {
        printf("Nenhuma aula registrada!\n");
        pausar();
        return -1;
    }
    
    char linha[300];
    int tem_aulas = 0;
    while (fgets(linha, sizeof(linha), file)) {
        int id, turma, prof_mat;
        char disciplina[50], data[11], horario[15];
        sscanf(linha, "%d|%d|%d|%[^|]|%[^|]|%s", &id, &turma, &prof_mat, disciplina, data, horario);
        
        if (turma == turma_id && prof_mat == matricula_atual) {
            printf("ID: %d | Disciplina: %s | Data: %s | Horario: %s\n", id, disciplina, data, horario);
            tem_aulas = 1;
        }
    }
    fclose(file);
    
    if (!tem_aulas) {
        printf("Nenhuma aula encontrada para esta turma!\n");
        pausar();
        return -1;
    }
    
    int aula_id;
    printf("\nID da aula (0 para cancelar): ");
    scanf("%d", &aula_id);
    limpar_buffer();
    
    if (aula_id == 0) {
        printf("Operacao cancelada.\n");
        pausar();
    }
    return aula_id;
}

void listar_alunos_turma(int turma_id) {
    printf("\n=== ALUNOS DA TURMA ===\n");
    FILE *file = fopen("alunos.txt", "r");
    if (file == NULL) {
        printf("Erro ao acessar alunos!\n");
        return;
    }
    
    char linha[300];
    while (fgets(linha, sizeof(linha), file)) {
        int mat, turma;
        char nome[100];
        sscanf(linha, "%d|%[^|]|%*[^|]|%*[^|]|%d", &mat, nome, &turma);
        if (turma == turma_id) {
            printf("Matricula: %d | Nome: %s\n", mat, nome);
        }
    }
    fclose(file);
}

void cadastrar_turma() {
    if (!admin_logado) {
        printf("Acesso negado!\n");
        pausar();
        return;
    }
    
    Turma nova_turma;
    nova_turma.id = gerar_id("turmas.txt");
    
    printf("=== CADASTRAR TURMA ===\n");
    
    do {
        printf("Serie (1, 2 ou 3): ");
        scanf("%d", &nova_turma.serie);
        limpar_buffer();
        if (nova_turma.serie < 1 || nova_turma.serie > 3) {
            printf("Erro: Serie deve ser 1, 2 ou 3!\n");
        }
    } while (nova_turma.serie < 1 || nova_turma.serie > 3);
    
    char letra;
    do {
        printf("Letra da turma (A-Z): ");
        scanf(" %c", &letra);
        limpar_buffer();
        
        // Converter para maiúscula
        letra = toupper(letra);
        
        if (letra < 'A' || letra > 'Z') {
            printf("Erro: Letra deve ser entre A e Z!\n");
        }
    } while (letra < 'A' || letra > 'Z');
    
    // Gerar nome automaticamente
    sprintf(nova_turma.nome, "%d Ano %c", nova_turma.serie, letra);
    
    do {
        printf("Turno (Matutino/Vespertino/Noturno): ");
        fgets(nova_turma.turno, 20, stdin);
        nova_turma.turno[strcspn(nova_turma.turno, "\n")] = 0;
        
        if (strcasecmp(nova_turma.turno, "Matutino") != 0 && 
            strcasecmp(nova_turma.turno, "Vespertino") != 0 && 
            strcasecmp(nova_turma.turno, "Noturno") != 0) {
            printf("Erro: Turno deve ser Matutino, Vespertino ou Noturno!\n");
        } else {
            // Padronizar primeira letra maiúscula
            if (strcasecmp(nova_turma.turno, "matutino") == 0) strcpy(nova_turma.turno, "Matutino");
            else if (strcasecmp(nova_turma.turno, "vespertino") == 0) strcpy(nova_turma.turno, "Vespertino");
            else if (strcasecmp(nova_turma.turno, "noturno") == 0) strcpy(nova_turma.turno, "Noturno");
        }
    } while (strcasecmp(nova_turma.turno, "Matutino") != 0 && 
             strcasecmp(nova_turma.turno, "Vespertino") != 0 && 
             strcasecmp(nova_turma.turno, "Noturno") != 0);
    
    do {
        printf("Ano letivo (2025 ou 2026): ");
        scanf("%d", &nova_turma.ano);
        limpar_buffer();
        if (nova_turma.ano != 2025 && nova_turma.ano != 2026) {
            printf("Erro: Ano letivo deve ser 2025 ou 2026!\n");
        }
    } while (nova_turma.ano != 2025 && nova_turma.ano != 2026);
    
    // Verificar se turma já existe (mesmo nome, turno e ano)
    FILE *file_check = fopen("turmas.txt", "r");
    if (file_check != NULL) {
        char linha[200];
        while (fgets(linha, sizeof(linha), file_check)) {
            int ano_existente, serie_existente;
            char nome_existente[100], turno_existente[20];
            sscanf(linha, "%*d|%[^|]|%d|%[^|]|%d", nome_existente, &serie_existente, turno_existente, &ano_existente);
            
            if (strcmp(nova_turma.nome, nome_existente) == 0 && 
                strcmp(nova_turma.turno, turno_existente) == 0 && 
                nova_turma.ano == ano_existente) {
                printf("Erro: Ja existe uma turma %s no turno %s para o ano %d!\n", 
                       nova_turma.nome, nova_turma.turno, nova_turma.ano);
                fclose(file_check);
                pausar();
                return;
            }
        }
        fclose(file_check);
    }
    
    FILE *file = fopen("turmas.txt", "a");
    if (file != NULL) {
        fprintf(file, "%d|%s|%d|%s|%d\n", nova_turma.id, nova_turma.nome, 
                nova_turma.serie, nova_turma.turno, nova_turma.ano);
        fclose(file);
        printf("Turma cadastrada com sucesso! ID: %d\n", nova_turma.id);
    } else {
        printf("Erro ao salvar turma!\n");
    }
    pausar();
}

void listar_turmas() {
    FILE *file = fopen("turmas.txt", "r");
    if (file == NULL) {
        printf("Nenhuma turma cadastrada!\n");
        pausar();
        return;
    }
    
    printf("=== TURMAS CADASTRADAS ===\n");
    char linha[200];
    while (fgets(linha, sizeof(linha), file)) {
        int id, serie, ano;
        char nome[100], turno[20];
        sscanf(linha, "%d|%[^|]|%d|%[^|]|%d", &id, nome, &serie, turno, &ano);
        printf("ID: %d | Nome: %s | Turno: %s | Ano: %d\n", 
               id, nome, turno, ano);
    }
    fclose(file);
    pausar();
}

void excluir_turma() {
    if (!admin_logado) {
        printf("Acesso negado!\n");
        pausar();
        return;
    }
    
    if (!verificar_arquivo_existe("turmas.txt", "Nenhuma turma cadastrada para excluir!")) return;
    
    listar_turmas();
    
    int id;
    printf("\nID da turma para excluir (0 para cancelar): ");
    scanf("%d", &id);
    limpar_buffer();
    
    if (!validar_id_positivo(id)) {
        pausar();
        return;
    }
    
    // Verificar se há aulas ou atividades vinculadas à turma
    int tem_aulas = 0, tem_atividades = 0;
    
    // Verificar aulas
    FILE *file_aulas = fopen("aulas.txt", "r");
    if (file_aulas != NULL) {
        char linha[300];
        while (fgets(linha, sizeof(linha), file_aulas)) {
            int turma_id;
            sscanf(linha, "%*d|%d", &turma_id);
            if (turma_id == id) {
                tem_aulas = 1;
                break;
            }
        }
        fclose(file_aulas);
    }
    
    // Verificar atividades
    FILE *file_atividades = fopen("atividades.txt", "r");
    if (file_atividades != NULL) {
        char linha[400];
        while (fgets(linha, sizeof(linha), file_atividades)) {
            int turma_id;
            sscanf(linha, "%*d|%*[^|]|%*[^|]|%d", &turma_id);
            if (turma_id == id) {
                tem_atividades = 1;
                break;
            }
        }
        fclose(file_atividades);
    }
    
    // Se há registros vinculados, perguntar confirmação
    if (tem_aulas || tem_atividades) {
        if (!confirmar_operacao("Atencao! Essa turma tem aulas e/ou atividades registradas, a exclusao da turma tambem apagara esses registros. Deseja prosseguir com a exclusao?")) {
            printf("Exclusao cancelada.\n");
            pausar();
            return;
        }
    }
    
    // Excluir aulas da turma
    if (tem_aulas) {
        FILE *file_aulas = fopen("aulas.txt", "r");
        FILE *temp_aulas = fopen("temp_aulas.txt", "w");
        
        if (file_aulas != NULL && temp_aulas != NULL) {
            char linha[300];
            while (fgets(linha, sizeof(linha), file_aulas)) {
                int turma_id;
                sscanf(linha, "%*d|%d", &turma_id);
                if (turma_id != id) {
                    fprintf(temp_aulas, "%s", linha);
                }
            }
            fclose(file_aulas);
            fclose(temp_aulas);
            remove("aulas.txt");
            rename("temp_aulas.txt", "aulas.txt");
        }
    }
    
    // Excluir atividades da turma
    if (tem_atividades) {
        FILE *file_atividades = fopen("atividades.txt", "r");
        FILE *temp_atividades = fopen("temp_atividades.txt", "w");
        
        if (file_atividades != NULL && temp_atividades != NULL) {
            char linha[400];
            while (fgets(linha, sizeof(linha), file_atividades)) {
                int turma_id;
                sscanf(linha, "%*d|%*[^|]|%*[^|]|%d", &turma_id);
                if (turma_id != id) {
                    fprintf(temp_atividades, "%s", linha);
                }
            }
            fclose(file_atividades);
            fclose(temp_atividades);
            remove("atividades.txt");
            rename("temp_atividades.txt", "atividades.txt");
        }
    }
    
    // Excluir a turma
    FILE *file = fopen("turmas.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    
    if (file == NULL || temp == NULL) {
        printf("Erro ao acessar arquivo!\n");
        pausar();
        return;
    }
    
    char linha[200];
    int encontrou = 0;
    
    while (fgets(linha, sizeof(linha), file)) {
        int id_arquivo;
        sscanf(linha, "%d", &id_arquivo);
        
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
        if (tem_aulas || tem_atividades) {
            printf("Aulas e atividades vinculadas tambem foram excluidas.\n");
        }
    } else {
        printf("Erro: Turma com ID %d nao encontrada!\n", id);
    }
    pausar();
}

void cadastrar_aluno() {
    if (!admin_logado) {
        printf("Acesso negado!\n");
        pausar();
        return;
    }
    
    Aluno novo_aluno;
    novo_aluno.matricula = gerar_id("alunos.txt");
    
    printf("=== CADASTRAR ALUNO ===\n");
    do {
        printf("Nome: ");
        fgets(novo_aluno.nome, 100, stdin);
        novo_aluno.nome[strcspn(novo_aluno.nome, "\n")] = 0;
        
        if (strlen(novo_aluno.nome) == 0) {
            printf("Erro: Nome do aluno nao pode estar vazio!\n");
        }
    } while (strlen(novo_aluno.nome) == 0);
    
    do {
        printf("CPF (11 digitos): ");
        scanf("%s", novo_aluno.cpf);
        limpar_buffer();
        if (!validar_cpf(novo_aluno.cpf)) {
            printf("Erro: CPF deve conter exatamente 11 digitos numericos!\n");
        }
    } while (!validar_cpf(novo_aluno.cpf));
    
    // Verificar se CPF já existe
    int cpf_existe = 0;
    do {
        cpf_existe = 0;
        FILE *file_check = fopen("alunos.txt", "r");
        if (file_check != NULL) {
            char linha[300];
            while (fgets(linha, sizeof(linha), file_check)) {
                char cpf_existente[12];
                sscanf(linha, "%*d|%*[^|]|%[^|]", cpf_existente);
                if (strcmp(novo_aluno.cpf, cpf_existente) == 0) {
                    printf("Erro: CPF ja cadastrado no sistema!\n");
                    cpf_existe = 1;
                    break;
                }
            }
            fclose(file_check);
        }
        if (cpf_existe) {
            printf("CPF (11 digitos): ");
            scanf("%s", novo_aluno.cpf);
            limpar_buffer();
            if (!validar_cpf(novo_aluno.cpf)) {
                printf("Erro: CPF deve conter exatamente 11 digitos numericos!\n");
                cpf_existe = 1;
            }
        }
    } while (cpf_existe);
    
    do {
        printf("Senha (minimo 4 caracteres): ");
        scanf("%s", novo_aluno.senha);
        limpar_buffer();
        if (strlen(novo_aluno.senha) < 4) {
            printf("Erro: Senha deve ter pelo menos 4 caracteres!\n");
        }
    } while (strlen(novo_aluno.senha) < 4);
    
    do {
        listar_turmas();
        printf("ID da turma (0 para sem turma): ");
        scanf("%d", &novo_aluno.turma_id);
        limpar_buffer();
        
        // Verificar se turma existe (se não for 0)
        if (novo_aluno.turma_id != 0) {
            FILE *file_turma = fopen("turmas.txt", "r");
            int turma_existe = 0;
            if (file_turma != NULL) {
                char linha[200];
                while (fgets(linha, sizeof(linha), file_turma)) {
                    int id_turma;
                    sscanf(linha, "%d", &id_turma);
                    if (id_turma == novo_aluno.turma_id) {
                        turma_existe = 1;
                        break;
                    }
                }
                fclose(file_turma);
            }
            if (!turma_existe) {
                printf("Erro: Turma com ID %d nao encontrada!\n", novo_aluno.turma_id);
            } else {
                break;
            }
        } else {
            break;
        }
    } while (1);
    
    FILE *file = fopen("alunos.txt", "a");
    if (file != NULL) {
        fprintf(file, "%d|%s|%s|%s|%d\n", novo_aluno.matricula, novo_aluno.nome,
                novo_aluno.cpf, novo_aluno.senha, novo_aluno.turma_id);
        fclose(file);
        printf("Aluno cadastrado com sucesso! Matricula: %d\n", novo_aluno.matricula);
    } else {
        printf("Erro ao salvar aluno!\n");
    }
    pausar();
}

void listar_alunos() {
    FILE *file = fopen("alunos.txt", "r");
    if (file == NULL) {
        printf("Nenhum aluno cadastrado!\n");
        pausar();
        return;
    }
    
    printf("=== ALUNOS CADASTRADOS ===\n");
    char linha[300];
    while (fgets(linha, sizeof(linha), file)) {
        int mat, turma_id;
        char nome[100], cpf[12], senha[20], nome_turma[100];
        sscanf(linha, "%d|%[^|]|%[^|]|%[^|]|%d", &mat, nome, cpf, senha, &turma_id);
        
        if (turma_id == 0) {
            strcpy(nome_turma, "Sem turma");
        } else {
            obter_dados_completos_turma(turma_id, nome_turma);
        }
        
        if (admin_logado) {
            printf("Matricula: %d | Nome: %s | CPF: %s | Senha: %s | Turma: %s\n", 
                   mat, nome, cpf, senha, nome_turma);
        } else {
            printf("Matricula: %d | Nome: %s | CPF: %s | Turma: %s\n", 
                   mat, nome, cpf, nome_turma);
        }
    }
    fclose(file);
    pausar();
}

void excluir_aluno() {
    if (!admin_logado) {
        printf("Acesso negado!\n");
        pausar();
        return;
    }
    
    if (!verificar_arquivo_existe("alunos.txt", "Nenhum aluno cadastrado para excluir!")) return;
    
    listar_alunos();
    
    int matricula;
    printf("\nMatricula do aluno para excluir (0 para cancelar): ");
    scanf("%d", &matricula);
    limpar_buffer();
    
    if (!validar_id_positivo(matricula)) {
        pausar();
        return;
    }
    
    FILE *file = fopen("alunos.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    
    if (file == NULL || temp == NULL) {
        printf("Erro ao acessar arquivo!\n");
        pausar();
        return;
    }
    
    char linha[300];
    int encontrou = 0;
    
    while (fgets(linha, sizeof(linha), file)) {
        int mat_arquivo;
        sscanf(linha, "%d", &mat_arquivo);
        
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
        printf("Erro: Aluno com matricula %d nao encontrado!\n", matricula);
    }
    pausar();
}

void minha_turma() {
    if (!aluno_logado) {
        printf("Acesso negado!\n");
        pausar();
        return;
    }
    
    FILE *file_aluno = fopen("alunos.txt", "r");
    if (file_aluno == NULL) {
        printf("Erro ao acessar dados!\n");
        pausar();
        return;
    }
    
    int turma_id = 0;
    char linha[300];
    
    while (fgets(linha, sizeof(linha), file_aluno)) {
        int mat;
        sscanf(linha, "%d|%*[^|]|%*[^|]|%*[^|]|%d", &mat, &turma_id);
        if (mat == matricula_atual) break;
    }
    fclose(file_aluno);
    
    if (turma_id == 0) {
        printf("Voce nao esta matriculado em nenhuma turma!\n");
        pausar();
        return;
    }
    
    FILE *file_turma = fopen("turmas.txt", "r");
    if (file_turma != NULL) {
        while (fgets(linha, sizeof(linha), file_turma)) {
            int id, serie, ano;
            char nome[100], turno[20];
            sscanf(linha, "%d|%[^|]|%d|%[^|]|%d", &id, nome, &serie, turno, &ano);
            
            if (id == turma_id) {
                printf("=== MINHA TURMA ===\n");
                printf("Nome: %s\n", nome);
                printf("Serie: %d\n", serie);
                printf("Turno: %s\n", turno);
                printf("Ano: %d\n", ano);
                break;
            }
        }
        fclose(file_turma);
    }
    pausar();
}

void cadastrar_professor() {
    if (!admin_logado) {
        printf("Acesso negado!\n");
        pausar();
        return;
    }
    
    Professor novo_prof;
    novo_prof.matricula = gerar_id("professores.txt");
    
    printf("=== CADASTRAR PROFESSOR ===\n");
    do {
        printf("Nome: ");
        fgets(novo_prof.nome, 100, stdin);
        novo_prof.nome[strcspn(novo_prof.nome, "\n")] = 0;
        
        if (strlen(novo_prof.nome) == 0) {
            printf("Erro: Nome do professor nao pode estar vazio!\n");
        }
    } while (strlen(novo_prof.nome) == 0);
    
    do {
        printf("CPF (11 digitos): ");
        scanf("%s", novo_prof.cpf);
        limpar_buffer();
        if (!validar_cpf(novo_prof.cpf)) {
            printf("Erro: CPF deve conter exatamente 11 digitos numericos!\n");
        }
    } while (!validar_cpf(novo_prof.cpf));
    
    // Verificar se CPF já existe
    FILE *file_check = fopen("professores.txt", "r");
    if (file_check != NULL) {
        char linha[400];
        while (fgets(linha, sizeof(linha), file_check)) {
            char cpf_existente[12];
            sscanf(linha, "%*d|%*[^|]|%[^|]", cpf_existente);
            if (strcmp(novo_prof.cpf, cpf_existente) == 0) {
                printf("Erro: CPF ja cadastrado no sistema!\n");
                fclose(file_check);
                pausar();
                return;
            }
        }
        fclose(file_check);
    }
    
    do {
        printf("Senha (minimo 4 caracteres): ");
        scanf("%s", novo_prof.senha);
        limpar_buffer();
        if (strlen(novo_prof.senha) < 4) {
            printf("Erro: Senha deve ter pelo menos 4 caracteres!\n");
        }
    } while (strlen(novo_prof.senha) < 4);
    
    do {
        printf("Materias disponiveis: Matematica, Ciencias, Geografia, Historia, Filosofia, Sociologia, Portugues, Ingles\n");
        printf("Materias (separadas por virgula): ");
        fgets(novo_prof.materias, 200, stdin);
        novo_prof.materias[strcspn(novo_prof.materias, "\n")] = 0;
        
        if (strlen(novo_prof.materias) == 0) {
            printf("Erro: Professor deve ter pelo menos uma materia!\n");
        } else {
            // Validar cada materia
            char materias_temp[200];
            strcpy(materias_temp, novo_prof.materias);
            char *token = strtok(materias_temp, ", ");
            int materias_validas = 1;
            
            while (token != NULL && materias_validas) {
                // Remover espacos no inicio
                while (*token == ' ') token++;
                
                if (!validar_disciplina(token)) {
                    printf("Erro: Materia '%s' invalida! Use apenas: Matematica, Ciencias, Geografia, Historia, Filosofia, Sociologia, Portugues, Ingles\n", token);
                    materias_validas = 0;
                }
                token = strtok(NULL, ", ");
            }
            
            if (materias_validas) break;
        }
    } while (1);
    
    FILE *file = fopen("professores.txt", "a");
    if (file != NULL) {
        fprintf(file, "%d|%s|%s|%s|%s\n", novo_prof.matricula, novo_prof.nome,
                novo_prof.cpf, novo_prof.senha, novo_prof.materias);
        fclose(file);
        printf("Professor cadastrado com sucesso! Matricula: %d\n", novo_prof.matricula);
    } else {
        printf("Erro ao salvar professor!\n");
    }
    pausar();
}

void listar_professores() {
    FILE *file = fopen("professores.txt", "r");
    if (file == NULL) {
        printf("Nenhum professor cadastrado!\n");
        pausar();
        return;
    }
    
    printf("=== PROFESSORES CADASTRADOS ===\n");
    char linha[400];
    while (fgets(linha, sizeof(linha), file)) {
        int mat;
        char nome[100], cpf[12], senha[20], materias[200];
        sscanf(linha, "%d|%[^|]|%[^|]|%[^|]|%[^\n]", &mat, nome, cpf, senha, materias);
        
        if (admin_logado) {
            printf("Matricula: %d | Nome: %s | CPF: %s | Senha: %s | Materias: %s\n", 
                   mat, nome, cpf, senha, materias);
        } else {
            printf("Matricula: %d | Nome: %s | CPF: %s | Materias: %s\n", 
                   mat, nome, cpf, materias);
        }
    }
    fclose(file);
    pausar();
}

void excluir_professor() {
    if (!admin_logado) {
        printf("Acesso negado!\n");
        pausar();
        return;
    }
    
    if (!verificar_arquivo_existe("professores.txt", "Nenhum professor cadastrado para excluir!")) return;
    
    listar_professores();
    
    int matricula;
    printf("\nMatricula do professor para excluir (0 para cancelar): ");
    scanf("%d", &matricula);
    limpar_buffer();
    
    if (matricula == 0) {
        printf("Operacao cancelada.\n");
        pausar();
        return;
    }
    
    if (matricula < 1) {
        printf("Erro: Matricula deve ser um numero positivo!\n");
        pausar();
        return;
    }
    
    // Verificar se há aulas ou atividades vinculadas ao professor
    int tem_aulas = 0, tem_atividades = 0;
    
    // Verificar aulas
    FILE *file_aulas = fopen("aulas.txt", "r");
    if (file_aulas != NULL) {
        char linha[300];
        while (fgets(linha, sizeof(linha), file_aulas)) {
            int prof_mat;
            sscanf(linha, "%*d|%*d|%d", &prof_mat);
            if (prof_mat == matricula) {
                tem_aulas = 1;
                break;
            }
        }
        fclose(file_aulas);
    }
    
    // Verificar atividades
    FILE *file_atividades = fopen("atividades.txt", "r");
    if (file_atividades != NULL) {
        char linha[400];
        while (fgets(linha, sizeof(linha), file_atividades)) {
            int prof_mat;
            sscanf(linha, "%*d|%*[^|]|%*[^|]|%*d|%d", &prof_mat);
            if (prof_mat == matricula) {
                tem_atividades = 1;
                break;
            }
        }
        fclose(file_atividades);
    }
    
    // Se há registros vinculados, perguntar confirmação
    if (tem_aulas || tem_atividades) {
        if (!confirmar_operacao("Atencao! Esse professor tem aulas e/ou atividades registradas, a exclusao da turma tambem apagara esses registros. Deseja prosseguir com a exclusao?")) {
            printf("Exclusao cancelada.\n");
            pausar();
            return;
        }
    }
    
    // Excluir aulas do professor
    if (tem_aulas) {
        FILE *file_aulas = fopen("aulas.txt", "r");
        FILE *temp_aulas = fopen("temp_aulas.txt", "w");
        
        if (file_aulas != NULL && temp_aulas != NULL) {
            char linha[300];
            while (fgets(linha, sizeof(linha), file_aulas)) {
                int prof_mat;
                sscanf(linha, "%*d|%*d|%d", &prof_mat);
                if (prof_mat != matricula) {
                    fprintf(temp_aulas, "%s", linha);
                }
            }
            fclose(file_aulas);
            fclose(temp_aulas);
            remove("aulas.txt");
            rename("temp_aulas.txt", "aulas.txt");
        }
    }
    
    // Excluir atividades do professor
    if (tem_atividades) {
        FILE *file_atividades = fopen("atividades.txt", "r");
        FILE *temp_atividades = fopen("temp_atividades.txt", "w");
        
        if (file_atividades != NULL && temp_atividades != NULL) {
            char linha[400];
            while (fgets(linha, sizeof(linha), file_atividades)) {
                int prof_mat;
                sscanf(linha, "%*d|%*[^|]|%*[^|]|%*d|%d", &prof_mat);
                if (prof_mat != matricula) {
                    fprintf(temp_atividades, "%s", linha);
                }
            }
            fclose(file_atividades);
            fclose(temp_atividades);
            remove("atividades.txt");
            rename("temp_atividades.txt", "atividades.txt");
        }
    }
    
    FILE *file = fopen("professores.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    
    if (file == NULL || temp == NULL) {
        printf("Erro ao acessar arquivo!\n");
        pausar();
        return;
    }
    
    char linha[400];
    int encontrou = 0;
    
    while (fgets(linha, sizeof(linha), file)) {
        int mat_arquivo;
        sscanf(linha, "%d", &mat_arquivo);
        
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
        if (tem_aulas || tem_atividades) {
            printf("Aulas e atividades vinculadas tambem foram excluidas.\n");
        }
    } else {
        printf("Erro: Professor com matricula %d nao encontrado!\n", matricula);
    }
    pausar();
}

void registrar_aula() {
    if (!admin_logado && !professor_logado) {
        printf("Acesso negado!\n");
        pausar();
        return;
    }
    
    Aula nova_aula;
    nova_aula.id = gerar_id("aulas.txt");
    
    printf("=== REGISTRAR AULA ===\n");
    
    listar_turmas();
    printf("ID da turma: ");
    scanf("%d", &nova_aula.turma_id);
    limpar_buffer();
    
    // Verificar se turma existe
    int turma_existe = 0;
    do {
        FILE *file_turma = fopen("turmas.txt", "r");
        turma_existe = 0;
        if (file_turma != NULL) {
            char linha[200];
            while (fgets(linha, sizeof(linha), file_turma)) {
                int id_turma;
                sscanf(linha, "%d", &id_turma);
                if (id_turma == nova_aula.turma_id) {
                    turma_existe = 1;
                    break;
                }
            }
            fclose(file_turma);
        }
        if (!turma_existe) {
            printf("Erro: Turma com ID %d nao encontrada!\n", nova_aula.turma_id);
            listar_turmas();
            printf("ID da turma: ");
            scanf("%d", &nova_aula.turma_id);
            limpar_buffer();
        }
    } while (!turma_existe);
    
    if (professor_logado) {
        nova_aula.professor_matricula = matricula_atual;
    } else {
        listar_professores();
        printf("Matricula do professor: ");
        scanf("%d", &nova_aula.professor_matricula);
        limpar_buffer();
        
        // Verificar se professor existe
        int prof_existe = 0;
        do {
            FILE *file_prof = fopen("professores.txt", "r");
            prof_existe = 0;
            if (file_prof != NULL) {
                char linha[400];
                while (fgets(linha, sizeof(linha), file_prof)) {
                    int mat_prof;
                    sscanf(linha, "%d", &mat_prof);
                    if (mat_prof == nova_aula.professor_matricula) {
                        prof_existe = 1;
                        break;
                    }
                }
                fclose(file_prof);
            }
            if (!prof_existe) {
                printf("Erro: Professor com matricula %d nao encontrado!\n", nova_aula.professor_matricula);
                listar_professores();
                printf("Matricula do professor: ");
                scanf("%d", &nova_aula.professor_matricula);
                limpar_buffer();
            }
        } while (!prof_existe);
    }
    
    // Obter disciplinas do professor
    char disciplinas_prof[200] = "";
    FILE *file_disc = fopen("professores.txt", "r");
    if (file_disc != NULL) {
        char linha[400];
        while (fgets(linha, sizeof(linha), file_disc)) {
            int mat;
            char materias[200];
            sscanf(linha, "%d|%*[^|]|%*[^|]|%*[^|]|%[^\n]", &mat, materias);
            if (mat == nova_aula.professor_matricula) {
                strcpy(disciplinas_prof, materias);
                break;
            }
        }
        fclose(file_disc);
    }
    
    printf("Disciplinas do professor: %s\n", disciplinas_prof);
    
    do {
        printf("Disciplina: ");
        fgets(nova_aula.disciplina, 50, stdin);
        nova_aula.disciplina[strcspn(nova_aula.disciplina, "\n")] = 0;
        
        if (strlen(nova_aula.disciplina) == 0) {
            printf("Erro: Disciplina nao pode estar vazia!\n");
        } else if (!professor_leciona_disciplina(nova_aula.professor_matricula, nova_aula.disciplina)) {
            printf("Erro: Professor nao leciona a disciplina %s!\n", nova_aula.disciplina);
            printf("Disciplinas disponiveis: %s\n", disciplinas_prof);
        }
    } while (strlen(nova_aula.disciplina) == 0 || !professor_leciona_disciplina(nova_aula.professor_matricula, nova_aula.disciplina));
    
    int ano_turma = 0;
    // Buscar ano da turma primeiro
    FILE *file_turma_ano = fopen("turmas.txt", "r");
    if (file_turma_ano != NULL) {
        char linha[200];
        while (fgets(linha, sizeof(linha), file_turma_ano)) {
            int id_turma;
            sscanf(linha, "%d|%*[^|]|%*d|%*[^|]|%d", &id_turma, &ano_turma);
            if (id_turma == nova_aula.turma_id) {
                break;
            }
        }
        fclose(file_turma_ano);
    }
    
    do {
        printf("Data (DD/MM/AAAA): ");
        scanf("%s", nova_aula.data);
        limpar_buffer();
        
        if (strlen(nova_aula.data) != 10 || nova_aula.data[2] != '/' || nova_aula.data[5] != '/') {
            printf("Erro: Data deve estar no formato DD/MM/AAAA!\n");
        } else {
            int ano_data = atoi(&nova_aula.data[6]);
            if (ano_data != ano_turma) {
                printf("Erro: Ano da aula (%d) deve coincidir com o ano da turma (%d)!\n", ano_data, ano_turma);
            }
        }
    } while (strlen(nova_aula.data) != 10 || nova_aula.data[2] != '/' || nova_aula.data[5] != '/' || atoi(&nova_aula.data[6]) != ano_turma);
    
    // Obter turno da turma para mostrar horários corretos
    char turno_turma[20];
    obter_turno_turma(nova_aula.turma_id, turno_turma);
    
    int opcao_horario;
    do {
        printf("\nHorarios disponiveis para turno %s:\n", turno_turma);
        
        if (strcmp(turno_turma, "Matutino") == 0) {
            printf("1. 07:00-07:50 (1a Aula)\n");
            printf("2. 07:50-08:40 (2a Aula)\n");
            printf("3. 08:40-09:30 (3a Aula)\n");
            printf("4. 10:00-10:50 (4a Aula)\n");
            printf("5. 10:50-11:40 (5a Aula)\n");
        } else if (strcmp(turno_turma, "Vespertino") == 0) {
            printf("1. 13:00-13:50 (1a Aula)\n");
            printf("2. 13:50-14:40 (2a Aula)\n");
            printf("3. 14:40-15:30 (3a Aula)\n");
            printf("4. 16:00-16:50 (4a Aula)\n");
            printf("5. 16:50-17:40 (5a Aula)\n");
        } else if (strcmp(turno_turma, "Noturno") == 0) {
            printf("1. 18:00-18:50 (1a Aula)\n");
            printf("2. 18:50-19:40 (2a Aula)\n");
            printf("3. 19:40-20:30 (3a Aula)\n");
            printf("4. 21:00-21:50 (4a Aula)\n");
            printf("5. 21:50-22:40 (5a Aula)\n");
        }
        
        printf("Escolha o horario (1-5): ");
        scanf("%d", &opcao_horario);
        limpar_buffer();
        
        if (opcao_horario < 1 || opcao_horario > 5) {
            printf("Erro: Opcao deve ser entre 1 e 5!\n");
        } else {
            // Definir horário baseado na opção e turno
            if (strcmp(turno_turma, "Matutino") == 0) {
                switch (opcao_horario) {
                    case 1: strcpy(nova_aula.horario, "07:00-07:50"); break;
                    case 2: strcpy(nova_aula.horario, "07:50-08:40"); break;
                    case 3: strcpy(nova_aula.horario, "08:40-09:30"); break;
                    case 4: strcpy(nova_aula.horario, "10:00-10:50"); break;
                    case 5: strcpy(nova_aula.horario, "10:50-11:40"); break;
                }
            } else if (strcmp(turno_turma, "Vespertino") == 0) {
                switch (opcao_horario) {
                    case 1: strcpy(nova_aula.horario, "13:00-13:50"); break;
                    case 2: strcpy(nova_aula.horario, "13:50-14:40"); break;
                    case 3: strcpy(nova_aula.horario, "14:40-15:30"); break;
                    case 4: strcpy(nova_aula.horario, "16:00-16:50"); break;
                    case 5: strcpy(nova_aula.horario, "16:50-17:40"); break;
                }
            } else if (strcmp(turno_turma, "Noturno") == 0) {
                switch (opcao_horario) {
                    case 1: strcpy(nova_aula.horario, "18:00-18:50"); break;
                    case 2: strcpy(nova_aula.horario, "18:50-19:40"); break;
                    case 3: strcpy(nova_aula.horario, "19:40-20:30"); break;
                    case 4: strcpy(nova_aula.horario, "21:00-21:50"); break;
                    case 5: strcpy(nova_aula.horario, "21:50-22:40"); break;
                }
            }
        }
    } while (opcao_horario < 1 || opcao_horario > 5);
    
    FILE *file = fopen("aulas.txt", "a");
    if (file != NULL) {
        fprintf(file, "%d|%d|%d|%s|%s|%s\n", nova_aula.id, nova_aula.turma_id,
                nova_aula.professor_matricula, nova_aula.disciplina, 
                nova_aula.data, nova_aula.horario);
        fclose(file);
        printf("Aula registrada com sucesso! ID: %d\n", nova_aula.id);
    } else {
        printf("Erro ao salvar aula!\n");
    }
    pausar();
}

void listar_aulas() {
    FILE *file = fopen("aulas.txt", "r");
    if (file == NULL) {
        printf("Nenhuma aula registrada!\n");
        pausar();
        return;
    }
    
    printf("=== AULAS REGISTRADAS ===\n");
    char linha[300];
    while (fgets(linha, sizeof(linha), file)) {
        int id, turma_id, prof_mat;
        char disciplina[50], data[11], horario[15], nome_turma[100];
        sscanf(linha, "%d|%d|%d|%[^|]|%[^|]|%s", &id, &turma_id, &prof_mat,
               disciplina, data, horario);
        char nome_professor[100];
        obter_dados_completos_turma(turma_id, nome_turma);
        obter_nome_professor(prof_mat, nome_professor);
        printf("ID: %d | Turma: %s | Professor: %s | Disciplina: %s | Data: %s | Horario: %s\n",
               id, nome_turma, nome_professor, disciplina, data, horario);
    }
    fclose(file);
    pausar();
}

void excluir_aula() {
    if (!admin_logado && !professor_logado) {
        printf("Acesso negado!\n");
        pausar();
        return;
    }
    
    if (!verificar_arquivo_existe("aulas.txt", "Nenhuma aula registrada para excluir!")) return;
    
    listar_aulas();
    
    int id;
    printf("\nID da aula para excluir (0 para cancelar): ");
    scanf("%d", &id);
    limpar_buffer();
    
    if (id == 0) {
        printf("Operacao cancelada.\n");
        pausar();
        return;
    }
    
    if (id < 1) {
        printf("Erro: ID deve ser um numero positivo!\n");
        pausar();
        return;
    }
    
    FILE *file = fopen("aulas.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    
    if (file == NULL || temp == NULL) {
        printf("Erro ao acessar arquivo!\n");
        pausar();
        return;
    }
    
    char linha[300];
    int encontrou = 0;
    
    while (fgets(linha, sizeof(linha), file)) {
        int id_arquivo, prof_mat;
        sscanf(linha, "%d|%*d|%d", &id_arquivo, &prof_mat);
        
        if (id_arquivo != id) {
            fprintf(temp, "%s", linha);
        } else if (professor_logado && prof_mat != matricula_atual) {
            fprintf(temp, "%s", linha);
            printf("Erro: Voce so pode excluir suas proprias aulas!\n");
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
        printf("Erro: Aula com ID %d nao encontrada ou voce nao tem permissao!\n", id);
    }
    pausar();
}

void minhas_aulas() {
    if (!aluno_logado && !professor_logado) {
        printf("Acesso negado!\n");
        pausar();
        return;
    }
    
    FILE *file = fopen("aulas.txt", "r");
    if (file == NULL) {
        printf("Nenhuma aula registrada!\n");
        pausar();
        return;
    }
    
    printf("=== MINHAS AULAS ===\n");
    char linha[300];
    int encontrou = 0;
    
    while (fgets(linha, sizeof(linha), file)) {
        int id, turma_id, prof_mat;
        char disciplina[50], data[11], horario[15];
        sscanf(linha, "%d|%d|%d|%[^|]|%[^|]|%s", &id, &turma_id, &prof_mat,
               disciplina, data, horario);
        
        int mostrar = 0;
        
        if (professor_logado && prof_mat == matricula_atual) {
            mostrar = 1;
        } else if (aluno_logado) {
            // Verificar se aluno está na turma
            FILE *file_aluno = fopen("alunos.txt", "r");
            if (file_aluno != NULL) {
                char linha_aluno[300];
                while (fgets(linha_aluno, sizeof(linha_aluno), file_aluno)) {
                    int mat, turma_aluno;
                    sscanf(linha_aluno, "%d|%*[^|]|%*[^|]|%*[^|]|%d", &mat, &turma_aluno);
                    if (mat == matricula_atual && turma_aluno == turma_id) {
                        mostrar = 1;
                        break;
                    }
                }
                fclose(file_aluno);
            }
        }
        
        if (mostrar) {
            char nome_turma[100];
            obter_dados_completos_turma(turma_id, nome_turma);
            printf("ID: %d | Turma: %s | Disciplina: %s | Data: %s | Horario: %s\n",
                   id, nome_turma, disciplina, data, horario);
            encontrou = 1;
        }
    }
    
    fclose(file);
    
    if (!encontrou) {
        printf("Nenhuma aula encontrada!\n");
    }
    pausar();
}

void lancar_atividade() {
    if (!admin_logado && !professor_logado) {
        printf("Acesso negado!\n");
        pausar();
        return;
    }
    
    Atividade nova_atividade;
    nova_atividade.id = gerar_id("atividades.txt");
    
    printf("=== LANCAR ATIVIDADE ===\n");
    
    do {
        printf("Tipo (Prova/Trabalho/Exercicio): ");
        fgets(nova_atividade.tipo, 50, stdin);
        nova_atividade.tipo[strcspn(nova_atividade.tipo, "\n")] = 0;
        
        if (strcasecmp(nova_atividade.tipo, "Prova") != 0 && 
            strcasecmp(nova_atividade.tipo, "Trabalho") != 0 && 
            strcasecmp(nova_atividade.tipo, "Exercicio") != 0) {
            printf("Erro: Tipo deve ser Prova, Trabalho ou Exercicio!\n");
        } else {
            // Padronizar primeira letra maiúscula
            if (strcasecmp(nova_atividade.tipo, "prova") == 0) strcpy(nova_atividade.tipo, "Prova");
            else if (strcasecmp(nova_atividade.tipo, "trabalho") == 0) strcpy(nova_atividade.tipo, "Trabalho");
            else if (strcasecmp(nova_atividade.tipo, "exercicio") == 0) strcpy(nova_atividade.tipo, "Exercicio");
        }
    } while (strcasecmp(nova_atividade.tipo, "Prova") != 0 && 
             strcasecmp(nova_atividade.tipo, "Trabalho") != 0 && 
             strcasecmp(nova_atividade.tipo, "Exercicio") != 0);
    
    do {
        printf("Nome da atividade: ");
        fgets(nova_atividade.nome, 100, stdin);
        nova_atividade.nome[strcspn(nova_atividade.nome, "\n")] = 0;
        
        if (strlen(nova_atividade.nome) == 0) {
            printf("Erro: Nome da atividade nao pode estar vazio!\n");
        }
    } while (strlen(nova_atividade.nome) == 0);
    
    listar_turmas();
    printf("ID da turma: ");
    scanf("%d", &nova_atividade.turma_id);
    limpar_buffer();
    
    // Verificar se turma existe
    FILE *file_turma = fopen("turmas.txt", "r");
    int turma_existe = 0;
    if (file_turma != NULL) {
        char linha[200];
        while (fgets(linha, sizeof(linha), file_turma)) {
            int id_turma;
            sscanf(linha, "%d", &id_turma);
            if (id_turma == nova_atividade.turma_id) {
                turma_existe = 1;
                break;
            }
        }
        fclose(file_turma);
    }
    if (!turma_existe) {
        printf("Erro: Turma com ID %d nao encontrada!\n", nova_atividade.turma_id);
        pausar();
        return;
    }
    
    if (professor_logado) {
        nova_atividade.professor_matricula = matricula_atual;
    } else {
        listar_professores();
        printf("Matricula do professor: ");
        scanf("%d", &nova_atividade.professor_matricula);
        limpar_buffer();
        
        // Verificar se professor existe
        FILE *file_prof = fopen("professores.txt", "r");
        int prof_existe = 0;
        if (file_prof != NULL) {
            char linha[400];
            while (fgets(linha, sizeof(linha), file_prof)) {
                int mat_prof;
                sscanf(linha, "%d", &mat_prof);
                if (mat_prof == nova_atividade.professor_matricula) {
                    prof_existe = 1;
                    break;
                }
            }
            fclose(file_prof);
        }
        if (!prof_existe) {
            printf("Erro: Professor com matricula %d nao encontrado!\n", nova_atividade.professor_matricula);
            pausar();
            return;
        }
    }
    
    // Obter disciplinas do professor
    char disciplinas_prof[200] = "";
    FILE *file_disc = fopen("professores.txt", "r");
    if (file_disc != NULL) {
        char linha[400];
        while (fgets(linha, sizeof(linha), file_disc)) {
            int mat;
            char materias[200];
            sscanf(linha, "%d|%*[^|]|%*[^|]|%*[^|]|%[^\n]", &mat, materias);
            if (mat == nova_atividade.professor_matricula) {
                strcpy(disciplinas_prof, materias);
                break;
            }
        }
        fclose(file_disc);
    }
    
    printf("Disciplinas do professor: %s\n", disciplinas_prof);
    
    do {
        printf("Disciplina: ");
        fgets(nova_atividade.disciplina, 50, stdin);
        nova_atividade.disciplina[strcspn(nova_atividade.disciplina, "\n")] = 0;
        
        if (strlen(nova_atividade.disciplina) == 0) {
            printf("Erro: Disciplina nao pode estar vazia!\n");
        } else if (!professor_leciona_disciplina(nova_atividade.professor_matricula, nova_atividade.disciplina)) {
            printf("Erro: Professor nao leciona a disciplina %s!\n", nova_atividade.disciplina);
            printf("Disciplinas disponiveis: %s\n", disciplinas_prof);
        }
    } while (strlen(nova_atividade.disciplina) == 0 || !professor_leciona_disciplina(nova_atividade.professor_matricula, nova_atividade.disciplina));
    
    int ano_turma = 0;
    // Buscar ano da turma primeiro
    FILE *file_turma_ano = fopen("turmas.txt", "r");
    if (file_turma_ano != NULL) {
        char linha[200];
        while (fgets(linha, sizeof(linha), file_turma_ano)) {
            int id_turma;
            sscanf(linha, "%d|%*[^|]|%*d|%*[^|]|%d", &id_turma, &ano_turma);
            if (id_turma == nova_atividade.turma_id) {
                break;
            }
        }
        fclose(file_turma_ano);
    }
    
    do {
        printf("Data (DD/MM/AAAA): ");
        scanf("%s", nova_atividade.data);
        limpar_buffer();
        
        if (strlen(nova_atividade.data) != 10 || nova_atividade.data[2] != '/' || nova_atividade.data[5] != '/') {
            printf("Erro: Data deve estar no formato DD/MM/AAAA!\n");
        } else {
            int ano_data = atoi(&nova_atividade.data[6]);
            if (ano_data != ano_turma) {
                printf("Erro: Ano da atividade (%d) deve coincidir com o ano da turma (%d)!\n", ano_data, ano_turma);
            }
        }
    } while (strlen(nova_atividade.data) != 10 || nova_atividade.data[2] != '/' || nova_atividade.data[5] != '/' || atoi(&nova_atividade.data[6]) != ano_turma);
    
    FILE *file = fopen("atividades.txt", "a");
    if (file != NULL) {
        fprintf(file, "%d|%s|%s|%d|%d|%s|%s\n", nova_atividade.id, nova_atividade.tipo,
                nova_atividade.nome, nova_atividade.turma_id, nova_atividade.professor_matricula,
                nova_atividade.disciplina, nova_atividade.data);
        fclose(file);
        printf("Atividade lancada com sucesso! ID: %d\n", nova_atividade.id);
    } else {
        printf("Erro ao salvar atividade!\n");
    }
    pausar();
}

void consultar_atividades() {
    FILE *file = fopen("atividades.txt", "r");
    if (file == NULL) {
        printf("Nenhuma atividade registrada!\n");
        pausar();
        return;
    }
    
    printf("=== ATIVIDADES ===\n");
    char linha[400];
    int encontrou = 0;
    
    while (fgets(linha, sizeof(linha), file)) {
        int id, turma_id, prof_mat;
        char tipo[50], nome[100], disciplina[50], data[11];
        sscanf(linha, "%d|%[^|]|%[^|]|%d|%d|%[^|]|%s", &id, tipo, nome, 
               &turma_id, &prof_mat, disciplina, data);
        
        int mostrar = 0;
        
        if (admin_logado) {
            mostrar = 1;
        } else if (professor_logado && prof_mat == matricula_atual) {
            mostrar = 1;
        } else if (aluno_logado) {
            // Verificar se aluno está na turma
            FILE *file_aluno = fopen("alunos.txt", "r");
            if (file_aluno != NULL) {
                char linha_aluno[300];
                while (fgets(linha_aluno, sizeof(linha_aluno), file_aluno)) {
                    int mat, turma_aluno;
                    sscanf(linha_aluno, "%d|%*[^|]|%*[^|]|%*[^|]|%d", &mat, &turma_aluno);
                    if (mat == matricula_atual && turma_aluno == turma_id) {
                        mostrar = 1;
                        break;
                    }
                }
                fclose(file_aluno);
            }
        }
        
        if (mostrar) {
            char nome_turma[100];
            obter_dados_completos_turma(turma_id, nome_turma);
            printf("ID: %d | Tipo: %s | Nome: %s | Turma: %s | Disciplina: %s | Data: %s\n",
                   id, tipo, nome, nome_turma, disciplina, data);
            encontrou = 1;
        }
    }
    
    fclose(file);
    
    if (!encontrou) {
        printf("Nenhuma atividade encontrada!\n");
    }
    pausar();
}

void excluir_atividade() {
    if (!admin_logado && !professor_logado) {
        printf("Acesso negado!\n");
        pausar();
        return;
    }
    
    if (!verificar_arquivo_existe("atividades.txt", "Nenhuma atividade registrada para excluir!")) return;
    
    consultar_atividades();
    
    int id;
    printf("\nID da atividade para excluir (0 para cancelar): ");
    scanf("%d", &id);
    limpar_buffer();
    
    if (id == 0) {
        printf("Operacao cancelada.\n");
        pausar();
        return;
    }
    
    if (id < 1) {
        printf("Erro: ID deve ser um numero positivo!\n");
        pausar();
        return;
    }
    
    FILE *file = fopen("atividades.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    
    if (file == NULL || temp == NULL) {
        printf("Erro ao acessar arquivo!\n");
        pausar();
        return;
    }
    
    char linha[400];
    int encontrou = 0;
    
    while (fgets(linha, sizeof(linha), file)) {
        int id_arquivo, prof_mat;
        sscanf(linha, "%d|%*[^|]|%*[^|]|%*d|%d", &id_arquivo, &prof_mat);
        
        if (id_arquivo != id) {
            fprintf(temp, "%s", linha);
        } else if (professor_logado && prof_mat != matricula_atual) {
            fprintf(temp, "%s", linha);
            printf("Erro: Voce so pode excluir suas proprias atividades!\n");
        } else {
            encontrou = 1;
        }
    }
    
    fclose(file);
    fclose(temp);
    
    remove("atividades.txt");
    rename("temp.txt", "atividades.txt");
    
    if (encontrou) {
        printf("Atividade excluida com sucesso!\n");
    } else {
        printf("Erro: Atividade com ID %d nao encontrada ou voce nao tem permissao!\n", id);
    }
    pausar();
}

void lancar_notas() {
    if (!admin_logado && !professor_logado) {
        printf("Acesso negado!\n");
        pausar();
        return;
    }
    
    consultar_atividades();
    
    int atividade_id;
    printf("\nID da atividade para lancar notas (0 para cancelar): ");
    scanf("%d", &atividade_id);
    limpar_buffer();
    
    if (atividade_id == 0) {
        printf("Operacao cancelada.\n");
        pausar();
        return;
    }
    
    // Verificar se atividade existe
    FILE *file_ativ = fopen("atividades.txt", "r");
    int ativ_existe = 0, turma_ativ = 0, prof_ativ = 0;
    if (file_ativ != NULL) {
        char linha[400];
        while (fgets(linha, sizeof(linha), file_ativ)) {
            int id, turma, prof;
            sscanf(linha, "%d|%*[^|]|%*[^|]|%d|%d", &id, &turma, &prof);
            if (id == atividade_id) {
                ativ_existe = 1;
                turma_ativ = turma;
                prof_ativ = prof;
                break;
            }
        }
        fclose(file_ativ);
    }
    
    if (!ativ_existe) {
        printf("Erro: Atividade nao encontrada!\n");
        pausar();
        return;
    }
    
    if (professor_logado && prof_ativ != matricula_atual) {
        printf("Erro: Voce so pode lancar notas para suas atividades!\n");
        pausar();
        return;
    }
    
    // Listar alunos da turma
    printf("\n=== ALUNOS DA TURMA ===\n");
    FILE *file_alunos = fopen("alunos.txt", "r");
    if (file_alunos == NULL) {
        printf("Erro ao acessar alunos!\n");
        pausar();
        return;
    }
    
    char linha[300];
    int tem_alunos = 0;
    while (fgets(linha, sizeof(linha), file_alunos)) {
        int mat, turma;
        char nome[100];
        sscanf(linha, "%d|%[^|]|%*[^|]|%*[^|]|%d", &mat, nome, &turma);
        if (turma == turma_ativ) {
            printf("Matricula: %d | Nome: %s\n", mat, nome);
            tem_alunos = 1;
        }
    }
    fclose(file_alunos);
    
    if (!tem_alunos) {
        printf("Nenhum aluno encontrado na turma!\n");
        pausar();
        return;
    }
    
    int matricula_aluno;
    float nota;
    
    do {
        printf("\nMatricula do aluno (0 para finalizar): ");
        scanf("%d", &matricula_aluno);
        limpar_buffer();
        
        if (matricula_aluno == 0) break;
        
        // Verificar se aluno existe na turma e obter nome
        FILE *file_check = fopen("alunos.txt", "r");
        int aluno_valido = 0;
        char nome_aluno[100] = "";
        if (file_check != NULL) {
            char linha_check[300];
            while (fgets(linha_check, sizeof(linha_check), file_check)) {
                int mat, turma;
                char nome[100];
                sscanf(linha_check, "%d|%[^|]|%*[^|]|%*[^|]|%d", &mat, nome, &turma);
                if (mat == matricula_aluno && turma == turma_ativ) {
                    aluno_valido = 1;
                    strcpy(nome_aluno, nome);
                    break;
                }
            }
            fclose(file_check);
        }
        
        if (!aluno_valido) {
            printf("Erro: Aluno nao encontrado na turma!\n");
            continue;
        }
        
        printf("Aluno selecionado: %s\n", nome_aluno);
        
        // Verificar se ja tem nota
        FILE *file_notas = fopen("notas.txt", "r");
        int ja_tem_nota = 0;
        if (file_notas != NULL) {
            char linha_nota[200];
            while (fgets(linha_nota, sizeof(linha_nota), file_notas)) {
                int ativ, aluno;
                sscanf(linha_nota, "%*d|%d|%d", &ativ, &aluno);
                if (ativ == atividade_id && aluno == matricula_aluno) {
                    ja_tem_nota = 1;
                    break;
                }
            }
            fclose(file_notas);
        }
        
        if (ja_tem_nota && !confirmar_operacao("Aluno ja possui nota para esta atividade. Deseja atualizar?")) {
            continue;
        }
        
        do {
            printf("Nota (0.0 a 10.0): ");
            scanf("%f", &nota);
            limpar_buffer();
            
            if (nota < 0.0 || nota > 10.0) {
                printf("Erro: Nota deve estar entre 0.0 e 10.0!\n");
            }
        } while (nota < 0.0 || nota > 10.0);
        
        if (ja_tem_nota) {
            // Atualizar nota existente
            FILE *file_notas = fopen("notas.txt", "r");
            FILE *temp_notas = fopen("temp_notas.txt", "w");
            
            if (file_notas != NULL && temp_notas != NULL) {
                char linha_nota[200];
                while (fgets(linha_nota, sizeof(linha_nota), file_notas)) {
                    int id_nota, ativ, aluno;
                    float nota_antiga;
                    sscanf(linha_nota, "%d|%d|%d|%f", &id_nota, &ativ, &aluno, &nota_antiga);
                    
                    if (ativ == atividade_id && aluno == matricula_aluno) {
                        fprintf(temp_notas, "%d|%d|%d|%.1f\n", id_nota, ativ, aluno, nota);
                    } else {
                        fprintf(temp_notas, "%s", linha_nota);
                    }
                }
                fclose(file_notas);
                fclose(temp_notas);
                remove("notas.txt");
                rename("temp_notas.txt", "notas.txt");
                printf("Nota atualizada com sucesso!\n");
            } else {
                printf("Erro ao atualizar nota!\n");
            }
        } else {
            // Salvar nova nota
            Nota nova_nota;
            nova_nota.id = gerar_id("notas.txt");
            nova_nota.atividade_id = atividade_id;
            nova_nota.aluno_matricula = matricula_aluno;
            nova_nota.nota = nota;
            
            FILE *file_nota = fopen("notas.txt", "a");
            if (file_nota != NULL) {
                fprintf(file_nota, "%d|%d|%d|%.1f\n", nova_nota.id, nova_nota.atividade_id, nova_nota.aluno_matricula, nova_nota.nota);
                fclose(file_nota);
                printf("Nota lancada com sucesso!\n");
            } else {
                printf("Erro ao salvar nota!\n");
            }
        }
    } while (1);
    
    pausar();
}

void consultar_notas() {
    FILE *file = fopen("notas.txt", "r");
    if (file == NULL) {
        printf("Nenhuma nota registrada!\n");
        pausar();
        return;
    }
    
    printf("=== NOTAS REGISTRADAS ===\n");
    char linha[200];
    int encontrou = 0;
    
    while (fgets(linha, sizeof(linha), file)) {
        int id, atividade_id, aluno_mat;
        float nota;
        sscanf(linha, "%d|%d|%d|%f", &id, &atividade_id, &aluno_mat, &nota);
        
        int mostrar = 0;
        
        if (admin_logado) {
            mostrar = 1;
        } else if (professor_logado) {
            // Verificar se e atividade do professor
            FILE *file_ativ = fopen("atividades.txt", "r");
            if (file_ativ != NULL) {
                char linha_ativ[400];
                while (fgets(linha_ativ, sizeof(linha_ativ), file_ativ)) {
                    int id_ativ, prof_mat;
                    sscanf(linha_ativ, "%d|%*[^|]|%*[^|]|%*d|%d", &id_ativ, &prof_mat);
                    if (id_ativ == atividade_id && prof_mat == matricula_atual) {
                        mostrar = 1;
                        break;
                    }
                }
                fclose(file_ativ);
            }
        } else if (aluno_logado && aluno_mat == matricula_atual) {
            mostrar = 1;
        }
        
        if (mostrar) {
            // Obter nome da atividade
            char nome_atividade[100] = "Atividade nao encontrada";
            FILE *file_ativ = fopen("atividades.txt", "r");
            if (file_ativ != NULL) {
                char linha_ativ[400];
                while (fgets(linha_ativ, sizeof(linha_ativ), file_ativ)) {
                    int id_ativ;
                    char nome[100];
                    sscanf(linha_ativ, "%d|%*[^|]|%[^|]", &id_ativ, nome);
                    if (id_ativ == atividade_id) {
                        strcpy(nome_atividade, nome);
                        break;
                    }
                }
                fclose(file_ativ);
            }
            
            // Obter nome do aluno
            char nome_aluno[100] = "Aluno nao encontrado";
            FILE *file_aluno = fopen("alunos.txt", "r");
            if (file_aluno != NULL) {
                char linha_aluno[300];
                while (fgets(linha_aluno, sizeof(linha_aluno), file_aluno)) {
                    int mat;
                    char nome[100];
                    sscanf(linha_aluno, "%d|%[^|]", &mat, nome);
                    if (mat == aluno_mat) {
                        strcpy(nome_aluno, nome);
                        break;
                    }
                }
                fclose(file_aluno);
            }
            
            // Obter nome do professor da atividade
            char nome_professor[100] = "Professor nao encontrado";
            FILE *file_ativ_prof = fopen("atividades.txt", "r");
            if (file_ativ_prof != NULL) {
                char linha_ativ_prof[400];
                while (fgets(linha_ativ_prof, sizeof(linha_ativ_prof), file_ativ_prof)) {
                    int id_ativ_prof, prof_mat;
                    sscanf(linha_ativ_prof, "%d|%*[^|]|%*[^|]|%*d|%d", &id_ativ_prof, &prof_mat);
                    if (id_ativ_prof == atividade_id) {
                        obter_nome_professor(prof_mat, nome_professor);
                        break;
                    }
                }
                fclose(file_ativ_prof);
            }
            
            printf("ID: %d | Atividade: %s | Aluno: %s | Professor: %s | Nota: %.1f\n", id, nome_atividade, nome_aluno, nome_professor, nota);
            encontrou = 1;
        }
    }
    
    fclose(file);
    
    if (!encontrou) {
        printf("Nenhuma nota encontrada!\n");
    }
    pausar();
}

void minhas_notas() {
    if (!aluno_logado) {
        printf("Acesso negado!\n");
        pausar();
        return;
    }
    
    FILE *file = fopen("notas.txt", "r");
    if (file == NULL) {
        printf("Nenhuma nota registrada!\n");
        pausar();
        return;
    }
    
    printf("=== MINHAS NOTAS ===\n");
    char linha[200];
    int encontrou = 0;
    float soma_notas = 0.0;
    int total_notas = 0;
    
    while (fgets(linha, sizeof(linha), file)) {
        int id, atividade_id, aluno_mat;
        float nota;
        sscanf(linha, "%d|%d|%d|%f", &id, &atividade_id, &aluno_mat, &nota);
        
        if (aluno_mat == matricula_atual) {
            // Obter dados da atividade
            char nome_atividade[100] = "Atividade nao encontrada";
            char disciplina[50] = "";
            int prof_mat = 0;
            FILE *file_ativ = fopen("atividades.txt", "r");
            if (file_ativ != NULL) {
                char linha_ativ[400];
                while (fgets(linha_ativ, sizeof(linha_ativ), file_ativ)) {
                    int id_ativ, prof;
                    char nome[100], disc[50];
                    sscanf(linha_ativ, "%d|%*[^|]|%[^|]|%*d|%d|%[^|]", &id_ativ, nome, &prof, disc);
                    if (id_ativ == atividade_id) {
                        strcpy(nome_atividade, nome);
                        strcpy(disciplina, disc);
                        prof_mat = prof;
                        break;
                    }
                }
                fclose(file_ativ);
            }
            
            // Obter nome do professor
            char nome_professor[100];
            obter_nome_professor(prof_mat, nome_professor);
            
            printf("%s (%s) - Professor: %s - Nota: %.1f\n", nome_atividade, disciplina, nome_professor, nota);
            soma_notas += nota;
            total_notas++;
            encontrou = 1;
        }
    }
    
    fclose(file);
    
    if (!encontrou) {
        printf("Voce ainda nao possui notas!\n");
    } else {
        // Calcular médias por matéria
        printf("\n--- MEDIAS POR MATERIA ---\n");
        char materias[8][20] = {"Matematica", "Ciencias", "Geografia", "Historia", "Filosofia", "Sociologia", "Portugues", "Ingles"};
        
        for (int i = 0; i < 8; i++) {
            float soma_materia = 0.0;
            int total_materia = 0;
            
            // Reabrir arquivo para calcular por matéria
            FILE *file_materia = fopen("notas.txt", "r");
            if (file_materia != NULL) {
                char linha_mat[200];
                while (fgets(linha_mat, sizeof(linha_mat), file_materia)) {
                    int id_mat, atividade_id_mat, aluno_mat_mat;
                    float nota_mat;
                    sscanf(linha_mat, "%d|%d|%d|%f", &id_mat, &atividade_id_mat, &aluno_mat_mat, &nota_mat);
                    
                    if (aluno_mat_mat == matricula_atual) {
                        // Obter disciplina da atividade
                        FILE *file_ativ_mat = fopen("atividades.txt", "r");
                        if (file_ativ_mat != NULL) {
                            char linha_ativ_mat[400];
                            while (fgets(linha_ativ_mat, sizeof(linha_ativ_mat), file_ativ_mat)) {
                                int id_ativ_mat;
                                char disc_mat[50];
                                sscanf(linha_ativ_mat, "%d|%*[^|]|%*[^|]|%*d|%*d|%[^|]", &id_ativ_mat, disc_mat);
                                if (id_ativ_mat == atividade_id_mat && strcmp(disc_mat, materias[i]) == 0) {
                                    soma_materia += nota_mat;
                                    total_materia++;
                                    break;
                                }
                            }
                            fclose(file_ativ_mat);
                        }
                    }
                }
                fclose(file_materia);
            }
            
            if (total_materia > 0) {
                printf("%s | Total de atividades: %d | Media Final: %.1f\n", materias[i], total_materia, soma_materia / total_materia);
            }
        }
        
        printf("\n--- RESUMO GERAL ---\n");
        printf("Total de notas: %d\n", total_notas);
        printf("Media geral: %.1f\n", soma_notas / total_notas);
    }
    pausar();
}

void excluir_nota() {
    if (!admin_logado && !professor_logado) {
        printf("Acesso negado!\n");
        pausar();
        return;
    }
    
    if (!verificar_arquivo_existe("notas.txt", "Nenhuma nota registrada para excluir!")) return;
    
    consultar_notas();
    
    int id;
    printf("\nID da nota para excluir (0 para cancelar): ");
    scanf("%d", &id);
    limpar_buffer();
    
    if (id == 0) {
        printf("Operacao cancelada.\n");
        pausar();
        return;
    }
    
    if (id < 1) {
        printf("Erro: ID deve ser um numero positivo!\n");
        pausar();
        return;
    }
    
    FILE *file = fopen("notas.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    
    if (file == NULL || temp == NULL) {
        printf("Erro ao acessar arquivo!\n");
        pausar();
        return;
    }
    
    char linha[200];
    int encontrou = 0;
    
    while (fgets(linha, sizeof(linha), file)) {
        int id_arquivo, atividade_id;
        sscanf(linha, "%d|%d", &id_arquivo, &atividade_id);
        
        if (id_arquivo != id) {
            fprintf(temp, "%s", linha);
        } else if (professor_logado) {
            // Verificar se é atividade do professor
            FILE *file_ativ = fopen("atividades.txt", "r");
            int pode_excluir = 0;
            if (file_ativ != NULL) {
                char linha_ativ[400];
                while (fgets(linha_ativ, sizeof(linha_ativ), file_ativ)) {
                    int id_ativ, prof_mat;
                    sscanf(linha_ativ, "%d|%*[^|]|%*[^|]|%*d|%d", &id_ativ, &prof_mat);
                    if (id_ativ == atividade_id && prof_mat == matricula_atual) {
                        pode_excluir = 1;
                        break;
                    }
                }
                fclose(file_ativ);
            }
            
            if (pode_excluir) {
                encontrou = 1;
            } else {
                fprintf(temp, "%s", linha);
                printf("Erro: Voce so pode excluir notas de suas atividades!\n");
            }
        } else {
            encontrou = 1;
        }
    }
    
    fclose(file);
    fclose(temp);
    
    remove("notas.txt");
    rename("temp.txt", "notas.txt");
    
    if (encontrou) {
        printf("Nota excluida com sucesso!\n");
    } else {
        printf("Erro: Nota com ID %d nao encontrada ou voce nao tem permissao!\n", id);
    }
    pausar();
}

void registrar_presenca() {
    if (!professor_logado) {
        printf("Acesso negado!\n");
        pausar();
        return;
    }
    
    int turma_id = selecionar_turma();
    if (turma_id <= 0) return;
    
    listar_alunos_turma(turma_id);
    
    int aula_id = selecionar_aula_professor(turma_id);
    if (aula_id <= 0) return;
    
    int matricula_aluno;
    printf("\nMatricula do aluno (0 para cancelar): ");
    scanf("%d", &matricula_aluno);
    limpar_buffer();
    
    if (matricula_aluno == 0) {
        printf("Operacao cancelada.\n");
        pausar();
        return;
    }
    
    char presenca;
    do {
        printf("Selecione a presenca do aluno (\"P\" para presente, \"F\" para falta): ");
        scanf(" %c", &presenca);
        limpar_buffer();
        presenca = toupper(presenca);
        
        if (presenca != 'P' && presenca != 'F') {
            printf("Erro: Digite P para Presente ou F para Falta!\n");
        }
    } while (presenca != 'P' && presenca != 'F');
    
    // Verificar se já existe presença registrada
    FILE *file_check = fopen("presencas.txt", "r");
    int ja_tem_presenca = 0;
    if (file_check != NULL) {
        char linha_check[100];
        while (fgets(linha_check, sizeof(linha_check), file_check)) {
            int aula, aluno;
            sscanf(linha_check, "%*d|%d|%d", &aula, &aluno);
            if (aula == aula_id && aluno == matricula_aluno) {
                ja_tem_presenca = 1;
                break;
            }
        }
        fclose(file_check);
    }
    
    if (ja_tem_presenca) {
        // Atualizar presença existente
        FILE *file_presencas = fopen("presencas.txt", "r");
        FILE *temp_presencas = fopen("temp_presencas.txt", "w");
        
        if (file_presencas != NULL && temp_presencas != NULL) {
            char linha_pres[100];
            while (fgets(linha_pres, sizeof(linha_pres), file_presencas)) {
                int id_pres, aula, aluno;
                char pres_antiga;
                sscanf(linha_pres, "%d|%d|%d|%c", &id_pres, &aula, &aluno, &pres_antiga);
                
                if (aula == aula_id && aluno == matricula_aluno) {
                    fprintf(temp_presencas, "%d|%d|%d|%c\n", id_pres, aula, aluno, presenca);
                } else {
                    fprintf(temp_presencas, "%s", linha_pres);
                }
            }
            fclose(file_presencas);
            fclose(temp_presencas);
            remove("presencas.txt");
            rename("temp_presencas.txt", "presencas.txt");
            printf("Presenca atualizada com sucesso!\n");
        } else {
            printf("Erro ao atualizar presenca!\n");
        }
    } else {
        // Registrar nova presença
        Presenca nova_presenca;
        nova_presenca.id = gerar_id("presencas.txt");
        nova_presenca.aula_id = aula_id;
        nova_presenca.aluno_matricula = matricula_aluno;
        nova_presenca.presenca = presenca;
        
        FILE *file_presenca = fopen("presencas.txt", "a");
        if (file_presenca != NULL) {
            fprintf(file_presenca, "%d|%d|%d|%c\n", nova_presenca.id, nova_presenca.aula_id, nova_presenca.aluno_matricula, nova_presenca.presenca);
            fclose(file_presenca);
            printf("Presenca registrada com sucesso!\n");
        } else {
            printf("Erro ao salvar presenca!\n");
        }
    }
    pausar();
}

void consultar_presencas() {
    if (!professor_logado) {
        printf("Acesso negado!\n");
        pausar();
        return;
    }
    
    int turma_id = selecionar_turma();
    if (turma_id <= 0) return;
    
    int aula_id = selecionar_aula_professor(turma_id);
    if (aula_id <= 0) return;
    
    printf("\n=== PRESENCAS DA AULA ===\n");
    FILE *file_alunos = fopen("alunos.txt", "r");
    if (file_alunos != NULL) {
        char linha[300];
        while (fgets(linha, sizeof(linha), file_alunos)) {
            int mat, turma;
            char nome[100];
            sscanf(linha, "%d|%[^|]|%*[^|]|%*[^|]|%d", &mat, nome, &turma);
            
            if (turma == turma_id) {
                char presenca = '-';
                FILE *file_presencas = fopen("presencas.txt", "r");
                if (file_presencas != NULL) {
                    char linha_pres[100];
                    while (fgets(linha_pres, sizeof(linha_pres), file_presencas)) {
                        int aula, aluno;
                        char pres;
                        sscanf(linha_pres, "%*d|%d|%d|%c", &aula, &aluno, &pres);
                        if (aula == aula_id && aluno == mat) {
                            presenca = pres;
                            break;
                        }
                    }
                    fclose(file_presencas);
                }
                printf("Aluno: %s | %c\n", nome, presenca);
            }
        }
        fclose(file_alunos);
    }
    pausar();
}

void menu_diario_eletronico() {
    int opcao;
    
    do {
        system("cls");
        printf("=== DIARIO ELETRONICO ===\n");
        printf("1. Lancar Notas\n");
        printf("2. Consultar Notas\n");
        printf("3. Excluir Nota\n");
        printf("4. Presenca do Aluno\n");
        printf("5. Consultar Presencas\n");
        printf("0. Voltar\n");
        printf("Opcao: ");
        scanf("%d", &opcao);
        limpar_buffer();
        
        switch (opcao) {
            case 1: lancar_notas(); break;
            case 2: consultar_notas(); break;
            case 3: excluir_nota(); break;
            case 4: registrar_presenca(); break;
            case 5: consultar_presencas(); break;
            case 0: break;
            default: printf("Opcao invalida!\n"); pausar();
        }
    } while (opcao != 0);
}

void menu_admin() {
    int opcao;
    
    do {
        system("cls");
        printf("=== MENU ADMINISTRADOR ===\n");
        printf("1. Cadastrar Turma\n");
        printf("2. Listar Turmas\n");
        printf("3. Excluir Turma\n");
        printf("4. Cadastrar Aluno\n");
        printf("5. Listar Alunos\n");
        printf("6. Excluir Aluno\n");
        printf("7. Cadastrar Professor\n");
        printf("8. Listar Professores\n");
        printf("9. Excluir Professor\n");
        printf("10. Registrar Aula\n");
        printf("11. Listar Aulas\n");
        printf("12. Excluir Aula\n");
        printf("13. Lancar Atividade\n");
        printf("14. Consultar Atividades\n");
        printf("15. Excluir Atividade\n");
        printf("16. Lancar Notas\n");
        printf("17. Consultar Notas\n");
        printf("18. Excluir Nota\n");
        printf("0. Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);
        limpar_buffer();
        
        switch (opcao) {
            case 1: cadastrar_turma(); break;
            case 2: listar_turmas(); break;
            case 3: excluir_turma(); break;
            case 4: cadastrar_aluno(); break;
            case 5: listar_alunos(); break;
            case 6: excluir_aluno(); break;
            case 7: cadastrar_professor(); break;
            case 8: listar_professores(); break;
            case 9: excluir_professor(); break;
            case 10: registrar_aula(); break;
            case 11: listar_aulas(); break;
            case 12: excluir_aula(); break;
            case 13: lancar_atividade(); break;
            case 14: consultar_atividades(); break;
            case 15: excluir_atividade(); break;
            case 16: lancar_notas(); break;
            case 17: consultar_notas(); break;
            case 18: excluir_nota(); break;
            case 0: break;
            default: printf("Opcao invalida!\n"); pausar();
        }
    } while (opcao != 0);
}

void menu_aluno() {
    int opcao;
    
    do {
        system("cls");
        printf("=== MENU ALUNO ===\n");
        printf("1. Minha Turma\n");
        printf("2. Minhas Aulas\n");
        printf("3. Consultar Atividades\n");
        printf("4. Minhas Notas\n");
        printf("0. Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);
        limpar_buffer();
        
        switch (opcao) {
            case 1: minha_turma(); break;
            case 2: minhas_aulas(); break;
            case 3: consultar_atividades(); break;
            case 4: minhas_notas(); break;
            case 0: break;
            default: printf("Opcao invalida!\n"); pausar();
        }
    } while (opcao != 0);
}

void menu_professor() {
    int opcao;
    
    do {
        system("cls");
        printf("=== MENU PROFESSOR ===\n");
        printf("1. Registrar Aula\n");
        printf("2. Minhas Aulas\n");
        printf("3. Excluir Aula\n");
        printf("4. Lancar Atividade\n");
        printf("5. Consultar Atividades\n");
        printf("6. Excluir Atividade\n");
        printf("7. Diario Eletronico\n");
        printf("0. Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);
        limpar_buffer();
        
        switch (opcao) {
            case 1: registrar_aula(); break;
            case 2: minhas_aulas(); break;
            case 3: excluir_aula(); break;
            case 4: lancar_atividade(); break;
            case 5: consultar_atividades(); break;
            case 6: excluir_atividade(); break;
            case 7: menu_diario_eletronico(); break;
            case 0: break;
            default: printf("Opcao invalida!\n"); pausar();
        }
    } while (opcao != 0);
}

int main() {
    int opcao;
    
    do {
        system("cls");
        printf("=== SISTEMA ESCOLAR SIMPLIFICADO ===\n");
        printf("1. Fazer Login\n");
        printf("0. Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);
        limpar_buffer();
        
        switch (opcao) {
            case 1:
                if (fazer_login()) {
                    if (admin_logado) {
                        menu_admin();
                        admin_logado = 0;
                    } else if (aluno_logado) {
                        menu_aluno();
                        aluno_logado = 0;
                    } else if (professor_logado) {
                        menu_professor();
                        professor_logado = 0;
                    }
                    matricula_atual = 0;
                }
                break;
            case 0:
                printf("Saindo do sistema...\n");
                break;
            default:
                printf("Opcao invalida!\n");
                pausar();
        }
    } while (opcao != 0);
    
    return 0;
}
