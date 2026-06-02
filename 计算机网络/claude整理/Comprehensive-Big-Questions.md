# Comprehensive Big Questions: Complete Problem-Solving Guide / 大题专题完全解题指南

> **Purpose / 用途**: This document consolidates ALL 19 big-question topics identified from past exams, senior reviews, and instructor materials. Each topic includes: core knowledge, step-by-step solution methodology, worked examples, and variant identification. Use this as your primary resource for mastering the 50-point big question section.

> **中文**：本文档汇总了从历年真题、学长学姐复习资料和教师材料中识别出的全部19个大题专题。每个专题包含：核心知识点、分步解题方法、例题详解和题型变体识别。这是攻克50分大题部分的核心资料，请重点使用。

---

## Topic 1: Frame Sequence Number Field Length Design / 帧序号字段长度设计 ★★★

**When you see / 当你看到**: Given bandwidth, propagation delay/RTT, frame size. "Design a sliding window protocol." "Find minimum sequence number bits."

> **中文**：题目给出带宽、传播时延/RTT、帧大小，问"设计滑动窗口协议"或"求最少序号位数"。

### Core Knowledge / 核心知识

The fundamental question / 核心问题: How many bits does the sequence number field need so the sliding window can fully utilize the channel? 【序号字段需要多少位才能让滑动窗口充分利用信道？】

**Key metric / 关键指标**: How many frames can be "in the pipe" simultaneously? 【管道中最多能同时容纳多少帧？】

### Solution Template / 解题模板

**Step 1 / 第1步** — Calculate frame transmission time / 计算帧发送时间:
$$T_f = \frac{\text{Frame size (bits)}}{\text{Bandwidth (bps)}}$$
> 帧发送时间 = 帧大小（比特）/ 带宽（bps）

**Step 2 / 第2步** — Calculate how many frames fit in one RTT / 计算一个RTT内能发多少帧:
$$\text{Frames per RTT} = \frac{RTT}{T_f}$$

**Step 3 / 第3步** — Total frames that can be outstanding / 可同时在途的总帧数:
$$W_{total} = 1 + \lfloor \text{Frames per RTT} \rfloor$$
> 加1是因为当前正在发送的那一帧（第一帧发完时ACK还没回来，这段时间内又可以发后续帧）

**Step 4 / 第4步** — Determine sequence number bits based on protocol / 根据协议确定序号位数:

| Protocol / 协议 | Constraint / 约束 | Formula / 公式 |
|----------|-----------|---------|
| Go-Back-N / 后退N帧 | $W_T \le 2^n - 1$ | $n = \lceil \log_2(W_{total} + 1) \rceil$ |
| Selective Repeat / 选择重发 | $W_T \le 2^{n-1}$ | $n = \lceil \log_2(W_{total}) \rceil + 1$ |

> **中文解释**：GBN发送窗口最大为2^n-1（因为接收窗口=1，需要区分新旧帧）。SR发送窗口最大为2^(n-1)（因为发送和接收窗口独立滑动，序列空间需要"双倍缓冲"）。

### Worked Example 1 / 例题1

> 1.6 Mbps link, RTT = 45 ms, frame = 1 KB. Find sequence bits for maximum throughput with SR.
> 1.6Mbps链路，RTT=45ms，帧长=1KB。用SR协议，求最大吞吐量下最少序号位数。

$T_f = \frac{1 \times 1024 \times 8}{1.6 \times 10^6} = \frac{8192}{1,600,000} = 5.12 \text{ ms}$ 【帧发送时间】

Frames per RTT = $45 / 5.12 \approx 8.79$ 【一个RTT内能发8.79帧】

$W_{total} = 1 + \lfloor 8.79 \rfloor = 9$ frames 【窗口需容纳9帧】

For SR: $2^{n-1} \ge 9 \Rightarrow n-1 \ge \lceil \log_2 9 \rceil = 4 \Rightarrow n \ge 5$

**Answer / 答案: 5 bits / 5位**

### Worked Example 2 / 例题2 (Bandwidth-Delay Product Method / 带宽时延积法)

> 4 Mbps link, 5000 km distance, signal = $3 \times 10^8$ m/s, frame = 1 KB. Find minimum sequence bits for SR.
> 4Mbps链路，距离5000km，信号速度3×10⁸m/s，帧=1KB。SR最少序号位数。

$T_{prop} = \frac{5000 \times 10^3}{3 \times 10^8} = 16.67 \text{ ms}$ 【单程传播时延】

$RTT = 2 \times 16.67 = 33.33 \text{ ms}$ 【往返时延】

$T_f = \frac{8192}{4 \times 10^6} = 2.048 \text{ ms}$ 【帧发送时间】

Frames per RTT = $33.33 / 2.048 \approx 16.28$ 【一个RTT内能发约16帧】

$W_{total} = 1 + \lfloor 16.28 \rfloor = 17$ frames 【窗口需容纳17帧】

For SR: $2^{n-1} \ge 17 \Rightarrow n-1 \ge \lceil \log_2 17 \rceil = 5 \Rightarrow n \ge 6$

**Answer / 答案: 6 bits / 6位**

### Common Pitfalls / 常见错误

- Don't forget the "+1" — the frame currently in transmission / 别忘了"+1"——当前正在发送的那一帧
- When calculating $T_f$, convert KB to bits: multiply by 1024 × 8 / KB转bits要×1024×8
- For SR: the constraint is $2^{n-1}$, not $2^n - 1$ (that's GBN) / SR的约束是2^(n-1)，不是2^n-1

---

## Topic 2: Switching Technology Delay Comparison / 交换技术延迟对比 ★★★

**When you see / 当你看到**: "Compare the delay of circuit switching, message switching, and packet switching for sending a file."
> 题目要求比较电路交换、报文交换和分组交换发送文件的延迟。

### Core Formulas / 核心公式

**Circuit Switching / 电路交换**:
$$T_{circuit} = T_{setup} + T_{trans} + T_{prop}$$

> **中文**：电路交换总时延 = 建立时间 + 发送时延 + 传播时延。建立后数据沿专用路径传输，无逐跳存储转发。

**Packet Switching / 分组交换** (store-and-forward pipeline / 存储转发流水线):
$$\boxed{T_{packet} = (m + n - 1) \times \frac{x}{b} + D_{prop}}$$

> **中文**：m=分组数，n=跳数，x=分组大小(bit)，b=带宽(bps)。第一个包需n次传输穿越网络，之后每个包隔x/b时间到达。

### Worked Example / 例题

> Send 1 MB file. Link = 1 Mbps, propagation = 10 ms. Packet size = 1000 B (20 B header). Hops = 4. Compare circuit (100 ms setup) vs packet switching.
> 发送1MB文件，链路1Mbps，传播时延10ms。分组1000B（20B头部），4跳。比较电路交换（建立100ms）和分组交换。

**Circuit switching / 电路交换**:
$T = 0.1 + \frac{10^6 \times 8}{10^6} + 0.01 = 0.1 + 8 + 0.01 = 8.11 \text{ s}$

**Packet switching / 分组交换**:
- Payload per packet / 每包载荷 = 1000 - 20 = 980 B
- Packets / 分组数 = $\lceil 10^6 / 980 \rceil = 1021$
- Packet transmission time / 每包发送时间: 8000 / 10⁶ = 8 ms
- First packet through 4 hops / 首包过4跳: 4 × 8 = 32 ms
- Trailing 1020 packets / 后续1020包: 1020 × 8 = 8160 ms
- Plus propagation / 加传播: 10 ms
- Total / 总计 ≈ 32 + 8160 + 10 = **8.20 s**

---

## Topic 3: Network Performance Metrics / 网络性能指标 ★★

参见 Ch00 Section 2.1 部分的详细解释。核心公式：

- **Transmission delay / 发送时延**: $T_{trans} = \text{Data size} / \text{Bandwidth}$
- **Propagation delay / 传播时延**: $T_{prop} = \text{Distance} / \text{Propagation speed}$
- **Bandwidth-delay product / 带宽时延积**: $BDP = \text{Bandwidth} \times RTT$
- **Utilization (stop-and-wait) / 利用率（停等）**: $U = \frac{T_D}{T_D + RTT + T_A}$

---

## Topic 4: Data Link Layer Sliding Window — GBN and SR / 滑动窗口GBN和SR ★★★

### Key Difference Summary / 核心区别总结

| | Go-Back-N / 后退N帧 | Selective Repeat / 选择重发 |
|--|:---:|:---:|
| Sender window / 发送窗口 | $\le 2^n - 1$ | $\le 2^{n-1}$ |
| Receiver window / 接收窗口 | $= 1$ | $\le 2^{n-1}$ |
| Out-of-order frames / 乱序帧 | **Discarded / 丢弃** | **Buffered / 缓存** |
| On error / 出错时 | Retransmit ALL unacked / 重发所有未确认帧 | Retransmit ONLY lost frame / 只重发丢失的那一帧 |
| ACK type / 确认类型 | Cumulative / 累积确认 | Individual / 逐个确认 |
| NAK used? / 使用NAK? | No | Optional (optimization) / 可选（优化手段） |

> **中文核心要点**：GBN的接收方只盯着一个序号，其他全部丢弃，保证了有序到达。SR的接收方可以缓存乱序帧，等整个窗口收齐后一起上交。

---

## Topic 5: Gigabit Ethernet Design / 千兆以太网设计 ★

**Problem / 问题**: At 1 Gbps, the standard 64-byte minimum frame is too short for collision detection 【在1Gbps速率下，标准64字节最短帧太短了，无法检测碰撞】.

**Solutions / 解决方案**:
1. **Carrier Extension / 载波扩展**: Artificially extend transmission to 512 bytes (4096 bit times) by padding 【通过填充将传输时间人为扩展到512字节】
2. **Frame Bursting / 帧突发**: After first frame, send consecutive frames without contending 【第一个帧后连续发送多个帧，无需每次竞争信道】

---

## Topic 6: M/M/1 Queuing Model / M/M/1排队模型 ★★★

### Core Formulas / 核心公式

$$\rho = \frac{\lambda}{\mu} \quad \text{(utilization / 利用率, must be < 1)}$$

$$\boxed{T = \frac{1}{\mu - \lambda}} \quad \text{(average time in system / 系统平均时间)}$$

$$N = \frac{\rho}{1 - \rho} = \frac{\lambda}{\mu - \lambda} \quad \text{(avg number in system / 系统平均人数)}$$

> **中文**：λ = 到达率（包/秒），μ = 服务率（包/秒），ρ = 利用率，T = 平均系统时间（排队+服务）

### Worked Example / 例题

> $\lambda = 5$ frames/s, $\mu = 10$ frames/s.
> **中文**：到达率λ=5帧/秒，服务率μ=10帧/秒。
> $\rho = 5/10 = 0.5$, $T = 1/(10-5) = \textbf{0.2 seconds}$
> **中文**：利用率ρ = 5/10 = 0.5，平均系统时间 T = 1/(10-5) = **0.2秒**

### FDM Analysis / FDM分析

If channel split into $N$ FDM subchannels:
$$T_{FDM} = \frac{N}{\mu C - \lambda} = N \times T$$

> **中文**：将信道分为N份后，平均延迟变为原来的N倍。这说明FDM不能有效处理通信的突发性。

---

## Topic 7: Broadcast Routing — Packet Count / 广播路由分组计数 ★★★

### Counting Rules / 计数规则

**Flooding / 泛洪法**: For each router receiving on $k$ of its $n$ ports:
- If $k = 1$: creates $n-1$ new packets / 产生n-1个新分组
- If $k \ge 2$: creates $n$ new packets in some counting models

**Reverse Path Forwarding (RPF) / 逆向路径转发**: Forward only if packet arrived on the link that's on the shortest path back to the source 【只有从最短路径方向来的分组才转发】. Produces fewer packets than flooding. 【产生的分组比泛洪法少】

**Spanning Tree / 生成树广播**: Exactly the number of edges in the spanning tree. This is the **minimum** possible 【最小分组数】.

---

## Topic 8: Tunneling / 隧道技术 ★

**Core Concept / 核心概念**: Tunneling encapsulates a packet of one protocol inside a packet of another protocol 【隧道是将一种协议的数据包封装在另一种协议的数据包中】.

**Key requirement / 关键条件**: At least two dual-protocol-stack routers at the tunnel endpoints 【隧道两端至少需要两个双协议栈路由器】.

> **中文**：例如IPv6数据包穿越IPv4网络时，IPv6数据包被封装成IPv4数据包的载荷。隧道入口路由器添加IPv4头，出口路由器剥离IPv4头还原IPv6数据包。

---

## Topic 9: Traffic Shaping — Leaky Bucket & Token Bucket / 漏桶与令牌桶 ★★★

### Leaky Bucket / 漏桶

**Constant output rate / 恒定输出速率**. Converts bursty input to smooth output 【将突发流量平滑化】.

> **中文**：漏桶的核心是"漏"——桶底有一个恒定速率的小孔，不管水（分组）怎么涌入，输出的速率是恒定的。如果桶满了，新来的分组被丢弃。

### Token Bucket / 令牌桶

Tokens arrive at rate $r / 令牌以速率r到达$, bucket holds max $b$ tokens / 桶最多存b个令牌. Each byte costs 1 token / 每个字节消耗一个令牌.

**Max burst duration at peak rate / 峰值速率下最大突发时间** $P$:
$$t = \frac{b}{P - r}$$

**Max sustained rate / 最大持续速率** = $r$

> **中文**：令牌桶的核心是"令牌"——桶不漏底，令牌以恒定速率产生。要发送数据必须消耗等量令牌。这允许了突发流量（桶里存的令牌可以一次性用完），但长期平均速率被令牌产生速率限制。

### Worked Example / 例题

> Token fill rate = 1 Mbps, bucket = 8 MB. Peak rate = 6 Mbps. Max burst time?
> **中文**：令牌填充速率 = 1 Mbps，桶容量 = 8 MB，峰值速率 = 6 Mbps。求最大突发时间？
> $b + r \cdot t = P \cdot t \Rightarrow 64 \text{ Mb} + 1 \cdot t = 6 \cdot t \Rightarrow t = 12.8 \text{ s}$
> **中文**：公式 b + r·t = P·t → 64 Mb + 1·t = 6·t → 解得 t = 12.8 秒

---

## Topic 10: WFQ Packet Scheduling / 加权公平排队 ★★★

### Algorithm / 算法

$$F_i = \max(A_i, F_{i-1}) + \frac{L_i}{W}$$

> **中文**：$F_i$ = 完成时间/轮数, $A_i$ = 到达时间, $F_{i-1}$ = 同队列前一包的完成时间, $L_i$ = 包长度（字节）, $W$ = 队列权重

Packets sorted by $F_i$ (ascending). Ties broken by arrival order. 【按Fi升序发送，平局按到达先后。】

### Worked Example / 例题

> Three queues: A(W=1), B(W=1), C(W=2). Packets:
> **中文**：三个队列：A（权重=1），B（权重=1），C（权重=2）。数据包如下：

| Pkt / 包 | Queue / 队列 | Arrival / 到达时间 | Length / 长度 |
|-----|:-----:|:-----:|:---:|
| A | A | 0 | 8 |
| B | B | 5 | 6 |
| C | C | 5 | 10 |
| D | A | 8 | 9 |
| E | C | 8 | 8 |

**Calculations / 计算**:

| Pkt / 包 | $F_i$ | Calculation / 计算过程 |
|-----|:---:|------|
| A | $\max(0,0) + 8/1 = 8$ | 队列A首包 |
| C | $\max(5,0) + 10/2 = 10$ | 队列C首包，W=2使长度减半 |
| B | $\max(5,0) + 6/1 = 11$ | 队列B首包 |
| E | $\max(8,10) + 8/2 = 14$ | C的F_{i-1}=10 |
| D | $\max(8,8) + 9/1 = 17$ | A的F_{i-1}=8 |

**Transmission order / 发送顺序**: A(8) → C(10) → B(11) → E(14) → D(17)

---

## Topic 13: IP Fragmentation and Reassembly / IP分片与重组 ★★★

### Key Points / 关键要点

- **Fragment offset / 片偏移**: measured in **8-byte (64-bit) units**【以8字节为单位】
- Data length in each fragment must be divisible by 8 【每个分片的数据长度必须是8的倍数】
- **MF = 1**: more fragments follow / 还有后续分片; **MF = 0**: last fragment / 最后一个分片
- **All fragments share the same Identification number【所有分片共享同一标识号】**

### Worked Example / 例题

> 3800 B UDP + 20 B IP header = 3820 B datagram. MTU = 1420 B.
> 3800字节UDP数据+20字节IP头=3820字节IP数据报，MTU=1420字节。

Max data per fragment = 1420 - 20 = 1400 (already multiple of 8 ✓)
Number of fragments = $\lceil 3800 / 1400 \rceil = 3$

| Fragment | Total Len / 总长 | Data Len / 数据长 | MF | Offset / 偏移(8B单位) |
|----------|:---:|:---:|:---:|:---:|
| 1 | 1420 | 1400 | 1 | 0 |
| 2 | 1420 | 1400 | 1 | 175 (=1400/8) |
| 3 | 1020 | 1000 | 0 | 350 (=2800/8) |

> 验证：1400 + 1400 + 1000 = 3800 ✓

---

## Topic 18: TCP 3-Way Handshake & 4-Way Release / TCP三次握手和四次挥手 ★★★

### 3-Way Handshake / 三次握手

```
Client / 客户端               Server / 服务器
SYN, seq=x         →
                   ←     SYN+ACK, seq=y, ack=x+1
ACK, seq=x+1, ack=y+1 →
```

> **中文解释**：第1步客户端发送SYN告诉服务器"我想建立连接"；第2步服务器回复SYN+ACK表示"我收到了，我也同意"；第3步客户端发送ACK表示"我知道你同意了"。这就是为什么需要3次而不是2次——第3次ACK是客户端告诉服务器"我知道你同意建立连接了"，防止旧的连接请求造成混乱。

### 4-Way Release / 四次挥手

```
Host1                            Host2
FIN, seq=u                →
                          ←     ACK, seq=v, ack=u+1
                          ←     FIN, seq=w, ack=u+1
ACK, seq=u+1, ack=w+1    →
```

> **中文解释**：为什么释放需要4次而不是3次？因为TCP释放是**对称的**——"你关你的方向，我关我的方向"。服务器收到客户端的FIN后，可能还有数据要发给客户端（路上还有数据在传输），所以ACK和FIN是分开发的。ACK告诉客户端"我知道你不发了"，FIN告诉客户端"我也不发了"。

**TIME_WAIT state / TIME_WAIT状态**: After sending final ACK, wait **2 × MSL** (typically 2 minutes). Ensures: (1) Final ACK retransmission if lost; (2) All stray packets die off.

> **中文**：客户端发送最后一个ACK后进入TIME_WAIT状态等待2个MSL（最大段生存时间，通常2分钟）。目的：(1)如果最后ACK丢失，服务器会重发FIN，客户端可以再ACK；(2)确保这个连接的所有迷路数据包在网络中消失。

---

## Topic 19: TCP Congestion Control / TCP拥塞控制 ★★★

### Algorithm State Machine / 算法状态机

1. **Slow Start / 慢开始**: cwnd doubles every RTT / 每RTT翻倍 (exponential / 指数增长). Exit when cwnd ≥ ssthresh.

2. **Congestion Avoidance / 拥塞避免**: cwnd += 1 per RTT / 每RTT加1 (linear / 线性增长). AIMD principle.

3. **On timeout / 超时发生时**: ssthresh = cwnd/2, cwnd = 1, go to Slow Start.

4. **On 3 dup ACKs / 收到3个重复ACK时**: Fast Retransmit + Fast Recovery. ssthresh = cwnd/2, cwnd = ssthresh + 3.

> **中文核心**：超时（真正丢包）→ 降到1重启慢开始。3个重复ACK（疑似丢包但网络还好）→ 减半然后快恢复，不降到1。这就是"快"和"慢"的区别。

### Congestion Window Graph Drawing Tips / 画图要点

- Slow start = steep curve (exponential) / 陡峭上升（指数）
- Congestion avoidance = gentle slope (linear) / 平缓上升（线性）
- Timeout = cliff drop to cwnd=1 / 断崖式降到1
- Fast recovery = drop to half, not to 1 / 降到一半不是1

---

## Formula Quick Reference / 公式速查（含中文）

| Topic / 专题 | Formula / 公式 | 中文名称 |
|-------|---------|------|
| Nyquist (noiseless) / 奈奎斯特 | $C = 2H \log_2 V$ | 无噪声信道最大速率 |
| Shannon (noisy) / 香农 | $C = H \log_2(1 + S/N)$ | 有噪声信道最大速率 |
| dB to ratio / dB转比值 | $S/N = 10^{dB/10}$ | 信噪比分贝转换 |
| Transmission delay / 发送时延 | $T_{trans} = L / B$ | 数据长/带宽 |
| Min frame (CSMA/CD) / 最短帧长 | $L_{min} = 2\tau \times B$ | 2倍传播时延×带宽 |
| Stop-and-wait U / 停等利用率 | $U = T_D / (T_D + RTT)$ | 发送时延/(发送+往返) |
| M/M/1 delay / 排队延迟 | $T = 1/(\mu - \lambda)$ | 平均系统时间 |
| WFQ finish number | $F_i = \max(A_i, F_{i-1}) + L_i/W$ | 加权公平排队完成时间 |
| Token bucket burst / 令牌桶突发 | $t = b / (P - r)$ | 最大突发时间 |
| GBN max window / GBN最大窗口 | $W_T \le 2^n - 1$ | n位序号最大发送窗口 |
| SR max window / SR最大窗口 | $W_T \le 2^{n-1}$ | n位序号最大发送窗口 |
| Fragment offset / 分片偏移 | Offset(8B) = prev_sum / 8 | 片偏移(以8字节为单位) |
| RTO / 重传超时 | $RTO = EstRTT + 4 \times DevRTT$ | TCP重传超时计算 |

---

## Chinese Topic Reference / 中文专题速查

| # | 专题 | 核心公式/要点 |
|:--:|------|------|
| 1 | 帧序号字段长度设计 | $W = 1 + \lfloor RTT/T_f \rfloor$, SR: $n = \lceil \log_2 W \rceil + 1$ |
| 2 | 交换技术延迟对比 | 电路: $T_{setup}+T_{trans}+T_{prop}$; 分组: 流水线模型 |
| 3 | 网络性能指标 | $T_{total}=T_{trans}+T_{prop}+T_{proc}+T_{queue}$ |
| 4 | 滑动窗口 GBN vs SR | GBN: $W_T \le 2^n-1, W_R=1$; SR: $W_T \le 2^{n-1}$ |
| 5 | 千兆以太网设计 | 载波扩展到512字节，帧突发 |
| 6 | M/M/1排队模型 | $T=1/(\mu-\lambda)$, $\rho=\lambda/\mu$, $N=\rho/(1-\rho)$ |
| 7 | 广播路由分组计数 | RPF: 来自最短路径方向才转发 |
| 8 | 隧道技术 | 原报文作为新协议载荷封装 |
| 9 | 漏桶/令牌桶 | 漏桶恒速；令牌桶: $t=b/(P-r)$ |
| 10 | WFQ分组调度 | $F_i=\max(A_i,F_{i-1})+L_i/W$ |
| 11 | 通信中地址使用 | MAC每跳变化，IP和端口不变 |
| 12 | 特殊IP地址 | 直接广播/受限广播/环回/本主机 |
| 13 | IP分片与重组 | 片偏移单位8字节；MF=1（最后为0）|
| 14 | NAT | NAT修改IP+端口号；普通路由器不修改 |
| 15 | 路由器 vs 交换机 | 路由器隔离广播域(L3)，交换机不能(L2) |
| 16 | 路由表与路由选择 | 最长前缀匹配 |
| 17 | RIP/OSPF/BGP | RIP=DV+UDP, OSPF=LS+IP, BGP=PV+TCP |
| 18 | TCP三次握手/四次挥手 | 3次: SYN→SYN+ACK→ACK; 4次: FIN→ACK→FIN→ACK |
| 19 | TCP拥塞控制 | 超时→cwnd=1; 3重复ACK→cwnd=cwnd/2+3 |
