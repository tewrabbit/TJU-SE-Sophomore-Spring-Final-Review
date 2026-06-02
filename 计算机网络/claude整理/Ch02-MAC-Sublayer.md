# Chapter 02: MAC Sublayer (Medium Access Control) / 第二章：MAC子层（介质访问控制）

> **Exam Relevance / 考试重点**: The MAC sublayer is a core exam area. Key topics include CSMA/CD (especially minimum frame length calculation), CSMA/CA (why CD fails in wireless), Ethernet frame structure, switch forwarding behavior, and VLAN concepts. The minimum frame length calculation and CSMA/CD backoff algorithm are classic big questions.
>
> **中文**：MAC子层是考试核心区域。关键主题包括CSMA/CD（特别是最小帧长计算）、CSMA/CA（为什么CD在无线中失效）、以太网帧结构、交换机转发行为以及VLAN概念。最小帧长计算和CSMA/CD退避算法是经典大题。

---

## 2.1 The Channel Allocation Problem / 信道分配问题

The fundamental question: How do multiple stations **share** a single broadcast channel?

> **中文**：基本问题：多个站点如何**共享**单个广播信道？

### 2.1.1 Static vs Dynamic Allocation / 静态 vs 动态分配

| Allocation Type / 分配类型 | Static (FDM/TDM) / 静态（频分复用/时分复用） | Dynamic / 动态 |
|---------------------------|----------------------------------------------|----------------|
| **When good / 何时适用** | Constant, predictable load with many users / 大量用户、恒定可预测负载 | Bursty, unpredictable traffic / 突发、不可预测流量 |
| **When bad / 何时不适用** | Few users or bursty traffic — wastes allocated but unused bandwidth / 用户少或流量突发——浪费已分配但未使用的带宽 | Adds protocol overhead / 增加协议开销 |
| **Key problem / 关键问题** | If N users are allocated 1/N each, a user with no data to send still holds its slot / 若N个用户各分配1/N，无数据发送的用户仍占用其时隙 | Must handle collisions when multiple users transmit at once / 多用户同时发送时必须处理碰撞 |

### 2.1.2 Assumptions for Dynamic Channel Allocation / 动态信道分配的假设

1. **Station model / 站点模型**: N independent stations generating frames / N个独立站点生成帧
2. **Single channel / 单信道**: All stations share one channel / 所有站点共享一个信道
3. **Collision / 碰撞**: If two frames overlap in time, both are garbled (collision) / 若两帧在时间上重叠，则两者都被破坏（碰撞）
4. **Time / 时间**: Can be continuous or slotted / 可以是连续或分时隙的
5. **Carrier sense / 载波监听**: Stations may or may not be able to sense the channel before transmitting / 站点在发送前可能能够也可能不能感知信道

### 2.1.3 Contention Systems Key Metrics / 竞争系统的关键指标

- **Throughput (S) / 吞吐量**: Fraction of channel capacity actually used for successful transmission ($0 \le S \le 1$) / 实际用于成功传输的信道容量比例
- **Offered load (G) / 提供的负载**: Total rate of frames presented to the channel, including retransmissions / 呈现给信道的帧总速率，包括重传

> **中文**：吞吐量(S)衡量信道容量的实际利用率；提供的负载(G)表示站点试图发送的总流量（包括重传）。这两个参数是分析竞争协议性能的基础。

---

## 2.2 Multiple Access Protocols / 多路访问协议

### 2.2.1 ALOHA / ALOHA协议

#### Pure ALOHA / 纯ALOHA

The most basic contention protocol. When a station has a frame to send, it transmits **immediately**. If a collision occurs, it waits a **random time** and retransmits.

> **中文**：最基本的竞争协议。当站点有帧要发送时，它**立即**发送。如果发生碰撞，则等待一个**随机时间**后重传。

**Vulnerability period / 脆弱期**: $2T$ (where $T$ = frame transmission time / 其中$T$ = 帧传输时间). A frame can collide with frames starting within $T$ seconds before or after its own start.

> **中文**：脆弱期为$2T$（$T$=帧传输时间）。一帧可能与在其开始前后$T$秒内开始的其他帧发生碰撞。

**Throughput analysis / 吞吐量分析** (中文说明：泊松到达，无限用户模型):
- Poisson arrival, infinite population / 泊松到达，无限用户
- $S = G \cdot e^{-2G}$
- Maximum throughput / 最大吞吐量: $S_{max} = \frac{1}{2e} \approx \boxed{18.4\%}$ at $G = 0.5$

This means pure ALOHA can utilize at most ~18% of the channel capacity.

> **中文**：这意味着纯ALOHA最多只能利用约18%的信道容量。效率很低。

#### Slotted ALOHA / 时隙ALOHA

Improvement: Divide time into discrete **slots** of length $T$ (one frame time). Stations may only transmit at the **beginning** of a slot. This halves the vulnerability period to $T$.

> **中文**：改进：将时间划分为长度为$T$（一帧时间）的离散**时隙**。站点只能在时隙的**开始**时刻发送。这将脆弱期减半为$T$。

**Throughput analysis / 吞吐量分析** (中文说明：时隙ALOHA的吞吐量公式):
- $S = G \cdot e^{-G}$
- Maximum throughput / 最大吞吐量: $S_{max} = \frac{1}{e} \approx \boxed{36.8\%}$ at $G = 1$

**Slotted ALOHA doubles the throughput** of pure ALOHA. It shows that **synchronization** is a powerful tool.

> **中文**：**时隙ALOHA使吞吐量翻倍**，相比纯ALOHA。这表明**同步**是一种强大的工具。关键区别在于纯ALOHA脆弱期为$2T$，而时隙ALOHA仅为$T$，两者吞吐量公式分别为$Ge^{-2G}$和$Ge^{-G}$。

### 2.2.2 CSMA (Carrier Sense Multiple Access) / CSMA（载波监听多路访问）

Before transmitting, a station **listens** to the channel. If the channel is **busy**, it waits. This dramatically reduces collisions.

> **中文**：发送前，站点**监听**信道。如果信道**忙**，则等待。这大大减少了碰撞。

#### Three Varieties of CSMA / CSMA的三种变体

| Type / 类型 | Behavior When Channel Busy / 信道忙时的行为 | Behavior When Idle / 信道空闲时的行为 | Throughput / 吞吐量 |
|------------|---------------------------------------------|--------------------------------------|--------------------|
| **1-persistent / 1-坚持** | Continuously listen; transmit immediately when idle / 持续监听；空闲立即发送 | Transmit immediately / 立即发送 | High collision risk when multiple stations are waiting / 多站等待时碰撞风险高 |
| **Nonpersistent / 非坚持** | Wait a random time before sensing again / 等待随机时间后再次监听 | Transmit immediately / 立即发送 | Better utilization but longer delays / 利用率更高但延迟更长 |
| **p-persistent / p-坚持** | Applies to slotted channels: when idle, transmit with probability $p$, defer to next slot with probability $1-p$ / 适用于时隙信道：空闲时以概率$p$发送，以概率$1-p$推迟到下一时隙 | As described / 如上所述 | $p$ can be tuned to optimize throughput / $p$可调以优化吞吐量 |

**Key insight / 关键理解**: 1-persistent has highest collision probability because all waiting stations pounce simultaneously. Smaller $p$ means fewer collisions but higher delay.

> **中文**：1-坚持的碰撞概率最高，因为所有等待的站点同时抢占。较小的$p$意味着较少碰撞但较高延迟。

### 2.2.3 CSMA/CD (with Collision Detection) ★★★ / CSMA/CD（带碰撞检测）★★★

**This is the protocol used by classic Ethernet.** The key insight: detect collisions **while** transmitting, and abort immediately upon detection, saving time.

> **中文**：**这是经典以太网所使用的协议。** 关键思路：在发送**过程中**检测碰撞，一旦检测到立即中止，从而节省时间。

**Full Name / 全称**: **C**arrier **S**ense **M**ultiple **A**ccess / **C**ollision **D**etection【载波监听多路访问/碰撞检测】

**The Three-Step Process / 三步过程**:
1. **Carrier Sense (先听后说)【载波监听】**: Before sending, listen to the channel. If idle for 96 bit times (interframe gap【帧间间隔】), start sending. If busy, wait.
2. **Collision Detection (边说边听)【碰撞检测】**: While sending, continuously monitor the channel. Compare the signal on the wire to what you're sending. If they differ → collision!
3. **Backoff (碰撞后退避)【退避】**: On collision, stop transmitting immediately. Send a jam signal【阻塞信号】. Wait a random time, then retry.

> **中文**：CSMA/CD的三步过程：(1)先听后说——发送前先监听信道，若空闲96比特时间（帧间间隔）则开始发送，若忙则等待；(2)边说边听——发送时持续监控信道，比较线缆上的信号与正在发送的信号，若不一致则发生碰撞；(3)碰撞后退避——碰撞后立即停止发送，发送阻塞信号，等待随机时间后重试。

**Why both CSMA AND CD? / 为什么既要CSMA又要CD？**
- CSMA alone **reduces the number of collisions** (by not starting when channel is busy) / 单独CSMA**减少碰撞数量**（通过在信道忙时不开始发送）
- CD alone **reduces the damage from collisions** (by aborting early rather than completing the full transmission) / 单独CD**减少碰撞造成的损害**（通过提前中止而非完成整个传输）
- Together: CSMA/CD reduces the **total impact** of collisions to an acceptable level / 两者结合：CSMA/CD将碰撞的**总影响**降低到可接受的水平

### 2.2.4 CSMA/CD Timing Analysis ★★★ (Big Question Territory) / CSMA/CD时序分析 ★★★（大题区域）

#### Contention Period (争用期 / Collision Window) / 争用期（碰撞窗口）

$2\tau$ = round-trip propagation delay of the network (end-to-end time × 2). This is the **worst-case time** for a station to detect a collision.

> **中文**：$2\tau$ = 网络往返传播时延（端到端时间 × 2）。这是站点检测到碰撞的**最坏情况时间**。

**Why $2\tau$? / 为什么是$2\tau$？** Imagine Station A starts sending at $t=0$. The signal takes time $\tau$ to reach Station B. Just before the signal arrives, B senses the channel as idle and starts sending. At that moment, a collision occurs. But A won't know about it until B's signal travels back — at time $t \approx 2\tau$. So A must still be sending at $t=2\tau$ to detect the collision.

> **中文**：为什么是$2\tau$？假设站A在$t=0$时开始发送。信号到达站B需要时间$\tau$。恰好在信号到达之前，B感知信道为空闲并开始发送。此时发生碰撞。但A直到B的信号传回才会知道——在$t \approx 2\tau$时刻。所以A在$t=2\tau$时必须在仍在发送中才能检测到碰撞。

#### Minimum Frame Length (最短帧长) ★★★

The frame must be long enough that the **sender is still transmitting when the collision signal returns**. Otherwise, the sender will have finished and won't respond to the collision.

> **中文**：帧必须足够长，使得**当碰撞信号返回时发送者仍在传输中**。否则发送者已发送完毕，不会对碰撞做出响应。

$$\boxed{\text{Min frame length / 最短帧长} = 2\tau \times \text{Bandwidth / 带宽}}$$

(中文说明：最短帧长 = 往返传播时延 × 带宽)

Or equivalently / 或等价地:

$$\boxed{\text{Min frame length (bits) / 最短帧长（比特）} = 2 \times \frac{\text{Distance / 距离}}{\text{Prop. speed / 传播速度}} \times \text{Bandwidth / 带宽}}$$

**Ethernet example / 以太网示例**: 2500m cable, 10 Mbps, $2 \times 10^8$ m/s propagation speed:
- $\tau = 2500 / (2 \times 10^8) = 12.5 \text{ μs}$
- $2\tau = 25 \text{ μs}$
- Min bits / 最小比特数 = $2\tau \times 10^7 = 250$ bits... but Ethernet uses **512 bits (64 bytes)** as the minimum, which corresponds to a 51.2 μs contention period at 10 Mbps

> **中文**：以太网示例：2500米线缆，10 Mbps，传播速度$2 \times 10^8$ m/s。计算得最小比特数为250比特，但以太网实际使用**512比特（64字节）**作为最小值，这对应于10 Mbps下51.2微秒的争用期。这说明规范考虑了中继器延迟等因素。

**Standard Ethernet values / 标准以太网值** (Must memorize / 必须牢记):

| Parameter / 参数 | Value / 值 |
|-----------------|------------|
| Minimum frame length / 最短帧长 | **64 bytes** (512 bits) / **64字节**（512比特） |
| Contention period / 争用期 | **51.2 μs** (at 10 Mbps) / **51.2微秒**（在10 Mbps下） |
| Maximum frame length (802.3) / 最大帧长（802.3） | **1518 bytes** (1500 data + 18 header/trailer) / **1518字节**（1500数据 + 18首部/尾部） |
| Maximum frame with VLAN (802.1Q) / 带VLAN的最大帧长（802.1Q） | **1522 bytes** (+4 bytes VLAN tag) / **1522字节**（+4字节VLAN标签） |
| Slot time / 时隙时间 | **512 bit times / 512比特时间** |

Any frame shorter than 64 bytes is considered a **collision fragment** and is discarded.

> **中文**：任何短于64字节的帧被视为**碰撞碎片**并被丢弃。

#### Truncated Binary Exponential Backoff (二进制指数退避算法) ★

After collision $n$ (where $n$ = retransmission count, starting at 1) / 第$n$次碰撞后（$n$ = 重传次数，从1开始）:

$$\boxed{\text{Backoff time / 退避时间} = \text{Slot time / 时隙时间} \times \text{random}[0, 2^k - 1]}$$

Where $k = \min(n, 10)$ / 其中$k = \min(n, 10)$

(中文说明：退避时间 = 时隙时间 × [0, 2^k-1]之间的随机整数)

- Each retransmission doubles the possible wait range (up to $k=10$) / 每次重传使可能的等待范围加倍（最多到$k=10$）
- After 16 retransmissions, the frame is **discarded** and the upper layer is notified / 16次重传后，帧被**丢弃**并通知上层
- This ensures stability under heavy load / 这确保了重负载下的稳定性

**Example / 示例**: After 3 collisions / 3次碰撞后:
- $k = \min(3, 10) = 3$
- Range of random number / 随机数范围: $[0, 2^3 - 1] = [0, 7]$
- Wait time / 等待时间 = random × 51.2 μs (between 0 and 358.4 μs / 在0到358.4微秒之间)

**Exponential Backoff Detailed Procedure / 指数退避详细过程**:

> **中文**：以下展示了每次碰撞后K值的选择范围如何指数增长：

| Collision count / 碰撞次数 | K chosen from / K选择范围 | Wait time range / 等待时间范围 |
|:---:|:---:|:---:|
| 1st | {0, 1} | 0 ~ 51.2 μs |
| 2nd | {0, 1, 2, 3} | 0 ~ 153.6 μs |
| 3rd | {0, 1, ..., 7} | 0 ~ 358.4 μs |
| ... | ... | ... |
| 10th | {0, 1, ..., 1023} | 0 ~ 52.4 ms |
| 16th | **Report failure / 报告失败** | Frame discarded / 帧被丢弃 |

**Three States of CSMA/CD Systems / CSMA/CD系统的三种状态**:

> **中文**：CSMA/CD系统在运行时处于三种状态之一：

1. **Idle / 空闲**: Channel is unused, stations are waiting for data to send / 信道未被使用，站点等待发送数据
2. **Contention / 竞争**: Multiple stations attempt to transmit simultaneously, collisions may occur / 多个站点同时尝试发送，可能发生碰撞
3. **Transmission / 传输**: A station has successfully acquired the channel and is transmitting data / 某站点成功获取信道并正在传输数据

**Collision Detection Time / 碰撞检测时间**:
- **Minimum**: propagation delay $\tau$ (when two stations are very close) / 最小为传播时延$\tau$（两站点非常近时）
- **Maximum**: round-trip propagation delay $2\tau$ (worst-case scenario, the standard contention period) / 最大为往返传播时延$2\tau$（最坏情况，即标准争用期）

**Jam Signal (阻塞信号) 和 Bit Time (比特时间)**:

> **中文**：当检测到碰撞时，站点中止发送并发送阻塞信号。比特时间是传输1比特所需的时间。

- **Jam Signal**: 48 bits long, sent after collision detection to ensure all other transmitters are aware of the collision / **阻塞信号**为48比特长，在检测到碰撞后发送，以确保所有其他发送方都知道发生了碰撞
- **Bit Time**: The time to transmit 1 bit on the medium. E.g., at 10 Mbps Ethernet, bit time = $1/10^7 = 0.1$ μs / **比特时间**是在介质上传输1比特所需的时间。例如10 Mbps以太网的比特时间为0.1微秒

### 2.2.5 CSMA/CA (Collision Avoidance) ★★★ / CSMA/CA（碰撞避免）★★★

**Used in wireless LANs (802.11)**. Why not CSMA/CD in wireless?

> **中文**：**用于无线局域网（802.11）**。为什么无线中不用CSMA/CD？

**The fundamental problems with CD in wireless / CD在无线中的根本问题**:

1. **Hidden Terminal Problem (隐藏终端问题)**: Station A is sending to B, Station C wants to send to B. C is out of A's range, so C cannot hear A's transmission. C senses the channel idle, transmits, and collides with A's transmission at B.

> **中文**：**隐藏终端问题**：站A正在向B发送，站C想要向B发送。C在A的范围之外，所以C听不到A的传输。C感知信道空闲，开始发送，在B处与A的传输发生碰撞。

```
   A ─ ─ ─ → B ← ─ ─ ─ C
   (A can't hear C, C can't hear A)
   (A听不到C，C听不到A)
```

2. **Exposed Terminal Problem (暴露终端问题)**: B is sending to A. C wants to send to D. C hears B's transmission and defers, but C's transmission would NOT interfere with A's reception (A and C are far apart). C is needlessly blocked.

> **中文**：**暴露终端问题**：B正在向A发送。C想要向D发送。C听到B的传输而推迟，但C的传输不会干扰A的接收（A和C相距很远）。C被不必要地阻塞了。

```
   A ←─ ─ ─ B          C ─ ─ ─ → D
   (C hears B but wouldn't interfere with A)
   (C听到B但不会干扰A)
```

3. **Can't detect collision while transmitting / 发送时无法检测碰撞**: A wireless station's own transmission drowns out all other signals (its transmit power is much stronger than the received signal from other stations). You can't "listen while talking" as CSMA/CD requires.

> **中文**：无线站点的自身传输会淹没所有其他信号（其发射功率远强于来自其他站点的接收信号）。无法像CSMA/CD要求的那样"边说话边听"。

**CSMA/CA Solution — Three techniques / CSMA/CA解决方案——三种技术**:

**A) Interframe Spaces (IFS) / 帧间间隔（IFS）**: Priority mechanism through waiting times / 通过等待时间实现的优先级机制。
- **SIFS** (Short IFS)【短帧间间隔】: 28 μs — highest priority (for ACK, CTS, data fragments) / 28微秒——最高优先级（用于ACK、CTS、数据片段）
- **DIFS** (DCF IFS)【DCF帧间间隔】: 128 μs — normal data frames / 128微秒——普通数据帧
- Stations with shorter IFS get priority because they can grab the channel sooner / IFS较短的站点获得优先权，因为它们可以更早抢占信道

**B) Virtual Carrier Sensing using NAV / 使用NAV的虚拟载波监听**:
Each frame carries a **duration field** indicating how long the transmission will take. All stations that hear this set their **NAV (Network Allocation Vector)【网络分配向量】** timer, effectively marking the channel as busy even if physical carrier sensing says idle.

> **中文**：每个帧携带一个**持续时间字段**，指示传输将持续多长时间。所有听到此信息的站点设置其**NAV（网络分配向量）**定时器，即使物理载波监听显示空闲，也有效标记信道为忙。

**C) RTS/CTS Handshake (Optional, solves hidden terminal) / RTS/CTS握手（可选，解决隐藏终端问题）**:
1. Source sends **RTS** (Request To Send)【请求发送】 — includes source, destination, and duration
2. Destination replies with **CTS** (Clear To Send)【清除发送】 — echoes the duration
3. All stations hearing RTS or CTS set their NAV
4. Source then sends the data frame
5. Destination sends ACK

> **中文**：RTS/CTS握手的五个步骤：(1)源站发送RTS（请求发送）——包含源地址、目的地址和持续时间；(2)目的站回复CTS（清除发送）——回传持续时间；(3)所有听到RTS或CTS的站点设置其NAV；(4)源站发送数据帧；(5)目的站发送ACK确认。

```
Sender:      RTS ──────────→
Receiver:    ←────────── CTS
Other nodes: (Set NAV, defer / 设置NAV，推迟)
Sender:      DATA ─────────→
Receiver:    ←────────── ACK
```

**CSMA/CA Complete Procedure / CSMA/CA完整过程**:
1. Sense channel. If idle for **DIFS** period → transmit immediately / 监听信道。若空闲**DIFS**时段 → 立即发送
2. If busy → wait until idle for DIFS, then: / 若忙 → 等待至空闲DIFS，然后：
   a. Choose a **random backoff** counter (from contention window) / 选择一个**随机退避**计数器（从竞争窗口）
   b. Decrement counter while channel is idle; freeze while channel is busy / 信道空闲时递减计数器；信道忙时冻结
   c. When counter reaches 0 → transmit / 计数器到0时 → 发送
3. Wait for ACK. If no ACK → increase contention window, retry / 等待ACK。若未收到ACK → 增大竞争窗口，重试

### 2.2.6 CSMA/CD vs CSMA/CA Comparison (Must Memorize) / CSMA/CD vs CSMA/CA 对比（必须牢记）

| Aspect / 方面 | CSMA/CD | CSMA/CA |
|--------------|---------|---------|
| Used in / 应用场景 | Wired Ethernet (802.3) / 有线以太网（802.3） | Wireless LAN (802.11) / 无线局域网（802.11） |
| Strategy / 策略 | Detect collisions after they happen / 碰撞发生后检测 | Try to **avoid** collisions before they happen / 尝试在碰撞发生前**避免**它们 |
| Collision detection / 碰撞检测 | Yes (voltage comparison on wire) / 是（线缆上电压比较） | No (can't listen while transmitting) / 否（发送时无法监听） |
| RTS/CTS | Not typically used / 通常不使用 | Used to solve hidden terminal / 用于解决隐藏终端 |
| ACK frames / ACK帧 | Not needed at MAC level / MAC层不需要 | Required (explicit ACK for every data frame) / 需要（每个数据帧都有显式ACK） |
| Contention sensing / 信道感知 | Physical carrier sense / 物理载波监听 | Physical + Virtual (NAV) / 物理 + 虚拟（NAV） |
| Efficiency / 效率 | High on wired (controlled medium) / 有线环境下高（受控介质） | Lower due to overhead (ACKs, RTS/CTS, backoff) / 较低，因开销（ACK、RTS/CTS、退避） |

### 2.2.7 Comparison of MAC Protocol Categories / MAC协议类别对比

A high-level comparison of the three major MAC protocol families:

| Category / 类别 | High Load / 高负载 | Low Load / 低负载 | Examples / 示例 |
|-----------------|:---:|:---:|---------|
| **Channel Partitioning (Static) / 信道划分（静态）** | Efficient and fairly shared / 高效且公平共享 | **Inefficient** — 1/N bandwidth even with 1 active user, access delay / **低效**——即使只有1个活跃用户也分配1/N带宽，有接入延迟 | FDMA, TDMA |
| **Random Access / 随机接入** | **Inefficient** — collisions cause overhead / **低效**——碰撞导致开销 | **Efficient** — single node can fully utilize channel / **高效**——单个节点可充分利用信道 | ALOHA, CSMA/CD, CSMA/CA |
| **Contention-Free (Taking Turns) / 无竞争（轮流）** | Efficient — organized access / 高效——有组织的接入 | Some overhead from polling/token passing, but better than partitioning / 有轮询/令牌传递开销，但优于信道划分 | Polling, Token Passing, Bit-Map |

> **中文**：三种MAC协议类别的对比：信道划分在高负载时高效但在低负载时低效；随机接入在低负载时高效但在高负载时因碰撞而低效；无竞争协议两者兼顾但始终有少量轮询/令牌开销。

---

## 2.3 Collision-Free Protocols / 无碰撞协议

| Protocol / 协议 | How it works / 工作原理 |
|---------------|------------------------|
| **Bit-Map / 位图协议** | Each station has a reserved bit in a contention period; station i sets bit i = 1 if it has data / 每个站点在争用期有一个预留位；站点i若有数据则设位i = 1 |
| **Token Passing / 令牌传递** | A special "token" frame circulates; only the station holding the token may transmit / 特殊的"令牌"帧循环；只有持有令牌的站点才能发送 |
| **Binary Countdown / 二进制倒计数** | Each station transmits its binary address; highest address wins / 每个站点发送其二进制地址；地址最高的胜出 |

> **中文**：无碰撞协议通过预留或轮流方式完全避免碰撞。位图协议中每个站点有固定的预留时隙；令牌传递中只有持令牌者可发送；二进制倒计数中地址最高的站点赢得信道访问权。

**Token Passing Protocol Details / 令牌传递协议细节**:
- **Problems** / **问题**: Overhead (令牌消息), Latency (延迟), Single point of failure (令牌丢失 → 系统瘫痪)
- The control token (令牌) is passed from one node to the next one sequentially / 控制令牌依次从一个节点传递给下一个节点

> **中文**：令牌传递的问题包括：令牌消息带来的开销、延迟、以及令牌丢失导致的单点故障。

**Bit-Map Protocol Efficiency / 位图协议效率**:
- $N$ users, each frame of $d$ bits
- **Low load** / **低负载**: Low efficiency (wasted reservation bits) / 效率低（预留位浪费）: $\frac{d}{d+N}$
- **High load** / **高负载**: High efficiency / 效率高: $\frac{kd}{kd+N}$ (where $k$ is the number of active stations / $k$为活跃站点数)

**Binary Countdown Efficiency / 二进制倒计数效率**:
$$\boxed{\text{Efficiency / 效率} = \frac{d}{d + \log_2 N}}$$

> **中文**：二进制倒计数的效率公式为$d/(d+\log_2 N)$，其中$d$为数据帧长度，$N$为站点数。由于地址长度仅为$\log_2 N$位，因此在高负载下效率很高。

**Polling Protocol (轮询协议)**:
- A master node invites slave nodes to transmit in turn / 主节点依次邀请从节点发送
- Typically used when slaves are "dumb" / 通常用于从节点"较简单"的场景
- **Problems** / **问题**: Overhead (轮询消息), Latency (延迟), Single point of failure (master fails → system fails / 主节点故障 → 系统瘫痪)

> **中文**：轮询协议由主节点依次邀请从节点发送，常用于从节点较简单的场景。问题包括：轮询开销、延迟、主节点单点故障。

---

## 2.4 Adaptive Tree Walk Protocol ★ (Classic Calculation Problem) / 自适应树遍历协议 ★（经典计算题）

Used to resolve contention among stations organized in a **binary tree**.

> **中文**：用于解决组织在**二叉树**中的站点之间的争用。

**Algorithm / 算法**: Depth-first search through the tree. A node (representing a group of stations) is "probed" in a time slot:
- If 0 or 1 station in the group wants to transmit → success or idle / 若组中0或1个站想要发送 → 成功或空闲
- If 2+ stations want to transmit → collision → recursively probe the children (left subtree first, then right) / 若2个以上站想要发送 → 碰撞 → 递归探测子节点（先左子树，后右子树）

> **中文**：算法本质是深度优先搜索。每个时隙探测树中的一个节点（代表一组站点）：若组中0或1个站要发送则成功或空闲；若2个以上站要发送则发生碰撞，递归探测子节点（先左后右）。

**Key formula / 关键公式**: For optimal start level $i$ with $q$ contending stations / 对于$q$个争用站点，最优起始级别$i$:

$$\boxed{i = \log_2 q}$$

Where $i=0$ is the root level. This minimizes the expected number of time slots.

> **中文**：其中$i=0$是根级别。这最小化了期望的时隙数。

**Example / 示例**: In a tree with 8 leaves (stations), stations G and H both want to send. How many time slots until one succeeds?

> **中文**：在一个有8个叶子节点（站点）的树中，站点G和H都想发送。需要多少个时隙直到其中一个成功？

- Slot 0 / 时隙0: Root (1) — collision (G and H are both under it) / 根节点——碰撞（G和H都在其下）
- Slot 1 / 时隙1: Node 2 (left child of root) — idle (G and H are under node 3, not node 2) / 节点2（根的左子）——空闲（G和H在节点3下，不在节点2下）
- Slot 2 / 时隙2: Node 6 (left child of node 3) — idle / 节点6（节点3的左子）——空闲
- Slot 3 / 时隙3: Station G — succeeds! (G is alone) / 站点G——成功！（G独自）

**Answer / 答案**: 4 time slots (0, 1, 2, 3). Station H will get its turn in the next probe sequence.

> **中文**：4个时隙（0、1、2、3）。站点H将在下一次探测序列中获得其机会。

### 2.4.1 Limited-Contention Protocols / 有限竞争协议

Limited-contention protocols combine the best of contention-based and contention-free approaches:

> **中文**：有限竞争协议结合了基于竞争和无竞争方法的优点。

**Trade-off / 权衡**:
- **Low load**: Contention is preferred — low delay / **低负载**：竞争方式更优——延迟低
- **High load**: Contention-free is preferred — high channel utilization / **高负载**：无竞争方式更优——信道利用率高

**Probability Analysis / 概率分析**:
- With $k$ stations contending and each transmitting with probability $p$ in a given slot:
  $$\alpha(p) = kp(1-p)^{k-1}$$
- The maximum of $\alpha(p)$ over $p$ is:
  $$\max_p \alpha(p) = \left(1 - \frac{1}{k}\right)^{k-1}$$
- As $k \to \infty$:
  $$\lim_{k\to\infty} \left(1 - \frac{1}{k}\right)^{k-1} = \frac{1}{e} \approx 0.368$$

> **中文**：当$k$个站点竞争信道时，每个时隙中成功传输的概率为$\alpha(p) = kp(1-p)^{k-1}$。最大概率为$(1-1/k)^{k-1}$，当$k\to\infty$时收敛于$1/e \approx 0.368$，这与时隙ALOHA的最大吞吐量一致。

---

## 2.5 Ethernet (IEEE 802.3) ★★★ / 以太网（IEEE 802.3）★★★

### 2.5.1 Ethernet Service: Connectionless, Unacknowledged / 以太网服务：无连接，无确认

- **No connection** established before sending / 发送前不建立连接
- **No acknowledgments** at the data link layer / 数据链路层无确认
- If a frame is corrupted, it's silently discarded / 若帧损坏，则静默丢弃
- Error recovery is the responsibility of higher layers (e.g., TCP) / 错误恢复由上层负责（如TCP）

### 2.5.2 MAC Address (48 bits) / MAC地址（48位）

A globally unique identifier burned into the NIC ROM.

> **中文**：烧录在网卡ROM中的全球唯一标识符。

**Structure / 结构**:

```
Byte 1    Byte 2    Byte 3    |  Byte 4    Byte 5    Byte 6
   OUI (Organizationally Unique Identifier) |  NIC-Specific Address
   (机构唯一标识符)                          |  (网卡特定地址)
   Assigned by IEEE to manufacturer         |  Assigned by manufacturer
   (由IEEE分配给制造商)                      |  (由制造商分配)
```

**Format / 格式**: 6 bytes, written in hexadecimal as `07-01-02-01-2C-4B` or `07:01:02:01:2C:4B` / 6字节，十六进制书写为`07-01-02-01-2C-4B`或`07:01:02:01:2C:4B`

**Key address types / 关键地址类型**:
- **Unknown / 未知**: `00-00-00-00-00-00` — indicates an unknown address / 表示未知地址
- **Unicast / 单播**: Bit 0 of Byte 1 = **0** → destined for one specific host / 字节1的第0位为**0** → 发往一个特定主机
- **Multicast / 多播**: Bit 0 of Byte 1 = **1** → destined for a group / 字节1的第0位为**1** → 发往一组; e.g., `01-00-5E-XX-XX-XX` is a commonly used multicast prefix (historically only 23 of the 48 bits are used for group addressing) / 例如`01-00-5E-XX-XX-XX`是常用的多播前缀（历史上48位中只有23位用于组寻址）
- **Broadcast / 广播**: All 48 bits = 1 → `FF-FF-FF-FF-FF-FF` → all hosts on the LAN / 全部48位均为1 → 发往LAN上所有主机

### 2.5.3 Four Modes of NIC (Network Interface Card) / 网卡的四种工作模式

A NIC can operate in different modes when receiving frames.

> **中文**：网卡在接收帧时可以工作在不同的模式下。

| Mode / 模式 | Description / 描述 |
|------------|-------------------|
| **Broadcast / 广播模式** | Accept broadcast frames (address FF-FF-FF-FF-FF-FF) / 接受广播帧（地址FF-FF-FF-FF-FF-FF） |
| **Multicast / 多播模式** | Accept all groupcast frames (even if not a member of the group) / 接受所有组播帧（即使不是该组成员） |
| **Direct (Unicast) / 直接模式（单播）** | Accept frames only destined to itself (unicast address) / 只接受发往自身（单播地址）的帧 |
| **Promiscuous / 混杂模式** | Accept **all** frames passing by — this is how Wireshark works. NIC of a switch/bridge works in this mode. / 接受**所有**经过的帧——Wireshark就是通过此方式工作的。交换机/网桥的NIC工作在此模式。 |

**Default mode / 默认模式**: Broadcast + Direct (广播模式 + 直接模式)

> **中文**：网卡默认同时开启广播模式和直接模式。

> **中文**：四种模式中，混杂模式是安全相关的重点——网卡接受所有经过的帧，这是网络监听（嗅探）的基础。默认情况下网卡只接收发给自己和广播的帧。

### 2.5.4 Ethernet Frame Format / 以太网帧格式

```
+--------+--------+--------+--------+--------+---------+--------+
|Preamble|  Dest  |  Src   |Type/Len|  Data  | Pad     |  CRC   |
| 前导码  |目的地址 |源地址   |类型/长度| 数据   | 填充    | 校验   |
|  8 B   |  6 B   |  6 B   |  2 B   |0-1500B | 0-46 B  |  4 B   |
+--------+--------+--------+--------+--------+---------+--------+
```

| Field / 字段 | Size / 大小 | Description / 描述 |
|-------------|------------|-------------------|
| **Preamble / 前导码** | 8 bytes / 8字节 | 7 bytes sync pattern (10101010...) + 1 byte Start Frame Delimiter (10101011) / 7字节同步模式 + 1字节帧起始定界符 |
| **Destination MAC / 目的MAC** | 6 bytes / 6字节 | Recipient's MAC address / 接收者的MAC地址 |
| **Source MAC / 源MAC** | 6 bytes / 6字节 | Sender's MAC address / 发送者的MAC地址 |
| **Type/Length / 类型/长度** | 2 bytes / 2字节 | If $\le 1500$: length of data field (802.3). If $\ge 1536$: type (DIX Ethernet, e.g., 0x0800 = IPv4, 0x0806 = ARP) / 若$\le 1500$：数据字段长度（802.3）。若$\ge 1536$：类型（DIX以太网，如0x0800=IPv4，0x0806=ARP） |
| **Data / 数据** | 0-1500 bytes / 0-1500字节 | Payload from upper layer / 来自上层的负载 |
| **Pad / 填充** | 0-46 bytes / 0-46字节 | Fills short frames to meet the 64-byte minimum / 填充短帧以满足64字节最小长度要求 |
| **CRC (FCS) / 循环冗余校验** | 4 bytes / 4字节 | CRC-32 error check over addresses, type/length, and data fields / 对地址、类型/长度和数据字段的CRC-32错误校验 |

**Frame size rules / 帧大小规则**:
- Minimum / 最小值: 64 bytes (excluding preamble) / 64字节（不含前导码）
- Maximum standard / 标准最大值: 1518 bytes / 1518字节
- Maximum with VLAN tag (802.1Q) / 带VLAN标签最大值: 1522 bytes (+4 bytes for VLAN tag: 2B TPID + 2B TCI) / 1522字节（+4字节VLAN标签：2B TPID + 2B TCI）

### 2.5.4 VLAN (Virtual LAN) with 802.1Q / VLAN（虚拟局域网）与802.1Q

VLAN logically segments a physical LAN into multiple broadcast domains. 802.1Q adds a 4-byte tag after the source MAC address:
- **TPID** (2 bytes): 0x8100 (identifies 802.1Q frame) / 标识802.1Q帧
- **TCI** (2 bytes): Priority (3 bits) + CFI (1 bit) + VLAN ID (12 bits, allowing 4094 VLANs) / 优先级(3位) + CFI(1位) + VLAN ID(12位，允许4094个VLAN)

> **中文**：VLAN将物理局域网逻辑分割为多个广播域。802.1Q在源MAC地址后添加4字节标签：TPID（2字节，值0x8100标识802.1Q帧）和TCI（2字节，包含3位优先级、1位CFI和12位VLAN ID，支持4094个VLAN）。

**Key concepts / 关键概念**:
- A VLAN is a **broadcast domain** — broadcasts are confined to the VLAN / VLAN是一个**广播域**——广播被限制在VLAN内
- Communication between VLANs requires a **router** (Layer 3) / VLAN间通信需要**路由器**（第3层）
- VLAN grouping is **logical**, not physical — devices on different floors can be in the same VLAN / VLAN分组是**逻辑的**而非物理的——不同楼层的设备可以在同一VLAN中

### 2.5.5 Ethernet Generations / 以太网代际

| Generation / 代际 | Speed / 速度 | Key Features / 关键特性 |
|------------------|-------------|----------------------|
| **Traditional Ethernet / 传统以太网** | 10 Mbps | 10BASE5 (thick coax / 粗同轴电缆), 10BASE2 (thin coax / 细同轴电缆), 10BASE-T (twisted pair / 双绞线), CSMA/CD, half-duplex / 半双工 |
| **Fast Ethernet / 快速以太网** | 100 Mbps | 100BASE-TX (Cat 5), 100BASE-FX (fiber / 光纤), backward compatible, still uses CSMA/CD / 向后兼容，仍使用CSMA/CD |
| **Gigabit Ethernet / 千兆以太网** | 1 Gbps | 1000BASE-T, full-duplex preferred / 首选全双工, carrier extension for half-duplex / 半双工时载波扩展 |
| **10 Gigabit Ethernet / 万兆以太网** | 10 Gbps | 10GBASE-T, fiber mostly / 主要为光纤, **no CSMA/CD** (full-duplex only / 仅全双工), no half-duplex option / 无双工选项 |

**Gigabit Ethernet Design Challenge (Big Question) / 千兆以太网设计挑战（大题）**: At 1 Gbps, a 64-byte frame takes only 512 ns to transmit. With a 200 m cable, the round-trip time is ~2 μs, which means the sender finishes transmitting long before a collision could be detected. Solutions:

> **中文**：在1 Gbps下，64字节帧仅需512纳秒即可发送完毕。对于200米线缆，往返时间约2微秒，这意味着发送者在碰撞可被检测到之前很久就已发送完毕。解决方案：

1. **Carrier Extension / 载波扩展**: Artificially extend short frames to 512 bytes (4096 bit times) by padding the transmission. This ensures the frame is long enough for collision detection.
   > **中文**：通过填充传输将短帧人为扩展到512字节（4096比特时间）。这确保了帧足够长以便碰撞检测。

2. **Frame Bursting / 帧突发**: Allow a sender to transmit multiple frames consecutively (with a small gap) without contending for the channel each time. This improves efficiency when many short frames need to be sent.
   > **中文**：允许发送者连续发送多个帧（有小间隔），无需每次都争用信道。这在需要发送大量短帧时提高了效率。

### 2.5.6 Ethernet Cables and 802.3 Standards / 以太网线缆与802.3标准

Ethernet specifications follow the naming convention: `<DataRate><Base><MediumType>`.

> **中文**：以太网规范遵循命名约定：`<数据速率><Base><介质类型>`。

| Specification / 规范 | Speed / 速率 | Cable / 线缆 | Max Segment / 最大段长 |
|---------------------|:-----------:|-------------|:---------------------:|
| **10Base-5** | 10 Mbps | Thick Coaxial (粗同轴电缆) | 500 m |
| **10Base-2** | 10 Mbps | Thin Coaxial (细同轴电缆) | 200 m |
| **10Base-T** | 10 Mbps | Twisted Pair (双绞线) | 100 m |
| **100Base-TX** | 100 Mbps | Cat 5 Twisted Pair | 100 m |
| **100Base-FX** | 100 Mbps | Fiber (光纤) | 2 km |
| **1000Base-T** | 1 Gbps | Cat 5e/6 Twisted Pair | 100 m |
| **2.5GBase-T** | 2.5 Gbps | Cat 5e | 100 m |
| **5GBase-T** | 5 Gbps | Cat 6 | 100 m |
| **10GBase-T** | 10 Gbps | Cat 6a/7 | 100 m |

> **中文**：以太网的发展伴随着线缆的演进：从粗/细同轴电缆到双绞线和光纤。常见的以太网类型包括10Base-T、100Base-TX、1000Base-T和10GBase-T等。新的标准如2.5GBase-T和5GBase-T允许在现有的Cat 5e/Cat 6线缆上提升速率。

**IEEE 802.2 LLC (Logical Link Control)** / **IEEE 802.2逻辑链路控制**:
All 802.3 Ethernet standards share a common MAC protocol and frame format but differ in the physical layer (speed and medium).

> **中文**：所有802.3以太网标准共享相同的MAC协议和帧格式，但在物理层（速度和介质）上有所不同。

---

### 2.6.1 How a Switch Learns / 交换机如何学习

A **transparent bridge** (or switch) maintains a **MAC address table** (forwarding table) mapping MAC addresses to ports.

> **中文**：**透明网桥**（或交换机）维护一个**MAC地址表**（转发表），将MAC地址映射到端口。

**Learning algorithm / 学习算法**:
1. When a frame arrives on port P / 当帧到达端口P时:
   - Record the **source MAC** → this MAC is on port P (update table) / 记录**源MAC** → 此MAC在端口P上（更新表）
   - Look up the **destination MAC** in the table / 在表中查找**目的MAC**
2. **If destination is found** in the table (known unicast) / **若在表中找到目的地址**（已知单播）:
   - If destination port ≠ arrival port → **forward** only to that port / 若目的端口 ≠ 到达端口 → 仅**转发**到该端口
   - If destination port = arrival port → **filter** (don't forward, destination is on same segment) / 若目的端口 = 到达端口 → **过滤**（不转发，目的在同一网段）
3. **If destination is NOT found** (unknown destination) → **flood** to all ports except the arrival port / **若未找到目的地址**（未知目的）→ 向除到达端口外的所有端口**泛洪**
4. **If destination is broadcast/multicast** → **flood** to all ports except arrival port / **若目的为广播/多播** → 向除到达端口外的所有端口**泛洪**

**Backward learning / 反向学习**: The switch learns which hosts are on which ports by observing the **source addresses** of incoming frames.

> **中文**：交换机通过观察入帧的**源地址**来学习哪些主机在哪些端口上。

### 2.6.2 Switching Modes / 交换模式

| Mode / 模式 | How It Works / 工作原理 | Advantage / 优点 | Disadvantage / 缺点 |
|------------|------------------------|-----------------|-------------------|
| **Store-and-Forward / 存储转发** | Receives entire frame, checks CRC, then forwards / 接收完整帧，检查CRC，然后转发 | Error filtering; can connect different speed ports / 错误过滤；可连接不同速率端口 | Higher latency (stores whole frame) / 较高延迟（存储整帧） |
| **Cut-through / 直通交换** | Reads destination address (first 6 bytes), forwards immediately / 读取目的地址（前6字节），立即转发 | Very low latency (~10 μs) / 极低延迟（约10微秒） | Forwards bad frames; can't connect different speeds easily / 转发坏帧；不易连接不同速率 |
| **Fragment-free / 无碎片交换** (Modified cut-through / 改进直通) | Reads first 64 bytes (minimum frame), forwards if no error appearance / 读取前64字节（最小帧），若无错误迹象则转发 | Compromise: filters most collision fragments / 折衷：过滤大多数碰撞碎片 | Still less than full error checking / 仍不如完整错误检查 |

### 2.6.3 Switch Bandwidth Calculation / 交换机带宽计算

Switches support **full-duplex** on each port, meaning each port can send AND receive simultaneously.

> **中文**：交换机每个端口支持**全双工**，意味着每个端口可同时发送和接收。

**Total switch bandwidth (背板带宽)**:
$$\boxed{\text{Total bandwidth / 总带宽} = 2 \times \sum (\text{Port speed}_i\text{ / 端口速率}_i)}$$

(中文说明：总带宽 = 2 × 各端口速率之和。因子2考虑了全双工，即每个端口发送加接收。)

The factor of 2 accounts for full-duplex (send + receive per port).

> **中文**：因子2考虑了全双工（每个端口的发送 + 接收）。

**Example / 示例**: If a switch has 22 ports at 100 Mbps (full-duplex) and total switch capacity is 8.4 Gbps, how many 1 Gbps (full-duplex) ports can it have?

> **中文**：若一个交换机有22个100 Mbps（全双工）端口，总交换容量为8.4 Gbps，还可以有多少个1 Gbps（全双工）端口？

Each 100 Mbps full-duplex port / 每个100 Mbps全双工端口 = $2 \times 0.1 = 0.2$ Gbps
Each 1 Gbps full-duplex port / 每个1 Gbps全双工端口 = $2 \times 1 = 2$ Gbps

$x = \frac{8.4 - 22 \times 0.2}{2} = \frac{8.4 - 4.4}{2} = \frac{4}{2} = \boxed{2 \text{ ports / 个端口}}$

### 2.6.4 Collision Domain vs Broadcast Domain / 冲突域 vs 广播域

| Device / 设备 | Layer / 层 | Segments Collision Domains? / 分隔冲突域？ | Segments Broadcast Domains? / 分隔广播域？ |
|--------------|:----------:|:-----------------------------------------:|:-----------------------------------------:|
| Repeater / Hub / 中继器/集线器 | 1 (Physical / 物理层) | No / 否 | No / 否 |
| Bridge / Switch / 网桥/交换机 | 2 (Data Link / 数据链路层) | **Yes / 是** | No / 否 |
| Router / 路由器 | 3 (Network / 网络层) | **Yes / 是** | **Yes / 是** |

**Key rule / 关键规则**: Each port of a switch is its own collision domain (no collisions in full-duplex switch). But all ports are in the same broadcast domain. Routers block broadcasts.

> **中文**：交换机的每个端口是自己的冲突域（全双工交换机中无碰撞）。但所有端口在同一广播域中。路由器阻断广播。

### 2.6.5 Spanning Tree Protocol (STP) / 生成树协议（STP）

Prevents loops in a switched network by creating a loop-free logical topology (a **spanning tree**):
- Switches elect a **root bridge** / 交换机选举一个**根网桥**
- Each switch determines its best path to the root / 每个交换机确定到根的最佳路径
- Redundant links are blocked (put in standby) / 冗余链路被阻塞（置于待机状态）
- If a link fails, a blocked link can be activated / 若某链路故障，可激活被阻塞的链路

> **中文**：STP通过创建无环逻辑拓扑（**生成树**）来防止交换网络中的环路。交换机选举根网桥，各交换机确定到根的最佳路径，冗余链路被阻塞（待机），故障时激活备用链路。

The spanning tree is used in the **sink tree** for broadcast routing (see Ch04).

> **中文**：生成树用于广播路由中的**汇集树**（见第4章）。

### 2.6.6 Hub (Repeater) Details / 集线器（中继器）细节

A hub is a **Physical Layer (Layer 1) "dumb" repeater** / 集线器是**物理层（第一层）的"哑"中继器**:

> **中文**：集线器是工作在物理层的简单设备，主要功能是扩展网络距离。

**Characteristics / 特性**:
1. Bits coming in one link go out **all other links** at the same rate / 从一条链路进入的比特以相同速率从**所有其他链路**出去
2. **No CSMA/CD** at the hub — host NICs are responsible for collision detection / 集线器本身**没有CSMA/CD功能**——主机网卡负责碰撞检测
3. All nodes connected to the same hub share **one large collision zone** (individual segment collision zones merge) / 连接到同一集线器的所有节点共享**一个大的冲突域**（各段冲突域合并）
4. **No frame buffering** / **没有帧缓冲**
5. Cannot interconnect different speeds (e.g., 10Base-T and 100Base-T) / 无法互联不同速率的以太网（如10Base-T和100Base-T）

### 2.6.7 Network Devices Comparison / 网络设备对比

> **中文**：以下是四种常见网络设备的对比总结：

| Device / 设备 | Layer / 层 | Function / 功能 | Key Feature / 关键特性 |
|--------------|:---------:|---------------|---------------------|
| **Modem / 调制解调器** | PHY (1) | Analog ↔ Digital conversion / 模拟↔数字转换 | Converts between analog and digital signals / 在模拟和数字信号之间转换 |
| **Hub / Repeater / 集线器/中继器** | PHY (1) | Broadcast received bits to all other ports / 广播接收到的比特到所有其他端口 | Extends distance, one collision zone, no buffering / 扩展距离，一个冲突域，无缓冲 |
| **Switch / Bridge / 交换机/网桥** | Link (2) | Forward frames based on MAC addresses / 基于MAC地址转发帧 | Self-learning, multiple collision zones, can filter local traffic / 自学习，多冲突域，可过滤本地流量 |
| **Router / 路由器** | Network (3) | Forward packets based on IP addresses / 基于IP地址转发分组 | Connects subnets, separates broadcast and collision domains / 连接子网，分隔广播域和冲突域 |

**Additional switch/bridge details**:
- **Switch**: Can use **Spanning Tree Protocol** to avoid broadcast storms / 交换机可使用**生成树协议**避免广播风暴
- **Bridge**: May have broadcast storm without STP / 网桥在没有STP时可能有广播风暴

> **中文**：交换机可避免广播风暴（使用STP），而传统网桥在没有STP时可能发生广播风暴。所有四种设备中，只有路由器可以分隔广播域。

---

## 2.7 Wireless LANs (IEEE 802.11) ★ / 无线局域网（IEEE 802.11）★

### 2.7.1 Wireless Link Characteristics / 无线链路特性

Wireless links differ fundamentally from wired links in several ways:

> **中文**：无线链路与有线链路有根本性的不同：

**Key Characteristics / 关键特性**:
1. **Attenuation (衰减)**: Decreased signal strength as it propagates through the medium. Signal loses strength over distance.
   > **中文**：信号在传播过程中强度衰减，距离越远信号越弱。

2. **Interference (干扰)**: Frequency bands are shared among multiple devices. Other devices transmitting on the same frequency cause interference.
   > **中文**：多个设备共享同一频段，其他设备在同一频率上发送会造成干扰。

3. **Multi-path Propagation (多径效应)**: Radio signals reflect off objects (walls, buildings), creating multiple copies of the same signal arriving at the receiver at slightly different times.
   > **中文**：无线电信号在传播过程中会被物体（墙壁、建筑物等）反射，产生多个副本在不同时间到达接收端，造成信号叠加和失真。

4. **SNR (Signal-to-Noise Ratio) / 信噪比**: Measures signal quality. Higher SNR means easier to extract signal from noise ⇒ lower BER (Bit Error Rate).
   > **中文**：信噪比衡量信号质量。较高的SNR意味着更容易从噪声中提取信号⇒较低的误比特率（BER）。

**SNR vs BER Tradeoff / SNR与BER的权衡**:
- Given a physical layer: increasing transmission power ⇒ increases SNR ⇒ decreases BER
- Given a target SNR: choose the modulation/PHY that meets BER requirements and has the highest throughput
- This is directly related to the **Shannon Capacity** formula from Chapter 1

> **中文**：给定物理层，增加发射功率⇒增加SNR⇒降低BER。给定SNR，选择满足BER要求且吞吐量最高的调制/PHY方案。这与第一章的香农容量公式直接相关。

### 2.7.2 802.11 Family / 802.11家族

| Standard / 标准 | Frequency / 频率 | Max Data Rate / 最大数据速率 | Modulation / 调制方式 |
|----------------|----------------|:---------------------------:|---------------------|
| 802.11b | 2.4 GHz | 11 Mbps | DSSS |
| 802.11a | 5 GHz | 54 Mbps | OFDM |
| 802.11g | 2.4 GHz | 54 Mbps | OFDM |
| 802.11n (Wi-Fi 4) | 2.4/5 GHz | 600 Mbps | OFDM, MIMO |
| 802.11ac (Wi-Fi 5) | 5 GHz | 6.9 Gbps (PPT: 3.4 Gbps) | OFDM, MU-MIMO |
| 802.11ax (Wi-Fi 6) | 2.4/5/6 GHz | 9.6 Gbps (PPT: 7 Gbps) | OFDMA |
| **802.11be (Wi-Fi 7)** | **2.4/5/6 GHz** | **46 Gbps** | OFDMA, 4096-QAM |
| **802.11bn (Wi-Fi 8)** | **2.4/5/6/42/71 GHz** | **100 Gbps** | — |

### 2.7.2 Two Operating Modes / 两种工作模式

| Mode / 模式 | Description / 描述 | Pros/Cons / 优缺点 |
|------------|-------------------|-------------------|
| **Infrastructure (AP-based) / 基础设施模式（基于AP）** | Stations communicate through an Access Point (AP) / 站点通过接入点通信 | Centralized management, better security, scalable — **most common** / 集中管理，更好安全性，可扩展——**最常见** |
| **Ad hoc (peer-to-peer) / 自组织模式（对等）** | Stations communicate directly without AP / 站点间直接通信，无需AP | No infrastructure needed, but harder to manage and scale / 无需基础设施，但较难管理和扩展 |

**Why infrastructure mode is preferred / 为何基础设施模式更受欢迎**: Centralized control, easier security/access management, better network organization, easier to extend and scale.

> **中文**：集中控制、更容易的安全/接入管理、更好的网络组织、更容易扩展和伸缩。

### 2.7.3 DCF and PCF / DCF和PCF

| | DCF (Distributed Coordination Function) / 分布式协调功能 | PCF (Point Coordination Function) / 点协调功能 |
|--|:-------------------------------------------------------:|:----------------------------------------------:|
| **Access method / 接入方法** | Contention-based (CSMA/CA) / 基于竞争（CSMA/CA） | Contention-free (polling) / 无竞争（轮询） |
| **Required/Optional / 必需/可选** | **Mandatory** (must implement) / **强制**（必须实现） | **Optional** (may implement) / **可选**（可实现） |
| **How it works / 工作原理** | Stations compete for channel using CSMA/CA with backoff / 站点使用CSMA/CA和退避竞争信道 | A Point Coordinator polls stations in turn, giving each permission to send / 点协调器依次轮询站点，逐个授予发送权限 |
| **When used / 适用场景** | Normal data traffic / 常规数据流量 | Time-sensitive traffic (e.g., real-time voice/video) / 时间敏感流量（如实时光/视频） |
| **Analogy / 类比** | Free-for-all / 自由竞争 | Teacher calling on students one by one / 老师逐个点名 |

### 2.7.4 RTS/CTS Detailed Process / RTS/CTS详细过程

To solve the hidden terminal problem / 为解决隐藏终端问题:
1. Source waits DIFS and contends for channel (backoff) / 源站等待DIFS并竞争信道（退避）
2. Source sends **RTS** (20 bytes, includes source, destination, duration) / 源站发送**RTS**（20字节，包含源、目的、持续时间）
3. Destination waits **SIFS** (higher priority), sends **CTS** (14 bytes, includes duration) / 目的站等待**SIFS**（更高优先级），发送**CTS**（14字节，包含持续时间）
4. Source waits SIFS, sends DATA / 源站等待SIFS，发送数据
5. Destination waits SIFS, sends ACK / 目的站等待SIFS，发送ACK

> **中文**：RTS/CTS的5步详细过程：(1)源站等待DIFS并竞争信道；(2)源站发送RTS（20字节，含源地址、目的地址、持续时间）；(3)目的站等待SIFS（更高优先级），发送CTS（14字节，含持续时间）；(4)源站等待SIFS后发送数据；(5)目的站等待SIFS后发送ACK确认。

**NAV (Network Allocation Vector)【网络分配向量】**: All stations that hear RTS or CTS set their NAV to the duration value. They will not transmit until NAV expires. This is **virtual carrier sensing** — the station knows the channel will be busy even if it doesn't physically hear the data transmission.

> **中文**：**NAV（网络分配向量）**：所有听到RTS或CTS的站点将其NAV设置为持续时间值。在NAV到期前它们不会发送。这是**虚拟载波监听**——即使站点物理上未听到数据传输，也知道信道将忙。

---

## 2.8 Exam Practice: MAC Sublayer Questions / 考试练习：MAC子层问题

### Fill-in-the-Blank / 填空题

1. CSMA/CD stands for ______. **(Carrier Sense Multiple Access with Collision Detection)**
   > **中文**：CSMA/CD的全称是______。（载波监听多路访问/碰撞检测）
2. The minimum Ethernet frame length is ______ bytes. **(64)**
   > **中文**：以太网最小帧长为______字节。（64）
3. In Ethernet, the maximum frame size is ______ bytes (with VLAN tagging: ______ bytes). **(1518, 1522)**
   > **中文**：以太网最大帧大小为______字节（带VLAN标签：______字节）。（1518, 1522）
4. A MAC address is ______ bits long. **(48)**
   > **中文**：MAC地址长度为______位。（48）
5. The first 3 bytes of a MAC address are called the ______. **(OUI — Organizationally Unique Identifier)**
   > **中文**：MAC地址的前3字节称为______。（OUI——机构唯一标识符）
6. The two problems that CSMA/CA addresses in wireless networks are the ______ and ______ problems. **(hidden terminal, exposed terminal)**
   > **中文**：CSMA/CA在无线网络中解决的两个问题是______和______问题。（隐藏终端，暴露终端）
7. In 802.11, SIFS = ______ μs and DIFS = ______ μs. **(28, 128)**
   > **中文**：802.11中，SIFS = ______微秒，DIFS = ______微秒。（28, 128）
8. Pure ALOHA has a maximum throughput of ______%, while Slotted ALOHA achieves ______%. **(18.4, 36.8)**
   > **中文**：纯ALOHA的最大吞吐量为______%，时隙ALOHA达到______%。（18.4, 36.8）
9. A ______ device operates at Layer 2 and can segment collision domains but NOT broadcast domains. **(switch/bridge)**
   > **中文**：______设备工作在第二层，可分隔冲突域但不能分隔广播域。（交换机/网桥）
10. VLAN stands for ______. **(Virtual Local Area Network)**
    > **中文**：VLAN的全称是______。（虚拟局域网）

### True/False / 判断题

1. (T/F) CSMA/CD is used in wireless networks. **(F — CSMA/CA is used in wireless)**
   > **中文**：CSMA/CD用于无线网络。（错——无线中使用CSMA/CA）
2. (T/F) A switch can separate broadcast domains. **(F — routers separate broadcast domains, switches only separate collision domains)**
   > **中文**：交换机可以分隔广播域。（错——路由器分隔广播域，交换机只分隔冲突域）
3. (T/F) In the binary exponential backoff algorithm, after 16 collisions the frame is discarded. **(T)**
   > **中文**：在二进制指数退避算法中，16次碰撞后帧被丢弃。（对）
4. (T/F) The Ethernet preamble is included in the minimum frame length of 64 bytes. **(F — the 64-byte minimum does NOT include the preamble)**
   > **中文**：以太网前导码包含在64字节最小帧长中。（错——64字节最小值不包含前导码）
5. (T/F) Full-duplex Ethernet does not use CSMA/CD because there are no collisions. **(T — full-duplex has separate send and receive paths, so no contention)**
   > **中文**：全双工以太网不使用CSMA/CD，因为没有碰撞。（对——全双工有独立的发送和接收路径，无争用）
6. (T/F) A switch using cut-through switching checks the entire frame for errors before forwarding. **(F — that's store-and-forward; cut-through forwards after reading only the destination address)**
   > **中文**：使用直通交换的交换机在转发前检查整个帧的错误。（错——那是存储转发；直通交换读取目的地址后即转发）
7. (T/F) In 802.11, PCF is mandatory and DCF is optional. **(F — DCF is mandatory, PCF is optional)**
   > **中文**：802.11中，PCF是强制性的，DCF是可选的。（错——DCF是强制性的，PCF是可选的）

### Multiple Choice / 选择题

1. What is the contention period in CSMA/CD equivalent to?
   a) $\tau$ b) $2\tau$ c) $\tau/2$ d) $4\tau$ **(b — round-trip propagation time)**
   > **中文**：CSMA/CD中的争用期等于什么？（b——往返传播时间）

2. A 1 Gbps Ethernet on a 1 km cable (propagation speed = $2 \times 10^8$ m/s). What is the minimum frame size?
   a) 5000 bits b) 10000 bits c) 1000 bits d) 2000 bits
   **Answer: b)** $2 \times (1000 / 2 \times 10^8) \times 10^9 = 2 \times 5 \times 10^{-6} \times 10^9 = 10,000$ bits
   > **中文**：1 Gbps以太网，1公里线缆（传播速度=$2 \times 10^8$ m/s）。最小帧大小是多少？答案：b) 10000比特。计算：$2 \times (1000 / 2 \times 10^8) \times 10^9 = 10,000$比特

3. Which MAC address is a broadcast address?
   a) 01-00-5E-00-00-01 b) FF-FF-FF-FF-FF-FF c) 00-00-00-00-00-00 d) 11-22-33-44-55-66 **(b)**
   > **中文**：哪个MAC地址是广播地址？（b）FF-FF-FF-FF-FF-FF

4. Which device works at Layer 2?
   a) Hub b) Repeater c) Switch d) Router **(c)**
   > **中文**：哪个设备工作在第二层？（c）交换机

5. In cut-through switching, the switch reads the ______ before forwarding:
   a) Entire frame b) First 6 bytes c) First 64 bytes d) CRC field **(b)**
   > **中文**：在直通交换中，交换机在转发前读取______。（b）前6字节

6. What does a switch do when it receives a frame with an unknown destination MAC?
   a) Drop it b) Send it to the default gateway c) Flood all ports except the incoming port d) Store it until the destination is learned **(c)**
   > **中文**：当交换机收到目的MAC未知的帧时会做什么？（c）向除入端口外的所有端口泛洪

### Calculation Problems / 计算题

**Problem 1 / 问题1** (Minimum frame length / 最短帧长): A 1 km cable, 1 Gbps CSMA/CD network. Signal speed = 200,000 km/s. Find minimum frame length.

> **中文**：1公里线缆，1 Gbps CSMA/CD网络。信号速度=200,000 km/s。求最短帧长。

$$\tau = \frac{1 \text{ km}}{200,000 \text{ km/s}} = 5 \times 10^{-6} \text{ s} = 5 \text{ μs}$$
$$2\tau = 10 \text{ μs}$$
$$\text{Min frame / 最短帧} = 2\tau \times \text{Bandwidth / 带宽} = 10 \times 10^{-6} \times 10^9 = \boxed{10,000 \text{ bits / 比特}}$$

> **中文**：计算过程：$\tau = 1 / 200000 = 5$微秒，$2\tau = 10$微秒，最短帧长 = $10 \times 10^{-6} \times 10^9 = 10000$比特。

**Problem 2 / 问题2** (ALOHA throughput / ALOHA吞吐量): In a Slotted ALOHA system with 100 stations, what is the success probability per slot when each station transmits with probability 0.01?

> **中文**：在一个有100个站点的时隙ALOHA系统中，当每个站点以概率0.01发送时，每个时隙的成功概率是多少？

$$G = 100 \times 0.01 = 1 \text{ frame/slot / 帧/时隙}$$
$$P(\text{success / 成功}) = G \cdot e^{-G} = 1 \cdot e^{-1} \approx \boxed{36.8\%}$$

**Problem 3 / 问题3** (Binary backoff / 二进制退避): After 5 consecutive collisions on Ethernet, what is the range of possible wait times?

> **中文**：以太网上连续5次碰撞后，可能的等待时间范围是多少？

$k = \min(5, 10) = 5$, range / 范围 = $[0, 2^5 - 1] = [0, 31]$
Wait time / 等待时间 = random × 51.2 μs, between / 介于 0 and **1587.2 μs / 微秒**

**Problem 4 / 问题4** (Switch bandwidth / 交换机带宽): A switch has a backplane capacity of 16 Gbps, has 24 100 Mbps full-duplex ports. How many 1 Gbps full-duplex ports can it additionally support?

> **中文**：一个交换机背板容量为16 Gbps，已有24个100 Mbps全双工端口。还能额外支持多少个1 Gbps全双工端口？

24 × (2 × 0.1) = 4.8 Gbps used / 已用
Remaining / 剩余 = 16 - 4.8 = 11.2 Gbps
Each 1 Gbps full-duplex / 每个1 Gbps全双工端口 = 2 Gbps
Max ports / 最大端口数 = floor(11.2 / 2) = $\boxed{5 \text{ ports / 个端口}}$

---

## Appendix: Chinese-English Terminology / 附录：中英文术语对照

### MAC Sublayer / MAC子层
| English / 英文 | Chinese / 中文 |
|------|------|
| Medium Access Control (MAC) | 介质访问控制 |
| Channel allocation | 信道分配 |
| Contention | 竞争/争用 |
| Collision | 碰撞/冲突 |
| Carrier Sense (CS) | 载波监听 |
| Collision Detection (CD) | 碰撞检测 |
| Collision Avoidance (CA) | 碰撞避免 |
| Contention period / Collision window | 争用期/碰撞窗口 |
| Minimum frame length | 最短帧长 |
| Binary exponential backoff | 二进制指数退避 |
| Interframe gap | 帧间间隔 |
| Hidden terminal problem | 隐藏终端问题 |
| Exposed terminal problem | 暴露终端问题 |
| RTS/CTS | 请求发送/清除发送 |
| NAV (Network Allocation Vector) | 网络分配向量 |
| Virtual carrier sensing | 虚拟载波监听 |
| SIFS / DIFS | 短帧间间隔 / DCF帧间间隔 |
| MAC address | MAC地址/物理地址 |
| OUI (Organizationally Unique Identifier) | 机构唯一标识符 |
| VLAN (Virtual LAN) | 虚拟局域网 |
| 802.1Q | VLAN标签协议 |
| Switch / Bridge | 交换机/网桥 |
| Hub | 集线器 |
| Collision domain | 冲突域 |
| Broadcast domain | 广播域 |
| Store-and-forward | 存储转发 |
| Cut-through | 直通交换 |
| Spanning Tree Protocol (STP) | 生成树协议 |
| Infrastructure mode | 基础设施模式 |
| Ad hoc mode | 自组织模式 |
| DCF / PCF | 分布式协调功能 / 点协调功能 |
| Gigabit Ethernet | 千兆以太网 |
| Carrier extension | 载波扩展 |
| Frame bursting | 帧突发 |
