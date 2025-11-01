# Sistema Escolar Simplificado

Sistema de gestão escolar desenvolvido em C com funcionalidades essenciais para controle de turmas, alunos, professores, aulas e atividades.

## Funcionalidades

### Sistema de Login
- **Administrador**: CPF: 12345678909 | Senha: admin
- **Alunos**: Login com CPF e senha cadastrados
- **Professores**: Login com CPF e senha cadastrados

### Gestão de Turmas (Admin)
- Cadastrar turmas (ID, nome, série, turno, ano)
- Listar todas as turmas
- Excluir turmas

### Gestão de Alunos (Admin)
- Cadastrar alunos (matrícula, nome, CPF, senha, turma)
- Listar todos os alunos
- Excluir alunos

### Gestão de Professores (Admin)
- Cadastrar professores (matrícula, nome, CPF, senha, matérias)
- Listar todos os professores
- Excluir professores

### Gestão de Aulas (Admin/Professor)
- Registrar aulas (turma, professor, disciplina, data, horário)
- Listar aulas (todas ou próprias)
- Excluir aulas (admin: todas / professor: apenas próprias)

### Gestão de Atividades (Admin/Professor)
- Lançar atividades (tipo, nome, turma, disciplina, data)
- Consultar atividades (filtradas por perfil)
- Excluir atividades (admin: todas / professor: apenas próprias)

### Funcionalidades por Perfil

**Administrador:**
- Acesso completo a todas as funcionalidades
- CRUD de turmas, alunos, professores, aulas e atividades

**Professor:**
- Registrar suas aulas
- Visualizar suas aulas
- Excluir suas aulas
- Lançar atividades
- Consultar suas atividades
- Excluir suas atividades

**Aluno:**
- Visualizar sua turma
- Visualizar aulas da sua turma
- Consultar atividades da sua turma

## Como Compilar e Executar

```bash
gcc sistema_simplificado.c -o sistema_simplificado
./sistema_simplificado
```

## Arquivos de Dados

O sistema cria automaticamente os seguintes arquivos:
- `turmas.txt` - Dados das turmas
- `alunos.txt` - Dados dos alunos
- `professores.txt` - Dados dos professores
- `aulas.txt` - Registro de aulas
- `atividades.txt` - Registro de atividades

## Validações

- CPF deve ter 11 dígitos numéricos
- IDs são gerados automaticamente
- Controle de permissões por perfil de usuário
- Professores só podem editar/excluir suas próprias aulas e atividades

## Características Técnicas

- **Linhas de código**: ~1950 linhas
- **Linguagem**: C (ANSI)
- **Armazenamento**: Arquivos de texto
- **Interface**: Console/Terminal
- **Compilador**: GCC

## Estrutura dos Dados

### Turmas
`ID|Nome|Serie|Turno|Ano`

### Alunos
`Matricula|Nome|CPF|Senha|TurmaID`

### Professores
`Matricula|Nome|CPF|Senha|Materias`

### Aulas
`ID|TurmaID|ProfessorMatricula|Disciplina|Data|Horario`

### Atividades
`ID|Tipo|Nome|TurmaID|ProfessorMatricula|Disciplina|Data`