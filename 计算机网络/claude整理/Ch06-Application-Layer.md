# Chapter 06: Application Layer / 应用层

> **Exam Relevance / 考试相关性**: The application layer is primarily tested through fill-in-blanks, true/false, and multiple choice questions. It's unlikely to appear as a big calculation question. Key topics include DNS resolution process, HTTP protocol basics, and email protocols. Understanding of application layer protocols is essential for the "addresses in communication" comprehensive question (Topic 11).
> **中文**：应用层主要通过填空题、判断题和选择题考查，不太可能出大题计算题。关键主题包括DNS解析过程、HTTP协议基础和电子邮件协议。理解应用层协议对于"通信中的地址"综合题（专题11）至关重要。

---

## 6.1 DNS — Domain Name System / 域名系统 ★★

### 6.1.1 What is DNS? / DNS 是什么？

DNS (Domain Name System / 域名系统) translates human-readable domain names into IP addresses. **DNS 将人类可读的域名转换为 IP 地址。**

**Core function / 核心功能**: `www.example.com` → `93.184.216.34`

**Why DNS? / 为什么需要 DNS？**
- Humans remember names / 人类记名字（如 google.com）
- Routers need IP addresses / 路由器需要 IP 地址（如 142.250.80.46）
- DNS bridges this gap / DNS 架起了这座桥梁

### 6.1.2 DNS Namespace / DNS 命名空间

DNS uses a **hierarchical, tree-structured** naming system. / DNS 使用**分层的树状**命名系统。

```
                    Root (.) / 根
         ┌───────────┬───────────┐
        com         org         net         ← Top-Level Domains (TLDs) / 顶级域名
      ┌──┴──┐        │           │
   google  example  wikipedia  example    ← Second-Level Domains / 二级域名
      │      │
    www    mail                            ← Subdomains / 子域名
```

**Domain levels / 域名层级**:
| Level / 层级 | Example / 示例 | Description / 描述 |
|:---:|------|------|
| Root / 根 | `.` (dot / 点) | The top of the DNS hierarchy, represented by a trailing dot / DNS 层级结构的顶端 |
| TLD / 顶级域名 | `.com`, `.org`, `.net`, `.cn`, `.edu` | Generic (.com, .org) or country-code (.cn, .uk) / 通用顶级域名或国家代码顶级域名 |
| Second-level / 二级 | `google`, `baidu` | Registered by organizations / 由组织注册 |
| Subdomain / 子域名 | `www`, `mail`, `docs` | Created by domain owner / 由域名所有者创建 |

**FQDN (Fully Qualified Domain Name) / 完全限定域名**: `www.example.com.` (includes the trailing root dot / 包含末尾的根点)

### 6.1.3 DNS Resolution Process / DNS 解析过程 ★★

When you type `www.example.com` in your browser, DNS resolution follows these steps:/当你输入网址时的解析步骤：

**Two types of resolution / 两种解析方式**:

#### Recursive Resolution / 递归解析

```
Client / 客户端 → Local DNS Server / 本地 DNS 服务器 → ... → Result / 结果
```

The client asks the local DNS server, which does all the work and returns the final answer./客户端请求本地DNS服务器，服务器完成所有查询工作后返回最终结果。

#### Iterative Resolution / 迭代解析

```
Client / 客户端
   ↓ ① Query / 查询 www.example.com
Local DNS Server / 本地 DNS 服务器
   ↓ ② Ask Root Server / 询问根服务器
Root DNS Server / 根 DNS 服务器
   ↓ ③ "I don't know, ask .com TLD server" / 我不知道，去问 .com 服务器
Local DNS Server
   ↓ ④ Ask .com TLD Server / 询问 .com 顶级域名服务器
TLD DNS Server / 顶级域名服务器
   ↓ ⑤ "I don't know, ask example.com's authoritative server" / 我不知道，去问权威服务器
Local DNS Server
   ↓ ⑥ Ask Authoritative Server / 询问权威 DNS 服务器
Authoritative DNS Server / 权威 DNS 服务器
   ↓ ⑦ "www.example.com = 93.184.216.34"
Local DNS Server → caches answer / 缓存答案 → Client / 客户端
```

**Key DNS record types / 主要 DNS 记录类型**:

| Record Type / 记录类型 | Full Name / 全称 | Purpose / 用途 |
|:---:|------|------|
| **A** | Address Record | Maps domain → IPv4 address / 域名 → IPv4地址 |
| **AAAA** | IPv6 Address Record | Maps domain → IPv6 address / 域名 → IPv6地址 |
| **CNAME** | Canonical Name | Alias / 别名 (www → actual server name) |
| **MX** | Mail Exchange | Points to mail server for the domain / 指定邮件服务器 |
| **NS** | Name Server | Points to authoritative DNS server / 指定权威DNS服务器 |
| **PTR** | Pointer Record | Reverse DNS: IP → domain name / 反向解析：IP → 域名 |

### 6.1.4 DNS Transport / DNS 使用的传输协议

**DNS primarily uses UDP port 53 / DNS 主要使用 UDP 端口 53**.

- Why UDP? / 为什么用 UDP？
  - DNS queries are small (typically < 512 bytes) / DNS查询报文小
  - Low overhead (no 3-way handshake) / 低开销（无三次握手）
  - If no response, client retries / 无响应时客户端重试
- For large responses (zone transfers), DNS uses TCP port 53 / 大响应（区域传输）时使用 TCP 53

**Key exam point / 考试要点**: DNS uses **UDP** port 53 (not TCP by default)./ DNS 默认用 **UDP** 53。

### 6.1.5 DNS Abbreviation Scheme / DNS 缩写方案

When a user enters an incomplete domain name (e.g., `example` instead of `example.com`), the **DNS resolver** / **DNS 解析器** automatically appends a local domain suffix to form a fully qualified domain name. / 当用户输入不完整的域名时，DNS解析器会自动追加本地域名后缀以形成完整域名。

**How it works / 工作方式**:
- The resolver tries the name with the local domain suffix appended / 解析器尝试追加本地后缀
- If resolution fails, it may try other configured suffixes / 如果解析失败，可尝试其他配置后缀
- This is why you can type `www` instead of `www.example.com` within an enterprise network / 这就是在企业网络中只需输入 `www` 而非 `www.example.com` 的原因

**Example / 示例**: User types `mail` → Resolver tries `mail.example.com` (if local domain is `example.com`)

> **中文**：DNS缩写方案允许用户输入不完整的域名，解析器自动追加本地域名后缀。例如，在 `example.com` 域中，输入 `mail` 将自动解析为 `mail.example.com`。

---

## 6.2 Email / 电子邮件 ★

### 6.2.1 Email System Architecture / 邮件系统架构

Three components / 三个组件：

```
Sender / 发件人                     Receiver / 收件人
    │                                   ▲
    ↓                                   │
User Agent (UA) / 用户代理              │
(Outlook, Gmail app)                    │
    │                                   │
    ↓ SMTP / 发送                       │ POP3/IMAP / 接收
    │                                   │
Sender's Mail Server / 发件服务器 ──SMTP──→ Receiver's Mail Server / 收件服务器
```

| Component / 组件 | Role / 角色 | Common Examples / 例子 |
|------|------|------|
| **UA (User Agent) / 用户代理** | Interface for reading & composing email / 读写邮件的界面 | Outlook, Thunderbird, Gmail web |
| **Mail Server / 邮件服务器** | Stores & forwards email / 存储和转发邮件 | Exchange, Postfix, Gmail servers |
| **Protocols / 协议** | Rules for transfer & retrieval / 传输和获取规则 | SMTP, POP3, IMAP |

**Detailed Internet Mail System Flow / 互联网邮件系统详细流程**【中文】:

```
User A (Sender / 发件人)                  User B (Receiver / 收件人)
      │                                          ▲
      │                                          │
   User Agent                                   User Agent
   (UA)                                         (UA)
   (Outlook)                                     (Outlook)
      │                                          │
      │ ① SMTP / 发送                            │ ④ POP3/IMAP / 获取
      ▼                                          │
 Sender's ──── ② SMTP Relay / 中继 ────→ Receiver's
 Mail Server                              Mail Server
 (Port 25)      (via Internet / 通过互联网)       (Port 110/143)
      │                                          │
      │                                          ▼
   Mailbox A                                Mailbox B
   (sent mail / 已发送)                    (received mail / 收件箱)
```

**Step-by-step flow / 逐步流程**:
1. **Sending / 发送**: UA sends email via **SMTP** to sender's mail server / 用户代理通过**SMTP**将邮件发送到发件服务器
2. **Relay / 中继**: Sender's mail server relays via **SMTP** to receiver's mail server (may pass through multiple intermediate MTAs) / 发件服务器通过**SMTP**中继到收件服务器（可能经过多个中间MTA）
3. **Storage / 存储**: Receiver's mail server stores email in the recipient's **Mailbox** (Mailbox is a storage area for incoming messages) / 收件服务器将邮件存储在收件人的**邮箱**中（邮箱是存储来信的存储区域）
4. **Retrieval / 获取**: UA retrieves email from the mailbox via **POP3** or **IMAP** / 用户代理通过**POP3**或**IMAP**从邮箱获取邮件

**Key terms / 关键术语**:
- **Mailbox / 邮箱**: A storage area on the mail server for storing received emails / 邮件服务器上用于存储收到的电子邮件的存储区域
- **MTA (Mail Transfer Agent) / 邮件传输代理**: A mail server that routes and relays emails / 负责路由和中继邮件的邮件服务器
- **MAA (Mail Access Agent) / 邮件访问代理**: Software that helps users access emails from the mailbox / 帮助用户从邮箱访问邮件的软件

> **中文**：互联网邮件系统完整流程：用户A的UA通过SMTP将邮件发送到发件服务器，发件服务器再通过SMTP将邮件中继到收件服务器。收件服务器将邮件存储在收件人的邮箱中，用户B随后通过POP3或IMAP从收件服务器获取邮件。

### 6.2.2 Email Protocols / 邮件协议

| Protocol / 协议 | Full Name / 全称 | Port / 端口 | Direction / 方向 | Purpose / 用途 |
|:---:|------|:---:|------|------|
| **SMTP** | Simple Mail Transfer Protocol / 简单邮件传输协议 | **25** (or 587) | UA → Server, Server → Server | **Send** email / **发送**邮件 |
| **POP3** | Post Office Protocol v3 / 邮局协议第3版 | **110** | Server → UA | **Download** email to local, then delete from server / **下载**邮件到本地后删除 |
| **IMAP** | Internet Message Access Protocol / 互联网邮件访问协议 | **143** | Server → UA | **Manage** email on server, keep copies / 在服务器上**管理**邮件并保留副本 |

**Exam note / 考试注意**: SMTP = push protocol (推送协议); POP3/IMAP = pull protocols (拉取协议).

### 6.2.3 SMTP vs HTTP for Email / SMTP vs HTTP 对比

- SMTP is a **push** protocol (sender initiates transfer) / SMTP 是**推送**协议
- SMTP uses **7-bit ASCII** for message body / SMTP 使用 7-bit ASCII
- MIME (Multipurpose Internet Mail Extensions / 多用途互联网邮件扩展) extends SMTP to support binary attachments, non-ASCII text / MIME 扩展了 SMTP 以支持二进制附件和非 ASCII 文本

### 6.2.4 SMTP Commands / SMTP 命令 【中文】

SMTP uses **text-based commands** for email transfer. Each command is a simple ASCII string followed by parameters. / SMTP 使用**基于文本的命令**进行邮件传输。

| Command / 命令 | Description / 描述 |
|:---:|------|
| **HELO** | Identify the sender's SMTP server to the receiver / 向接收服务器标识发件SMTP服务器 |
| **MAIL FROM** | Specify the sender's email address / 指定发件人邮箱地址 |
| **RCPT TO** | Specify the recipient's email address / 指定收件人邮箱地址 |
| **DATA** | Indicate the beginning of email content (terminated by a line with a single `.`) / 表示邮件正文开始（以单独一行的`.`结束）|
| **QUIT** | Terminate the SMTP session / 终止SMTP会话 |
| **RSET** | Reset the current transaction — abort current email but keep connection / 重置当前事务——中止当前邮件但保持连接 |
| **VRFY** | Verify if a user's mailbox exists (often disabled for security reasons) / 验证用户邮箱是否存在（常因安全原因禁用）|

**Typical SMTP session / 典型SMTP会话**:
```
C: HELO smtp.example.com
S: 250 Hello
C: MAIL FROM:<alice@example.com>
S: 250 OK
C: RCPT TO:<bob@test.com>
S: 250 OK
C: DATA
S: 354 Start mail input
C: (email content...)
C: .
S: 250 Message accepted
C: QUIT
S: 221 Bye
```

> **中文**：SMTP使用文本命令传输邮件，关键命令包括：HELO（标识服务器）、MAIL FROM（声明发件人）、RCPT TO（声明收件人）、DATA（正文开始）、QUIT（结束）和RSET（重置当前事务）。

### 6.2.5 POP3 Commands / POP3 命令 【中文】

POP3 uses **text-based commands** for email retrieval. / POP3 使用基于文本的命令获取邮件。

| Command / 命令 | Description / 描述 |
|:---:|------|
| **USER** | Send username to the server for authentication / 发送用户名到服务器进行身份验证 |
| **PASS** | Send password for authentication / 发送密码进行身份验证 |
| **LIST** | List all messages with their sizes (message number + size in bytes) / 列出所有邮件及其大小（邮件编号+字节数）|
| **RETR** | Retrieve a specific message by number / 按编号获取特定邮件 |
| **DELE** | Mark a message for deletion (deleted after QUIT) / 标记邮件为删除（QUIT后真正删除）|
| **QUIT** | End session and apply changes (delete marked messages, release locks) / 结束会话并应用更改（删除已标记邮件，释放锁）|
| **STAT** | Get mailbox status: number of messages and total size / 获取邮箱状态：邮件数量和总大小 |

**Typical POP3 session / 典型POP3会话**:
```
C: USER bob
S: +OK
C: PASS secret123
S: +OK logged in
C: STAT
S: +OK 5 1280
C: LIST
S: +OK 5 messages
S: 1 300
S: 2 450
S: .
C: RETR 1
S: +OK (email content...)
C: DELE 1
S: +OK
C: QUIT
S: +OK Bye
```

> **中文**：POP3使用文本命令获取邮件，关键命令包括：USER（用户名）、PASS（密码）、LIST（列出邮件及大小）、RETR（按编号获取邮件）、DELE（标记删除）、QUIT（结束）和STAT（查看邮箱状态）。

### 6.2.6 MIME Details / MIME 详解 【中文】

**MIME** (Multipurpose Internet Mail Extensions / 多用途互联网邮件扩展) extends the limited SMTP (7-bit ASCII only) to support richer content. / MIME 扩展了仅支持7位ASCII的SMTP以支持更丰富的内容。

**What MIME adds / MIME 增加的功能**:
- Non-ASCII text (e.g., Chinese, Japanese, Arabic characters) / 非ASCII文本（如中文、日文、阿拉伯文字符）
- Binary attachments (images, audio, video, PDF files) / 二进制附件（图片、音频、视频、PDF文件）
- Multipart messages (combining multiple content types in one email) / 多部分消息（在一个邮件中组合多种内容类型）

**Key MIME headers / 重要MIME头部**:

| Header / 头部 | Purpose / 用途 | Example / 示例 |
|:---:|------|:---:|
| **MIME-Version** | Indicates MIME version / 指示MIME版本 | `MIME-Version: 1.0` |
| **Content-Type** | Media type of the message body / 消息体的媒体类型 | `Content-Type: text/plain` |
| **Content-Transfer-Encoding** | Encoding method used (base64, quoted-printable, 7bit, 8bit, binary) / 使用的编码方式 | `Content-Transfer-Encoding: base64` |
| **Content-Disposition** | Whether content is inline or attachment / 内容为内联还是附件 | `Content-Disposition: attachment` |

**Common Content-Type values / 常见 Content-Type 值**:
- `text/plain` — Plain text / 纯文本
- `text/html` — HTML formatted text / HTML格式文本
- `image/jpeg`, `image/png` — Image attachments / 图片附件
- `application/pdf` — PDF document / PDF文档
- `multipart/mixed` — Mixed content types (text + attachments) / 混合内容类型（文本+附件）
- `multipart/alternative` — Same content in multiple formats (e.g., plain text + HTML) / 同一内容的多种格式（如纯文本+HTML）

**Multipart message example / 多部分消息示例**:
```
From: alice@example.com
To: bob@test.com
MIME-Version: 1.0
Content-Type: multipart/mixed; boundary="sep123"

--sep123
Content-Type: text/plain

Hello Bob, please see the attached file.

--sep123
Content-Type: application/pdf
Content-Transfer-Encoding: base64

(base64-encoded PDF content...)
--sep123--
```

> **中文**：MIME扩展了SMTP以支持非ASCII字符、二进制附件和多部分消息。关键头部包括Content-Type（内容类型）、Content-Transfer-Encoding（传输编码）和MIME-Version。multipart/mixed用于同时包含文本和附件，boundary参数分隔不同部分。

### 6.2.7 Mail Gateway and Mail Exploder / 邮件网关与邮件分发器 【中文】

**Mail Gateway / 邮件网关**:
- Connects **different email systems** (e.g., between a corporate internal email system and the public Internet) / 连接**不同的邮件系统**（如企业内部邮件系统与公共互联网之间）
- Performs **protocol conversion** if needed (e.g., between different email formats or protocols) / 必要时进行**协议转换**（如不同邮件格式或协议之间）
- Can act as a **spam filter**, **virus scanner**, and **security firewall** / 可作为**垃圾邮件过滤器**、**病毒扫描器**和**安全防火墙**
- Relays emails between different network domains / 在不同网络域之间中继邮件

```
Internet ──→ Mail Gateway ──→ Corporate Email System
                 │
            (filter + convert / 过滤 + 转换)
```

**Mail Exploder (Mailing List) / 邮件分发器（邮件列表）**:
- Also known as a **mailing list** / 也称为**邮件列表**
- When an email is sent to a single **list address**, the exploder forwards copies to **all subscribers** / 当邮件发送到**列表地址**时，分发器将副本转发给**所有订阅者**
- Uses a single email address to reach many people / 使用单一邮件地址到达多人

```
Sender ──→ Mail Exploder ──→ Subscriber 1
                  │              Subscriber 2
                  │              Subscriber 3
                  └──→ ...
```

**Common uses / 常见用途**:
- Discussion groups (e.g., mailing lists for courses, open-source projects) / 讨论组（如课程邮件列表、开源项目）
- Newsletters and announcements / 新闻通讯和公告
- Broadcast messages within organizations / 组织内部广播消息

> **中文**：邮件网关连接不同邮件系统，进行协议转换、垃圾过滤和安全检查；邮件分发器（邮件列表）将发送到列表地址的邮件自动转发给所有订阅者，实现一对多通信。

---

## 6.3 HTTP — HyperText Transfer Protocol / 超文本传输协议 ★★

### 6.3.1 HTTP Basics / HTTP 基础

HTTP is the protocol for transferring web pages. / HTTP 是传输网页的协议。

**Default port / 默认端口**: **80** (HTTP), **443** (HTTPS)

**HTTP is stateless / HTTP 是无状态的** — each request is independent; the server doesn't remember previous requests / 每个请求互相独立，服务器不记录之前的请求。

### 6.3.2 Web Client-Server Interaction Model / Web 客户端-服务器交互模型 【中文】

The Web operates on a **client-server architecture** / Web 基于**客户端-服务器架构**。

**Four key components / 四个关键组件**:
- **Browser / 浏览器** (Client / 客户端): Sends HTTP requests and renders HTML responses / 发送HTTP请求并渲染HTML响应
- **HTTP Protocol / HTTP协议**: The communication language between browser and server / 浏览器与服务器之间的通信语言
- **Web Server / Web服务器**: Receives requests, processes them, and returns responses (e.g., Apache, Nginx) / 接收请求、处理并返回响应（如Apache、Nginx）
- **HTML / 超文本标记语言**: The content language that browsers render as web pages / 浏览器渲染为网页的内容语言

**Interaction flow / 交互流程**:
```
┌──────────┐  HTTP Request / HTTP请求  ┌──────────┐
│ Browser  │ ─────────────────────────→│   Web    │
│ (Client) │                           │  Server  │
│ Chrome,  │ ←─────────────────────────│ Apache,  │
│ Firefox  │  HTTP Response / HTTP响应  │  Nginx   │
└──────────┘   (HTML page / HTML页面)   └──────────┘
```

**Step-by-step process / 逐步过程**:
1. User enters URL in browser / 用户在浏览器中输入URL
2. **DNS** resolves domain to IP address / DNS将域名解析为IP地址
3. Browser establishes **TCP connection** to the server / 浏览器与服务器建立TCP连接
4. Browser sends **HTTP request** (e.g., GET /index.html) / 浏览器发送HTTP请求
5. Server processes request and returns **HTTP response** with HTML content / 服务器处理请求并返回包含HTML内容的HTTP响应
6. Browser renders the HTML as a **web page** / 浏览器将HTML渲染为网页

> **中文**：Web客户端-服务器交互模型中，浏览器作为客户端发送HTTP请求，Web服务器处理请求并返回HTML响应，最终由浏览器渲染为可视化网页。DNS、TCP和HTTP三者协作完成整个通信过程。

### 6.3.3 HTTP Request Methods / HTTP 请求方法

| Method / 方法 | Purpose / 用途 |
|:---:|------|
| **GET** | Retrieve a resource / 获取资源（下载网页/图片）|
| **POST** | Submit data to server / 提交数据到服务器 |
| **PUT** | Upload/Replace a resource / 上传/替换资源 |
| **HEAD** | Like GET but only return headers / 和GET一样但只返回头部 |
| **DELETE** | Delete a resource / 删除资源 |

**HTTP Command Categories / HTTP 命令分类**:

HTTP methods can be categorized by their **safety** and **idempotence** properties: / HTTP方法可按**安全性**和**幂等性**分类：

| Category / 分类 | Definition / 定义 | Methods / 方法 |
|:---:|------|:---:|
| **Safe / 安全** | Does not change server state (read-only) / 不改变服务器状态（只读） | **GET**, **HEAD** |
| **Idempotent / 幂等** | Multiple identical requests have the same effect as one / 多个相同请求的效果与单个请求相同 | **GET**, **HEAD**, **PUT**, **DELETE** |
| **Non-idempotent / 非幂等** | Multiple identical requests may have different effects / 多个相同请求可能产生不同效果 | **POST** |

- **Safe methods / 安全方法**: GET and HEAD never modify server resources. Users can safely click links and refresh pages. / GET和HEAD从不修改服务器资源，用户可以安全地点击链接和刷新页面。
- **Idempotent methods / 幂等方法**: PUT (update/replace) and DELETE (remove) — repeating the same request produces the same result. / PUT（更新/替换）和DELETE（删除）——重复相同请求产生相同结果。
- **POST is neither safe nor idempotent / POST既不安全也不幂等**: Submitting a form twice may create two orders. / 提交两次表单可能会创建两个订单。

> **中文**：GET和HEAD是安全方法（不改变服务器状态）；GET、HEAD、PUT和DELETE是幂等方法（多次请求效果相同）；POST既不安全也不幂等。

### 6.3.4 HTTP Connections / HTTP 连接方式

| Type / 类型 | How it works / 工作方式 | Pros/Cons / 优缺点 |
|------|------|------|
| **Non-persistent HTTP / 非持久连接** | One TCP connection per object / 每个对象一个TCP连接 | Simple but slow (TCP handshake for each) / 简单但慢 |
| **Persistent HTTP / 持久连接** | Multiple objects over one TCP connection / 一个TCP连接传输多个对象 | Faster, default in HTTP/1.1 / 更快，HTTP/1.1默认 |

### 6.3.5 HTTP Request/Response Format / HTTP 请求/响应格式

**Request / 请求**:
```
GET /index.html HTTP/1.1           ← Request line / 请求行 (Method URL Version)
Host: www.example.com              ← Header lines / 头部行
Connection: keep-alive
                                   ← Blank line / 空行
                                   ← (Optional body / 可选的消息体)
```

**Response / 响应**:
```
HTTP/1.1 200 OK                    ← Status line / 状态行 (Version Status-Code Phrase)
Content-Type: text/html            ← Header lines / 头部行
Content-Length: 1234

<html>...</html>                   ← Body / 消息体
```

**Common status codes / 常见状态码**:

| Code / 状态码 | Phrase / 短语 | Meaning / 含义 |
|:---:|------|------|
| **200** | OK | Request succeeded / 请求成功 |
| **301** | Moved Permanently | Resource moved to new URL / 资源永久转移到新URL |
| **400** | Bad Request | Request syntax error / 请求语法错误 |
| **404** | Not Found | Resource not found on server / 服务器上找不到资源 |
| **500** | Internal Server Error | Server-side error / 服务器内部错误 |

### 6.3.6 Cookies / Cookie

Since HTTP is stateless, **cookies** are used to maintain state (e.g., login sessions, shopping carts)./由于 HTTP 无状态，**Cookie** 用来维持状态信息。

**Cookie components / Cookie 的组成**:
1. Cookie header in HTTP response (Set-Cookie) / 响应头中的 Set-Cookie
2. Cookie header in HTTP request (Cookie) / 请求头中的 Cookie
3. Cookie file stored on user's computer / 用户计算机上的 Cookie 文件
4. Back-end database at the server / 服务器端的后端数据库

### 6.3.8 URL Structure / URL 结构 【中文】

**URL** (Uniform Resource Locator / 统一资源定位符) is the address of a resource on the Web. / URL 是Web上资源的地址。

**URL components / URL 组成**:
```
  scheme://host:port/path?query#fragment
  ───┬── ─┬─ ─┬─ ─┬─ ──┬── ────┬──
     │     │   │   │    │       └── Fragment / 片段 (e.g., #section2)
     │     │   │   │    └────────── Query string / 查询字符串 (e.g., ?id=123&page=1)
     │     │   │   └────────────── Path / 路径 (e.g., /folder/page.html)
     │     │   └────────────────── Port / 端口 (e.g., :8080, default 80 for HTTP)
     │     └────────────────────── Host / 主机 (e.g., www.example.com)
     └──────────────────────────── Scheme / 协议 (e.g., http, https, ftp)
```

| Component / 组件 | Description / 描述 | Example / 示例 |
|:---:|------|:---:|
| **Scheme / 协议** | Protocol used to access the resource / 访问资源所用的协议 | `http`, `https`, `ftp` |
| **Host / 主机** | Domain name or IP address of the server / 服务器的域名或IP地址 | `www.example.com` |
| **Port / 端口** | Server port number (default: 80 for HTTP) / 服务器端口号 | `:8080` |
| **Path / 路径** | Resource location on the server / 资源在服务器上的位置 | `/search/index.html` |
| **Query / 查询** | Additional parameters passed to the server / 传递给服务器的额外参数 | `?q=hello&lang=en` |
| **Fragment / 片段** | A specific section within the page / 页面内的特定部分 | `#section2` |

**Example breakdown / 示例分解**:
```
https://www.example.com:8080/products?id=100#reviews
```

> **中文**：URL由多个组件组成：scheme（协议，如http/https）、host（主机域名）、port（端口号）、path（路径）、query（查询字符串）和fragment（片段标识符）。格式为 `scheme://host:port/path?query#fragment`。

### 6.3.9 HTML Basics / HTML 基础 【中文】

**HTML** (HyperText Markup Language / 超文本标记语言) is the standard content language for creating web pages. / HTML 是创建网页的标准内容语言。

**Key concepts / 关键概念**:
- **HyperText / 超文本**: Text that contains links (hyperlinks) to other documents / 包含指向其他文档的链接（超链接）的文本
- **Markup / 标记**: Uses tags like `<h1>`, `<p>`, `<a>` to structure content / 使用标签来结构化内容
- **Language / 语言**: A markup language, not a programming language / 标记语言，非编程语言

**Basic HTML structure / 基本HTML结构**:
```html
<!DOCTYPE html>
<html>
  <head><title>Page Title</title></head>
  <body>
    <h1>Heading</h1>
    <p>This is a paragraph.</p>
    <a href="https://example.com">Link</a>
  </body>
</html>
```

**Role in the Web / 在Web中的角色**:
- Browsers download HTML from web servers / 浏览器从Web服务器下载HTML
- Browsers parse and render HTML as visual web pages / 浏览器解析HTML并将其渲染为可视化网页
- HTML works with CSS (styling / 样式) and JavaScript (behavior / 行为)

> **中文**：HTML（超文本标记语言）是Web的内容语言。它使用标签结构化内容，浏览器解析HTML并将其渲染为可视化网页。HTML常与CSS和JavaScript配合使用。

### 6.3.10 Web Document Types / 网页文档类型 【中文】

Web documents can be categorized into three types based on when and where content is generated: / 网页文档根据内容生成的时间和位置可分为三类：

| Type / 类型 | Content Source / 内容来源 | Characteristics / 特点 | Examples / 示例 |
|:---:|------|------|:---:|
| **Static / 静态** | Pre-written HTML files on server / 服务器上预存的HTML文件 | Fixed content, same for every request / 内容固定，每次请求相同 | `.html` files |
| **Dynamic / 动态** | Generated by server-side programs on request / 由服务器端程序按请求生成 | Content varies per request / 内容因请求而异 | PHP, JSP, ASP pages |
| **Active / 活动** | Runs on the client side (browser) / 在客户端（浏览器）运行 | Executes on client, interacts with user / 在客户端执行，与用户交互 | JavaScript, Applets |

- **Static Documents / 静态文档**: Server returns pre-stored HTML files. Fast, cacheable, but content cannot auto-update. / 服务器直接返回预存HTML文件，速度快、可缓存，但内容不能自动更新。
- **Dynamic Documents / 动态文档**: Server runs a program (PHP/JSP) per request, generating unique HTML. Content depends on query parameters, database, authentication, etc. / 服务器每次请求时运行程序生成HTML，内容取决于查询参数、数据库、身份等。
- **Active Documents / 活动文档**: Server sends a script (usually JavaScript) that runs on the client's browser. Responds to user actions without contacting the server again. / 服务器发送脚本到客户端执行，可响应用户操作而无需再次联系服务器。

> **中文**：网页文档分为三类——静态文档（内容固定，预存在服务器上）、动态文档（服务器按需生成，如PHP/JSP）和活动文档（客户端执行，如JavaScript）。三者的主要区别在于内容生成的时间和位置。

### 6.3.7 Web Caching (Proxy Server) / Web 缓存（代理服务器）

A **proxy server (Web cache)** sits between client and origin server. It stores copies of frequently accessed resources./**代理服务器**位于客户端和源服务器之间，缓存常用资源副本。

**Benefits / 优点**:
- Reduces response time / 减少响应时间
- Reduces traffic on access link / 减少网络流量
- Reduces load on origin server / 减少源服务器负载

### 6.3.11 Browser Cache / 浏览器缓存 【中文】

There are two main types of web caching — **client-side** and **proxy**: / Web缓存主要有两种类型——**客户端缓存**和**代理缓存**。

**Client-side Cache (Browser Cache) / 客户端缓存（浏览器缓存）**:
- Stores web resources on the **local computer's disk or memory** / 存储在**本地计算机的磁盘或内存**中
- Controlled entirely by the browser / 完全由浏览器控制
- When revisiting a page, the browser checks the local cache first / 再次访问页面时，浏览器先检查本地缓存
- Reduces network traffic and speeds up page loading / 减少网络流量并加速页面加载

**Proxy Cache (Shared Cache) / 代理缓存（共享缓存）**:
- Stores resources on an **intermediate proxy server** between client and origin server / 存储在客户端和源服务器之间的**代理服务器**上
- **Shared among multiple users** / 由多个用户**共享**
- Typically deployed by organizations or ISPs / 通常由组织或ISP部署
- Reduces bandwidth usage and server load / 减少带宽使用和服务器负载

| Feature / 特性 | Browser Cache / 浏览器缓存 | Proxy Cache / 代理缓存 |
|:---:|:---:|:---:|
| Location / 位置 | Client's device / 客户端设备 | Intermediate server / 中间服务器 |
| Scope / 范围 | Single user / 单用户 | Multiple users / 多用户 |
| Control / 控制 | User's browser settings / 用户浏览器设置 | Network administrator / 网络管理员 |
| Benefit / 好处 | Faster revisits / 快速回访 | Bandwidth saving / 节省带宽 |

> **中文**：浏览器缓存存储在客户端本地，仅服务于单个用户，由浏览器控制；代理缓存存储在中间服务器上，由多个用户共享，由网络管理员控制。两者都用于减少网络流量和加速页面加载。

### 6.3.12 AJAX — Asynchronous JavaScript and XML / AJAX 异步交互 【中文】

**AJAX** (Asynchronous JavaScript and XML) allows web pages to update content **asynchronously** without reloading the entire page. / AJAX 允许网页**异步**更新内容，无需重新加载整个页面。

**Traditional vs AJAX model / 传统模型与AJAX模型对比**:
```
Traditional / 传统方式:
Browser ──→ Server (full page request) ──→ Browser (full page reload / 整个页面刷新)

AJAX way / AJAX方式:
Browser ──→ JavaScript Engine ──→ Server (partial data / 部分数据)
    ↑                                    │
    └──── (update part of page / 更新部分页面) ←────────┘
```

**Key characteristics / 关键特性**:
- Uses **XMLHttpRequest** or **Fetch API** to communicate with the server in the background / 使用 XMLHttpRequest 或 Fetch API 在后台与服务器通信
- Page is updated **incrementally** without full reload / 页面**增量更新**，无需完全刷新
- Common data formats: JSON (modern / 现代), XML (traditional / 传统)
- Enables rich, interactive web applications (e.g., Gmail, Google Maps) / 实现丰富的交互式Web应用（如Gmail、Google地图）

**Advantages / 优点**:
- Better user experience (faster, more responsive) / 更好的用户体验（更快、更响应）
- Reduced server load (only send/receive necessary data) / 减少服务器负载（只发送/接收必要数据）
- Bandwidth efficiency / 带宽效率更高

> **中文**：AJAX（异步JavaScript和XML）使网页能与服务器异步通信，仅更新页面部分内容而无需重新加载整个页面。这提供了更流畅的用户体验和更高效的带宽利用。现代AJAX通常使用JSON而非XML进行数据交换。

---

## 6.4 FTP — File Transfer Protocol / 文件传输协议 ★

| Feature / 特性 | Detail / 详情 |
|------|------|
| **Port / 端口** | **21** (control / 控制连接), **20** (data / 数据连接) |
| **Transport / 传输** | TCP |
| **Connection / 连接** | Out-of-band: control and data on separate connections / 带外传输：控制和数据分开 |

**FTP modes / FTP 模式**:
- **Active mode / 主动模式**: Server initiates data connection to client / 服务器主动连接客户端
- **Passive mode / 被动模式**: Client initiates both control and data connections / 客户端发起所有连接

### 6.4.1 FTP Detailed Architecture / FTP 详细架构 【中文】

FTP uses **two separate connections** — this is called **out-of-band** control. / FTP 使用**两个独立的连接**——这称为**带外传输**控制。

```
┌───────────┐   Control Connection / 控制连接    ┌───────────┐
│           │ ────────────────────────────────→ │           │
│   FTP     │      Port 21 (TCP)                │   FTP     │
│  Client   │                                   │  Server   │
│           │   Data Connection / 数据连接       │           │
│           │ ←──────────────────────────────── │           │
│           │      Port 20 (TCP)                │           │
└───────────┘                                   └───────────┘
```

**Two-connection model details / 双连接模型详解**:

| Connection / 连接 | Port / 端口 | Protocol / 协议 | Purpose / 用途 |
|:---:|:---:|:---:|------|
| **Control / 控制连接** | **21** | TCP | Send commands (USER, PASS, LIST, RETR) and receive responses / 发送命令和接收响应 |
| **Data / 数据连接** | **20** | TCP | Transfer actual file data / 传输实际文件数据 |

**FTP Interaction Flow / FTP 交互流程**:
1. **Control connection setup**: Client connects to server on port **21** (TCP three-way handshake) / 客户端通过端口21与服务器建立TCP控制连接
2. **Authentication**: Client sends USER + PASS commands / 客户端发送用户名和密码进行身份验证
3. **Command phase**: Client sends commands via control connection (e.g., LIST, RETR filename, STOR filename) / 客户端通过控制连接发送命令
4. **Data transfer**: Server opens data connection on port **20** and transfers file / 服务器在端口20打开数据连接传输文件
5. **Connection closure**: After transfer, data connection closes; control connection **remains open** for more commands / 传输完成后数据连接关闭，控制连接保持打开以发送更多命令
6. **Session end**: Client sends QUIT to close the control connection / 客户端发送QUIT关闭控制连接

**FTP Commands / FTP 常用命令**:
| Command / 命令 | Description / 描述 |
|:---:|------|
| **USER** | Send username / 发送用户名 |
| **PASS** | Send password / 发送密码 |
| **LIST** | List files in directory / 列出目录中的文件 |
| **RETR** | Download a file (retrieve) / 下载文件 |
| **STOR** | Upload a file (store) / 上传文件 |
| **QUIT** | Terminate connection / 终止连接 |

> **中文**：FTP使用双连接模型——控制连接（端口21，TCP）用于发送命令和接收响应，数据连接（端口20，TCP）用于传输实际文件数据。控制连接在整个会话期间保持打开，而数据连接在每次文件传输时动态建立，传输完毕后关闭。

---

## 6.5 DHCP — Dynamic Host Configuration Protocol / 动态主机配置协议 ★

DHCP automatically assigns IP addresses to hosts. / DHCP 自动给主机分配 IP 地址。

**Port / 端口**: **67** (server), **68** (client), both use UDP

**DHCP 4-step process (DORA) / DHCP 四步过程**:

| Step / 步骤 | Message / 消息 | Direction / 方向 | Purpose / 用途 |
|:---:|------|:---:|------|
| 1 | **D**iscover / 发现 | Client → Broadcast | "I need an IP!" / "我需要IP地址！" |
| 2 | **O**ffer / 提供 | Server → Client | "You can use this IP" / "你可以用这个IP" |
| 3 | **R**equest / 请求 | Client → Broadcast | "I want this IP" / "我要这个IP" |
| 4 | **A**cknowledge / 确认 | Server → Client | "OK, it's yours" / "好的，给你" |

**DHCP provides / DHCP 提供**:
- IP address / IP 地址
- Subnet mask / 子网掩码
- Default gateway / 默认网关
- DNS server address / DNS 服务器地址
- Lease time / 租约时间

---

## 6.6 Other Application Layer Protocols / 其他应用层协议

### 6.6.1 Telnet / 远程登录协议

- **Port / 端口**: 23 (TCP)
- Provides remote terminal access / 提供远程终端访问
- **No encryption** / 无加密 → SSH has largely replaced it / SSH 基本取代了它

### 6.6.2 SSH (Secure Shell) / 安全外壳协议

- **Port / 端口**: 22 (TCP)
- Encrypted remote login / 加密的远程登录
- Replaces Telnet / 取代 Telnet

### 6.6.3 SNMP (Simple Network Management Protocol) / 简单网络管理协议

- **Port / 端口**: 161 (UDP)
- Monitor and manage network devices / 监控和管理网络设备

### 6.6.4 P2P Applications / P2P 应用

- No central server / 无中心服务器
- Each peer is both client and server / 每个节点既是客户端也是服务器
- Example / 例子: BitTorrent, blockchain / 区块链

### 6.6.5 TFTP (Trivial File Transfer Protocol) / 简单文件传输协议 【中文】

**TFTP** (Trivial File Transfer Protocol / 简单文件传输协议) is a simplified version of FTP. / TFTP 是 FTP 的简化版本。

| Feature / 特性 | Detail / 详情 |
|:---:|------|
| **Port / 端口** | **69** (UDP) |
| **Transport / 传输层** | **UDP** (unlike FTP which uses TCP) / UDP（与使用TCP的FTP不同）|
| **Authentication / 认证** | **No authentication** (no user/password) / **无认证**（无用户名/密码）|
| **Complexity / 复杂度** | Very simple / 非常简单 |

**Key characteristics / 关键特性**:
- Uses UDP port 69 / 使用 UDP 端口 69
- No authentication mechanism — anyone can read/write files / 无认证机制——任何人都可以读写文件
- Supports only basic file read (RRQ) and write (WRQ) operations / 仅支持基本的文件读取（RRQ）和写入（WRQ）操作
- Uses **lock-step** protocol (stop-and-wait): sends one block at a time, waits for ACK / 使用**停等式**协议：一次发送一个数据块，等待确认
- Each data block is 512 bytes (last block < 512 signals end of file) / 每个数据块512字节（最后一块<512字节标示文件结束）

**Common use case / 常见用途**:
- Booting **diskless workstations** over a network (PXE boot) / 用于无盘工作站的网络启动（PXE启动）
- Loading firmware onto network devices (routers, switches) / 为网络设备加载固件
- Simple file transfers in embedded systems / 嵌入式系统中的简单文件传输

**Comparison: FTP vs TFTP / FTP 与 TFTP 对比**:
| Feature / 特性 | FTP | TFTP |
|:---:|:---:|:---:|
| Transport / 传输层 | TCP | UDP |
| Port / 端口 | 21, 20 | 69 |
| Authentication / 认证 | Yes (user/pass) / 是 | No / 否 |
| Directory listing / 目录列表 | Yes (LIST) / 是 | No / 否 |
| Reliability / 可靠性 | Built-in (TCP) / 内置 | Application-level ACK / 应用层确认 |

> **中文**：TFTP是FTP的简化版本，使用UDP端口69，无认证机制，仅支持基本的文件读写操作。它采用停等式协议，常用于无盘工作站的网络启动（PXE启动）和网络设备的固件加载。

---

## 6.7 Application Layer Protocol Summary / 应用层协议汇总

| Protocol / 协议 | Port / 端口 | Transport / 传输层 | Key Function / 核心功能 |
|:---:|:---:|:---:|------|
| **HTTP** | 80 | TCP | Web browsing / 网页浏览 |
| **HTTPS** | 443 | TCP | Secure web / 安全网页 |
| **FTP** | 21, 20 | TCP | File transfer / 文件传输 |
| **SMTP** | 25 | TCP | Send email / 发送邮件 |
| **POP3** | 110 | TCP | Receive email (download) / 接收邮件（下载）|
| **IMAP** | 143 | TCP | Receive email (server-managed) / 接收邮件（服务器管理）|
| **DNS** | 53 | **UDP** (also TCP) | Name resolution / 域名解析 |
| **DHCP** | 67/68 | UDP | Dynamic IP assignment / 动态IP分配 |
| **Telnet** | 23 | TCP | Remote login (unencrypted) / 远程登录（无加密）|
| **SSH** | 22 | TCP | Secure remote login / 安全远程登录 |
| **SNMP** | 161 | UDP | Network management / 网络管理 |

---

## 6.8 Concurrent vs Iterative Server / 并发服务器与迭代服务器

An important architectural distinction in application layer servers: / 应用层服务器的一个重要架构区别：

### 6.8.1 Iterative Server / 迭代服务器

Processes **one request at a time**. Subsequent requests must wait until the current one finishes. / **一次只处理一个请求**。后续请求必须等待当前请求完成。

```
Client 1 ──→ [Server] ←── Client 2 (waits)
                │
         (processing Client 1)
                │
         (then processing Client 2)
```

- Simple to implement / 实现简单
- Suitable for services where each request completes quickly / 适用于每个请求能快速完成的服务
- Example: traditional DNS server (single-threaded) / 示例：传统DNS服务器（单线程）

### 6.8.2 Concurrent Server / 并发服务器

Handles **multiple requests simultaneously**. Creates a new **process** or **thread** for each client. / **同时处理多个请求**。为每个客户端创建新的**进程**或**线程**。

```
Client 1 ──→ [Server] ←── Client 2
                │              │
        Process/Thread 1  Process/Thread 2
```

- More complex to implement / 实现更复杂
- Necessary for services with long-lived connections / 对于长连接服务是必要的
- Most web servers (Apache, Nginx) use concurrent models / 大多数Web服务器（Apache、Nginx）使用并发模型

### 6.8.3 Comparison / 对比

| Feature / 特性 | Iterative / 迭代 | Concurrent / 并发 |
|:---:|:---:|:---:|
| Requests / 请求处理 | One at a time / 一次一个 | Multiple simultaneously / 同时多个 |
| Complexity / 复杂度 | Low / 低 | High / 高 |
| Resource usage / 资源使用 | Low / 低 | Higher / 较高 |
| Use case / 适用场景 | Short tasks (e.g., DNS) / 短任务 | Long connections (e.g., HTTP, FTP) / 长连接 |

> **中文**：迭代服务器一次只处理一个请求，实现简单但效率低；并发服务器同时处理多个请求（每个客户端一个进程/线程），实现复杂但效率高。大多数Web服务器（如Apache、Nginx）采用并发模型。

---

## 6.9 Exam Practice / 练习题

### Fill-in-the-Blank / 填空题

1. DNS primarily uses ______ (TCP/UDP) on port ______. **(UDP, 53)**
   > **中文**：DNS主要使用______（TCP/UDP）端口______。**（UDP, 53）**
2. HTTP uses port ______; HTTPS uses port ______. **(80, 443)**
   > **中文**：HTTP使用端口______；HTTPS使用端口______。**（80, 443）**
3. SMTP is a ______ (push/pull) protocol used for ______ (sending/receiving) email. **(push, sending)**
   > **中文**：SMTP是一种______（推送/拉取）协议，用于______（发送/接收）电子邮件。**（推送, 发送）**
4. DHCP uses ports ______ (server) and ______ (client). **(67, 68)**
   > **中文**：DHCP使用端口______（服务器）和______（客户端）。**（67, 68）**
5. The three key components of an email system are: ______, ______, and ______. **(User Agent, Mail Server, Protocols)**
   > **中文**：电子邮件系统的三个关键组件是：______、______和______。**（用户代理, 邮件服务器, 协议）**
6. DNS translates ______ names into ______ addresses. **(domain, IP)**
   > **中文**：DNS将______名称转换为______地址。**（域名, IP）**
7. In HTTP, a **200** status code means ______; a **404** means ______. **(OK/Success, Not Found)**
   > **中文**：HTTP中，**200**状态码表示______；**404**表示______。**（成功, 未找到）**
8. FTP uses port ______ for control and port ______ for data transfer. **(21, 20)**
   > **中文**：FTP使用端口______进行控制，端口______进行数据传输。**（21, 20）**
9. HTTP is a ______ (stateful/stateless) protocol. **(stateless)**
   > **中文**：HTTP是一种______（有状态/无状态）协议。**（无状态）**
10. The DNS record type for mapping domain to IPv4 is ______; for mail servers it's ______. **(A, MX)**
    > **中文**：DNS中用于将域名映射到IPv4的记录类型是______；用于邮件服务器的是______。**（A, MX）**

### True/False / 判断对错

1. (T/F) DNS always uses TCP for reliable name resolution. **(F — primarily uses UDP; TCP for zone transfers)**
   > **中文**：DNS总是使用TCP进行可靠的名称解析。**（错——DNS主要使用UDP；TCP仅用于区域传输）**
2. (T/F) HTTP is a stateful protocol. **(F — HTTP is stateless; cookies are used to simulate state)**
   > **中文**：HTTP是一种有状态协议。**（错——HTTP是无状态的；Cookie用于模拟状态）**
3. (T/F) An AAAA DNS record maps a domain to an IPv4 address. **(F — AAAA maps to IPv6; A maps to IPv4)**
   > **中文**：AAAA DNS记录将域名映射到IPv4地址。**（错——AAAA映射到IPv6；A映射到IPv4）**
4. (T/F) SMTP is used to retrieve email from a mail server to a user agent. **(F — SMTP is for sending; POP3/IMAP are for retrieval)**
   > **中文**：SMTP用于从邮件服务器检索电子邮件到用户代理。**（错——SMTP用于发送；POP3/IMAP用于检索）**
5. (T/F) Telnet uses encryption for secure communication. **(F — Telnet sends data in plaintext; SSH provides encryption)**
   > **中文**：Telnet使用加密进行安全通信。**（错——Telnet以明文发送数据；SSH提供加密）**
6. (T/F) DHCP can provide the DNS server address to clients. **(T — DHCP can provide DNS, gateway, subnet mask, and IP)**
   > **中文**：DHCP可以向客户端提供DNS服务器地址。**（对——DHCP可以提供DNS、网关、子网掩码和IP）**

### Multiple Choice / 单选题

1. Which protocol is used to SEND email?
   a) POP3 b) IMAP c) SMTP d) HTTP **(c)**
   > **中文**：哪个协议用于发送电子邮件？a) POP3 b) IMAP c) SMTP d) HTTP **（c）**
2. DNS uses which transport protocol by default?
   a) TCP b) UDP c) Both d) Neither **(b)**
   > **中文**：DNS默认使用哪种传输协议？a) TCP b) UDP c) 两者皆可 d) 都不是 **（b）**
3. Which HTTP status code indicates "Not Found"?
   a) 200 b) 301 c) 404 d) 500 **(c)**
   > **中文**：哪个HTTP状态码表示"未找到"？a) 200 b) 301 c) 404 d) 500 **（c）**
4. DHCP's DORA process includes: Discover, Offer, Request, and:
   a) Accept b) Acknowledge c) Answer d) Allow **(b)**
   > **中文**：DHCP的DORA过程包括：发现、提供、请求和：a) 接受 b) 确认 c) 回答 d) 允许 **（b）**
5. Which protocol allows users to manage emails on the server without downloading?
   a) POP3 b) SMTP c) IMAP d) FTP **(c)**
   > **中文**：哪个协议允许用户在服务器上管理电子邮件而无需下载？a) POP3 b) SMTP c) IMAP d) FTP **（c）**
6. FTP's control connection uses port:
   a) 20 b) 21 c) 22 d) 80 **(b)**
   > **中文**：FTP的控制连接使用哪个端口？a) 20 b) 21 c) 22 d) 80 **（b）**
