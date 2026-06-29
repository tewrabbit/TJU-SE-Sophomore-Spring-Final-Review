# Chapter 7: Relational Database Design / Normalization (关系数据库设计与规范化)
# 第七章 完整复习资料 — 中英双语版

---

## 1. FEATURES OF GOOD RELATIONAL DESIGN (好的关系设计的特征)

### 反面教材：组合模式的问题

将 `instructor` 和 `department` 合并成一张表 `in_dep` (natural join的结果) 会产生：

| 问题 | English | 中文解释 |
|------|---------|---------|
| **信息重复** | Repetition of information | dept_name, building, budget 对同系的每个教师都重复一遍 |
| **必须用NULL** | Need NULL values | 新增一个没有教师的系时，必须用NULL填充教师相关属性 |

### 解决方案：分解 (Decomposition)

将 `in_dep(ID, name, salary, dept_name, building, budget)` 分解为:
- `instructor(ID, name, salary, dept_name)` — dept_name是外码
- `department(dept_name, building, budget)`

### 无损分解 vs 有损分解 (Lossless vs Lossy)

| 类型 | 英文 | 定义 | 条件 |
|------|------|------|------|
| **无损连接分解** | Lossless Decomposition | 可以通过自然连接重建原始关系: Π_R₁(r) ⋈ Π_R₂(r) = r | R₁ ∩ R₂ → R₁ 或 R₁ ∩ R₂ → R₂ 属于 F⁺ |
| **有损分解** | Lossy Decomposition | 自然连接会产生多余的"伪元组" (spurious tuples)，信息丢失 | 上述条件不成立 |

**示例**: 
```
R = (A, B, C), F = {A → B, B → C}

✅ 好分解: R1=(A,B), R2=(B,C)
   交集={B}, B → BC ✓ → 无损

❌ 坏分解: R1=(A,B), R2=(A,C)  
   交集={A}, A → AB ✓ BUT B→C无法检查 → 有依赖丢失
```

### 三大设计目标 (Three Design Goals)

| 优先级 | 目标 | 说明 |
|--------|------|------|
| 1 | **BCNF** | 尽可能高的范式 |
| 2 | **无损连接 (Lossless Join)** | 强制要求 — 不能丢失信息 |
| 3 | **依赖保持 (Dependency Preservation)** | 期望达到 — 可以在单表上检查FD |

如果三者无法同时满足，接受以下之一：
- 缺少依赖保持 (ONLY BCNF with lossless)
- 使用3NF容忍一定冗余 (保证lossless + dependency preserving)

---

## 2. FUNCTIONAL DEPENDENCIES (函数依赖)

### 函数依赖的定义 (Definition)

设R为关系模式，α ⊆ R，β ⊆ R。**α → β** 在R上成立当且仅当:
> 对任何合法的关系r(R)，只要任意两个元组 t₁ 和 t₂ 在 α 上取值相同，它们在 β 上也取值相同。
> **t₁[α] = t₂[α] ⇒ t₁[β] = t₂[β]**

### 关键概念

| 概念 | English | 定义 |
|------|---------|------|
| **平凡FD** | Trivial FD | β ⊆ α（总是成立，如 ID, name → ID） |
| **非平凡FD** | Nontrivial FD | β ⊈ α（有意义的约束） |
| **超码判定** | Superkey test | K 是超码 ⇔ K → R |
| **候选码判定** | Candidate key test | K → R 且 不存在真子集 α ⊂ K 满足 α → R |

### 用FD表达联系基数

| 联系类型 | FD表示 |
|---------|--------|
| **一对一 (1:1)** | PK(student) → PK(instructor) AND PK(instructor) → PK(student) |
| **多对一 (N:1)** | PK(student) → PK(instructor) （学生确定导师，反之不行） |

---

## 3. ARMSTRONG'S AXIOMS AND CLOSURE (阿姆斯特朗公理与闭包)

### 阿姆斯特朗公理 (Armstrong's Axioms)

三条**正确(Sound)**且**完备(Complete)**的推理规则:

| 公理 | English | 规则 | 中文 |
|------|---------|------|------|
| **自反律** | Reflexivity | 若 β ⊆ α，则 α → β | 子集被包含集合决定 |
| **增广律** | Augmentation | 若 α → β，则 γα → γβ | 两边加同样属性仍成立 |
| **传递律** | Transitivity | 若 α → β 且 β → γ，则 α → γ | 链式传递 |

### 附加(可推导)规则

| 规则 | 规则内容 | 中文 |
|------|---------|------|
| **合并律 (Union)** | α → β 且 α → γ ⇒ α → βγ | 相同左边可合并 |
| **分解律 (Decomposition)** | α → βγ ⇒ α → β 且 α → γ | 右边可拆分 |
| **伪传递律 (Pseudotransitivity)** | α → β 且 γβ → δ ⇒ αγ → δ | |

### 函数依赖的闭包 F⁺
- **定义**: 由F**逻辑蕴含**的所有FD的集合
- 反复应用阿姆斯特朗公理直到不再变化即可求出F⁺

### 属性集闭包 α⁺ (Attribute Set Closure)

**定义**: 在F下由α函数确定的所有属性的集合

**算法**:
```
result := α
while (result发生变化) do
    for each β → γ in F do
        if β ⊆ result then result := result ∪ γ
return result
```

**属性闭包的用途 (三大用途)**:
1. **测试超码**: α 是超码 ⇔ α⁺ 包含R的所有属性
2. **测试函数依赖**: α → β 成立 ⇔ β ⊆ α⁺
3. **计算F⁺**: 对每个 γ ⊆ R，计算 γ⁺，输出 γ → S (其中 S ⊆ γ⁺)

### 属性闭包计算示例 (重要！)

```
R = (A, B, C, G, H, I)
F = {A → B, A → C, CG → H, CG → I, B → H}

计算 (AG)⁺:
步骤1: result = AG
步骤2: result = ABCG      (因为 A→C 和 A→B, A⊆AG)
步骤3: result = ABCGH     (因为 CG→H, CG⊆ABCG)
步骤4: result = ABCGHI    (因为 CG→I, CG⊆ABCGH)

判断: AG是候选码吗?
- (AG)⁺ = R = ABCGHI ✓  (是超码)
- A⁺ = ABCGHI? 否, A⁺ = ABCHI (缺少G) → A不是超码
- G⁺ = G → 不是超码
→ AG是候选码 ✓
```

---

## 4. CANONICAL COVER (正则覆盖 / 最小函数依赖集)

### 什么是正则覆盖？

**Fc** 是F的正则覆盖当且仅当:
1. F 逻辑蕴含 Fc 中的所有依赖
2. Fc 逻辑蕴含 F 中的所有依赖
3. Fc 中**没有FD包含无关属性**(extraneous attribute)
4. Fc 中每个FD的**左边是唯一的**

**为什么要用正则覆盖?** 简化约束检查——数据库系统每次更新只需检查更少的依赖。

### 无关属性 (Extraneous Attributes)

一个属性A在 FD α → β 中是**无关的(extraneous)**，如果移除它不会改变 F⁺：

**左侧无关属性(移除使依赖变强)**:
- A ∈ α 是无关的，如果: F 逻辑蕴含 (F − {α→β}) ∪ {(α−A)→β}
- **测试方法**: 计算 (α − {A})⁺（使用F），如果包含 β，则A是无关的

**右侧无关属性(移除使依赖变弱)**:
- A ∈ β 是无关的，如果: (F − {α→β}) ∪ {α→(β−A)} 逻辑蕴含 F
- **测试方法**: 使用 F' = (F − {α→β}) ∪ {α→(β−A)} 计算 α⁺，如果包含 A，则A是无关的

### 计算正则覆盖的算法

```
Fc = F
repeat
    // 步骤1: 合并律 — 合并相同左侧
    将 α→β₁ 和 α→β₂ 替换为 α→β₁β₂
    // 步骤2: 删除无关属性
    找到Fc中有无关属性的FD
    删除该无关属性
until Fc不再变化
```

### 正则覆盖计算完整示例

```
F = {A → BC, B → C, A → B, AB → C}

第1步: 合并 A→BC 和 A→B → A→BC
  当前: {A → BC, B → C, AB → C}

第2步: 检查 AB→C, A是无关的吗?
  计算 B⁺ (使用 F' = {A→BC, B→C})
  B⁺ = {B, C} — 包含 C ✓
  所以A是无关的，删除A
  当前: {A → BC, B → C}

第3步: 检查 A→BC, C是无关的吗?
  计算 A⁺ (使用 F' = {A→B, B→C})
  A⁺ = {A, B, C} — 包含 C ✓
  所以C是无关的，删除C
  当前: {A → B, B → C}  ← 最终正则覆盖
```

### 重要注意点

- **不能同时删除多个无关属性** — 删除一个后，另一个可能不再无关
- 正则覆盖**可能不唯一**
- 合并律可能需要反复应用

---

## 5. NORMAL FORMS (范式完整详解)

---

### First Normal Form — 1NF (第一范式)

**定义**: 关系模式R的所有属性的**域都是原子的(atomic)** — 即属性值不可再分。

**不允许**: 集合、复合属性、表中套表

**我们假设所有关系都满足1NF** (虽然第8章会放松此假设)。

---

### Boyce-Codd Normal Form — BCNF (BC范式)

**定义**: 关系R属于BCNF，当且仅当对 F⁺ 中所有的 α → β (非平凡)，**α 是R的超码**。

**通俗理解**: 每个有意义的函数依赖，其决定方必须是超码。即**只有码才能决定其他属性**。

**BCNF的判定方法**:
1. 对 F⁺ 中每个非平凡FD α→β
2. 计算 α⁺
3. 检查 α⁺ 是否包含R的所有属性
4. 如果任何一个不包含 → 违反BCNF

**BCNF违反示例**:
```
in_dep(ID, name, salary, dept_name, building, budget)
FD: dept_name → building, budget
dept_name⁺ = {dept_name, building, budget} ≠ R
→ dept_name 不是超码 → 违反BCNF!
```

---

### Third Normal Form — 3NF (第三范式)

**定义**: 关系R属于3NF，当且仅当对 F⁺ 中所有的 α → β (非平凡)，**至少满足以下之一**:
1. α → β 是平凡的 (β ⊆ α)
2. **α 是R的超码**
3. **β−α 中的每个属性都包含在R的某个候选码中** ← 这是对BCNF的放松

**通俗理解**: 在BCNF基础上，允许"非码→码属性"这样的依赖（但不能是"非码→非码"）。

**3NF违反示例**:
```
R = (A, B, C), F = {A → B, B → C}
候选码: A
B → C: B不是超码，且C不在任何候选码中 → 违反3NF!
```

**3NF满足但BCNF违反的经典示例**:
```
dept_advisor(s_ID, i_ID, dept_name)
FDs: i_ID → dept_name, s_ID, dept_name → i_ID
候选码: {s_ID, dept_name}, {s_ID, i_ID}

检查 i_ID → dept_name:
  - 非平凡 ✓
  - i_ID 不是超码 ✓ (违反BCNF!)
  - 但: dept_name − i_ID = {dept_name}，而 dept_name 在候选码 {s_ID, dept_name} 中 ✓
→ 满足3NF（不满足BCNF）
```

---

### 范式对比表 (Normal Form Comparison)

| 特征 | BCNF | 3NF | 2NF | 1NF |
|------|------|-----|-----|-----|
| **无损分解** | 总是能 | 总是能 | 总是能 | — |
| **依赖保持** | 不总能 | 总是能 | 总是能 | — |
| **冗余** | 无 | 可能有 | 有 | 有 |
| **NULL问题** | 不需要 | 可能需要 | 可能需要 | 可能需要 |

### 范式层次结构 (Normal Form Hierarchy)
```
1NF ⊃ 2NF ⊃ 3NF ⊃ BCNF ⊃ 4NF ⊃ 5NF
(外层)                        (内层，更严格)
```
每个更高范式自动满足低范式。

---

## 6. DECOMPOSITION ALGORITHMS (分解算法详解)

### BCNF分解算法

```
result := {R}
done := false
compute F⁺
while (not done) do
    if (存在result中的模式Rᵢ不满足BCNF) then
        设 α→β 是Rᵢ上的非平凡FD，满足:
            α→Rᵢ 不在F⁺中 (即α不是Rᵢ的超码)
            且 α ∩ β = ∅
        result := (result − Rᵢ) ∪ (Rᵢ − β) ∪ (α, β)
    else done := true
```

**⚠️ 重要陷阱**: 对分解后的子关系测试BCNF时，**不能只用F中的FD**！必须用F⁺中只包含该子关系属性的所有FD，或者用属性闭包方法。

**对子关系Rᵢ的BCNF测试**:
- 对每个 α ⊆ Rᵢ，检查 α⁺ 要么不包含 Rᵢ−α 的任何属性，要么包含 Rᵢ 的全部属性
- 如果违反，违规的FD是: α → (α⁺ − α) ∩ Rᵢ

### BCNF分解完整示例

```
R = class(course_id, title, dept_name, credits, sec_id, semester, year,
          building, room_number, capacity, time_slot_id)

FDs:
  course_id → title, dept_name, credits
  building, room_number → capacity
  course_id, sec_id, semester, year → building, room_number, time_slot_id

候选码: {course_id, sec_id, semester, year}

第1步: course_id → title, dept_name, credits 违反BCNF
  course_id不是超码
  → 分解: course(course_id, title, dept_name, credits)  ✅ BCNF
          class-1(course_id, sec_id, semester, year, building, room_number, capacity, time_slot_id)

第2步: building, room_number → capacity 违反BCNF
  → 分解: classroom(building, room_number, capacity)  ✅ BCNF
          section(course_id, sec_id, semester, year, building, room_number, time_slot_id)  ✅ BCNF

最终: {course, classroom, section} — 全部BCNF, 无损
```

### 3NF分解算法 (保证无损+依赖保持!)

```
设 Fc 为F的正则覆盖
i := 0
for each FD α→β in Fc do
    if 所有已有Rⱼ都不包含 αβ then
        i := i + 1
        Rᵢ := αβ    -- 为该FD创建一个关系
if 所有Rⱼ都不包含R的某个候选码 then
    i := i + 1
    Rᵢ := 任意一个候选码  -- 必须加一个含候选码的关系！
// 可选: 删除被其他关系包含的冗余关系
repeat
    if 某个Rⱼ ⊆ 另一个Rₖ then
        删除Rⱼ
return (R₁, R₂, ..., Rᵢ)
```

### 3NF分解完整示例

```
cust_banker_branch = (customer_id, employee_id, branch_name, type)
FDs:
  1. customer_id, employee_id → branch_name, type
  2. employee_id → branch_name
  3. customer_id, branch_name → employee_id

计算正则覆盖:
  FD1中branch_name是右侧无关属性 (可由 employee_id→branch_name 得出)
  Fc = {customer_id, employee_id → type,
        employee_id → branch_name,
        customer_id, branch_name → employee_id}

生成表:
  R1 = (customer_id, employee_id, type)       -- 来自FD1
  R2 = (employee_id, branch_name)             -- 来自FD2
  R3 = (customer_id, branch_name, employee_id) -- 来自FD3

检查候选码: R1包含候选码 → 无需额外添加
删除冗余: R2 ⊆ R3 → 删R2

最终: {(customer_id, employee_id, type), (customer_id, branch_name, employee_id)}
```

---

## 7. LOSSLESS JOIN AND DEPENDENCY PRESERVATION (无损连接与依赖保持)

### 无损连接判定

**对于分解为两个关系**: R₁ ∩ R₂ → R₁ 或 R₁ ∩ R₂ → R₂ 在 F⁺ 中即可。

**通用判定(Chase算法)**:
```
R = (A, B, C), F = {A → B, B → C}

分解1: R1=(A,B), R2=(B,C)
  R1 ∩ R2 = {B}, B → BC → 无损 ✓, 且依赖保持 ✓

分解2: R1=(A,B), R2=(A,C)  
  R1 ∩ R2 = {A}, A → AB → 无损 ✓
  但: B → C 无法在单表上检查 → NOT 依赖保持 ✗
```

### 依赖保持的判定

设 Fᵢ 为 F⁺ 在 Rᵢ 上的限制 (只包含Rᵢ中属性的FD)。

**依赖保持**: (F₁ ∪ F₂ ∪ ... ∪ Fₙ)⁺ = F⁺

**测试方法**: 对F中的每个FD α→β:
```
result := α
repeat
    for each Rᵢ in decomposition
        t = (result ∩ Rᵢ)⁺ ∩ Rᵢ
        result = result ∪ t
until result不再变化
如果result包含β → 该FD被保持
```

---

## 8. MULTIVALUED DEPENDENCIES (多值依赖 MVD)

### 多值依赖的定义

设 R 为关系模式，α ⊆ R，β ⊆ R。**α →→ β** 在R上成立当且仅当:
> 在任何合法关系 r(R) 中，对所有满足 t₁[α] = t₂[α] 的元组对 t₁, t₂，存在元组 t₃ 和 t₄ 使得:
> - t₁[α] = t₂[α] = t₃[α] = t₄[α]
> - t₃[β] = t₁[β], t₃[R−β] = t₂[R−β]
> - t₄[β] = t₂[β], t₄[R−β] = t₁[R−β]

### 等价定义 (Y, Z, W分区)

将属性分为三个非空子集 Y, Z, W，则 **Y →→ Z** 当且仅当:
> 若 ⟨y, z₁, w₁⟩ 和 ⟨y, z₂, w₂⟩ 在r中，则 ⟨y, z₁, w₂⟩ 和 ⟨y, z₂, w₁⟩ 也必须在r中。

**含义**: 给定Y值，Z的取值集合与W的取值集合**相互独立**。

**对称性**: Y →→ Z ⇔ Y →→ W

### 关键性质: FD是MVD的特例
若 α → β，则 α →→ β （当 z₁=z₂ 时交换就是原地操作）

### MVD的经典示例 (BCNF但非4NF!)

```
inst_info(ID, child_name, phone_number)
数据 (冗余示例):
  (99999, David,   512-555-1234)
  (99999, David,   512-555-4321)
  (99999, William, 512-555-1234)
  (99999, William, 512-555-4321)

MVD: ID →→ child_name   (孩子的集合与电话的集合独立)
MVD: ID →→ phone_number

该关系没有非平凡FD → 属于BCNF!
但有明显冗余 → 不满足4NF
```

---

## 9. FOURTH NORMAL FORM (第四范式 4NF)

### 4NF定义

关系R属于**4NF**当且仅当对所有 D⁺ 中的 α →→ β (非平凡, α ⊆ R, β ⊆ R):
1. α →→ β 是平凡的 (β ⊆ α 或 α ∪ β = R)，或
2. **α 是R的超码**

**注意**: 4NF ⇒ BCNF (但反之不成立)

### 4NF分解算法

```
result := {R}
done := false
compute D⁺
while (not done) do
    if (存在Rᵢ不满足4NF) then
        设 α→→β 是Rᵢ上的非平凡MVD, α不是超码, α∩β=∅
        result := (result − Rᵢ) ∪ (Rᵢ − β) ∪ (α, β)
    else done := true
```

### 4NF分解示例

```
R = (A, B, C, G, H, I)
D = {A →→ B, B →→ HI, CG →→ H}

1. A→→B: A不是超码 → 分解
   R1 = (A, B)           ← 4NF ✓
   R2 = (A, C, G, H, I)  ← 非4NF

2. A→→HI (由A→→B和B→→HI传递): 
   A→→I (限制到R2)
   分解R2:
   R3 = (C, G, H)        ← 4NF ✓
   R4 = (A, C, G, I)     ← 非4NF

3. A→→I: A不是超码 → 分解R4
   R5 = (A, I)           ← 4NF ✓
   R6 = (A, C, G)        ← 4NF ✓

最终: {(A,B), (C,G,H), (A,I), (A,C,G)}
```

### 更高范式 (了解即可)
- **PJNF (5NF)** — 基于连接依赖 (Join Dependencies)
- **DKNF** — 域-码范式
- 实际中很少使用（推理困难，没有完备的推理规则集）

---

## 10. COMPLETE EXERCISE SOLUTIONS (课后习题全解)

### 7.1 — 无损分解判定

**题目**: R=(A,B,C,D,E), F={A→BC, CD→E, B→D, E→A}，分解为 (A,B,C) 和 (A,D,E)，判断是否无损。

**解答**: 
(A,B,C) ∩ (A,D,E) = {A}
由习题7.6知A是候选码，所以 A → ABC
→ 无损 ✓

### 7.2 — 从实例列出非平凡FD

**解答**: 从图7.17，非平凡FD为: `A→B`, `C→B`, `AC→B`

### 7.3 — FD表达联系基数

**一对一**: PK(student)→PK(instructor) AND PK(instructor)→PK(student)
**多对一**: PK(student)→PK(instructor) (学生确定唯一的导师)

### 7.4 — 证明合并律

**证明**:
1. α→β (给定)
2. αα→αβ (增广律) → α→αβ
3. α→γ (给定)
4. αβ→γβ (增广律) → αβ→βγ
5. α→βγ (传递律: α→αβ, αβ→βγ) ✓

### 7.5 — 证明伪传递律

**证明**:
1. α→β (给定)
2. αγ→γβ (增广律)
3. γβ→δ (给定)
4. αγ→δ (传递律) ✓

### 7.6 — 计算F⁺和候选码

**题目**: R=(A,B,C,D,E), F={A→BC, CD→E, B→D, E→A}

**求解候选码**:
- A→BC → A→B, A→C。加B→D → A→D。加A→CD和CD→E → A→E。
  所以 A→ABCDE → **A是候选码** ✓
- E→A 且 A→ABCDE → **E是候选码** ✓
- CD→E 且 E→ABCDE → **CD是候选码** ✓
- B→D, BC→CD, CD→E → BC→ABCDE → **BC是候选码** ✓
- **候选码: A, BC, CD, E**

### 7.7 — 正则覆盖

对于上述F，没有FD包含无关属性。**Fc = F** 本身。

### 7.8 — 高效闭包算法

图7.18的算法复杂度为O(n)，而图7.8是O(n²)。通过学习每个FD是否"可用"来避免重复扫描。

### 7.9 — SQL测试FD

```sql
-- B→C是否成立? 以下查询非空则FD不成立
SELECT B FROM r GROUP BY B HAVING COUNT(DISTINCT C) > 1;

-- 用ASSERTION强制执行
CREATE ASSERTION b_to_c CHECK (
    NOT EXISTS (
        SELECT B FROM r GROUP BY B HAVING COUNT(DISTINCT C) > 1
    )
);
```

### 7.10 — NULL值对无损连接的影响

FD左侧属性有NULL值时，自然连接会排除含NULL的元组（NULL≠NULL），导致分解变成有损。

### 7.11 — BCNF分解中的主码/外码

r(α,β,γ) 分解为 r1(α,β) 和 r2(α,γ)，使用 α→β:
- α是r1的主码，r2中引用r1的外码
- 外码约束对保持数据一致性至关重要

### 7.13 — 证明不保持依赖

分解 (A,B,C) 和 (A,D,E): B→D 不能被保持。
- F₁只有涉及{A,B,C}的FD
- F₂只有涉及{A,D,E}的FD
- B→D需要B→α在F₁⁺中且α→D在F₂⁺中
- α必须是A (交集)，但B→A不在F₁⁺中 (B⁺={B,D})
- 因此B→D不被保持

### 7.14 — 正则覆盖不唯一

F={X→YZ, Y→XZ, Z→XY} 有两种不同的正则覆盖:
- 删除不同顺序的无关属性得到不同结果
- Fc₁ = {X→Y, Y→Z, Z→X}
- Fc₂ = {X→Z, Y→X, Z→Y}

### 7.15 — 为什么一次只能删一个无关属性

在 X→YZ 中，Y和Z都看起来是无关的。但如果同时删除两者，得到的新集合无法再推导出 X→YZ。删除Y后Z不再无关，删除Z后Y不再无关。算法一次只删一个以避免此问题。

### 7.18 — 3NF的等价定义

教科书上的3NF定义(关于超码或候选码属性的条件) 等价于 "不存在非主属性对码的传递函数依赖"。

### 7.19 — 3NF ⇒ 2NF 的证明

每个部分函数依赖也是传递函数依赖。如果3NF消除了所有传递依赖，自然也就消除了所有部分依赖。

### 7.20 — BCNF但不满足4NF的示例

R=(A,B,C) 只有MVD A→→B。无FD违反BCNF，但A不是超码 → 不满足4NF。

### 7.25 — 阿姆斯特朗公理的正确性证明

- **自反律**: 若β⊆α，则任何两个在α上相同的元组在β的子集上也相同
- **增广律**: 若α→β且添加γ，在γα上相同的元组在γ和β上都相同
- **传递律**: 若α→β且β→γ，则α相同的元组在β上相同，进而在γ上相同

### 7.26 — 附加规则可从阿姆斯特朗公理推导

通过组合基本三条公理可以推导出合并律、分解律、伪传递律。

### 7.27 — 证明分解律

若α→βγ，由自反律βγ→β和βγ→γ，由传递律α→β和α→γ。✓

### 7.28 — 计算 B⁺

使用7.6的FD: B⁺ = {B, D}

### 7.29 — 有损的BCNF分解示例

R=(A,B,C,D,E), F={A→BC, CD→E, B→D, E→A}
分解为 (A,B,C) 和 (C,D,E): 交集={C}, C⁺={C} → 有损!

### 7.30 — 综合题

R=(A,B,C,D,E,G), F={A→BCD, BC→DE, B→D, D→A}
- a. B⁺ = {A,B,C,D,E}
- b. AG是超码: A→BCDE, AG→ABCDEG = R ✓
- c. Fc = {A→BC, BC→E, B→D, D→A}
- d. 3NF分解: {{A,B,C}, {B,C,E}, {B,D}, {D,A}, {A,G}}
- e. BCNF分解: {{A,B,C}, {B,D}, {A,E}, {A,G}}

### 7.31 — BCNF分解与依赖保持

R=(A,B,C,D,E,G), F={AB→CD, B→D, DE→B, DEG→AB, AC→DE}
BCNF结果: {{A,B,G}, {A,B,E}, {A,B,C}, {B,D}}
NOT dependency preserving (DE→B无法检查)

### 7.32 — 逐步BCNF分解

R=(A,B,C,D,E,G), F={A→BC, BD→E, CD→AB}
最终: {{A,D,G}, {A,D,E}, {A,B,C}}
无损 ✓ 但依赖不保持 ✗

### 7.33 — 综合3NF分解题

R=(A,B,C,D,E,G), F={AB→CD, ADE→GDE, B→GC, G→DE}
- a. 唯一候选码: AB
- b. Fc = {ADE→G, B→GC, G→DE}
- c. 3NF分解: {{A,D,E,G}, {B,C,G}, {A,B}}

### 7.34 — 更复杂的3NF

R=(A,B,C,D,E,G,H), F={AB→CD, D→C, DE→B, DEH→AB, AC→DC}
Fc = {AB→D, D→C, DE→B, DEH→A, AC→D}
候选码: {A,B,E,G,H}, {D,E,G,H}
最终: {{A,B,D}, {B,D,E}, {A,D,E,H}, {A,C,D}, {D,E,G,H}}

### 7.36 — 每个两属性模式都是BCNF

证明: 对R=(A,B)，可能左侧{ A }, { B }, { A,B }。
- A→B: 则A→AB (A是超码) ✓
- B→A: 则B→AB (B是超码) ✓
- {A,B}总是超码 ✓
不可能违反BCNF。

### 7.40 — A→→BC 是否蕴含 A→→B?

**否!** 反例:
```
| A | B | C | D |
| 1 | 3 | 7 | 2 |
| 1 | 6 | 4 | 5 |
| 1 | 3 | 7 | 5 |
| 1 | 6 | 4 | 2 |
```
A→→BC成立，但A→→B不成立（缺少元组(1,3,4,5)）。

### 7.41 — 为什么4NF比BCNF更好

4NF消除了BCNF无法捕获的多值依赖引起的冗余。

### 7.42 — 分解到4NF

**books(accessionno, isbn, title, author, publisher)**:
- isbn→→author → 分解为 b1(isbn, author) 和 b2(accessionno, isbn, title, publisher)
- b2中 isbn→→title，publisher → 再分解为 b2.1(accessionno, isbn) 和 b2.2(isbn, title, publisher)
- 最终: b1(isbn, author), b2.1(accessionno, isbn), b2.2(isbn, title, publisher)

**users(userid, name, deptid, deptname)**:
- deptid→→deptname → 分解为 u1(deptid, deptname) 和 u2(userid, name, deptid)

### 7.43 — 用物化视图强制执行FD

```sql
CREATE MATERIALIZED VIEW fd_check AS
SELECT B, COUNT(DISTINCT C) AS X FROM r GROUP BY B;
-- 约束: X <= 1
```

### 7.44 — 时态自然连接(Temporal Natural Join)

```sql
SELECT A, r.B, r.validtime * s.validtime
FROM r INNER JOIN s
ON (r.B = s.B AND r.validtime && s.validtime)
```

---

## 11. PRACTICE EXAM PROBLEM (练习考试题 — 来自参考试卷)

### 规范化综合题

**已知**: R<U, F>, U={A, B, C, D, E}, F={AC→BD, B→C, C→D, B→E}

**a. 证明 AC→E**:
```
AC→BD (给定)
→ AC→B (分解律)
B→E (给定)
→ AC→E (传递律) ✓
```

**b. 计算 (AB)⁺ 和 (BC)⁺**:
- (AB)⁺ = ABCDE (全部属性 → 是超码!)
- (BC)⁺ = BCDE

**c. 计算正则覆盖 Fc**:
```
最小依赖集 Fc = {AC→B, B→C, C→D, B→E}
(可能存在多种答案)
```

**d. 候选码和范式判定**:
- 候选码: AC, AB
- 主属性: A, B, C
- 非主属性: D, E
- C→D: 非主属性D对候选码AC**部分函数依赖** → 违反2NF
- **R ∈ 1NF** (不满足2NF)

**e. 3NF分解**:
```
U₁ = {A,B,C}, F₁ = {AC→B, B→C}
U₂ = {B,C,E}, F₂ = {B→C, B→E}
U₃ = {C,D},   F₃ = {C→D}
ρ = {R₁, R₂, R₃} 为保持函数依赖且无损连接的分解
```

**f. 证明无损连接性**:
由于候选码 AC和AB 都包含在 U₁ 中，通过检测算法（Chase），可以找到一行变为全a → 无损连接 ✓

---

## 12. KEY FORMULAS AND ALGORITHMS SUMMARY (核心公式与算法速查)

### 属性闭包 α⁺
```
result := α
while (changed)
    for each β→γ in F
        if β ⊆ result then result := result ∪ γ
```

### 无损条件 (两个关系)
```
R₁ ∩ R₂ → R₁ ∈ F⁺  OR  R₁ ∩ R₂ → R₂ ∈ F⁺
```

### 范式判定速查
```
1NF: 所有属性原子性
2NF: 1NF + 无非主属性对候选码的部分依赖
3NF: 2NF + 无非主属性对候选码的传递依赖
      教科书定义: ∀α→β (非平凡): α是超码 OR (β−α)中每个属性在某个候选码中
BCNF: ∀α→β (非平凡): α是超码
4NF: ∀α→→β (非平凡): α是超码
```

### 设计目标优先级
```
1. BCNF (理想目标)
2. 无损连接 (强制要求 — 必须满足)
3. 依赖保持 (期望目标)
4. 若1+2+3不可能 → 选择: 3NF(满足2+3) 或 BCNF(满足2)
```

---

## 13. COMMON MISTAKES (常见错误 — 务必避免!)

| 错误 | 正确做法 |
|------|---------|
| **对分解后的关系只检查F中的FD** | 必须检查F⁺中涉及该子关系属性的所有FD，或用属性闭包法 |
| **3NF分解时忘记加候选码表** | 3NF算法必须确保某个关系包含候选码，否则不是无损的 |
| **同时删除多个无关属性** | 每次只能删除一个! 删一个后另一个可能不再无关 |
| **混淆无损和依赖保持** | 两个完全独立的概念! 都要分别检查 |
| **以为BCNF一定比3NF好** | BCNF消除冗余但不一定保持依赖; 看需求选择 |
| **在正则覆盖中使用F而非Fc测试无关属性** | 测试无关属性必须使用当前的Fc! |
| **计算属性闭包时遗漏传递依赖** | 闭包算法是迭代的，必须一直循环到不变为止 |

---

> **复习提示**: 第7章是考试的重中之重！务必熟练掌握:
> 1. 求候选码 / 2. 计算属性闭包 / 3. 求正则覆盖 / 4. 判断范式 (1NF/2NF/3NF/BCNF) / 5. BCNF分解 / 6. 3NF分解 / 7. 判断无损连接和依赖保持
> 考试中必有一道规范化大题，通常占15-20分。
