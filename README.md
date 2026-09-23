# Fase 3 (Aprofundada) — macOS Security Específico

Essa é a fase mais estratégica pro seu perfil: pouca gente foca nisso, então a concorrência é menor e o conhecimento generaliza bem pra Apple Security Bounty. Vou dividir em blocos temáticos com o que estudar, ferramentas e como praticar cada um.

---

## 1. Arquitetura do macOS — antes de atacar, entenda o terreno

- **XNU kernel** (híbrido Mach + BSD) — não precisa virar especialista em kernel, mas entenda a diferença entre user space e kernel space, e como macOS herda conceitos de BSD (permissões Unix) + Mach (IPC, ports)
- **Estrutura de arquivos**: `/System`, `/Library` vs `~/Library`, `/private/var`, bundles (`.app`, `.framework`, `.kext`, `.plist`)
- **Code signing**: como toda a segurança do macOS moderno depende de assinatura de código — entenda `codesign`, entitlements, e o conceito de "hardened runtime"

Comandos pra já ir explorando no seu M1:
```bash
codesign -dv --entitlements - /Applications/Safari.app
spctl -a -vv /Applications/Safari.app
```

---

## 2. TCC (Transparency, Consent and Control) — sua melhor porta de entrada

TCC é o sistema que controla permissões (câmera, microfone, Contatos, acessibilidade, automação entre apps). É **uma das categorias mais produtivas** pra bugs em macOS porque bypasses de TCC são constantemente reportados e pagos.

**O que estudar:**
- Como o TCC.db funciona (`~/Library/Application Support/com.apple.TCC/TCC.db`)
- **TCC bypass via inheritance** — apps que herdam permissão de processos pai
- Ataques clássicos: abusar de apps com permissão de Automação (AppleScript/Apple Events) pra "pedir emprestado" acesso de outro app já autorizado
- Pesquisadores de referência: **Csaba Fitzl** (Offensive Security) tem talks excelentes sobre TCC bypasses, e o **Wojciech Reguła** (blog "Wojciech Regula" / SecuRing) é praticamente o nome de referência nesse tópico

**Prática:**
```bash
sqlite3 ~/Library/Application\ Support/com.apple.TCC/TCC.db "SELECT * FROM access"
```
(precisa de Full Disk Access no Terminal pra rodar isso — ótimo exercício de entender o próprio sistema de permissão)

---

## 3. XPC Services — o vetor mais comum de privilege escalation

XPC é o mecanismo de IPC (comunicação entre processos) do macOS. Apps privilegiados expõem XPC services, e configuração errada ali = escalação de privilégio.

**O que estudar:**
- Como funciona um XPC listener e como um app cliente se conecta a ele
- **Falta de validação do processo que conecta** (não checar `audit_token` ou entitlements do client) é a vulnerabilidade clássica
- Named vs anonymous XPC connections

**Ferramentas:**
- **`ps aux | grep xpc`** e **Activity Monitor** pra mapear XPC services rodando
- **Ghidra** ou **Hopper** pra decompilar binários que implementam XPC listeners e ver se validam o cliente
- **`lsof`**, **`dtrace`** pra observar comunicação em tempo real

Ferramenta open-source excelente pra praticar: **XPoCe** e os labs do **"Sandbox Escape" da Offensive Security macOS courses (EXP-312)** — se puder investir no curso, é o mais direcionado que existe pro que você quer.

---

## 4. LaunchAgents / LaunchDaemons — persistência e escalação

- Diferença entre LaunchAgent (`~/Library/LaunchAgents`, roda como usuário) e LaunchDaemon (`/Library/LaunchDaemons`, roda como root)
- Vulnerabilidade clássica: **plist mal configurado** apontando pra um binário gravável pelo usuário → escalação pra root quando o daemon reinicia ou o sistema boota
- **Race conditions** em scripts chamados por LaunchDaemons rodando como root

**Prática:**
```bash
ls -la /Library/LaunchDaemons/
ls -la /Library/LaunchAgents/
plutil -p /Library/LaunchDaemons/algum.plist
```
Procure plists apontando pra caminhos com permissão de escrita do seu usuário — isso é literalmente o exercício de "encontre a falha" que auditores fazem.

---

## 5. Gatekeeper, Notarização e Sandbox Escape

- **Gatekeeper**: valida se um app baixado da internet está assinado e notarizado antes de rodar
- Bypasses históricos envolveram **quarantine attribute** (`com.apple.quarantine`) sendo removido ou não aplicado corretamente em certos tipos de arquivo/extração
- **App Sandbox**: entitlements como `com.apple.security.*` definem o que um app sandboxed pode acessar — sandbox escapes (sair do container do app) são categoria de alto valor no Apple Security Bounty

**Comandos úteis:**
```bash
xattr -l arquivo_baixado.dmg   # ver quarantine attribute
codesign --verify --deep --strict --verbose=2 /Applications/App.app
```

---

## 6. Ferramentas de observação dinâmica (essenciais no dia a dia)

- **`dtrace`** — tracing de syscalls, chamadas de função, altamente poderoso (nota: precisa desabilitar parcialmente o SIP pra usar em todo seu potencial — faça isso só numa máquina de teste, nunca na sua principal)
- **`fs_usage`** — monitora acesso a arquivos em tempo real, ótimo pra ver o que um app realmente lê/escreve
- **Console.app** — logs do sistema, essencial pra ver crashes, negações de TCC, erros de sandbox
- **`log stream --predicate`** — versão CLI do Console, scriptável

Exemplo prático (ver em tempo real o que um app tenta acessar):
```bash
sudo fs_usage -w -f filesystem | grep NomeDoApp
```

---

## Ordem de estudo sugerida (dentro da Fase 3, ~3-4 meses)

| Semanas | Tópico |
|---|---|
| 1-2 | Code signing, entitlements, estrutura do sistema |
| 3-5 | TCC — leia tudo do Wojciech Reguła e Csaba Fitzl, replique bypasses antigos (já corrigidos) em VM |
| 6-8 | XPC services — decompile apps reais, procure validação fraca de cliente |
| 9-10 | LaunchAgents/Daemons — audite seu próprio sistema em busca de plists mal configurados |
| 11-13 | Gatekeeper/Sandbox — estude CVEs antigas de sandbox escape (bom exercício: ler os writeups completos) |
| 14+ | Consolidação: monte 1-2 "mini pesquisas" próprias em apps de terceiro que você usa, documentando como se fosse um report |

⚠️ **Importante sobre ambiente**: pesquisa de macOS deve ser feita numa **VM dedicada** (via UTM, com uma imagem de macOS separada da sua principal) ou numa máquina secundária. Desabilitar SIP e mexer em TCC/Gatekeeper na sua máquina de trabalho é arriscado.

---

Quer que eu aprofunde como configurar essa **VM de macOS isolada no UTM** pra pesquisa segura, ou prefere que eu monte um roteiro de **CVEs históricas de TCC/XPC bypass** pra você estudar como case studies?
