# Sistema Escolar

Sistema de gestão escolar desenvolvido em C com funcionalidades completas para controle acadêmico, incluindo turmas, alunos, professores, aulas, atividades, notas e presenças.

## 🚀 Funcionalidades

### 🔐 Sistema de Login
- **Administrador (Único)**: CPF: `12345678909` | Senha: `admin`
- **Alunos**: Login com CPF e senha cadastrados
- **Professores**: Login com CPF e senha cadastrados

### 🏫 Gestão de Turmas (Admin)
- Cadastrar turmas (ID automático, nome, série, turno, ano)
- Listar todas as turmas cadastradas
- Excluir turmas (com validação de dependências)

### 👨‍🎓 Gestão de Alunos (Admin)
- Cadastrar alunos (matrícula automática, nome, CPF, senha, turma)
- Listar todos os alunos com informações da turma
- Excluir alunos do sistema

### 👨‍🏫 Gestão de Professores (Admin)
- Cadastrar professores (matrícula automática, nome, CPF, senha, matérias)
- Listar todos os professores e suas matérias
- Excluir professores (com validação de dependências)

### 📚 Gestão de Aulas (Admin/Professor)
- Registrar aulas (turma, professor, disciplina, data, horário)
- Listar aulas (todas ou filtradas por usuário)
- Excluir aulas (admin: todas / professor: apenas próprias)
- Horários automáticos por turno (Matutino/Vespertino/Noturno)

### 📝 Gestão de Atividades (Admin/Professor)
- Lançar atividades (tipo, nome, turma, disciplina, data)
- Tipos: Prova, Trabalho, Exercício
- Consultar atividades (filtradas por perfil)
- Excluir atividades (admin: todas / professor: apenas próprias)

### 📊 Diário Eletrônico (Professor)
- **Sistema de Notas**:
  - Lançar notas (0.0 a 10.0) para atividades
  - Consultar notas das atividades
  - Atualizar notas existentes
  - Excluir notas
- **Controle de Presenças**:
  - Registrar presença dos alunos (P/F)
  - Consultar presenças por aula
  - Atualizar presenças existentes

## 👥 Funcionalidades por Perfil

### 🔧 **Administrador**
- Acesso completo a todas as funcionalidades
- CRUD de turmas, alunos, professores, aulas e atividades
- Lançamento e consulta de notas
- Controle total do sistema

### 👨‍🏫 **Professor**
- Registrar e gerenciar suas aulas
- Lançar e gerenciar atividades de suas matérias
- Diário eletrônico completo:
  - Lançar e consultar notas
  - Registrar e consultar presenças
- Visualizar apenas dados relacionados às suas turmas

### 👨‍🎓 **Aluno**
- Visualizar informações da sua turma
- Consultar aulas da sua turma
- Visualizar atividades da sua turma
- **Minhas Notas**:
  - Visualizar todas as notas
  - Médias por matéria
  - Média geral
  - Resumo acadêmico completo

## 📁 Arquivos de Dados

O sistema cria automaticamente os seguintes arquivos:
- `turmas.txt` - Dados das turmas
- `alunos.txt` - Dados dos alunos  
- `professores.txt` - Dados dos professores
- `aulas.txt` - Registro de aulas
- `atividades.txt` - Registro de atividades
- `notas.txt` - Notas dos alunos
- `presencas.txt` - Controle de presenças

## ✅ Validações e Regras

- **CPF**: Deve ter exatamente 11 dígitos numéricos
- **IDs**: Gerados automaticamente e únicos
- **Senhas**: Mínimo de 4 caracteres
- **Disciplinas**: Apenas matérias válidas do sistema
- **Notas**: Entre 0.0 e 10.0
- **Datas**: Formato DD/MM/AAAA
- **Permissões**: Controle rigoroso por perfil de usuário
- **Integridade**: Professores só editam seus próprios dados

## 🎯 Matérias Disponíveis

- Matemática
- Ciências  
- Geografia
- História
- Filosofia
- Sociologia
- Português
- Inglês

## ⚙️ Características Técnicas

- **Linhas de código**: ~2800 linhas
- **Linguagem**: C (ANSI)
- **Armazenamento**: Arquivos de texto (formato pipe-separated)
- **Interface**: Console/Terminal
- **Compilador**: GCC
- **Módulos**: 7 módulos principais
- **Estruturas**: 6 estruturas de dados principais

## 📋 Estrutura dos Dados

### Turmas
```
ID|Nome|Serie|Turno|Ano
1|1 Ano A|1|Matutino|2025
```

### Alunos  
```
Matricula|Nome|CPF|Senha|TurmaID
1001|João Silva|12345678901|1234|1
```

### Professores
```
Matricula|Nome|CPF|Senha|Materias
2001|Maria Santos|98765432109|prof123|Matematica, Ciencias
```

### Aulas
```
ID|TurmaID|ProfessorMatricula|Disciplina|Data|Horario
1|1|2001|Matematica|15/03/2025|07:00-07:50
```

### Atividades
```
ID|Tipo|Nome|TurmaID|ProfessorMatricula|Disciplina|Data
1|Prova|Prova Bimestral|1|2001|Matematica|20/03/2025
```

### Notas
```
ID|AtividadeID|AlunoMatricula|Nota
1|1|1001|8.5
```

### Presenças
```
ID|AulaID|AlunoMatricula|Presenca
1|1|1001|P
```

## 🔄 Fluxo de Uso

1. **Login** com credenciais apropriadas
2. **Administrador** configura turmas, alunos e professores
3. **Professores** registram aulas e atividades
4. **Professores** lançam notas e controlam presenças
5. **Alunos** consultam suas informações acadêmicas

## 🛡️ Segurança

- Controle de acesso por perfil
- Validação de dados de entrada
- Proteção contra operações não autorizadas
- Confirmação para operações críticas

## 📈 Relatórios Disponíveis

- Lista completa de turmas, alunos e professores
- Aulas por turma e professor
- Atividades por turma e disciplina
- Notas por atividade e aluno
- Presenças por aula
- Médias acadêmicas individuais

---

**Desenvolvido em C** | **Sistema Completo de Gestão Escolar**