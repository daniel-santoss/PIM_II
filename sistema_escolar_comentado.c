/*
 * SISTEMA DE GESTÃO ESCOLAR - PIM II
 * Análise e Desenvolvimento de Sistemas
 * 
 * Este sistema gerencia alunos, professores, turmas, aulas, atividades e notas
 * de uma instituição de ensino através de interface de linha de comando.
 */

// ========== BIBLIOTECAS E DEFINIÇÕES ==========
#include <stdio.h>      // Entrada/saída padrão (printf, scanf, etc.)
#include <stdlib.h>     // Funções gerais (malloc, free, atoi, etc.)
#include <string.h>     // Manipulação de strings (strcpy, strcmp, etc.)
#include <locale.h>     // Configuração de localização (acentos)

#define MAX_ALUNOS_POR_TURMA 30  // Limite máximo de alunos por turma

// ========== ESTRUTURAS DE DADOS ==========

// Estrutura para representar uma turma
typedef struct {
    char nome[100];           // Nome da turma (ex: "1 Ano A")
    int ano;                  // Ano letivo (2025, 2026)
    int serie;                // Série (1, 2, 3)
    char turno[20];           // Turno (Matutino, Vespertino, Noturno)
    int capacidade_maxima;    // Capacidade máxima de alunos
    int id;                   // Identificador único da turma
} Turma;

// Estrutura para representar um aluno
typedef struct {
    char nome[100];           // Nome completo do aluno
    char email[100];          // Email do aluno
    char senha[50];           // Senha para login
    char data_nascimento[15]; // Data de nascimento (DD/MM/AAAA)
    char genero;              // Gênero (M/F)
    char cpf[15];             // CPF (apenas números)
    int matricula;            // Matrícula única do aluno
} Aluno;

// Estrutura para representar um professor
typedef struct {
    char nome[100];           // Nome completo do professor
    char email[100];          // Email do professor
    char senha[50];           // Senha para login
    char data_nascimento[15]; // Data de nascimento (DD/MM/AAAA)
    char genero;              // Gênero (M/F)
    char cpf[15];             // CPF (apenas números)
    int matricula;            // Matrícula única do professor
    char materia[50];         // Matérias que leciona (separadas por vírgula)
} Professor;

// Estrutura para representar uma aula
typedef struct {
    int professor_matricula;  // Matrícula do professor responsável
    int turma_id;            // ID da turma onde a aula acontece
    char dia[15];            // Data da aula (DD/MM/AAAA)
    char hora[20];           // Horário da aula (HH:MM-HH:MM)
    char materia[50];        // Matéria da aula
    int id;                  // Identificador único da aula
} Aula;

// Estrutura para representar uma atividade
typedef struct {
    int id;                   // Identificador único da atividade
    int professor_matricula;  // Matrícula do professor responsável
    int turma_id;            // ID da turma para qual a atividade é aplicada
    char nome[100];          // Nome da atividade
    char tipo[20];           // Tipo (Atividades diversas, Trabalhos, Avaliações)
    char disciplina[50];     // Disciplina da atividade
    int peso;                // Peso da atividade (7, 9 ou 10)
    char em_grupo;           // Se é em grupo (S/N)
    char observacao[500];    // Observações adicionais
} Atividade;

// Estrutura para representar um grupo de alunos
typedef struct {
    int id;                  // Identificador único do grupo
    int atividade_id;        // ID da atividade relacionada
    char nome[50];           // Nome do grupo (ex: "Grupo 1")
    int alunos[30];          // Array com matrículas dos alunos
    int num_alunos;          // Número de alunos no grupo
} Grupo;

// Estrutura para representar uma nota
typedef struct {
    int id;                  // Identificador único da nota
    int atividade_id;        // ID da atividade avaliada
    int aluno_matricula;     // Matrícula do aluno
    int grupo_id;            // ID do grupo (0 se individual)
    float nota;              // Valor da nota
} Nota;

// ========== VARIÁVEIS GLOBAIS ==========

// Contadores para gerar IDs únicos
int proximo_id_turma = 1;        // Próximo ID disponível para turma
int proximo_id_aula = 1;         // Próximo ID disponível para aula
int proxima_matricula = 10000001; // Próxima matrícula disponível
int proximo_id_atividade = 1;    // Próximo ID disponível para atividade
int proximo_id_grupo = 1;        // Próximo ID disponível para grupo
int proximo_id_nota = 1;         // Próximo ID disponível para nota

// Variáveis de controle de login
int admin_logado = 0;            // 1 se administrador está logado
int usuario_logado = 0;          // 1 se algum usuário está logado
int aluno_logado = 0;            // 1 se aluno está logado
int matricula_aluno_logado = 0;  // Matrícula do aluno logado
int matricula_professor_logado = 0; // Matrícula do professor logado

// Credenciais do administrador
char admin_cpf[] = "12345678909";
char admin_senha[] = "admin";

// ========== DECLARAÇÕES DE FUNÇÕES ==========

// Funções utilitárias básicas
void limpar_buffer();                    // Limpa buffer do teclado
void carregar_contadores();              // Carrega contadores dos arquivos
void salvar_contadores();                // Salva contadores nos arquivos
void pausar_sistema();                   // Pausa e aguarda Enter

// Funções de validação
int validar_cpf(char *cpf);             // Valida formato do CPF
int validar_email(char *email);         // Valida formato do email
int validar_data(char *data);           // Valida formato da data
int validar_matricula_numerica(char *token); // Valida se string é número válido

// Funções de verificação de existência
int aluno_existe(int matricula);        // Verifica se aluno existe
int professor_existe(int matricula);    // Verifica se professor existe
int turma_existe(int turma_id);         // Verifica se turma existe
int cpf_ja_existe(char *cpf);          // Verifica se CPF já está cadastrado
int turma_ja_existe(int serie, char letra, int ano, char *turno); // Verifica duplicata de turma

// Funções de relacionamento
int aluno_esta_na_turma(int matricula, int turma_id);     // Verifica se aluno está na turma
int aluno_ja_tem_turma(int matricula);                    // Verifica se aluno já tem turma
int professor_ja_tem_turma(int matricula, int turma_id);  // Verifica se professor está na turma
int aluno_ja_em_grupo(int matricula, int atividade_id);   // Verifica se aluno está em grupo
int horario_ocupado(int turma_id, char *data, char *horario); // Verifica conflito de horário

// Funções de obtenção de dados
char* obter_nome_aluno(int matricula);     // Retorna nome do aluno
char* obter_nome_professor(int matricula); // Retorna nome do professor
void formatar_cpf(char *cpf, char *cpf_formatado); // Formata CPF com pontos e hífen

// Funções de exibição
void exibir_alunos_disponiveis(int turma_id);    // Lista alunos sem turma
void exibir_alunos_na_turma(int turma_id);       // Lista alunos de uma turma
void exibir_professores_disponiveis();           // Lista todos os professores
void exibir_turmas_disponiveis();                // Lista todas as turmas
void exibir_turmas_professor(int professor_matricula); // Lista turmas do professor
void exibir_horarios_disponiveis(char *turno, int turma_id, char *data); // Mostra horários livres

// Funções de processamento de entrada
int processar_matriculas_input(char *input, int *matriculas, int *count, int max_count);
int processar_ids_input(char *input, int *ids, int *count, int max_count);
int validar_matriculas_grupo(char *input, int turma_id, int atividade_id, int *matriculas, int *count);
void obter_input_cancelavel(char *buffer, int tamanho, char *prompt);
int confirmar_operacao(char *mensagem);

// Funções de gerenciamento de grupos
int obter_proximo_numero_grupo(int atividade_id);
void criar_grupo(int atividade_id, int *matriculas, int num_alunos);
void renomear_grupos_atividade(int atividade_id);
void excluir_grupo(int grupo_id, int atividade_id);
void excluir_grupos_atividade(int atividade_id);

// Funções de relacionamento turma-usuário
void incluir_aluno_turma(int matricula, int turma_id);
void incluir_professor_turma(int matricula, int turma_id);
void remover_aluno_turma(int matricula);

// Funções de seleção e validação
int selecionar_horario_disponivel(char *turno, int turma_id, char *data, char *horario_selecionado);
int obter_opcao_valida(int min, int max, char *prompt);
int validar_e_obter_id(char *prompt, int (*funcao_existe)(int));

// ========== FUNÇÕES PRINCIPAIS DO SISTEMA ==========

// Função de login
int fazer_login();

// Funções de cadastro
void cadastrar_aluno();
void cadastrar_professor();
void criar_turma();

// Funções de listagem
void listar_alunos();
void listar_professores();
void listar_turmas();
void listar_aulas();
void listar_atividades();
void listar_atividades_aluno();
void listar_notas();

// Funções de exclusão
void excluir_aluno();
void excluir_professor();
void excluir_turma();
void excluir_aula();
void excluir_atividade();

// Funções de edição
void editar_turmas();
void editar_aula();
void editar_atividade();

// Funções de aulas
void registrar_aula();

// Funções de atividades e notas
void lancar_atividade();
void lancar_notas();
void minhas_notas();

// Funções específicas para alunos
void minha_turma();

// Funções de consulta
void consultar_horarios();

// Funções de menu
void menu_principal();
void menu_alunos();
void menu_professores();
void menu_turmas();
void menu_aulas();
void menu_atividades();
void menu_atividades_aluno();

// ========== IMPLEMENTAÇÃO DAS FUNÇÕES UTILITÁRIAS ==========

/*
 * Limpa o buffer do teclado para evitar problemas com entrada de dados
 * Usado após scanf para remover caracteres extras
 */
void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/*
 * Carrega os contadores de IDs do arquivo contadores.txt
 * Permite manter sequência única mesmo após reiniciar o programa
 */
void carregar_contadores() {
    FILE *file = fopen("contadores.txt", "r");
    if (file != NULL) {
        fscanf(file, "%d %d %d %d %d %d", 
               &proximo_id_turma, &proximo_id_aula, &proxima_matricula, 
               &proximo_id_atividade, &proximo_id_grupo, &proximo_id_nota);
        fclose(file);
    }
}

/*
 * Salva os contadores atuais no arquivo contadores.txt
 * Garante persistência dos dados entre execuções
 */
void salvar_contadores() {
    FILE *file = fopen("contadores.txt", "w");
    if (file != NULL) {
        fprintf(file, "%d %d %d %d %d %d", 
                proximo_id_turma, proximo_id_aula, proxima_matricula, 
                proximo_id_atividade, proximo_id_grupo, proximo_id_nota);
        fclose(file);
    }
}

/*
 * Valida se o CPF tem exatamente 11 dígitos numéricos
 * Retorna 1 se válido, 0 se inválido
 */
int validar_cpf(char *cpf) {
    if (strlen(cpf) != 11) return 0;  // Deve ter exatamente 11 caracteres
    
    // Verifica se todos os caracteres são dígitos
    for (int i = 0; i < 11; i++) {
        if (cpf[i] < '0' || cpf[i] > '9') return 0;
    }
    return 1;
}

/*
 * Valida formato básico de email (deve conter @ e . após o @)
 * Retorna 1 se válido, 0 se inválido
 */
int validar_email(char *email) {
    if (strlen(email) < 5) return 0;  // Email muito curto
    
    int tem_arroba = 0, tem_ponto = 0;
    for (int i = 0; i < strlen(email); i++) {
        if (email[i] == '@') tem_arroba = 1;
        if (tem_arroba && email[i] == '.') tem_ponto = 1;
    }
    return tem_arroba && tem_ponto;
}

/*
 * Valida formato de data DD/MM/AAAA
 * Verifica estrutura básica e valores de dia/mês
 */
int validar_data(char *data) {
    if (strlen(data) != 10) return 0;  // Deve ter exatamente 10 caracteres
    if (data[2] != '/' || data[5] != '/') return 0;  // Deve ter barras nas posições corretas
    
    // Verifica se posições de números contêm apenas dígitos
    for (int i = 0; i < 10; i++) {
        if (i != 2 && i != 5 && (data[i] < '0' || data[i] > '9')) return 0;
    }
    
    // Extrai e valida dia e mês
    int dia = (data[0] - '0') * 10 + (data[1] - '0');
    int mes = (data[3] - '0') * 10 + (data[4] - '0');
    
    if (dia < 1 || dia > 31 || mes < 1 || mes > 12) return 0;
    return 1;
}

/*
 * Pausa o sistema e aguarda o usuário pressionar Enter
 * Usado para dar tempo ao usuário ler mensagens
 */
void pausar_sistema() {
    printf("\nPressione Enter para voltar ao menu...");
    getchar();
}

/*
 * Formata CPF adicionando pontos e hífen (XXX.XXX.XXX-XX)
 */
void formatar_cpf(char *cpf, char *cpf_formatado) {
    sprintf(cpf_formatado, "%.3s.%.3s.%.3s-%.2s", cpf, cpf+3, cpf+6, cpf+9);
}

// ========== FUNÇÕES DE VERIFICAÇÃO ==========

/*
 * Verifica se um aluno existe no sistema através da matrícula
 * Retorna 1 se existe, 0 se não existe
 */
int aluno_existe(int matricula) {
    FILE *file = fopen("alunos.txt", "r");
    if (file == NULL) return 0;
    
    char linha[500];
    while (fgets(linha, sizeof(linha), file)) {
        int mat;
        sscanf(linha, "%d|", &mat);  // Lê apenas a matrícula da linha
        if (mat == matricula) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

/*
 * Verifica se um professor existe no sistema através da matrícula
 */
int professor_existe(int matricula) {
    FILE *file = fopen("professores.txt", "r");
    if (file == NULL) return 0;
    
    char linha[500];
    while (fgets(linha, sizeof(linha), file)) {
        int mat;
        sscanf(linha, "%d|", &mat);
        if (mat == matricula) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

/*
 * Verifica se uma turma existe através do ID
 */
int turma_existe(int turma_id) {
    FILE *file = fopen("turmas.txt", "r");
    if (file == NULL) return 0;
    
    char linha[500];
    while (fgets(linha, sizeof(linha), file)) {
        int id;
        sscanf(linha, "%d|", &id);
        if (id == turma_id) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

/*
 * Verifica se um CPF já está cadastrado no sistema
 * Procura tanto em alunos quanto em professores
 */
int cpf_ja_existe(char *cpf) {
    FILE *file;
    char linha[500];
    char cpf_arquivo[15];
    
    // Verifica no arquivo de alunos
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
    
    // Verifica no arquivo de professores
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

// ========== FUNÇÃO PRINCIPAL ==========

/*
 * Função principal do programa
 * Inicializa o sistema e chama o menu principal
 */
int main() {
    setlocale(LC_ALL, "Portuguese");  // Configura acentuação portuguesa
    carregar_contadores();            // Carrega contadores dos arquivos
    menu_principal();                 // Inicia o menu principal
    return 0;
}

/*
 * Menu principal do sistema
 * Controla o fluxo principal baseado no tipo de usuário logado
 */
void menu_principal() {
    int opcao;
    
    while (1) {
        // Limpa a tela (comando específico do Windows)
        system("cls");
        
        printf("=== SISTEMA DE GESTAO ESCOLAR ===\n");
        
        // Exibe opções diferentes baseado no status de login
        if (!usuario_logado) {
            printf("1. Fazer login\n");
            printf("2. Consultar horarios\n");
            printf("0. Sair\n");
        } else if (admin_logado) {
            // Menu do administrador - acesso completo
            printf("Logado como: ADMINISTRADOR\n\n");
            printf("1. Alunos\n");
            printf("2. Professores\n");
            printf("3. Turmas\n");
            printf("4. Aulas\n");
            printf("5. Atividades e controle de nota\n");
            printf("6. Consultar horarios\n");
            printf("0. Sair\n");
        } else if (aluno_logado) {
            // Menu do aluno - acesso limitado
            printf("Logado como: ALUNO (Matricula: %d)\n\n", matricula_aluno_logado);
            printf("1. Minha turma\n");
            printf("2. Minhas aulas\n");
            printf("3. Atividades e notas\n");
            printf("4. Consultar horarios\n");
            printf("0. Sair\n");
        } else if (matricula_professor_logado) {
            // Menu do professor - acesso intermediário
            printf("Logado como: PROFESSOR (Matricula: %d)\n\n", matricula_professor_logado);
            printf("1. Alunos\n");
            printf("2. Professores\n");
            printf("3. Turmas\n");
            printf("4. Aulas\n");
            printf("5. Atividades e controle de nota\n");
            printf("6. Consultar horarios\n");
            printf("0. Sair\n");
        }
        
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limpar_buffer();
        
        // Processa a opção escolhida baseado no tipo de usuário
        if (!usuario_logado) {
            switch (opcao) {
                case 1:
                    fazer_login();
                    break;
                case 2:
                    consultar_horarios();
                    break;
                case 0:
                    printf("Saindo do sistema...\n");
                    return;
                default:
                    printf("Opcao invalida!\n");
                    pausar_sistema();
            }
        } else {
            // Lógica para usuários logados (admin, professor, aluno)
            switch (opcao) {
                case 1:
                    if (aluno_logado) {
                        minha_turma();
                    } else {
                        menu_alunos();
                    }
                    break;
                case 2:
                    if (aluno_logado) {
                        listar_aulas();
                    } else {
                        menu_professores();
                    }
                    break;
                case 3:
                    if (aluno_logado) {
                        menu_atividades_aluno();
                    } else {
                        menu_turmas();
                    }
                    break;
                case 4:
                    if (aluno_logado) {
                        consultar_horarios();
                    } else {
                        menu_aulas();
                    }
                    break;
                case 5:
                    if (!aluno_logado) {
                        menu_atividades();
                    } else {
                        printf("Opcao invalida!\n");
                        pausar_sistema();
                    }
                    break;
                case 6:
                    if (!aluno_logado) {
                        consultar_horarios();
                    } else {
                        printf("Opcao invalida!\n");
                        pausar_sistema();
                    }
                    break;
                case 0:
                    printf("Saindo do sistema...\n");
                    return;
                default:
                    printf("Opcao invalida!\n");
                    pausar_sistema();
            }
        }
    }
}

/*
 * NOTA: Este é apenas o início do código comentado.
 * O arquivo original tem mais de 3000 linhas com todas as funcionalidades.
 * 
 * PRINCIPAIS FUNCIONALIDADES IMPLEMENTADAS:
 * 
 * 1. SISTEMA DE LOGIN:
 *    - Administrador (CPF: 12345678909, Senha: admin)
 *    - Alunos e professores (login com CPF e senha)
 * 
 * 2. GESTÃO DE ALUNOS:
 *    - Cadastro com validação completa
 *    - Exclusão e listagem
 *    - Controle de turmas
 * 
 * 3. GESTÃO DE PROFESSORES:
 *    - Cadastro com múltiplas matérias
 *    - Exclusão e listagem
 *    - Controle de turmas
 * 
 * 4. GESTÃO DE TURMAS:
 *    - Criação (1º, 2º, 3º ano + letras A-Z)
 *    - Edição avançada (dados, alunos, professores)
 *    - Exclusão e listagem detalhada
 * 
 * 5. GESTÃO DE AULAS:
 *    - Registro com validação de horários
 *    - Edição completa
 *    - Exclusão e listagem
 * 
 * 6. GESTÃO DE ATIVIDADES:
 *    - Lançamento (individual ou em grupo)
 *    - Edição e exclusão
 *    - Controle de grupos
 * 
 * 7. GESTÃO DE NOTAS:
 *    - Lançamento para atividades
 *    - Consulta por aluno
 *    - Relatórios por atividade
 * 
 * 8. VALIDAÇÕES E SEGURANÇA:
 *    - CPF (11 dígitos)
 *    - Email (formato básico)
 *    - Data (DD/MM/AAAA)
 *    - Horários por turno
 *    - Capacidade de turmas
 * 
 * 9. PERSISTÊNCIA DE DADOS:
 *    - Arquivos .txt para cada entidade
 *    - Relacionamentos entre tabelas
 *    - Contadores para IDs únicos
 * 
 * 10. INTERFACE DE USUÁRIO:
 *     - Menus hierárquicos
 *     - Mensagens de erro específicas
 *     - Opção "0 para cancelar" em todas as operações
 */