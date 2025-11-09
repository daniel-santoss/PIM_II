EVIDÊNCIAS DE USO DE INTELIGÊNCIA ARTIFICIAL

SISTEMA ESCOLAR - PIM

Versão: 1.0  
Data: 2025  
Documento: Evidências de Uso de IA no Desenvolvimento

---

1. INTRODUÇÃO

Este documento apresenta as evidências do uso de ferramentas de Inteligência Artificial durante o desenvolvimento do Sistema Escolar, conforme solicitado na documentação do PIM. Todas as interações foram realizadas com o Amazon Q Developer, assistente de IA integrado ao ambiente de desenvolvimento.

2. FERRAMENTAS DE IA UTILIZADAS

2.1 Amazon Q Developer
- Plataforma: Amazon Web Services (AWS)
- Integração: Plugin IDE
- Funcionalidades: Geração de código, documentação, análise e otimização

3. PROMPTS UTILIZADOS NO DESENVOLVIMENTO

3.1 Estruturação Inicial do Projeto

Prompt:
"Preciso criar um sistema escolar em C com as seguintes funcionalidades: 
- Gestão de turmas, alunos e professores
- Sistema de login por perfil
- Controle de aulas e atividades
- Diário eletrônico com notas e presenças
Pode me ajudar a estruturar o código?"

Resultado: Estrutura completa do sistema com definição de structs, funções e arquitetura modular.

3.2 Implementação do Sistema de Login

Prompt:
"Como implementar um sistema de autenticação em C que suporte 3 tipos de usuário: 
administrador, professor e aluno? Preciso validar CPF e senha."

Resultado: Função fazer_login() com validação de CPF, autenticação por arquivo e controle de sessão.

3.3 Validações e Regras de Negócio

Prompt:
"Preciso implementar validações para:
- CPF com 11 dígitos numéricos
- Disciplinas predefinidas (8 matérias)
- Horários por turno escolar
- Controle de permissões por perfil"

Resultado: Funções de validação validar_cpf(), validar_disciplina() e sistema de controle de acesso.

3.4 Diário Eletrônico

Prompt:
"Como implementar um diário eletrônico que permita:
- Lançamento de notas (0.0 a 10.0)
- Controle de presenças (P/F)
- Cálculo de médias por matéria
- Atualização de registros existentes"

Resultado: Módulo completo com funções lancar_notas(), registrar_presenca(), minhas_notas() e cálculos automáticos.

3.5 Documentação Técnica

Prompt:
"Gere um README.md completo para o sistema escolar incluindo:
- Funcionalidades por perfil
- Instruções de compilação
- Estrutura dos dados
- Características técnicas"

Resultado: Documentação completa em formato Markdown com estrutura profissional.

3.6 Manual de Usuário

Prompt:
"Preciso de um manual de uso do sistema em formato acadêmico com:
- Procedimentos passo a passo
- Fluxos operacionais
- Solução de problemas
- Validações do sistema"

Resultado: Manual técnico estruturado com 9 seções principais e formato acadêmico.

3.7 Regras de Negócio

Prompt:
"Gerar as regras de negócio do sistema escolar cobrindo:
- Autenticação e controle de acesso
- Validações de dados
- Integridade referencial
- Regras por módulo (turmas, alunos, professores, etc.)"

Resultado: Documento com 30 regras numeradas (RN001-RN030) organizadas por categoria.

3.8 Otimização e Refatoração

Prompt:
"Todas as partes do código estão comentadas? 
Preciso que o código tenha comentários explicativos em todas as funções."

Resultado: Código completamente comentado com explicações detalhadas de cada função e bloco lógico.

3.9 Análise de Arquitetura

Prompt:
"É possível no mínimo 2 usuários conectados simultaneamente? 
Como posso implementar essa arquitetura de cliente servidor?"

Resultado: Análise técnica detalhada sobre limitações atuais e alternativas de implementação.

4. TEXTOS TÉCNICOS GERADOS

4.1 Comentários de Código
- Quantidade: ~500 comentários
- Tipo: Explicações de funções, validações e lógica de negócio
- Padrão: Comentários em português com descrição técnica precisa

4.2 Documentação de Sistema
- README.md: Visão geral completa do projeto
- MANUAL_DE_USO.md: Guia operacional detalhado
- REGRAS_DE_NEGOCIO.md: Especificação técnica de regras

4.3 Mensagens do Sistema
- Validações: Mensagens de erro padronizadas
- Confirmações: Prompts de confirmação para operações críticas
- Feedback: Mensagens de sucesso e orientação ao usuário

5. APOIO AO DESENVOLVIMENTO

5.1 Geração de Código
- Estruturas de dados: Definição de structs completas
- Funções utilitárias: Validações, conversões e formatações
- Lógica de negócio: Implementação de regras complexas

5.2 Debugging e Otimização
- Identificação de bugs: Análise de lógica e fluxos
- Melhorias de performance: Otimização de algoritmos
- Padronização: Consistência de código e nomenclatura

5.3 Arquitetura de Software
- Modularização: Separação lógica de funcionalidades
- Padrões de projeto: Estruturação adequada do código
- Escalabilidade: Considerações para futuras expansões

6. BENEFÍCIOS OBTIDOS

6.1 Produtividade
- Redução de tempo: ~70% menos tempo de desenvolvimento
- Qualidade de código: Padrões consistentes e boas práticas
- Documentação: Geração automática de textos técnicos

6.2 Qualidade Técnica
- Validações robustas: Controle rigoroso de entrada de dados
- Tratamento de erros: Mensagens claras e recuperação adequada
- Manutenibilidade: Código bem estruturado e comentado

6.3 Documentação Profissional
- Completude: Cobertura total das funcionalidades
- Padronização: Formato acadêmico e técnico adequado
- Clareza: Linguagem técnica precisa e acessível

7. METODOLOGIA DE TRABALHO

7.1 Processo Iterativo
1. Definição de requisitos via prompt específico
2. Geração de código/documentação pela IA
3. Revisão e ajustes conforme necessário
4. Integração ao projeto principal
5. Validação de funcionalidade

7.2 Controle de Qualidade
- Revisão manual de todo código gerado
- Testes funcionais de cada módulo
- Validação de documentação técnica

8. CONCLUSÃO

O uso da Inteligência Artificial (Amazon Q Developer) foi fundamental para o desenvolvimento eficiente e de qualidade do Sistema Escolar. A ferramenta proporcionou:

- Aceleração significativa do processo de desenvolvimento
- Padronização de código e documentação
- Qualidade técnica superior através de boas práticas
- Documentação completa em formato acadêmico

Todos os prompts utilizados demonstram a aplicação prática da IA como ferramenta de apoio ao desenvolvimento, resultando em um sistema robusto e bem documentado.

---

Sistema Escolar - Evidências de Uso de IA  
Desenvolvido com apoio do Amazon Q Developer