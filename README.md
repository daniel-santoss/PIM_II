# Sistema Escolar em C

## Descrição
Aplicação console em C para gerenciamento de sistema escolar com as seguintes funcionalidades:

### Funcionalidades
- **Criação de turmas**: Nome, ano, série, turno, capacidade máxima
- **Cadastro de aluno**: Nome, email, senha, data de nascimento, gênero, CPF, número da matrícula
- **Cadastro de professor**: Nome, email, senha, data de nascimento, gênero, CPF, número da matrícula, especialidade
- **Registro de aulas**: Professor, turma, dia da aula, hora da aula, matéria
- **Sistema de login**: Utilizando CPF e senha
- **5 aulas por período**: Cada aula com duração de 50 minutos
- **3 períodos**: Matutino, Vespertino e Noturno
- **Intervalos**: 30 minutos entre a 3ª e 4ª aula

### Horários de Funcionamento

**Período Matutino (07h00-11h40)**
- 1ª Aula: 07h00-07h50
- 2ª Aula: 07h50-08h40
- 3ª Aula: 08h40-09h30
- Intervalo: 09h30-10h00 (30 minutos)
- 4ª Aula: 10h00-10h50
- 5ª Aula: 10h50-11h40

**Período Vespertino (13h00-17h40)**
- 1ª Aula: 13h00-13h50
- 2ª Aula: 13h50-14h40
- 3ª Aula: 14h40-15h30
- Intervalo: 15h30-16h00 (30 minutos)
- 4ª Aula: 16h00-16h50
- 5ª Aula: 16h50-17h40

**Período Noturno (18h00-22h40)**
- 1ª Aula: 18h00-18h50
- 2ª Aula: 18h50-19h40
- 3ª Aula: 19h40-20h30
- Intervalo: 20h30-21h00 (30 minutos)
- 4ª Aula: 21h00-21h50
- 5ª Aula: 21h50-22h40

## Menu de Opções
1. **Login** - Fazer login com CPF e senha
2. **Criar turma** - Cadastrar nova turma
3. **Cadastrar aluno** - Registrar novo aluno
4. **Cadastrar professor** - Registrar novo professor
5. **Registrar aula** - Agendar nova aula
6. **Listar turmas** - Visualizar todas as turmas
7. **Listar aulas** - Visualizar todas as aulas
0. **Sair** - Encerrar o programa

## Estrutura dos Dados

### Turma
- ID (gerado automaticamente)
- Nome
- Ano
- Série
- Turno
- Capacidade máxima

### Aluno/Professor
- Nome
- Email
- Senha
- Data de nascimento
- Gênero
- CPF
- Número da matrícula (chave primária)
- Especialidade (apenas professor)

### Aula
- ID (gerado automaticamente)
- Matrícula do professor
- ID da turma
- Dia da aula
- Hora da aula
- Matéria

## Limites do Sistema
- Máximo 30 alunos por turma
- Sem limite para número total de alunos e professores (limitado apenas pela memória disponível)

# Sistema de Gestão Escolar

Sistema completo de gerenciamento escolar desenvolvido em linguagem C para controle de alunos, professores, turmas e aulas.

## Sobre o Projeto

Este sistema foi desenvolvido como parte do **Projeto Integrado Multidisciplinar II (PIM II)** do curso de **Análise e Desenvolvimento de Sistemas**. O objetivo é criar uma solução robusta para gestão educacional com interface de linha de comando.

## Funcionalidades Principais

### Sistema de Autenticação
- **Administrador**: Login com CPF e Senha cadastrados
- **Alunos**: Login com CPF e senha cadastrados
- **Professores**: Login com CPF e senha cadastrados

### Gestão de Alunos
- Cadastro com validação completa (CPF, email, dados pessoais)
- Exclusão de registros
- Listagem detalhada
- Consulta de turma individual (para alunos)

### Gestão de Professores
- Cadastro com múltiplas matérias
- Exclusão de registros
- Listagem completa
- Controle de aulas próprias

### Gestão de Turmas
- Criação (1º, 2º, 3º ano + letras A-Z)
- Exclusão de turmas
- **Edição Avançada**:
  - Alterar dados (série, letra, ano letivo, turno)
  - Incluir alunos (máx. 30 por turma)
  - Remover alunos
- Listagem com detalhes e estatísticas

### Gestão de Aulas
- Registro com validação de horários
- Edição completa (turma, professor, matéria, data, horário)
- Exclusão de registros
- Listagem organizada

## Validações e Segurança

### Validação de Dados
- **CPF**: Formato com 11 dígitos numéricos
- **Email**: Validação de formato (@, .)
- **Data**: Formato DD/MM/AAAA com validação
- **Ano Letivo**: Consistência com ano da turma

### Regras de Negócio
- Turmas únicas (sem duplicatas)
- Limite de 30 alunos por turma
- Professores editam apenas suas aulas
- Alteração de turma bloqueada com aulas registradas
- Horários específicos por turno

### Controle de Fluxo
- **"0 para cancelar"** em todas as operações
- Mensagens de erro específicas
- Confirmações de ações críticas

## Horários de Funcionamento

| **Turno** | **Horário** | **Aulas** |
|-----------|-------------|----------|
| **Matutino** | 07h00 - 11h40 | 5 aulas de 50min |
| **Vespertino** | 13h00 - 17h40 | 5 aulas de 50min |
| **Noturno** | 18h00 - 22h40 | 5 aulas de 50min |

### Grade Detalhada

**Matutino**
- 1ª Aula: 07h00-07h50
- 2ª Aula: 07h50-08h40
- 3ª Aula: 08h40-09h30
- **Intervalo**: 09h30-10h00
- 4ª Aula: 10h00-10h50
- 5ª Aula: 10h50-11h40

**Vespertino**
- 1ª Aula: 13h00-13h50
- 2ª Aula: 13h50-14h40
- 3ª Aula: 14h40-15h30
- **Intervalo**: 15h30-16h00
- 4ª Aula: 16h00-16h50
- 5ª Aula: 16h50-17h40

**Noturno**
- 1ª Aula: 18h00-18h50
- 2ª Aula: 18h50-19h40
- 3ª Aula: 19h40-20h30
- **Intervalo**: 20h30-21h00
- 4ª Aula: 21h00-21h50
- 5ª Aula: 21h50-22h40

## Matérias Disponíveis

| **Exatas** | **Humanas** | **Linguagens** |
|------------|-------------|----------------|
| Matemática | Geografia | Português |
| Ciências | História | Inglês |
| | Filosofia | |
| | Sociologia | |

## Como Executar

### Pré-requisitos
- Compilador GCC
- Sistema operacional Windows/Linux/macOS

### Compilação
```bash
gcc sistema_escolar.c -o sistema_escolar
```

### Execução
```bash
./sistema_escolar
```

## Estrutura do Projeto

```
PIM_II/
├── sistema_escolar.c     # Código fonte principal
├── README.md             # Documentação
└── output/               # Arquivos de dados
    ├── alunos.txt
    ├── professores.txt
    ├── turmas.txt
    ├── aulas.txt
    ├── aluno_turma.txt
    └── contadores.txt
```

## Tecnologias Utilizadas

- **Linguagem**: C (Padrão ANSI)
- **Armazenamento**: Arquivos de texto (.txt)
- **Interface**: Terminal/Console
- **Compilador**: GCC

## Desenvolvedor

**Projeto desenvolvido para o PIM II - Análise e Desenvolvimento de Sistemas**
>>>>>>> testes
