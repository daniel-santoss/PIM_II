# Sistema Escolar em C

## Descrição
Aplicação console em C para gerenciamento de sistema escolar com as seguintes funcionalidades:

### Funcionalidades
- **Criação de turmas**: Nome, ano, série, turno, capacidade máxima
- **Cadastro de aluno**: Nome, email, senha, data de nascimento, gênero, CPF, número da matrícula
- **Cadastro de professor**: Nome, email, senha, data de nascimento, gênero, CPF, número da matrícula, especialidade
- **Registro de aulas**: Professor, turma, dia da aula, hora da aula, matéria
- **Sistema de login**: Utilizando CPF e senha
- **6 aulas por dia**: Cada aula com duração de 50 minutos

### Horários das Aulas
- Aula 1: 07:00-07:50
- Aula 2: 08:00-08:50
- Aula 3: 09:00-09:50
- Aula 4: 10:00-10:50
- Aula 5: 11:00-11:50
- Aula 6: 12:00-12:50

## Como Compilar

### Opção 1: GCC (Linux/MinGW)
```bash
gcc -Wall -Wextra -std=c99 -o sistema_escolar sistema_escolar.c
```

### Opção 2: Visual Studio (Windows)
```cmd
cl sistema_escolar.c
```

### Opção 3: Code::Blocks ou Dev-C++
Abra o arquivo `sistema_escolar.c` no IDE e compile normalmente.

## Como Executar
```bash
./sistema_escolar
```

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
- Máximo 50 turmas
- Máximo 30 alunos por turma
- Máximo 1000 aulas
- Sem limite para número total de alunos e professores (limitado apenas pela memória disponível)

## Observações
- O sistema mantém os dados apenas em memória durante a execução
- Para persistência de dados, seria necessário implementar salvamento em arquivo
- O login funciona tanto para alunos quanto para professores