# REGRAS DE NEGÓCIO

## SISTEMA ESCOLAR

**Versão:** 1.0  
**Data:** 2025  
**Documento:** Especificação de Regras de Negócio

---

## 1. REGRAS GERAIS DO SISTEMA

### RN001 - Autenticação de Usuários
- O sistema deve permitir apenas usuários autenticados
- Cada CPF pode estar associado a apenas um usuário
- Senhas devem ter no mínimo 4 caracteres
- Existe apenas um administrador padrão (CPF: 12345678909)

### RN002 - Controle de Acesso
- Administrador: acesso total ao sistema
- Professor: acesso restrito às suas turmas e atividades
- Aluno: acesso somente leitura às suas informações

### RN003 - Geração de Identificadores
- IDs são gerados automaticamente e sequencialmente
- Matrículas são únicas e não reutilizáveis
- Sistema deve garantir unicidade dos identificadores

## 2. REGRAS DE TURMAS

### RN004 - Cadastro de Turmas
- Série deve ser 1, 2 ou 3
- Letra da turma deve ser de A a Z
- Turno deve ser Matutino, Vespertino ou Noturno
- Ano letivo deve ser 2025 ou 2026
- Não pode existir turma duplicada (mesmo nome, turno e ano)

### RN005 - Exclusão de Turmas
- Turma com aulas registradas requer confirmação para exclusão
- Exclusão de turma remove automaticamente aulas e atividades vinculadas
- Sistema deve alertar sobre dependências antes da exclusão

## 3. REGRAS DE USUÁRIOS

### RN006 - Cadastro de Professores
- Nome não pode estar vazio
- CPF deve ter exatamente 11 dígitos numéricos
- CPF deve ser único no sistema
- Deve lecionar pelo menos uma disciplina
- Disciplinas devem estar na lista de matérias válidas

### RN007 - Cadastro de Alunos
- Nome não pode estar vazio
- CPF deve ter exatamente 11 dígitos numéricos
- CPF deve ser único no sistema
- Pode ser cadastrado sem turma (valor 0)
- Se vinculado a turma, esta deve existir

### RN008 - Exclusão de Usuários
- Professor com aulas/atividades requer confirmação
- Exclusão remove automaticamente registros vinculados
- Sistema deve validar dependências antes da exclusão

## 4. REGRAS DE DISCIPLINAS

### RN009 - Disciplinas Válidas
- Sistema aceita apenas 8 disciplinas predefinidas:
  - Matemática, Ciências, Geografia, História
  - Filosofia, Sociologia, Português, Inglês
- Nomes devem seguir grafia exata (primeira letra maiúscula)
- Professor só pode lecionar disciplinas cadastradas em seu perfil

### RN010 - Validação de Disciplinas
- Aulas só podem ser criadas para disciplinas do professor
- Atividades só podem ser criadas para disciplinas do professor
- Sistema deve verificar compatibilidade professor-disciplina

## 5. REGRAS DE AULAS

### RN011 - Registro de Aulas
- Turma deve existir no sistema
- Professor deve existir no sistema
- Professor deve lecionar a disciplina informada
- Data deve estar no formato DD/MM/AAAA
- Ano da aula deve coincidir com ano da turma
- Horário deve ser compatível com turno da turma

### RN012 - Horários por Turno
- **Matutino**: 07:00-07:50, 07:50-08:40, 08:40-09:30, 10:00-10:50, 10:50-11:40
- **Vespertino**: 13:00-13:50, 13:50-14:40, 14:40-15:30, 16:00-16:50, 16:50-17:40
- **Noturno**: 18:00-18:50, 18:50-19:40, 19:40-20:30, 21:00-21:50, 21:50-22:40

### RN013 - Exclusão de Aulas
- Professor pode excluir apenas suas próprias aulas
- Administrador pode excluir qualquer aula
- Sistema deve confirmar operação de exclusão

## 6. REGRAS DE ATIVIDADES

### RN014 - Tipos de Atividades
- Sistema aceita apenas: Prova, Trabalho, Exercício
- Tipo deve seguir grafia exata (primeira letra maiúscula)
- Nome da atividade não pode estar vazio

### RN015 - Lançamento de Atividades
- Turma deve existir no sistema
- Professor deve existir no sistema
- Professor deve lecionar a disciplina informada
- Data deve estar no formato DD/MM/AAAA
- Ano da atividade deve coincidir com ano da turma

### RN016 - Exclusão de Atividades
- Professor pode excluir apenas suas próprias atividades
- Administrador pode excluir qualquer atividade
- Sistema deve confirmar operação de exclusão

## 7. REGRAS DE NOTAS

### RN017 - Lançamento de Notas
- Nota deve estar entre 0.0 e 10.0
- Atividade deve existir no sistema
- Aluno deve existir no sistema
- Aluno deve pertencer à turma da atividade
- Professor só pode lançar notas para suas atividades

### RN018 - Atualização de Notas
- Sistema deve permitir atualização de notas existentes
- Deve solicitar confirmação antes de atualizar
- Histórico de alterações não é mantido

### RN019 - Exclusão de Notas
- Professor pode excluir apenas notas de suas atividades
- Administrador pode excluir qualquer nota
- Sistema deve confirmar operação de exclusão

## 8. REGRAS DE PRESENÇAS

### RN020 - Registro de Presenças
- Presença deve ser 'P' (Presente) ou 'F' (Falta)
- Aula deve existir no sistema
- Aluno deve existir no sistema
- Aluno deve pertencer à turma da aula
- Professor só pode registrar presenças para suas aulas

### RN021 - Atualização de Presenças
- Sistema deve permitir atualização de presenças existentes
- Deve solicitar confirmação antes de atualizar
- Histórico de alterações não é mantido

### RN022 - Consulta de Presenças
- Professor visualiza presenças apenas de suas aulas
- Sistema exibe '-' para alunos sem registro de presença
- Consulta organizada por turma e aula

## 9. REGRAS DE CONSULTAS

### RN023 - Consultas do Aluno
- Aluno visualiza apenas informações de sua turma
- Notas são organizadas por disciplina
- Sistema calcula médias por matéria e geral automaticamente
- Resumo acadêmico inclui total de atividades

### RN024 - Consultas do Professor
- Professor visualiza apenas dados de suas turmas/atividades
- Listagens filtradas por matrícula do professor
- Acesso restrito a informações sob sua responsabilidade

### RN025 - Consultas do Administrador
- Administrador tem acesso a todas as informações
- Visualização completa de dados do sistema
- Inclui informações sensíveis (senhas) nas listagens

## 10. REGRAS DE INTEGRIDADE

### RN026 - Validação de Dados
- CPF deve conter apenas dígitos numéricos
- Datas devem seguir formato DD/MM/AAAA rigorosamente
- IDs devem ser números positivos
- Campos obrigatórios não podem estar vazios

### RN027 - Consistência Referencial
- Exclusão em cascata para registros dependentes
- Validação de existência antes de criar vínculos
- Verificação de integridade antes de operações críticas

### RN028 - Controle de Sessão
- Apenas um usuário logado por vez
- Logout automático ao sair dos menus
- Reset de variáveis de sessão após logout

## 11. REGRAS DE ARMAZENAMENTO

### RN029 - Persistência de Dados
- Dados armazenados em arquivos de texto (.txt)
- Formato pipe-separated (|) para campos
- Codificação de caracteres compatível com sistema

### RN030 - Backup e Recuperação
- Sistema não implementa backup automático
- Responsabilidade do usuário manter cópias dos arquivos
- Reset através de exclusão manual dos arquivos

---

**Sistema Escolar - Regras de Negócio**