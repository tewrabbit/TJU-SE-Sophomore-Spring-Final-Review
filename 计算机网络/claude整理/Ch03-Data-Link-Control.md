# Chapter 03: Data Link Layer Control / 第三章：数据链路层控制

> **Exam Relevance** / **考试重要性**：This is one of the most important chapters for big questions. The sliding window protocol (especially frame sequence number field design) is a guaranteed big question topic. CRC calculation, Hamming code, and PPP frame length are also tested. The ARQ protocol timing diagrams (filling in the blanks on sequence numbers) appear frequently.
>
> **中文**：这是做大题最重要的章节之一。滑动窗口协议（特别是帧序号字段设计）是必考大题。CRC计算、海明码和PPP帧长度也经常考查。ARQ协议时序图（填写序号空白）也频繁出现。

---

## 3.1 Data Link Layer Services / 数据链路层服务

The data link layer provides node-to-node (hop-to-hop) delivery. It uses the physical layer's raw bit service to create a reliable link.

> **中文**：数据链路层提供节点到节点（逐跳）的交付服务。它利用物理层的原始比特服务来创建一条可靠的链路。

| Service Type / 服务类型 | Description / 描述 | Example / 示例 |
|-------------|-------------|---------|
| **Unacknowledged connectionless** / **无确认无连接** | No connection, no ACK; corrupted frames discarded silently / 无连接，无确认；损坏的帧被静默丢弃 | **Ethernet** / **以太网** |
| **Acknowledged connectionless** / **有确认无连接** | No connection, but each frame is acknowledged individually / 无连接，但每一帧都被单独确认 | **Wi-Fi (802.11)** |
| **Acknowledged connection-oriented** / **有确认面向连接** | Connection established, numbered frames exchanged, acknowledged / 建立连接，交换带序号的帧并进行确认 | **Used in some WAN links** / **用于某些广域网链路** |

---

## 3.2 Framing: Four Methods / 成帧：四种方法

The data link layer must break the bit stream into discrete **frames**【帧】.

> **中文**：数据链路层必须将比特流分割成离散的**帧**。

### Method 1: Byte Count / 方法1：字节计数法

- First field of the frame indicates the **number of bytes** in the frame / 帧的第一个字段指示该帧的**字节数**
- **Fatal flaw**: If the count byte is corrupted, all subsequent frames are misaligned (resynchronization is nearly impossible) / **致命缺陷**：如果计数字节出错，后续所有帧都会错位（几乎无法重新同步）
- **No longer used** / **已不再使用**

> **中文**：字节计数法在帧的首部设置一个计数字段，标明该帧包含多少字节。其致命问题在于，一旦计数字节在传输中出错，收端将无法正确划分后续所有帧的边界，且几乎不可能重新同步。因此这种方法已不再使用。

### Method 2: Flag Bytes with Byte Stuffing (Character Stuffing) / 方法2：标志字节与字节填充（字符填充）

- Use special flag byte (e.g., `0x7E`) to mark frame start/end / 使用特殊的标志字节（如`0x7E`）标记帧的开始和结束
- If flag byte appears in data, precede it with an **escape byte** (`ESC`) / 如果标志字节出现在数据中，在其前面添加一个**转义字节**（`ESC`）
- If `ESC` appears in data, precede it with another `ESC` / 如果`ESC`出现在数据中，在其前面再添加一个`ESC`
- Used by **PPP** protocol / 用于**PPP**协议

> **中文**：字节填充法使用特殊标志字节（如`0x7E`）标示帧的起止。若数据中出现了与标志字节相同的值，发送端在其前面插入一个转义字节`ESC`；若数据中出现了`ESC`本身，则在其前面再插入一个`ESC`。接收端收到后删除插入的转义字节，恢复原始数据。PPP协议使用此方法。

### Method 3: Flag Bits with Bit Stuffing ★★★ (Must Master / 必须掌握)

**Used by HDLC protocol** / **用于HDLC协议**。The flag pattern is: **`01111110`** (six consecutive 1s) / 标志模式为：**`01111110`**（6个连续的1）。

**Transparency rule** / **透明传输规则**：To prevent the data from accidentally containing the flag / 为了防止数据中意外出现标志模式：
- **Sender** / **发送端**：After every **5 consecutive 1 bits** in the data, **automatically insert a 0** (bit stuffing) / 在数据中每遇到**连续5个1**，就**自动插入一个0**（比特填充）
- **Receiver** / **接收端**：After receiving **5 consecutive 1 bits**, **automatically delete the following 0** (bit destuffing) / 收到**连续5个1**后，**自动删除后面的0**（比特删除）

**Example** / **示例** — Sender wants to transmit / 发送端要发送：`0110111111110111110`

1. Scan for 5 consecutive 1s and note where to insert 0s / 扫描连续5个1，标记需要插入0的位置
2. Original / 原始数据：`0110 1111111 10 11111 0`
3. After bit stuffing (0 inserted after each group of 5 consecutive 1s) / 比特填充后（在每组连续5个1后插入0）：
   `0110 11111 0 11 0 10 11111 0 0`
4. Frame with flags / 加上标志后的帧：`01111110` + stuffed_data + `01111110`

> **中文**：HDLC协议使用比特填充法。标志模式为`01111110`（即6个连续1）。发送端在数据中每检测到连续5个1，就自动插入一个0；接收端收到连续5个1后，自动删除后面的0。这样数据中就不会出现类似标志模式`01111110`的序列，实现了透明传输。

**Key exam point** / **考试要点**：Bit stuffing is used when the **physical layer has no special encoding** to mark frame boundaries. The bit-stuffing approach is **easy to implement in hardware**.

**In practice** / **实际应用**:
- Framing methods are often **combined** to achieve both transparency and efficiency / 成帧方法常常**结合使用**以达到透明传输和效率的平衡
- **USB** uses a combination of bit stuffing and other techniques / **USB**协议就使用了比特填充与其他技术的结合

> **中文**：在实际系统中，成帧方法经常组合使用。例如USB协议就结合了比特填充等多种技术来实现帧边界标识。

> **中文**：当**物理层没有特殊编码**来标记帧边界时，使用比特填充。比特填充方法**易于用硬件实现**。

**What the receiver does** / **接收端的工作**：Given `01101111101101111100` / 给定数据`01101111101101111100`：
- Remove stuffed 0s after each group of 5 consecutive 1s / 删除每组连续5个1后面的填充0
- Recover / 恢复：`0110 1111111 10 11111 0 → 0110111111110111110`

### Method 4: Physical Layer Coding Violations / 方法4：物理层编码违例

- Use invalid physical encoding to mark frame boundaries / 使用非法的物理编码来标记帧边界
- e.g., in Manchester encoding, a transition always occurs in the middle of each bit; a miss can signal frame start/end / 例如，在曼彻斯特编码中，每个比特中间必然有电平跳变；没有跳变则可表示帧的起止
- No overhead for bit/byte stuffing, but depends on physical layer / 没有比特/字节填充的开销，但依赖于物理层

> **中文**：物理层编码违例方法利用物理层中非法的编码信号来标记帧边界。例如在曼彻斯特编码中，每个比特中间都有电平跳变，若检测到无跳变则代表帧的开始或结束。此方法没有填充开销，但依赖于物理层的能力。

---

## 3.3 Error Detection and Correction / 差错检测与纠正

### 3.3.1 Key Concepts / 关键概念

- **Codeword (码字)**: The $n$-bit unit transmitted over the channel, consisting of $m$ data bits + $r$ check bits. Total $n = m + r$.
  - **Encoding**: $m$-bit data → $n$-bit codeword (adds $r$ check bits) / 编码：$m$位数据 → $n$位码字（添加$r$位校验）
  - **Decoding**: $n$-bit codeword → $m$-bit data (with possible error detection/correction) / 解码：$n$位码字 → $m$位数据（可能包含错误检测/纠正）
  > **中文**：码字是在信道上传输的$n$比特单元，由$m$个数据比特 + $r$个校验比特组成，总长度$n = m + r$。
- **Hamming distance【海明距离/码距】**: Number of bit positions in which two codewords differ / 两个码字之间不同的比特位数
  - To calculate: XOR the two codewords and count the 1s in the result / 计算方法：将两个码字异或（XOR），结果中1的个数即为海明距离
  > **中文**：海明距离的计算方法是将两个码字异或，结果中1的个数即为距离。
- **Code rate【码率】**: $m/n$ (ratio of information bits to total bits) / 信息比特数与总比特数的比率

**Additional ECC Definitions / 其他ECC定义**:

- **Minimum Distance of the Code (码的最小距离)**: The smallest Hamming distance between any pair of valid codewords. Determines the error detection/correction capability. / 任意两个有效码字之间的最小海明距离，决定了检错/纠错能力。
  > **中文**：码的最小距离 = 任意两个有效码字之间的最小海明距离，决定了检错和纠错能力。

- **Burst of Errors (突发错误)**: A sequence of consecutive bits in which the first and last bits are erroneous (bits in between may or may not be erroneous). / 从第一个错误比特到最后一个错误比特之间连续的比特序列（中间的比特可能有错也可能无错）。
  > **中文**：突发错误是从第一个错误比特到最后一个错误比特之间连续的比特序列。

- **Burst Error Detecting Capability (突发错误检测能力)**: The largest integer $B$ such that a code can detect all bursts of length $B$ or less. / 一个码能检测的所有长度不超过$B$的突发错误的最大$B$值。
  > **中文**：突发错误检测能力是指码能保证检测的所有长度不超过$B$的突发错误的最大$B$值。

**Binary Symmetric Channel Model (BSC) / 二进制对称信道模型**:

- Models how bit errors occur: a bit is flipped with probability $p_e$ (the Bit Error Rate)
- Transmitted 1 → received 0 with probability $p_e$; Transmitted 0 → received 1 with probability $p_e$
- $p_{1,0} = p_{0,1} = p_e$
- Bit errors are assumed to be independent

> **中文**：BSC模型描述比特错误的发生方式：比特以概率$p_e$（误比特率）被翻转。发送1→收到0的概率为$p_e$，发送0→收到1的概率也为$p_e$。同时假设比特错误之间相互独立。

- **Bit Error Rate (BER) / 误比特率**: The probability that a single bit is flipped during transmission / 单个比特在传输中被翻转的概率
- **Packet Error Rate (PER) / 误包率**: For a packet of $N$ bits, $PER = 1 - (1 - BER)^N$ / 一个$N$比特的数据包出错的概率

> **中文**：对于$N$比特的数据包，误包率$PER = 1 - (1 - BER)^N$。

### 3.3.2 Hamming Distance Rules (Must Memorize / 必须牢记)

To **detect** up to $d$ errors: need Hamming distance $\ge d+1$

> **中文**：要**检测**最多$d$个错误：需要海明距离$\ge d+1$

To **correct** up to $d$ errors: need Hamming distance $\ge 2d+1$

> **中文**：要**纠正**最多$d$个错误：需要海明距离$\ge 2d+1$

### 3.3.3 Parity Check / 奇偶校验

**Simple parity【简单奇偶校验】**: Add 1 parity bit so the total number of 1s is even (even parity) or odd (odd parity).

> **中文**：简单奇偶校验添加1个校验比特，使得整个码字中1的总数为偶数（偶校验）或奇数（奇校验）。

- Calculation: XOR (mod-2 sum) of all data bits gives the parity bit / 计算方法：所有数据比特的异或（模2和）即为校验位
- **Minimum distance** of simple parity code: **2** (flipping any two data bits keeps parity unchanged) / 简单奇偶校验码的最小海明距离为**2**
- Detects **single-bit errors** / 检测**单个比特错误**
- Cannot detect even numbers of bit errors / 无法检测偶数个比特错误
- Cannot correct any errors / 无法纠正任何错误

**Horizontal and Vertical Parity Checks (2-D Parity) / 水平垂直奇偶校验（二维奇偶校验）**:

A two-dimensional (2-D) array is constructed with 1-bit parity check for each **row** (horizontal) and 1-bit check for each **column** (vertical).

> **中文**：二维奇偶校验将数据排列成二维阵列，每行（水平）和每列（垂直）分别计算奇偶校验位。

```
Data bits (organized in rows):
  1 0 1 1  | 1  ← horizontal parity for row 1
  0 1 0 1  | 0  ← horizontal parity for row 2
  1 1 0 0  | 0  ← horizontal parity for row 3
  ---------+--
  0 0 1 0  |    ← vertical parity column
```

- **Advantage / 优点**: Can detect and **correct** single-bit errors (the intersection of the bad row and bad column identifies the erroneous bit) / 可检测并**纠正**单比特错误（错误行和错误列的交点即为出错比特）
- **Detection capability / 检测能力**: Can detect most multi-bit error patterns / 可检测大多数多比特错误模式
- **Minimum distance / 最小海明距离**: Higher than simple parity (depends on array size) / 大于简单奇偶校验

> **中文**：二维奇偶校验的最大优点是能定位并纠正单比特错误——错误行与错误列的交点即为出错比特。这是简单奇偶校验无法做到的。

### 3.3.4 CRC (Cyclic Redundancy Check) ★★★ / 循环冗余校验

CRC is the most widely used error-detecting code【检错码】. It treats bit strings as polynomial coefficients over GF(2) (Galois Field of 2 elements: 0 and 1, addition = XOR, no carries).

> **中文**：CRC是使用最广泛的检错码。它将比特串视为GF(2)（二元伽罗瓦域：元素为0和1，加法为异或，无进位）上的多项式系数。

**The Algorithm** / **算法步骤**：

1. Let the data polynomial be $M(x)$ of degree $k-1$ (k data bits) / 设数据多项式为$M(x)$，次数为$k-1$（共$k$个数据比特）
2. Let the generator polynomial【生成多项式】be $G(x)$ of degree $r$ ($r+1$ bits) / 设生成多项式为$G(x)$，次数为$r$（共$r+1$位）
3. Append $r$ zeros to the data: $M(x) \cdot x^r$ / 在数据后附加$r$个0
4. Divide $M(x) \cdot x^r$ by $G(x)$ using **polynomial division (mod-2, XOR)** / 用**多项式除法（模2运算，异或）**计算$M(x) \cdot x^r$除以$G(x)$
5. The remainder $R(x)$ (degree $< r$) is the **CRC checksum【CRC校验和】** / 余数$R(x)$（次数$< r$）即为**CRC校验和**
6. Transmit: $T(x) = M(x) \cdot x^r + R(x)$ (original data + checksum) / 发送：$T(x) = M(x) \cdot x^r + R(x)$（原始数据 + 校验和）
7. At receiver: divide the received $T(x)$ by $G(x)$. If remainder = 0 → no error. Otherwise → error detected. / 接收端：用$G(x)$除收到的$T(x)$。若余数为0 → 无错误。否则 → 检测到错误。

**Important** / **重要**：The division is polynomial division using XOR (no borrow/carry).

> **中文**：除法是使用异或（XOR）的多项式除法（无借位/进位）。

**Common generator polynomials** / **常用生成多项式**：

| Name / 名称 | $G(x)$ | Binary / 二进制 | $r$ |
|------|--------|--------|-----|
| CRC-8 | $x^8 + x^2 + x + 1$ | 100000111 | 8 |
| CRC-12 | $x^{12} + x^{11} + x^3 + x^2 + x + 1$ | — | 12 |

> **中文**：CRC-8生成多项式为$x^8+x^2+x+1$（8位校验），CRC-12为$x^{12}+x^{11}+x^3+x^2+x+1$（12位校验）。

| Name / 名称 | $G(x)$ | Binary / 二进制 | $r$ |
|------|--------|--------|-----|
| CRC-16 | $x^{16} + x^{15} + x^2 + 1$ | — | 16 |
| CRC-CCITT | $x^{16} + x^{12} + x^5 + 1$ | — | 16 |
| **CRC-32** (Ethernet / 以太网) | $x^{32} + x^{26} + x^{23} + x^{22} + x^{16} + x^{12} + x^{11} + x^{10} + x^8 + x^7 + x^5 + x^4 + x^2 + x + 1$ | — | 32 |

> **中文**：CRC-16和CRC-CCITT均为16位校验，生成多项式不同。CRC-32（用于以太网）为32位校验，是最常用的生成多项式之一。

**Example** / **示例**: Data = `10010101010`, Generator = `10101`. Find CRC / 数据 = `10010101010`，生成多项式 = `10101`，求CRC。

Data = 10010101010 (11 bits), append 4 zeros (since $G(x)$ degree = 4):
Dividend / 被除数 = 10010101010**0000**

> **中文**：数据为10010101010（11位），附加4个0（因为$G(x)$的次数为4）：被除数 = 10010101010**0000**。

Perform XOR-based division / 执行基于异或的除法：
```
100101010100000 ÷ 10101 = ... remainder / 余数 = 1110
```

CRC = `1110`. Transmitted frame / 发送帧: `100101010101110`.

**Verification** / **验证**: The transmitted frame divided by the generator should give remainder 0.

> **中文**：发送帧除以生成多项式应得到余数0，以此验证传输正确性。

**CRC Hardware Implementation** / **CRC硬件实现**: Uses a **linear feedback shift register (LFSR)【线性反馈移位寄存器】** with XOR gates positioned according to $G(x)$ terms. The circuit shown in the PPT processes bits sequentially.

> **中文**：CRC的硬件实现使用线性反馈移位寄存器，其中异或门的位置由生成多项式$G(x)$的各项决定。PPT中展示的电路逐位处理输入比特。

**Real-World CRC Polynomials / 实际应用的CRC生成多项式**:

In addition to the common polynomials listed above, the PPT specifically mentions:

| Name / 名称 | Generator Polynomial / 生成多项式 | Application / 应用 |
|------------|:---:|:---:|
| **CRC-16** | $x^{16} + x^{15} + x^2 + 1$ | General purpose / 通用 |
| **CRC-ITU-T** | $x^{16} + x^{12} + x^5 + 1$ | Telecommunications / 电信 |
| **CRC-32 (IEEE 802)** | $x^{32} + x^{26} + x^{23} + x^{22} + x^{16} + x^{12} + x^{11} + x^{10} + x^8 + x^7 + x^5 + x^4 + x^2 + x + 1$ | Ethernet, USB, etc. / 以太网、USB等 |

> **中文**：CRC-16和CRC-ITU-T都是16位校验，但生成多项式不同，适用于不同场景。CRC-32是32位校验，广泛应用于以太网等协议中。

**Requirements for $G(x)$ / 对$G(x)$的要求**:
- $G(x)$ must have **at least 2 non-zero terms** (typically $x^r$ and 1) / $G(x)$必须**至少有两项非零项**（通常为$x^r$和1）
- $G(x) = x^r + \dots + 1$ (the coefficient of $x^r$ and $x^0$ are both 1) / $x^r$和$x^0$的系数均为1

> **中文**：生成多项式$x^r$项和常数项的系数必须为1，且至少有两项非零，这是一个基本要求。

**Error Detection with CRC (数学原理)**:

Let the received message be $Y(x) = T(x) + E(x)$, where $E(x)$ is the error polynomial:

> **中文**：设接收到的消息为$Y(x) = T(x) + E(x)$，其中$E(x)$为错误多项式。

- If no error: $E(x) = 0$ → Remainder$[Y(x)/G(x)] = 0$
- If error exists ($E(x) \neq 0$):
  - Remainder$[Y(x)/G(x)] \neq 0$ → **Error is detectable** / 错误可检测
  - Remainder$[Y(x)/G(x)] = 0$ → **NOT detectable** (this only happens when $E(x) = G(x) \cdot z(x)$, i.e., the error pattern is itself a multiple of $G(x)$) / 不可检测（仅当错误模式本身是$G(x)$的倍数时）

### CRC Properties (CRC的性质) ★

The effectiveness of CRC depends on the properties of the generator polynomial $G(x)$:

**Property 1: All Single-Bit Errors / 所有单比特错误均可检测**
- Single error: $E(x) = x^j$ for some $j$
- $G(x)$ has at least 2 non-zero terms (e.g., $x^r$ and 1)
- Therefore $x^j$ is **not divisible** by $G(x)$ → remainder $\neq 0$ → **detected**
- **Conclusion**: All single-bit errors can be detected / **所有单比特错误均可检测**

**Property 2: All Double-Bit Errors / 所有双比特错误均可检测**
- Double error: $E(x) = x^i + x^j = x^j(x^{i-j} + 1)$
- If $G(x)$ is carefully chosen (e.g., $G(x) = x^{15} + x^{14} + 1$), then $x^k + 1$ is not divisible by $G(x)$ for any $k$ up to a very large value (e.g., 32768)
- **Conclusion**: All double-bit errors can be detected with properly chosen $G(x)$ / **选择适当的$G(x)$，所有双比特错误均可检测**

**Property 3: All Odd Numbers of Errors / 所有奇数个错误均可检测**
- In most implementations, $G(x) = (x+1) \cdot P(x)$ (i.e., $G(x)$ has $x+1$ as a factor)
- Any error polynomial $E(x)$ with an odd number of non-zero coefficients will **not** be divisible by $x+1$
- Therefore it will not be divisible by $G(x)$ either
- **Conclusion**: All odd-numbered errors can be detected / **所有奇数个错误均可检测**

**Property 4: Burst Errors / 突发错误检测**
- All burst errors of length $\le r$ (where $r$ is the degree of $G(x)$) can be detected
- **Conclusion**: CRC can detect any burst error whose length does not exceed the degree of $G(x)$ / **CRC可检测任何长度不超过$G(x)$次数的突发错误**

> **中文**：CRC的四条关键性质：(1)可检测所有单比特错误——因为$G(x)$至少有两项；(2)选择适当的$G(x)$可检测所有双比特错误；(3)若$G(x)$包含$(x+1)$因子，可检测所有奇数个错误；(4)可检测所有长度不超过$r$的突发错误，其中$r$为$G(x)$的次数。

### 3.3.5 Hamming Code (Error Correction) ★ / 海明码（纠错）

**Goal** / **目标**: Detect AND correct single-bit errors / 检测并纠正单比特错误。

**Design equation** / **设计公式**: For $k$ data bits, we need $r$ parity bits such that / 对于$k$个数据比特，需要$r$个校验比特满足：

$$\boxed{2^r \ge k + r + 1}$$

Or equivalently / 或等价地: $(n+1) \le 2^r$ where $n = k+r$

| $k$ (data bits / 数据位) | $r$ (parity bits needed / 所需校验位) | $n$ (total bits / 总位数) | Code rate / 码率 |
|:---:|:---:|:---:|:---:|
| 4 | 3 | 7 | 4/7 |
| 8 | 4 | 12 | 8/12 |

> **中文**：当k=4和k=8时，分别需要r=3和r=4个海明码校验位，总码长n分别为7和12。

| $k$ (data bits / 数据位) | $r$ (parity bits needed / 所需校验位) | $n$ (total bits / 总位数) | Code rate / 码率 |
|:---:|:---:|:---:|:---:|
| 16 | 5 | 21 | 16/21 |
| 32 | 6 | 38 | 32/38 |

> **中文**：当k=16和k=32时，分别需要r=5和r=6个校验位，总码长n分别为21和38。校验位数量随数据位数增长而缓慢增加。例如从k=4到k=32，数据位增加了8倍，但校验位仅从3增至6。

**Why this formula?** — Proof sketch / **为什么是这个公式？** — 推导思路：

- There are $2^n$ possible $n$-bit patterns / 共有$2^n$种可能的$n$比特模式
- There are $2^k$ valid codewords / 共有$2^k$个有效码字
- Each valid codeword has $n$ neighbors at Hamming distance 1 (each neighbor is an illegal codeword from a single-bit error) / 每个有效码字有$n$个海明距离为1的相邻码字（每个相邻码字对应一个单比特错误）
- So each valid codeword needs $1 + n$ patterns (itself + $n$ one-bit errors) / 因此每个有效码字需要$1 + n$种模式（自身 + $n$个单比特错误）
- Total / 总计: $(n+1) \times 2^k \le 2^n$ → $(n+1) \le 2^{n-k} = 2^r$ → $(k+r+1) \le 2^r$

> **中文**：海明码设计公式的推导思路：总共有$2^n$种可能的比特模式，其中$2^k$个是有效码字。每个有效码字有$n$个海明距离为1的相邻非法码字（即该码字发生单比特错误后的结果），所以每个有效码字需要占用$1+n$种比特模式（自身加上$n$种单比特错误模式）。由此得到不等式$(n+1) \times 2^k \le 2^n$，化简即得$2^r \ge k + r + 1$。

**Hamming Code Limitations / 海明码的局限性**:

- The minimum Hamming distance of the **(7,4) Hamming code is 3** / (7,4)海明码的最小海明距离为**3**
- **1-bit error**: Can **detect AND correct** (distance 3 ≥ $2 \times 1 + 1$) / **能检测并纠正**（距离3 ≥ $2 \times 1 + 1$）
- **2-bit error**: Can **detect but NOT correct** (distance 3 ≥ $2 + 1$ for detection but $3 < 2 \times 2 + 1$ for correction). The decoder will misinterpret the result and attempt to "correct" to a wrong codeword! / **能检测但不能纠正**（距离3 ≥ $2 + 1$可检测，但$3 < 2 \times 2 + 1$不可纠正）。解码器会误判并尝试"纠正"到一个错误的码字！

> **中文**：(7,4)海明码的关键特性：最小海明距离为3，可检测并纠正1比特错误；可检测2比特错误但不能纠正（错误纠正后可能得到错误的码字）。

**Cost-Efficiency Comparison / 成本效率对比**:

| Code / 码 | Data bits / 数据位 | Check bits / 校验位 | Code Rate / 码率 | Capability / 能力 |
|----------|:---:|:---:|:---:|------|
| Single Parity | 7 | 1 | 7/8 (87.5%) | Detect odd # errors / 检测奇数个错误 |
| (7,4) Hamming | 4 | 3 | **4/7 (57.1%)** | Correct 1-bit, detect 2-bit / 纠正1位，检测2位 |

> **中文**：海明码的纠错能力是以码率为代价的。与简单奇偶校验相比，(7,4)海明码的码率从87.5%降至57.1%，但获得了单比特纠错能力。

---

## 3.4 Elementary Data Link Protocols / 基本数据链路协议

### 3.4.1 Stop-and-Wait Protocol / 停-等协议

The simplest protocol with flow control / 最简单的带流量控制的协议：

1. Sender sends one frame / 发送端发送一帧
2. Sender **waits** for ACK【确认帧】 / 发送端**等待**ACK
3. Receiver sends ACK after receiving frame correctly / 接收端正确收到帧后发送ACK
4. Sender sends next frame only after receiving ACK / 发送端只有在收到ACK后才发送下一帧

**Sequence number** / **序号**: 1 bit (0 or 1) to distinguish new frame from retransmission / 使用1比特（0或1）区分新帧和重传帧。

**The Three Armies Problem (三军问题)** - Why ACKs and timers are necessary:

> **中文**：三军问题说明了一个根本性困境：发送方需要ACK来确认数据已到达，但ACK本身也可能丢失。这需要一个隐式的默认动作——超时重传。

- Error can happen to the **data** (sender → receiver) / 错误可能发生在**数据**上（发送方→接收方）
- Error can also happen to the **ACK** (receiver → sender) / 错误也可能发生在**ACK**上（接收方→发送方）
- **Solution**: If no ACK is received within a timeout period, assume the data was not delivered and **retransmit** / **解决方案**：若在超时时间内未收到ACK，假设数据未送达并**重传**
- This is why we need both **timers** and **sequence numbers**: timers trigger retransmission; sequence numbers prevent duplicate delivery / 这就是为什么需要**定时器**和**序号**：定时器触发重传；序号防止重复交付

**Sequence Number (SN) and Request Number (RN) Detailed Algorithm / SN和RN详细算法**:

The PPT presents a formal algorithm with two variables:
- **SN (Sequence Number)**: The sequence number of the next frame the sender will transmit / 发送方将要发送的下一帧的序号
- **RN (Request Number)**: The sequence number that the receiver expects to receive next (equivalent to the ACK number) / 接收方期望下一个接收到的序号（等同于ACK序号）

**Sender (Node A) Algorithm / 发送方（节点A）算法**:

```
1. Set SN = 0
2. Accept a packet from the higher layer; assign number SN
3. Transmit the SN-th frame (containing SN in the sequence number field)
4. Start a timer with timeout value T
5. If an error-free frame is received from B containing RN > SN:
       Update SN = RN, go to step 2
   Else if timeout occurs:
       Go to step 3 (retransmit)
```

**Receiver (Node B) Algorithm / 接收方（节点B）算法**:

```
1. Set RN = 0
2. Whenever an error-free frame is received from A with SN == RN:
       Deliver the packet to the higher layer
       Increment RN (RN = RN + 1)
3. Transmit a frame to A containing RN in the request number field
   (this RN serves as the acknowledgment for all frames up to RN-1)
```

> **中文**：上述算法是SAW的形式化描述。SN是发送序号，RN是请求序号（即期望收到的下一帧序号）。关键点：发送方维护定时器、SN；接收方维护RN。收到SN=RN的帧时，接收方接受并递增RN，然后在回复帧中携带RN作为确认。

### 3.4.2 Stop-and-Wait ARQ (Automatic Repeat reQuest) / 停-等自动重发请求

Adds error handling to stop-and-wait / 在停-等协议基础上增加了差错处理：

**Key Assumptions in ARQ Error Control / ARQ差错控制的关键假设**:

> **中文**：ARQ协议基于以下关键假设：

1. **Error detection is reliable**: The receiver can **always** distinguish erroneous frames from error-free frames (using CRC) / 接收方**总能够**区分错误帧和无错误帧（使用CRC）
2. **Frame states**: A frame can be in one of three states: **error-free**, **erroneous**, or **lost** (never arrives) / 帧可能处于三种状态之一：**无错误**、**有错误**或**丢失**（从未到达）
3. **ACK/NAK also protected**: ACK and NAK frames are also protected with CRC, because errors can happen to them too / ACK和NAK帧也受CRC保护，因为错误也可能发生在它们身上
4. **Arbitrary delay**: Each frame is subject to an arbitrary delay / 每帧可能经历任意延迟

```
Frame arrives at DLC of receiver:
  ┌─ Is it received? ──No──→ Lost (need retransmission)
  └─ Yes
     └─ Is it error-free? ──No──→ Erroneous (need retransmission)
        └─ Yes → Error-free (deliver to upper layer)
```

> **中文**：差错控制的前提条件：差错检测可靠（CRC保证）、ACK/NAK同样受CRC保护。帧到达接收方DLC层时，首先判断是否收到，再判断是否有错误，只有无错误的帧才能被交付给上层。

- **Timer【定时器】** : After sending a frame, sender starts a timer / 发送帧后，发送端启动定时器
- **Timeout【超时】** : If timer expires before ACK arrives ⇒ assumed loss ⇒ **retransmit** / 若定时器在ACK到达前超时 ⇒ 认为帧丢失 ⇒ **重传**
- **Sequence number【序号】** : Distinguish between original and retransmitted frame (preventing duplicates) / 区分原始帧和重传帧（防止重复接收）

**Channel utilization for Stop-and-Wait** / **停-等协议的信道利用率**：

$$\boxed{U = \frac{T_D}{T_D + RTT + T_A}} \quad \text{(信道利用率公式)}$$

Where $T_D$ = data frame transmission time【数据帧发送时间】, $RTT$ = round-trip time【往返时间】, $T_A$ = ACK transmission time【ACK发送时间】.

> **中文**：公式中$T_D$为数据帧的发送时间，$RTT$为往返时间，$T_A$为ACK帧的发送时间。分子$T_D$是实际用于传输数据的时间，分母$T_D + RTT + T_A$是发送一帧所需的全部时间。利用率$U$即为二者的比值。

**Example (Satellite link)** / **示例（卫星链路）**: 50 kbps link, 1000-bit frame, RTT = 500 ms / 50 kbps链路，1000比特帧，RTT = 500 ms

- $T_D = 1000/50000 = 0.02$ s = 20 ms
- $U = \frac{20}{20+500} \approx 3.8\%$ — **Very poor utilization!** / **利用率非常低！**

This is why we need **sliding window protocols** — to keep the pipe full.

> **中文**：这就是为什么我们需要**滑动窗口协议**——让管道始终保持满载，避免等待ACK时的空闲浪费。

**SAW Efficiency with Errors / 有错误时的SAW效率**:

When frame errors occur, the efficiency decreases further. Let $p$ = probability of successful transmission (error-free):

$$\boxed{U = \frac{L/R}{S + L/R + T \cdot (1-p)/p}}$$

Where $L$ = frame size, $R$ = data rate, $S$ = RTT, $T$ = timeout value (typically $T \ge S$).

> **中文**：当考虑帧错误时，SAW的效率公式变为$U = \frac{L/R}{S + L/R + T \cdot (1-p)/p}$。其中$p$是帧传输成功的概率。错误率越高，效率越低，因为每次重传都需要等待超时时间$T$。

**Improvement: Pipelining (流水线) / 改进：流水线**:

The key insight: instead of sending one frame and waiting, let the sender send **multiple frames** before waiting for ACKs.

> **中文**：流水线的核心思路：发送方连续发送多个帧，而不是发送一帧后等待ACK。

**Efficiency comparison** / **效率对比**:

- Without pipelining (SAW): $U = \frac{L/R}{S + L/R}$ — large gaps of idle time
- With pipelining (3 frames): $U = \frac{3L/R}{S + 3L/R}$ — much better utilization

$$\boxed{U_{pipelined} = \frac{W \cdot L/R}{S + W \cdot L/R}}$$

Where $W$ = number of outstanding frames (window size).

> **中文**：流水线的效率公式为$U = \frac{W \cdot L/R}{S + W \cdot L/R}$，其中$W$为窗口大小（未确认帧数）。当$W \cdot L/R \ge S$时，效率可达100%。这就是**带宽-时延积**（Bandwidth-Delay Product）的概念——管道容量决定了需要多大的窗口才能充分利用信道。

---

## 3.5 Sliding Window Protocols ★★★ (Core Big Question Material / 核心大题内容)

### 3.5.1 Core Concepts / 核心概念

**Why sliding window?** / **为什么需要滑动窗口？** To improve channel utilization by allowing multiple outstanding unacknowledged frames ("keeping the pipe full").

> **中文**：滑动窗口协议允许发送端连续发送多个未被确认的帧，充分利用信道，避免停-等协议中的空闲等待，从而显著提高信道利用率（即"让管道保持满载"）。

**Window** / **窗口**: The set of sequence numbers the sender is permitted to send (or receiver is permitted to accept).

> **中文**：窗口是发送端允许发送（或接收端允许接收）的序号集合。

| Window / 窗口 | Maintained By / 维护者 | Controls / 控制内容 | Maximum Size / 最大尺寸 |
|--------|:---:|------|:---:|
| **Sending Window ($W_T$)** / **发送窗口** | Sender / 发送端 | Number of outstanding unacknowledged frames / 未确认的待发帧数 | Protocol-dependent / 取决于协议 |
| **Receiving Window ($W_R$)** / **接收窗口** | Receiver / 接收端 | Which frames to accept / 接收哪些帧 | Protocol-dependent, fixed size / 取决于协议且固定大小 |

**How windows move** / **窗口如何移动**:

- Sender's **lower edge** advances when an ACK arrives / 发送窗口的**下界**在收到ACK时向前移动
- Sender's **upper edge** advances when it sends a new frame / 发送窗口的**上界**在发送新帧时向前移动
- When (upper - lower) = max window size → stop sending / 当（上界 - 下界）= 最大窗口大小时 → 停止发送
- Receiver's window advances when a frame is delivered to the network layer / 接收窗口在帧被交付给网络层时向前移动

**Key principle** / **关键原则**: $\text{发送流量} \le \text{接收流量}$ (sender's rate ≤ receiver's processing rate / 发送速率 ≤ 接收端处理速率)

### 3.5.2 Piggybacking (捎带确认)

In full-duplex communication, acknowledgments are **piggybacked** onto data frames traveling the opposite direction. Instead of sending a separate ACK frame, the receiver sets the `ACK` field in its next outgoing data frame.

> **中文**：在全双工通信中，确认信息被**捎带**到反方向传输的数据帧上。接收端不单独发送ACK帧，而是在它下一次发送数据帧时，设置该帧中的`ACK`字段来携带确认信息。

**Efficiency** / **效率**: Saves the overhead of separate ACK frames. Without piggybacking, you'd need separate ACKs which consume bandwidth.

**Sliding Window Operation Example / 滑动窗口操作示例**:

> **中文**：以下示例展示了滑动窗口的基本操作流程（假设无错误、无丢失）：

1. Sender sends P0, P1, P2 (frames with seq. # 0, 1, 2) / 发送方发送P0、P1、P2
2. Sender sends P3, P4, P5, P6 (more frames as window allows) / 发送方继续发送P3、P4、P5、P6
3. Receiver sends **ACK 3** (meaning: ready to receive frame 3, so frames 0, 1, 2 are confirmed) / 接收方发送**ACK 3**（确认帧0、1、2已收到）
4. Receiver sends **ACK 4** (confirming frame 3) / 接收方发送**ACK 4**（确认帧3已收到）

> **中文**：注意ACK-n表示接收方准备接收帧n（即之前的n-1帧已正确接收）。这是累积确认——ACK 3确认了帧0、1、2全部成功接收。

**Implementation Note / 实现说明**:
In reality, the sliding window only contains **sequence numbers** (or addresses to where the actual frame data is stored). The implementation does not need to handle frames of different lengths in the window itself.

> **中文**：在实际实现中，窗口仅包含**序号**（或指向实际帧数据存储位置的地址），实现本身不需要在窗口中处理不同长度的帧。

> **中文**：捎带确认节省了单独发送ACK帧的开销。如果没有捎带确认，就需要发送独立的ACK帧，这会消耗带宽资源。

### 3.5.3 Protocol Comparison Table (Must Memorize / 必须牢记)

| Protocol / 协议 | $W_T$ (Sender Window / 发送窗口) | $W_R$ (Receiver Window / 接收窗口) | Sequence Number Bits / 序号位数 |
|----------|:---:|:---:|:---:|
| **Stop-and-Wait** / **停-等** | $1$ | $1$ | 1 bit |
| **Go-Back-N (GBN)** / **后退N帧** | $1 < W_T \le 2^n-1$ | $= 1$ | $n$ bits |
| **Selective Repeat (SR)** / **选择重发** | $1 < W_T \le 2^{n-1}$ | $1 < W_R \le W_T \le 2^{n-1}$ | $n$ bits |

**Why $W_T \le 2^n - 1$ for GBN?** / **为什么GBN中$W_T \le 2^n - 1$？** To avoid ambiguity between a new frame and a retransmitted frame. If window size were $2^n$, all sequence numbers would be in use, and the receiver couldn't distinguish between a retransmission of frame 0 and a new instance of frame 0.

> **中文**：为了避免新帧和重传帧之间的歧义。如果窗口大小为$2^n$，所有序号都会被使用，接收端无法区分重传的帧0和新的帧0。因此发送窗口最大为$2^n - 1$，确保总有一个序号未被使用，从而消除歧义。

**Why $W_T \le 2^{n-1}$ for SR?** / **为什么SR中$W_T \le 2^{n-1}$？** For the same reason but with independent sending and receiving windows — both can slide independently, so the available sequence space must be double-buffered effectively.

> **中文**：原因相同但更严格，因为发送窗口和接收窗口可以独立滑动，可用序号空间需要有效加倍。发送窗口最大为$2^{n-1}$，接收窗口也最多为$2^{n-1}$，二者之和不超过$2^n$，从而避免歧义。

---

## 3.6 Go-Back-N ARQ ★★★ / 后退N帧自动重发请求

### 3.6.1 Sender Behavior / 发送端行为

The sender maintains a window of up to $2^n-1$ outstanding frames.

> **中文**：发送端维护一个最多包含$2^n-1$个未确认帧的窗口。

**Key parameters / 关键参数**:
- **N**: The window size $W$ — how many successive frames can be sent without ACK / 窗口大小$W$——可以连续发送而不需ACK的帧数
- **Timeout value**: Must be at least $T \ge W \cdot L/R$ (the time to transmit all frames in the window) / 超时值必须至少$T \ge W \cdot L/R$（传输窗口内所有帧所需时间）

**Rules** / **规则**：

1. When a frame arrives from the network layer: if window not full, send it (with current sequence number), upper edge advances / 当网络层送来一个帧：如果窗口未满，用当前序号发送该帧，上界前移
2. When an ACK arrives: **cumulative** acknowledgment【累积确认】 — all frames up to and including the ACK number are acknowledged. Lower edge advances. / 当收到ACK：**累积确认**——序号等于或小于ACK序号的所有帧都被确认，下界前移
3. When **timer expires** (for oldest unacknowledged frame): **go back N** — retransmit ALL unacknowledged frames starting from the timed-out one / 当**定时器超时**（针对最老的未确认帧）：**后退N帧**——从超时的帧开始重传所有未确认的帧

### 3.6.2 Receiver Behavior / 接收端行为

The receiver window size is **1** (only accepts frames in order).

> **中文**：接收窗口大小为**1**（只按顺序接收帧）。

1. When a frame arrives: check if its sequence number matches the **expected** sequence number / 当帧到达时：检查其序号是否与**期望**序号匹配
2. If **matches** → accept, deliver to network layer, send ACK(number+1), advance expected number / 若**匹配** → 接收，交付给网络层，发送ACK(序号+1)，期望序号前移
3. If **doesn't match** → **discard** frame, send ACK(last correct frame number) / 若**不匹配** → **丢弃**该帧，发送ACK(最后一个正确帧的序号)
4. All correctly received but out-of-order frames are **silently discarded** / 所有正确接收但乱序的帧都被**静默丢弃**

**Key** / **关键**: The receiver is simple — it just expects one sequence number. Everything else is rejected.

> **中文**：接收端逻辑简单——它只期望一个特定序号，所有其他帧都被拒绝。

### 3.6.3 Efficiency Analysis / 效率分析

**Basic Parameters / 基本参数**:
- $\tau = L/R$: Frame transmission delay (time to send one frame) / 帧发送时延
- $S$: RTT (round-trip time) / 往返时间
- $W$: Window size / 窗口大小

**Case 1: No errors, no delay / 无错误、无延迟**:
- Sender transmits P0, P1, ..., P(W-1) and waits for the first ACK
- If $W \cdot \tau \ge S$ (window fills the pipe) → $U = 1.0$ (100% utilization)
- Otherwise: $U = \frac{W \cdot \tau}{S + \tau}$ (the +$\tau$ accounts for the last frame being partially transmitted when ACK arrives)

> **中文**：无错误时，若$W \cdot \tau \ge S$（窗口足够大以填满管道），则利用率为100%。否则$U = W \cdot \tau / (S + \tau)$。

**Case 2: With errors / 有错误时**:
- Errors reduce efficiency because they trigger retransmissions
- Let $W_{opt} = \lceil S/\tau \rceil$ be the minimum window size needed for 100% utilization / $W_{opt}$是实现100%利用率所需的最小窗口大小
- Transmission timeout $T = S = W \cdot \tau$ (typical setting / 典型设置)

> **中文**：有错误时，效率会下降。$W_{opt} = \lceil S/\tau \rceil$是实现100%利用率所需的最小窗口。$T = S$是典型的超时设置。

Let $W$ = window size【窗口大小】, $T_f$ = frame transmission time【帧发送时间】, $T_p$ = one-way propagation delay【单向传播时延】.

Let $W$ = window size【窗口大小】, $T_f$ = frame transmission time【帧发送时间】, $T_p$ = one-way propagation delay【单向传播时延】.

- If $W \times T_f \ge T_f + 2T_p$ (window large enough to keep pipe full / 窗口足够大，能保持管道满载):
  $$U = 1.0 \quad \text{(100% utilization / 100%利用率)}$$

- Otherwise / 否则:
  $$U = \frac{W \times T_f}{T_f + 2T_p} \quad \text{(窗口不够大时的信道利用率)}$$

### 3.6.4 GBN Detailed Error Handling Cases / GBN详细差错处理情况

> **中文**：PPT中详细描述了三种差错情况的处理流程，以下逐一说明。ACK-n表示准备接收帧n（即之前的n-1帧已正确接收）。NAK-n表示帧n有错（但之前的n-1帧已正确接收）。

**Case 1: Damaged Frame (帧损坏)**:
- A transmits frame $i$. B detects an error, with the previous $i-1$ frames successfully received / A发送帧$i$，B检测到错误，之前$i-1$帧已成功接收
- B must send **NAK $i$ immediately** / B必须**立即发送NAK $i$**
- When A receives NAK $i$, it must retransmit frame $i$ and **all subsequent frames** it has transmitted / A收到NAK $i$后，必须重传帧$i$及**之后所有已发送的帧**

> **中文**：帧损坏时，接收方立即发送NAK，发送方收到NAK后重传该帧及后续所有帧。

**Case 2: Lost Frame or Lost ACK (帧丢失或ACK丢失)**:
- **2a. Frame $i$ is lost; B has received subsequent frame $i+1$**: B treats it as damaged frame $i$, sends NAK $i$ / 帧$i$丢失，B收到了后续帧$i+1$：B将其视为帧$i$损坏，发送NAK $i$
- **2b. Frame $i$ is lost; A hasn't sent additional frames**: B may return either ACK $i$ or NAK $i$. Timer at A for frame $i$ will eventually expire / 帧$i$丢失，A未发送额外帧：B可能返回ACK $i$或NAK $i$。A的定时器最终会超时
- **2c. B receives frame $i$ and sends ACK $i+1$, which is lost**: A receives an ACK for **subsequent** frames, so timer for frame $i$ will not expire, everything is fine / B收到帧$i$并发送ACK $i+1$但丢失：A收到**后续帧**的ACK，因此帧$i$的定时器不会超时，一切正常
- **2d. Timer for frame $i$ expires**: A retransmits frame $i$ and subsequent frames / 帧$i$的定时器超时：A重传帧$i$及后续帧

> **中文**：注意Case 2c的情况——若后续帧的ACK成功到达，即使某个ACK丢失也不会触发重传，因为GBN使用累积确认。

**Case 3: Lost NAK (NAK丢失)**:
- If a NAK $i$ is lost, A will time out on associated (later) frames, and retransmit frame $i$ and all subsequent frames / 若NAK $i$丢失，A会在相关（后续）帧上超时，然后重传帧$i$及所有后续帧

> **中文**：NAK丢失不会造成严重问题，因为发送方的定时器最终会超时并触发重传。NAK只是性能优化。

### 3.6.5 GBN Window Size Discussion / GBN窗口大小讨论

Why must the window size $W \le 2^k - 1$ for $k$-bit sequence numbers?

> **中文**：为什么$k$比特序号的GBN窗口大小必须$W \le 2^k - 1$？

**Ambiguity Example / 歧义示例**: Consider $k=3$ bits ($2^k = 8$ sequence numbers: 0-7) with $W = 8$:

1. A transmits P0, gets ACK1, then transmits P1, P2, P3, P4, P5, P6, P7, P0 (8 frames)
2. Now A receives another **ACK1** — but which frame does it confirm?
   - **Possibility 1**: All 8 frames were successfully received, and B is requesting frame 1 / 全部8帧成功接收，B请求帧1
   - **Possibility 2**: All 8 frames were lost/erroneous, and B is **repeating** the previous ACK1 / 全部8帧丢失/错误，B在**重复**之前的ACK1
3. **Sender A is confused!** / **发送方A混淆了！**

**Solution / 解决方案**: Limit $W \le 2^k - 1$. With $k=3$, $W_{max} = 7$. This ensures at least one sequence number is always unused, removing the ambiguity.

> **中文**：窗口大小限制为$W \le 2^k - 1$确保总有一个序号未被使用，从而消除歧义。例如$k=3$时，最大窗口为7（序号0-6），序号7始终空闲。

**Duplicate ACKs / 重复ACK**: Some protocols use duplicate ACKs as a "NAK-free" way to signal retransmission (e.g., TCP uses three duplicate ACKs).

> **中文**：有些协议使用重复ACK作为"无NAK"的重传信号（例如TCP使用3个重复ACK触发快速重传）。

---

## 3.7 Selective Repeat ARQ ★★★ / 选择重发自动重发请求

### 3.7.1 Sender Behavior / 发送端行为

Similar to GBN, but each frame has its **own timer**.

> **中文**：与GBN类似，但每个帧有**自己的定时器**。

**Rules** / **规则**：

1. Send frames as in GBN / 像GBN一样发送帧
2. Each frame gets its own timer on transmission / 每个帧在发送时启动自己的定时器
3. When a specific frame's timer expires → retransmit **only that frame** / 当某个特定帧的定时器超时时 → 只重传**该帧**
4. Accept **NAK** (Negative Acknowledgment)【否定确认】 — retransmit only the NAK'd frame / 接受**NAK**——只重传被NAK的帧

### 3.7.2 Receiver Behavior / 接收端行为

**Receiver window > 1** — accepts frames out of order / **接收窗口 > 1** —— 可以接收乱序帧。

1. When a frame arrives / 当帧到达时：
   - If within receiving window → **buffer** it (even if out of order), send ACK for that frame / 如果在接收窗口内 → **缓存**起来（即使乱序），发送该帧的ACK
   - If outside window → discard / 如果在窗口外 → 丢弃
2. When **all frames** in the current window have been received → deliver the entire window to network layer, slide window forward / 当当前窗口中的**所有帧**都已收到 → 将整个窗口交付给网络层，窗口向前滑动
3. If a missing frame is detected (higher-numbered frames have arrived but a lower one hasn't) → optionally send **NAK** for the missing frame / 如果检测到缺失帧（高序号帧已到但低序号帧未到）→ 可选择发送**NAK**请求缺失帧

**Key distinction from GBN** / **与GBN的关键区别**: The receiver buffers out-of-order frames. Only missing frames are retransmitted (not the entire window).

> **中文**：接收端缓存乱序帧。只有缺失的帧才被重传（而不是重传整个窗口）。

### 3.7.3 NAK in Selective Repeat / SR中的否定确认

**NAK (Negative Acknowledgment)** tells the sender: "I'm missing frame X, please retransmit it."

> **中文**：NAK（否定确认）告诉发送端："我缺失了帧X，请重传它。"

**Why use NAK?** / **为什么使用NAK？** Without NAK, the receiver must wait for the sender's timer to expire. With NAK, the receiver can proactively request retransmission as soon as it detects a gap, improving efficiency.

> **中文**：没有NAK时，接收端必须等待发送端的定时器超时才能触发重传。有了NAK，接收端一旦检测到序号空缺就可以主动请求重传，提高了效率。

**What if NAK is lost?** / **如果NAK丢失了怎么办？** No critical damage — the sender's timer will eventually expire and trigger retransmission anyway. NAK is a performance optimization, not a correctness requirement.

> **中文**：不会造成严重问题——发送端的定时器最终会超时并触发重传。NAK是性能优化手段，而非正确性要求。

### 3.7.4 Disadvantages of Selective Repeat / 选择重发的缺点

Despite its efficiency advantages, SR has several drawbacks:

> **中文**：尽管SR在效率上有优势，但它也有若干缺点：

1. **More complicated protocol / 协议更复杂**: Requires individual timers for each frame and more complex buffer management / 需要为每个帧维护单独的定时器和更复杂的缓存管理
2. **Each frame must be individually ACKed / 每帧需单独确认**: Cannot use cumulative ACKs as efficiently as GBN / 不能像GBN那样高效使用累积确认
3. **Needs buffer for out-of-order frames / 需要为乱序帧提供缓存**: Receiver must buffer potentially many frames until the missing ones arrive / 接收方必须缓存可能很多的帧，直到缺失帧到达

**Efficiency note / 效率说明**: Without errors, SR has the same efficiency as GBN ($U = \frac{W \cdot T_f}{T_f + 2T_p}$). With errors, SR outperforms GBN by avoiding unnecessary retransmissions of correctly received frames.

> **中文**：无错误时，SR与GBN效率相同。有错误时，SR避免了GBN中不必要的重传，因此效率更高。

### 3.7.5 Timing Diagram Conventions / 时序图约定

Timing diagrams are frequently used in exams to test understanding of ARQ protocols.

> **中文**：时序图在考试中经常出现，用于测试对ARQ协议的理解。

**Key elements / 关键元素**:
- **Horizontal lines**: Represent the sender and receiver over time (time flows downward) / 水平线表示发送方和接收方（时间向下流动）
- **Arrow from sender to receiver**: Frame transmission / 从发送方到接收方的箭头：帧传输
- **Arrow from receiver to sender**: ACK/NAK transmission / 从接收方到发送方的箭头：ACK/NAK传输
- **Labels on frames**: Usually include the **frame type** and **sequence number** (e.g., P0, ACK1, NAK3) / 帧上的标签通常包含**帧类型**和**序号**（如P0、ACK1、NAK3）
- **Delayed/lost frames**: Often shown with a cross (X) or dashed lines / 延迟/丢失的帧通常用叉号（X）或虚线表示

**Common exam task / 常见考试题型**: Fill in the blanks on a timing diagram with:
- The correct **frame numbers** (P#) / 正确的**帧号**（P#）
- The correct **ACK numbers** (ACK #) / 正确的**ACK号**（ACK #）
- The correct **NAK numbers** (NAK #) / 正确的**NAK号**（NAK #）
- The receiver's **actions** (A = Accept, DE = Discard as Error, DD = Discard as Duplicate) / 接收方的**动作**（A = 接受，DE = 作为错误丢弃，DD = 作为重复丢弃）

> **中文**：在时序图中填写空白的常见题型包括：填写正确的帧号、ACK号、NAK号以及接收方的动作（接受A、错误丢弃DE、重复丢弃DD）。

### 3.7.6 GBN vs SR: Decision Guide / GBN与SR对比选择指南

| Scenario / 场景 | Better Choice / 更优选择 |
|----------|:---:|
| High error rate / 高差错率 | **SR** (less retransmission overhead / 更少的重传开销) |
| Low bandwidth, many errors / 低带宽、多差错 | **SR** (avoid wasting bandwidth on unnecessary retransmissions / 避免在无谓重传上浪费带宽) |
| Simple receiver implementation / 简单接收端实现 | **GBN** (receiver only needs buffer size 1 / 接收端只需1的缓冲区) |
| Ordered delivery required / 要求按序交付 | **GBN** (enforces order naturally / 天然保证顺序) |
| Long propagation + high bandwidth / 长传播时延 + 高带宽 | **SR** (more efficient use of the pipe / 更有效地利用管道) |

---

## 3.8 Frame Sequence Number Field Design ★★★ (Classic Big Question / 经典大题)

This is the calculation that ties everything together.

> **中文**：这是将所有概念联系起来的核心计算题类型。

**Problem type** / **问题类型**: Given bandwidth, propagation delay, and frame size, determine the minimum number of bits needed for the sequence number field to achieve maximum throughput using a sliding window protocol.

> **中文**：给定带宽、传播时延和帧大小，确定在使用滑动窗口协议实现最大吞吐量时，序号字段所需的最小比特数。

**Step-by-step solution template** / **分步解题模板**：

**Step 1** / **第一步**: Calculate the number of frames that can fill the pipe / 计算可填满管道的帧数。

$$\text{Number of frames in pipe} = \frac{\text{Bandwidth} \times \text{RTT}}{\text{Frame size}} + 1 \quad \text{(管道中的帧数)}$$

Or more precisely / 或更精确地：

$$\text{Frames in one RTT} = \frac{\text{RTT}}{T_f} \quad \text{(一个RTT内的帧数)}$$

Total frames that can be outstanding = $1 + \text{Frames in one RTT}$ (the "+1" is the frame currently being transmitted when the first ACK arrives)

> **中文**：可同时未确认的总帧数 = $1 + \text{一个RTT内的帧数}$（"+1"是第一个ACK到达时正在发送的帧）

**Step 2** / **第二步**: Determine the window size needed / 确定所需窗口大小。

$$\text{Window size needed} = \lceil 1 + \frac{RTT}{T_f} \rceil \quad \text{(所需窗口大小，向上取整)}$$

(round up because window must be an integer / 向上取整因为窗口必须是整数)

**Step 3** / **第三步**: Determine sequence number bits based on protocol / 根据协议确定序号位数。

- For **Go-Back-N** / **后退N帧**: $2^n - 1 \ge W_{needed}$ → $n = \lceil \log_2(W_{needed} + 1) \rceil$
- For **Selective Repeat** / **选择重发**: $2^{n-1} \ge W_{needed}$ → $n = \lceil \log_2(W_{needed}) + 1 \rceil$

**Example 1** / **示例1**: 1.6 Mbps link, RTT = 45 ms, frame size = 1 KB. Find sequence number bits for maximum throughput. / 1.6 Mbps链路，RTT = 45 ms，帧大小 = 1 KB。求最大吞吐量所需的序号位数。

- $T_f = \frac{1 \text{ KB} \times 8}{1.6 \text{ Mbps}} = \frac{8192}{1.6 \times 10^6} = 5.12 \times 10^{-3} \text{ s} = 5.12 \text{ ms}$
  
  > **中文**：帧发送时间 = 帧大小（比特）/ 带宽 = 8192 / 1.6×10^6 = 5.12 ms

- Frames in RTT = 45 / 5.12 ≈ 8.79 → 8 frames (rounded down, since partial frame can't be sent) / 一个RTT内可发送的帧数 = 45 / 5.12 ≈ 8.79 → 8帧（向下取整，因为不能发送部分帧）
- Total outstanding / 总未确认帧数 = 1 + 8 = 9 frames

- For SR / 对于SR协议: need $2^{n-1} \ge 9$, so $n \ge \lceil \log_2 9 \rceil + 1 = 4 + 1 = 5$ bits

Wait, let me reconsider / 等等，重新考虑：$W \le 2^{n-1}$ for SR.
So $2^{n-1} \ge 9$, meaning $n-1 \ge \log_2 9 \approx 3.17$, so $n \ge 4.17 \rightarrow n = 5$ bits

> **中文**：对于SR，$2^{n-1} \ge 9$，即$n-1 \ge \log_2 9 \approx 3.17$，所以$n \ge 4.17 \rightarrow n = 5$比特。

Actually, for the GBN case / 对于GBN的情况: need $W \le 2^n - 1$, so $2^n \ge 10$, $n \ge \lceil \log_2 10 \rceil = 4$ bits.

> **中文**：对于GBN，需要$W \le 2^n - 1$，所以$2^n \ge 10$，$n \ge \lceil \log_2 10 \rceil = 4$比特。

**Example 2** / **示例2**: 4 Mbps point-to-point, 5000 km distance, signal speed = $3 \times 10^8$ m/s, frame = 1 KB. Find minimum sequence bits for SR. / 4 Mbps点对点链路，距离5000 km，信号速度 = $3 \times 10^8$ m/s，帧 = 1 KB。求SR所需的最小序号位数。

- $T_{prop} = \frac{5000 \times 10^3}{3 \times 10^8} = \frac{1}{60} \text{ s} \approx 16.67 \text{ ms}$
  
  > **中文**：传播时延 = 距离 / 信号速度 = 5000×10^3 / 3×10^8 ≈ 16.67 ms

- $RTT = 2 \times T_{prop} = \frac{1}{30} \text{ s} \approx 33.33 \text{ ms}$

- Bandwidth-delay product / 带宽-时延积: $4 \times 10^6 \text{ bps} \times \frac{1}{30} \text{ s} = \frac{4 \times 10^6}{30} \approx 133.3 \text{ kbits}$

Wait, let me use the approach from the senior notes / 使用学长笔记中的方法：

- $RTT = \frac{10^7}{3 \times 10^8} = \frac{100}{3} \text{ ms}$（往返距离 = 2 × 5000 km = 10^7 m）

- Bandwidth × RTT = $\frac{100}{3} \times 10^{-3} \times 4 \times 10^6 = \frac{400}{3} \times 10^3 \text{ bits}$
  Actually / 实际上: $\frac{100}{3} \times 10^{-3} \times 0.5 \times 10^6 \text{ bytes/s} = \frac{50}{3} \times 10^3 \text{ bytes} = \frac{50}{3} \text{ KB}$
- Total frames in pipe / 管道中总帧数: $\frac{50}{3} \text{ KB} / 1 \text{ KB} = \frac{50}{3} \approx 16.67 \rightarrow 16$ frames (floor, since can't send partial frame / 向下取整，因为不能发送部分帧)
- For SR / 对于SR: $2^{n-1} \ge 16 \rightarrow n-1 \ge 4 \rightarrow n \ge 5$
- Minimum sequence bits / 最小序号位数: $\boxed{5 \text{ bits}}$

**Example 3** / **示例3**: Using the simple bandwidth-delay product method / 使用简化的带宽-时延积方法：

$$\text{Frames in pipe} = \frac{\text{BDP}}{\text{Frame size}} = \frac{\text{Bandwidth} \times RTT}{\text{Frame size in bits}} \quad \text{(管道中帧数 = 带宽-时延积 / 帧大小)}$$

Then $n = \lceil \log_2(\text{frames in pipe}) \rceil$ or adjust based on the protocol constraint.

> **中文**：然后根据协议约束调整：对于GBN需确保$2^n - 1 \ge$ 帧数，对于SR需确保$2^{n-1} \ge$ 帧数。

---

## 3.9 PPP (Point-to-Point Protocol) ★ / 点对点协议

PPP is used for point-to-point links (e.g., dial-up, DSL). It handles framing, link control, and network layer protocol negotiation.

> **中文**：PPP用于点对点链路（如拨号、DSL）。它处理成帧、链路控制和网络层协议协商。

### 3.9.1 Three Components of PPP / PPP的三个组成部分

1. **Framing method** / **成帧方法**: Encapsulation of IP datagrams on serial links (supports async and sync) / 在串行链路上封装IP数据报（支持异步和同步）
2. **LCP (Link Control Protocol)** / **链路控制协议**: Establish, configure, maintain, and terminate the data link connection; testing; negotiating options / 建立、配置、维护和终止数据链路连接；测试；协商选项
3. **NCP (Network Control Protocol)** / **网络控制协议**: A family of protocols to negotiate network layer options (e.g., IPCP for IP, IPXCP for IPX) / 一系列用于协商网络层选项的协议（如IPCP用于IP，IPXCP用于IPX）

### 3.9.2 PPP Frame Format / PPP帧格式

| Flag / 标志 | Address / 地址 | Control / 控制 | Protocol / 协议 | Information / 信息 | FCS / 帧检验序列 | Flag / 标志 |
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| `7E` | `FF` | `03` | Variable / 可变 | IP Datagram / IP数据报 | Variable / 可变 | `7E` |
| 1 B | 1 B | 1 B | 1 or 2 B | 0-1500 B (default / 默认) | 2 or 4 B | 1 B |

### 3.9.3 PPP Frame Length Calculation / PPP帧长度计算

| Component / 组成部分 | Default / 默认 | Negotiable Range / 可协商范围 |
|-----------|:---:|------|
| Flag / 标志 | 1 B | Not negotiable / 不可协商 |
| Address / 地址 | 1 B (always `FF` / 固定为`FF`) | Not negotiable / 不可协商 |
| Control / 控制 | 1 B (always `03` / 固定为`03`) | Not negotiable / 不可协商 |
| Protocol / 协议 | **2 B** | Can be negotiated to **1 B** by LCP / 可通过LCP协商为**1 B** |
| Information / 信息 | **1500 B** | Can be negotiated to any size by LCP / 可通过LCP协商为任意大小 |
| FCS / 帧检验序列 | **2 B** | Can be negotiated to **4 B** by LCP / 可通过LCP协商为**4 B** |

**PPP frame length bounds** / **PPP帧长度范围**：

- **Default** / **默认**: $1+1+1+2+1500+2+1 = \boxed{1508 \text{ bytes}}$
- **Minimum** / **最小**: $1+1+1+1+0+2+1 = \boxed{7 \text{ bytes}}$
- **Maximum** / **最大**: $1+1+1+2+1500+4+1 = \boxed{1510 \text{ bytes}}$

**Key exam point** / **考试要点**: PPP provides **unreliable** transmission (no error correction, no flow control). It only detects errors.

> **中文**：PPP提供**不可靠**传输（无纠错、无流控）。它只检测错误，不纠正错误。

**Protocol field values** / **协议字段值**：

- Bit 15 = 0: Information field contains a network layer protocol / 信息字段包含网络层协议
- Bit 15 = 1: Information field contains a PPP **configuration protocol** (LCP or NCP) / 信息字段包含PPP**配置协议**（LCP或NCP）

---

## 3.10 HDLC (High-Level Data Link Control) / 高级数据链路控制

HDLC is a bit-oriented data link protocol. It uses **bit stuffing** for transparency.

> **中文**：HDLC是一种面向比特的数据链路协议，使用**比特填充**实现透明传输。

### 3.10.1 Frame Types / 帧类型

| Type / 类型 | Full Name / 全称 | Function / 功能 |
|------|-----------|----------|
| **I-frame** / **信息帧** | Information frame / 信息帧 | Carries user data; includes sequence numbers (N(S), N(R)) / 携带用户数据；包含序号N(S)、N(R) |
| **S-frame** / **监督帧** | Supervisory frame / 监督帧 | Carries control information: RR (Receiver Ready), RNR (Receiver Not Ready), REJ (Reject), SREJ (Selective Reject) / 携带控制信息：RR（接收就绪）、RNR（接收未就绪）、REJ（拒绝）、SREJ（选择拒绝） |
| **U-frame** / **无编号帧** | Unnumbered frame / 无编号帧 | Link management: set mode, disconnect, etc. / 链路管理：设置模式、断开连接等 |

### 3.10.2 Operating Modes / 工作模式

| Mode / 模式 | Full Name / 全称 | Description / 描述 |
|------|-----------|-------------|
| **NRM** / **正常响应模式** | Normal Response Mode / 正常响应模式 | Primary/secondary relationship; secondary can only transmit when polled / 主/从关系；从站只有在被轮询时才能发送 |
| **ARM** / **异步响应模式** | Asynchronous Response Mode / 异步响应模式 | Secondary may initiate transmission without polling / 从站无需轮询即可发起传输 |
| **ABM** / **异步平衡模式** | Asynchronous Balanced Mode / 异步平衡模式 | Both stations are equals; full-duplex, no primary/secondary / 双方平等；全双工，无主/从之分 |

---

## 3.11 Exam Practice: Data Link Control Questions / 考试练习：数据链路控制问题

### Fill-in-the-Blank / 填空题

1. The three core mechanisms of ARQ protocols are ______, ______, and ______. / ARQ协议的三个核心机制是______、______和______。
   **(acknowledgment, timer, frame sequence number) / （确认、定时器、帧序号）**

2. In Go-Back-N ARQ, the receiver window size is ______. / 在后退N帧ARQ中，接收窗口大小为______。
   **(1)**

3. The PPP frame starts and ends with the flag byte ______. / PPP帧以标志字节______开始和结束。
   **(0x7E)**

4. HDLC uses ______ stuffing to achieve data transparency. / HDLC使用______填充实现数据透明传输。
   **(bit — more precisely, 0-bit insertion) / （比特填充——更精确地说是0比特插入）**

5. To correct 1-bit errors, the Hamming distance must be at least ______. / 要纠正1比特错误，海明距离至少为______。
   **(3, since $2 \times 1 + 1 = 3$) / （3，因为$2 \times 1 + 1 = 3$）**

6. In Selective Repeat ARQ, the maximum sender window size with n-bit sequence numbers is ______. / 在选择重发ARQ中，使用n比特序号的最大发送窗口大小为______。
   **($2^{n-1}$)**

7. CRC is a(n) ______ code (error-detecting or error-correcting). / CRC是一种______码（检错或纠错）。
   **(error-detecting) / （检错码）**

8. In PPP, the default protocol field size is ______ byte(s), negotiable to ______ byte(s). / 在PPP中，默认协议字段大小为______字节，可协商为______字节。
   **(2, 1)**

9. The four framing methods are: ______, ______, ______, and ______. / 四种成帧方法是：______、______、______和______。
   **(byte count, byte stuffing, bit stuffing, physical layer coding violations) / （字节计数法、字节填充、比特填充、物理层编码违例）**

10. In the bit stuffing approach, the flag pattern is ______. / 在比特填充方法中，标志模式为______。
    **(01111110)**

### True/False / 判断题

1. (T/F) Stop-and-wait ARQ uses a 2-bit sequence number. / 停-等ARQ使用2比特序号。
   **(F — it only needs 1 bit, alternating 0 and 1) / （错——只需要1比特，交替使用0和1）**

2. (T/F) In Go-Back-N, the receiver accepts frames out of order. / 在后退N帧中，接收端接受乱序帧。
   **(F — GBN receiver discards out-of-order frames; only SR accepts them) / （错——GBN接收端丢弃乱序帧；只有SR才接受乱序帧）**

3. (T/F) CRC can correct single-bit errors. / CRC可以纠正单比特错误。
   **(F — CRC only detects errors, cannot correct them) / （错——CRC只检测错误，不能纠正错误）**

4. (T/F) With 3-bit sequence numbers, Go-Back-N allows a sender window of up to 8 frames. / 使用3比特序号时，后退N帧允许发送窗口最大为8帧。
   **(F — max is $2^3 - 1 = 7$) / （错——最大为$2^3 - 1 = 7$）**

5. (T/F) NAK in Selective Repeat is essential for correctness. / 选择重发中的NAK对正确性是必不可少的。
   **(F — it's an optimization; timers would eventually trigger retransmission) / （错——NAK是优化手段；定时器最终会触发重传）**

6. (T/F) PPP uses byte stuffing, not bit stuffing. / PPP使用字节填充，而非比特填充。
   **(T — PPP uses 0x7E flag and byte stuffing with escape characters) / （对——PPP使用0x7E标志和带转义字符的字节填充）**

### Multiple Choice / 选择题

1. Which protocol requires the receiver to buffer out-of-order frames? / 哪个协议要求接收端缓存乱序帧？
   a) Stop-and-Wait / 停-等 b) Go-Back-N / 后退N帧 c) Selective Repeat / 选择重发 d) All of them / 全部
   **(c) / （c）**

2. A 3-bit sequence number in Go-Back-N allows a maximum sender window of: / 后退N帧中使用3比特序号允许的最大发送窗口为：
   a) 7 b) 8 c) 4 d) 3
   **(a — $2^3-1=7$)**

3. What is the CRC remainder appended to the data if $G(x) = x^4 + x + 1$? / 如果$G(x) = x^4 + x + 1$，CRC余数有多少位？
   a) 1 bit b) 2 bits c) 3 bits d) 4 bits
   **(d — degree of $G(x)$ = 4) / （d——$G(x)$的次数为4）**

4. The PPP frame's address field is always: / PPP帧的地址字段始终是：
   a) 00 b) FF c) 7E d) 03
   **(b — broadcast address, indicating all stations; since PPP is point-to-point, there's only one station) / （b——广播地址，指示所有站点；由于PPP是点对点，实际上只有一个站点）**

5. Hamming distance of 4 can detect up to __ errors: / 海明距离为4最多可检测__个错误：
   a) 1 b) 2 c) 3 d) 4
   **(c — detect up to $d$ errors with distance $d+1=4$ → $d=3$) / （c——海明距离$d+1=4$可检测最多$d=3$个错误）**

### Calculation Problems / 计算题

**Problem 1** / **题目1** (CRC): Data / 数据 = `1101011011`, $G(x) = x^4 + x + 1$ (generator / 生成多项式 = `10011`). Find the CRC and the transmitted frame / 求CRC和发送帧。

Data has 10 bits. $G(x)$ has degree 4 → append 4 zeros.
Dividend / 被除数: `11010110110000`

> **中文**：数据有10位，$G(x)$次数为4 → 附加4个0，被除数为`11010110110000`。

Perform XOR division (not shown here for brevity — practice this!). Result (remainder) / 余数 = `1110`.
Transmitted frame / 发送帧: `1101011011` + `1110` = `11010110111110`.

**Problem 2** / **题目2** (Hamming code bits / 海明码校验位数): A message has 64 data bits. How many check bits are needed for single-error correction? / 一条消息有64个数据比特，单纠错需要多少校验比特？

$2^r \ge 64 + r + 1 = 65 + r$

Try values / 尝试取值：
- $r=7$: $2^7 = 128$, $65 + 7 = 72$ → $128 \ge 72$ ✓

$\boxed{7 \text{ check bits needed / 需要7个校验比特}}$

**Problem 3** / **题目3** (Sliding window / 滑动窗口): A 2 Mbps link, frame size 1000 bytes, RTT = 50 ms. Find minimum sequence bits for Selective Repeat to fully utilize the link. / 2 Mbps链路，帧大小1000字节，RTT = 50 ms。求选择重发协议完全利用链路所需的最小序号位数。

- $T_f = \frac{1000 \times 8}{2 \times 10^6} = 0.004 \text{ s} = 4 \text{ ms}$
  
  > **中文**：帧发送时间 = 1000×8 / 2×10^6 = 4 ms

- Frames per RTT: 50 ms / 4 ms = 12.5 / 每个RTT内的帧数：50 / 4 = 12.5
- Frames in pipe: 1 + 12.5 = 13.5 → need window size at least 14 / 管道中的帧数：1 + 12.5 = 13.5 → 需要窗口大小至少为14
- For SR / 对于SR: $2^{n-1} \ge 14$ → $n-1 \ge \lceil \log_2 14 \rceil = 4$ → $n \ge 5$
- $\boxed{5 \text{ bits}}$

**Problem 4** / **题目4** (Stop-and-wait utilization / 停-等协议利用率): 1 Mbps link, RTT = 20 ms, frame = 1000 bytes. Utilization? / 1 Mbps链路，RTT = 20 ms，帧 = 1000字节。求利用率。

- $T_f = 8000 / 10^6 = 8 \text{ ms}$
- $U = \frac{8}{8 + 20} = \frac{8}{28} \approx \boxed{28.6\%}$

**Problem 5** / **题目5** (PPP frame / PPP帧): Default PPP frame length = ? / 默认PPP帧长度 = ?
- $1+1+1+2+1500+2+1 = \boxed{1508 \text{ bytes}}$

**Problem 6** / **题目6** (Bit stuffing / 比特填充): After bit stuffing, what is transmitted for the data `1111101111100111110`? / 对数据`1111101111100111110`进行比特填充后，发送的是什么？

Scan for 5 consecutive 1s / 扫描连续5个1：
- `11111`0 → after 5th 1, insert 0 / 第5个1之后插入0: `111110`0
- `011111`0 → after 5th 1, insert 0 / 第5个1之后插入0: `0111110`0
- `011111`0 → after 5th 1, insert 0 / 第5个1之后插入0: `0111110`0

Stuffed / 填充后: `1111100111110001111100`

**Problem 7** / **题目7** (Sequence number field design / 序号字段设计): 1 Gbps link, RTT = 1 ms, frame = 1500 bytes (12000 bits). Using Selective Repeat, how many bits for sequence numbers? / 1 Gbps链路，RTT = 1 ms，帧 = 1500字节（12000比特）。使用选择重发，需要多少序号位？

- $T_f = 12000 / 10^9 = 12 \text{ μs}$
  
  > **中文**：帧发送时间 = 12000 / 10^9 = 12 μs

- Frames per RTT: 1000 μs / 12 μs ≈ 83.33 → 83 / 每个RTT内帧数：1000 / 12 ≈ 83.33 → 83
- Window needed / 所需窗口: $1 + 83 = 84$
- SR: $2^{n-1} \ge 84$ → $n-1 \ge \lceil \log_2 84 \rceil = 7$ → $n \ge 8$
- $\boxed{8 \text{ bits}}$

---

## Appendix: Chinese-English Terminology / 附录：中英文术语对照

### Data Link Control / 数据链路控制
| English / 英文 | Chinese / 中文 |
|------|------|
| Framing | 成帧 |
| Byte count | 字节计数法 |
| Byte stuffing | 字节填充 |
| Bit stuffing (0-bit insertion) | 比特填充（0比特插入） |
| Flag byte / Flag pattern | 标志字节 / 标志模式 |
| Error detection | 差错检测 |
| Error correction | 差错纠正 |
| Parity check | 奇偶校验 |
| CRC (Cyclic Redundancy Check) | 循环冗余校验 |
| Generator polynomial | 生成多项式 |
| Hamming code | 海明码 |
| Hamming distance | 海明距离/码距 |
| Codeword | 码字 |
| Forward Error Correction (FEC) | 前向纠错 |
| Stop-and-Wait | 停-等协议 |
| ARQ (Automatic Repeat reQuest) | 自动重发请求 |
| Sliding window | 滑动窗口 |
| Piggybacking | 捎带确认 |
| Go-Back-N (GBN) | 后退N帧 |
| Selective Repeat (SR) | 选择重发 |
| Cumulative ACK | 累积确认 |
| NAK (Negative Acknowledgment) | 否定确认 |
| Sequence number | 序号/序列号 |
| Window size | 窗口大小 |
| PPP (Point-to-Point Protocol) | 点对点协议 |
| LCP (Link Control Protocol) | 链路控制协议 |
| NCP (Network Control Protocol) | 网络控制协议 |
| HDLC (High-Level Data Link Control) | 高级数据链路控制 |
| I-frame / S-frame / U-frame | 信息帧/监督帧/无编号帧 |
| NRM / ARM / ABM | 正常响应模式/异步响应模式/异步平衡模式 |
