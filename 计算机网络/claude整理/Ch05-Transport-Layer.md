# Chapter 05: Transport Layer / 第五章：传输层

> **Exam Relevance**: TCP's three-way handshake, four-way connection release, and congestion control are guaranteed big question topics. Know the sequence number changes in each handshake step, the congestion window evolution graph, and the difference between slow start and congestion avoidance. UDP vs TCP comparison is a classic fill-in/choice question.
>
> **考试重点**：TCP的三次握手、四次挥手和拥塞控制是必考大题。必须掌握每一步的序号变化、拥塞窗口演化图以及慢开始与拥塞避免的区别。UDP与TCP的对比是经典的填空/选择题。

---

## 5.1 Transport Layer Services / 传输层服务

### 5.1.1 Role of the Transport Layer / 传输层的作用

The transport layer provides **process-to-process** (end-to-end) communication. It sits between the application layer and the network layer.

> **中文**：传输层提供**进程到进程**（端到端）通信，位于应用层和网络层之间。

**Key insight**: The network layer delivers packets between **hosts** (IP addresses). The transport layer delivers data between **processes** (port numbers).

> **中文**：**关键理解**：网络层在**主机**之间传递数据包（通过IP地址），而传输层在**进程**之间传递数据（通过端口号）。

### 5.1.2 Transport vs Network Layer / 传输层与网络层的对比

| 特性 | Network Layer / 网络层 | Transport Layer / 传输层 |
|-----|:---:|:---:|
| Communication unit / 通信单位 | Host ↔ Host / 主机到主机 | Process ↔ Process / 进程到进程 |
| Address / 地址 | IP address (32-bit) / IP地址（32位） | Port number (16-bit) / 端口号（16位） |
| Provides to / 为谁提供服务 | Transport layer / 传输层 | Application layer / 应用层 |
| Reliability / 可靠性 | Best-effort (unreliable) / 尽力而为（不可靠） | Can provide reliability (TCP) / 可提供可靠服务（TCP） |
| End-to-end? / 端到端？ | No (processed at routers) / 否（路由器处理） | **Yes** (only at endpoints) / **是**（仅在端点） |

### 5.1.3 Berkeley Sockets (Socket API Primitives) / 伯克利套接字（套接字API原语）

| Primitive / 原语 | Meaning / 含义 | Used By / 使用者 |
|------------------|---------|---------|
| **SOCKET** | Create a new communication endpoint / 创建新的通信端点 | Both / 双方 |
| **BIND** | Attach a local port number to a socket / 将本地端口号绑定到套接字 | Server / 服务器 |
| **LISTEN** | Announce willingness to accept connections / 宣告愿意接受连接 | Server / 服务器 |
| **ACCEPT** | Block until a client connects; create new socket for this connection / 阻塞直到客户端连接；为此连接创建新套接字 | Server / 服务器 |
| **CONNECT** | Actively attempt to establish a connection / 主动尝试建立连接 | Client / 客户端 |
| **SEND** | Send data over the connection / 通过连接发送数据 | Both / 双方 |
| **RECEIVE** | Receive data from the connection / 从连接接收数据 | Both / 双方 |
| **CLOSE** | Release the connection / 释放连接 | Both / 双方 |

**State diagram for TCP connections**: CLOSED → LISTEN → SYN_RCVD → ESTABLISHED → FIN_WAIT → TIME_WAIT → CLOSED (and similar for client side).

> **中文**：**TCP连接状态图**：CLOSED → LISTEN → SYN_RCVD → ESTABLISHED → FIN_WAIT → TIME_WAIT → CLOSED（客户端侧有类似的状态转换）。

### 5.1.4 Port Numbers / 端口号

- 16-bit numbers (0–65535) / 16位数字（0–65535）
- Ports **0–1023**: Well-known ports (reserved for standard services) / **0–1023**：知名端口（为标准服务保留）
- Ports **1024–49151**: Registered ports / **1024–49151**：注册端口
- Ports **49152–65535**: Dynamic/private ports / **49152–65535**：动态/私有端口

| Service / 服务 | Port / 端口 | Protocol / 协议 |
|----------------|:---:|----------|
| HTTP | 80 | TCP |
| HTTPS | 443 | TCP |
| FTP (control / 控制) | 21 | TCP |
| FTP (data / 数据) | 20 | TCP |
| SMTP | 25 | TCP |
| DNS | 53 | UDP (also TCP / 也可用TCP) |
| Telnet | 23 | TCP |
| SSH | 22 | TCP |
| DHCP (server / 服务器) | 67 | UDP |
| DHCP (client / 客户端) | 68 | UDP |
| RIP | 520 | UDP |

---

## 5.2 UDP (User Datagram Protocol) / 用户数据报协议

### 5.2.1 Characteristics / 特点

- **Connectionless** / **无连接**：No setup before sending / 发送前无需建立连接
- **Unreliable** / **不可靠**：No acknowledgment, no retransmission / 无确认，无重传
- **No flow control** / **无流量控制**：Sender can overwhelm receiver / 发送方可能淹没接收方
- **No congestion control** / **无拥塞控制**：Sender can overwhelm the network / 发送方可能淹没网络
- **Preserves message boundaries** / **保留消息边界**：Each UDP send = one UDP datagram received / 每次UDP发送对应一个UDP数据报接收
- **Low overhead** / **低开销**：8-byte header / 8字节首部

### 5.2.2 UDP Header (8 bytes) / UDP首部（8字节）

```
 0      16      31
+-------+-------+
| Src Port (16) | Dst Port (16) |   源端口(16位) / 目的端口(16位)
+-------+-------+
| Length (16)   | Checksum (16) |   长度(16位) / 校验和(16位)
+-------+-------+
|       Data (variable)         |   数据（可变长度）
+-------------------------------+
```

> **中文**：UDP首部格式：包含源端口（16位）、目的端口（16位）、长度（16位）和校验和（16位），共8字节。

- **Source Port**【源端口】(16 bits): Sender's port (optional, can be 0) / 发送方端口（可选，可为0）
- **Destination Port**【目的端口】(16 bits): Receiver's port / 接收方端口
- **Length**【长度】(16 bits): Total UDP datagram length (header + data), in bytes / UDP数据报总长度（首部+数据），以字节为单位
- **Checksum**【校验和】(16 bits): Optional error check / 可选错误检查

### 5.2.3 UDP Pseudo-Header (for Checksum Calculation) / UDP伪首部（用于校验和计算）

The UDP checksum is computed over a **pseudo-header**【伪首部】+ UDP header + data. The pseudo-header is **NOT transmitted** — it's only used for the checksum calculation.

> **中文**：UDP校验和的计算范围包括**伪首部** + UDP首部 + 数据。伪首部**不实际传输**——仅用于校验和计算。

| Field / 字段 | Size / 大小 | Content / 内容 |
|-------|:----:|---------|
| Source IP Address / 源IP地址 | 32 bits | From IP header / 来自IP首部 |
| Destination IP Address / 目的IP地址 | 32 bits | From IP header / 来自IP首部 |
| Zero / 零 | 8 bits | All zeros / 全零 |
| Protocol / 协议 | 8 bits | 17 (UDP protocol number) / UDP协议号 |
| UDP Length / UDP长度 | 16 bits | Same as UDP header length field / 与UDP首部长度字段相同 |

**Why include IP addresses?** / **为什么要包含IP地址？** To detect misdelivery — if a packet is delivered to the wrong IP address, the checksum will fail.

> **中文**：为了检测误投递——如果一个数据包被错误地投递到错误的IP地址，校验和会失败。

### 5.2.4 When to Use UDP / 何时使用UDP

- **DNS queries**【DNS查询】(fast, no connection overhead / 快速，无需建立连接的开销)
- **Streaming media**【流媒体】(some packet loss acceptable, real-time matters more / 允许一定丢包，实时性更重要)
- **VoIP**【网络电话】(low latency critical / 低延迟至关重要)
- **Online gaming**【在线游戏】(real-time state updates / 实时状态更新)
- **Multicast**【多播】applications
- Applications where the app handles its own reliability / 应用程序自行处理可靠性的场景

---

## 5.3 TCP (Transmission Control Protocol) ★★★ / 传输控制协议

### 5.3.1 TCP Service Model / TCP服务模型

| Feature / 特性 | Detail / 详情 |
|---------|--------|
| Connection type / 连接类型 | **Connection-oriented**【面向连接】, full-duplex【全双工】 |
| Reliability / 可靠性 | Reliable byte stream (no message boundaries) / 可靠字节流（无消息边界） |
| Data flow / 数据流 | Byte stream — no record markers preserved / 字节流——不保留记录标记 |
| Flow control / 流量控制 | Sliding window (receiver-driven) / 滑动窗口（接收方驱动） |
| Congestion control / 拥塞控制 | Multiple algorithms (slow start, congestion avoidance, fast recovery) / 多种算法（慢开始、拥塞避免、快恢复） |
| Sequencing / 序号 | Each byte is numbered / 每个字节都被编号 |

### 5.3.2 TCP Segment Header (20-60 bytes) / TCP段首部（20-60字节）

```
 0          4      10    16                    31
+----------+--------+------+-------------------+
| Src Port (16)     |  Dst Port (16)           |  源端口(16) / 目的端口(16)
+----------+--------+------+-------------------+
| Sequence Number (32)                         |  序号(32)
+----------+--------+------+-------------------+
| Acknowledgment Number (32)                   |  确认号(32)
+----------+--------+------+-------------------+
|HdrLen|Reserved|Flags|   Window Size (16)     |  首部长度/保留/标志/窗口大小(16)
+----------+--------+------+-------------------+
| Checksum (16)     |   Urgent Pointer (16)     |  校验和(16) / 紧急指针(16)
+----------+--------+------+-------------------+
| Options (variable, if any)                    |  选项（可变）
+----------+--------+------+-------------------+
```

**Key fields (Must know for exam)** / **关键字段（考试必知）**：

| Field / 字段 | Bits / 位数 | Purpose / 用途 |
|-------|:----:|---------|
| **Source Port**【源端口】 | 16 | Sender's application port / 发送方应用端口 |
| **Destination Port**【目的端口】 | 16 | Receiver's application port / 接收方应用端口 |
| **Sequence Number**【序号】 | 32 | Byte number of first data byte in this segment / 本段中第一个数据字节的编号 |
| **Acknowledgment Number**【确认号】 | 32 | Next byte number the sender of this segment expects to receive (valid only if ACK flag set) / 本段发送方期望接收的下一个字节号（仅当ACK标志置位时有效） |
| **Header Length**【首部长度】 | 4 | TCP header length in 32-bit words (minimum 5 = 20 bytes) / TCP首部长度，以32位字为单位（最小为5 = 20字节） |
| **Flags**【标志位】 | 6+2 | URG, **ACK**, PSH, RST, **SYN**, **FIN** |
| **Window Size**【窗口大小】 | 16 | Receiver's available buffer space (flow control) / 接收方可用的缓冲区空间（流量控制） |
| **Checksum**【校验和】 | 16 | Error detection (includes pseudo-header like UDP) / 错误检测（与UDP类似包含伪首部） |
| **Urgent Pointer**【紧急指针】 | 16 | Offset to urgent data (valid only if URG=1) / 紧急数据的偏移量（仅当URG=1时有效） |

**The 6 Flags** / **6个标志位**：
- **URG**: Urgent pointer is valid / 紧急指针有效
- **ACK**: Acknowledgment number is valid (set in all segments except the very first SYN) / 确认号有效（除第一个SYN段外，所有段都设置此标志）
- **PSH**: Push — deliver data to application immediately / 推送——立即将数据交付给应用
- **RST**: Reset the connection / 重置连接
- **SYN**: Synchronize sequence numbers (used to establish connection) / 同步序号（用于建立连接）
- **FIN**: Sender has finished sending data (used to release connection) / 发送方已完成数据发送（用于释放连接）

### 5.3.3 TCP Sequence Numbers and ACKs / TCP序号与确认

- **Sequence number**【序号】：Identifies the **first data byte** in this segment (not the segment number!) / 标识本段中**第一个数据字节**（不是段编号！）
- **Acknowledgment number**【确认号】：The **next** sequence number the sender expects to receive (cumulative ACK) / 发送方期望接收的**下一个**序号（累积确认）

**Example** / **示例**：Host A sends 1000 bytes starting at seq=100 / 主机A发送1000字节，起始序号seq=100。
- A sends: seq=100, data 1000 bytes / A发送：seq=100，数据1000字节
- B replies: ACK=1100 (meaning: I've received bytes through 1099, expect 1100 next) / B回复：ACK=1100（表示：已收到1099及之前的所有字节，期望下一个收到1100）

**Cumulative ACK**【累积确认】：ACK=$N$ means "I have received ALL bytes up to byte $N-1$." / ACK=$N$ 表示"已收到字节 $N-1$ 及之前的所有字节。"
> **中文**：TCP使用累积确认机制，这意味着确认号告诉发送方该号之前的所有字节都已经被正确接收。

### 5.3.4 Three-Way Handshake (Connection Establishment) ★★★ / 三次握手（建立连接）

**The fundamental process** / **基本过程**：

```
Host A (Client / 客户端)               Host B (Server / 服务器)
CLOSED                                    LISTEN
                                               ↓
SYN_SENT    ──SYN, seq=x────────────→    SYN_RCVD
                                               ↓
ESTABLISHED ←──SYN+ACK, seq=y, ack=x+1── SYN_RCVD
                                               ↓
ESTABLISHED ──ACK, seq=x+1, ack=y+1──→   ESTABLISHED
```

**What happens in each step** / **每一步发生了什么**：

**Step 1**【第一步】：Client → Server / 客户端 → 服务器端：SYN (seq=x)
- Client picks an initial sequence number $x$ / 客户端选择一个初始序号 $x$
- Sets SYN flag, no ACK flag (so ACK field is meaningless) / 设置SYN标志，不设置ACK标志（因此确认号字段无意义）
- Client enters SYN_SENT state / 客户端进入 SYN_SENT 状态
- > **中文**：第一次握手：客户端向服务器发送SYN段，初始序号为x。此时客户端进入SYN_SENT状态，服务器收到后进入SYN_RCVD状态。

**Step 2**【第二步】：Server → Client / 服务器端 → 客户端：SYN+ACK (seq=y, ack=x+1)
- Server picks its own initial sequence number $y$ / 服务器端选择自己的初始序号 $y$
- Sets SYN and ACK flags / 设置SYN和ACK标志
- ACK=x+1 acknowledges the client's SYN / ACK=x+1 确认客户端的SYN
- Server enters SYN_RCVD state / 服务器端进入 SYN_RCVD 状态
- > **中文**：第二次握手：服务器向客户端发送SYN+ACK段，服务器的初始序号为y，确认号为x+1（表示已收到客户端的SYN）。服务器仍处于SYN_RCVD状态。

**Step 3**【第三步】：Client → Server / 客户端 → 服务器端：ACK (seq=x+1, ack=y+1)
- Client acknowledges server's SYN with ack=y+1 / 客户端以ack=y+1确认服务器的SYN
- ACK flag is set / ACK标志置位
- Both sides enter ESTABLISHED state / 双方进入 ESTABLISHED 状态
- > **中文**：第三次握手：客户端向服务器发送ACK段，序号为x+1，确认号为y+1。至此，双方进入ESTABLISHED状态，连接建立完成。

**Why 3-way instead of 2-way?** / **为什么是3次而不是2次握手？** To prevent old duplicate connection requests from causing confusion. If a stale SYN arrives at the server, and the server immediately establishes the connection (2-way), resources would be wasted. With 3-way, the server only allocates resources after the client confirms it actually wants the connection (via the third ACK).

> **中文**：为了防止旧的重复连接请求造成混淆。如果过期的SYN到达服务器，而服务器立即建立连接（2次握手），则会浪费资源。使用3次握手，服务器只在客户端确认其确实想要建立连接（通过第三次ACK）后才分配资源。

**Why 3-way is the minimum for a single unidirectional connection** / **为什么3次是单向连接的最小次数**：
- For the sender / 对于发送方：needs to know the **receiver knows it wants to establish** (step 1→2→3) / 需要知道**接收方知道它想建立连接**（步骤1→2→3）
- For the receiver / 对于接收方：needs to know the **sender knows the receiver agrees** (step 2→3) / 需要知道**发送方知道接收方同意**（步骤2→3）
- Two directions of certainty require at least 3 messages / 双向的确定性至少需要3条消息
- > **中文**：简而言之：客户端需要确认服务器收到了连接请求，服务器需要确认客户端收到了确认。这两件事合起来最少需要3条消息。

### 5.3.5 Four-Way Handshake (Connection Release) ★★★ / 四次挥手（释放连接）

**The symmetric release process** / **对称释放过程**：

```
Host A / 主机A                           Host B / 主机B
ESTABLISHED                               ESTABLISHED
                                                ↓
FIN_WAIT_1 ──FIN, seq=u────────────→     CLOSE_WAIT
                                                ↓
FIN_WAIT_2 ←──ACK, seq=v, ack=u+1────    CLOSE_WAIT
                                                ↓ (app closes / 应用关闭)
           ←──FIN, seq=w, ack=u+1────    LAST_ACK
                                                ↓
TIME_WAIT  ──ACK, seq=u+1, ack=w+1──→    CLOSED
   ↓ (wait 2MSL / 等待2倍最大段生存时间)
CLOSED
```

**Step-by-step** / **逐步说明**：

1. **Client → Server / 客户端 → 服务器端**: **FIN (seq=u)** — "I'm done sending data" / "我已完成数据发送"
   - Client enters FIN_WAIT_1 / 客户端进入 FIN_WAIT_1 状态
   - > **中文**：第一次挥手：客户端发送FIN段，序号为u，表示客户端不再发送数据。客户端进入FIN_WAIT_1状态。

2. **Server → Client / 服务器端 → 客户端**: **ACK (ack=u+1)** — "I acknowledge your FIN" / "我确认你的FIN"
   - Server enters CLOSE_WAIT (can still send remaining data) / 服务器进入 CLOSE_WAIT 状态（仍可发送剩余数据）
   - Client enters FIN_WAIT_2 / 客户端进入 FIN_WAIT_2 状态
   - > **中文**：第二次挥手：服务器发送ACK段，确认号为u+1。服务器进入CLOSE_WAIT状态（此时服务器仍可向客户端发送未传完的数据），客户端收到后进入FIN_WAIT_2状态。

3. **Server → Client / 服务器端 → 客户端**: **FIN (seq=w, ack=u+1)** — "I'm also done sending data" / "我也已完成数据发送"
   - Server enters LAST_ACK / 服务器进入 LAST_ACK 状态
   - Note: ack=u+1 is the same as before (no new data from client) / 注意：ack=u+1 与之前相同（客户端没有新的数据）
   - > **中文**：第三次挥手：服务器发送FIN段，序号为w（因为服务器可能在此期间发送了更多数据），确认号仍为u+1。服务器进入LAST_ACK状态。

4. **Client → Server / 客户端 → 服务器端**: **ACK (ack=w+1)** — "I acknowledge your FIN" / "我确认你的FIN"
   - Server enters CLOSED / 服务器进入 CLOSED 状态
   - Client enters **TIME_WAIT** (waits 2 × MSL before closing) / 客户端进入 **TIME_WAIT** 状态（等待 2 × MSL 后才关闭）
   - > **中文**：第四次挥手：客户端发送ACK段，确认号为w+1。服务器收到后立即进入CLOSED状态，而客户端进入TIME_WAIT状态，等待2倍最大段生存时间（2MSL）后才完全关闭。

**Why 4-way instead of 3-way?** / **为什么是4次而不是3次挥手？** TCP connection release is **symmetric** — each direction is closed independently. The server's ACK (step 2) and FIN (step 3) are sent separately because:
> **中文**：TCP连接释放是**对称的**——每个方向的关闭是独立的。服务器的ACK（第二步）和FIN（第三步）分开发送，原因是：

- After receiving the client's FIN, the server may still have data to send / 收到客户端的FIN后，服务器可能还有数据要发送
- The server sends its FIN only when it has finished sending all pending data / 服务器只有在完成所有待发送数据后才发送FIN
- TCP requires this separation: "you stop your direction" and "I stop my direction" are independent decisions / TCP要求这种分离："你停止你的方向"和"我停止我的方向"是独立的决定

> **中文**：**关键理解**：与三次握手不同，四次挥手不能合并为三次，因为服务器的ACK和FIN之间可能有数据传输。ACK只是确认收到了关闭请求，而FIN是在服务器自己也准备好关闭时才发送。这体现了TCP全双工通信的特性。

**TIME_WAIT state**【TIME_WAIT状态】：After sending the final ACK, the client waits for **2 × MSL** (Maximum Segment Lifetime【最大段生存时间】, typically 2 minutes) before fully closing. This ensures:
> **中文**：发送最终ACK后，客户端等待**2 × MSL**（通常为2分钟）才完全关闭。这确保了：

1. If the final ACK is lost, the server will retransmit its FIN, and the client can re-ACK / 如果最终的ACK丢失，服务器会重传其FIN，客户端可以重新ACK
2. All stray packets from this connection die off before the same port pair is reused / 在相同的端口对被重用之前，来自此连接的所有迷路数据包都会失效

> **中文**：**为什么是2MSL？** MSL是数据包在网络中的最大生存时间。2MSL确保了在发送最后一个ACK后，足够让所有可能"迷路"的数据包在网络中消失（1个MSL用于等待已经出发的数据包到达目的地，另一个MSL用于等待可能的回复到达），从而保证使用相同端口对的新连接不会受到旧连接数据包的干扰。

### 5.3.6 TCP Flow Control / TCP流量控制

**Purpose**: Prevent a fast sender from overwhelming a slow receiver.
> **中文**：**目的**：防止快速发送方压垮慢速接收方。

**Mechanism** / **机制**：Sliding window at the **byte level** / **字节级**滑动窗口。

- Each ACK contains a **Window Size**【窗口大小】field (16 bits): how many bytes the receiver can currently accept / 每个ACK包含一个**窗口大小**字段（16位）：表示接收方当前能接收多少字节
- Sender's effective window = $\min(\text{congestion window【拥塞窗口】, receiver's advertised window【接收方通告窗口】})$ / 发送方的有效窗口 = $\min(\text{拥塞窗口, 接收方通告窗口})$
- If receiver advertises window = 0, sender stops (except for probe segments) / 如果接收方通告窗口=0，发送方停止发送（探测段除外）
- **Silly window syndrome**【傻瓜窗口综合征】：Avoid sending tiny segments when window opens up by a small amount / 避免在窗口打开很小的时候发送微小段

**Key difference from data link layer sliding window** / **与数据链路层滑动窗口的关键区别**：

| 特性 | Data Link Layer / 数据链路层 | Transport Layer (TCP) / 传输层（TCP） |
|-----|:---:|:---:|
| Window unit / 窗口单位 | Frames (fixed size) / 帧（固定大小） | Bytes (variable) / 字节（可变） |
| Window flexibility / 窗口灵活性 | Fixed-size window / 固定大小 | Dynamically changing / 动态变化 |
| What controls window / 什么控制窗口 | Protocol / 协议 | Receiver buffer + congestion / 接收方缓冲区 + 拥塞控制 |

### 5.3.7 Nagle's Algorithm / Nagle算法

Nagle's algorithm is designed to prevent the "tinygram" problem — sending many small segments when the application generates data one byte at a time.

> **中文**：Nagle算法旨在防止"小数据报"问题——当应用程序逐字节生成数据时，发送方可能会发送大量的小段。

**Algorithm Rule** / **算法规则**:
- If there is unacknowledged small data in flight, do NOT send a new small segment — buffer it until:
  - The outstanding data is acknowledged, OR
  - Enough data accumulates to send a full-size segment (MSS)
- If no unacknowledged data exists, send immediately (even if it's just one byte)
- Only one small (less than MSS) unacknowledged segment allowed at a time

> **中文**：**算法规则**：
> - 如果网络中已有未确认的小数据，则不发送新的小段——将其缓冲，直到：
>   - 未确认的数据被确认，或者
>   - 积累的数据足够发送一个完整大小的段（达到MSS）
> - 如果没有未确认的数据，则立即发送（即使只有一个字节）
> - 任何时候最多只允许有一个小于MSS的未确认段

**Why it matters** / **为什么重要**:
- Reduces header overhead: each tiny segment carries a 40+ byte header for just a few bytes of data
- Essential for interactive applications (Telnet, SSH) that send keystroke-by-keystroke
- Example: typing "dir" in Telnet — instead of 3 separate segments (one per character), Nagle's algorithm can batch them

> **中文**：**重要性**：
> - 减少首部开销：每个小段可能只携带几个字节的数据，但首部就占40多字节
> - 对逐键发送的交互式应用（Telnet、SSH）尤为重要
> - 例如：在Telnet中输入"dir"——Nagle算法可以将3个独立的段（每字符一个）合并发送

**Interaction with Delayed ACK** / **与延迟确认的交互**:
When Nagle's algorithm (sender-side) interacts with Delayed ACK (receiver-side), a temporary deadlock can occur: Nagle waits for an ACK before sending the next small segment, but Delayed ACK waits for data before sending the ACK. This can cause visible lag in interactive applications. Disabling Nagle via the **TCP_NODELAY** socket option is often recommended for real-time applications.

> **中文**：**与延迟确认的交互问题**：当发送方的Nagle算法遇到接收方的延迟ACK机制时，可能出现临时死锁：Nagle等待ACK才发送下一小段，但延迟ACK等待有数据才发送ACK。这可能导致交互式应用出现明显的延迟。对于实时应用通常建议通过**TCP_NODELAY**套接字选项禁用Nagle算法。

### 5.3.8 Delayed ACK / 延迟确认

Delayed ACK is a receiver-side optimization that reduces the number of ACK packets sent.

> **中文**：延迟确认是一种接收方优化技术，用于减少发送的ACK数据包数量。

**How it works** / **工作原理**:
- When the receiver receives a data segment, it does NOT immediately send an ACK
- Instead, it waits up to **500ms** (or until one of the following conditions is met):
  1. A second full-size segment arrives → send cumulative ACK immediately
  2. The application reads the data → ACK can be piggybacked on outgoing data
  3. The 500ms timer expires → send standalone ACK

> **中文**：**工作原理**：
> - 当接收方收到数据段时，不会立即发送ACK
> - 而是最多等待**500毫秒**（或满足以下任一条件时立即发送）：
>   1. 第二个完整段到达 → 立即发送累积ACK
>   2. 应用程序读取了数据 → ACK可以捎带在发送数据中
>   3. 500ms定时器超时 → 发送独立ACK

**Typical delay** / **典型延迟**: 200ms (common implementation default, though RFC allows up to 500ms)

> **中文**：**典型延迟**：200ms（常见实现的默认值，RFC允许最长500ms）

**Benefits** / **好处**:
- Reduces ACK processing overhead on both sender and receiver
- Enables ACK piggybacking on reverse-direction data segments
- Reduces overall packet count on the network (especially important for small transfers)

> **中文**：**优点**：
> - 减少发送方和接收方的ACK处理开销
> - 允许ACK捎带在反向数据段中
> - 减少网络中的总数据包数量（对小数据传输尤其重要）

**Downside** / **缺点**:
- Slightly delayed ACKs can slow down slow start growth (since slow start depends on each ACK triggering new segment sends)
- Interaction with Nagle's algorithm can cause performance issues for interactive applications

> **中文**：**缺点**：
> - 延迟ACK可能减慢慢开始的增长速度（因为慢开始阶段每个ACK触发新段发送）
> - 与Nagle算法交互时可能导致交互式应用的性能问题

### 5.3.9 SACK (Selective Acknowledgment) / 选择性确认

SACK is a TCP option that allows the receiver to inform the sender precisely which segments were received, going beyond the standard cumulative ACK.

> **中文**：SACK是一种TCP选项，允许接收方精确告知发送方已收到了哪些数据段，超越了标准累积ACK的能力。

**Problem SACK solves** / **SACK解决的问题**:
When multiple segments are lost within a single window, the standard cumulative ACK only tells the sender where the loss started, but not which later segments arrived successfully. This forces unnecessary retransmissions of already-received data.

> **中文**：当在一个窗口内丢失多个段时，标准累积ACK只能告知发送方丢失的起始位置，却无法告知哪些段已被正确接收。这会导致不必要的重复重传。

**How it works** / **工作原理**:
- Uses the TCP **Options** field in the TCP header
- Receiver includes SACK option blocks listing non-contiguous received data ranges
- Each SACK block describes a contiguous range of received data:
  - Left Edge: sequence number of the first received byte in this block
  - Right Edge: sequence number of the last received byte + 1 in this block
- The sender uses this information to retransmit only the truly missing segments

> **中文**：**工作原理**：
> - 使用TCP首部的**选项**字段
> - 接收方在SACK选项块中列出非连续接收的数据范围
> - 每个SACK块描述一个连续接收的数据区间：
>   - 左边界：该块中第一个已接收字节的序号
>   - 右边界：该块中最后一个已接收字节的序号 + 1
> - 发送方根据这些信息，只重传真正丢失的数据段

**Format** / **格式**:
```
TCP Option Kind = 5 (SACK)
+------+------+------+------+------+------+------+------+
| Kind | Len  |      Left Edge 1       |     Right Edge 1      | ...
|(8bit)|(8bit)|         (32bit)         |        (32bit)        |
+------+------+------+------+------+------+------+------+
```

**Performance benefit** / **性能优势**:
With multiple losses in one window (common in high-bandwidth networks):
- **Without SACK**: retransmit from the first lost byte + everything after it, potentially wasting bandwidth on already-received data
- **With SACK**: retransmit only the specific lost segments
- Significant throughput improvement, especially over high-delay links (e.g., satellite links)

> **中文**：**性能优势**：
> 当一个窗口内发生多次丢包时（在高速网络中很常见）：
> - **无SACK**：从第一个丢失的字节开始重传之后的所有内容，可能浪费带宽在已接收的数据上
> - **使用SACK**：只重传真正丢失的特定段
> - 吞吐量显著提高，特别是在高延迟链路上（如卫星链路）

**Support** / **支持情况**:
Both sender and receiver must negotiate SACK during the TCP handshake (SYN segments include the **SACK-permitted** option). It is now widely supported and enabled by default in all major operating systems.

> **中文**：发送方和接收方必须在TCP握手期间协商启用SACK（SYN段中包含**SACK-permitted**选项）。目前SACK已被广泛支持，并在所有主流操作系统中默认启用。

---

## 5.4 TCP Congestion Control ★★★ (Classic Big Question) / TCP拥塞控制（经典大题）

### 5.4.1 Core Concepts / 核心概念

TCP must determine how much data it can send without congesting the network. It uses a **congestion window (cwnd)** 【拥塞窗口】that limits the number of unacknowledged bytes.
> **中文**：TCP必须确定能够在不会造成网络拥塞的情况下发送多少数据。它使用**拥塞窗口（cwnd）** 来限制未确认的字节数。

$$\text{Effective window【有效窗口】} = \min(\text{cwnd【拥塞窗口】}, \text{rwnd【接收窗口】})$$

Where rwnd = receiver's advertised window【接收方通告窗口】, cwnd = congestion window【拥塞窗口】.

> **中文**：其中 rwnd 是接收方通告的窗口大小（接收方剩余缓冲区大小），cwnd 是拥塞窗口（发送方估计的网络容量）。实际能发送的数据量取两者中的较小值。

### 5.4.2 The Four Algorithms / 四种算法

#### Algorithm 1: Slow Start (慢开始)

- **cwnd starts at 1 MSS**【最大段长度】(Maximum Segment Size, typically 1) / **cwnd从1个MSS开始**
- **Doubles cwnd every RTT**【往返时延】(exponential growth) / **每个RTT内cwnd翻倍**（指数增长）
- Growth continues until / 增长持续直到：
  - cwnd reaches **ssthresh**【慢开始阈值】(slow start threshold) → switch to congestion avoidance / cwnd达到**ssthresh** → 切换到拥塞避免
  - A timeout occurs → ssthresh = cwnd/2, cwnd = 1, restart slow start / 发生超时 → ssthresh = cwnd/2, cwnd = 1, 重新开始慢开始
- > **中文**：**慢开始算法详解**：发送方初始设置cwnd=1个MSS，每当收到一个ACK确认，cwnd就增加1个MSS（相当于每个RTT内cwnd翻倍）。这是一种指数增长方式。当cwnd达到ssthresh时，切换到拥塞避免阶段。如果发生超时，则ssthresh更新为cwnd/2，cwnd重置为1，重新开始慢开始。

**Why "slow"?** / **为什么叫"慢开始"？** Despite exponential growth being fast, the initial window of 1 is "slow" compared to immediately sending a full window. Old TCP implementations used to start with a full receiver window.
> **中文**：虽然指数增长很快，但与一开始就发送整个窗口相比，初始窗口为1确实"慢"。旧的TCP实现曾使用完整的接收窗口大小作为初始窗口。

#### Algorithm 2: Congestion Avoidance (拥塞避免)

Active once cwnd ≥ ssthresh / 当cwnd ≥ ssthresh时激活。

- **Increases cwnd by 1 MSS per RTT** (linear growth / additive increase) / **每个RTT增加1个MSS**（线性增长/加性增加）
- This is the **Additive Increase**【加性增加】part of the **AIMD**【加性增乘性减】(Additive Increase Multiplicative Decrease) principle / 这是**AIMD**原则中的**加性增加**部分
- > **中文**：**拥塞避免算法详解**：当cwnd >= ssthresh时，TCP进入拥塞避免阶段。每个RTT内，cwnd仅增加1个MSS（而非翻倍），即线性增长。具体实现是：每个RTT内，每收到一个ACK，cwnd增加 1/cwnd 个MSS，因此在完整的一个RTT内共增加1个MSS。这是AIMD原则中的"加性增加"部分。

#### Algorithm 3: Fast Retransmit (快重传)

If the sender receives **3 duplicate ACKs**【3个重复确认】for the same sequence number (meaning the receiver got 3 packets after a gap), the sender retransmits the missing segment **immediately** without waiting for a timeout.
> **中文**：如果发送方收到对于同一序号的**3个重复ACK**（意味着接收方在丢包后又收到了3个数据包），发送方**立即**重传丢失的段，而不等待超时。

**Why 3 duplicates?** / **为什么是3个重复确认？** A single duplicate could be from reordering. Two is unlikely but possible. Three almost certainly means a lost packet.
> **中文**：1个重复ACK可能是由于数据包乱序到达。2个重复ACK不太可能但仍有可能是乱序。3个几乎肯定意味着发生了丢包。

#### Algorithm 4: Fast Recovery (快恢复)

After fast retransmit triggers / 快重传触发后：

1. **ssthresh = cwnd / 2** (multiplicative decrease【乘性减少】) / **ssthresh = cwnd / 2**（乘性减少）
2. **cwnd = ssthresh + 3 × MSS** (not setting to 1 — that's what distinguishes "fast" recovery) / **cwnd = ssthresh + 3 × MSS**（不设为1——这就是"快恢复"区别于慢开始的地方）
3. For each subsequent duplicate ACK: cwnd += 1 MSS / 对于后续的每个重复ACK：cwnd增加1个MSS
4. When a new ACK arrives: cwnd = ssthresh, enter congestion avoidance / 当新的ACK到达时：cwnd设为ssthresh，进入拥塞避免

> **中文**：**快恢复算法详解**：与超时后cwnd重置为1（重新慢开始）不同，快恢复在快重传后将cwnd设为ssthresh + 3×MSS，而不是1。这是因为收到3个重复ACK意味着网络仍在传输数据（只是丢失了一个包），网络情况不像超时那么严重，所以不必回到cwnd=1从头开始。收到新的ACK后，cwnd设为ssthresh，进入拥塞避免阶段。

### 5.4.3 Congestion Control Evolution Diagram (The "Sawtooth") / 拥塞控制演化图（"锯齿"形）

```
cwnd ↑
     |    /\
     |   /  \    /\      /\        ←  congestion avoidance (linear) / 拥塞避免（线性）
     |  /    \  /  \    /  \
     | /      \/    \  /    \
     |/              \/      \
 ssthresh ──────────────────────────   慢开始阈值
     |        |  timeout  |  timeout             超时
     |   exponential      ↑                      指数增长
     |   (slow start)     fast retransmit + fast recovery   慢开始 / 快重传 + 快恢复
     |   （慢开始）          快重传 + 快恢复
     |
     └───────────────────────────────────→ time / 时间
```

**Key events on the graph** / **图中的关键事件**：
1. **Slow start**【慢开始】：Exponential (doubling each RTT) / 指数增长（每个RTT翻倍）
2. **Threshold crossed**【达到阈值】：Switch to congestion avoidance (linear, +1 per RTT) / 切换到拥塞避免（线性，每个RTT+1）
3. **3 duplicate ACKs**【3个重复ACK】：Fast retransmit + fast recovery (cwnd halved, not set to 1) / 快重传 + 快恢复（cwnd减半，不设为1）
4. **Timeout**【超时】：cwnd = 1, ssthresh = cwnd/2, restart slow start / cwnd = 1, ssthresh = cwnd/2, 重新慢开始

> **中文**：**"锯齿"形曲线理解**：TCP的拥塞窗口随时间变化呈现出锯齿状。在慢开始阶段指数增长（陡峭上升），遇到ssthresh后进入拥塞避免阶段线性增长（平缓上升）。当发生3个重复ACK时，cwnd减半（锯齿的"齿"落下），然后继续增长。当发生超时时，cwnd回落到1，重新慢开始，形成更大的"锯齿"。这种"加性增、乘性减"（AIMD）的行为是TCP拥塞控制的核心特征。

### 5.4.4 Congestion Window Calculation (Exam Problem Type) / 拥塞窗口计算（考试题型）

**Given**: Initial ssthresh, initial cwnd, a series of events (timeouts, 3-dup ACKs), and segment numbers.
> **中文**：**已知**：初始ssthresh、初始cwnd、一系列事件（超时、3重复ACK）和段号。

**Calculate**: cwnd and ssthresh after each event.
> **中文**：**计算**：每个事件后的cwnd和ssthresh。

**Example / 示例 1**:
- Initial: cwnd = 1, ssthresh = 16 / 初始：cwnd = 1, ssthresh = 16
- Each RTT: double cwnd (slow start) or increment by 1 (congestion avoidance) / 每个RTT：cwnd翻倍（慢开始）或增加1（拥塞避免）
- At transmission #8: timeout occurs (cwnd was 8, ssthresh was 16) / 第8次传输时：发生超时（当时cwnd=8, ssthresh=16）

**After timeout / 超时后**: ssthresh = $\max(8/2, 2) = 4$, cwnd = 1

**RTT by RTT / 逐RTT分析**:
- RTT 1: cwnd = 1 → send 1 / 发送1个段
- RTT 2: cwnd = 2 → send 2 / 发送2个段
- RTT 3: cwnd = 4 → send 4 / 发送4个段
- RTT 4: cwnd = 5 (crossed ssthresh=4, now in congestion avoidance, +1) / 超过ssthresh=4，进入拥塞避免，+1
- RTT 5: cwnd = 6
- ...and so on until next event / 以此类推直到下一个事件

> **中文**：**解题思路**：当发生超时时，首先将ssthresh设为当前cwnd的一半（但至少为2），然后将cwnd重置为1，重新开始慢开始阶段的指数增长。当cwnd增长到新的ssthresh时，切换为拥塞避免的线性增长。

**Another example with fast retransmit / 示例 2（含快重传）**：
- cwnd = 12, ssthresh = 8, then 3 duplicate ACKs arrive / cwnd = 12, ssthresh = 8, 然后收到3个重复ACK

**After 3-dup ACKs / 收到3重复ACK后**: ssthresh = 12/2 = 6, cwnd = 6 + 3 = 9 (fast recovery / 快恢复)
After new ACK / 收到新ACK后: cwnd = ssthresh = 6, enter congestion avoidance / 进入拥塞避免

> **中文**：**快重传/快恢复解题思路**：当收到3个重复ACK时，首先将ssthresh设为当前cwnd的一半（乘性减少），然后将cwnd设为ssthresh+3（快恢复），继续接收重复ACK时cwnd递增，直到收到新的ACK才将cwnd设为ssthresh，进入拥塞避免阶段。

### 5.4.5 TCP Throughput Approximation / TCP吞吐量近似

$$\text{Avg throughput / 平均吞吐量} \approx \frac{0.75 \times W_{max}}{RTT}$$

> **中文**：其中 $W_{max}$ 是拥塞窗口的最大值（以MSS为单位），RTT为往返时延。该公式给出了TCP平均吞吐量的近似值。

Or more commonly / 或者更常用的公式：
$$\text{Avg throughput / 平均吞吐量} \approx \frac{1.22 \times MSS}{RTT \times \sqrt{p}}$$

Where $p$ is the packet loss probability. This shows TCP throughput is inversely proportional to $\sqrt{p}$ and to RTT.
> **中文**：其中 $p$ 是丢包率。该公式表明TCP吞吐量与 $\sqrt{p}$ 成反比，与RTT成反比。这意味着：丢包率越大，TCP吞吐量越低；RTT越大，TCP吞吐量也越低。

### 5.4.6 TCP Fairness / TCP公平性

AIMD (Additive Increase Multiplicative Decrease) converges to **fair sharing** of bandwidth among competing TCP flows. Each flow gets approximately equal bandwidth over time.
> **中文**：AIMD（加性增乘性减）机制使得竞争的TCP流收敛到带宽的**公平共享**。随着时间的推移，每条流获得大致相等的带宽。这一点可以通过"锯齿"形曲线的收敛性质来证明——当多条TCP流竞争同一链路时，加性增保证了它们共同增长，而乘性减保证了每条流在拥塞时按比例减少，从而趋于公平。

### 5.4.7 Congestion Detection: Timeout vs 3 Duplicate ACKs / 拥塞检测：超时与3重复ACK的区别

TCP interprets packet loss differently depending on how it is detected. The response differs accordingly.

> **中文**：TCP根据检测到丢包的方式不同，对丢包的解释也不同，相应的响应策略也不同。

**Timeout (超时) — Severe Congestion / 严重拥塞信号**:
- A timeout means **ALL** packets sent (and possibly their ACKs) were lost — the network is completely blocked
- The sender has zero feedback about the state of the network
- **Response**: cwnd = 1 MSS (reset to minimum), ssthresh = max(cwnd/2, 2), restart slow start
- This is the most aggressive response because the congestion is likely severe

> **中文**：**超时——严重拥塞**：
> - 超时意味着发送的**所有**数据包（以及可能的ACK）都丢失了——网络完全堵塞
> - 发送方对网络状态一无所知
> - **响应**：cwnd = 1 MSS（重置为最小值），ssthresh = max(cwnd/2, 2)，重新慢开始
> - 这是最激进的响应，因为拥塞可能非常严重

**3 Duplicate ACKs (3重复ACK) — Mild Congestion / 轻度拥塞信号**:
- Three duplicate ACKs mean the receiver is **still receiving** packets (it generated ACKs for packets after the gap)
- The network is still delivering data, just one (or a few) packets were lost
- **Response**: fast retransmit + fast recovery — cwnd halved, not reset to 1
- The congestion is likely mild, so a moderate response is appropriate

> **中文**：**3重复ACK——轻度拥塞**：
> - 3个重复ACK意味着接收方**仍在接收**数据包（它对丢失间隙后的数据包生成了ACK）
> - 网络仍在传输数据，只是丢失了一个（或少量）数据包
> - **响应**：快重传 + 快恢复——cwnd减半，而非重置为1
> - 拥塞可能较轻，因此采取适度的响应

**Why this distinction matters** / **为什么这种区分很重要**:

| Event / 事件 | Congestion Level / 拥塞程度 | cwnd After / 后的cwnd | Growth Phase After / 后续阶段 |
|:---:|:---:|:---:|:---:|
| Timeout / 超时 | **Severe / 严重** | **1** | Slow start / 慢开始 |
| 3 Dup ACKs / 3重复ACK | **Mild / 轻度** | **cwnd/2** (or ssthresh+3) | Congestion avoidance / 拥塞避免 |

> **中文**：**对比总结**：超时意味着网络可能完全瘫痪，发送方一无所知，因此必须极端保守地将cwnd重置为1并从头开始慢开始。3重复ACK则表明网络仍然在正常传输数据包（只是丢了其中一个），因此采取更温和的响应——将cwnd减半并进入拥塞避免阶段。这是TCP拥塞控制中"激进程度分级"的重要体现。

### 5.4.8 TCP Tahoe vs Reno / TCP Tahoe与Reno的区别

The document above describes **TCP Reno** (the 1990 version). Before Reno, the original congestion control was **TCP Tahoe** (1988). They differ in how they handle 3 duplicate ACKs.

> **中文**：上文描述的是**TCP Reno**（1990年版本）。在Reno之前，最初的拥塞控制协议是**TCP Tahoe**（1988年版本）。两者处理3重复ACK的方式不同。

**TCP Tahoe (1988)** / **TCP Tahoe（1988年）**:
- On 3 duplicate ACKs → **cwnd = 1**, ssthresh = cwnd/2, restart slow start
- Tahoe includes: slow start, congestion avoidance, fast retransmit
- Tahoe does **NOT** implement fast recovery — after fast retransmit, it falls back to slow start
- This is very conservative and can significantly underutilize bandwidth

> **中文**：**TCP Tahoe（1988年）**：
> - 收到3重复ACK → **cwnd = 1**，ssthresh = cwnd/2，重新慢开始
> - Tahoe包含：慢开始、拥塞避免、快重传
> - Tahoe**没有**实现快恢复——快重传后回退到慢开始
> - 这非常保守，可能显著降低带宽利用率

**TCP Reno (1990)** / **TCP Reno（1990年）**:
- On 3 duplicate ACKs → **fast recovery**: cwnd = ssthresh + 3, then enter congestion avoidance
- Reno adds fast recovery on top of everything Tahoe has
- More efficient — maintains higher throughput after a single packet loss
- This is what the document above describes in section 5.4.2 Algorithm 4

> **中文**：**TCP Reno（1990年）**：
> - 收到3重复ACK → **快恢复**：cwnd = ssthresh + 3，然后进入拥塞避免
> - Reno在Tahoe的所有算法基础上增加了快恢复
> - 更高效——在单个数据包丢失后仍能保持较高的吞吐量
> - 这就是上文5.4.2中算法4描述的内容

**Comparison Table / 对比表**:

| Feature / 特性 | TCP Tahoe (1988) | TCP Reno (1990) |
|:------|:---:|:---:|
| Slow Start / 慢开始 | Yes / 有 | Yes / 有 |
| Congestion Avoidance / 拥塞避免 | Yes / 有 | Yes / 有 |
| Fast Retransmit / 快重传 | Yes / 有 | Yes / 有 |
| **Fast Recovery / 快恢复** | **No / 无** | **Yes / 有** |
| On 3 dup ACKs / 收到3重复ACK时 | cwnd = **1**, restart slow start | cwnd = **ssthresh+3**, fast recovery |
| After recovery / 恢复后 | Slow start / 慢开始 | Congestion avoidance / 拥塞避免 |
| Performance / 性能 | Conservative (low throughput) / 保守（吞吐量低） | Efficient (higher throughput) / 高效（吞吐量更高） |

**Exam tip / 考试技巧**: If a problem says "TCP Reno" or "using fast recovery", apply Reno behavior. If it says "TCP Tahoe" or "no fast recovery", apply Tahoe behavior. Most modern TCP implementations use Reno or newer variants (NewReno, Cubic, etc.).

> **中文**：**考试技巧**：如果题目说"TCP Reno"或"使用快恢复"，则按Reno的行为答题。如果题目说"TCP Tahoe"或"无快恢复"，则按Tahoe的行为答题。大多数现代TCP实现使用Reno或更新的变体（NewReno、Cubic等）。

---

## 5.5 TCP Timer Management ★ / TCP定时器管理

### 5.5.1 Retransmission Timeout (RTO) Calculation / 重传超时计算

TCP uses an adaptive retransmission timer based on measured RTT.
> **中文**：TCP使用基于测量RTT的自适应重传定时器。超时时间（RTO）不是固定值，而是根据网络状况动态调整。

**Step 1 — EstimatedRTT**【估计RTT】(exponential weighted moving average / 指数加权移动平均):
$$EstimatedRTT = (1 - \alpha) \times EstimatedRTT + \alpha \times SampleRTT$$
Typically $\alpha = 0.125$ (1/8).
> **中文**：估计RTT是对历史RTT和最新采样RTT的加权平均。$\alpha$ 越小，历史值权重越大，平滑效果越好；$\alpha$ 越大，对网络变化响应越快。

**Step 2 — DevRTT**【RTT偏差】(measures variability / 测量波动性):
$$DevRTT = (1 - \beta) \times DevRTT + \beta \times |SampleRTT - EstimatedRTT|$$
Typically $\beta = 0.25$ (1/4).
> **中文**：DevRTT衡量RTT的波动程度。如果RTT变化很大，DevRTT就大，RTO也会相应增大，以减少不必要的重传。

**Step 3 — RTO**【重传超时】(Retransmission Timeout):
$$\boxed{RTO = EstimatedRTT + 4 \times DevRTT}$$

RTO is set to **EstimatedRTT + 4 × DevRTT** to provide a safety margin. The factor of 4 is chosen empirically to handle most RTT variations without excessive timeouts.
> **中文**：RTO = EstimatedRTT + 4 × DevRTT。其中"4"是一个经验值，提供了一个安全余量，既能容忍RTT的正常波动，又不会在真正丢包时等待过长时间。

**Example calculation** / **计算示例**：Given successive RTT samples, compute EstimatedRTT, DevRTT, and RTO / 给定连续的RTT采样值，计算EstimatedRTT、DevRTT和RTO。
- Initial / 初始值: EstimatedRTT = 100ms, DevRTT = 10ms
- SampleRTT = 120ms
- New EstimatedRTT = $0.875 \times 100 + 0.125 \times 120 = 87.5 + 15 = 102.5$ ms / 新的估计RTT
- New DevRTT = $0.75 \times 10 + 0.25 \times |120 - 102.5| = 7.5 + 4.375 = 11.875$ ms / 新的RTT偏差
- New RTO = $102.5 + 4 \times 11.875 = 102.5 + 47.5 = 150$ ms / 新的重传超时

> **中文**：**完整计算过程**：首先用新采样值（120ms）更新EstimatedRTT得到102.5ms，然后计算采样值与估计值的绝对差（17.5ms）来更新DevRTT得到11.875ms，最后代入RTO公式得到150ms。这意味着发送方在发送数据后，如果150ms内没有收到ACK，就会认为数据丢失并进行重传。

### 5.5.2 Karn's Algorithm / Karn算法

Karn's Algorithm solves a fundamental ambiguity in RTT estimation: when a segment is retransmitted and an ACK arrives, is the ACK responding to the **original** transmission or the **retransmission**?

> **中文**：Karn算法解决了RTT估计中的一个根本性歧义：当一个段被重传后收到ACK时，这个ACK是对**原始**发送的响应还是对**重传**的响应？

**The Ambiguity Problem** / **歧义问题**:
```
Sender                     Receiver
  |-------- Segment X -------->|   (Original / 原始发送, RTT unknown)
  |            (timeout)       |
  |-------- Segment X -------->|   (Retransmission / 重传)
  |<-------- ACK for X --------|   (Which transmission does this ACK match? / 这个ACK匹配哪次发送?)
```
If we mistakenly associate the ACK with the original transmission, the SampleRTT becomes **artificially large** (includes the timeout wait). If we associate it with the retransmission, it becomes **artificially small** (the real RTT is larger). Both cases corrupt the RTT estimate.

> **中文**：**歧义问题**：如果错误地将ACK与原始发送关联，则SampleRTT会变得**人为地大**（包含了超时等待时间）。如果将其与重传关联，则会变得**人为地小**（真实RTT更大）。两种情况都会破坏RTT估计的准确性。

**Karn's Algorithm Rule** / **Karn算法规则**:
1. **Do NOT** use retransmitted segments for RTT estimation — ignore their ACKs entirely
2. Only update RTT estimates from **non-retransmitted** segments (those that were acknowledged on the first try)
3. When a timeout/retransmission occurs, **freeze** the current RTO value and continue using it (do not update until a valid non-retransmitted ACK arrives)

> **中文**：**Karn算法规则**：
> 1. **不要**使用重传的段进行RTT估计——完全忽略它们的ACK
> 2. 仅从**非重传**的段（即首次发送就被确认的段）更新RTT估计
> 3. 当发生超时/重传时，**冻结**当前的RTO值并继续使用它（直到收到有效的非重传ACK才更新）

**Why this matters** / **为什么这很重要**:
- Without Karn's Algorithm, retransmissions cause RTT estimates to become incorrectly low (if ACK matches the retransmission), leading to overly aggressive RTO values
- This creates a feedback loop: short RTO → more false timeouts → more retransmissions → even shorter RTO
- Karn's Algorithm breaks this loop, ensuring RTT estimation remains accurate

> **中文**：**重要性**：
> - 没有Karn算法，重传会导致RTT估计变得错误地低（如果ACK匹配了重传），从而导致过于激进的RTO值
> - 这会产生一个反馈循环：短RTO → 更多的虚假超时 → 更多重传 → 更短的RTO
> - Karn算法打破了这个循环，确保RTT估计保持准确性

**Note** / **注意**: Karn's Algorithm is applied together with the **exponential backoff** of RTO (see next section). After a timeout, the RTO is doubled (backoff), and Karn's Algorithm freezes this backed-off RTO until a valid non-retransmitted ACK arrives.

> **中文**：Karn算法与RTO的**指数退避**（见下一节）一起使用。超时后，RTO翻倍（退避），Karn算法冻结这个退避后的RTO，直到收到有效的非重传ACK。

### 5.5.3 RTO Exponential Backoff / RTO指数退避

When a retransmission timeout occurs and the segment needs to be retransmitted, TCP doubles the RTO value for the next retransmission. This is called **exponential backoff**.

> **中文**：当发生重传超时且段需要重传时，TCP将RTO值翻倍用于下一次重传。这称为**指数退避**。

**Backoff Rule** / **退避规则**:
$$\text{RTO}_{\text{new}} = \text{RTO}_{\text{old}} \times 2$$

- Each consecutive timeout doubles the RTO again (2×, 4×, 8×, 16×, ...)
- An **upper bound** is typically applied: most implementations cap RTO at **60 seconds** (RFC 2988 recommends minimum 1s, maximum 60s)
- A **lower bound** is also applied: RTO is typically not allowed below **1 second** to prevent spurious timeouts

> **中文**：**退避规则**：
> - 每次连续超时都会再次将RTO翻倍（2倍、4倍、8倍、16倍……）
> - 通常设置**上限**：大多数实现将RTO上限设为**60秒**（RFC 2988建议最小值1秒，最大值60秒）
> - 也设置**下限**：RTO通常不低于**1秒**，以防止虚假超时

**Example** / **示例**:
1. Initial RTO = 1.5s → timeout → RTO = 3.0s
2. RTO = 3.0s → timeout → RTO = 6.0s
3. RTO = 6.0s → timeout → RTO = 12.0s
4. RTO = 12.0s → timeout → RTO = 24.0s
5. RTO = 24.0s → timeout → RTO = 48.0s
6. RTO = 48.0s → timeout → RTO = 60.0s (capped / 上限)

> **中文**：**示例**：
> 初始RTO = 1.5秒 → 超时 → RTO = 3.0秒
> RTO = 3.0秒 → 超时 → RTO = 6.0秒
> RTO = 6.0秒 → 超时 → RTO = 12.0秒
> 以此类推，直到达到60秒上限。

**Reset Condition** / **重置条件**:
The RTO is reset to its base value (computed from RTT estimation) when:
- A valid ACK for a **non-retransmitted** segment arrives
- This signals that the network has recovered and normal timing can resume

> **中文**：**重置条件**：当收到对**非重传**段的有效ACK时，RTO重置为基于当前RTT估计计算的基础值。这表明网络已经恢复，可以恢复正常计时。

**Why exponential backoff?** / **为什么需要指数退避？**
- Prevents flooding the network with retransmissions during severe congestion
- If congestion is severe, retransmitting at the same rate only makes it worse
- The backoff gives the network time to recover before the next retransmission attempt
- This is analogous to the "backoff" in Ethernet's CSMA/CD

> **中文**：**为什么需要指数退避？**
> - 防止在严重拥塞期间用重传淹没网络
> - 如果拥塞严重，以相同速率重传只会使情况恶化
> - 退避给网络恢复时间，然后再进行下一次重传尝试
> - 这与以太网CSMA/CD中的"退避"原理类似

---

## 5.6 Addresses in Internet Communication ★★★ (Comprehensive Big Question) / 互联网通信中的地址（综合大题）

When Host A on Network 1 sends data to Host B on Network 2, addresses change at each hop:
> **中文**：当网络1中的主机A向网络2中的主机B发送数据时，地址在每一跳的变化情况如下：

### Layer-by-Layer Address Analysis / 逐层地址分析

| Layer / 层次 | Address Used / 使用的地址 | Changes at Each Hop? / 每跳是否变化？ |
|-------|-------------|:---:|
| Application / 应用层 | URL, domain name → resolved by DNS / URL、域名 → DNS解析 | No (fixed) / 否（固定） |
| Transport / 传输层 | **Port numbers**【端口号】(src port, dst port) | **No** — port numbers remain unchanged / **不变化** |
| Network / 网络层 | **IP addresses**【IP地址】(src IP, dst IP) | **No** — IP addresses remain unchanged (except at NAT) / **一般不变化**（NAT处除外） |
| Data Link / 数据链路层 | **MAC addresses**【MAC地址】(src MAC, dst MAC) | **YES** — MAC addresses change at EVERY hop / **变化**——每一跳MAC地址都变化 |

### End-to-End Communication Flow / 端到端通信流程

1. **Application generates data**【应用层生成数据】：Domain name is resolved to destination IP via DNS / 域名通过DNS解析为目标IP地址
> **中文**：例如用户输入 www.example.com，DNS将其解析为 93.184.216.34。

2. **Transport layer**【传输层】：Adds TCP header with source/destination ports (e.g., 49152→80) / 添加TCP首部，包含源端口和目的端口（例如 49152→80）
> **中文**：源端口通常是系统分配的临时端口（如49152），目的端口是应用层服务的知名端口（如HTTP的80端口）。

3. **Network layer**【网络层】：Adds IP header with source/destination IPs / 添加IP首部，包含源IP和目的IP
> **中文**：源IP是发送主机的IP地址，目的IP是DNS解析得到的目标主机的IP地址。

4. **Data Link layer**【数据链路层】：
   - If destination is on the same network / 如果目的地在同一网络中：encapsulates with dest MAC = ARP result for dest IP / 封装时目标MAC = 对目的IP进行ARP解析的结果
   - If destination is on a different network / 如果目的地在不同网络中：encapsulates with dest MAC = ARP result for **default gateway's IP** / 封装时目标MAC = 对**默认网关IP**进行ARP解析的结果
   - > **中文**：这是关键区别：当目标不在同一网络时，数据帧的目标MAC地址不是目标主机的MAC地址，而是默认网关（路由器）的MAC地址，因为数据需要先发送到路由器进行转发。

5. **At each router**【在每个路由器处】：
   - Router strips data link header / 路由器去掉数据链路层首部
   - Router examines IP header (routing decision), decrements TTL / 路由器检查IP首部（路由决策），TTL减1
   - Router re-encapsulates with new MAC addresses for next hop / 路由器用新的MAC地址重新封装（下一跳的MAC地址）
   - **IP addresses and port numbers never change** (except through NAT) / **IP地址和端口号从不改变**（通过NAT时除外）
   - > **中文**：**地址变化总结**：在逐跳转发过程中，MAC地址在每一跳都发生变化（因为每一段链路的发送方和接收方不同），而IP地址和端口号在整个传输过程中保持不变（除非经过NAT转换）。这使得网络层和传输层能够实现端到端的通信。

6. **At destination**【在目的地】：Data travels up the stack, each layer stripping its header / 数据沿协议栈向上传递，每一层去掉对应的首部

---

## 5.7 TCP vs UDP Comparison Table (Must Memorize) / TCP与UDP对比表（必须熟记）

| 特性 | TCP | UDP |
|-----|-----|-----|
| **Connection / 连接** | Connection-oriented / 面向连接 | Connectionless / 无连接 |
| **Reliability / 可靠性** | Reliable (ACKs, retransmission) / 可靠（确认、重传） | Unreliable (no ACK) / 不可靠（无确认） |
| **Ordering / 有序性** | In-order delivery / 按序交付 | No ordering guarantee / 不保证顺序 |
| **Data boundary / 数据边界** | Byte stream (no boundaries) / 字节流（无边界） | Message boundaries preserved / 保留消息边界 |
| **Flow control / 流量控制** | Yes (sliding window) / 是（滑动窗口） | No / 无 |
| **Congestion control / 拥塞控制** | Yes (slow start, AIMD) / 是（慢开始、AIMD） | No / 无 |
| **Header size / 首部大小** | 20-60 bytes / 20-60字节 | 8 bytes / 8字节 |
| **Speed / 速度** | Slower (more overhead) / 较慢（开销大） | Faster (less overhead) / 较快（开销小） |
| **Broadcast/Multicast / 广播/多播** | No (point-to-point only) / 否（仅点对点） | Yes / 支持 |
| **Typical applications / 典型应用** | Web, email, file transfer, SSH / 网页、邮件、文件传输、SSH | DNS, streaming, VoIP, gaming / DNS、流媒体、网络电话、游戏 |

> **中文**：**记忆技巧**：TCP提供可靠但慢的服务（适合需要数据完整性的应用），UDP提供不可靠但快的服务（适合实时性要求高的应用）。考试中常考的对比点包括：是否面向连接、是否可靠、首部大小、是否支持广播/多播、典型应用等。

---

## 5.8 Exam Practice: Transport Layer Questions / 考试练习：传输层问题

### Fill-in-the-Blank / 填空题

1. TCP establishes a connection using a ______-way handshake and releases it using a ______-way handshake. / TCP使用______次握手建立连接，使用______次握手释放连接。
   **Answer: three (三), four (四)**

2. The TCP flags used in connection establishment are ______ and ______. / 建立连接时使用的TCP标志是______和______。
   **Answer: SYN, ACK**

3. The TCP flag used to initiate connection release is ______. / 用于发起连接释放的TCP标志是______。
   **Answer: FIN**

4. In TCP, the congestion window is measured in units of ______. / 在TCP中，拥塞窗口以______为单位。
   **Answer: MSS — Maximum Segment Size, or bytes / MSS（最大段长度）或字节数**

5. TCP's congestion control has four algorithms: ______, ______, ______, and ______. / TCP拥塞控制的四种算法是：______、______、______和______。
   **Answer: slow start (慢开始), congestion avoidance (拥塞避免), fast retransmit (快重传), fast recovery (快恢复)**

6. After a timeout, TCP sets cwnd = ______ and restarts slow start. / 超时后，TCP将cwnd设为______并重新开始慢开始。
   **Answer: 1**

7. The UDP header is ______ bytes; the minimum TCP header is ______ bytes. / UDP首部为______字节；TCP首部最小为______字节。
   **Answer: 8 (八), 20 (二十)**

8. DNS typically uses ______ (TCP/UDP) on port ______. / DNS通常使用______（TCP/UDP）协议，端口号为______。
   **Answer: UDP, 53**

9. The 16-bit window size field in TCP is used for ______ control. / TCP中16位的窗口大小字段用于______控制。
   **Answer: flow (流量)**

10. The receiver's advertised window (rwnd) and the sender's ______ together determine the effective sending window. / 接收方通告窗口（rwnd）和发送方的______共同决定有效发送窗口。
    **Answer: cwnd — congestion window (拥塞窗口)**

### True/False / 判断题

1. (T/F) UDP provides reliable data delivery. / UDP提供可靠的数据交付。
   **Answer: F / 错** — it's unreliable, no ACKs / UDP不可靠，无确认机制

2. (T/F) In TCP's three-way handshake, the first segment has only the SYN flag set. / 在TCP三次握手中，第一个段只设置了SYN标志。
   **Answer: T / 对** — it has SYN set; ACK flag is not set in the very first segment / 第一个段只设置了SYN，ACK标志未设置

3. (T/F) TCP's fast retransmit is triggered after receiving 3 duplicate ACKs. / TCP快重传在收到3个重复ACK后触发。
   **Answer: T / 对**

4. (T/F) During slow start, cwnd increases by 1 MSS per RTT. / 在慢开始阶段，cwnd每个RTT增加1个MSS。
   **Answer: F / 错** — slow start is exponential: cwnd doubles each RTT; congestion avoidance is +1 per RTT / 慢开始是指数增长：cwnd每个RTT翻倍；拥塞避免是每个RTT+1

5. (T/F) In TCP's four-way connection release, the server's ACK and FIN can be combined into a single segment. / 在TCP四次挥手中，服务器的ACK和FIN可以合并为一个段发送。
   **Answer: F / 错** — they are separate because the server may still have data to send after acknowledging the client's FIN / 它们是分开的，因为服务器在确认客户端的FIN后可能还有数据要发送

6. (T/F) The TIME_WAIT state lasts for 1 MSL. / TIME_WAIT状态持续1个MSL。
   **Answer: F / 错** — it lasts for 2 × MSL / 持续 2 × MSL

7. (T/F) TCP sequence numbers count segments, not bytes. / TCP序号对段进行编号，而非对字节编号。
   **Answer: F / 错** — TCP sequence numbers count BYTES, not segments / TCP序号对字节进行编号，而非对段编号

### Multiple Choice / 选择题

1. Which flag is set in ALL TCP segments except the initial SYN? / 除初始SYN外，哪个标志在所有TCP段中都设置？
   a) SYN b) FIN c) ACK d) RST
   **Answer: c / 答案：c (ACK)** — once connection is established, ACK is always set / 一旦连接建立，ACK总是被设置

2. If a TCP receiver gets bytes 0-999 correctly but byte 1000 is lost, and then gets bytes 1001-2000, what ACK does it send? / 如果TCP接收方正确收到字节0-999但字节1000丢失，然后又收到字节1001-2000，它会发送什么ACK？
   a) 1000 b) 1001 c) 2000 d) 2001
   **Answer: a / 答案：a (1000)** — TCP uses cumulative ACK; it ACKs the next byte it expects, which is 1000 / TCP使用累积确认；它确认期望的下一个字节，即1000

3. After a timeout, TCP's ssthresh is set to: / 超时后，TCP的ssthresh设为：
   a) cwnd b) cwnd/2 c) 1 d) 2 × cwnd
   **Answer: b / 答案：b** — multiplicative decrease / 乘性减少

4. What is the purpose of the pseudo-header in UDP checksum calculation? / UDP校验和计算中伪首部的目的是什么？
   a) Reduce overhead / 减少开销 b) Verify IP addresses are correct / 验证IP地址正确 c) Encrypt the data / 加密数据 d) Compress the header / 压缩首部
   **Answer: b / 答案：b**

5. During congestion avoidance, cwnd increases: / 在拥塞避免阶段，cwnd的增长方式为：
   a) Exponentially / 指数增长 b) By 1 MSS per RTT / 每个RTT增加1个MSS c) By 2 MSS per RTT / 每个RTT增加2个MSS d) Stays constant / 保持不变
   **Answer: b / 答案：b**

6. Which transport protocol is suitable for real-time voice calls? / 哪个传输协议适合实时语音通话？
   a) TCP b) UDP c) Both equally / 两者均可 d) Neither / 两者都不
   **Answer: b / 答案：b** — UDP's low latency is preferred for real-time; occasional packet loss is acceptable / UDP的低延迟适合实时应用；偶尔丢包可接受

### Calculation / Diagram Problems / 计算/图示题

**Problem 1** (3-way handshake) / **问题1**（三次握手）：Given an initial SYN from client with seq=200, fill in the sequence numbers for all three segments. / 给定客户端的初始SYN的seq=200，填写所有三个段的序号。

1. Client → Server / 客户端 → 服务器端: SYN, **seq=200** (初始序号为200)
2. Server → Client / 服务器端 → 客户端: SYN+ACK, **seq=500**, **ack=201** (服务器初始序号500，确认客户端序号200+1)
3. Client → Server / 客户端 → 服务器端: ACK, **seq=201**, **ack=501** (客户端序号200+1，确认服务器序号500+1)

> **中文**：**解析**：第一次握手客户端SYN序号为200；第二次握手服务器SYN+ACK，服务器自己的初始序号为500（任意），确认号为200+1=201；第三次握手客户端ACK，序号为200+1=201，确认号为500+1=501。关键规律：序号在对方ACK的基础上取对方的初始序号+1。

**Problem 2** (Congestion control graph) / **问题2**（拥塞控制图）：Initial ssthresh=8, cwnd=1. Draw the cwnd evolution through RTTs 1-12 assuming a timeout occurs after RTT 7 when cwnd=10. / 初始ssthresh=8，cwnd=1。假设RTT 7后（cwnd=10时）发生超时，画出RTT 1-12的cwnd演化。

- RTT 1: cwnd=1 (slow start / 慢开始)
- RTT 2: cwnd=2 (slow start / 慢开始)
- RTT 3: cwnd=4 (slow start / 慢开始)
- RTT 4: cwnd=8 (reaches ssthresh, transitions / 达到ssthresh，过渡)
- RTT 5: cwnd=9 (congestion avoidance, +1 / 拥塞避免)
- RTT 6: cwnd=10 (congestion avoidance / 拥塞避免)
- RTT 7: **Timeout / 超时** → ssthresh=5, cwnd=1 (ssthresh = 10/2 = 5)
- RTT 8: cwnd=1 (slow start / 慢开始)
- RTT 9: cwnd=2 (slow start / 慢开始)
- RTT 10: cwnd=4 (slow start / 慢开始)
- RTT 11: cwnd=5 (reaches new ssthresh=5 / 达到新阈值)
- RTT 12: cwnd=6 (congestion avoidance / 拥塞避免)

> **中文**：**解析**：前4个RTT为慢开始阶段（1→2→4→8），cwnd翻倍增长。RTT 4达到ssthresh=8后进入拥塞避免阶段（RTT 5: 9, RTT 6: 10）。RTT 7发生超时，ssthresh更新为10/2=5，cwnd重置为1。RTT 8-10重新慢开始（1→2→4）。RTT 11达到新阈值5后再次进入拥塞避免（RTT 12: 6）。

**Problem 3** (RTO) / **问题3**（RTO计算）：Given EstimatedRTT=30ms, DevRTT=5ms, SampleRTT=50ms. $\alpha$=0.125, $\beta$=0.25. Find new RTO. / 已知EstimatedRTT=30ms, DevRTT=5ms, SampleRTT=50ms，求新的RTO。

- New EstimatedRTT / 新的估计RTT = 0.875×30 + 0.125×50 = 26.25 + 6.25 = **32.5 ms**
- New DevRTT / 新的RTT偏差 = 0.75×5 + 0.25×|50-32.5| = 3.75 + 0.25×17.5 = 3.75 + 4.375 = **8.125 ms**
- RTO = 32.5 + 4×8.125 = 32.5 + 32.5 = **65 ms**

> **中文**：**解析**：三步计算法——第一步用指数加权移动平均更新EstimatedRTT（32.5ms），第二步更新DevRTT衡量RTT波动性（8.125ms），第三步用RTO = EstimatedRTT + 4×DevRTT公式得出RTO为65ms。这意味着在新的网络条件下，TCP发送方将在65ms后判断数据包丢失。

> **中文**：**考试技巧**：拥塞控制大题通常需要同学们：
> 1. 能画出拥塞窗口随时间变化的"锯齿"图
> 2. 能计算超时和3重复ACK后的cwnd和ssthresh值
> 3. 能区分超时和3重复ACK两种情况的处理差异
> 4. 能计算RTO

---

## Appendix: Chinese-English Terminology / 中英文术语对照

### Transport Layer / 传输层
| English / 英文 | Chinese / 中文 |
|------|------|
| Process-to-process delivery | 进程到进程传输 |
| End-to-end | 端到端 |
| Port number | 端口号 |
| Socket | 套接字 |
| Well-known port | 知名端口 |
| UDP (User Datagram Protocol) | 用户数据报协议 |
| TCP (Transmission Control Protocol) | 传输控制协议 |
| Segment | 段（TCP数据单元）|
| Datagram | 数据报（UDP数据单元）|
| Pseudo-header | 伪首部 |
| Checksum | 校验和 |
| MSS (Maximum Segment Size) | 最大段长度 |
| Three-way handshake | 三次握手 |
| Four-way handshake | 四次挥手 |
| SYN flag | 同步标志 |
| FIN flag | 终止标志 |
| ACK flag | 确认标志 |
| RST flag | 复位标志 |
| TIME_WAIT state | TIME_WAIT状态 |
| MSL (Maximum Segment Lifetime) | 最大段生存时间 |
| Flow control | 流量控制 |
| Congestion control | 拥塞控制 |
| Slow start | 慢开始 |
| Congestion avoidance | 拥塞避免 |
| Fast retransmit | 快重传 |
| Fast recovery | 快恢复 |
| AIMD (Additive Increase Multiplicative Decrease) | 加性增加乘性减少 |
| cwnd (Congestion Window) | 拥塞窗口 |
| rwnd (Receiver Window) | 接收窗口 |
| ssthresh (Slow Start Threshold) | 慢开始阈值 |
| RTO (Retransmission Timeout) | 重传超时 |
| RTT (Round-Trip Time) | 往返时延 |
| Cumulative ACK | 累积确认 |
| Duplicate ACK | 重复确认 |
| Piggybacking | 捎带确认 |
| Silly window syndrome | 傻瓜窗口综合征 |
| Nagle's algorithm | Nagle算法 |
| Delayed ACK | 延迟确认 |
| SACK (Selective Acknowledgment) | 选择性确认 |
| Karn's algorithm | Karn算法 |
| Exponential backoff | 指数退避 |
| TCP Tahoe | TCP Tahoe |
| TCP Reno | TCP Reno |
| Tinygram problem | 小数据报问题 |
| TCP_NODELAY | TCP_NODELAY套接字选项 |
