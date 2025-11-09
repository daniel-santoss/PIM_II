MANUAL DE USUÁRIO

SISTEMA ESCOLAR

Versão: 1.0  
Data: 2025  
Linguagem: C (ANSI)

---

SUMÁRIO

1. INTRODUÇÃO
2. INSTALAÇÃO E CONFIGURAÇÃO
3. SISTEMA DE AUTENTICAÇÃO
4. MÓDULO ADMINISTRATIVO
5. MÓDULO DO PROFESSOR
6. MÓDULO DO ALUNO
7. VALIDAÇÕES E REGRAS DE NEGÓCIO
8. SOLUÇÃO DE PROBLEMAS
9. CONSIDERAÇÕES FINAIS

---

1. INTRODUÇÃO

O Sistema Escolar é uma aplicação desenvolvida em linguagem C para gestão acadêmica, contemplando funcionalidades de controle de turmas, alunos, professores, aulas, atividades avaliativas, notas e presenças.

2. INSTALAÇÃO E CONFIGURAÇÃO

2.1 Compilação
gcc sistema_escolar.c -o sistema_escolar

2.2 Execução
sistema_escolar.exe

2.3 Interface Inicial
O sistema apresenta a seguinte interface de entrada:
=== SISTEMA ESCOLAR ===
1. Fazer Login
0. Sair
Opcao:

3. SISTEMA DE AUTENTICAÇÃO

3.1 Credenciais de Acesso
- Administrador: CPF: 12345678909 | Senha: admin
- Professores: Credenciais cadastradas pelo administrador
- Alunos: Credenciais cadastradas pelo administrador

3.2 Procedimento de Login
1. Selecionar opção "1" no menu principal
2. Inserir CPF (11 dígitos numéricos)
3. Inserir senha correspondente
4. Acesso ao módulo específico do usuário

4. MÓDULO ADMINISTRATIVO

4.1 Interface do Administrador
=== MENU ADMINISTRADOR ===
1. Cadastrar Turma          10. Registrar Aula
2. Listar Turmas           11. Listar Aulas
3. Excluir Turma           12. Excluir Aula
4. Cadastrar Aluno         13. Lançar Atividade
5. Listar Alunos           14. Consultar Atividades
6. Excluir Aluno           15. Excluir Atividade
7. Cadastrar Professor     16. Lançar Notas
8. Listar Professores      17. Consultar Notas
9. Excluir Professor       18. Excluir Nota
0. Sair

4.2 Configuração Inicial do Sistema

4.2.1 Cadastro de Turmas (Opção 1)
- Informar série (1, 2 ou 3)
- Definir letra identificadora (A-Z)
- Selecionar turno (Matutino/Vespertino/Noturno)
- Estabelecer ano letivo (2025 ou 2026)

4.2.2 Cadastro de Professores (Opção 7)
- Inserir nome completo
- Informar CPF (11 dígitos numéricos)
- Definir senha de acesso (mínimo 4 caracteres)
- Especificar disciplinas lecionadas (separadas por vírgula)

4.2.3 Cadastro de Alunos (Opção 4)
- Inserir nome completo
- Informar CPF único (11 dígitos numéricos)
- Definir senha de acesso (mínimo 4 caracteres)
- Vincular à turma correspondente

4.3 Gestão Acadêmica

4.3.1 Registro de Aulas (Opção 10)
- Seleção da turma
- Definição do professor responsável
- Especificação da disciplina
- Inserção da data (formato DD/MM/AAAA)
- Escolha do horário disponível

4.3.2 Lançamento de Atividades (Opção 13)
- Definição do tipo (Prova/Trabalho/Exercício)
- Inserção do nome da atividade
- Seleção da turma
- Definição do professor responsável
- Especificação da disciplina
- Inserção da data (formato DD/MM/AAAA)

5. MÓDULO DO PROFESSOR

5.1 Interface do Professor
=== MENU PROFESSOR ===
1. Registrar Aula
2. Minhas Aulas
3. Excluir Aula
4. Lançar Atividade
5. Consultar Atividades
6. Excluir Atividade
7. Diário Eletrônico
0. Sair

5.2 Diário Eletrônico

5.2.1 Interface do Diário
=== DIARIO ELETRONICO ===
1. Lançar Notas
2. Consultar Notas
3. Excluir Nota
4. Presença do Aluno
5. Consultar Presenças
0. Voltar

5.2.2 Lançamento de Notas
1. Seleção da atividade por identificador
2. Escolha do aluno por matrícula
3. Inserção da nota (escala 0.0 a 10.0)
4. Continuidade para demais alunos ou finalização

5.2.3 Controle de Presenças
1. Seleção da turma
2. Escolha da aula específica
3. Seleção do aluno
4. Registro da presença (P - Presente / F - Falta)

5.3 Fluxo Operacional

1. Registro de aulas para as turmas
2. Lançamento de atividades avaliativas
3. Utilização do diário eletrônico para notas e presenças
4. Consulta e verificação dos dados inseridos

6. MÓDULO DO ALUNO

6.1 Interface do Aluno
=== MENU ALUNO ===
1. Minha Turma
2. Minhas Aulas
3. Consultar Atividades
4. Minhas Notas
0. Sair

6.2 Funcionalidades Disponíveis

6.2.1 Consulta de Notas (Opção 4)
- Visualização de todas as notas obtidas
- Cálculo de médias por disciplina
- Apresentação da média geral
- Resumo acadêmico completo

7. VALIDAÇÕES E REGRAS DE NEGÓCIO

7.1 Validações de Entrada
- CPF: Exatamente 11 dígitos numéricos
- Senhas: Mínimo de 4 caracteres
- Datas: Formato obrigatório DD/MM/AAAA
- Notas: Escala de 0.0 a 10.0
- Disciplinas: Restrito às matérias cadastradas

7.2 Disciplinas Disponíveis
- Matemática
- Ciências
- Geografia
- História
- Filosofia
- Sociologia
- Português
- Inglês

7.3 Grade Horária por Turno

7.3.1 Turno Matutino
- 1ª Aula: 07:00-07:50
- 2ª Aula: 07:50-08:40
- 3ª Aula: 08:40-09:30
- 4ª Aula: 10:00-10:50
- 5ª Aula: 10:50-11:40

7.3.2 Turno Vespertino
- 1ª Aula: 13:00-13:50
- 2ª Aula: 13:50-14:40
- 3ª Aula: 14:40-15:30
- 4ª Aula: 16:00-16:50
- 5ª Aula: 16:50-17:40

7.3.3 Turno Noturno
- 1ª Aula: 18:00-18:50
- 2ª Aula: 18:50-19:40
- 3ª Aula: 19:40-20:30
- 4ª Aula: 21:00-21:50
- 5ª Aula: 21:50-22:40

8. SOLUÇÃO DE PROBLEMAS

8.1 Diagnóstico de Erros Comuns

8.1.1 "CPF já cadastrado"
- Causa: Tentativa de cadastro com CPF já existente no sistema
- Solução: Verificar a digitação ou utilizar CPF diferente

8.1.2 "Professor não leciona a disciplina"
- Causa: Tentativa de atribuir disciplina não cadastrada para o professor
- Solução: Verificar as matérias do professor ou atualizar seu perfil

8.1.3 "Turma não encontrada"
- Causa: Referência a identificador de turma inexistente
- Solução: Verificar a listagem de turmas e utilizar ID válido

8.1.4 "Acesso negado"
- Causa: Tentativa de acesso a funcionalidade restrita ao perfil
- Solução: Verificar o tipo de usuário logado e suas permissões

8.2 Gerenciamento de Dados
- Armazenamento: Arquivos de texto com extensão .txt
- Backup: Cópia dos arquivos de dados
- Reset: Exclusão dos arquivos para reinicialização

9. CONSIDERAÇÕES FINAIS

9.1 Fluxo Operacional Recomendado

9.1.1 Configuração Inicial (Administrador)
1. Cadastro de turmas
2. Cadastro de professores
3. Cadastro de alunos
4. Vinculação de alunos às turmas

9.1.2 Operação Acadêmica (Professor)
1. Registro de aulas
2. Lançamento de atividades avaliativas
3. Controle de presenças
4. Lançamento de notas

9.1.3 Acompanhamento (Aluno)
1. Consulta de informações da turma
2. Verificação de aulas e atividades
3. Acompanhamento do desempenho acadêmico

9.2 Observações Importantes
- Utilizar "0" para cancelamento de operações
- Manter backup regular dos dados
- Seguir as validações estabelecidas pelo sistema

---

Sistema Escolar - Manual de Usuário