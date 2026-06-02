# Chapter 00: Introduction to Computer Networks / 计算机网络概述

> **Exam Relevance / 考试相关性**: This chapter provides the foundational framework for the entire course. Key exam areas include performance metrics calculation (big question), switching technology delay comparison (big question), OSI vs TCP/IP model (fill-in-blanks & multiple choice), and protocol classification (true/false & multiple choice).

> **中文**：本章为整个课程提供基础框架。考试重点包括：性能指标计算（大题）、交换技术延迟对比（大题）、OSI与TCP/IP模型对比（填空题/选择题）、协议分类（判断题/选择题）。

---

## 1.1 What is a Computer Network? / 什么是计算机网络？

A **computer network**【计算机网络】is a collection of autonomous computers 【自主计算机的集合】interconnected by a single technology 【通过单一技术互连】. Two computers are said to be interconnected if they can exchange information 【两台计算机如果能交换信息，就说它们互连了】. The connection can be via copper wire 【铜线】, fiber optics 【光纤】, microwaves 【微波】, infrared 【红外】, or communication satellites 【通信卫星】.

> **中文**：计算机网络是通过单一技术互连的自主计算机的集合。两台计算机如果能交换信息，就说它们互连了。连接方式可以是铜线、光纤、微波、红外或通信卫星。

### Network Classification by Transmission Technology / 按传输技术分类

Networks can also be classified by how they transmit data:

| Type / 类型 | Description / 描述 | Example / 示例 |
|------|-------------|---------|
| **Broadcast / Multicast**【广播/多播】 | One transmission reaches all devices on the network / 一次传输到达网络上的所有设备 | Wireless (WiFi), traditional Ethernet |
| **Point-to-Point**【点对点】 | Direct connection between two devices / 两个设备之间的直接连接 | Fiber link between two routers |

> **中文**：网络也可按传输技术分为广播/多播型（一次传输到达所有设备）和点对点型（两个设备间直接连接）。

### Network Classification by Scale / 按规模分类

| Type / 类型 | Full Name / 全称 | Range / 范围 | Example Technologies / 示例技术 |
|------|-----------|-------|---------------------|
| **PAN** | Personal Area Network / 个域网 | < 10 meters / 米 | Bluetooth, ZigBee, NFC |
| **LAN** | Local Area Network / 局域网 | Room, building, campus / 房间、楼宇、校园（几千米） | Ethernet (802.3), Wi-Fi (802.11) |
| **MAN** | Metropolitan Area Network / 城域网 | City-wide / 城市范围 | Cable TV networks, WiMAX (802.16) |
| **WAN** | Wide Area Network / 广域网 | Country, continent, global / 国家、洲际、全球 | MPLS, SD-WAN, Internet backbone |
| **Internet** | Internetwork / Internet / 互联网 | Global / 全球 | TCP/IP protocol suite / TCP/IP协议族 |

### Core Network Technologies / 核心网技术

The **network core**【核心网】consists of interconnected routers that exchange data between networks:

| Technology / 技术 | Description / 描述 |
|-----------|-------------|
| **IP** (Internet Protocol / 网际协议) | Dominant core protocol; provides best-effort packet delivery / 主要的核⼼协议；提供尽力而为的分组投递 |
| **Frame Relay / 帧中继** | WAN technology; simpler than ATM, used for connecting LANs / 广域网技术；比ATM简单，用于连接局域网 |
| **ATM** (Asynchronous Transfer Mode / 异步传输模式) | Uses fixed-length packets (cells); provides QoS guarantees / 使⽤固定长度分组（信元）；提供QoS保证 |

> **中文**：网络核心由互连的路由器组成，在不同⽹络之间交换数据。主要技术包括IP、帧中继和ATM。

### Modem vs Router / 调制解调器与路由器的区别

| Device / 设备      | Function / 功能                                                                                                                                                 |
| ---------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **Modem**【调制解调器】 | Converts digital signals (binary) to/from analog signals (sinusoidal waves) for transmission over communication channels / 将数字信号（⼆进制）与模拟信号（正弦波）相互转换以便通过通信信道传输 |
| **Router**【路由器】  | Connects (sub)networks and exchanges packets between them; decides how/where to forward each packet / 连接（⼦）⽹络并在它们之间交换分组；决定每个分组的转发⽅式                           |

> **中文**：Modem将"计算机能理解的数字信号"与"通信信道能传输的模拟信号"相互转换。路由器连接不同的（子）网络并交换分组。如果只有一个设备要连接，不需要路由器；如果全世界只有一个网络，也不需要路由器（只需要交换机/集线器）。有时路由器和调制解调器集成在一个设备中。

### Best-Effort Model of the Internet / 互联⽹的尽力而为模型

The Internet is a **packet-switched network**【分组交换网络】with a **best-effort**【尽力而为】service model:

> **中文**：互联⽹是⼀个分组交换⽹络，采⽤尽力而为的服务模型。

| Property / 特性 | Meaning / 含义 |
|----------|---------|
| Each packet routed individually | Packets may take different paths / 每个分组独⽴路由，可能⾛不同路径 |
| No guarantee of timely delivery | Delay is variable / 不保证及时送达 |
| No guarantee of delivery order | Packets may arrive out of order / 不保证送达顺序 |
| No guarantee of delivery at all | Packets may be lost / 甚⾄不保证送达 |
| Data integrity may be breached | Transmission errors may occur / 数据完整性可能被破坏 |
| Data may be fragmented | Packets may be split / 数据可能被分⽚ |
| Packets may be duplicated | Duplicate copies may arrive / 分组可能被重复 |

**Implied question / 隐含问题**: If the Internet provides such weak guarantees, how do applications work reliably? **Answer**: Higher-layer protocols (like TCP) add reliability on top of this best-effort foundation.
> **中文**：如果互联⽹只提供如此薄弱的保证，应用程序如何可靠⼯作？**答案**：更⾼层的协议（如TCP）在尽力而为的基础上增加了可靠性。

> **中文**：五种网络规模的划分：
> - **PAN（个域网）**：小于10米，如蓝牙、ZigBee
> - **LAN（局域网）**：房间/楼宇/校园范围，如以太网、Wi-Fi
> - **MAN（城域网）**：城市范围，如有线电视网络
> - **WAN（广域网）**：国家/洲际范围，如MPLS
> - **Internet（互联网）**：全球范围，使用TCP/IP协议族

**Exam Tip (T/F) / 考试提示（判断题）**: The distinction is not just physical scale — behind each classification are different protocol designs and technologies. For example, PAN emphasizes low power consumption with simpler protocols.

> **中文**：这些分类的区别不仅是物理规模——每种分类背后是不同的协议设计和技术。例如PAN（个域网）强调低功耗，使用更简单的协议。

### Tri-Network Convergence (三网合一) — Classic Fill-in-Blank / 经典填空题

Three traditional networks are converging toward a unified IP-based architecture 【三种传统网络正在向统一的基于IP的架构融合】:

| Network Type / 网络类型 | Original Signal / 原始信号 | Key Requirement / 核心要求 | Bandwidth Need / 带宽需求 |
|-------------|-----------------|-----------------|----------------|
| **Telecom Network** (电信网络) | Voice / 语音 | Real-time / 实时性, moderate reliability / 中等可靠性 | Low / 低 |
| **Cable TV Network** (有线电视网络) | Video / 视频 | Real-time / 实时性, moderate reliability / 中等可靠性 | High / 高 |
| **Computer Network** (计算机网络) ★ | Data / 数据 | High reliability / 高可靠性 | Moderate / 中等 |

The **computer network**【计算机网络】plays the core role 【起核心作用】and is developing the fastest 【发展最快】among the three.

> **中文**：三网合一指的是电信网、有线电视网和计算机网络向统一IP架构融合。其中计算机网络起核心作用且发展最快。

---

## 1.2 Why Do We Need Networks? / 为什么需要网络？

### 1.2.1 Key Enablers / 关键驱动因素

**Digitization**【数字化】: Converting analog signals (text, audio, video, image) to binary streams through sampling, quantization, and encoding. This makes information storable and suitable for computer communication.

> **中文**：数字化：通过采样、量化和编码将模拟信号（文本、音频、视频、图像）转换为二进制流。这使得信息可存储并适合计算机通信。

**Economies of Scale**【规模经济】: A network has (almost) fixed costs — the cost per user decreases as the network grows.
> **中文**：规模经济：网络的成本几乎是固定的——随着网络增长，每用户成本降低。

**Metcalfe's Law**【梅特卡夫定律】: The value of a network of size $n$ is proportional to $n^2$ (the number of possible connections).
> **中文**：梅特卡夫定律：规模为 $n$ 的网络的价值与 $n^2$（可能的连接数）成正比。

**Network Externalities**【网络外部性】: As a network's size increases, it attracts more users, which further increases its value — a positive feedback loop.
> **中文**：网络外部性：随着网络规模增大，会吸引更多用户，进一步增加其价值——正反馈循环。

### 1.2.2 Purpose of Networking / 网络化的目的

| Purpose / 目的 | Explanation / 解释 |
|-------|-------------|
| **Resource Sharing / 资源共享** | Share files, data, processors, printers across different computers / 跨不同计算机共享文件、数据、处理器、打印机 |
| **Information Sharing / 信息共享** | IM, video, games, cloud services / 即时通讯、视频、游戏、云服务 |
| **Universal Tool / 通用工具** | Computers become a universal tool available everywhere / 计算机成为无处不在的通用工具 |

---

## 2.1 Network Performance Metrics / 网络性能指标 ★★★ (Big Question Territory / 大题重点)

This is one of the most important sections for **calculation problems**【计算题】in the exam. Master all four types of delay and their relationships.

> **中文**：这是考试中计算题最重要的部分。必须掌握四种时延及其关系。

### 2.1.1 Bandwidth / 带宽

**Bandwidth**【带宽】has two meanings depending on context / 根据上下文有两种含义:

1. **Analog bandwidth / 模拟带宽** (Hz): The range of frequencies a channel can pass 【信道能通过的频率范围】. e.g., a telephone line has bandwidth of ~3 kHz (300 Hz – 3.3 kHz).

> **中文**：模拟带宽指信道能通过的频率范围，单位是赫兹（Hz）。例如电话线的带宽约为3 kHz。

2. **Digital bandwidth / 数字带宽** (bps): The maximum data rate a channel can support 【信道能支持的最高数据率】. **This is the definition used in most exam calculations.【这是考试计算中使用的定义】** Also called **data rate / 数据率** or **bit rate / 比特率**.

> **中文**：数字带宽指信道能支持的最高数据率，单位是bps（比特/秒）。考试中提到的"带宽"几乎都是指数字带宽。

**Key distinction / 关键区别**: Bandwidth ≠ Propagation speed. Bandwidth determines **how fast you can push bits onto the channel** (sending rate), while propagation speed determines **how fast a single bit travels through the channel**.

> **中文**：带宽 ≠ 传播速度。带宽决定你能多快地把比特推到信道上（发送速率），而传播速度决定单个比特在信道中传播的快慢。两者完全独立。

### 2.1.2 Delay (Latency) — Four Types / 时延——四种类型

The total delay from source to destination has four components / 从源到目的地的总时延有四个组成部分:

**Type 1: Transmission Delay / 发送时延（传输时延）**

The time required to **push all bits of a packet onto the link**【把数据包所有比特推到链路上所需的时间】.

$$\boxed{T_{trans} = \frac{\text{Data Length (bits)}}{\text{Bandwidth (bps)}}}$$

> **中文**：发送时延 = 数据长度（比特）/ 带宽（bps）。取决于数据包大小和带宽，与距离无关。

- Depends on **packet size**【数据包大小】and **bandwidth**【带宽】, NOT on distance 【与距离无关】
- Example: Sending a 1000-bit frame on a 100 Mbps link takes: 1000 / 100×10⁶ = 10 μs

> **中文**：示例：在100 Mbps链路上发送1000比特帧需要：1000 / 100×10⁶ = 10微秒。

**Type 2: Propagation Delay / 传播时延**

The time for a single bit to **travel through the physical medium**【一个比特通过物理介质传播所需的时间】.

$$\boxed{T_{prop} = \frac{\text{Distance (m)}}{\text{Propagation Speed (m/s)}}}$$

> **中文**：传播时延 = 距离（米）/ 传播速度（米/秒）。只取决于距离和介质，与数据大小无关。

- Depends on **distance**【距离】and **medium**【介质】, NOT on packet size or bandwidth 【与数据大小和带宽无关】
- Speed of light in vacuum: 3 × 10⁸ m/s 【真空中光速】
- In copper/fiber: roughly 2 × 10⁸ m/s (about 2/3 of light speed) 【铜线/光纤中约为光速的2/3】

**Type 3: Processing Delay / 处理时延**

Time spent by routers/switches to examine packet header, determine output link, and check errors. Usually small (microseconds) and typically **given directly** in exam problems.

> **中文**：路由器/交换机检查包头、确定输出链路、差错校验所花的时间。通常在考试中直接给出数值。

**Type 4: Queuing Delay / 排队时延**

Time a packet spends waiting in output buffer before transmission. The most variable component — depends on network congestion level.

> **中文**：数据包在输出缓冲区等待发送的时间。是最可变的时延成分，取决于网络拥塞程度。

**Total delay formula / 总时延公式**:
$$\boxed{T_{total} = T_{trans} + T_{prop} + T_{proc} + T_{queue}}$$
$$\boxed{\text{总时延} = \text{发送时延} + \text{传播时延} + \text{处理时延} + \text{排队时延}}$$

### 2.1.3 Throughput / 吞吐量

**Throughput**【吞吐量】is the **actual** rate of successful data transfer 【实际成功传输数据的速率】(as opposed to bandwidth, which is the **theoretical maximum**【理论最大值】). Measured in bps.

> **中文**：吞吐量是实际成功传输的速率（而带宽是理论最大值）。瓶颈链路决定端到端吞吐量。

### 2.1.4 Channel Utilization / 信道利用率

Channel utilization $U$ measures the fraction of time the channel is busy 【信道利用率衡量信道忙的时间占比】:

$$U = \frac{T_{trans}}{T_{trans} + 2 \cdot T_{prop} + T_{ack}}$$

> **中文**：信道利用率 = 发送时延 / (发送时延 + 往返传播时延 + ACK发送时延)

For a simpler model / 简化模型 (ignoring ACK transmission time / 忽略ACK发送时间):

$$\boxed{U = \frac{T_D}{T_D + RTT + T_A}}$$

**Key consequence / 关键结论**: High utilization causes high delay 【高利用率导致高延迟】:

$$\boxed{D = \frac{D_0}{1-U}}$$

> **中文**：D₀是信道空闲时的时延，U是利用率。信道利用率并非越高越好——当U→1时，D→∞。

### 2.1.5 Bandwidth-Delay Product / 带宽时延积

$$\boxed{\text{BDP} = \text{Bandwidth} \times \text{RTT}}$$

> **中文**：带宽时延积 = 带宽 × 往返时延。BDP表示"管道"中最多能容纳的比特数，这对于滑动窗口协议设计至关重要（见Ch03和大题专题）。

---

## 3.1 Switching Technologies / 交换技术 ★★★ (Big Question Territory / 大题重点)

Networks can use three fundamental switching approaches. Comparing their delays is a **classic big question**.

> **中文**：网络可以使用三种基本交换方式。比较它们的延迟是经典的考试大题。

### 3.1.1 Circuit Switching / 电路交换

Follows the telephone system model 【遵循电话系统模型】. A dedicated physical path is established before communication 【在通信前建立专用物理路径】.

**Three phases / 三个阶段**:
1. **Circuit establishment / 电路建立**: Setup signal propagates through network, allocating resources hop-by-hop 【建立信号通过网络传播，逐跳分配资源】
2. **Data transfer / 数据传输**: Data flows along the dedicated path without addressing overhead 【数据沿专用路径流动，无寻址开销】
3. **Circuit teardown / 电路释放**: Connection is released 【连接被释放】

**Advantages / 优点**:
- Low transmission delay 【低传输延迟】（建立后无逐跳存储转发）
- Ordered delivery 【有序传输】（数据沿固定路径）
- No collisions 【无冲突】
- Strong real-time capability 【实时性强】

**Disadvantages / 缺点**:
- Long setup time 【建立时间长】
- Line is exclusively occupied 【线路独占】
- Low utilization for intermittent traffic 【间歇性流量利用率低】
- Poor flexibility 【灵活性差】

**Total delay formula / 总时延公式**:
$$T_{circuit} = T_{setup} + \frac{\text{Message Size}}{\text{Bandwidth}} + T_{prop}$$
$$T_{电路交换} = T_{建立} + \frac{\text{报文大小}}{\text{带宽}} + T_{传播}$$

### 3.1.2 Message Switching / 报文交换

The entire message is sent as a unit, stored at each intermediate node, then forwarded to the next. Uses **store-and-forward**【存储转发】at each hop.

> **中文**：整个报文作为一个单元发送，在每个中间节点存储后再转发到下一跳。

**Advantages / 优点**: No connection setup 【无需建立连接】, dynamic route allocation 【动态分配线路】, better line utilization 【更好的线路利用率】

**Disadvantages / 缺点**: Store-and-forward delay at each hop 【每跳存储转发延迟】, requires large buffer 【需要大缓冲区】, extra overhead 【额外开销】

### 3.1.3 Packet Switching / 分组交换 ★★★

Breaks messages into smaller **packets**【分组/包】of uniform size, then transmits each packet independently using store-and-forward. **This is the foundation of the Internet.【这是互联网的基础】**

> **中文**：将报文分割成统一格式的小分组，然后独立传输每个分组。这是互联网的根基。

**Two variants / 两种变体**:

| Aspect / 方面 | Datagram / 数据报 | Virtual Circuit / 虚电路 |
|--------|-------------------|-------------------------|
| Connection setup / 连接建立 | Not needed / 不需要 | Needed (logical connection) / 需要（逻辑连接）|
| Addressing / 寻址 | Each packet carries full destination address / 每个包携带完整目的地址 | Each packet carries VC identifier / 每个包携带VC标识符 |
| Routing / 路由 | Per-packet independent routing / 每个包独立路由 | Fixed path for all packets in VC / VC中所有包走固定路径 |
| State in routers / 路由器状态 | Stateless / 无状态 | Per-VC state maintained / 维护每个VC的状态 |
| Packet order / 包序 | May arrive out of order / 可能乱序到达 | Arrives in order / 有序到达 |

**Total delay for packet switching / 分组交换总时延** (key formula for big questions / 大题核心公式):
$$\boxed{T_{packet} = \frac{x}{b} \times (m + n - 1) + T_{prop}}$$

> **中文**：x = 分组大小（比特），b = 链路带宽（bps），m = 分组数量，n = 跳数（中间路由器数），T_prop = 总传播时延。第一个分组需要n次传输穿越网络，之后每个分组隔x/b时间到达一个。

### 3.1.4 System Capacity Comparison: Circuit vs Packet / 系统容量对比：电路交换vs分组交换

**Classic example / 经典例题**: A router/link with 1 Mbps capacity. Each user uses 100 kbps when active, and is active 10% of the time.

> **中文**：一个1 Mbps的链路，每个用户活跃时使用100 kbps，活跃时间占10%。

**Circuit switching / 电路交换**: Users get dedicated channels:
- Max users = 1 Mbps / 100 kbps = **10 users** (hard limit / 硬限制)

**Packet switching / 分组交换**: Users share bandwidth on demand:
- Capacity is based on statistical multiplexing — can admit far more users
- Packet switching admits more users because resource is shared

> **中文**：电路交换最多10个用户（硬限制）；分组交换基于统计复用，可以接纳更多用户。

### 3.1.4 Comparison Table / 三种交换对比 (Must Memorize / 必须记住)

| Aspect / 方面 | Circuit / 电路交换 | Message / 报文交换 | Packet / 分组交换 |
|--------|:---:|:---:|:---:|
| Dedicated path / 专用路径 | Yes / 是 | No / 否 | No (shared / 共享) |
| Connection setup / 建立连接 | Required / 需要 | No / 否 | Datagram: No / VC: Yes |
| Store-and-forward / 存储转发 | No (end-to-end) | Yes / 是 | Yes / 是 |
| Bandwidth utilization / 带宽利用 | Low / 低 | Medium / 中 | High / 高 |
| Suitability / 适用场景 | Real-time, constant traffic / 实时恒定流量 | — | Bursty data traffic / 突发数据流量 |

---

### 3.1.5 Circuit vs Packet: Detailed Comparison / 电路交换与分组交换详细对比 (from PPT / 来自课件)

| Aspect / 方面 | Circuit Switching / 电路交换 | Packet Switching / 分组交换 |
|--------|----------------------|---------------------|
| **Delay / 时延** | Constant (fixed) / 恒定 | Variable (queuing) / 可变 |
| **Order / 顺序** | Data arrives in order / 有序到达 | Packets may arrive out of order / 可能乱序 |
| **BW Efficiency / 带宽效率** | Inefficient (dedicated) / 低效（专用） | Efficient (on-demand) / 高效（按需） |
| **Complexity / 复杂度** | Low / 低 | High / 高 |
| **Routing / 路由** | Simple (fixed path) / 简单（固定路径） | Complex (per-packet decision) / 复杂（每包决策） |
| **QoS / 服务质量** | "All or nothing" / "要么全有要么全无" | Graceful degradation / 优雅降级 |
| **Congestion / 拥塞** | No congestion (dedicated) / 无拥塞 | Possible (shared resource) / 可能发生 |
| **Scenario / 适用场景** | Voice communication / 语音通信 | Data communication / 数据通信 |

> **中文**：电路交换时延恒定、有序、低效但简单；分组交换时延可变、可能乱序、高效但复杂。注意：VoIP（网络电话）已经模糊了这一界限——"May not hold any more"。

---

## 4.1 Network Architecture: Protocol Hierarchies / 协议层次结构

### 4.1.1 Core Concepts / 核心概念

**Protocol / 协议**: A set of rules governing the format and meaning of the information exchanged between **peer entities**【对等实体】within the same layer.

> **中文**：协议是同一层中对等实体之间交换信息的格式和含义的规则集合。协议定义了三个要素：

| Element / 要素 | Meaning / 含义 | Example / 示例 |
|---------|---------|---------|
| **Syntax / 语法** | Structure/format of data and control information / 数据和控制信息的结构格式 | TCP segment header format / TCP段头格式 |
| **Semantics / 语义** | What control information means, what actions to take / 控制信息的含义和应执行的动作 | ACK means "acknowledge receipt" / ACK表示确认收到 |
| **Timing / 时序** | Order and speed of events / 事件的顺序和速度 | SYN → SYN-ACK → ACK sequence / 三次握手顺序 |

**Service / 服务**: The functionality that a layer provides to the layer **above** it. A service is **vertical** (between layers) 【服务是垂直的（层间关系）】.

**Interface / 接口**: The way a lower layer exposes its services to the upper layer 【下层向上层暴露服务的方式】.

**Why Layering? / 为什么要分层？**
Computer networks are **complex systems**【复杂系统】. Layering helps in two ways:
> **中文**：计算机网络是复杂系统。分层在两个维度上帮助管理复杂性：

- **Horizontally / 水平方向**: Divide large networks into WAN, MAN, LAN / 将大型网络划分为WAN、MAN、LAN
- **Vertically / 垂直方向**: Finer granularity through protocol hierarchy / 通过协议层次结构实现更细粒度

| Benefit / 好处 | Explanation / 说明 |
|---------|-------------|
| **Explicit structure / 明确结构** | Easy to identify relationships between parts / 易于识别各部分之间的关系 |
| **Modularization / 模块化** | Easy to maintain/update the system ("Divide and Conquer" / 分而治之) / 易于维护/更新系统 |
| **Add new function? Add a new layer** | A well-known programmer saying / 著名程序员格言 |

> **中文**：分层的好处：明确的结构使各部分关系清晰；模块化使系统易于维护和更新（分而治之）；"需要新功能？加一个新层。"

### 4.1.4 Design Objectives of Protocols / 协议的设计目标

The layered protocols must fulfill several key design objectives / 分层协议必须实现几个关键设计目标：

| Objective / 目标 | Description / 描述 |
|-----------|-------------|
| **Addressing / 寻址** | How to identify a host/server? / 如何标识一台主机/服务器？ |
| **Routing / 路由** | Which way to choose for a packet from source to destination? / 为分组选择从源到目的地的路径？ |
| **Error Control / 差错控制** | How to detect and correct errors in transmission? / 如何检测和纠正传输中的错误？ |
| **Flow Control / 流量控制** | How to keep a fast sender from overloading a slow receiver? / 如何防止快速发送方压垮慢速接收方？ |
| **Multiplexing / 多路复用** | How to share the same medium among multiple users? / 多个用户如何共享同一介质？ |

> **中文**：协议的设计目标包括：寻址（标识主机/服务器）、路由（选择路径）、差错控制（检测/纠正错误）、流量控制（防止发送方过载接收方）、多路复用（多个用户共享介质）。

**Key principle / 核心原则**: "Protocols are **horizontal / 水平的** (peer-to-peer / 对等实体间), services are **vertical / 垂直的** (layer-to-layer / 层间)."

> **中文**：协议是"水平的"（同一层之间），服务是"垂直的"（上下层之间）。协议的实现保证了能够向上一层提供服务。

### 4.1.2 Encapsulation and Decapsulation / 封装与解封装

When data travels **down** the protocol stack (sender side) / 数据向下经过协议栈时（发送方）:
1. Application layer creates message / 应用层创建消息
2. Transport layer adds transport header → **Segment / 段**
3. Network layer adds network header → **Packet / 分组**
4. Data Link layer adds frame header + trailer → **Frame / 帧**
5. Physical layer converts to bits for transmission / 物理层转换为比特流传输

> **中文**：发送方从上到下逐层封装——每层在来自上层的数据前加上自己的头部（有的还加尾部）。接收方从下到上逐层解封——每层剥离对应头部后将载荷交给上层。

**Important / 重要**: Lower layers treat ALL data from upper layers as **payload**【载荷/净荷】— they cannot interpret upper layer headers 【下层不能解析上层头部】. This is the essence of layering 【这就是分层的本质】.

**Protocol efficiency / 协议效率**:
$$\text{Efficiency / 效率} = \frac{\text{Payload Size / 载荷大小}}{\text{Payload Size + Header Size / 载荷+头部}}$$

### 4.1.3 Connection-Oriented vs Connectionless / 面向连接 vs 无连接

| Aspect / 方面 | Connection-Oriented / 面向连接 | Connectionless / 无连接 |
|--------|---------------------|----------------|
| Modeled after / 类比 | Telephone system / 电话系统 | Postal system / 邮政系统 |
| Connection setup / 建立连接 | Required / 需要 | Not needed / 不需要 |
| Ordering / 排序 | Guaranteed in-order / 保证有序 | May arrive out of order / 可能乱序 |
| Reliability / 可靠性 | Usually reliable / 通常可靠 | Usually unreliable / 通常不可靠 |
| Example / 示例 | TCP | UDP, IP |

---

## 5.1 OSI Reference Model / OSI参考模型 (7 Layers / 七层) ★★★

The **Open Systems Interconnection**【开放系统互连】model was developed by ISO 【由ISO制定】. It defines 7 layers. While **not actually implemented**【没有实际实现】, it is the **theoretical standard**【理论标准】and frequently tested 【经常考试】.

> **中文**：OSI模型是ISO制定的理论标准，有7层。虽然实际上没有产品实现这7层，但它是考试重点。

### Layer-by-Layer Breakdown / 逐层详解

| # | Layer / 层 | English / 英文 | PDU Name | Core Function / 核心功能 |
|---|-------|-------------|----------|---------------|
| 7 | 应用层 | Application | APDU | Provides network services to user applications (HTTP, FTP, SMTP, DNS) / 为用户应用提供网络服务 |
| 6 | 表示层 | Presentation | PPDU | Data translation, encryption, compression / 数据转换、加密、压缩 |
| 5 | 会话层 | Session | SPDU | Establish, manage, terminate sessions / 建立、管理、终止会话 |
| 4 | 传输层 | Transport | TPDU / Segment / 段 | End-to-end reliable delivery / 端到端可靠传输 |
| 3 | 网络层 | Network | Packet / 分组 | Routing between networks / 网络间路由 |
| 2 | 数据链路层 | Data Link | Frame / 帧 | Node-to-node delivery; framing / 节点到节点传输；成帧 |
| 1 | 物理层 | Physical | Bit / 比特 | Raw bit transmission over medium / 通过介质传输原始比特 |

### Memory Aid for OSI Layers / 七层记忆口诀 (Top to Bottom / 从上到下)

> **A**ll **P**eople **S**eem **T**o **N**eed **D**ata **P**rocessing
> (Application, Presentation, Session, Transport, Network, Data Link, Physical)
>
> **中文**：记忆口诀："All People Seem To Need Data Processing"（所有人似乎都需要数据处理），对应：应用层、表示层、会话层、传输层、网络层、数据链路层、物理层。

### Key Exam Points for OSI / OSI考试要点

1. **Data Link Layer** provides **hop-to-hop / 跳到跳** delivery; **Network Layer** provides **host-to-host / 主机到主机** delivery; **Transport Layer** provides **process-to-process / 进程到进程** delivery.

2. Routers operate at **Layer 3**【路由器工作在第3层】(Network); Switches operate at **Layer 2**【交换机工作在第2层】(Data Link); Hubs operate at **Layer 1**【集线器工作在第1层】(Physical).

3. The **Transport Layer** is the lowest end-to-end layer 【传输层是最低的端到端层】— it is only processed at source and destination, not by intermediate routers.

> **中文**：路由器在第3层工作（查看IP头做转发决策），交换机在第2层工作（查看MAC地址），集线器在第1层（纯信号放大）。传输层只在端系统处理，中间路由器不处理传输层。

---

## 6.1 TCP/IP Reference Model / TCP/IP参考模型 (4 Layers / 四层)

The TCP/IP model is the **de facto standard**【事实上的标准】actually used in the Internet 【互联网实际使用的模型】. It has only 4 layers / 只有4层:

| # | Layer / 层 | Key Protocols / 主要协议 | Function / 功能 |
|---|-------|---------------|----------|
| 4 | **Application / 应用层** | HTTP, FTP, SMTP, DNS | User-facing services / 面向用户的服务 |
| 3 | **Transport / 传输层** | TCP (reliable / 可靠), UDP (unreliable / 不可靠) | End-to-end process delivery / 端到端进程传输 |
| 2 | **Internet / 网际层** | IP, ICMP, ARP, IGMP | Packet routing across networks / 跨网络分组路由 |
| 1 | **Link / 网络接口层** | Ethernet, Wi-Fi, PPP | Physical transmission / 物理传输 |

### OSI vs TCP/IP Comparison / OSI与TCP/IP对比

```
OSI (7层):                         TCP/IP (4层):
+-------------+              +-------------+
| Application |  (7)         |             |
+-------------+              | Application |  (4)  ← 合并了OSI的上三层
| Presentation|  (6)         |             |
+-------------+              +-------------+
| Session     |  (5)         |             |
+-------------+              | Transport   |  (3)  ← 对应OSI传输层
| Transport   |  (4)         |             |
+-------------+              +-------------+
| Network     |  (3)         | Internet    |  (2)  ← 对应OSI网络层
+-------------+              +-------------+
| Data Link   |  (2)         | Link        |  (1)  ← 合并了OSI下两层
+-------------+              |             |
| Physical    |  (1)         |             |
+-------------+              +-------------+
```

| Aspect / 方面 | OSI (7-Layer / 七层) | TCP/IP (4-Layer / 四层) |
|--------|:-----------:|:-------------:|
| Origin / 来源 | ISO (theoretical / 理论的) | DARPA (practical / 实践的) |
| Status / 地位 | Legal international standard / 法律上的国际标准 | De facto industry standard / 事实上的工业标准 |
| Layers / 层数 | 7 | 4 |
| Presentation & Session / 表示层与会话层 | Separate layers / 独立层 | Merged into Application / 合并到应用层 |
| Adoption / 实际使用 | Rarely implemented / 很少实现 | Universally used / 全球使用 |
| Standards vs Protocols / 标准与协议关系 | Standards before protocols / 先有标准后有协议 | Protocols before standards (description of existing protocols) / 先有协议后有标准（对现有协议描述） |
| Protocol/Service/Interface / 协议/服务/接口 | Clearly distinguished / 清晰区分 | No clear distinction / 区分不明显 |
| Network layer (L3) / 网络层 | Both connectionless & connection-oriented / 同时支持无连接和面向连接 | Connectionless only (IP) / 仅支持无连接 |
| Transport layer (L4) / 传输层 | Connection-oriented only / 仅面向连接 | Both connectionless (UDP) & connection-oriented (TCP) / 同时支持无连接和面向连接 |

### OSI Critique / OSI模型的批评

Despite being comprehensive, the OSI model and its protocols were **never widely adopted**【从未被广泛采用】:

> **中文**：尽管OSI模型很全面，但从未被广泛采用。

| Issue / 问题 | Explanation / 说明 |
|-------|-------------|
| **Bad Timing / 时机不佳** | OSI investment fell behind research; TCP/IP was already working / OSI投资落后于研究；TCP/IP已经在运行 |
| **Bad Technology / 技术不佳** | Political rather than technical decisions; empty presentation and session layers; difficult to understand or implement / 政治而非技术决策；空的表示层和会话层；难以理解或实现 |
| **Bad Implementations / 实现不佳** | Huge and slow; the free Berkeley UNIX TCP/IP implementation beat OSI / 庞大且缓慢；免费的Berkeley UNIX TCP/IP实现击败了OSI |
| **Bad Politics / 政治不佳** | EU vs US ministries vs academia — more political than technical / 欧盟vs美国政府vs学术界——更多是政治而非技术 |

> **中文**：OSI的失败原因：时机不佳（投入落后于研究）、技术不佳（政治而非技术决策、空的表示层和会话层）、实现不佳（庞大且缓慢）、政治不佳（政治争斗）。

### TCP/IP Critique / TCP/IP模型的批评

The TCP/IP model also has its shortcomings / TCP/IP模型也有其缺点：

| Issue / 问题 | Explanation / 说明 |
|-------|-------------|
| Service/Interface confusion / 服务/接口混淆 | Does not clearly distinguish between protocol, service, and interface / 没有清晰区分协议、服务和接口 |
| Not a general model / 不是通用模型 | Designed specifically for TCP/IP suite, hard to describe other protocol stacks / 专门为TCP/IP协议族设计，难以描述其他协议栈 |
| Link layer issues / 链路层问题 | Actually a "host-to-network" interface, not a true layer; doesn't distinguish physical and data link layers / 实际上是"主机到网络"接口，不是真正的层；不区分物理层和数据链路层 |
| Entrenched protocols / 根深蒂固的协议 | TCP/IP are well developed but other protocols (e.g. TELNET designed for 10-char/s mechanical terminals) are still in use and hard to replace / TCP/IP很成熟但其他协议（如为10字符/秒机械终端设计的TELNET）仍在用且难以替换 |

### Hybrid 5-Layer Model Used in This Course / 本课程使用的混合五层模型

This course uses a **hybrid model**【混合模型】that combines the strengths of both OSI and TCP/IP:

> **中文**：本课程使用一个结合OSI和TCP/IP优点的混合模型。

| Layer / 层 | Function / 功能 | Protocol Example / 协议示例 | PDU / 数据单元 |
|-------|----------|----------------|:---:|
| **Application / 应用层** | Supporting network applications / 支持网络应用 | HTTP, FTP, SMTP, DNS, TELNET | Message / 消息 |
| **Transport / 传输层** | Process-to-process data transfer / 进程到进程数据传输 | TCP (reliable), UDP (best-effort) | Segment / 段; Packet / 分组 |
| **Network / 网络层** | Routing from source to destination / 源到目的地的路由 | IP, routing protocols | Datagram / 数据报 |
| **Link / 链路层** | Data transfer between neighboring network elements / 相邻网络元素间数据传输 | Ethernet (802.3 MAC), WiFi (802.11 MAC) | Frame / 帧 |
| **Physical / 物理层** | Raw bits over communication channel / 通过通信信道传输原始比特 | Ethernet (802.3 PHY), WiFi (802.11 PHY) | Bits / 比特 |

---

### Cross-layer Design / 跨层设计

**Concept**: In wireless/ad-hoc networking (popular after 2000), strict layering is sometimes violated to improve performance.

> **中文**：在无线/自组网中（2000年后流行），有时违反严格分层以提升性能。

| Aspect / 方面 | Traditional Layering / 传统分层 | Cross-layer Design / 跨层设计 |
|--------|------------------------|-----------------------|
| Information / 信息 | Each layer has limited info / 每层信息有限 | More information available to decision makers / 决策者拥有更多信息 |
| Efficiency / 效率 | May duplicate procedures / 可能有重复流程 | Eliminates some procedures / 消除一些流程 |
| Performance / 性能 | May be suboptimal / 可能非最优 | Usually improves performance / 通常提升性能 |

**Problem / 问题**: Cross-layer design **violates the modularity principle**【违反模块化原则】, making the system harder to maintain. Trade-off between performance and maintainability.

### Cross-cutting Topics / 横切主题

**Software Defined Network (SDN) / 软件定义网络**: Network Function Virtualization (NFV) separates the control plane from the data plane, enabling programmable networks.
> **中文**：SDN/软件定义网络：网络功能虚拟化（NFV）将控制平面与数据平面分离，实现可编程网络。

---

## 7.1 History and Standardization / 网络历史与标准化

### 7.1.1 Brief History of the Internet / 互联网简史

| Period / 时期 | Key Events / 关键事件 |
|----------|-------------|
| **1961-1972**: Early packet-switching | 1961: Kleinrock — queueing theory shows effectiveness of packet-switching; 1964: Baran — packet switching in military networks; 1967: ARPAnet conceived by ARPA; 1969: First ARPAnet node operational; 1972: ARPAnet public demo, first email program, NCP (Network Control Protocol) |
| **1972-1980**: Internetworking & new nets | 1970: ALOHAnet satellite network (Hawaii); 1974: Cerf & Kahn — architecture for interconnecting networks; 1976: Ethernet at Xerox PARC; Late 70's: proprietary architectures (DECnet, SNA, XNA); ATM precursor (fixed-length packets); 1979: ARPAnet has 200 nodes |
| **1980-1990**: New protocols proliferation | 1983: TCP/IP deployment; 1982: SMTP email protocol; 1983: DNS (name-to-IP); 1985: FTP protocol; 1988: TCP congestion control |
| **1990-2000's**: Commercialization & Web | Early 1990's: ARPAnet decommissioned → Internet; Web: hypertext (Bush 1945, Nelson 1960's), HTML, HTTP; Late 1990's: web commercialization; IM, P2P file sharing; network security; 50M hosts, 100M users; Internet backbone at Gbps |
| **After 2005**: Mobile Internet | Smartphones, broadband access, high-speed wireless; Social networks (Facebook, WeChat), E-commerce (Taobao, Amazon), Video streaming (YouTube, Netflix), Gaming, Cloud |
| **Future**: Ongoing directions | Vehicular Network (IoV / 车联网); Edge Computing Network (边缘计算网络) |

> **中文**：互联网从1960年代的ARPAnet发展而来，经历了分组交换诞生、互联网络构建、TCP/IP部署、Web商业化、移动互联网等阶段，正在向车联网和边缘计算演进。

### 7.1.2 Standardization / 标准化

Internet standards are documented as **RFCs (Request for Comments)**【请求评议】:

> **中文**：互联网标准以RFC文档形式发布。

| Standard Level / 标准级别 | Description / 描述 |
|-----------------|-------------|
| **Internet Draft / 互联网草案** | Initial working document / 初始工作草案 |
| **Proposed Standard / 建议标准** | Stable, well-understood / 稳定且充分理解 |
| **Draft Standard / 草案标准** | At least 2 independent interoperable implementations / 至少2个独立可互操作实现 |
| **Internet Standard / 互联网标准** | Mature standard; assigned an STD number / 成熟的标准；分配STD编号 |
| **Historic / 历史标准** | Superseded or no longer used / 被取代或不再使用 |

**Standards Organizations / 标准化组织**:
- **3GPP** (3rd Generation Partnership Project): Cellular network standards (3G/4G/5G) / 蜂窝网络标准
- **IEEE**: LAN/MAN standards (Ethernet 802.3, WiFi 802.11) / 局域网/城域网标准
- **IETF**: Internet standards (RFCs) / 互联网标准

> **中文**：主要标准化组织包括：3GPP（蜂窝网络）、IEEE（局域网/城域网）、IETF（互联网RFC标准）。

---

## 7.2 Protocol Stack Quick Reference / 各层协议速查

| Layer / 层 | Protocols / 协议 | Key Port/Protocol Numbers / 端口/协议号 |
|-----------|--------------------------|:--:|
| **Application / 应用层** | HTTP, HTTPS, FTP, SMTP, POP3, IMAP, DNS, DHCP, Telnet, SSH | HTTP=80, HTTPS=443, FTP=21, SMTP=25, DNS=53, SSH=22 |
| **Transport / 传输层** | TCP, UDP | TCP: Protocol# 6, UDP: Protocol# 17 |
| **Network / 网络层** | IP, ICMP, IGMP, ARP, RARP | ICMP: Protocol# 1, OSPF: Protocol# 89 |
| **Data Link / 数据链路层** | SDLC, HDLC, PPP, Ethernet (802.3), Wi-Fi (802.11) | — |
| **Physical / 物理层** | Manchester encoding, NRZ, 802.3 physical specs | — |

> **中文**：这张表列出了各层的主要协议。考试填空题常考"XX协议属于哪一层"，记住这张表就行。

---

## 8. Exam Practice / 本章练习题

### Fill-in-the-Blank / 填空题

1. The three elements of a network protocol are ______, ______, and ______. **(syntax / 语法, semantics / 语义, timing / 时序)**

> **中文**：网络协议三要素：语法（数据格式）、语义（控制信息含义）、时序（事件顺序）。

2. The OSI model has ______ layers; the TCP/IP model has ______ layers. **(7, 4)**

> **中文**：OSI模型有7层；TCP/IP模型有4层。

3. In the OSI model, ______ layer is responsible for routing packets between networks. **(Network / 网络层)**

> **中文**：OSI模型中负责网络间路由的是网络层（第3层）。

4. ______ switching requires connection setup before data transmission. **(Circuit / 电路交换)**

> **中文**：电路交换需要在数据传输前先建立连接。

5. The PDU at the data link layer is called a ______. **(frame / 帧)**; at the network layer it's a ______. **(packet / 分组)**; at the transport layer it's a ______. **(segment / 段 or TPDU)**

> **中文**：数据链路层的PDU称为帧（frame）；网络层的PDU称为分组（packet）；传输层的PDU称为段（segment）或TPDU。

6. The delay caused by pushing all bits of a packet onto the link is called ______ delay. **(transmission / 发送时延)**

> **中文**：将数据包所有比特推到链路上产生的时延称为发送时延（transmission delay）。

7. The delay caused by signal traveling through the medium is called ______ delay. **(propagation / 传播时延)**

> **中文**：信号通过介质传播所产生的时延称为传播时延（propagation delay）。

8. PAN stands for ______; WAN stands for ______. **(Personal Area Network, Wide Area Network)**

> **中文**：PAN代表个域网（Personal Area Network）；WAN代表广域网（Wide Area Network）。

9. In TCP/IP, the transport layer includes two main protocols: ______ (connection-oriented / 面向连接) and ______ (connectionless / 无连接). **(TCP, UDP)**

> **中文**：在TCP/IP中，传输层包括两个主要协议：TCP（面向连接）和UDP（无连接）。

10. The bandwidth-delay product represents the maximum number of ______ that can be "in flight". **(bits / 比特)**

> **中文**：带宽时延积 = 带宽 × RTT，表示管道中同时在路上的最大比特数。

### True/False / 判断题

1. (T/F) TCP/IP's application layer corresponds exactly to OSI's application layer. **(F)** — TCP/IP application layer includes functions of OSI's application, presentation, and session layers.

> **中文**：错。TCP/IP的应用层对应OSI的应用层+表示层+会话层。

2. (T/F) Circuit switching provides higher line utilization than packet switching. **(F)** — packet switching has higher utilization.

> **中文**：错。分组交换的线路利用率更高（因为多用户共享链路）。

3. (T/F) Propagation delay depends on the size of the data being sent. **(F)** — depends only on distance and medium.

> **中文**：错。传播时延只取决于距离和介质，与数据大小无关。

4. (T/F) A router processes all layers up through the transport layer. **(F)** — router only processes up through network/internet layer.

> **中文**：错。路由器最多只处理到网络层（第3层），不处理传输层。

5. (T/F) High channel utilization always improves network performance. **(F)** — very high utilization causes exponentially increasing delays: $D = D_0/(1-U)$.

> **中文**：错。利用率趋近1时延迟趋近无穷大。

6. (T/F) Baud rate is always equal to bit rate. **(F)** — bit rate = baud rate × log₂(V), equal only when V=2.

> **中文**：错。波特率等于比特率仅在二进制信号(V=2)时成立。

### Multiple Choice / 单选题

1. Which of the following network scales covers a city? / 哪种网络覆盖城市范围？
   a) PAN b) LAN c) MAN d) WAN **(c — MAN / 城域网)**

2. In circuit switching, which phase occurs first? / 电路交换最先进行哪个阶段？
   a) Data transfer b) Circuit establishment c) Circuit teardown d) Routing **(b — 先建立电路)**

3. The PDU at the transport layer in OSI is called: / 传输层的数据单元叫？
   a) Packet b) Frame c) TPDU d) Bit **(c — TPDU / 传输协议数据单元)**

4. Which protocol operates at the transport layer? / 哪个是传输层协议？
   a) IP b) HTTP c) TCP d) Ethernet **(c — TCP)**

5. Which delay depends on packet size and bandwidth? / 哪种时延取决于包大小和带宽？
   a) Propagation delay b) Transmission delay c) Processing delay d) Queuing delay **(b — 发送时延)**

6. Which layer provides end-to-end (process-to-process) communication? / 哪一层提供端到端（进程到进程）通信？
   a) Network layer b) Data link layer c) Physical layer d) Transport layer **(d — 传输层)**

### Calculation Problems / 计算题

**Problem 1 / 例题1**: A file of 1 MB (10⁶ bytes) needs to be sent over a 1 Mbps link with 10 ms propagation delay. Calculate total delay using: (a) Circuit switching with 100 ms setup time (b) Packet switching with 1000-byte packets, 3 routers (4 hops), 20-byte header per packet.

> **中文**：1MB文件通过1Mbps链路发送，传播时延10ms。计算：(a)电路交换（建立时间100ms）(b)分组交换（1000字节分组，3个路由器即4跳，每包20字节头部）的延迟。

**Solution / 解答**:
(a) Circuit switching / 电路交换:
- Setup / 建立: 100 ms = 0.1 s
- Transmission / 传输: 10⁶ bytes = 8×10⁶ bits; 8×10⁶ / 10⁶ = 8 s
- Propagation / 传播: 10 ms = 0.01 s
- Total / 总计 = 0.1 + 8 + 0.01 = **8.11 s**

(b) Packet switching / 分组交换:
- Data per packet / 每包数据 = 1000 - 20 = 980 bytes
- Number of packets / 分组数 = ⌈10⁶ / 980⌉ ≈ 1021 packets
- Each packet / 每包: 1000 bytes = 8000 bits
- Per-hop transmission / 每跳传输: 8000 / 10⁶ = 0.008 s = 8 ms
- First packet through 4 hops / 第一包过4跳: 8 × 4 = 32 ms
- Remaining 1020 packets trailing / 后续1020包: 1020 × 8 = 8160 ms
- Plus propagation / 加上传播: 10 ms
- Total / 总计 ≈ 32 + 8160 + 10 = 8202 ms = **8.20 s**

> **中文**：两种方式的延迟很接近。电路交换无逐跳存储转发延迟但需建立时间；分组交换通过流水线并行传输。

**Problem 2 / 例题2** (Channel Utilization / 信道利用率): A satellite channel has 50 kbps bandwidth. Frame size is 1000 bits. Uplink and downlink propagation delays are each 125 ms. Calculate utilization using stop-and-wait.

> **中文**：卫星信道50kbps，帧长1000bit，上下行传播时延各125ms。计算停-等协议的信道利用率。

**Solution / 解答**:
- $T_D = 1000 / (50 \times 10^3) = 0.02$ s = 20 ms 【发送时延】
- RTT = 2 × (125 + 125) = 500 ms 【往返时延】
- $U = T_D / (T_D + RTT) = 20 / (20 + 500) = 20/520 \approx 0.0385 = \boxed{3.85\%}$
- 利用率仅3.85%！这说明为什么停-等协议在高延迟链路上效率极低——因此需要滑动窗口协议。

---

## Appendix: Chinese-English Terminology / 中英文术语对照

### Network Classification / 网络分类
| English / 英文 | Chinese / 中文 |
|------|------|
| Personal Area Network (PAN) | 个域网 |
| Local Area Network (LAN) | 局域网 |
| Metropolitan Area Network (MAN) | 城域网 |
| Wide Area Network (WAN) | 广域网 |
| Internet / Internetwork | 互联网 |

### Performance Metrics / 性能指标
| English / 英文 | Chinese / 中文 |
|------|------|
| Bandwidth | 带宽 |
| Transmission delay | 发送时延/传输时延 |
| Propagation delay | 传播时延 |
| Processing delay | 处理时延 |
| Queuing delay | 排队时延 |
| Throughput | 吞吐量 |
| Channel utilization | 信道利用率 |
| Bandwidth-delay product (BDP) | 带宽时延积 |
| Round-Trip Time (RTT) | 往返时延 |

### Switching / 交换技术
| English / 英文 | Chinese / 中文 |
|------|------|
| Circuit switching | 电路交换 |
| Message switching | 报文交换 |
| Packet switching | 分组交换 |
| Store-and-forward | 存储转发 |
| Datagram | 数据报 |
| Virtual circuit | 虚电路 |

### Protocol Architecture / 协议体系结构
| English / 英文 | Chinese / 中文 |
|------|------|
| Protocol | 协议 |
| Syntax | 语法 |
| Semantics | 语义 |
| Timing | 时序 |
| Protocol Data Unit (PDU) | 协议数据单元 |
| Service | 服务 |
| Interface | 接口 |
| Encapsulation | 封装 |
| Payload | 净荷/有效载荷 |
| Peer entity | 对等实体 |
| Connection-oriented | 面向连接 |
| Connectionless | 无连接 |

### OSI 7-Layer Model / OSI七层模型
| Layer / 层 | English / 英文 | PDU | Chinese / 中文 |
|:---:|------|:---:|------|
| 7 | Application | APDU | 应用层 |
| 6 | Presentation | PPDU | 表示层 |
| 5 | Session | SPDU | 会话层 |
| 4 | Transport | TPDU / Segment | 传输层 |
| 3 | Network | Packet | 网络层 |
| 2 | Data Link | Frame | 数据链路层 |
| 1 | Physical | Bit | 物理层 |

### TCP/IP 4-Layer Model / TCP/IP四层模型
| Layer / 层 | English / 英文 | Key Protocols / 主要协议 |
|:---:|------|------|
| 4 | Application / 应用层 | HTTP, FTP, SMTP, DNS |
| 3 | Transport / 传输层 | TCP, UDP |
| 2 | Internet / 网际层 | IP, ICMP, ARP |
| 1 | Link / 网络接口层 | Ethernet, Wi-Fi, PPP |
