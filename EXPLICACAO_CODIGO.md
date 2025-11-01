# Explicação Detalhada do Código - Sistema Escolar

## 📋 Visão Geral

O sistema está organizado em seções bem definidas, cada uma com responsabilidades específicas:

## 🏗️ 1. ESTRUTURA BÁSICA

### Bibliotecas e Definições
```c
#include <stdio.h>      // Entrada/saída (printf, scanf)
#include <stdlib.h>     // Funções gerais (malloc, atoi)
#include <string.h>     // Strings (strcpy, strcmp)
#include <locale.h>     // Acentuação portuguesa

#define MAX_ALUNOS_POR_TURMA 30  // Limite de alunos por turma
```

### Estruturas de Dados (typedef struct)
- **Turma**: Armazena dados da turma (nome, ano, série, turno, capacidade, ID)
- **Aluno**: Dados pessoais + matrícula + credenciais de login
- **Professor**: Similar ao aluno + matérias que leciona
- **Aula**: Relaciona professor, turma, data, horário e matéria
- **Atividade**: Trabalhos/provas com tipo, peso, observações
- **Grupo**: Para atividades em grupo (lista de alunos)
- **Nota**: Relaciona atividade, aluno/grupo e valor da nota

### Variáveis Globais
```c
// Contadores para IDs únicos
int proximo_id_turma = 1;
int proxima_matricula = 10000001;

// Controle de login
int admin_logado = 0;
int aluno_logado = 0;
int matricula_aluno_logado = 0;

// Credenciais do admin
char admin_cpf[] = "12345678909";
char admin_senha[] = "admin";
```

## 🔧 2. FUNÇÕES UTILITÁRIAS

### Gerenciamento de Arquivos
- **carregar_contadores()**: Lê IDs do arquivo para manter sequência
- **salvar_contadores()**: Salva IDs atualizados no arquivo
- **limpar_buffer()**: Remove caracteres extras do teclado

### Validações
- **validar_cpf()**: Verifica se tem 11 dígitos numéricos
- **validar_email()**: Checa presença de @ e ponto
- **validar_data()**: Formato DD/MM/AAAA com valores válidos

### Verificações de Existência
- **aluno_existe()**: Procura matrícula no arquivo alunos.txt
- **professor_existe()**: Procura matrícula no arquivo professores.txt
- **cpf_ja_existe()**: Evita CPFs duplicados no sistema

## 📊 3. SISTEMA DE ARQUIVOS

### Estrutura de Dados Persistente
```
output/
├── alunos.txt          # Dados dos alunos
├── professores.txt     # Dados dos professores  
├── turmas.txt          # Dados das turmas
├── aulas.txt           # Registro de aulas
├── atividades.txt      # Atividades lançadas
├── grupos.txt          # Grupos de atividades
├── notas.txt           # Notas dos alunos
├── aluno_turma.txt     # Relaciona aluno → turma
├── professor_turma.txt # Relaciona professor → turma
└── contadores.txt      # IDs sequenciais
```

### Formato dos Arquivos (separados por |)
```
alunos.txt: matricula|nome|email|senha|nascimento|genero|cpf
turmas.txt: id|nome|ano|serie|turno|capacidade
aulas.txt: id|professor_matricula|turma_id|data|horario|materia
```

## 🔐 4. SISTEMA DE LOGIN

### Tipos de Usuário
1. **Administrador**: Acesso total ao sistema
2. **Professor**: Pode gerenciar suas aulas e atividades
3. **Aluno**: Consulta suas informações e notas

### Processo de Autenticação
```c
int fazer_login() {
    // 1. Solicita CPF e senha
    // 2. Verifica se é admin (credenciais fixas)
    // 3. Procura em alunos.txt
    // 4. Procura em professores.txt
    // 5. Define variáveis globais de controle
}
```

## 📚 5. FUNCIONALIDADES PRINCIPAIS

### Gestão de Alunos
- **cadastrar_aluno()**: Coleta dados, valida e salva em arquivo
- **excluir_aluno()**: Remove por matrícula(s)
- **listar_alunos()**: Exibe todos (admin vê senhas)

### Gestão de Professores  
- **cadastrar_professor()**: Similar aos alunos + seleção de matérias
- **excluir_professor()**: Remove por matrícula(s)
- **listar_professores()**: Exibe todos com suas matérias

### Gestão de Turmas
- **criar_turma()**: Série (1-3) + Letra (A-Z) + Ano + Turno
- **editar_turmas()**: Altera dados, inclui/remove alunos e professores
- **excluir_turma()**: Remove turma por ID
- **listar_turmas()**: Exibe com estatísticas e detalhes

### Gestão de Aulas
- **registrar_aula()**: Professor + Turma + Data + Horário + Matéria
- **editar_aula()**: Permite alterar todos os campos
- **excluir_aula()**: Remove por ID(s)
- **listar_aulas()**: Filtra por professor/turma

## ⏰ 6. SISTEMA DE HORÁRIOS

### Turnos e Horários
```c
// Matutino: 07h00-11h40 (5 aulas de 50min)
// Vespertino: 13h00-17h40 (5 aulas de 50min)  
// Noturno: 18h00-22h40 (5 aulas de 50min)
```

### Validação de Conflitos
- **horario_ocupado()**: Verifica se turma já tem aula no horário
- **exibir_horarios_disponiveis()**: Mostra slots livres
- **selecionar_horario_disponivel()**: Interface para escolha

## 📝 7. SISTEMA DE ATIVIDADES E NOTAS

### Tipos de Atividade
- Atividades diversas (peso 7)
- Trabalhos (peso 9)  
- Avaliações (peso 10)

### Atividades em Grupo
- **criar_grupo()**: Forma grupos com alunos selecionados
- **renomear_grupos_atividade()**: Mantém numeração sequencial
- **excluir_grupos_atividade()**: Remove grupos ao excluir atividade

### Lançamento de Notas
- **lancar_notas()**: Individual ou por grupo
- **minhas_notas()**: Consulta do aluno
- **listar_notas()**: Relatório do professor

## 🎯 8. REGRAS DE NEGÓCIO

### Validações Importantes
- Turmas únicas (série + letra + ano + turno)
- Máximo 30 alunos por turma
- Professor só edita suas próprias aulas
- Ano da aula deve coincidir com ano da turma
- Aluno só pode estar em uma turma

### Controle de Fluxo
- "0 para cancelar" em todas as operações
- Confirmações para ações críticas
- Mensagens de erro específicas
- Validação de entrada em tempo real

## 🔄 9. MENUS HIERÁRQUICOS

### Estrutura de Navegação
```
Menu Principal
├── Login (se não logado)
├── Alunos (cadastrar, excluir, listar)
├── Professores (cadastrar, excluir, listar)
├── Turmas (criar, editar, excluir, listar)
├── Aulas (registrar, editar, excluir, listar)
├── Atividades (lançar, editar, excluir, listar, notas)
└── Consultar Horários
```

### Controle de Acesso
- **Admin**: Acesso total
- **Professor**: Limitado às suas turmas/aulas
- **Aluno**: Apenas consultas de suas informações

## 🛠️ 10. FUNÇÕES DE APOIO

### Processamento de Entrada
- **processar_matriculas_input()**: Converte string em array de números
- **validar_entrada_string()**: Verifica se campo não está vazio
- **obter_opcao_valida()**: Força entrada dentro de faixa válida

### Exibição de Dados
- **exibir_alunos_disponiveis()**: Lista alunos sem turma
- **exibir_turmas_professor()**: Mostra turmas do professor logado
- **formatar_cpf()**: Adiciona pontos e hífen ao CPF

### Utilitários Específicos
- **obter_nome_aluno()**: Busca nome por matrícula
- **confirmar_operacao()**: Pede confirmação S/N
- **pausar_sistema()**: Aguarda Enter para continuar

## 🎨 11. INTERFACE DO USUÁRIO

### Características
- Interface em português com acentuação
- Limpeza de tela entre operações
- Mensagens informativas e de erro
- Navegação intuitiva com números
- Feedback imediato das operações

### Tratamento de Erros
- Validação de entrada em tempo real
- Mensagens específicas para cada tipo de erro
- Opção de cancelar operações a qualquer momento
- Recuperação graceful de erros de arquivo

## 📈 12. ESCALABILIDADE E MANUTENÇÃO

### Pontos Fortes
- Código modular com funções específicas
- Separação clara de responsabilidades
- Validações centralizadas
- Estrutura de dados bem definida

### Possíveis Melhorias
- Implementar banco de dados real
- Adicionar criptografia para senhas
- Interface gráfica
- Relatórios mais avançados
- Sistema de backup automático

---

Este sistema demonstra conceitos fundamentais de programação em C, incluindo estruturas de dados, manipulação de arquivos, validação de entrada, e design de interface de usuário em modo texto.