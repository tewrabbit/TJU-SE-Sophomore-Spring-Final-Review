# Computer Networks Final Exam — Quick Reference / 计算机网络期末考试速查表

> **Exam Format / 考试题型**: Fill-in-blanks (10 pts, 0.5 ea) / 填空题(10分) + Single Choice (40 pts, 2 ea) / 单选题(40分) + Big Questions (50 pts, 6 questions) / 大题(50分, 6题)
> **Exam Language / 考试语言**: English / 英文
> **Use this document / 使用时机**: Last 1-2 hours before the exam / 考前1-2小时。Contains every critical formula, table, and pitfall / 包含所有关键公式、对比表和易错点。

---

## 1. ALL Formulas / 全部公式 (2 Pages / 2页)

### Physical Layer / 物理层
| Formula / 公式 | When to Use / 使用场景 | 中文说明 |
|---------|-------------|------|
| $C = 2H \log_2 V$ | Nyquist: max data rate, NO noise / 奈奎斯特：无噪声最大速率 | 最大比特率 = 2 × 带宽 × log₂(电平数) |
| $C = H \log_2(1 + S/N)$ | Shannon: max data rate, WITH noise / 香农：有噪声最大速率 | 最大比特率 = 带宽 × log₂(1+信噪比) |
| $S/N = 10^{\text{dB}/10}$ | Convert dB to power ratio / dB转功率比值 | 信噪比 = 10的(dB/10)次方 |
| $\text{Bit rate} = \text{Baud} \times \log_2 V$ | Baud vs bit rate / 波特率转比特率 | 比特率 = 波特率 × log₂(电平数) |
| $\text{Sampling rate} \ge 2 \times f_{max}$ | PCM sampling / PCM采样率 | 采样率 ≥ 2 × 最高频率 |
| $\text{T1 rate} = 193 / 125\text{μs} = 1.544\text{ Mbps}$ | T1 calculation / T1速率计算 | T1速率 = 193比特 / 125微秒 |

### Delay and Performance / 时延与性能
| Formula / 公式 | When to Use / 使用场景 | 中文说明 |
|---------|-------------|------|
| $T_{trans} = L / B$ | Transmission delay / 发送时延 | 发送时延 = 数据长度 / 带宽 |
| $T_{prop} = D / v$ | Propagation delay / 传播时延 | 传播时延 = 距离 / 传播速度 |
| $T_{total} = T_{trans} + T_{prop} + T_{proc} + T_{queue}$ | Total delay / 总时延 | 总时延 = 发送+传播+处理+排队 |
| $BDP = B \times RTT$ | Bandwidth-delay product / 带宽时延积 | 带宽时延积 = 带宽 × 往返时延 |
| $U = T_D / (T_D + RTT + T_A)$ | Stop-and-wait utilization / 停等利用率 | 利用率 = 发送时延 / 总时间 |
| $D = D_0 / (1 - U)$ | Delay under utilization / 利用率下的时延 | 实际时延 = 空闲时延 / (1-利用率) |

### MAC / Data Link / MAC层与数据链路层
| Formula / 公式 | When to Use / 使用场景 | 中文说明 |
|---------|-------------|------|
| $L_{min} = 2\tau \times B$ | CSMA/CD minimum frame length / 最短帧长 | 最短帧长 = 2倍传播时延 × 带宽 |
| $S_{pure} = G \cdot e^{-2G}$, max = $1/2e \approx 18.4\%$ | Pure ALOHA throughput / 纯ALOHA吞吐量 | 纯ALOHA最高约18.4% |
| $S_{slotted} = G \cdot e^{-G}$, max = $1/e \approx 36.8\%$ | Slotted ALOHA throughput / 时隙ALOHA吞吐量 | 时隙ALOHA最高约36.8% |
| $\text{Backoff} = \text{Slot} \times \text{rand}[0, 2^k-1], k=\min(n,10)$ | Binary exponential backoff / 二进制指数退避 | 退避时间 = 时隙 × 随机数 |
| $2^r \ge k + r + 1$ | Hamming code redundancy bits / 海明码冗余位 | 2的r次方 ≥ 数据位+校验位+1 |

### Sliding Window / 滑动窗口
| Formula / 公式 | When to Use / 使用场景 | 中文说明 |
|---------|-------------|------|
| $W_T \le 2^n - 1$ | GBN max sender window / GBN最大发送窗口 | n位序号→最大窗口2^n-1 |
| $W_T \le 2^{n-1}$, $W_R \le 2^{n-1}$ | SR max window / SR最大窗口 | n位序号→最大窗口2^(n-1) |
| $n = \lceil \log_2(W + 1) \rceil$ | GBN sequence bits / GBN序号位数 | GBN所需位数 |
| $n = \lceil \log_2 W \rceil + 1$ | SR sequence bits / SR序号位数 | SR所需位数 |
| $W = 1 + \lfloor RTT / T_f \rfloor$ | Window needed for full pipe / 满管道所需窗口 | 窗口大小 = 1 + 往返时延/帧发送时间 |

### Network Layer / 网络层
| Formula / 公式 | When to Use / 使用场景 | 中文说明 |
|---------|-------------|------|
| $\text{Offset (8B units)} = \text{previous\_data\_bytes} / 8$ | IP fragment offset / IP分片偏移 | 片偏移(以8字节为单位) |
| $\text{Max data per fragment} = \lfloor (MTU - 20) / 8 \rfloor \times 8$ | Fragmentation / 分片数据量 | 每片最大数据 = 向下取整到8的倍数 |
| $\text{Usable hosts} = 2^{32-n} - 2$ | Subnet host count (for /n prefix) / 子网可用主机数 | 可用主机 = 2^(32-前缀长) - 2 |

### QoS / Queuing / 服务质量和排队
| Formula / 公式 | When to Use / 使用场景 | 中文说明 |
|---------|-------------|------|
| $T = 1 / (\mu - \lambda)$ | M/M/1 avg time in system / 平均系统时间 | 系统时间 = 1/(服务率-到达率) |
| $\rho = \lambda / \mu$ | M/M/1 utilization / 利用率 | 利用率 = 到达率/服务率 |
| $N = \rho / (1 - \rho)$ | M/M/1 avg queue length / 平均队列长度 | 平均队长 = 利用率/(1-利用率) |
| $F_i = \max(A_i, F_{i-1}) + L_i/W$ | WFQ finish number / WFQ完成时间 | 完成轮数 = max(到达,前包完成)+长度/权重 |
| $t = b / (P - r)$ | Token bucket max burst time / 令牌桶最大突发时间 | 突发时间 = 桶容量/(峰值速率-令牌速率) |
| $\text{Max rate} = r$ | Token bucket sustained rate / 令牌桶持续速率 | 最大持续速率 = 令牌产生速率 |

### Transport Layer / 传输层
| Formula / 公式 | When to Use / 使用场景 | 中文说明 |
|---------|-------------|------|
| $RTO = EstimatedRTT + 4 \times DevRTT$ | TCP retransmission timeout / TCP重传超时 | 重传超时 = 估计RTT + 4 × RTT偏差 |
| $EstimatedRTT = (1-\alpha) \times Est + \alpha \times Sample$ | RTT estimation / RTT估计 ($\alpha=0.125$) | 新估计RTT = 87.5%旧值 + 12.5%样本值 |
| $DevRTT = (1-\beta) \times Dev + \beta \times |Sample - Est|$ | RTT deviation / RTT偏差 ($\beta=0.25$) | 新偏差 = 75%旧值 + 25%本次偏差 |

---

## 2. ALL Comparison Tables / 全部对比表

### OSI (7) vs TCP/IP (4) / OSI七层 vs TCP/IP四层
| OSI Layer / OSI层 | Chinese / 中文 | PDU | TCP/IP Layer / TCP/IP层 | Key Protocols / 主要协议 |
|-----------|------|-----|-------------|------|
| Application (7) | 应用层 | APDU | Application / 应用层 | HTTP, FTP, SMTP, DNS |
| Presentation (6) | 表示层 | PPDU | ↑ merged / 合并 ↑ | — |
| Session (5) | 会话层 | SPDU | ↑ merged / 合并 ↑ | — |
| Transport (4) | 传输层 | TPDU/Segment / 段 | Transport / 传输层 | TCP, UDP |
| Network (3) | 网络层 | Packet / 分组 | Internet / 网际层 | IP, ICMP, ARP |
| Data Link (2) | 数据链路层 | Frame / 帧 | Link / 网络接口层 | Ethernet, Wi-Fi, PPP |
| Physical (1) | 物理层 | Bit / 比特 | ↑ merged / 合并 ↑ | 802.3 PHY, Manchester |

### Circuit vs Packet Switching / 电路交换 vs 分组交换
| Aspect / 方面 | Circuit / 电路交换 | Packet (Datagram) / 分组交换(数据报) |
|--|:---:|:---:|
| Setup / 建立连接 | Required / 需要 | None / 不需要 |
| Path / 路径 | Dedicated / 专用 | Shared / 共享 |
| Addressing / 寻址 | During setup only / 仅建立时 | Every packet / 每个分组 |
| Utilization / 利用率 | Low / 低 | High / 高 |
| Delay formula / 时延公式 | $T_{setup}+T_{trans}+T_{prop}$ | Pipeline: $(m+n-1)T_{pkt}+T_{prop}$ |

### GBN vs Selective Repeat / 后退N帧 vs 选择重发
| Aspect / 方面 | Go-Back-N / 后退N帧 | Selective Repeat / 选择重发 |
|--|:---:|:---:|
| Sender window / 发送窗口 | $\le 2^n - 1$ | $\le 2^{n-1}$ |
| Receiver window / 接收窗口 | 1 | $\le 2^{n-1}$ |
| Out-of-order frames / 乱序帧 | Discard / 丢弃 | Buffer / 缓存 |
| On loss / 丢帧处理 | Retransmit all unacked / 重发全部 | Retransmit only lost / 只重发丢失的 |

### CSMA/CD vs CSMA/CA / 有线 vs 无线
| Aspect / 方面 | CSMA/CD (802.3) / 有线以太网 | CSMA/CA (802.11) / 无线局域网 |
|--|:---:|:---:|
| Medium / 介质 | Wired / 有线 | Wireless / 无线 |
| Strategy / 策略 | Detect collisions / 检测碰撞 | Avoid collisions / 避免碰撞 |
| Listen-while-talk / 边说边听 | Yes / 能 | No (can't) / 不能 |
| ACK at MAC / MAC层确认 | No / 无 | Yes / 有 |
| RTS/CTS | No / 无 | Yes / 有 |
| Hidden terminal / 隐藏终端 | Not an issue / 不是问题 | Addressed by RTS/CTS / 用RTS/CTS解决 |

### RIP vs OSPF vs BGP / 三种路由协议对比
| Aspect / 方面 | RIP | OSPF | BGP |
|--|:---:|:---:|:---:|
| Type / 类型 | IGP / 内部网关协议 | IGP / 内部网关协议 | EGP / 外部网关协议 |
| Algorithm / 算法 | Distance Vector / 距离向量 | Link State / 链路状态 | Path Vector / 路径向量 |
| Metric / 度量 | Hops (≤15) / 跳数 | Cost / 代价 | AS Path / AS路径 |
| Encapsulation / 封装 | UDP 520 | IP Proto 89 | TCP 179 |
| Scope / 范围 | Small nets / 小型网络 | Large nets / 大型网络 | Global Internet / 全球互联网 |

### TCP vs UDP / TCP与UDP对比
| Aspect / 方面 | TCP | UDP |
|--|:---:|:---:|
| Connection / 连接 | Connection-oriented / 面向连接 | Connectionless / 无连接 |
| Reliable / 可靠性 | Yes / 可靠 | No / 不可靠 |
| Ordering / 有序性 | In-order / 有序 | No guarantee / 无保证 |
| Flow control / 流量控制 | Yes / 有 | No / 无 |
| Congestion control / 拥塞控制 | Yes / 有 | No / 无 |
| Header / 头部大小 | 20-60 B | 8 B |
| Applications / 应用 | Web, email, FTP, SSH | DNS, VoIP, streaming / 流媒体 |

### Router vs Switch / 路由器 vs 交换机
| Aspect / 方面 | Router (L3) / 路由器 | Switch (L2) / 交换机 |
|--|:---:|:---:|
| Forward by / 转发依据 | IP address / IP地址 | MAC address / MAC地址 |
| Ports have IP? / 端口有IP? | Yes / 有 | No / 无 |
| Segments broadcasts? / 隔离广播域? | **Yes / 能** | **No / 不能** |
| Segments collisions? / 隔离冲突域? | Yes / 能 | Yes / 能 |

### Leaky Bucket vs Token Bucket / 漏桶 vs 令牌桶
| Aspect / 方面 | Leaky Bucket / 漏桶 | Token Bucket / 令牌桶 |
|--|:---:|:---:|
| Output / 输出 | Constant rate / 恒定速率 | Bursty, then token-limited / 允许突发 |
| Burst handling / 突发处理 | Smoothes out / 平滑化 | Allows bursts / 允许突发 (up to bucket size) |
| Overflow behavior / 溢出行为 | Drops packets / 丢弃包 | Tokens limited (packets wait) / 令牌限制（包等待）|
| Primary use / 主要用途 | Traffic smoothing / 流量平滑 | Burst tolerance / 突发容忍 |

### IPv4 vs IPv6 / IPv4与IPv6对比
| Aspect / 方面 | IPv4 | IPv6 |
|--|:---:|:---:|
| Address size / 地址长度 | 32 bits / 32位 | 128 bits / 128位 |
| Notation / 表示法 | Dotted decimal / 点分十进制 | Hex colon groups / 冒号十六进制 |
| Header / 头部 | Variable (20-60) / 可变长 | Fixed 40 bytes / 固定40字节 |
| Fragmentation / 分片 | Routers + hosts / 路由器+主机 | Hosts only / 仅主机 |
| Checksum / 校验和 | Yes / 有 | No / 无 |

---

## 3. Special IP Addresses / 特殊IP地址 (Must Memorize / 必背)

| Type / 类型 | Example / 示例 | Used As Source? / 作源地址? | Used As Dest? / 作目的地址? | 中文说明 |
|------|---------|:---:|:---:|------|
| Network address / 网络地址 | 192.168.1.0 | No | No | 标识网络本身 |
| Direct broadcast / 直接广播 | 192.168.1.255 | No | **Yes** | 对特定网络所有主机广播 |
| Limited broadcast / 受限广播 | 255.255.255.255 | No | **Yes** | 仅在本网络广播（路由器不转发）|
| This host (bootstrap) / 本主机 | 0.0.0.0 | **Yes** | No | 启动时使用 |
| Loopback / 环回 | 127.x.x.x | **Yes** | **Yes** | 本地软件回送测试 |

**Private addresses / 私有地址**: `10.0.0.0/8`, `172.16.0.0/12`, `192.168.0.0/16`
> **中文**：私有地址不能出现在公网上，需通过NAT转换。

**Class ranges / 分类地址范围**: A: 1-126, B: 128-191, C: 192-223, D: 224-239 (multicast / 组播)

---

## 4. TCP State Flags & Handshake / TCP状态标志与握手

### Flags / 标志位: SYN, ACK, FIN, RST, PSH, URG

### 3-Way Handshake / 三次握手
```
C → S: SYN (seq=x, no ACK flag)    【客户端发起连接】
S → C: SYN+ACK (seq=y, ack=x+1)    【服务器确认并同意】
C → S: ACK (seq=x+1, ack=y+1)      【客户端确认服务器的SYN】
```

> **中文**：为什么是3次？——防止已失效的连接请求报文突然传到服务器造成资源浪费。第3次ACK是客户端告诉服务器"我知道你同意建立连接了"。

### 4-Way Release / 四次挥手
```
A → B: FIN (seq=u)                  【A说"我不发了"】
B → A: ACK (ack=u+1)                【B说"知道了"】
B → A: FIN (seq=w, ack=u+1)         【B说"我也不发了"】
A → B: ACK (ack=w+1)                【A说"知道了"】
[A enters TIME_WAIT for 2MSL / A进入TIME_WAIT等待2倍MSL]
```

> **中文**：为什么是4次？——TCP连接释放是**对称的**（各关各的方向）。B的ACK和FIN分开发是因为B收到A的FIN后可能还有数据要发送。

---

## 5. TCP Congestion Control / TCP拥塞控制

### State Machine / 状态机
```
         ┌─────────────┐
         │  Slow Start  │  cwnd doubles per RTT / cwnd每RTT翻倍
         │  cwnd = 1    │  (慢开始：从1开始指数增长)
         └──────┬──────┘
                │ cwnd ≥ ssthresh (达到阈值)
                ↓
         ┌─────────────┐
         │  Congestion  │  cwnd += 1 per RTT (linear) / 每RTT线性增长+1
         │  Avoidance   │  (拥塞避免：加性增加)
         └──┬───────┬──┘
            │       │
    timeout │       │ 3 dup ACKs (3个重复ACK)
     (超时)  ↓       ↓
   ssthresh=cwnd/2  ssthresh=cwnd/2
   cwnd=1           cwnd=ssthresh+3
   → Slow Start     → Fast Recovery (快恢复)
```

> **中文核心记忆**：
> - **超时** = 真正拥塞 → cwnd降到1 → 重新慢开始
> - **3个重复ACK** = 疑似丢包但网络还好 → cwnd减半 → 快恢复（不降到1）
> - **慢开始阈值(ssthresh)** = cwnd减半后的值，作为慢开始和拥塞避免的分界线

---

## 6. Common Port Numbers / 常见端口号

| Port / 端口 | Service / 服务 | Protocol / 协议 | 中文说明 |
|:---:|---------|:---:|------|
| 20 | FTP (data) / 文件传输(数据) | TCP | 文件传输协议数据通道 |
| 21 | FTP (control) / 文件传输(控制) | TCP | 文件传输协议控制通道 |
| 22 | SSH / 安全外壳 | TCP | 加密远程登录 |
| 23 | Telnet / 远程登录 | TCP | 远程登录（无加密）|
| 25 | SMTP / 简单邮件传输 | TCP | 发送邮件 |
| 53 | DNS / 域名系统 | UDP/TCP | 域名解析 |
| 67/68 | DHCP / 动态主机配置 | UDP | 动态IP分配 |
| 80 | HTTP / 超文本传输 | TCP | 网页浏览 |
| 443 | HTTPS / 安全HTTP | TCP | 加密网页浏览 |
| 520 | RIP / 路由信息协议 | UDP | 距离向量路由协议 |

---

## 7. Common Pitfalls / 易错点 (Must Read Before Exam / 考前必读)

### Physical Layer / 物理层
- ❌ "Baud rate = bit rate" → ✓ Only when V=2 (binary) / 仅V=2时成立
- ❌ "Propagation delay depends on data size" → ✓ Only depends on distance and medium / 仅取决于距离和介质
- ❌ "Shannon SNR uses dB directly" → ✓ Must convert: $S/N = 10^{dB/10}$ / 必须先转比值

### MAC / Data Link / MAC层
- ❌ "64-byte minimum includes preamble" → ✓ Preamble NOT included / 前导码不计入64字节
- ❌ "GBN receiver window can be > 1" → ✓ Always exactly 1 / GBN接收窗口永远是1
- ❌ "SR max window = $2^n - 1$" → ✓ $2^{n-1}$ for SR / SR窗口是2^(n-1)
- ❌ "CSMA/CD used in Wi-Fi" → ✓ CSMA/CA for wireless / 无线用CSMA/CA

### Network Layer / 网络层
- ❌ "Fragment offset is in bytes" → ✓ In units of 8 bytes / 以8字节为单位！
- ❌ "255.255.255.255 is direct broadcast" → ✓ Limited broadcast (routers don't forward) / 受限广播
- ❌ "Routers change IP addresses" → ✓ Only NAT routers do / 只有NAT路由器改
- ❌ "RIP uses OSPF's algorithm" → ✓ RIP = DV; OSPF = LS / RIP距离向量，OSPF链路状态

### Transport Layer / 传输层
- ❌ "TCP seq numbers count segments" → ✓ Count bytes / 序号是按字节编号的！
- ❌ "ACK = received seq" → ✓ ACK = received seq + 1 (next expected) / ACK是期望的下一个序号
- ❌ "SYN has ACK flag set" → ✓ First SYN does NOT have ACK flag / 第一个SYN没有ACK标志
- ❌ "Fast recovery drops cwnd to 1" → ✓ Drops to cwnd/2 (not 1 — that's timeout behavior) / 快恢复降到一半

---

## 8. Key Numbers to Memorize / 关键数字速记

| Value / 数值 | Meaning / 含义 | 中文说明 |
|-------|---------|------|
| **64 bytes / 字节** | Minimum Ethernet frame / 以太网最短帧 | 小于64字节视为碰撞碎片丢弃 |
| **1518 bytes** | Maximum Ethernet frame / 以太网最长帧 | 1522 with VLAN / 含VLAN为1522 |
| **48 bits / 位** | MAC address length / MAC地址长度 | 前3字节OUI + 后3字节NIC特定 |
| **32 bits / 位** | IPv4 address length / IPv4地址长度 | 点分十进制表示 |
| **128 bits / 位** | IPv6 address length / IPv6地址长度 | 冒号十六进制表示 |
| **16 bits / 位** | Port number length / 端口号长度 | 0-65535 |
| **8 bytes / 字节** | IP fragment offset unit / IP分片偏移单位 | 片偏移值 × 8 = 实际字节偏移 |
| **20 bytes** | Minimum IP/TCP header / 最小IP/TCP头 | IP头20B, TCP头20B, UDP头8B |
| **1.544 Mbps** | T1 data rate / T1数据速率 | 193 bits / 125 μs |
| **51.84 Mbps** | STS-1 / OC-1 SONET rate | SONET基本速率 |
| **64 kbps** | PCM voice channel / PCM语音信道 | 8000 × 8 bits |
| **125 μs** | PCM sampling interval / PCM采样间隔 | T1帧时长 |
| **51.2 μs** | Ethernet slot time (10Mbps) / 以太网时隙 | 512 bit times at 10 Mbps |
| **9.6 μs** | Ethernet interframe gap / 以太网帧间间隔 | 96 bit times at 10 Mbps |
| **28 μs / 128 μs** | 802.11 SIFS / DIFS | 短帧间间隔 / DCF帧间间隔 |
| **3 duplicate ACKs** | Trigger TCP fast retransmit / 触发快重传 | 收到3个重复ACK立即重传 |
| **2 MSL** | TCP TIME_WAIT duration / TIME_WAIT时长 | 约2分钟 |
| **16 hops / 跳** | RIP infinity / RIP无穷大 | ≥16 = unreachable / 不可达 |
| **3 kHz** | Telephone voice bandwidth / 电话语音带宽 | 300-3300 Hz |
| **8000 Hz** | PCM sampling rate / PCM采样率 | 2 × 最高频率4kHz |
| **$3 \times 10^8$ m/s** | Speed of light (vacuum) / 光速(真空) | 真空中 |
| **$2 \times 10^8$ m/s** | Signal speed in copper/fiber / 铜线/光纤信号速度 | 约2/3光速 |

---

## 9. Big Question Self-Checklist / 大题自检清单 (19 Topics / 19个专题)

| # | Topic / 专题 | Key Step / 关键步骤 | 中文要点 | ✓ |
|:--:|-------|-------------------|------|:--:|
| 1 | Frame seq field design / 帧序号设计 | $W = 1 + \lfloor RTT/T_f \rfloor$, then protocol constraint | 求窗口→选协议约束→求位数 | |
| 2 | Switching delay / 交换延迟 | Circuit: $T_s+T_t+T_p$; Packet: pipeline model | 电路加建立时间，分组用流水线 | |
| 3 | Performance metrics / 性能指标 | Know all 4 delay types + utilization formula | 四种时延+利用率公式 | |
| 4 | Sliding window (GBN/SR) / 滑动窗口 | Window constraints, receiver behavior | GBN接收窗口=1, SR可缓存乱序 | |
| 5 | Gigabit Ethernet / 千兆以太网 | Carrier extension (to 512B), frame bursting | 载波扩展+帧突发 | |
| 6 | M/M/1 queuing / 排队模型 | $T = 1/(\mu-\lambda)$, $\rho = \lambda/\mu$ | 系统时间公式 | |
| 7 | Broadcast routing / 广播路由 | RPF: forward if on shortest path to source | RPF只看最短路径方向 | |
| 8 | Tunneling / 隧道 | Encapsulate original in new protocol header | 原报文做载荷封装 | |
| 9 | Leaky/Token bucket / 漏桶令牌桶 | Token: $t = b/(P-r)$; Leaky: constant rate | 令牌桶可突发，漏桶恒速 | |
| 10 | WFQ scheduling / 加权公平排队 | $F_i = \max(A_i, F_{i-1}) + L_i/W$ | 按完成轮数排序发送 | |
| 11 | Addresses in communication / 通信地址 | MAC changes/hop, IP & port unchanged | MAC每跳变，IP端口不变 | |
| 12 | Special IP addresses / 特殊IP | Memorize the 6-row table | 背6行特殊IP表 | |
| 13 | IP fragmentation / IP分片 | Offset in 8B units; MF=1 except last | 片偏移以8字节为单位！ | |
| 14 | NAT / 网络地址转换 | Translation table; NAT changes IP + port | NAT改IP+端口，普通路由不改 | |
| 15 | Router vs Switch / 路由vs交换 | L3 vs L2; broadcast domain difference | 路由器隔离广播域 | |
| 16 | Routing table / 路由表 | Longest prefix match / 最长前缀匹配 | 最具体匹配优先 | |
| 17 | RIP/OSPF/BGP | DV vs LS vs PV; UDP vs IP vs TCP | 算法类型+封装协议 | |
| 18 | TCP handshake/release / TCP握手挥手 | 3-way: SYN→SYN+ACK→ACK; 4-way: FIN→ACK→FIN→ACK | 序号变化规律 | |
| 19 | TCP congestion control / TCP拥塞控制 | Timeout→cwnd=1; 3dup→cwnd=cwnd/2+3 | 超时降到1，3重复ACK减半 | |
