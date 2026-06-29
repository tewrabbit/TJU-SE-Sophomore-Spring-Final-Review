# Chapter 7 课后习题全解 + 考试压轴题 + 公式速查 + 常见错误

---

## 10. COMPLETE EXERCISE SOLUTIONS (课后习题全解)

---

### 7.1 — 无损分解判定 ★★★★★

**题目**: 设 $R = (A, B, C, D, E)$, $F = \{A \rightarrow BC, CD \rightarrow E, B \rightarrow D, E \rightarrow A\}$。将 R 分解为 $(A, B, C)$ 和 $(A, D, E)$，判断该分解是否无损。

**解答**:

判定条件：对于二元分解 $R_1, R_2$，若 $R_1 \cap R_2 \rightarrow R_1$ 或 $R_1 \cap R_2 \rightarrow R_2$ 属于 $F^+$，则无损。

- $R_1 = (A, B, C)$，$R_2 = (A, D, E)$
- $R_1 \cap R_2 = \{A\}$
- 由习题 7.6 知 A 是候选码，故 $A \rightarrow ABCDE$，当然 $A \rightarrow ABC$
- 即 $R_1 \cap R_2 \rightarrow R_1 \in F^+$

∴ **无损 ✓**

---

### 7.2 — 从实例列出非平凡FD ★★★

**题目**: 列出图 7.17 中关系实例满足的所有非平凡函数依赖。

**解答**:

从图 7.17 的实例数据：
- $A \rightarrow B$（A 相同的行 B 也相同）
- $C \rightarrow B$（C 相同的行 B 也相同）
- $AC \rightarrow B$（由前两条逻辑蕴含）

注意：
- $C \not\rightarrow A$（第 1、3 行 C 相同但 A 不同）
- $B \not\rightarrow A$（第 1、2 行 B 相同但 A 不同）
- $A \not\rightarrow C$（第 1、2 行 A 相同但 C 不同）

∴ **非平凡 FD = {A → B, C → B, AC → B}**

---

### 7.3 — FD 表达联系基数 ★★★

**题目**: 解释如何用函数依赖表示以下联系：
- student 和 instructor 之间的**一对一**联系
- student 和 instructor 之间的**多对一**联系

**解答**:

设 $PK(r)$ 表示关系 $r$ 的主码。

| 联系类型 | FD 表示 | 含义 |
|---------|---------|------|
| **一对一 (1:1)** | $PK(student) \rightarrow PK(instructor)$ 且 $PK(instructor) \rightarrow PK(student)$ | 学生确定唯一的导师，导师也确定唯一的学生 |
| **多对一 (N:1)** | $PK(student) \rightarrow PK(instructor)$ | 学生确定唯一的导师，但一个导师可对应多个学生 |

---

### 7.4 — 证明合并律 (Union Rule) ★★★★

**题目**: 用阿姆斯特朗公理证明合并律：若 $\alpha \rightarrow \beta$ 且 $\alpha \rightarrow \gamma$，则 $\alpha \rightarrow \beta\gamma$。

**解答**:

| 步骤 | 推导 | 所用公理 |
|------|------|---------|
| 1 | $\alpha \rightarrow \beta$ | 给定 |
| 2 | $\alpha\alpha \rightarrow \alpha\beta$ | 增广律 (用 α 增广步骤1) |
| 3 | $\alpha \rightarrow \alpha\beta$ | αα = α (化简) |
| 4 | $\alpha \rightarrow \gamma$ | 给定 |
| 5 | $\alpha\beta \rightarrow \gamma\beta$ | 增广律 (用 β 增广步骤4) |
| 6 | $\alpha\beta \rightarrow \beta\gamma$ | 交换律 |
| 7 | $\alpha \rightarrow \beta\gamma$ | 传递律 (步骤3 + 步骤6) ✓ |

---

### 7.5 — 证明伪传递律 (Pseudotransitivity Rule) ★★★★

**题目**: 用阿姆斯特朗公理证明伪传递律：若 $\alpha \rightarrow \beta$ 且 $\gamma\beta \rightarrow \delta$，则 $\alpha\gamma \rightarrow \delta$。

**解答**:

| 步骤 | 推导 | 所用公理 |
|------|------|---------|
| 1 | $\alpha \rightarrow \beta$ | 给定 |
| 2 | $\alpha\gamma \rightarrow \gamma\beta$ | 增广律 (用 γ 增广步骤1) |
| 3 | $\gamma\beta \rightarrow \delta$ | 给定 |
| 4 | $\alpha\gamma \rightarrow \delta$ | 传递律 (步骤2 + 步骤3) ✓ |

---

### 7.6 — 计算 F⁺ 和候选码 ★★★★★

**题目**: 设 $R = (A, B, C, D, E)$, $F = \{A \rightarrow BC, CD \rightarrow E, B \rightarrow D, E \rightarrow A\}$。计算 $F^+$（列出非平凡成员），并找出 R 的所有候选码。

**解答**:

**计算闭包推导链**:
- $A \rightarrow BC \Rightarrow A \rightarrow B$ 且 $A \rightarrow C$（分解律）
- $A \rightarrow B$ 且 $B \rightarrow D \Rightarrow A \rightarrow D$（传递律）
- $A \rightarrow CD$ 且 $CD \rightarrow E \Rightarrow A \rightarrow E$（传递律）
- $A \rightarrow A$（自反律），综上 $A \rightarrow ABCDE$ → **A 是候选码 ✓**
- $E \rightarrow A$ 且 $A \rightarrow ABCDE \Rightarrow E \rightarrow ABCDE$ → **E 是候选码 ✓**
- $CD \rightarrow E$ 且 $E \rightarrow ABCDE \Rightarrow CD \rightarrow ABCDE$ → **CD 是候选码 ✓**
- $BC \rightarrow CD$（B→D 增广），$CD \rightarrow E$，$E \rightarrow A$，$A \rightarrow BC \Rightarrow$ **BC 是候选码 ✓**

**候选码: {A, BC, CD, E}**

---

### 7.7 — 正则覆盖 ★★★★

**题目**: 使用习题 7.6 的函数依赖，计算正则覆盖 $F_c$。

**解答**:

给定 $F = \{A \rightarrow BC, CD \rightarrow E, B \rightarrow D, E \rightarrow A\}$。

- 每个 FD 的左部都唯一
- 左部和右部均没有无关属性
- 不需要合并（没有左部相同的 FD）

∴ **$F_c = F$ 本身 = $\{A \rightarrow BC, CD \rightarrow E, B \rightarrow D, E \rightarrow A\}$**

---

### 7.8 — 高效闭包算法 ★★

**题目**: 图 7.18 的算法（线性时间）比图 7.8（二次时间）更高效，说明其正确性和复杂度。

**解答**:

| 对比 | 图 7.8 算法 | 图 7.18 算法 |
|------|-----------|------------|
| **复杂度** | $O(n^2)$ — 外层循环最多 n 次，每次扫描所有 FD | $O(n)$ — 每个 FD 只扫描一次 |
| **核心思路** | 反复扫描所有 FD，直到 result 不变 | 用 `fdcount` 记录每个 FD 左部还剩多少属性未加入 result，为 0 时触发添加右部属性 |
| **正确性** | 归纳法可证：若 $A \in \alpha^+$，则 A 最终被加入 result | 同上，且通过 `appears` 数组实现类似 BFS 的传播 |

---

### 7.9 — SQL 测试 FD ★★★

**题目**: 给定关系模式 $R(A, B, C)$ 和实例 $r$，写 SQL 查询测试 $B \rightarrow C$ 是否成立。再写一个 ASSERTION 强制执行该 FD。

**解答**:

**测试查询**（结果非空 ⇔ FD **不**成立）：
```sql
SELECT B
FROM r
GROUP BY B
HAVING COUNT(DISTINCT C) > 1;
```

**ASSERTION 约束**（注意：目前主流数据库均不支持）：
```sql
CREATE ASSERTION b_to_c CHECK (
    NOT EXISTS (
        SELECT B
        FROM r
        GROUP BY B
        HAVING COUNT(DISTINCT C) > 1
    )
);
```

---

### 7.10 — NULL 值对无损连接的影响 ★★★

**题目**: 讨论如果 FD 左侧属性允许 NULL 值，无损分解会出现什么问题。

**解答**:

自然连接基于笛卡尔积和选择运算。选择运算对 NULL 值的任何比较都返回 **unknown**。

- 因此，自然连接会**排除**公共属性上有 NULL 值的元组
- 结果：即使理论上无损的分解，若连接属性含 NULL，实际连接会丢失那些元组 → **变成有损**
- 注意：只有 FD **左侧**（连接属性）的 NULL 会引起此问题；仅右侧属性含 NULL 不影响

---

### 7.11 — BCNF 分解中的主码/外码 ★★★★

**题目**: 在 BCNF 分解算法中，用 $\alpha \rightarrow \beta$ 将 $r(\alpha, \beta, \gamma)$ 分解为 $r_1(\alpha, \beta)$ 和 $r_2(\alpha, \gamma)$。

a. 期望在分解后的关系上保持什么主码和外码约束？
b. 如果不强制外码约束，举一个可能发生的不一致例子。
c. 对 3NF 分解（7.5.2 节算法），期望保持什么主码和外码约束？

**解答**:

**a.** $\alpha$ 是 $r_1$ 的**主码**，$\alpha$ 是 $r_2$ 的**外码**（引用 $r_1$）。

**b.** 若没有外码约束，从 $r_1$ 删除一个元组时，$r_2$ 中对应的引用元组不会级联删除 → 原本应该整体删除的元组被部分保留（或 $\alpha$ 被设为 NULL）。

**c.** 对每个因 $\alpha \rightarrow \beta$ 加入的 $r_i(\alpha\beta)$，$\alpha$ 应是主码。原始关系的候选码 $\gamma$ 应位于某个 $r_k$ 中并作为其主码。外码约束：若 $r_i$ 的主码属性也出现在 $r_j$ 中，则 $r_j$ 中那些属性构成外码引用 $r_i$。

---

### 7.13 — 证明不保持依赖 ★★★★

**题目**: 证明习题 7.1 的分解**不是**依赖保持的。

**解答**:

要证明 $B \rightarrow D$ 不被保持。

- $F_1$（$(A,B,C)$ 上的限制）：B 作为左部的 FD 只有平凡的 $B \rightarrow B$
- $F_2$（$(A,D,E)$ 上的限制）：D 仅出现在右部，左部没有 B
- 要使 $B \rightarrow D$ 被保持，需要 $B \rightarrow \alpha$ 在 $F_1^+$ 中且 $\alpha \rightarrow D$ 在 $F_2^+$ 中
- 两表交集为 A，所以 $\alpha$ 只能是 A
- 但 $B \rightarrow A$ 不在 $F_1^+$ 中（$B^+ = \{B, D\}$，不含 A）

∴ **$B \rightarrow D$ 不被保持 ✗**

---

### 7.14 — 正则覆盖不唯一 ★★★★

**题目**: 用以下 FD 集合证明正则覆盖可以不唯一：
$$X \rightarrow YZ, \quad Y \rightarrow XZ, \quad Z \rightarrow XY$$

**解答**:

**路径 1**（先删每个 FD 右部第二个属性）：

| 步骤 | 操作 | 结果 |
|------|------|------|
| 1 | $X \rightarrow YZ$ 中删 Y（Z 是无关的） | $X \rightarrow Z$ |
| 2 | $Y \rightarrow XZ$ 中删 Z（X 是无关的） | $Y \rightarrow X$ |
| 3 | $Z \rightarrow XY$ 中删 X（Y 是无关的） | $Z \rightarrow Y$ |

$$F_{c1} = \{X \rightarrow Z, Y \rightarrow X, Z \rightarrow Y\}$$

**路径 2**（先删每个 FD 右部第一个属性）：

| 步骤 | 操作 | 结果 |
|------|------|------|
| 1 | $X \rightarrow YZ$ 中删 Z（Y 是无关的） | $X \rightarrow Y$ |
| 2 | $Y \rightarrow XZ$ 中删 X（Z 是无关的） | $Y \rightarrow Z$ |
| 3 | $Z \rightarrow XY$ 中删 Y（X 是无关的） | $Z \rightarrow X$ |

$$F_{c2} = \{X \rightarrow Y, Y \rightarrow Z, Z \rightarrow X\}$$

两种不同的正则覆盖！✓

---

### 7.15 — 为什么一次只能删一个无关属性 ★★★

**题目**: 用习题 7.14 的 FD 说明为什么同时删除两个无关属性会出问题。

**解答**:

在 $X \rightarrow YZ$ 中，Y 和 Z 各自看起来都是无关的。但如果**同时删除两者**：
- 删掉 Y 和 Z 后得到 $X \rightarrow \emptyset$（无意义）
- 新集合无法再推导出 $X \rightarrow YZ$

关键是：**删 Y 后 Z 不再无关，删 Z 后 Y 不再无关**。算法一次只删一个属性正是为了避免此问题。

---

### 7.18 — 3NF 的等价定义 ★★★★

**题目**: 用"非主属性对码的传递函数依赖"重新表述 3NF 定义，并证明与教科书定义的等价性。

**教科书定义**: $\forall \alpha \rightarrow \beta$（非平凡），$\alpha$ 是超码 OR $(\beta - \alpha)$ 中每个属性都在某个候选码中。

**习题定义**: 不存在非主属性 A 对候选码的传递函数依赖。

**解答**:

**正向（教科书 ⇒ 习题）**: 假设 R 满足教科书 3NF 但不满足习题定义。则存在非主属性 A 传递依赖于候选码 $\alpha$：
- 存在 $\beta$ 使 $\alpha \rightarrow \beta$，$\beta \rightarrow A$，$A \notin \alpha, A \notin \beta$，且 $\beta \rightarrow \alpha$ 不成立
- 则 $\beta \rightarrow A$ 违反教科书 3NF：非平凡、$\beta$ 不是超码、A 不在任何候选码中。矛盾。

**反向（习题 ⇒ 教科书）**: 假设 R 违反教科书 3NF。则存在 $\alpha \rightarrow \beta$ 非平凡，$\alpha$ 不是超码，且某 $A \in \beta - \alpha$ 不在任何候选码中。
- A 是非主属性，$\alpha \rightarrow A$
- 取候选码 $\gamma$：$\gamma \rightarrow \alpha$，$\alpha \not\rightarrow \gamma$，$A \notin \alpha, A \notin \gamma$
- 所以 A 传递依赖于 $\gamma$，违反习题定义。

∴ 两个定义**等价** ✓

---

### 7.19 — 3NF ⇒ 2NF 的证明 ★★★

**题目**: 证明每个 3NF 模式也属于 2NF。（提示：证明每个部分依赖也是传递依赖。）

**解答**:

回顾定义：
- **2NF**：无非主属性对候选码的**部分**依赖
- **3NF**（习题 7.18 定义）：无非主属性对候选码的**传递**依赖

证明：若非主属性 A 部分依赖于候选码 $\alpha$：
- 存在 $\alpha$ 的真子集 $\beta$ 使 $\beta \rightarrow A$
- $\alpha \rightarrow \beta$（自反律，$\beta \subset \alpha$）
- $\beta \not\rightarrow \alpha$（因为 $\alpha$ 是候选码，极小）
- $A \notin \beta$，$A \notin \alpha$（A 是非主属性）
- ⇒ $\alpha \rightarrow A$ 是**传递**依赖

因此每个部分依赖也是传递依赖。3NF 消除了所有传递依赖 ⇒ 自然也消除所有部分依赖 ⇒ **3NF ⊆ 2NF ✓**

---

### 7.20 — BCNF 但不满足 4NF 的示例 ★★★★

**题目**: 举例说明一个关系模式属于 BCNF 但不属于 4NF。

**解答**:

最简示例：$R = (A, B, C)$，唯一的非平凡依赖是 MVD $A \twoheadrightarrow B$。

- **无 FD 违反 BCNF**：没有非平凡 FD，所以 BCNF 自动满足
- **不满足 4NF**：$A \twoheadrightarrow B$ 是非平凡 MVD，且 A 不是超码（A 不能唯一确定 B）

这就是教科书中的 `inst_info(ID, child_name, phone_number)` 例子 — BCNF 但充满冗余！

---

### 7.25 — 阿姆斯特朗公理的正确性证明 ★★★★

**题目**: 用函数依赖的定义论证阿姆斯特朗三条公理（自反律、增广律、传递律）的正确性。

**解答**:

**自反律**: 若 $\beta \subseteq \alpha$，则 $\alpha \rightarrow \beta$。

证明：设 $t_1[\alpha] = t_2[\alpha]$。对任意 $x \in \beta$，由于 $\beta \subseteq \alpha$，有 $x \in \alpha$。故 $t_1[x] = t_2[x]$，即 $t_1[\beta] = t_2[\beta]$。✓

**增广律**: 若 $\alpha \rightarrow \beta$，则 $\gamma\alpha \rightarrow \gamma\beta$。

证明：设 $t_1[\gamma\alpha] = t_2[\gamma\alpha]$。对任意 $x \in \gamma\beta$：
- 若 $x \in \gamma$：由 $t_1[\gamma\alpha] = t_2[\gamma\alpha]$ 直接得 $t_1[x] = t_2[x]$
- 若 $x \in \beta$：由 $\alpha \rightarrow \beta$ 及 $t_1[\alpha] = t_2[\alpha]$（从 $t_1[\gamma\alpha] = t_2[\gamma\alpha]$ 可得），有 $t_1[x] = t_2[x]$

故 $t_1[\gamma\beta] = t_2[\gamma\beta]$。✓

**传递律**: 若 $\alpha \rightarrow \beta$ 且 $\beta \rightarrow \gamma$，则 $\alpha \rightarrow \gamma$。

证明：设 $t_1[\alpha] = t_2[\alpha]$。
- 由 $\alpha \rightarrow \beta$ 得 $t_1[\beta] = t_2[\beta]$
- 由 $\beta \rightarrow \gamma$ 得 $t_1[\gamma] = t_2[\gamma]$

故 $t_1[\gamma] = t_2[\gamma]$。✓

---

### 7.26 — 非公理的"规则"不成立 ★★

**题目**: 考虑以下提出的规则：若 $\alpha \rightarrow \beta$ 且 $\gamma \rightarrow \beta$，则 $\alpha \rightarrow \gamma$。通过反例证明该规则不正确。

**解答**:

构造反例：

| α | γ | β |
|---|---|---|
| 1 | 6 | 7 |
| 2 | 3 | 5 |
| 2 | 4 | 5 |

- $\alpha \rightarrow \beta$ ✓（α=1→β=7, α=2→β=5，α 相同则 β 相同）
- $\gamma \rightarrow \beta$ ✓（γ=6→β=7, γ=3→β=5, γ=4→β=5）
- 但 $\alpha \rightarrow \gamma$ ✗（α=2 时，γ 分别为 3 和 4，不相同）

∴ 该规则**不正确** ✗

---

### 7.27 — 证明分解律 (Decomposition Rule) ★★★★

**题目**: 用阿姆斯特朗公理证明分解律的正确性。

**解答**:

分解律：若 $\alpha \rightarrow \beta\gamma$，则 $\alpha \rightarrow \beta$ 且 $\alpha \rightarrow \gamma$。

证明：

| 步骤 | 推导 | 所用公理 |
|------|------|---------|
| 1 | $\alpha \rightarrow \beta\gamma$ | 给定 |
| 2 | $\beta\gamma \rightarrow \beta$ | 自反律（$\beta \subseteq \beta\gamma$） |
| 3 | $\beta\gamma \rightarrow \gamma$ | 自反律（$\gamma \subseteq \beta\gamma$） |
| 4 | $\alpha \rightarrow \beta$ | 传递律（步骤1 + 步骤2） |
| 5 | $\alpha \rightarrow \gamma$ | 传递律（步骤1 + 步骤3）✓ |

---

### 7.28 — 计算 B⁺ ★★★

**题目**: 使用习题 7.6 的 FD，计算 $B^+$。

**解答**:

$F = \{A \rightarrow BC, CD \rightarrow E, B \rightarrow D, E \rightarrow A\}$

使用闭包算法：
- result = {B}
- $B \rightarrow D$，B ⊆ result ⇒ result = {B, D}
- 检查所有 FD，无更多可添加

∴ **$B^+ = \{B, D\}$**

---

### 7.29 — 有损的 BCNF 分解示例 ★★★★

**题目**: 证明以下分解是**有损**的：将习题 7.1 的 R 分解为 $(A, B, C)$ 和 $(C, D, E)$。

**解答**:

构造反例 — 取 r(R) 为：

| A | B | C | D | E |
|---|---|---|---|---|
| 1 | 6 | 5 | 7 | 3 |
| 2 | 8 | 5 | 9 | 4 |

$\Pi_{A,B,C}(r)$:

| A | B | C |
|---|---|---|
| 1 | 6 | 5 |
| 2 | 8 | 5 |

$\Pi_{C,D,E}(r)$:

| C | D | E |
|---|---|---|
| 5 | 7 | 3 |
| 5 | 9 | 4 |

自然连接后产生 4 行（笛卡尔积效应）：

| A | B | C | D | E |
|---|---|---|---|---|
| 1 | 6 | 5 | 7 | 3 |
| 1 | 6 | 5 | 9 | 4 |
| 2 | 8 | 5 | 7 | 3 |
| 2 | 8 | 5 | 9 | 4 |

多了 2 行"伪元组" → **有损 ✗**

（原因：交集 = {C}，$C^+ = \{C\}$，不满足无损条件）

---

### 7.30 — 综合题 (B⁺, 超码, Fc, 3NF, BCNF) ★★★★★

**题目**: $R = (A, B, C, D, E, G)$, $F = \{A \rightarrow BCD, BC \rightarrow DE, B \rightarrow D, D \rightarrow A\}$

**a.** 计算 $B^+$。
**b.** 证明 AG 是超码。
**c.** 计算 Fc。
**d.** 给出基于 Fc 的 3NF 分解。
**e.** 给出 BCNF 分解。

**解答**:

**a.** $B^+$:
- result = {B}
- $B \rightarrow D$ ⇒ result = {B, D}
- $D \rightarrow A$ ⇒ result = {A, B, D}
- $A \rightarrow BCD$ ⇒ result = {A, B, C, D}
- $BC \rightarrow DE$ ⇒ result = {A, B, C, D, E}

∴ **$B^+ = \{A, B, C, D, E\}$**

**b.**
- $A \rightarrow BCD$ ⇒ $A \rightarrow BC$（分解律）
- $BC \rightarrow DE$（给定）
- $A \rightarrow DE$（传递律）
- $A \rightarrow BCDE$（合并律）
- $AG \rightarrow ABCDEG$（增广律）
- ∴ **AG 是超码 ✓**

**c. 正则覆盖:**
- D 在 $A \rightarrow BCD$ 中是无关的（$A \rightarrow BC$ 已能推导 D）
- D 在 $BC \rightarrow DE$ 中是无关的（$BC \rightarrow E$ 已够）

$$F_c = \{A \rightarrow BC, BC \rightarrow E, B \rightarrow D, D \rightarrow A\}$$

**d. 3NF 分解:**
$$\{\{A, B, C\}, \{B, C, E\}, \{B, D\}, \{D, A\}, \{A, G\}\}$$

（最后加入 $\{A, G\}$ 确保包含候选码 AG）

**e. BCNF 分解:**
$$\{\{A, B, C\}, \{B, D\}, \{A, E\}, \{A, G\}\}$$

---

### 7.31 — BCNF 分解与依赖保持 ★★★★★

**题目**: $R = (A, B, C, D, E, G)$, $F = \{AB \rightarrow CD, B \rightarrow D, DE \rightarrow B, DEG \rightarrow AB, AC \rightarrow DE\}$

$AB \rightarrow CD$ 说明 R 不是 BCNF。以它为起点做 BCNF 分解，判断结果是否依赖保持。

**解答**:

$AB \rightarrow CD$ 非平凡，且 $(AB)^+ = \{A, B, C, D, E\}$（不是全 R），故 AB 不是超码 ⇒ R 不满足 BCNF。

**BCNF 分解:**
$$\{\{A, B, G\}, \{A, B, E\}, \{A, B, C\}, \{B, D\}\}$$

**依赖保持判定**: **不保持 ✗**

$DE \rightarrow B$ 无法在单个关系上检查（D 和 E 分散在不同表中，没有哪张表同时包含 D、E、B）。

---

### 7.32 — 逐步 BCNF 分解 ★★★★★

**题目**: $R = (A, B, C, D, E, G)$, $F = \{A \rightarrow BC, BD \rightarrow E, CD \rightarrow AB\}$

a. 找出一个无无关属性的非平凡 FD（由 F 逻辑蕴含）。
b. 以 $A \rightarrow BC$ 为起点做 BCNF 分解。
c. 判断是否无损。
d. 判断是否依赖保持。

**解答**:

**a.** 这样的 FD **不存在**。理由：假设存在 $\alpha \rightarrow \beta$，则 $\beta$ 中任何属性 X 都可以被证明是无关的（因为 F 本身已经逻辑蕴含 $\alpha \rightarrow \beta$），矛盾。

**b. BCNF 分解步骤:**

1. 用 $A \rightarrow BC$ 分解：
   - $R_1 = \{A, D, E, G\}$，$R_2 = \{A, B, C\}$（BCNF ✓）

2. 在 $R_1$ 上：由增广+传递律得 $AD \rightarrow E$，分解得：
   - $R_3 = \{A, D, G\}$，$R_4 = \{A, D, E\}$

**最终:** $\{\{A, D, G\}, \{A, D, E\}, \{A, B, C\}\}$

**c. 无损:** BCNF 分解算法始终保证无损 ✓

**d. 依赖保持:** **不保持 ✗**。$BD \rightarrow E$ 不被保持（B 在 $\{A,B,C\}$ 中，D 和 E 在 $\{A,D,E\}$ 中，没有哪张表同时包含 B、D、E）。

---

### 7.33 — 综合 3NF 分解题 ★★★★★

**题目**: $R = (A, B, C, D, E, G)$, $F = \{AB \rightarrow CD, ADE \rightarrow GDE, B \rightarrow GC, G \rightarrow DE\}$

a. 列出所有候选码
b. 计算 Fc
c. 给出 3NF 分解步骤
d. 最终分解

**解答**:

**a. 候选码:**
- $(AB)^+ = \{A, B, C, D, E, G\} = R$
- $(A)^+ = \{A\} \neq R$，$(B)^+ = \{B, C, D, E, G\} \neq R$
- ∴ **AB 是唯一的候选码**

**b. 正则覆盖:**

| 步骤 | 操作 | Fc |
|------|------|-----|
| 初始 | — | $\{AB \rightarrow CD, ADE \rightarrow GDE, B \rightarrow GC, G \rightarrow DE\}$ |
| 1 | $AB \rightarrow CD$ 中 C 无关 (B→GC → B→C) | $\{AB \rightarrow D, ADE \rightarrow GDE, B \rightarrow GC, G \rightarrow DE\}$ |
| 2 | $ADE \rightarrow GDE$ 中 E 无关 | $\{AB \rightarrow D, ADE \rightarrow GD, B \rightarrow GC, G \rightarrow DE\}$ |
| 3 | $ADE \rightarrow GD$ 中 D 无关 | $\{AB \rightarrow D, ADE \rightarrow G, B \rightarrow GC, G \rightarrow DE\}$ |
| 4 | $AB \rightarrow D$ 可由 B→GC, G→DE 推出 → 删除 | $\{ADE \rightarrow G, B \rightarrow GC, G \rightarrow DE\}$ |

$$F_c = \{ADE \rightarrow G, B \rightarrow GC, G \rightarrow DE\}$$

**c. 3NF 分解步骤:**
- $R_1 = \{A, D, E, G\}$（来自 ADE → G）
- $R_2 = \{B, C, G\}$（来自 B → GC）
- $R_3 = \{D, E, G\}$（来自 G → DE）
- 无一包含候选码 AB → 加入 $R_4 = \{A, B\}$
- 删除 $R_3$（$R_3 \subseteq R_1$）

**d. 最终:**
$$\{\{A, D, E, G\}, \{B, C, G\}, \{A, B\}\}$$

---

### 7.34 — 更复杂的 3NF ★★★★★

**题目**: $R = (A, B, C, D, E, G, H)$, $F = \{AB \rightarrow CD, D \rightarrow C, DE \rightarrow B, DEH \rightarrow AB, AC \rightarrow DC\}$

a. 列出所有候选码
b. 计算 Fc
c. 3NF 分解步骤
d. 最终分解

**解答**:

**a. 候选码:** $\{A, B, E, G, H\}$ 和 $\{D, E, G, H\}$

**b. 正则覆盖:**

| 步骤 | 操作 | Fc |
|------|------|-----|
| 初始 | — | $\{AB \rightarrow CD, D \rightarrow C, DE \rightarrow B, DEH \rightarrow AB, AC \rightarrow DC\}$ |
| 1 | $AC \rightarrow DC$ 中 C 无关 | $\{AB \rightarrow CD, D \rightarrow C, DE \rightarrow B, DEH \rightarrow AB, AC \rightarrow D\}$ |
| 2 | $AB \rightarrow CD$ 中 C 无关 (AB→D ∧ D→C ⇒ AB→C) | $\{AB \rightarrow D, D \rightarrow C, DE \rightarrow B, DEH \rightarrow AB, AC \rightarrow D\}$ |
| 3 | $DEH \rightarrow AB$ 中 B 无关 (DE→B) | $\{AB \rightarrow D, D \rightarrow C, DE \rightarrow B, DEH \rightarrow A, AC \rightarrow D\}$ |

$$F_c = \{AB \rightarrow D, D \rightarrow C, DE \rightarrow B, DEH \rightarrow A, AC \rightarrow D\}$$

**c. 生成关系:**
- $R_1 = \{A, B, D\}$，$R_2 = \{C, D\}$，$R_3 = \{B, D, E\}$，$R_4 = \{A, D, E, H\}$，$R_5 = \{A, C, D\}$
- 无包含候选码的 → 加入 $R_6 = \{D, E, G, H\}$
- 删除 $R_2$（$R_2 \subseteq R_5$）

**d. 最终:**
$$\{\{A, B, D\}, \{B, D, E\}, \{A, D, E, H\}, \{A, C, D\}, \{D, E, G, H\}\}$$

---

### 7.36 — 每个两属性模式都是 BCNF ★★★★

**题目**: 证明任何恰好有两个属性的关系模式必定属于 BCNF，无论给定什么 FD。

**解答**:

设 $R = (A, B)$。假设 R 不满足 BCNF，则存在 $\alpha \rightarrow \beta$ 在 $F^+$ 中，非平凡且 $\alpha$ 不是超码。

可能的 $\alpha, \beta \in \{\{A\}, \{B\}, \{A,B\}\}$，逐一排查：

| $\alpha$ | $\beta$ | 为何不可能 |
|----------|---------|-----------|
| {A} | {A} | 平凡 ✗ |
| {A} | {B} | 则 A → AB，A 是超码 ✗ |
| {A} | {A,B} | A 是超码 ✗ |
| {B} | {A} | 则 B → AB，B 是超码 ✗ |
| {B} | {B} | 平凡 ✗ |
| {B} | {A,B} | B 是超码 ✗ |
| {A,B} | 任意 | {A,B} 总是超码 ✗ |

所有情况均导致矛盾。∴ **R 必定是 BCNF ✓**

---

### 7.40 — A →→ BC 是否蕴含 A →→ B？★★★★

**题目**: 给定 $r(A, B, C, D)$，$A \twoheadrightarrow BC$ 是否逻辑蕴含 $A \twoheadrightarrow B$ 和 $A \twoheadrightarrow C$？若成立则证明，否则给反例。

**解答**:

**不蕴含！✗**

反例 — 以下实例满足 $A \twoheadrightarrow BC$：

| A | B | C | D |
|---|---|---|---|
| 1 | 3 | 7 | 2 |
| 1 | 6 | 4 | 5 |
| 1 | 3 | 7 | 5 |
| 1 | 6 | 4 | 2 |

但 $A \twoheadrightarrow B$ **不成立**：取 $t_1 = (1,3,7,2)$, $t_2 = (1,6,4,5)$。

按 MVD 定义，应存在 $t_3$ 和 $t_4$ 使：
- $t_3[B] = t_1[B] = 3$，$t_3[R-B] = t_2[R-B] = (1, 4, 5)$ ⇒ $t_3 = (1, 3, 4, 5)$
- 但 $(1, 3, 4, 5)$ 不在实例中！

∴ **MVD 不像 FD 那样满足分解律** — $A \twoheadrightarrow BC$ 不能推出 $A \twoheadrightarrow B$。

---

### 7.41 — 为什么 4NF 比 BCNF 更好 ★★★★

**题目**: 解释为什么 4NF 是比 BCNF 更理想的范式。

**解答**:

4NF 能消除 BCNF **无法捕获**的冗余。

**教科书例子**: $r_2(ID, dept\_name, street, city)$

- 无 FD 违反 BCNF → 属于 BCNF
- 但存在 MVD：$ID \twoheadrightarrow dept\_name$（一个教师的多个系名和多个地址之间产生笛卡尔积冗余）
- 必须重复存储：对教师的每个地址重复系名，对每个系名重复地址
- **分解到 4NF**: $(ID, dept\_name)$ 和 $(ID, street, city)$ → 冗余消除

∴ **4NF ≥ BCNF**，消除 FD 和 MVD 引起的所有冗余。

---

### 7.42 — 分解到 4NF ★★★★

**题目**: 将以下模式规范化到 4NF。

**books(accessionno, isbn, title, author, publisher)**

约束:
- $accessionno \rightarrow isbn$
- $isbn \rightarrow title$
- $isbn \rightarrow publisher$
- $isbn \twoheadrightarrow author$

**users(userid, name, deptid, deptname)**

约束:
- $userid \rightarrow name$
- $userid \rightarrow deptid$
- $deptid \rightarrow deptname$
- （注：deptid →→ deptname 也是 MVD）

**解答**:

**books 分解:**

1. $isbn \twoheadrightarrow author$ 非平凡，isbn 不是超码 → 分解：
   - $b_1(isbn, author)$
   - $b_2(accessionno, isbn, title, publisher)$

2. 在 $b_2$ 上：$isbn \twoheadrightarrow title, publisher$ 非平凡 → 分解：
   - $b_{2.1}(accessionno, isbn)$
   - $b_{2.2}(isbn, title, publisher)$

**最终**: $\{b_1(isbn, author), b_{2.1}(accessionno, isbn), b_{2.2}(isbn, title, publisher)\}$

**users 分解:**

$deptid \twoheadrightarrow deptname$ 非平凡，deptid 不是超码 → 分解：
- $u_1(deptid, deptname)$
- $u_2(userid, name, deptid)$

**最终**: $\{u_1(deptid, deptname), u_2(userid, name, deptid)\}$

---

### 7.43 — 用物化视图强制执行 FD ★★

**题目**: 虽然 SQL 不支持 FD 约束，但如果数据库系统支持**物化视图**且立即维护，说明如何用物化视图强制执行 $B \rightarrow C$（在 $r(A, B, C)$ 上）。

**解答**:

创建物化视图：
```sql
CREATE MATERIALIZED VIEW fd_check AS
SELECT B, COUNT(DISTINCT C) AS X
FROM r
GROUP BY B;
```

加约束：$X \leq 1$（即每个 B 对应最多一个 C 值）。如果物化视图被立即维护，任何违反 $B \rightarrow C$ 的插入/更新都会导致约束违反。

---

### 7.44 — 时态自然连接 (Temporal Natural Join) ★

**题目**: 给定 $r(A, B, validtime)$ 和 $s(B, C, validtime)$，写 SQL 计算时态自然连接。可用 `&&` 判断区间重叠，`*` 计算区间交集。

**解答**:

```sql
SELECT A, r.B, r.validtime * s.validtime
FROM r INNER JOIN s
    ON (r.B = s.B
        AND r.validtime && s.validtime)
```

---

## 11. PRACTICE EXAM PROBLEM (练习考试题) ★★★★★

### 题目

已知: $R\langle U, F\rangle$, $U = \{A, B, C, D, E\}$, $F = \{AC \rightarrow BD, B \rightarrow C, C \rightarrow D, B \rightarrow E\}$

**a.** 证明 $AC \rightarrow E$

**b.** 计算 $(AB)^+$ 和 $(BC)^+$

**c.** 计算正则覆盖 $F_c$

**d.** 求候选码并判定范式

**e.** 给出保持依赖且无损连接的 3NF 分解

**f.** 证明分解的无损连接性

---

### 解答

**a. 证明 $AC \rightarrow E$:**

| 步骤 | 推导 | 依据 |
|------|------|------|
| 1 | $AC \rightarrow BD$ | 给定 |
| 2 | $AC \rightarrow B$ | 分解律 (步骤1) |
| 3 | $B \rightarrow E$ | 给定 |
| 4 | $AC \rightarrow E$ | 传递律 (步骤2 + 步骤3) ✓ |

**b. 属性闭包:**

- **$(AB)^+$**:
  - result = {A, B}
  - $B \rightarrow C$ ⇒ result = {A, B, C}
  - $B \rightarrow E$ ⇒ result = {A, B, C, E}
  - $C \rightarrow D$ ⇒ result = {A, B, C, D, E}
  - **$(AB)^+ = ABCDE$ = 全 U → AB 是超码!**

- **$(BC)^+$**:
  - result = {B, C}
  - $B \rightarrow E$ ⇒ result = {B, C, E}
  - $C \rightarrow D$ ⇒ result = {B, C, D, E}
  - **$(BC)^+ = BCDE$**（不含 A）

**c. 正则覆盖:**

| 步骤 | 操作 | 结果 |
|------|------|------|
| 初始 | — | $\{AC \rightarrow BD, B \rightarrow C, C \rightarrow D, B \rightarrow E\}$ |
| 1 | $AC \rightarrow BD$ 中 D 是无关的（AC→B, B→C, C→D ⇒ AC→D） | $\{AC \rightarrow B, B \rightarrow C, C \rightarrow D, B \rightarrow E\}$ |
| 2 | 合并 $B \rightarrow C$ 和 $B \rightarrow E$ | $\{AC \rightarrow B, B \rightarrow CE, C \rightarrow D\}$ |

$$F_c = \{AC \rightarrow B, B \rightarrow CE, C \rightarrow D\}$$

（可能有多解）

**d. 候选码和范式判定:**

- **候选码**: AC, AB
- **主属性**: A, B, C
- **非主属性**: D, E

检查范式：
- $C \rightarrow D$：非主属性 D 对候选码 AC 的**部分**函数依赖（C 是 AC 的真子集）
- ⇒ **违反 2NF**！
- ∴ **$R \in 1NF$**（不满足 2NF，更不满足 3NF/BCNF）

**e. 3NF 分解:**

基于 $F_c$:
- $U_1 = \{A, B, C\}$，$F_1 = \{AC \rightarrow B, B \rightarrow C\}$
- $U_2 = \{B, C, E\}$，$F_2 = \{B \rightarrow CE\}$
- $U_3 = \{C, D\}$，$F_3 = \{C \rightarrow D\}$

候选码 AC 和 AB 都含在 $U_1$ 中（$U_1$ 包含 A, B, C）。

$$\rho = \{R_1, R_2, R_3\}$$

该分解**保持函数依赖且无损连接** ✓

**f. 无损连接性证明:**

由于候选码 AC 和 AB 都包含在 $U_1 = \{A, B, C\}$ 中，通过 Chase 算法可验证存在一行变为全 a → 无损连接 ✓

（核心直觉：包含候选码的表能与所有其他表无损连接）

---

## 12. KEY FORMULAS AND ALGORITHMS SUMMARY (核心公式与算法速查)

### 属性闭包 $\alpha^+$

```
result := α
while (changed)
    for each β → γ in F
        if β ⊆ result then
            result := result ∪ γ
```

### 无损条件（两个关系）

$$R_1 \cap R_2 \rightarrow R_1 \in F^+ \;\; \text{OR} \;\; R_1 \cap R_2 \rightarrow R_2 \in F^+$$

### 范式判定速查表

| 范式 | 条件 | 消除的冗余 |
|------|------|-----------|
| **1NF** | 所有属性原子性（无多值属性、无嵌套关系） | 基本结构 |
| **2NF** | 1NF + 无非主属性对候选码的**部分**依赖 | 部分依赖冗余 |
| **3NF** | 2NF + 无非主属性对候选码的**传递**依赖；或：$\forall \alpha \rightarrow \beta$ (非平凡): $\alpha$ 是超码 OR $(\beta-\alpha)$ 中每个属性在某个候选码中 | 传递依赖冗余 |
| **BCNF** | $\forall \alpha \rightarrow \beta$ (非平凡): $\alpha$ 是超码 | FD 引起的所有冗余 |
| **4NF** | $\forall \alpha \twoheadrightarrow \beta$ (非平凡): $\alpha$ 是超码 | MVD 引起的所有冗余 |

### 设计目标优先级

```
1. BCNF (理想目标 — 最高范式)
2. 无损连接 (强制要求 — 必须满足)
3. 依赖保持 (期望目标 — 尽量满足)

若 1+2+3 不可能同时实现：
  → 选择 3NF (满足 2+3) 或 BCNF (满足 1+2)
```

### 核心算法速查

| 算法 | 输入 | 输出 | 关键步骤 |
|------|------|------|---------|
| **属性闭包** | $\alpha$, F | $\alpha^+$ | 迭代扩展直到不变 |
| **正则覆盖** | F | $F_c$ | 逐一删除无关属性 + 合并左部相同的 FD |
| **候选码** | R, F | 所有 K | 逐一测试每个子集：$K^+ = R$ 且 K 极小 |
| **BCNF 分解** | R, F | $\rho$ | 找违反 BCNF 的 FD → 二路分解 → 递归 |
| **3NF 分解** | R, $F_c$ | $\rho$ | 为 $F_c$ 中每个 FD 建表 + 确保含候选码 |
| **无损判定** | $\rho$, F | T/F | 二元：交集条件；多元：Chase 算法 |
| **依赖保持判定** | $\rho$, F | T/F | $(F_1 \cup \dots \cup F_n)^+ = F^+$ |

---

## 13. COMMON MISTAKES (常见错误 — 务必避免!)

| # | 错误 ❌ | 正确做法 ✅ |
|---|---------|------------|
| 1 | 对分解后的关系**只检查 F 中的 FD** | 必须检查 $F^+$ 中涉及该子关系属性的**所有** FD，或用属性闭包法 |
| 2 | 3NF 分解时**忘记加候选码表** | 3NF 算法必须确保某个关系包含候选码，否则**不是无损的** |
| 3 | **同时删除多个**无关属性 | 每次只能删除一个！删一个后另一个可能不再无关（见 7.15） |
| 4 | **混淆无损和依赖保持** | 两个完全独立的概念！都要分别检查 |
| 5 | 以为 BCNF **一定比 3NF 好** | BCNF 消除冗余但**不一定保持依赖**；需根据需求选择 |
| 6 | 在正则覆盖中使用 F 而非 $F_c$ 测试无关属性 | 测试无关属性必须使用**当前的** $F_c$！ |
| 7 | 计算属性闭包时**遗漏传递依赖** | 闭包算法是**迭代的**，必须一直循环到 result 不再变化为止 |
| 8 | 认为 MVD 和 FD 一样满足分解律 | MVD 不满足！$A \twoheadrightarrow BC \not\Rightarrow A \twoheadrightarrow B$（见 7.40） |
| 9 | 4NF 分解后忘记检查是否还需要继续 | 每次分解后都要重新检查每张子表是否 4NF |

---

### 复习提示

> **第 7 章是考试的重中之重！务必熟练掌握：**
>
> 1. 求候选码 &emsp; 2. 计算属性闭包 &emsp; 3. 求正则覆盖 &emsp; 4. 判断范式 (1NF/2NF/3NF/BCNF/4NF)
>
> 5. BCNF 分解 &emsp; 6. 3NF 分解 &emsp; 7. 判断无损连接和依赖保持
>
> **考试中必有一道规范化大题，通常占 15-20 分！**
