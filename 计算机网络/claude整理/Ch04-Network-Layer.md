# Chapter 04: Network Layer / 第四章：网络层

> **Exam Relevance / 考试重点**：This is the largest and most exam-heavy chapter. Key big question topics include: broadcast routing algorithm (group count calculation), IP fragmentation and reassembly, CIDR addressing/subnetting, NAT translation, WFQ packet scheduling, leaky/token bucket traffic shaping, M/M/1 queuing model, and routing protocol comparison (RIP/OSPF/BGP). Special IP addresses are a fill-in-the-blank favorite.
>
> **中文**：这是最大且考试最密集的章节。关键的大题主题包括：广播路由算法（分组计数计算）、IP分片与重组、CIDR寻址/子网划分、NAT转换、WFQ分组调度、漏桶/令牌桶流量整形、M/M/1排队模型以及路由协议比较（RIP/OSPF/BGP）。特殊IP地址是填空题的常考内容。

---

## 4.1 Network Layer Design Issues / 网络层设计问题

### 4.1.1 Services Provided to Transport Layer / 提供给传输层的服务

The network layer provides **host-to-host**【主机到主机】 delivery. Its service should be independent of the underlying network technology.

> **中文**：网络层提供**主机到主机**的交付服务。其服务应独立于底层网络技术。

Two fundamentally different service models:

> **中文**：两种根本不同的服务模型：

| Aspect / 方面 | Datagram (Connectionless) / 数据报（无连接） | Virtual Circuit (Connection-Oriented) / 虚电路（面向连接） |
|--------|:---:|:---:|
| Connection setup / 连接建立 | None / 无需建立 | Required / 需要建立 |
| Addressing / 寻址方式 | Each packet carries full source/dest address / 每个分组携带完整的源/目标地址 | Each packet carries VC identifier / 每个分组携带虚电路标识符 |
| State in routers / 路由器状态 | Stateless (no per-flow state) / 无状态（无每流状态） | Stateful (per-VC table entry) / 有状态（每虚电路表项） |
| Routing / 路由决策 | Per-packet independent routing decisions / 每个分组独立路由决策 | All packets follow same path / 所有分组走相同路径 |
| Robustness / 健壮性 | Adapts to failures automatically / 自动适应故障 | Must re-establish VC on failure / 故障时必须重建虚电路 |
| Quality of Service / 服务质量 | Harder / 较难实现 | Easier (resources can be reserved) / 较易实现（可预留资源） |
| Example / 典型示例 | **The Internet (IP)** / **互联网（IP）** | ATM, **MPLS**, Frame Relay |

**Note on Connection-Oriented / 关于面向连接的补充**：
- Virtual circuit routing is also called **label-switching**【标签交换】
  > **中文**：虚电路路由也称为**标签交换**
- **MPLS (Multiprotocol Label Switching)**【多协议标签交换】：
  - A modern implementation of connection-oriented switching at Layer 2.5 (between L2 and L3)
  - Uses labels instead of IP addresses for fast forwarding through the network
  - Commonly used in ISP backbone networks for traffic engineering

> **中文**：MPLS是一种现代的面向连接交换技术，工作在第2.5层，使用标签代替IP地址进行快速转发，广泛用于ISP骨干网中的流量工程。

**The Internet uses the datagram model (IP)**. It provides a **best-effort**【尽力而为】, **unreliable**【不可靠的】, **connectionless**【无连接的】 service.

> **中文**：**互联网使用数据报模型（IP）**。它提供**尽力而为**、**不可靠**、**无连接**的服务。

### 4.1.2 Design Principles of Names and Addresses / 名称和地址的设计原则

Three design principles for IP addresses (from the PPT):

> **中文**：IP地址的三个设计原则：

1. **No hardcoded addresses / 无硬编码地址**
   - IP address can change (e.g., dial-in, mobile hosts) — unlike MAC addresses which are hardcoded
   > **中文**：IP地址可以改变（例如拨号、移动主机）——与硬编码的MAC地址不同

2. **Single naming structure / 单一命名结构**
   - Addresses must be unambiguous → can differentiate interfaces/routes
   - Upper-layer protocols must be able to identify end-points unambiguously
   > **中文**：地址必须无歧义 → 能够区分接口/路由；上层协议必须能够无歧义地识别端点

3. **Easy to process/compute / 易于处理/计算**
   - Binary computation: AND, OR, XOR...
   - Name should be case-insensitive ASCII → easy to process
   - Should be easy to convert and compute as binary in route computing
   > **中文**：应便于进行二进制计算（AND、OR、XOR等）；名称应为大小写不敏感的ASCII，便于处理；在路由计算中应易于转换和计算为二进制

---

### 4.1.3 Routing, Forwarding, and Routing Algorithms / 路由、转发与路由算法

Three related but distinct concepts:

> **中文**：三个相关但不同的概念：

| Concept / 概念 | Definition / 定义 |
|--------|------|
| **Routing / 路由** | Finding the way for packets to go from source to destination / 为分组寻找从源到目的地的路径 |
| **Forwarding / 转发** | The action on packets taken by the router according to the routing/forwarding table / 路由器根据路由/转发表对分组采取的动作 |
| **Routing Algorithm / 路由算法** | The algorithm to construct and update the routing table / 构造和更新路由表的算法 |

> **中文**：路由是寻找路径的过程；转发是根据路由表对分组实际执行的动作；路由算法是构造和更新路由表的算法。

---

## 4.2 Routing Algorithms / 路由算法 ★★★

### 4.2.0 Graph Abstraction of the Routing Problem / 路由问题的图抽象

The routing problem is modeled as a graph:

> **中文**：路由问题被建模为一个图：

- **Graph G(N, E)** where N = set of routers (nodes), E = set of links (edges)
  > **中文**：图G(N, E)，其中N是路由器（节点）集合，E是链路（边）集合
- **Link cost l(u,v)**: cost of sending a packet over link (u,v). Defined for u,v ∈ N where (u,v) ∈ E
  > **中文**：链路代价l(u,v)：在链路(u,v)上发送分组的代价
- **Path cost**: The length/cost of a path (x¹, x², x³, ..., xᵖ) = l(x¹,x²) + l(x²,x³) + ... + l(xᵖ⁻¹,xᵖ)
  > **中文**：路径代价：路径上所有链路代价之和
- The routing algorithm finds the **shortest path** (minimum cost path) between nodes
  > **中文**：路由算法寻找节点之间的**最短路径**（最小代价路径）

**Example** / **示例**：G(N,E) where N = {u,v,w,x,y,z}, E = {(u,v), (u,x), (v,x), (v,w), (x,w), (x,y), (w,y), (w,z), (y,z)}, with costs assigned to each link.

> **中文**：路由问题图抽象示例：节点集合N，边集合E，每条边有代价。

**Classification Dimension 1: Global vs Decentralized / 分类维度1：全局 vs 分散**

| Type / 类型 | Knowledge / 知识 | Algorithms / 算法 |
|------|---------|-----------|
| **Global / 全局** | All routers know complete topology G(N,E) and all link costs / 所有路由器知道完整拓扑和所有链路代价 | **Link-state algorithms** (e.g., Dijkstra) / **链路状态算法** |
| **Decentralized / 分散** | Each router only knows physically connected neighbors and link costs to them / 每个路由器仅知道物理连接的邻居及到它们的链路代价 | **Distance-vector algorithms** (e.g., Bellman-Ford) / **距离向量算法** |

> **中文**：全局路由算法中，所有路由器掌握完整拓扑信息（链路状态算法，如Dijkstra）。分散路由算法中，每个路由器仅知道本地邻居信息（距离向量算法，如Bellman-Ford）。

**Classification Dimension 2: Static vs Dynamic / 分类维度2：静态 vs 动态**

| Type / 类型 | Behavior / 行为 |
|------|---------|
| **Static / 静态** | Routes change slowly over time, e.g., when network topology changes (new router joins, link goes up/down) / 路由随时间缓慢变化（如网络拓扑变化时） |
| **Dynamic / 动态** | Routes change quickly, e.g., periodic update, in response to link cost changes / 路由快速变化（如定期更新、响应链路代价变化） |

The routing algorithm is the part of the network layer that decides which output line an incoming packet should be transmitted on.

> **中文**：路由算法是网络层的一部分，它决定到达的分组应从哪条输出线路转发出去。

### 4.2.1 Types of Routing Algorithms / 路由算法的类型

| Type / 类型 | Description / 描述 | Pros / 优点 | Cons / 缺点 |
|------|-------------|------|------|
| **Non-adaptive (Static)** / **非自适应（静态）** | Routes computed offline, loaded at boot / 离线计算路由，启动时加载 | Simple, low overhead / 简单，开销低 | Can't adapt to failures or congestion / 无法适应故障或拥塞 |
| **Adaptive (Dynamic)** / **自适应（动态）** | Routes change in response to topology/load changes / 路由随拓扑/负载变化而变化 | Handles failures, adapts to traffic / 处理故障，适应流量 | Complex, overhead, potential instability / 复杂，开销大，可能不稳定 |

### 4.2.2 Desirable Properties of a Routing Algorithm / 路由算法的期望特性

- **Correctness / 正确性**：Must find valid routes / 必须能找到有效路由
- **Simplicity / 简单性**：Should not burden the network / 不应给网络带来过重负担
- **Robustness / 健壮性**：Must handle failures gracefully / 必须能优雅地处理故障
- **Stability (Convergence) / 稳定性（收敛）**：Must converge quickly to stable routes / 必须快速收敛到稳定路由
- **Fairness / 公平性**：Treat all users equitably / 公平对待所有用户
- **Optimality / 最优性**：Minimize cost (latency, hops, etc.) / 最小化代价（延迟、跳数等）

### 4.2.3 The Optimality Principle and Sink Tree / 最优性原则与汇集树

**Optimality Principle / 最优性原则**：If router J is on the optimal path from router I to router K, then the optimal path from J to K also falls along the same route.

> **中文**：如果路由器J位于从路由器I到路由器K的最优路径上，那么从J到K的最优路径也沿着同样的路线。

**Sink Tree (汇集树)**: The set of optimal routes from **all** sources to a **given** destination forms a **tree** rooted at the destination. No loops. This is a directed tree toward the destination.

> **中文**：从**所有**源节点到**某个给定**目的节点的最优路径集合构成一棵以该目的节点为根的**树**。无环路。这是一棵指向目的节点的有向树。

**Exam use / 考试用途**：Sink trees are used in **broadcast routing**【广播路由】 (see Section 4.3) and **reverse path forwarding**【反向路径转发】.

> **中文**：汇集树用于**广播路由**（见第4.3节）和**反向路径转发**。

### 4.2.4 Shortest Path Algorithm (Dijkstra) / 最短路径算法（迪杰斯特拉）

Finds the shortest path from a source node to ALL other nodes in the network. Used by **link-state routing protocols (OSPF, IS-IS)**【链路状态路由协议（OSPF、IS-IS）】.

> **中文**：找出从源节点到网络中所有其他节点的最短路径。由**链路状态路由协议（OSPF、IS-IS）**使用。

**Shortest Path Properties / 最短路径性质**：
- **Non-negative link costs / 非负链路代价**：Otherwise "shortest" cannot be well-defined / 否则"最短"无法正确定义
- **Additive costs / 可加性代价**：Cost of a path = sum of costs of each link on the path / 路径代价 = 路径上每条链路的代价之和
- **Directional costs / 方向性代价**：A link can have different costs/lengths on different directions / 一条链路在不同方向上可以有不同的代价/长度

> **中文**：最短路径要求链路代价非负、可加（路径总代价=各链路代价之和），且链路在不同方向上可以有不同代价。

**Algorithm (Must know for exam calculation) / 算法（考试计算必须掌握）**：

```
1. Mark source node as "permanent" with distance 0
   将源节点标记为"永久"，距离为0
2. For each node newly marked "permanent":
   对于每个新标记为"永久"的节点：
   a. Update distances to all its neighbors:
      更新到其所有邻居的距离：
      new_dist = dist_to_permanent + link_cost
      新距离 = 到永久节点的距离 + 链路代价
   b. If new_dist < neighbor's current distance, update it
      如果新距离小于邻居的当前距离，则更新
3. Among all "tentative" nodes, select the one with smallest distance
   在所有"临时"节点中，选择距离最小的节点
4. Mark it "permanent" and repeat from step 2
   将其标记为"永久"，从第2步重复
```

**Example / 示例**：Given a network topology with nodes and link costs, compute the shortest path tree from a given source.

> **中文**：给定一个包含节点和链路代价的网络拓扑，计算从给定源节点出发的最短路径树。

**Dijkstra Formal Notation / 迪杰斯特拉正式符号**：

For source node **s**, let:
- **N** = set of nodes whose shortest path is known / 已知最短路径的节点集合
- **l(i,j)** = link cost from i to j (∞ if no direct link) / 从i到j的链路代价（无直接链路则为∞）
- **D(v)** = current cost value of the path from source to v / 从源到v的当前路径代价
- **p(v)** = predecessor node (a neighbor of v) along the path from source to v / 从源到v路径上的前驱节点

**Algorithm / 算法**：
1. **Initialization / 初始化**：N = {s}, D(v) = l(s,v) for any node v ∉ N
2. **Repeat step / 重复步骤**：For node w ∉ N whose D(w) is minimum, add w to N. Then update for all remaining nodes v ∉ N: D(v) ← min{D(v), D(w) + l(w,v)}

After k iterations, outputs the shortest path from the source to k closest destinations.

> **中文**：经过k次迭代后，输出从源到k个最近目的节点的最短路径。

**Algorithm Complexity / 算法复杂度**：
- **O(n²)** basic implementation: each iteration checks all nodes not in N / 每次迭代检查所有不在N中的节点
- **O(n log n)** with improved data structures (heap) / 使用改进的数据结构（堆）
- Total comparisons: n(n+1)/2 → O(n²)

> **中文**：基本实现复杂度为O(n²)，使用堆优化可达O(n log n)。

**Possible Oscillations / 可能的震荡问题**：
- When link cost = amount of traffic on the link, routes may oscillate between paths
  > **中文**：当链路代价等于链路上的流量大小时，路由可能在路径之间震荡
- Example: traffic shifts to the shortest path, making it congested, then the algorithm recomputes and shifts traffic to another path, causing oscillation
  > **中文**：流量转移到最短路径导致该路径拥塞，算法重新计算后将流量转移到另一条路径，导致震荡

> **中文**：Dijkstra算法在链路代价取决于流量时可能产生震荡现象。

### 4.2.5 Flooding / 泛洪法

Every incoming packet is sent out on **every outgoing line except the one it arrived on**.

> **中文**：每个到达的分组被发送到**除到达线路之外的所有输出线路**。

**Problem / 问题**：Generates vast numbers of duplicate packets unless controlled.

> **中文**：除非加以控制，否则会产生大量重复分组。

**Control mechanisms / 控制机制**：
1. **Hop counter (TTL) / 跳数计数器（TTL）**：Decrement at each hop; discard when 0 / 每跳减1；为0时丢弃
2. **Sequence number / 序列号**：Each source numbers its packets; routers track (source, seq) pairs to discard duplicates / 每个源为其分组编号；路由器跟踪（源，序列号）对以丢弃重复
3. **Selective flooding / 选择性泛洪**：Forward only in roughly the right direction / 仅向大致正确的方向转发

**Uses of flooding / 泛洪的用途**：
- Military applications (robustness) / 军事应用（健壮性）
- Distributed database updates / 分布式数据库更新
- **Link state packet distribution in OSPF** / **OSPF中链路状态分组的分发**
- As a baseline for comparing other routing algorithms / 作为比较其他路由算法的基准

**Problem: Broadcast Storm / 问题：广播风暴**：
Flooding can cause a **broadcast storm**【广播风暴】 under certain conditions:

> **中文**：泛洪在某些条件下可能引起**广播风暴**。

Possible causes / 可能的原因：
1. **Loops in the network / 网络中的环路**：Duplicate packets loop endlessly, consuming bandwidth / 重复分组无休止循环，消耗带宽
2. **The broadcast domain is too large / 广播域太大**：Too many devices receiving and retransmitting broadcast frames / 太多设备接收并重传广播帧
3. **A large number of IP address requests via DHCP / 大量通过DHCP的IP地址请求**：When many hosts broadcast DHCP requests simultaneously / 当许多主机同时广播DHCP请求时

> **中文**：广播风暴的三个原因：网络环路导致分组无限循环；广播域过大导致过多设备重传；DHCP请求过多导致广播泛滥。

### 4.2.6 Distance Vector Routing / 距离向量路由 ★★ (RIP uses this / RIP使用此算法)

Also called **Bellman-Ford algorithm**【贝尔曼-福特算法】. Each router maintains a table: (destination, distance, next_hop).

> **中文**：也称为**贝尔曼-福特算法**。每个路由器维护一张表：（目标，距离，下一跳）。

**Key characteristics / 关键特性**：
- **Decentralized computing / 分散计算**：Each router only knows local information about its neighbors / 每个路由器只知道关于其邻居的本地信息
- **Dynamic routing algorithm / 动态路由算法**：Routes are updated periodically / 路由定期更新
- Function: Finds the shortest path from **all** nodes to a given **destination d**
  > **中文**：功能：找出从**所有**节点到给定**目的地d**的最短路径
- The algorithm ends with every node v having: **D(v)** = distance to d, and **n(v)** = next node along the shortest path to d
  > **中文**：算法结束时，每个节点v知道：D(v)=到d的距离，n(v)=沿最短路径到d的下一跳

**Bellman-Ford Algorithm Detailed Steps / 贝尔曼-福特算法详细步骤**：

1. **Initialization / 初始化**：Let d = destination node. D(d) = 0. For any other node v ≠ d: D(v) = ∞, label it n(v) = (─, ∞)
   > **中文**：设d为目的节点。D(d)=0。对于任何其他节点v≠d：D(v)=∞，标记为n(v)=(─,∞)

2. **Repeat step / 重复步骤**：For each node v ≠ d, do the following. For each neighbor w of v:
   > **中文**：对于每个非目的节点v，对其每个邻居w：
   - Update D(v) ← min_{w, (v,w)∈E} { D(w) + l(v,w) }
     > **中文**：更新D(v)为所有邻居中的最小距离
   - Update n(v) with the neighbor that achieves the minimum value: n(v) = arg min_w { D(w) + l(v,w) }
     > **中文**：用达到最小值的邻居更新n(v)

3. **Convergence / 收敛**：Repeat Step 2 until there is **no further change**
   > **中文**：重复步骤2直到**不再变化**

**Example update procedure / 示例更新过程**：Given an update order (e.g., B → C → D → E → F), cycle through all nodes computing D(v) = min{D(w) + l(v,w)} for each neighbor w of v.

> **中文**：按指定顺序（如B→C→D→E→F）循环更新所有节点，每个节点计算到其所有邻居的最小距离。

**Operation / 工作原理**：
1. Each router periodically sends its **entire routing table** to all **directly connected neighbors**
   > **中文**：每个路由器周期性地将其**完整路由表**发送给所有**直接相连的邻居**
2. On receiving neighbor's table:
   收到邻居的路由表时：
   - For each destination in neighbor's table:
     对于邻居表中的每个目标：
     - New distance = my distance to neighbor + neighbor's distance to destination
       新距离 = 我到邻居的距离 + 邻居到目标的距离
     - If this is better (or same next_hop with changed distance) → update entry
       如果该距离更优（或者下一跳相同但距离已变）→ 更新表项

**Key formula / 关键公式**：$D_x(Y) = \min_{v \in \text{neighbors}(x)} [c(x,v) + D_v(Y)]$
(中文：从节点x到目标Y的最短距离等于对x的所有邻居v取最小值，即x到v的链路代价c(x,v)加上v到Y的距离D_v(Y))

**The Count-to-Infinity Problem / 计数到无穷问题**：When a link goes down, routers may slowly count up toward infinity before converging to the correct unavailable status. This is because they exchange stale routes that point back to each other.

> **中文**：当一条链路断开时，路由器可能会慢慢递增计数到无穷大，然后才收敛到正确的不可达状态。这是因为它们交换了指向彼此的过时路由信息。

**Famous saying / 著名说法**：
- **Good news travels fast** / **好消息传播得快**：When a new, better route becomes available, DV converges quickly
  > **中文**：当新的更优路由出现时，距离向量收敛快
- **Bad news travels slow** / **坏消息传播得慢**：When a link fails, the convergence is slow due to count-to-infinity
  > **中文**：当链路故障时，由于计数到无穷问题，收敛缓慢

> **中文**：距离向量路由算法的特点是"好消息传播得快，坏消息传播得慢"。

**Solutions / 解决方案**：
- **Split horizon / 水平分割**：Don't advertise a route back to the neighbor you learned it from / 不将路由通告回从其学习到该路由的邻居
- **Poison reverse / 毒性逆转**：Advertise the route back but with infinite distance / 将路由通告回去，但距离设为无穷大
- **Small infinity / 小无穷**：RIP defines infinity as **16 hops** / RIP将无穷大定义为**16跳**

**Example calculation / 计算示例**：Given a network of 5 nodes with link costs, and initial routing tables, show how tables update after each exchange.

> **中文**：给定一个5节点网络及其链路代价和初始路由表，展示每次交换后路由表如何更新。

### 4.2.7 Link State Routing / 链路状态路由 ★★ (OSPF uses this / OSPF使用此算法)

Each router has complete knowledge of the network topology.

> **中文**：每个路由器拥有完整的网络拓扑信息。

**Five steps / 五个步骤**：
1. **Discover neighbors / 发现邻居**：Send **HELLO packets**【HELLO分组】 on each point-to-point link. Nodes that receive the HELLO reply with their address.
   > **中文**：在每条点对点链路上发送**HELLO分组**。收到HELLO的节点回复其地址。

2. **Measure link cost / 测量链路代价**：Send an **ECHO packet**【ECHO分组】 over each link; measure the round-trip time. Two measurement philosophies:
   > **中文**：在每条链路上发送**ECHO分组**；测量往返时间。两种测量思路：
   - Opinion 1: Only measure transmission delay (avoids oscillation between paths)
     > **中文**：观点1：仅测量传输延迟（避免路径间震荡）
   - Opinion 2: Measure all delays including queuing (more accurate for overall delay)
     > **中文**：观点2：测量包括排队在内的所有延迟（更准确反映总延迟）

3. **Build Link State Packet (LSP) / 构建链路状态分组（LSP）**
   - **LSP structure / LSP结构**：Contains (sender ID, **Sequence Number 32 bits**【序列号32比特】, **Age**【老化时间】, list of neighbors with costs)
     > **中文**：包含（发送者ID，32位序列号，老化时间，邻居及代价列表）
   - **Sequence Number (Seq#) / 序列号**：32 bits, created for each LSP / 32位，为每个LSP创建
   - **Age / 老化时间**：Acts as a timer; decremented by **-1 per second**【每秒减1】. When it reaches 0, the LSP is discarded.
     > **中文**：充当定时器，**每秒减1**。减到0时LSP被丢弃。

4. **Distribute LSPs / 分发LSP**：Flood LSPs to ALL routers in the network using **flooding with refinements (Seq# + Age)**【使用序列号+老化时间的改进泛洪】
   > **中文**：通过**带序列号和老化时间的改进泛洪**将LSP分发给网络中所有路由器
   - **Seq#**: prevents duplicated packets from being propagated (lower Seq# = older packet → rejected as obsolete)
     > **中文**：序列号防止重复分组传播（序列号低 = 更旧的分组 → 被视为过时而拒绝）
   - **Age**: guarantees only live information are distributed
     > **中文**：老化时间确保只有活跃信息被分发

5. **Compute shortest paths / 计算最短路径**：Each router locally runs Dijkstra's algorithm on the complete topology to compute the shortest path tree
   > **中文**：每个路由器在完整拓扑上本地运行Dijkstra算法，计算最短路径树

**Sequence number and age fields / 序列号和老化时间字段**：Prevent old LSPs from being accepted repeatedly. Age field causes old info to eventually be purged.

> **中文**：防止旧的LSP被重复接受。老化时间字段使旧信息最终被清除。

**LSR vs DVR Comparison / 链路状态路由与距离向量路由对比**：

| Aspect / 方面 | Link State (OSPF) / 链路状态（OSPF） | Distance Vector (RIP) / 距离向量（RIP） |
|--------|:---:|:---:|
| Knowledge / 拓扑知识 | Complete topology / 完整拓扑 | Only neighbor info / 仅邻居信息 |
| Algorithm / 算法 | Dijkstra (shortest path) / 迪杰斯特拉（最短路径） | Bellman-Ford (distributed) / 贝尔曼-福特（分布式） |
| Update scope / 更新范围 | Flood to all routers / 泛洪到所有路由器 | Send only to neighbors / 仅发送给邻居 |
| Update content / 更新内容 | Link states (local info) / 链路状态（本地信息） | Entire routing table / 完整路由表 |
| Convergence / 收敛速度 | Fast / 快 | Slow (count-to-infinity) / 慢（计数到无穷） |
| Bandwidth usage / 带宽使用 | More during updates (flood) / 更新时较多（泛洪） | Less per update, but frequent / 每次更新较少，但频繁 |
| Scalability / 可扩展性 | Better for large networks / 更适合大型网络 | Limited (max 15 hops) / 有限（最大15跳） |
| CPU/memory / CPU/内存 | Higher (full topology) / 较高（完整拓扑） | Lower / 较低 |

### 4.2.8 Hierarchical Routing / 层次路由

For very large networks, flat routing doesn't scale. Solution: **hierarchical routing**【层次路由】.

> **中文**：对于非常大的网络，平面路由无法扩展。解决方案：**层次路由**。

- Routers are grouped into **regions**【区域】
  > **中文**：路由器被分组到**区域**中
- Each router knows routes to all routers **in its own region**
  > **中文**：每个路由器知道到达其**所在区域内**所有路由器的路由
- For other regions, it only knows **one route per region**
  > **中文**：对于其他区域，它只知道**每个区域一条路由**
- This reduces routing table size at the cost of possibly suboptimal paths
  > **中文**：这减少了路由表大小，但代价是路径可能不是最优的

**Table size calculation exam point / 表大小计算考点**：For a network with $N$ routers organized into $K$ regions with $N/K$ routers each:
（中文：对于一个有N个路由器、划分为K个区域、每个区域有N/K个路由器的网络：）

- Flat routing / 平面路由：each router needs $N$ entries / 每个路由器需要N个表项
- Hierarchical / 层次路由：each router needs $(N/K) + (K-1)$ entries / 每个路由器需要(N/K)+(K-1)个表项

> **中文解释**：层次路由通过将路由器分组为区域来减少路由表大小。平面路由需要N个表项，而层次路由只需要(N/K)+(K-1)个表项，因为每个路由器知道区域内所有路由器的路由（N/K个），以及到每个其他区域的一条路由（K-1条）。

**Why hierarchical routing is needed / 为什么需要层次路由**：
1. **Scalability / 可扩展性**：Routing table grows as network grows — overhead for computing, storing, and communicating routing information
   > **中文**：路由表随网络规模增长而增长——计算、存储和通信路由信息的开销
2. **Administrative autonomy / 管理自治**：Different organizations want control over their own routing
   > **中文**：不同组织希望控制自己的路由

**Multiple entries collapsed / 多个条目合并**：Multiple entries to the same AS are collapsed into **one entry** in the routing table.

> **中文**：到同一AS的多个条目在路由表中合并为**一个条目**。

**Why different protocols for intra-AS and inter-AS? / 为什么AS内部和AS之间使用不同协议？**

| Reason / 原因 | Intra-AS / AS内部 | Inter-AS / AS之间 |
|---------|:---:|:---:|
| **Policy / 策略** | Single admin, no policy decisions needed / 单一管理域，无需策略决策 | Each admin wants control over traffic routing / 每个管理域希望控制流量路由 |
| **Scale / 规模** | Hierarchical routing saves space, reduces update traffic / 层次路由节省空间、减少更新流量 | Global scale — must handle enormous routing tables / 全球规模——必须处理巨大的路由表 |
| **Performance / 性能** | Can focus on performance / 可专注于性能 | Policy may dominate over performance (e.g., security) / 策略可能优先于性能（如安全性） |

> **中文**：AS内部和AS之间使用不同协议的原因：策略（单一管理域 vs 多管理域）、规模（局部 vs 全球）、性能（性能优先 vs 策略优先）。

---

## 4.3 Broadcast Routing / 广播路由 ★★★ (Big Question Territory / 大题重点区域)

Sending a packet to **all** destinations in a network. Several approaches:

> **中文**：将分组发送给网络中**所有**目的地。有几种方法：

### Method 1: Source Sends to Each Destination Individually / 方法1：源节点分别发送给每个目的地

- Source creates $N-1$ copies, each addressed to a different destination
  > **中文**：源节点创建N-1个副本，每个副本发送给不同的目的地
- **Packets in network / 网络中的分组数**：$N-1$ (or more, depending on routing / 或更多，取决于路由)
- **Disadvantage / 缺点**：Source must know all destinations; wasteful / 源节点必须知道所有目的地；浪费资源

### Method 2: Multi-Destination Routing / 方法2：多目的地路由

- Each packet contains a **list of destinations**【目的地列表】
  > **中文**：每个分组包含一个**目的地列表**
- Router splits the list and makes copies accordingly
  > **中文**：路由器拆分列表并相应地制作副本
- **More efficient** than Method 1 / 比方法1**更高效**

### Method 3: Flooding / 方法3：泛洪法 ★

- Each router forwards packets to all lines except the incoming one
  > **中文**：每个路由器将分组转发到除到达线路之外的所有线路
- **Packets generated / 产生的分组数**：Count by considering each router's inbound/outbound behavior / 通过考虑每个路由器的入站/出站行为来计算：
  - If router receives from 1 direction → creates $(degree - 1)$ new packets
    如果路由器从1个方向接收 → 产生（度数-1）个新分组
  - If router receives from multiple directions → creates $degree$ new packets
    如果路由器从多个方向接收 → 产生（度数）个新分组

### Method 4: Reverse Path Forwarding (RPF) / 方法4：逆向路径转发（RPF）★★★

**Key idea / 关键思想**：Only forward a broadcast packet if it arrived on the link that is on the **shortest path back to the source**.

> **中文**：仅当广播分组从**返回源节点的最短路径**所在的链路上到达时才转发。

**Algorithm / 算法**：
1. Router receives broadcast from source S on port P
   路由器从端口P收到来自源S的广播
2. If P is the port used to send unicast packets **to** S (i.e., P is on the shortest path to S) → **forward** the broadcast to all other ports
   如果P是用于向S发送单播分组的端口（即P在到S的最短路径上）→ 将广播**转发**到所有其他端口
3. Otherwise → **discard** the broadcast silently
   否则 → 静默**丢弃**该广播

**Advantage / 优点**：Simple, efficient, doesn't require knowledge of a spanning tree. Only uses the existing unicast routing table.

> **中文**：简单、高效，不需要知道生成树的信息。仅使用现有的单播路由表。

**Packets generated / 产生分组数**：Significantly fewer than flooding, but more than spanning tree.

> **中文**：分组数量远少于泛洪，但多于生成树方法。

### Method 5: Spanning Tree Broadcast / 方法5：生成树广播 ★

- Construct a **spanning tree**【生成树】 of the network (using STP or manual configuration)
  > **中文**：构建网络的**生成树**（使用STP或手动配置）
- Source sends a packet that is forwarded only along the tree edges
  > **中文**：源节点发送的分组仅沿树的边转发
- **Minimum number of packets / 最小分组数**：Exactly $N-1$ for a network of $N$ nodes (if each node is a router... actually, it's the number of tree edges)
  > **中文**：对于N个节点的网络恰好为N-1（如果每个节点都是路由器……实际上，是树边的数量）
- **Availability note / 可用性说明**：Spanning tree is available with **link-state routing** (complete topology known) but **NOT available with distance vector routing** (no topology info). / 生成树在**链路状态路由**中可用（已知完整拓扑），但在**距离向量路由中不可用**（无拓扑信息）。
  > **中文**：生成树需要完整的拓扑信息，因此链路状态路由支持，但距离向量路由不支持。

**Comparison of broadcast methods (for a specific network) / 广播方法比较（针对特定网络）**：

| Method / 方法 | Packets Generated / 产生分组数 |
|--------|:---:|
| Individual sending / 单独发送 | Depends on routing / 取决于路由 |
| Multi-destination routing / 多目的地路由 | Moderate / 中等 |
| Flooding / 泛洪法 | Very high / 非常高 |
| Reverse Path Forwarding / 逆向路径转发 | High (filtered) / 高（已过滤） |
| Spanning tree / 生成树 | **Minimum** ($E_{tree}$) / **最少**（树边数） |

### Broadcast Routing Packet Count Calculation / 广播路由分组计数计算 ★★

**Typical method / 典型方法**：Count how many new packets each router creates:
（中文：计算每个路由器产生多少个新分组：）

- Router receiving on 1 port, $n$ total ports → creates $n-1$ copies
  路由器从1个端口接收，共有n个端口 → 产生n-1个副本
- Router receiving on all ports → creates 0 (or $n$ copies) — depends on context
  路由器从所有端口接收 → 产生0个（或n个副本）— 取决于上下文
- Sum across all routers to get total
  对所有路由器求和得到总数

> **中文**：广播路由分组计数方法：考虑每个路由器的端口数。如果从1个方向收到广播，则产生（度数-1）个新分组；如果从所有方向收到，则产生0个新分组。对所有路由器求和得到总数。

---

## 4.4 Multicast Routing / 组播路由

Sending to a **subset** of destinations (a group). Group members are identified by a **multicast (Class D) address**【组播（D类）地址】.

> **中文**：发送给目的地的**子集**（一个组）。组成员通过**组播（D类）地址**标识。

### 4.4.1 Core-Based Trees (CBT) / 核心基本树（CBT）

**Without core-based tree / 没有核心基本树时**：Each router needs to store $m \times n$ trees (one per source per group).
（中文：每个路由器需要存储m乘以n棵树，即每个组中每个源一棵树）

**With core-based tree / 使用核心基本树时**：Each group has one **core** (rendezvous point)【核心（汇集点）】. All senders send to the core; the core distributes to all members. Each router only needs to store $n$ trees (one per group).
（中文：每个组有一个**核心**（汇集点）。所有发送者发送到核心；核心分发给所有成员。每个路由器只需要存储n棵树，即每个组一棵树）

$$\text{Without CBT: } mn \text{ trees} \quad \text{With CBT: } n \text{ trees}$$
（中文：无CBT时：mn棵树；有CBT时：n棵树）

This is a dramatic reduction in router state.

> **中文**：这极大地减少了路由器状态信息量。

**Group management / 组管理**：Uses **IGMP (Internet Group Management Protocol)**【互联网组管理协议】 for hosts to join/leave multicast groups.

> **中文**：使用**IGMP（互联网组管理协议）**让主机加入/离开组播组。

### 4.4.2 Multicast Routing with Distance Vector / 组播路由与距离向量

When used with distance vector routing (which does not have full topology knowledge), multicast uses a **PRUNE mechanism**【剪枝机制】:

> **中文**：与距离向量路由（没有完整拓扑信息）一起使用时，组播使用**剪枝机制**。

**Process / 工作过程**：
1. Source initially floods multicast packets to all routers (broadcast)
   > **中文**：源节点初始时将组播分组泛洪到所有路由器（广播）
2. When a router receives a packet for a multicast group for which it has **no subscribers** (no hosts or downstream routers interested), it sends a **PRUNE message** back to the source router
   > **中文**：当路由器收到一个组播分组，但该组播组**没有订阅者**（没有感兴趣的主机或下游路由器）时，它向源路由器发送**PRUNE消息**
3. The source stops sending multicast packets down that branch
   > **中文**：源节点停止沿该分支发送组播分组

> **中文**：距离向量组播路由使用剪枝机制：初始泛洪，无订阅者的分支发送PRUNE消息，源节点停止向该分支发送组播数据。

---

## 4.5 Mobile Host Routing / 移动主机路由

**Key concept / 关键概念**：A mobile host has a **permanent home address**【永久家乡地址】. When away, it has a **care-of address**【转交地址】.

> **中文**：移动主机有一个**永久家乡地址**。当离开家乡网络时，它有一个**转交地址**。

**Home Agent (HA) / 家乡代理**：Router at the mobile host's home network. Intercepts packets for the mobile host and forwards them via **tunneling**【隧道技术】.

> **中文**：位于移动主机家乡网络的路由器。截获发往移动主机的分组并通过**隧道技术**转发。

**Triangle routing process / 三角路由过程**：
1. Sender sends packet to mobile host's **home address**
   发送者将分组发送到移动主机的**家乡地址**
2. Home agent intercepts, encapsulates packet, tunnels to **care-of address**
   家乡代理截获、封装分组，通过隧道发送到**转交地址**
3. Mobile host receives, decapsulates, sends reply **directly** back to sender
   移动主机接收、解封装，将回复**直接**发送回发送者
4. Sender learns care-of address, may set up a **direct tunnel** to bypass home agent
   发送者获知转交地址，可建立**直接隧道**绕过家乡代理

**Why "triangle"? / 为什么叫"三角"？** Packets go: Sender → Home Agent → Mobile Host → Sender (the first packets trace a geographic triangle).
> **中文**：分组路径为：发送者 → 家乡代理 → 移动主机 → 发送者（最初几个分组在地理上形成一个三角形）。

---

## 4.5a Routing in Ad Hoc Networks / 自组织网络中的路由

**Ad Hoc Network**【自组织网络】：A wireless network without fixed infrastructure. Nodes move freely, so the network topology changes dynamically.

> **中文**：**自组织网络**是一种没有固定基础设施的无线网络。节点自由移动，因此网络拓扑动态变化。

**Key routing characteristics / 关键路由特性**：
- Topology changes as wireless nodes move / 拓扑随无线节点移动而变化
- Routes are computed/constructed **on demand** (when needed) / 路由**按需**计算/构建
- Traditional routing protocols (RIP/OSPF) do not work well due to frequent topology changes
  > **中文**：由于拓扑频繁变化，传统路由协议（RIP/OSPF）效果不佳

**Example protocol / 示例协议**：
- **AODV (Ad hoc On-Demand Distance Vector)**【按需距离向量】：A reactive routing protocol that discovers routes only when needed, using RREQ (Route Request) and RREP (Route Reply) messages
  > **中文**：**AODV（自组网按需距离向量路由协议）**：一种反应式路由协议，仅在需要时发现路由，使用RREQ（路由请求）和RREP（路由回复）消息

---

## 4.6 Congestion Control and QoS / 拥塞控制与服务质量 ★★★

### 4.6.0 Application Requirements and QoS Basics / 应用需求与QoS基础

**Different applications have different QoS requirements / 不同应用有不同QoS需求**：

| Application / 应用 | Bandwidth / 带宽 | Delay / 延迟 | Jitter / 抖动 | Loss / 丢包 |
|----------------|:---:|:---:|:---:|:---:|
| Email / 邮件 | Low / 低 | Low / 低 | Low / 低 | Medium / 中 |
| File transfer / 文件传输 | High / 高 | Low / 低 | Low / 低 | Medium / 中 |
| Web / 网页 | Medium / 中 | Medium / 中 | Low / 低 | Medium / 中 |
| SSH / 远程登录 | Low / 低 | High / 高 | Medium / 中 | Medium / 中 |
| Streaming video / 流视频 | High / 高 | Medium / 中 | High / 高 | Medium / 中 |
| VoIP / 网络电话 | Low / 低 | **High / 高** | **High / 高** | Low / 低 |

> **中文**：不同应用有不同QoS需求。SSH对延迟敏感，流媒体对抖动敏感，VoIP对延迟和抖动都敏感。

**Jitter and Buffering / 抖动与缓冲**：

- **Jitter**【抖动】：The **variation** (standard deviation) in the packets' arrival time / 分组到达时间的**变化**（标准差）
  > **中文**：抖动是分组到达时间的变异程度（标准差）
- **Buffering for jitter smoothing / 缓冲平滑抖动**：
  - Place buffers on the **receiving side** before delivering data to the application
    > **中文**：在**接收端**交付数据给应用之前放置缓冲区
  - Buffers delay the playback slightly, absorbing the jitter so that the application sees a smooth stream
    > **中文**：缓冲区稍微延迟播放，吸收抖动，使应用看到平滑的流
  - Trade-off: more buffering = better jitter smoothing but longer delay
    > **中文**：权衡：更多缓冲 = 更好的抖动平滑但更长的延迟

> **中文**：抖动是分组到达时间的变化程度。在接收端使用缓冲区可以平滑抖动，但会增加延迟。

### 4.6.1 Congestion vs Flow Control / 拥塞控制与流量控制的区别

| | Congestion Control / 拥塞控制 | Flow Control / 流量控制 |
|--|:---:|:---:|
| **Scope / 范围** | Global (involves all hosts, routers, links) / 全局（涉及所有主机、路由器、链路） | Point-to-point (between sender and receiver) / 点到点（发送者和接收者之间） |
| **Cause / 原因** | Too much traffic for the network to handle / 网络中的流量过大 | Sender faster than receiver can process / 发送者比接收者处理速度快 |
| **Goal / 目标** | Keep the subnet from being overwhelmed / 防止子网过载 | Prevent sender from overwhelming receiver / 防止发送者压垮接收者 |

> **中文**：拥塞控制是全局性的，涉及所有网络元素；流量控制是点到点的，仅涉及单条连接上的发送方和接收方。

### 4.6.2 Congestion Control Techniques / 拥塞控制技术

**Two Types of Solutions / 两种解决方案**：

| Type / 类型 | Description / 描述 |
|------|-------------|
| **Open-loop / 开环** | NO runtime feedback. Prevent problems before they happen / 无运行时反馈，在问题发生前预防 |
| **Closed-loop / 闭环** | Use feedback (measurements of system performance) to make corrections at runtime — Monitor → Pass information → Take actions / 使用反馈（系统性能测量）在运行时进行纠正 — 监控→传递信息→采取措施 |

> **中文**：开环控制在问题发生前预防，无需运行时反馈。闭环控制使用性能测量反馈进行运行时纠正。

**Metrics to monitor / 监控指标**：% of packets discarded/timeout / 丢包/超时百分比, average queue length / 平均队列长度, average packet delay / 平均分组延迟

**Active Queue Management (AQM) at Routers / 路由器主动队列管理**：

| Policy / 策略 | Description / 描述 |
|---------|-------------|
| **Drop-tail / 尾部丢弃** | When congestion is detected, drop newly incoming packets (simplest, but may cause TCP global synchronization) / 检测到拥塞时丢弃新到达的分组（最简单，但可能导致TCP全局同步） |
| **RED (Random Early Detection)** / **随机早期检测** | When congestion is **going to happen**, randomly select and drop packets in the buffer (before buffer is full) / 拥塞**即将发生**时，随机选择并丢弃缓冲区中的分组（在缓冲区满之前） |
| **ECN (Early Congestion Notification)** / **早期拥塞通知** | Router sets a bit in packet header instead of dropping; receiver echoes back to sender in ACK / 路由器在分组头部设置标记位而非丢弃；接收方在ACK中回显给发送方 |

> **中文**：AQM策略包括尾部丢弃（最简单但可能引起TCP全局同步）、RED（缓冲区满前随机丢包）、ECN（标记而非丢包）。

**Congestion Prevention vs Reduction / 拥塞预防与缓解**：

| Approach / 方法 | Actions / 措施 |
|---------|---------|
| **Prevention (Increase Resources) / 预防（增加资源）** | Use faster lines / 使用更快线路; Obtain additional lines / 获取额外线路; Utilize alternate pathways / 使用替代路径; Utilize "spare" routers / 使用"备用"路由器 |
| **Reduction (Decrease Traffic) / 缓解（减少流量）** | Send messages to senders to slow down / 通知发送者减速; Deny service to some users / 拒绝某些用户的服务; Downgrade service to some/all users / 降低部分/全部用户的服务等级; Schedule usage for better load balance / 调度使用以达到更好的负载均衡 |

> **中文**：拥塞预防通过增加资源来解决，拥塞缓解通过减少流量来解决。

**Congestion Control Techniques Table / 拥塞控制技术表**：

| Technique / 技术 | Layer / 层 | How it works / 工作原理 |
|-----------|:---:|-------------|
| **Warning bit (ECN)** / **警告位（ECN）** | 3 | Router sets a bit in packet header; receiver echoes it back to sender in ACK / 路由器在分组头部设置一个比特；接收方在ACK中将其回显给发送方 |
| **Choke packets** / **抑制分组** | 3 | Router sends explicit "slow down" message back to source / 路由器向源端发送明确的"减速"消息 |
| **Hop-by-hop choke** / **逐跳抑制** | 3 | Choke effect propagates hop by hop (faster response) / 抑制效果逐跳传播（响应更快） |
| **Load shedding** / **负载脱落** | 3 | Router drops packets when overloaded (wine vs. milk strategy) / 路由器过载时丢弃分组（葡萄酒与牛奶策略） |
| **RED (Random Early Detection)** / **随机早期检测** | 3 | Router drops packets randomly before buffer is full, signaling congestion early / 路由器在缓冲区满之前随机丢弃分组，提前发出拥塞信号 |
| **Traffic shaping** / **流量整形** | 3 | Regulate the rate and volume of traffic entering the network / 调节进入网络的流量速率和总量 |

> **中文**：拥塞控制技术包括：ECN警告位（在分组头部标记拥塞）、抑制分组（向源端发送减速消息）、逐跳抑制（逐级传播）、负载脱落（选择性丢包）、RED（随机早期检测，在缓冲区满之前提前丢包）和流量整形（调节流量注入速率）。

### 4.6.3 Traffic Shaping: Leaky Bucket / 流量整形：漏桶 ★★★

**Analogy / 类比**：A bucket with a hole in the bottom. Water (packets) pours in at variable rates but leaks out at a **constant rate**【恒定速率】.

> **中文**：一个底部有洞的水桶。水（分组）以可变速率注入，但以**恒定速率**漏出。

**Key properties / 关键特性**：
- **Constant output rate ($M$) / 恒定输出速率（M）**：Smooth bursty traffic / 平滑突发流量
- **Bucket size ($\beta$) / 桶容量（β）**：Maximum burst that can be absorbed / 可吸收的最大突发量
- Packets arrive at variable rates, leave at constant rate / 分组以可变速率到达，以恒定速率离开
- If bucket overflows → packet is dropped or marked non-conformant / 如果桶溢出 → 分组被丢弃或被标记为不符合规则

**Mathematical model / 数学模型**：
- Let X(s,t) be the amount of traffic generated during time interval [s, t]
- Leaky bucket constraint: **X(s, s+t) ≤ M × t** for all s (the output rate never exceeds M)
  > **中文**：漏桶约束：在任何时间区间内，输出的流量不超过M乘以时间长度
- This ensures a **smooth, constant output rate** / 这确保了**平滑、恒定的输出速率**

> **中文**：漏桶的数学模型：对于任何时间段[s, s+t]，输出的流量X(s,s+t) ≤ M×t，即输出速率恒定不超过M。

**Use case / 应用场景**：Smoothing bursty traffic for consistent downstream processing.

> **中文**：平滑突发流量，使下游处理保持一致。

**Calculation pattern 1 — Finding required leak rate / 计算模式1 — 找到所需漏出速率**：
> Computer generates 25 MB/s with 40 ms bursts per second. Router max rate = 2 MB/s. Find leak rate for smooth output.
> **中文**：计算机以25 MB/s速率生成数据，每秒突发持续40 ms。路由器最大速率为2 MB/s。求平滑输出所需的漏出速率。

Burst data per second / 每秒突发数据量：$25 \text{ MB/s} \times 0.04 \text{ s} = 1 \text{ MB}$
To drain 1 MB over 1 second / 在1秒内排空1 MB：$\boxed{1 \text{ MB/s}}$ minimum leak rate / 最小漏出速率。

The leak rate should be between 1 MB/s and 2 MB/s (router max).

> **中文**：漏出速率应在1 MB/s和2 MB/s（路由器最大速率）之间。

### 4.6.4 Traffic Shaping: Token Bucket / 流量整形：令牌桶 ★★★

**Analogy / 类比**：Tokens are added to a bucket at a **constant rate** ($\rho$). Each packet must obtain a token to leave. If no token is available, the packet must wait.

> **中文**：令牌以**恒定速率（ρ）**添加到桶中。每个分组必须获取令牌才能离开。如果没有令牌可用，分组必须等待。

**Key properties / 关键特性**：
- **Token rate ($\rho$) / 令牌速率（ρ）**：Sustained average transmission rate / 持续平均传输速率
- **Bucket capacity ($C$) / 桶容量（C）**：Maximum burst size (how many bytes can be sent at once) / 最大突发大小（一次能发送多少字节）
- **Max output rate ($M$) / 最大输出速率（M）**：Peak drain rate (M > $\rho$) / 峰值漏出速率（M > ρ）
- **Allows bursts / 允许突发**：If the bucket is full, can burst up to $C$ bytes at once / 如果桶满了，可以一次性突发发送最多C字节
- After a burst, rate is limited to the token arrival rate $\rho$ / 突发后，速率限制为令牌到达速率ρ

**Mathematical model / 数学模型**：
- Let X(s,t) be the amount of traffic transmitted during time interval [s, t]
- Token bucket constraint: **X(s, s+t) ≤ min{Mt, ρt + C}** for all s
  > **中文**：令牌桶约束：在任何时间区间内，传输的流量不超过min(Mt, ρt + C)
- This means: the rate is bounded by both the peak drain rate M and the token generation (ρt + initial tokens C)
  > **中文**：这意味着速率受两个因素限制：峰值漏出速率M和令牌生成速率加上初始令牌

> **中文**：令牌桶数学模型：X(s,s+t) ≤ min{Mt, ρt + C}。允许短期突发（利用桶中积攒的令牌），但长期平均速率受限于ρ。

**Key formulas / 关键公式**：
$$\boxed{\text{Max burst duration} = \frac{b}{\text{Peak rate} - r}}$$
（中文：最大突发持续时间 = 桶容量 /（峰值速率 - 令牌速率））

$$\boxed{\text{Max sustained data rate} = r}$$
（中文：最大持续数据速率 = 令牌速率r）

**Example 1 / 示例1**：Token bucket: $r = 1$ Mbps, $b = 8$ MB. Peak rate / 峰值速率 = 6 Mbps. How long can full-speed burst last? / 全速突发能持续多久？

Tokens consumed per second at peak / 峰值时每秒消耗令牌：$t \cdot 1 \text{ Mbps} + 64 \text{ Mb} = t \cdot 6 \text{ Mbps}$
$64 = 5t \Rightarrow t = \boxed{12.8 \text{ seconds / 秒}}$

> **中文**：令牌桶参数：r=1 Mbps, b=8 MB, 峰值速率=6 Mbps。设持续时间为t秒，则消耗的令牌 = t x 1 Mbps + 64 Mb（桶中初始令牌）= t x 6 Mbps（发送速率）。解得t=12.8秒。

**Example 2 / 示例2**：Token arrives every 5 μs. Each token allows 48 bytes. What is the max sustained rate? / 令牌每5微秒到达一个。每个令牌允许发送48字节。最大持续速率是多少？

Tokens/second / 每秒令牌数：$1 / (5 \times 10^{-6}) = 200,000$ tokens/s
Max rate / 最大速率：$200,000 \times 48 \times 8 = \boxed{76.8 \text{ Mbps}}$

> **中文**：每秒令牌数 = 1 / (5 x 10^-6) = 200,000个/秒。每个令牌48字节 = 384比特。总速率 = 200,000 x 384 = 76.8 Mbps。

### 4.6.5 Leaky Bucket + Token Bucket Combined / 漏桶与令牌桶结合

**Arrangement / 排列方式**：Token bucket → Leaky bucket / 令牌桶 → 漏桶

**Purpose / 目的**：
- Token bucket allows bursts / 令牌桶允许突发
- Leaky bucket smooths the bursts / 漏桶平滑突发
- Together: allow bursts, smooth output, reduce peak rate / 结合：允许突发，平滑输出，降低峰值速率

**Calculation (from senior review) / 计算（来自学长复习资料）**：
> 25 MB/s generator, 40 ms burst/s, token rate 2 MB/s, token bucket 500 KB, leaky bucket 10 MB/s. Find working time.
> **中文**：25 MB/s发生器，每秒突发40ms，令牌速率2 MB/s，令牌桶500 KB，漏桶10 MB/s。求工作时间。

**Phase 1 / 阶段1** (token bucket full, data fills leaky bucket / 令牌桶满，数据填充漏桶)：
Rate into leaky bucket = 25 - 10 = 15 MB/s (differential / 差值)
$500 \text{ KB} + 2 \text{ MB/s} \times t = 25 \text{ MB/s} \times t$
$t = 500 \text{ KB} / (23 \text{ MB/s}) = 0.5 / 23 \approx 21.74 \text{ ms / 毫秒}$

> **中文解释**：阶段1中，令牌桶初始满，数据以25 MB/s进入漏桶，漏桶以10 MB/s漏出，差值为15 MB/s。令牌桶以2 MB/s补充令牌，同时数据以25 MB/s消耗。解题公式：500KB + 2t = 25t，t = 0.5/23 ≈ 21.74ms。

**Phase 2 / 阶段2** (token bucket at 2 MB/s, leaky bucket drains / 令牌桶以2 MB/s工作，漏桶排水)：
Drain rate differential / 排水速率差：10 - 2 = 8 MB/s
Data accumulated in P1 / 阶段1积累的数据：$21.74 \text{ ms} \times 15 \text{ MB/s} = 21.74 \text{ ms} \times 15 \text{ MB/s}$
Duration of P2 / 阶段2持续时间：P1_data / 8 MB/s

**Total / 总计**：P1 + P2 duration. Key result: **62.47 ms** total working time / 总工作时间。

> **中文**：阶段2中，令牌桶达到稳态（2 MB/s），漏桶以10 MB/s漏出，桶内积累的数据以8 MB/s的净速率排出。阶段1积累的数据除以8 MB/s即为阶段2持续时间。总工作时间 = 阶段1 + 阶段2 ≈ 62.47 ms。

### 4.6.6 Packet Scheduling Methods / 分组调度方法

**Overview / 概述**：Packets coming from various networks are multiplexed together and queued for transmission at output buffers. Scheduling determines **which packet goes out first**.

> **中文**：来自不同网络的分组被多路复用并在输出缓冲区排队。调度决定**哪个分组先发出**。

#### FIFO (First-In-First-Out) / 先入先出

**Concept / 概念**：Packets leave the queue in the **same order** as they arrived. Simplest scheduling discipline.

> **中文**：分组按照到达的**相同顺序**离开队列。最简单的调度方式。

**Queue Dynamics — Lindley's Recursion / 队列动态 — Lindley递归**：

Let / 设：
- **Xₙ** = amount of traffic arrival in the nth time slot / 第n个时隙到达的流量
- **Qₙ** = queue length at the nth time slot / 第n个时隙的队列长度
- **C** = service rate (max amount that can be served in one time slot) / 服务速率（一个时隙能服务的最大流量）
- **B** = buffer size (maximum queue length) / 缓冲区大小（最大队列长度）

**Lindley's Recursion / Lindley递归**：
$$Q_{n+1} = \min\{\max\{Q_n + X_n - C, 0\}, B\}$$

> **中文**：下一个时隙的队列长度 = 当前队列长度 + 到达流量 - 服务速率（不能小于0，不能超过缓冲区大小B）

For infinite buffer (B = ∞): $Q_{n+1} = Q_n + X_n - C$ (with the constraint Q ≥ 0)

> **中文**：在无限缓冲区的情况下：Q_{n+1} = Q_n + X_n - C（但Q不能小于0）

#### Priority Queuing (PQ) / 优先级队列

**Concept / 概念**：Packets are classified into **two or more priority classes**【优先级类别】 at the output queue.

> **中文**：分组在输出队列被分为**两个或多个优先级类别**。

**Classification methods / 分类方法**：
- Can be explicitly written in the **ToS (Type of Service) field** of IP header / 可以在IP头部的**ToS字段**中明确标记
- Can be determined by values of other fields / 可以通过其他字段的值来确定

> **中文**：优先级可以通过IP头部ToS字段或其他字段确定。

**PQ Discipline / PQ规则**：
1. Packets in the same priority class: **FIFO** / 同一优先级内的分组：**FIFO**
2. Transmit a packet from the **highest priority class** that has a non-empty queue / 始终从**最高优先级**的非空队列发送分组

> **中文**：同一优先级内按FIFO顺序发送；始终从最高优先级的非空队列发送。

**Issue / 问题**：Lower priority packets may be **starved**【饿死】 (never transmitted) if higher priority traffic is continuous.

> **中文**：如果高优先级流量持续不断，低优先级分组可能被**饿死**（永远得不到发送机会）。

#### Round Robin (RR) Scheduler / 轮询调度器

**Concept / 概念**：Packets are classified into classes (as in PQ). The scheduler **alternates services among classes** in a circular manner.

> **中文**：分组被分为多个类别，调度器以循环方式**在类别之间交替服务**。

**Two-class example / 两类示例**：Transmit a class 1 packet, then a class 2 packet, then another class 1 packet, then another class 2 packet, and so on.

> **中文**：两类轮询示例：发送一个类别1分组，然后一个类别2分组，再一个类别1分组，以此类推。

**Work-conserving / 工作保持**：The queuing discipline will **never allow the link to remain idle** whenever there are packets queued for transmission.

> **中文**：**工作保持**特性：只要队列中还有分组等待发送，调度器就不会让链路空闲。

> **中文**：轮询调度以循环方式在各类别间交替服务，具有工作保持特性（不会让链路空闲）。

### 4.6.7 M/M/1 Queuing Model / M/M/1 排队模型 ★★★

> **Note / 注意**：Section 4.6.6 covers FIFO/PQ/Round Robin scheduling. M/M/1 is a separate analytical model.

Models a single server with Poisson arrivals and exponential service times.

> **中文**：对具有泊松到达和指数服务时间的单服务器进行建模。

**Notation / 符号说明**：M = Markov (memoryless/Poisson)【马尔可夫（无记忆/泊松）】, 1 = single server / 单服务器。

**Parameters / 参数**：
- $\lambda$ = arrival rate (packets/second) / 到达速率（分组/秒）
- $\mu$ = service rate (packets/second) / 服务速率（分组/秒）
- $\rho = \lambda / \mu$ = utilization (must be < 1 for stability) / 利用率（必须小于1以保持稳定）

**Key formulas / 关键公式**：

$$\boxed{T = \frac{1}{\mu - \lambda}} \quad \text{(Average time in system, including service)}$$
（中文：T = 1 / (μ - λ)，系统平均时间，包括服务时间）

$$\boxed{N = \frac{\rho}{1 - \rho} = \frac{\lambda}{\mu - \lambda}} \quad \text{(Average number in system)}$$
（中文：N = ρ / (1 - ρ) = λ / (μ - λ)，系统中平均分组数）

**Example / 示例**：Arrival rate / 到达速率 = 10 packets/s, service rate / 服务速率 = 20 packets/s.
- $T = 1 / (20 - 10) = \boxed{0.1 \text{ s / 秒}}$
- $\rho = 10/20 = 0.5 = 50\%$
- $N = 0.5 / (1 - 0.5) = \boxed{1 \text{ packet / 分组}}$ (average / 平均)

> **中文解释**：当λ=10分组/秒，μ=20分组/秒时，系统利用率ρ=50%，平均时间T=0.1秒，系统中平均分组数N=1个。

**FDM example / 频分复用示例**：Channel capacity C / 信道容量C, arrival rate λ / 到达速率λ, average frame length $1/\mu$ / 平均帧长度1/μ.

$$\text{Single channel / 单信道：} T = \frac{1}{\mu C - \lambda}$$

If channel is split into $N$ FDM subchannels / 如果信道被分为N个FDM子信道：
$$T_{FDM} = \frac{N}{\mu C - \lambda} = N \times T$$

**Conclusion / 结论**：FDM increases average delay by a factor of N.

> **中文**：FDM将平均延迟增加N倍。因为每个子信道的服务速率变为原来的1/N，而到达率不变，导致延迟增大。

### 4.6.8 Packet Scheduling: WFQ (Weighted Fair Queuing) / 分组调度：加权公平排队 ★★★

**WFQ as generalization of Round Robin / WFQ作为轮询的推广**：
- Like Round Robin: serve in a circular manner / 与轮询相同：以循环方式服务
- Difference: Each class is assigned a **weight wᵢ**. Class i is guaranteed to receive a fraction **wᵢ / ∑wⱼ** of service
  > **中文**：与轮询的不同点：每个类别被分配一个**权重wᵢ**，类别i保证获得服务份额的**wᵢ / ∑wⱼ**
- For a link of rate R, class i (with packets to send) receives a throughput of at least **R · wᵢ / ∑wⱼ**
  > **中文**：对于速率为R的链路，类别i（有待发送分组）至少获得R · wᵢ / ∑wⱼ的吞吐量

**Concept / 概念**：Multiple queues (one per flow). Scan queues byte-by-byte (or weighted byte-by-byte) in a round-robin fashion. A packet is considered "done" when its number of rounds is reached.

> **中文**：多个队列（每个流一个队列）。以轮询方式逐字节（或加权逐字节）扫描队列。当一个分组的轮次数到达时，该分组被视为"完成"。

**WFQ formula / WFQ公式**：
$$\boxed{F_i = \max(A_i, F_{i-1}) + \frac{L_i}{W}}$$
（中文：完成编号Fi = max(到达轮数Ai, 同一队列前一个分组的完成编号Fi-1) + 分组长度Li / 队列权重W）

Where / 其中：
- $F_i$ = finish number (time/round at which packet completes) / 完成编号（分组完成的轮次/时间）
- $A_i$ = arrival time (in rounds) / 到达时间（轮次数）
- $F_{i-1}$ = finish number of the previous packet in the same queue / 同一队列前一个分组的完成编号
- $L_i$ = length of packet $i$ (in bytes) / 分组i的长度（字节）
- $W$ = weight of the queue (higher weight = faster service) / 队列权重（权重越高，服务越快）

**Packets are transmitted in order of increasing $F_i$** / **分组按Fi递增的顺序发送**。

**Example / 示例**：Three queues A (W=1), B (W=1), C (W=2) / 三个队列A（权重1）、B（权重1）、C（权重2）。

| Packet / 分组 | Queue / 队列 | Arrival / 到达轮数 | Length / 长度 | Calculation / 计算过程 | Finish / 完成编号 |
|--------|-------|---------|--------|-------------|--------|
| A | A | 0 | 8 | $0 + 8/1 = 8$ | **8** |
| B | B | 5 | 6 | $5 + 6/1 = 11$ | **11** |
| C | C | 5 | 10 | $5 + 10/2 = 10$ | **10** |

> **中文**：前三行（分组A、B、C）：A属于队列A(W=1)，到达轮数0，长度8，Fi=8；B属于队列B(W=1)，到达5，长度6，Fi=11；C属于队列C(W=2即权重更大)，到达5，长度10，Fi=10。C因权重更大，其完成编号(10)小于B(11)，虽同时到达但C先发送。

| Packet / 分组 | Queue / 队列 | Arrival / 到达轮数 | Length / 长度 | Calculation / 计算过程 | Finish / 完成编号 |
|--------|-------|---------|--------|-------------|--------|
| D | A | 8 | 9 | $\max(8,8) + 9/1 = 17$ | **17** |
| E | C | 8 | 8 | $\max(8,10) + 8/2 = 14$ | **14** |
| F | B | 10 | 6 | $\max(10,11) + 6/1 = 17$ | **17** |

> **中文**：中间三行（分组D、E、F）：D队列A到达8长度9Fi=17；E队列C到达8长度8，因C的W=2，Fi=14；F队列B到达10长度6Fi=17。D和F的Fi均为17，需按FIFO处理。

| Packet / 分组 | Queue / 队列 | Arrival / 到达轮数 | Length / 长度 | Calculation / 计算过程 | Finish / 完成编号 |
|--------|-------|---------|--------|-------------|--------|
| G | C | 11 | 10 | $\max(11,14) + 10/2 = 19$ | **19** |
| H | A | 20 | 8 | $\max(20,17) + 8/1 = 28$ | **28** |

> **中文**：最后两行（分组G、H）：G队列C到达11长度10Fi=19；H队列A到达20长度8Fi=28。

**Order / 发送顺序**：A(8) → C(10) → B(11) → E(14) → D(17) → F(17) → G(19) → H(28)

**When finish numbers tie / 当完成编号相同时**：FIFO among tied packets / 按先来先服务处理相同的分组。

> **中文解释**：WFQ调度中，每个队列有独立权重。完成编号Fi = max(到达轮数Ai, 前一个分组完成编号Fi-1) + 分组长度Li/权重W。按完成编号从小到大发送分组。权重大的队列（如C的W=2）服务更快，完成编号更小。C的分组虽然与B同时到达（轮数5），但C的完成编号10小于B的11，因此C先于B发送。

### 4.6.9 Service Models in the Internet / 互联网中的服务模型

Three service models for QoS in the Internet:

> **中文**：互联网中三种QoS服务模型。

#### 1. Best Effort Model / 尽力而为模型

- Every user receives the **same level** of service / 每个用户收到**相同等级**的服务
- No guarantees on delay, bandwidth, or packet loss / 不保证延迟、带宽或丢包
- Congestion results in severe degradation to applications requiring minimum bandwidth/delay
  > **中文**：对于需要最小带宽/延迟的应用，拥塞会导致严重的性能下降

> **中文**：尽力而为模型是所有用户获得相同等级的服务，无任何保证。

#### 2. Integrated Services (IntServ) Model / 综合服务模型

- Requires resources (bandwidth/buffers) to be **explicitly reserved** for each traffic flow
  > **中文**：需要对每个流量流**显式预留**资源（带宽/缓冲区）
- Signaling protocol: **RSVP (Resource ReSerVation Protocol)**【资源预留协议】
  > **中文**：信令协议：**RSVP（资源预留协议）**
- Requires: **packet classification**, **packet scheduling**, and **CAC (Call Admission Control)**【呼叫准入控制】
  > **中文**：需要：**分组分类**、**分组调度**和**呼叫准入控制**
- Not scalable for large networks (per-flow state in every router)
  > **中文**：在大规模网络中不可扩展（每个路由器需要维护每个流的状态）

> **中文**：综合服务模型为每个流显式预留资源，使用RSVP信令协议，需要分组分类、调度和呼叫准入控制。

#### 3. Differentiated Services (DiffServ) Model / 区分服务模型

- Scalable and flexible services based on **"class" of traffic**【流量类别】
  > **中文**：基于**流量类别**的可扩展且灵活的服务
- Packets are marked with different **DSCP (DiffServ Code Point)**【区分服务码点】 in the IP header
  > **中文**：分组在IP头部被标记不同的**DSCP（区分服务码点）**
- Per-hop behavior (PHB) defines how each class is treated at each router
  > **中文**：逐跳行为（PHB）定义每个类别在每个路由器上的处理方式
- No per-flow state — only per-class state → highly scalable
  > **中文**：无需每流状态——仅需每类状态，高度可扩展

> **中文**：区分服务模型基于流量类别提供可扩展的服务，使用DSCP标记和PHB逐跳行为，无需每流状态。

### 4.6.10 Admission Control / 准入控制

**Concept / 概念**：Prevent congestion by not admitting new flows when resources are insufficient.

> **中文**：通过资源不足时不接纳新的流来防止拥塞。

**Two aspects / 两个方面**：

1. **QoS Routing / QoS路由**：Reservations are made **along the route** — the path is chosen based on resource availability
   > **中文**：沿着路径进行资源预留——基于资源可用性选择路径

2. **Flow Negotiation / 流协商**：Application and routers agree on a set of flow parameters, called a **flow specification**【流规范】
   > **中文**：应用和路由器就一组流量参数达成一致，称为**流规范**
   - Parameters include: required bandwidth, maximum delay, maximum jitter, etc.
     > **中文**：参数包括：所需带宽、最大延迟、最大抖动等
   - If the network cannot meet the specification, the flow is **rejected** (not admitted)
     > **中文**：如果网络无法满足规范，该流被**拒绝**（不接纳）

> **中文**：准入控制通过QoS路由（沿路径预留）和流协商（应用与路由器达成流规范协议）来防止拥塞。

---

## 4.7 Internet Protocol (IP) / 互联网协议（IP） ★★★

### 4.7.0 IPv4 Header Fields / IPv4头部字段 (All Fields, Must Memorize)

The IPv4 header is typically 20 bytes (without options). All fields:

> **中文**：IPv4头部通常为20字节（不含选项）。所有字段如下：

| Field / 字段 | Bits / 比特 | Description / 描述 |
|---------|:---:|-------------|
| **Version / 版本** | 4 | IP version (4 for IPv4) / IP版本（IPv4为4） |
| **IHL (Header Length) / 头部长度** | 4 | Length of header in 32-bit words (min=5, max=15) / 头部长度，以32位字为单位（最小5，最大15） |
| **TOS (Type of Service) / 服务类型** | 8 | DSCP + ECN for QoS differentiation / 用于QoS区分的DSCP+ECN |
| **Total Length / 总长度** | 16 | Total datagram size in bytes (header + data) / 数据报总字节数（头部+数据） |
| **Identification / 标识符** | 16 | Used for fragmentation: all fragments share same ID / 用于分片：同一数据报的所有分片共享相同ID |
| **Flags / 标志** | 3 | DF (Don't Fragment), MF (More Fragments), reserved bit / 不分片、更多分片、保留位 |
| **Fragment Offset / 片偏移** | 13 | Position in 8-byte units from start of original datagram / 以8字节为单位的偏移量 |
| **TTL (Time to Live) / 生存时间** | 8 | Decremented by each router; dropped when 0 / 每台路由器减1；为0时丢弃 |
| **Protocol / 协议** | 8 | Identifies next-layer protocol (TCP=6, UDP=17, ICMP=1, OSPF=89) / 标识上层协议 |
| **Header Checksum / 头部校验和** | 16 | Error-checking on header only (not payload) / 仅对头部进行差错校验 |
| **Source IP Address / 源IP地址** | 32 | Source's IP address / 源IP地址 |
| **Destination IP Address / 目的IP地址** | 32 | Destination's IP address / 目的IP地址 |
| **Options / 选项** | variable | Optional: security, record route, timestamp, etc. (padded to 32-bit boundary) / 可选：安全、记录路由、时间戳等（填充到32位边界） |
| **PAD / 填充** | variable | Ensures header ends on a 32-bit boundary / 确保头部结束在32位边界上 |

> **中文**：IPv4头部共14个字段。关键字段：Version(4b)、IHL(4b)、TOS(8b)、Total Length(16b)、Identification(16b)、Flags(3b)、Fragment Offset(13b)、TTL(8b)、Protocol(8b)、Header Checksum(16b)、Source IP(32b)、Dest IP(32b)、Options(可变)、PAD(可变)。总长度指整个数据报（头部+数据）的字节数。

**Key points / 关键点**：
- Not all fields are used by "core" IP functions — some are only needed for fragmentation or options
  > **中文**：并非所有字段都被"核心"IP功能使用——某些仅用于分片或选项
- "Context-less": IP has no state or session information (stateless design)
  > **中文**：IP是无上下文的，不维护状态或会话信息（无状态设计）
- IHL = 5 (minimum) means header is 5 × 32-bit words = 20 bytes (no options)
  > **中文**：IHL=5（最小值）表示头部为5×32位字=20字节（无选项）
- TOS field in modern use: DSCP (6 bits) + ECN (2 bits)
  > **中文**：现代TOS字段使用：DSCP（6比特）+ ECN（2比特）

### 4.7.1 IPv4 Addresses / IPv4地址

**Format / 格式**：
- 32-bit address, usually written in **dotted decimal notation**【点分十进制记法】：4 bytes separated by dots, e.g., `192.168.1.1`
  > **中文**：32位地址。通常写作**点分十进制记法**：4个字节用点分隔，例如192.168.1.1。
- **Function / 功能**：Specifies an **interface** (network connection), not a host! A multi-homed host may have multiple IP addresses.
  > **中文**：IP地址指定一个**接口**（网络连接），而非一台主机！多宿主主机可以有多个IP地址。
- **1-to-1 mapping / 一一映射**：dotted-decimal ↔ 32-bit binary (no naming — DNS handles naming)
  > **中文**：点分十进制与32位二进制一一映射。命名由DNS处理。
- **Hierarchical / 层次化**：2-level (network id + host id) → 3-level (network id + subnet id + host id) with subnetting
  > **中文**：层次化结构：2层（网络号+主机号）→ 3层（网络号+子网号+主机号）通过子网划分

**Dotted Decimal to Binary Conversion Example / 点分十进制转二进制示例**：

> **中文**：`202.120.188.98` → `11001010 01111000 10111100 01100010`

### 4.7.2 Classful Addressing / 分类编址

| Class / 类别 | First bits / 起始比特 | Range (1st byte) / 范围（首字节） | Network bits / 网络位 | Host bits / 主机位 | Networks / 网络数 | Hosts/Network / 每网络主机数 |
|:-----:|:---:|------|:---:|:---:|:---:|:---:|
| **A** | 0 | 1–126 | 8 | 24 | 126 | $2^{24}-2 \approx 16.7M$ |
| **B** | 10 | 128–191 | 16 | 16 | 16,384 | $2^{16}-2 = 65,534$ |
| **C** | 110 | 192–223 | 24 | 8 | ~2M | $2^8-2 = 254$ |
| **D** | 1110 | 224–239 | Multicast / 组播 | N/A | N/A | N/A |
| **E** | 1111 | 240–255 | Reserved / 保留 | N/A | N/A | N/A |

**Note / 注意**：127.0.0.0/8 is reserved for **loopback**【环回】 (127.0.0.1 is "localhost"). So Class A technically starts at 1 but 127 is excluded.

> **中文**：127.0.0.0/8保留用于**环回**（127.0.0.1是"本地主机"）。所以A类从1开始但排除了127。

### 4.7.3 Special IP Addresses / 特殊IP地址 ★★★ (Must Memorize / 必须牢记)

| Type / 类型 | Netid / 网络号 | Hostid / 主机号 | As Source? / 能否作源地址？ | As Destination? / 能否作目的地址？ | Purpose / 用途 |
|------|:---:|:---:|:---:|:---:|------|
| **Network address / 网络地址** | Specific / 特定 | All 0s / 全0 | No / 否 | No / 否 | Identifies the network itself / 标识网络本身 |
| **Direct broadcast / 直接广播** | Specific / 特定 | All 1s / 全1 | No / 否 | **Yes / 是** | Broadcast to all hosts on a specific network / 向特定网络上的所有主机广播 |
| **Limited broadcast / 有限广播** | All 1s / 全1 | All 1s / 全1 | No / 否 | **Yes / 是** | Broadcast to all hosts on this local network (255.255.255.255) / 向本地网络上的所有主机广播 |
| **This host on this net / 本网上的本主机** | All 0s / 全0 | All 0s / 全0 | **Yes / 是** | No / 否 | Used during bootstrap (0.0.0.0) / 在引导过程中使用 |
| **Specific host on this net / 本网上的特定主机** | All 0s / 全0 | Specific / 特定 | **Yes / 是** | **Yes / 是** | Communicate on local network without knowing network ID / 在不知道网络ID的情况下在本地网络上通信 |
| **Loopback / 环回** | 127 | Any / 任意 | **Yes / 是** | **Yes / 是** | Test TCP/IP stack locally (127.0.0.1) / 本地测试TCP/IP协议栈 |

**Key distinctions for exam / 考试关键区分**：
- **Direct broadcast / 直接广播**（e.g. / 例如 192.168.1.255）：Routers **may** forward to the target network / 路由器**可以**转发到目标网络
- **Limited broadcast / 有限广播**（255.255.255.255）：Routers **NEVER** forward — stays on local network / 路由器**从不**转发 — 仅在本地网络

### 4.7.4 Private IP Addresses / 私有IP地址 (RFC 1918)

These addresses are NOT routable on the public Internet:

> **中文**：这些地址在公共互联网上**不可路由**：

| Class / 类别 | Private Range / 私有范围 | CIDR | Number of Addresses / 地址数量 |
|:---:|------|------|:---:|
| A | 10.0.0.0 – 10.255.255.255 | 10.0.0.0/8 | $2^{24}$ |
| B | 172.16.0.0 – 172.31.255.255 | 172.16.0.0/12 | $2^{20} \times 16$ |
| C | 192.168.0.0 – 192.168.255.255 | 192.168.0.0/16 | $2^{16} \times 256$ |

### 4.7.5 Subnetting and CIDR / 子网划分与CIDR ★★★

**Subnetting**【子网划分】：Borrow host bits to create subnet bits. A network is divided into smaller subnetworks. (RFC 950, RFC 1122 — host must support subnetting)

> **中文**：借用主机位来创建子网位。将一个网络划分为更小的子网。（RFC 950，RFC 1122——主机必须支持子网划分）

**Key benefits of subnetting / 子网划分的关键优势**：
- Hides internal network structure from the outside world (good for administration)
  > **中文**：对外部隐藏内部网络结构（便于管理）
- Does **NOT** increase the size of routing tables
  > **中文**：**不**增加路由表大小
- Helps with address space exhaustion
  > **中文**：帮助缓解地址空间耗尽问题

**Subnet Masks (子网掩码) / 掩码详解**：
- **Masking**: The process of extracting the address of the physical network from an IP address
  > **中文**：**掩码操作**：从IP地址中提取物理网络地址的过程
- **Mask**: A 32-bit number. Bits are set to:
  > **中文**：**掩码**：一个32位数。位设置规则：
  - **1**: If this bit is part of the (sub-)network address / 如果该位属于（子）网络地址
  - **0**: If this bit is part of the host address / 如果该位属于主机地址
- **Subnet mask** → extracts Subnet ID; **Network mask** → extracts Network ID
  > **中文**：子网掩码→提取子网ID；网络掩码→提取网络ID

**Bitwise-AND Operation Example / 按位与运算示例**：

Given IP address `152.1.54.48`:

| Operation / 操作 | Binary / 二进制 | Dotted Decimal / 点分十进制 |
|---------|:---:|:---:|
| IP Address / IP地址 | 10011000 00000001 00110110 00110000 | 152.1.54.48 |
| Classful (implied) mask / 默认（分类）掩码 | 11111111 11111111 00000000 00000000 | 255.255.0.0 |
| **Network Address (AND result)** / **网络地址** | **10011000 00000001 00000000 00000000** | **152.1.0.0** |
| Subnet Mask / 子网掩码 | 11111111 11111111 11111111 00000000 | 255.255.255.0 |
| **Subnetwork Address (AND result)** / **子网地址** | **10011000 00000001 00110110 00000000** | **152.1.54.0** |

> **中文**：通过按位与运算提取网络地址。IP地址与掩码按位与：结果中掩码为1的位保留原值，为0的位清零。使用不同掩码可得到不同级别的网络地址（网络地址vs子网地址）。

- Given IP / 给定IP：`192.168.1.0/24` → 256 addresses (254 usable for hosts) / 256个地址（254个可用）
- Subnet into 4 subnets / 划分为4个子网：need 2 bits / 需要2个比特 → `/26`
- Each subnet / 每个子网：64 addresses, 62 usable / 64个地址，62个可用
- Subnet mask / 子网掩码：`255.255.255.192`

**CIDR (Classless Inter-Domain Routing) / 无类别域间路由**：Abolishes class boundaries. Address written as `X.X.X.X/n` where `/n` is the prefix length (number of network bits).

> **中文**：废除类别边界。地址写作X.X.X.X/n，其中/n是前缀长度（网络位数量）。

**CIDR as "Cinderella solution" / CIDR作为"灰姑娘"解决方案**：Not a permanent fix — still a short-term solution. The long-term solution is **IPv6**.

> **中文**：CIDR并非永久解决方案——仍是短期方案。长期解决方案是**IPv6**。

**CIDR Core Idea / CIDR核心思想**：
- Collapse a block of contiguous addresses into a **single logical network**
  > **中文**：将一块连续地址**折叠成一个逻辑网络**
- Not restricted to Class C addresses — can arbitrary sizes
  > **中文**：不限于C类地址——可以有任意大小
- Entry in routing table: **(network address, mask)** / 路由表项：（网络地址，掩码）
- 32-bit Network address: the **smallest address** in the block / 32位网络地址：块中**最小地址**
- 32-bit Mask: can derive the number of addresses in this network / 32位掩码：可推导该网络中的地址数量

| Site / 站点 | Lowest / 最低地址 | Highest / 最高地址 | Mask / 掩码 | Total / 总数 |
|:---:|:---:|:---:|:---:|:---:|
| 1 | 194.24.0.0 | 194.24.7.255 | 255.255.248.0 | $2^{11} = 2048$ |
| 2 | 194.24.8.0 | 194.24.11.255 | 255.255.252.0 | $2^{10} = 1024$ |
| 3 | 194.24.16.0 | 194.24.31.255 | 255.255.240.0 | $2^{12} = 4096$ |

> **中文**：CIDR将连续地址块折叠为单一逻辑网络。路由表项为（网络地址，掩码）。32位网络地址是块中最小地址，掩码可推导地址数量。

**CIDR Forwarding Example (Binary AND) / CIDR转发示例（二进制AND）**：

Packet addressed to `194.24.17.4`:
1. `194.24.17.4` = **11000010 00011000 00010001 00000100**
2. AND with Site 1 mask (255.255.248.0): `11000010 00011000 00010000 00000000` → DOES NOT match Site 1 base (194.24.0.0) ❌
3. AND with Site 2 mask (255.255.252.0): `11000010 00011000 00010000 00000000` → DOES NOT match Site 2 base (194.24.8.0) ❌
4. AND with Site 3 mask (255.255.240.0): `11000010 00011000 00010000 00000000` → **MATCHES** Site 3 base (194.24.16.0) ✓ → **Sent to R3**

> **中文**：CIDR转发时，路由器将目的IP与每个路由表项的掩码做按位与，结果与该项的网络地址比较。匹配的项用于转发。

**Route Aggregation (路由聚合)**: Combine multiple contiguous prefixes into one shorter prefix. This reduces routing table size.

> **中文**：将多个连续的前缀合并为一个更短的前缀。这减少了路由表的大小。

**Supernetting (超网) / 超网技术**：
- **Why / 为什么**：Exhaustion of Class B addresses; routing table size explosion
  > **中文**：B类地址耗尽；路由表大小爆炸
- **Main idea / 核心思想**：Aggregation of **2ⁿ contiguous Class C addresses** into a single supernet
  > **中文**：将**2ⁿ个连续的C类地址**聚合成一个超网
- **Example / 示例**：Need 2000 addresses → allocate a block of 2048 addresses (8 contiguous Class C blocks = 8 × 256 = 2048)
  > **中文**：需要2000个地址→分配一个2048地址的块（8个连续C类块）
- **Flexible masking / 灵活掩码**：Using a shorter prefix to aggregate multiple routes
  > **中文**：使用更短的前缀聚合多条路由

**Example / 聚合示例**：192.15.32.0 through 192.15.35.0/24 (4 Class C blocks):
- With 4 separate entries: 192.15.32.0/24, 192.15.33.0/24, 192.15.34.0/24, 192.15.35.0/24
- Aggregated: **192.15.32.0/22** (255.255.252.0) → covers all 4 blocks

> **中文**：超网示例：4个连续的C类地址块（192.15.32.0/24~192.15.35.0/24）可聚合成单一超网192.15.32.0/22。

**Example / 示例**：Aggregate / 聚合 `192.168.0.0/24` and `192.168.1.0/24`：
They share the first 23 bits / 它们共享前23位 → aggregated prefix / 聚合前缀 = `192.168.0.0/23`

**Longest Prefix Match / 最长前缀匹配**：When routing, if multiple entries match a destination IP, use the entry with the **longest prefix** (most specific match).

> **中文**：路由时，如果多个表项匹配某个目的IP，使用**前缀最长**（最具体匹配）的表项。

### 4.7.6 IP Forwarding and Routing Tables / IP转发与路由表

**Core Principle / 核心原则**：Routers look at the **destination IP address** of a packet to make forwarding decisions, and **nothing else**!

> **中文**：路由器仅根据分组的**目的IP地址**做出转发决策，不看其他信息！

**Routing Table Structure / 路由表结构**：

| Entry / 表项 | Description / 描述 |
|---------|-------------|
| **Destination / 目的地址** | Network/subnet IP address (or host address) / 网络/子网IP地址（或主机地址） |
| **Outgoing link / 输出链路** | Which interface to forward the packet out of / 将分组转发到哪个接口 |
| **Next-Hop Router / 下一跳路由器** | The next router on the path to the destination / 到达目的地的路径上的下一台路由器 |

> **中文**：路由表包含目标网络地址和对应的输出链路/下一跳路由器。

**Indexing rule / 索引规则**：Routers index by the **network part** of the destination IP address (not the full host address).

> **中文**：路由器按目的IP地址的**网络部分**进行索引（而非完整主机地址）。

**Example / 示例**：Router A's routing table:

| Network / Subnet / 网络/子网 | Delivery / 交付方式 |
|--------------------|--------------|
| 128.171.17.0 | Local (direct delivery) / 本地（直接交付） |
| 142.99.0.0 | Next-Hop Router B / 下一跳路由器B |

> **中文**：路由表示例：128.171.17.0 → 本地直接交付；142.99.0.0 → 下一跳路由器B。

#### Forwarding Tables with Masks / 带掩码的转发表

After subnetting, the routing table adds a **mask column**【掩码列】:

> **中文**：引入子网划分后，路由表增加**掩码列**。

| Net/Subnet Mask (# of 1's) / 掩码（1的数量） | Destination / 目的地 | Interface / 接口 | Next-Hop / 下一跳 |
|:---:|:---:|:---:|:---:|
| 24 | 128.171.17.0 | 1 | Local / 本地 |
| 8 | 142.0.0.0 | 2 | Router B |

**How it works / 工作原理**：
- Both destination and mask are 32-bit numbers
- Router performs **bitwise-AND** of destination IP with the mask, then compares with the stored destination
  > **中文**：路由器对目的IP与掩码执行**按位与**运算，然后与存储的目标地址比较
- E.g., destination IP 128.171.17.13, mask /24 → AND with 255.255.255.0 → 128.171.17.0 → matches local delivery
  > **中文**：例如目的IP 128.171.17.13，掩码/24，与255.255.255.0按位与得到128.171.17.0，匹配本地直接交付

### 4.7.7 Default Routes / 默认路由

**Problem / 问题**：Too many networks/destinations in the network. Technically, every router would need next-hop information for **every network in the Internet**, which is impossible.

> **中文**：网络中的目的地太多，理论上每个路由器需要知道到互联网中**每个网络**的下一跳信息，这是不可能的。

**Solution / 解决方案**：**Default Route**【默认路由】
- Keep detailed routes only for the **local neighborhood** / 仅对**局部区域**保持详细路由
- Use a **default route/next-hop** for all unknown destinations / 对所有未知目的地使用**默认路由/下一跳**
- In the routing table: destination = `0.0.0.0`, mask = `0.0.0.0` (matches everything)
  > **中文**：路由表中默认路由表示为dest=0.0.0.0, mask=0.0.0.0（匹配所有地址）

**Results / 结果**：
- **Saves space** in the routing table / 节省路由表空间
- But may select **non-optimal paths** for distant destinations / 但可能为远距离目的地选择**非最优路径**

> **中文**：默认路由为所有未知目的地提供下一跳信息，节省路由表空间但可能选择非最优路径。

### 4.7.8 Forwarding Table Selection / Matching Rules / 转发表选择/匹配规则

**Process / 过程**：
1. Compare destination IP of arriving packet against **ALL rows** in the forwarding table
   > **中文**：将到达分组的目的是IP与转发表中的**所有行**进行比较
2. Select the row that **matches** (after applying the mask)
   > **中文**：选择**匹配**的行（应用掩码后）
3. If **multiple rows match**, select the **longest match** (most specific)
   > **中文**：如果**多行匹配**，选择**最长匹配**（最具体的）
4. If **no match**, select the **default row** (0.0.0.0/0)
   > **中文**：如果**无匹配**，选择**默认行**（0.0.0.0/0）

**Example / 示例**：

| Line / 行 | Destination / 目的地址 | Netmask / 掩码 | Interface / 接口 | Next-Hop Router / 下一跳 |
|:---:|:---:|:---:|:---:|:---:|
| 1 | 152.19.0.0 | /16 | 2 | B |
| 2 | 152.15.33.0 | /24 | 1 | Local / 本地 |
| 3 | 152.40.0.0 | /16 | 2 | B |
| 4 | 152.229.0.0 | /16 | 1 | D |
| 5 | 152.40.6.0 | /24 | 3 | E |
| 6 (default) | 0.0.0.0 | /0 | 3 | H |

- Packet to `152.40.6.31`: matches lines 3 AND 5 → **longest match** = line 5 (/24 is more specific than /16) → sent to E
  > **中文**：目的IP 152.40.6.31同时匹配第3行和第5行，选择**最长匹配**（第5行，/24比/16更具体），发送到E
- Packet to `152.19.5.3`: matches line 1 → sent to B
- Packet to `1.2.3.4`: no match → default route → sent to H

> **中文**：最长前缀匹配规则：在多个匹配的路由表项中选择前缀长度最长（最具体）的那个。

### 4.7.9 Static Configuration Example / 静态配置示例

A simple network with routers and hosts:

> **中文**：一个简单的路由器与主机静态配置示例。

**Topology / 拓扑**：
```
H1 (192.168.1.2) ─── R1 (192.168.1.1 / 152.40.6.31) ─── R2 (152.40.6.35) ─── H3
                    192.168.1.0/24       152.40.6.0/24       192.168.2.0/24
H2
```

**H1 Configuration / H1配置**：
- IP address: 192.168.1.2
- Subnet Mask: 255.255.255.0
- Default Gateway: 192.168.1.1

**Forwarding Table at R1 / R1的转发表**：

| Destination / 目的地址 | Mask / 掩码 | Next-Hop / 下一跳 |
|:---:|:---:|:---:|
| 192.168.1.0 | 255.255.255.0 | direct (local) / 直接交付 |
| 152.40.6.0 | 255.255.255.0 | direct (local) / 直接交付 |
| 0.0.0.0 | 0.0.0.0 | 152.40.6.35 (R2) |

> **中文**：H1配置：IP=192.168.1.2, 掩码=255.255.255.0, 默认网关=192.168.1.1。R1转发表：192.168.1.0/24和152.40.6.0/24直连，默认路由指向R2(152.40.6.35)。

### 4.7.10 IP Fragmentation and Reassembly / IP分片与重组 ★★★ (Classic Big Question / 经典大题)

When an IP datagram is larger than the MTU of the next network, it must be **fragmented**【分片】.

> **中文**：当IP数据报大于下一网络的MTU时，必须进行**分片**。

**Key fields / 关键字段**：
- **Identification / 标识符** (16 bits / 比特)：All fragments of the same datagram share the same ID / 同一数据报的所有分片共享相同ID
- **MF (More Fragments) / 更多分片标志** (1 bit / 比特)：0 = last fragment / 最后一个分片，1 = more fragments follow / 后面还有更多分片
- **DF (Don't Fragment) / 不分片标志** (1 bit / 比特)：If set, router drops packet rather than fragment it / 如果设置，路由器丢弃分组而不进行分片
- **Fragment Offset / 片偏移** (13 bits / 比特)：Position of this fragment in the original datagram, measured in **8-byte units** / 该分片在原始数据报中的位置，以**8字节为单位**计量

**IMPORTANT / 重要**：Fragment offset is in units of **8 bytes (64 bits)**. Fragment data length (excluding IP header) must be a multiple of 8 bytes.

> **中文**：片偏移以**8字节（64比特）**为单位。分片的数据长度（不含IP头部）必须是8的倍数。

**Example / 示例**：3800-byte UDP datagram / UDP数据报 + 20-byte IP header / IP头部 = 3820-byte IP datagram / IP数据报. MTU = 1420 bytes. Fragment? / 需要分片吗？

Each fragment can have at most $1420 - 20 = 1400$ bytes of IP payload. But must be multiple of 8 → max 1400 is already a multiple of 8. ✓

> **中文**：每个分片最多可以有1420-20=1400字节的IP负载。但必须是8的倍数，1400正好是8的倍数（1400/8=175）。

| Fragment / 分片 | Total Length / 总长度 | Data Length / 数据长度 | Identification / 标识符 | MF / 更多分片 | DF / 不分片 | Fragment Offset / 片偏移 |
|----------|:---:|:---:|:---:|:---:|:---:|:---:|
| Original / 原始 | 3820 | 3800 | 12345 | 0 | 0 | 0 |
| Fragment 1 / 分片1 | 1420 | 1400 | 12345 | **1** | 0 | **0** |
| Fragment 2 / 分片2 | 1420 | 1400 | 12345 | **1** | 0 | **175** ($1400/8$) |
| Fragment 3 / 分片3 | 1020 | 1000 | 12345 | **0** | 0 | **350** ($2800/8$) |

Check / 验证：$1400 + 1400 + 1000 = 3800 =$ original data / 原始数据长度 ✓

**Fragment offset calculation / 片偏移计算**：Each fragment's offset = (sum of data bytes in all previous fragments) / 8.
（中文：每个分片的偏移量 = 之前所有分片的数据字节数之和 / 8）

> **中文详细解释**：分片时，每个分片的数据长度必须是8的倍数，因为片偏移字段以8字节为单位。
> 分片1：偏移0，数据1400字节
> 分片2：偏移 = 1400/8 = 175，数据1400字节
> 分片3：偏移 = (1400+1400)/8 = 350，数据1000字节
> 所有分片的数据之和 = 1400+1400+1000 = 3800字节 = 原始数据长度

---

## 4.8 NAT (Network Address Translation) / 网络地址转换 ★★★

**Purpose / 目的**：Allow multiple hosts on a private network to share a single public IP address. (RFC 2663)

> **中文**：允许私有网络上的多个主机共享一个公共IP地址。（RFC 2663）

### 4.8.0 NAT Translation Rules / NAT转换规则

NAT follows specific translation rules based on the direction of traffic:

> **中文**：NAT根据流量方向遵循特定的转换规则。

| Direction / 方向 | Rule / 规则 |
|---------|--------|
| **Outside to Inside** / 外部到内部 | **Translate then route** / 先转换再路由 |
| **Inside to Outside** / 内部到外部 | **Route then translate** / 先路由再转换 |

> **中文**：外部到内部：先转换再路由；内部到外部：先路由再转换。

**PAT (Port Address Translation) / 端口地址转换**：
- Also known as **NAPT (Network Address Port Translation)** / 也称为**NAPT（网络地址端口转换）**
- "One to Many" mapping: one public IP + many port numbers → many private hosts
  > **中文**："一对多"映射：一个公共IP + 多个端口号 → 多个私有主机
- This is the most common form of NAT used in home/small office routers
  > **中文**：这是家庭/小型办公室路由器中最常见的NAT形式

### 4.8.1 How NAT Works / NAT的工作原理

1. Internal host sends packet with private source IP
   内部主机发送带有私有源IP的分组
2. NAT router replaces source IP with its public IP, source port with a new port number
   NAT路由器将源IP替换为其公共IP，源端口替换为新端口号
3. NAT router records this mapping in the **NAT translation table**【NAT转换表】
   NAT路由器将此映射记录在**NAT转换表**中
4. Reply arrives at NAT router; router looks up destination port, translates back to private IP/port
   回复到达NAT路由器；路由器查找目的端口，转换回私有IP/端口
5. Forward to internal host
   转发给内部主机

### 4.8.2 Key Distinction: NAT Router vs Regular Router / 关键区别：NAT路由器与普通路由器

| | Regular Router / 普通路由器 | NAT Router / NAT路由器 |
|--|:---:|:---:|
| Layers examined / 检查的层 | Network layer (IP only) / 网络层（仅IP） | Network + **Transport** layer (IP + ports) / 网络层 + **传输层**（IP+端口） |
| Source IP change / 源IP是否改变 | Never changes / 从不改变 | Changes (from private to public) / 改变（私有→公共） |
| Port number touched / 端口号是否修改 | Never / 从不 | Changes (port translation) / 改变（端口转换） |

> **中文**：普通路由器仅检查网络层（IP地址），不看传输层信息。NAT路由器需要检查传输层端口号，因为多个内部主机共享一个公共IP时，必须通过端口号来区分不同的连接。

### 4.8.3 NAT Translation Table / NAT转换表

| WAN-side IP / WAN侧IP | WAN-side Port / WAN侧端口 | LAN-side IP / LAN侧IP | LAN-side Port / LAN侧端口 |
|-------------|:---:|-------------|:---:|
| Public IP of router / 路由器的公共IP | 80 (e.g. / 例如) | 192.168.1.2 | 80 |

### 4.8.4 Typical Exam Problem / 典型考试题目

Given topology with two subnets using same private address space and NAT, fill in the NAT table and trace IP address changes as a packet traverses the network.

> **中文**：给定一个拓扑，其中两个子网使用相同的私有地址空间并经过NAT，填写NAT表并追踪分组穿越网络时IP地址的变化。

**Steps / 步骤**：
1. Identify source and destination at each hop / 识别每跳的源地址和目的地址
2. At NAT router: source IP changes (internal → external direction) / 在NAT路由器：源IP改变（内部→外部方向）
3. At NAT router: destination IP changes (external → internal direction based on table) / 在NAT路由器：目的IP改变（外部→内部方向，基于转换表）
4. Port numbers may change depending on NAPT vs basic NAT / 端口号可能根据NAPT与基本NAT而改变

> **中文**：NAT典型题目中，分组从内部主机发送到外部服务器时，源IP从私有地址变为公共地址。返回时，目的IP从公共地址根据NAT表转换回私有地址。端口号也被映射以区分不同内部主机的连接。

---

## 4.9 ARP (Address Resolution Protocol) / 地址解析协议

**Purpose / 目的**：Maps an IP address to a MAC address on a local network.

> **中文**：将IP地址映射为本地网络上的MAC地址。

**Process / 工作过程**：
1. Host A wants to send to IP X. Checks its **ARP cache**【ARP缓存】 for (IP X → MAC).
   主机A想发送给IP X。检查其**ARP缓存**中是否有（IP X → MAC）的映射。
2. If not found: broadcasts **ARP Request**【ARP请求】："Who has IP X? Tell A"
   如果未找到：广播发送**ARP请求**："谁有IP X？请告诉A"
3. Host with IP X replies with **unicast ARP Reply**【ARP回复】："IP X is at MAC xx:xx:xx:xx:xx:xx"
   拥有IP X的主机以**单播ARP回复**回应："IP X 的MAC地址是xx:xx:xx:xx:xx:xx"
4. A caches the mapping and sends the data frame
   A缓存该映射并发发送数据帧

**Important points / 重要要点**：
- ARP request is **broadcast** (MAC destination = FF-FF-FF-FF-FF-FF) / ARP请求是**广播**的（MAC目的地址为全F）
- ARP reply is **unicast** (sent directly to requester) / ARP回复是**单播**的（直接发送给请求者）
- ARP only works within a **single broadcast domain** (LAN) / ARP仅在**单个广播域**（局域网）内工作
- For hosts on different networks: use ARP to find the **default gateway's MAC**【默认网关的MAC】，then router handles routing / 对于不同网络上的主机：使用ARP查找默认网关的MAC，然后由路由器处理路由
- ARP cache entries have a TTL (typically a few minutes) / ARP缓存表项有生存时间（通常几分钟）

**ARP cache entry / ARP缓存表项**：(IP address / IP地址, MAC address / MAC地址, Type/TTL / 类型/生存时间)

**RARP (Reverse ARP) / 反向地址解析协议**：
- **RARP**: Maps a **MAC address → IP address** (the reverse of ARP) / **RARP**：将**MAC地址映射到IP地址**（ARP的逆过程）
- Used historically by diskless workstations to discover their IP address at boot time
  > **中文**：历史上由无盘工作站启动时用来发现自己的IP地址
- Has been largely replaced by **BOOTP** and **DHCP**
  > **中文**：已被BOOTP和DHCP取代

**NDP (Neighbor Discovery Protocol) / 邻居发现协议**：
- In **IPv6**, ARP is replaced by **NDP** (Neighbor Discovery Protocol) / 在**IPv6**中，ARP被**NDP**取代
- NDP provides similar functionality (address resolution) plus additional features (router discovery, prefix discovery, etc.)
  > **中文**：NDP提供类似功能（地址解析）以及其他额外功能（路由器发现、前缀发现等）

---

## 4.10 ICMP (Internet Control Message Protocol) / 互联网控制报文协议

**Encapsulation / 封装方式**：ICMP messages are carried **inside IP datagrams** (protocol number = 1).

> **中文**：ICMP报文**封装在IP数据报内部**传输（协议号=1）。

**Key concept / 关键概念**：ICMP only **reports** errors — it does **NOT perform handling actions**. It informs the source about network-level errors or unexpected circumstances.

> **中文**：ICMP**仅报告**错误——它**不执行处理行动**。它通知源端网络层错误或意外情况。

**Common tools using ICMP / 使用ICMP的常见工具**：
- **ping** / **ping命令**：Uses ECHO Request (Type 8) and ECHO Reply (Type 0)
- **traceroute** / **traceroute命令**：Uses TIME EXCEEDED (Type 11)

### 4.10.1 Error Reporting Messages / 差错报告报文

| Type / 类型 | Name / 名称 | When Sent / 发送时机 |
|------|------|-----------|
| 3 | Destination Unreachable / 目的地不可达 | Router cannot deliver packet / 路由器无法交付分组 |
| 4 | Source Quench / 源端抑制 | Router requests sender to slow down (now deprecated) / 路由器要求发送方减速（现已废弃） |
| 5 | Redirect / 重定向 | Router tells host to use a better route / 路由器告诉主机使用更优路由 |
| 11 | Time Exceeded / 超时 | TTL reached 0 (used by **traceroute**) / TTL达到0（用于**traceroute**） |
| 12 | Parameter Problem / 参数问题 | Invalid header field / 无效的头部字段 |

### 4.10.2 Query Messages / 查询报文

| Type / 类型 | Name / 名称 | Purpose / 用途 |
|------|------|---------|
| 8/0 | Echo Request / Echo Reply / 回显请求/回显应答 | Test reachability (used by **ping**) / 测试可达性（用于**ping**） |
| 13/14 | Timestamp Request / Reply / 时间戳请求/应答 | Clock synchronization and RTT estimation / 时钟同步和RTT估计 |

---

## 4.11 IPv6 / 互联网协议第6版

128-bit addresses. Written as 8 groups of 4 hexadecimal digits separated by colons.

> **中文**：128位地址。写作8组4个十六进制数字，用冒号分隔。

**Key improvements over IPv4 / 相对于IPv4的关键改进**：
- **Larger address space / 更大的地址空间**：128 bits vs 32 bits ($2^{96}$ times more addresses) / 128位 vs 32位（地址数量多2的96次方倍）
- Simplified fixed-length header (40 bytes) / 简化的固定长度头部（40字节）
- **Flow label**【流标签】：New field in IPv6 header for per-flow packet classification without examining transport-layer headers / IPv6头部新增字段，用于逐流分组分类，无需检查传输层头部
- Better support for options (extension headers) / 更好地支持选项（扩展头部）
- Built-in security (IPsec) / 内置安全机制（IPsec）
- No fragmentation by routers (path MTU discovery required) / 路由器不再进行分片（需要路径MTU发现）
- No checksum in header / 头部中无校验和
- ARP replaced by **NDP (Neighbor Discovery Protocol)** / ARP被**NDP**取代

---

## 4.12 Routing Protocols / 路由协议 ★★★

### 4.12.1 Autonomous System (AS) / 自治系统（AS）

An **Autonomous System (AS)**【自治系统】 is a collection of networks under a single administrative domain. Routing protocols are categorized based on whether they operate **within** or **between** ASes.

> **中文**：**自治系统（AS）**是在单一管理域下的网络集合。路由协议根据其是在AS**内部**还是**之间**运行进行分类。

| Type / 类型 | Full Name / 全称 | Scope / 范围 | Examples / 示例 |
|------|-----------|-------|----------|
| **IGP** | Interior Gateway Protocol / 内部网关协议 | **Within** an AS / AS**内部** | RIP, OSPF |
| **EGP** | Exterior Gateway Protocol / 外部网关协议 | **Between** ASes / AS**之间** | BGP |

### 4.12.2 RIP (Routing Information Protocol) / 路由信息协议 ★

| Feature / 特性 | Detail / 详细说明 |
|---------|--------|
| Algorithm / 算法 | **Distance Vector** (Bellman-Ford) / **距离向量**（贝尔曼-福特） |
| Metric / 度量 | **Hop count** (max 15, 16 = infinity/unreachable) / **跳数**（最大15，16=无穷/不可达） |
| Transport / 传输层协议 | **UDP** port 520 / **UDP**端口520 |
| Updates / 更新方式 | Broadcast every ~30 seconds (full routing table) / 每约30秒广播一次（完整路由表） |
| Convergence / 收敛速度 | Slow (count-to-infinity problem) / 慢（计数到无穷问题） |
| Suitability / 适用场景 | Small networks only / 仅适用于小型网络 |
| CIDR Support / CIDR支持 | RIPv1 (no / 不支持), RIPv2 (yes, carries subnet mask / 支持，携带子网掩码) |
| IPv6 | RIPng (Next Generation / 下一代) |

**RIP distance vector update example / RIP距离向量更新示例**：Given router R1's table and neighbor R2's advertised table, compute R1's updated table. For each destination:
（中文：给定路由器R1的路由表和邻居R2通告的路由表，计算R1更新后的路由表。对于每个目的地：）
- $D_{new} = c(R1, R2) + D_{R2}(dest)$
（中文：新距离 = R1到R2的代价 + R2到目的地的距离）
- If $D_{new} < D_{current}$ → update to use next_hop = R2 with distance $D_{new}$
（中文：如果新距离 < 当前距离 → 更新为下一跳=R2，距离=新距离）

### 4.12.3 OSPF (Open Shortest Path First) / 开放式最短路径优先 ★

| Feature / 特性 | Detail / 详细说明 |
|---------|--------|
| Algorithm / 算法 | **Link State** (Dijkstra SPF) / **链路状态**（迪杰斯特拉SPF） |
| Metric / 度量 | Configurable cost (bandwidth, delay, etc.) / 可配置的代价（带宽、延迟等） |
| Transport / 传输层协议 | **Directly over IP** (protocol 89) / **直接基于IP**（协议号89） |
| Updates / 更新方式 | Floods LSPs when topology changes; periodic refresh every 30 min / 拓扑变化时泛洪LSP；每30分钟定期刷新 |
| Convergence / 收敛速度 | Fast / 快 |
| Hierarchy / 分层结构 | Supports **areas** (area 0 = backbone) / 支持**区域**（区域0=骨干区域） |
| Authentication / 认证 | Yes (password or cryptographic) / 支持（密码或加密认证） |
| CIDR Support / CIDR支持 | Yes / 支持 |
| Load balancing / 负载均衡 | Yes (equal-cost multipath) / 支持（等价多路径） |

**OSPF Areas / OSPF区域**：Divide the AS into areas. Area 0 is the backbone. This reduces LSP flooding scope and routing table size.

> **中文**：将AS划分为多个区域。区域0是骨干区域。这减少了LSP泛洪范围和路由表大小。

**Related protocol: IS-IS (Intermediate System - Intermediate System) / 相关协议：IS-IS（中间系统到中间系统）**：
- Similar to OSPF, IS-IS is also a link-state routing protocol
- Earlier version than OSPF, similar in operation
- Also widely used in large ISP networks
- Uses Dijkstra's algorithm for SPF computation

> **中文**：IS-IS是与OSPF类似的链路状态路由协议，比OSPF更早出现，在大型ISP网络中广泛使用。

### 4.12.4 BGP (Border Gateway Protocol) / 边界网关协议 ★

| Feature / 特性 | Detail / 详细说明 |
|---------|--------|
| Algorithm / 算法 | **Path Vector** / **路径向量** |
| Transport / 传输层协议 | **TCP** port 179 / **TCP**端口179 |
| Scope / 范围 | Inter-AS (between autonomous systems) / AS间（自治系统之间） |
| Routing basis / 路由依据 | **Policy-based** (not just shortest path) / **基于策略**（不仅是最短路径） |
| Path information / 路径信息 | Carries full AS path (list of ASes traversed) / 携带完整AS路径（经过的AS列表） |
| Loop prevention / 环路防止 | Rejects routes containing its own AS number / 拒绝包含自身AS号的路由 |

### 4.12.5 RIP vs OSPF vs BGP Summary Table / RIP、OSPF与BGP对比汇总表

| | RIP | OSPF | BGP |
|--|:---:|:---:|:---:|
| Type / 类型 | IGP | IGP | EGP |
| Algorithm / 算法 | Distance Vector / 距离向量 | Link State / 链路状态 | Path Vector / 路径向量 |
| Metric / 度量 | Hop count (1-15) / 跳数 | Cost (configurable) / 代价（可配置） | AS path + policies / AS路径+策略 |
| Encapsulation / 封装 | UDP (520) | IP (proto 89) | TCP (179) |
| Update method / 更新方式 | Regular broadcast (30s) / 定期广播（30秒） | Flood on change / 变化时泛洪 | Incremental update / 增量更新 |
| Convergence / 收敛速度 | Slow / 慢 | Fast / 快 | Moderate / 中等 |
| Network size / 网络规模 | Small / 小型 | Large / 大型 | Global / 全球 |
| Hierarchy / 分层 | No / 无 | Areas / 区域 | AS Confederation / AS联盟 |
| CIDR | RIPv2 yes / RIPv2支持 | Yes / 支持 | Yes / 支持 |

---

## 4.13 Router vs Switch / 路由器与交换机对比 ★

| Aspect / 方面 | Router (L3) / 路由器（三层） | Switch (L2) / 交换机（二层） |
|--------|-------------|-------------|
| OSI Layer / OSI层 | Network (Layer 3) / 网络层（第3层） | Data Link (Layer 2) / 数据链路层（第2层） |
| Forwarding basis / 转发依据 | **IP address** (logical address) / **IP地址**（逻辑地址） | **MAC address** (physical address) / **MAC地址**（物理地址） |
| Table / 转发表 | Routing table (from routing protocol) / 路由表（来自路由协议） | MAC address table (self-learning) / MAC地址表（自学习） |
| Broadcast isolation / 广播隔离 | **Yes** (blocks broadcasts) / **是**（阻断广播） | **No** (floods broadcasts) / **否**（泛洪广播） |
| Collision domain / 冲突域 | Segmented (per port) / 隔离（每端口） | Segmented (per port) / 隔离（每端口） |
| Each port has IP? / 每个端口有IP？ | **Yes** (different network per port) / **是**（每端口不同网络） | **No** (switch works below IP) / **否**（交换机在IP层以下工作） |
| Used for / 用途 | Connecting different networks / 连接不同网络 | Connecting hosts within same network / 连接同一网络内的主机 |
| Speed / 速度 | Slower (software-based processing) / 较慢（基于软件处理） | Faster (hardware-based switching) / 较快（基于硬件交换） |

---

## 4.14 RSVP (Resource ReSerVation Protocol) / 资源预留协议

Used in **Integrated Services (IntServ)**【综合服务】 QoS architecture to reserve resources along a path.

> **中文**：用于**综合服务（IntServ）**QoS架构，沿着路径预留资源。

**Key features / 关键特性**：
- Receiver-initiated reservation / 接收方发起的资源预留
- Allows multiple senders and multiple receivers / 允许多个发送者和多个接收者
- Receivers can switch between data sources / 接收者可以在数据源之间切换
- Soft state (must be periodically refreshed) / 软状态（必须定期刷新）
- Routers merge reservation requests for the same flow / 路由器合并同一流的预留请求
- Works with multicast / 支持组播

---

## 4.15 Tunneling / 隧道技术

Encapsulating a packet (of one protocol) inside the payload of another protocol's packet. Used to connect networks of the same type through a network of a different type.

> **中文**：将一个协议的分组封装在另一个协议的分组负载中。用于通过不同类型的网络连接同类型的网络。

**Example / 示例**：IPv6 packet traversing an IPv4 network / IPv6分组穿越IPv4网络：
- The IPv6 packet is the **payload** of an IPv4 packet / IPv6分组作为IPv4分组的**负载**
- At the tunnel entrance, the router adds an IPv4 header / 在隧道入口，路由器添加IPv4头部
- At the tunnel exit, the router removes the IPv4 header and processes the original IPv6 packet / 在隧道出口，路由器移除IPv4头部并处理原始IPv6分组

**Key components / 关键组件**：At least two dual-stack routers (implementing both protocols) at the tunnel endpoints.

> **中文**：隧道端点至少需要两台双栈路由器（同时实现两种协议）。

---

## 4.16 Exam Practice: Network Layer Questions / 考试练习：网络层题目

### Fill-in-the-Blank / 填空题

1. IPv4 uses ______-bit addresses; IPv6 uses ______-bit addresses. **(32, 128)**
   **中文**：IPv4使用____位地址；IPv6使用____位地址。答案：(32, 128)

2. In IP fragmentation, the fragment offset field is measured in units of ______ bytes. **(8)**
   **中文**：在IP分片中，片偏移字段以____字节为单位。答案：(8)

3. ARP resolves ______ addresses to ______ addresses. **(IP, MAC)**
   **中文**：ARP将____地址解析为____地址。答案：(IP, MAC)

4. RIP uses the ______ algorithm; OSPF uses ______. **(distance vector, link state)**
   **中文**：RIP使用____算法；OSPF使用____算法。答案：(距离向量, 链路状态)

5. The limited broadcast address is ______. **(255.255.255.255)**
   **中文**：有限广播地址是____。答案：(255.255.255.255)

6. The loopback address range is ______. **(127.0.0.0/8)**
   **中文**：环回地址范围是____。答案：(127.0.0.0/8)

7. RIP uses ______ as its transport protocol; OSPF is encapsulated directly in ______. **(UDP, IP)**
   **中文**：RIP使用____作为传输协议；OSPF直接封装在____中。答案：(UDP, IP)

8. The Class D address range (multicast) is ______ to ______. **(224.0.0.0 to 239.255.255.255)**
   **中文**：D类地址范围（组播）是从____到____。答案：(224.0.0.0, 239.255.255.255)

9. In CIDR notation, /24 means the network prefix is ______ bits. **(24)**
   **中文**：在CIDR表示法中，/24表示网络前缀为____位。答案：(24)

10. NAT routers modify both the ______ layer address and the ______ layer port numbers. **(network, transport)**
    **中文**：NAT路由器同时修改____层地址和____层端口号。答案：(网络, 传输)

### True/False / 判断题

1. (T/F) A regular router changes the source IP address when forwarding packets. **(F — only NAT routers change IP addresses)**
   **中文**：普通路由器在转发分组时会改变源IP地址。**(F — 只有NAT路由器会改变IP地址)**

2. (T/F) 127.0.0.1 can be used as a destination address. **(T — it's the loopback address)**
   **中文**：127.0.0.1可以用作目的地址。**(T — 它是环回地址)**

3. (T/F) OSPF uses UDP port 89 for transport. **(F — OSPF uses IP protocol 89, not UDP)**
   **中文**：OSPF使用UDP端口89传输。**(F — OSPF使用IP协议号89，不是UDP)**

4. (T/F) Fragment offset = 175 means the fragment starts at byte 1400 of the original datagram. **(T — 175 × 8 = 1400)**
   **中文**：片偏移=175表示该分片从原始数据报的第1400字节开始。**(T — 175 × 8 = 1400)**

5. (T/F) 0.0.0.0 can be used as a source address during bootstrap. **(T)**
   **中文**：0.0.0.0可以在引导过程中用作源地址。**(T)**

6. (T/F) RIP's "infinity" is 16 hops; any route with ≥16 hops is considered unreachable. **(T)**
   **中文**：RIP的"无穷大"为16跳；任何≥16跳的路由被视为不可达。**(T)**

7. (T/F) Switches separate broadcast domains. **(F — routers separate broadcast domains; switches separate collision domains only)**
   **中文**：交换机隔离广播域。**(F — 路由器隔离广播域；交换机仅隔离冲突域)**

8. (T/F) IPv6 routers perform fragmentation. **(F — IPv6 routers do NOT fragment; end hosts use Path MTU Discovery)**
   **中文**：IPv6路由器执行分片。**(F — IPv6路由器不执行分片；终端主机使用路径MTU发现)**

9. (T/F) With core-based multicast trees, each router must store $n$ trees, where $n$ is the number of groups. **(T)**
   **中文**：使用核心基本组播树时，每个路由器必须存储n棵树，其中n是组数。**(T)**

10. (T/F) BGP uses TCP for reliable transport between routers. **(T — TCP port 179)**
    **中文**：BGP使用TCP在路由器之间进行可靠传输。**(T — TCP端口179)**

### Multiple Choice / 选择题

1. A network with prefix /26 supports how many usable host addresses? / 前缀为/26的网络支持多少个可用主机地址？
   a) 64 b) 62 c) 128 d) 126 **(b — $2^6 - 2 = 62$)**
   **中文**：/26表示主机位占6位，可用地址数为2^6 - 2 = 62。选b。

2. Which IP address is a private address? / 哪个IP地址是私有地址？
   a) 172.15.0.1 b) 192.168.1.1 c) 11.0.0.1 d) 128.0.0.1 **(b)**
   **中文**：192.168.x.x是私有地址范围（RFC 1918）。172.15.0.1不在私有范围（私有范围是172.16.0.0-172.31.255.255）。选b。

3. In link state routing, each router: / 在链路状态路由中，每个路由器：
   a) Sends its routing table to neighbors / 将路由表发送给邻居
   b) Floods link state info to ALL routers / 向所有路由器泛洪链路状态信息
   c) Sends updates to a designated router / 向指定路由器发送更新
   d) Only exchanges updates with the core / 仅与核心交换更新 **(b)**
   **中文**：链路状态路由的核心步骤是将LSP泛洪给网络中所有路由器。选b。

4. ARP requests are sent as: / ARP请求以什么方式发送？
   a) Unicast / 单播 b) Broadcast / 广播 c) Multicast / 组播 d) Anycast / 任播 **(b)**
   **中文**：ARP请求是广播发送的（MAC目的地址FF-FF-FF-FF-FF-FF）。选b。

5. The MF flag in IP: / IP中的MF标志：
   a) Means "Must Fragment" / 表示"必须分片"
   b) Indicates more fragments follow / 表示后面还有更多分片
   c) Prevents fragmentation / 防止分片
   d) Is set only in the last fragment / 仅在最后一个分片中设置 **(b — More Fragments)**
   **中文**：MF = More Fragments，表示后面还有更多分片。最后一个分片的MF=0。选b。

6. Which routing protocol uses path vector algorithm? / 哪个路由协议使用路径向量算法？
   a) RIP b) OSPF c) BGP d) IS-IS **(c)**
   **中文**：BGP使用路径向量（Path Vector）算法。RIP使用距离向量，OSPF使用链路状态。选c。

### Calculation Problems / 计算题

*(See Comprehensive-Big-Questions.md for full step-by-step solutions and more problems.)*
*（完整的分步解答和更多题目请参见Comprehensive-Big-Questions.md）*

1. **IP Fragmentation / IP分片**：See Section 4.7 example above / 见上文第4.7节示例
2. **CIDR Allocation / CIDR分配**：ISP with /22 block needs to allocate to customers needing 9, 15, 20, 41, 128, 260 addresses — use /28, /27, /27, /26, /24, /23 respectively
   **中文**：拥有/22地址块的ISP需要为需要9、15、20、41、128、260个地址的客户分配 —— 分别使用/28、/27、/27、/26、/24、/23掩码
3. **WFQ Scheduling / WFQ调度**：See Section 4.6.8 example above / 见上文第4.6.8节示例
4. **Token Bucket / 令牌桶**：See Section 4.6.4 examples above / 见上文第4.6.4节示例
5. **M/M/1 Queue / M/M/1队列**：$\lambda = 10$, $\mu = 20$, $T = 0.1$s, $N = 1$
6. **Distance Vector / 距离向量**：Given topology and initial tables, compute after exchange / 给定拓扑和初始路由表，计算交换后的路由表
7. **Broadcast RPF / 广播RPF**：Given sink tree, determine which packets are forwarded/discarded / 给定汇集树，确定哪些分组被转发/丢弃

---

## Appendix: Chinese-English Terminology / 中英文术语对照

### Network Layer / 网络层
| English / 英文 | Chinese / 中文 |
|------|------|
| Datagram | 数据报 |
| Virtual circuit | 虚电路 |
| Routing algorithm | 路由算法 |
| Static routing | 静态路由 |
| Dynamic routing | 动态路由 |
| Distance vector | 距离向量 |
| Link state | 链路状态 |
| Dijkstra algorithm | 迪杰斯特拉算法 |
| Sink tree | 汇集树 |
| Flooding | 泛洪法 |
| Reverse Path Forwarding (RPF) | 逆向路径转发 |
| Spanning tree broadcast | 生成树广播 |
| Multicast | 组播/多播 |
| Core-based tree | 核心基本树 |
| Hierarchical routing | 层次路由 |
| Autonomous System (AS) | 自治系统 |
| IGP / EGP | 内部网关协议 / 外部网关协议 |
| Congestion control | 拥塞控制 |
| QoS (Quality of Service) | 服务质量 |
| Traffic shaping | 流量整形 |
| Leaky bucket | 漏桶 |
| Token bucket | 令牌桶 |
| WFQ (Weighted Fair Queuing) | 加权公平排队 |
| M/M/1 queuing model | M/M/1排队模型 |
| IPv4 / IPv6 | 互联网协议第4/6版 |
| CIDR (Classless Inter-Domain Routing) | 无类别域间路由 |
| Subnetting | 子网划分 |
| Subnet mask | 子网掩码 |
| Route aggregation | 路由聚合 |
| Longest prefix match | 最长前缀匹配 |
| Fragmentation | 分片/分段 |
| MTU (Maximum Transmission Unit) | 最大传输单元 |
| MF (More Fragments) | 更多分片标志 |
| DF (Don't Fragment) | 不分片标志 |
| Fragment offset | 片偏移 |
| NAT (Network Address Translation) | 网络地址转换 |
| ARP (Address Resolution Protocol) | 地址解析协议 |
| ICMP (Internet Control Message Protocol) | 互联网控制报文协议 |
| RIP (Routing Information Protocol) | 路由信息协议 |
| OSPF (Open Shortest Path First) | 开放式最短路径优先 |
| BGP (Border Gateway Protocol) | 边界网关协议 |
| Design principles of names and addresses | 名称和地址的设计原则 |
| Routing vs Forwarding | 路由 vs 转发 |
| Graph abstraction | 图抽象 |
| Link cost | 链路代价 |
| Oscillation (routing) | 路由震荡 |
| Good news travels fast | 好消息传播得快 |
| Bad news travels slow | 坏消息传播得慢 |
| IS-IS (Intermediate System - Intermediate System) | 中间系统到中间系统 |
| Broadcast storm | 广播风暴 |
| PRUNE message (multicast) | 剪枝消息（组播） |
| Ad Hoc Network | 自组织网络 |
| AODV (Ad hoc On-Demand Distance Vector) | 按需距离向量路由协议 |
| Jitter | 抖动 |
| Lindley's Recursion | Lindley递归 |
| Priority Queuing (PQ) | 优先级队列 |
| Round Robin (RR) | 轮询调度 |
| Best Effort Model | 尽力而为模型 |
| Integrated Services (IntServ) | 综合服务模型 |
| Differentiated Services (DiffServ) | 区分服务模型 |
| DSCP (DiffServ Code Point) | 区分服务码点 |
| Call Admission Control (CAC) | 呼叫准入控制 |
| Active Queue Management (AQM) | 主动队列管理 |
| Drop-tail | 尾部丢弃 |
| Forwarding table selection | 转发表选择 |
| Default route | 默认路由 |
| Longest prefix match | 最长前缀匹配 |
| Supernetting | 超网 |
| Subnet mask / Network mask | 子网掩码 / 网络掩码 |
| Masking (bitwise-AND) | 掩码操作（按位与） |
| Label-switching | 标签交换 |
| MPLS (Multiprotocol Label Switching) | 多协议标签交换 |
| PAT (Port Address Translation) | 端口地址转换 |
| RARP (Reverse ARP) | 反向地址解析协议 |
| NDP (Neighbor Discovery Protocol) | 邻居发现协议 |
| Flow label (IPv6) | 流标签（IPv6） |
| Router | 路由器 |
| Tunneling | 隧道技术 |
| RSVP (Resource ReSerVation Protocol) | 资源预留协议 |
| Private IP address | 私有IP地址 |
| Loopback address | 环回地址 |
| Broadcast address | 广播地址 |
