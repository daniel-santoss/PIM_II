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
- Máximo 50 turmas
- Máximo 30 alunos por turma
- Máximo 1000 aulas
- Sem limite para número total de alunos e professores (limitado apenas pela memória disponível)
