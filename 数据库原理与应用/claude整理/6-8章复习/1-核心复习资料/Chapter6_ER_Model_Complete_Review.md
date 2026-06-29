# Chapter 6: Database Design Using the E-R Model (使用E-R模型进行数据库设计)
# 第六章 完整复习资料 — 中英双语版

---

## 1. DESIGN PROCESS OVERVIEW (设计过程概述)

### Design Phases (设计阶段)

| Phase (阶段) | Task (任务) | 中文说明 |
|------|-------|------|
| **Initial Phase** (初始阶段) | Characterize data needs of database users | 全面刻画数据库用户的数据需求 |
| **Second Phase** (第二阶段) | Choose a data model; apply its concepts; translate into conceptual schema | 选择数据模型，应用模型概念，转化为概念模式 |
| **Final Phase** (最终阶段) | Move from abstract to implementation | 从抽象模型转化为数据库实现 |

**最终阶段包含两个子步骤：**
- **Logical Design (逻辑设计)** — 决定数据库模式（database schema），找到"好的"关系模式集合。这是商业决策（记录哪些属性？）和计算机科学决策（怎样的关系模式？属性如何分布？）
- **Physical Design (物理设计)** — 决定数据库的物理布局

### Two Major Design Pitfalls (两大设计陷阱)

| Pitfall (陷阱) | English Explanation | 中文解释 |
|---------|-------------------|------|
| **Redundancy** (冗余) | Repeating information leads to data inconsistency among copies | 信息重复存储，导致各副本之间数据不一致 |
| **Incompleteness** (不完整) | Bad design makes certain aspects difficult or impossible to model | 糟糕的设计使某些方面难以甚至无法建模 |

### Two Design Approaches (两种设计方法)
- **ER Model (E-R模型)** (本章): 将企业建模为实体(entities)和联系(relationships)的集合
- **Normalization Theory (规范化理论)** (第7章): 形式化地定义什么是坏设计，并测试之

---

## 2. THE ENTITY-RELATIONSHIP MODEL (实体-联系模型基础)

### Three Basic Concepts (三个基本概念)
1. **Entity Sets (实体集)**
2. **Relationship Sets (联系集)**
3. **Attributes (属性)**

### Entity Sets (实体集)

**实体(Entity)**: 企业中存在且可与其他对象区分的"事物"或"对象"
- 例: 特定的人、公司、事件、植物

**实体集(Entity Set)**: 具有相同类型、共享相同属性的实体集合
- 例: 所有人的集合、所有公司的集合

**属性(Attribute)**: 实体集中所有成员具有的描述性属性
- 例: `instructor = (ID, name, salary)`, `course = (course_id, title, credits)`

**主码(Primary Key)**: 属性子集，能够唯一标识实体集中的每个实体

**E-R图中的表示:**
- **矩形(Rectangle)** 表示实体集
- 属性列在矩形内部
- **下划线(Underline)** 表示主码属性

### Relationship Sets (联系集)

**联系(Relationship)**: 多个实体之间的关联

**联系集(Relationship Set)**: n≥2 个实体集之间的数学关系：
> {(e₁, e₂, ..., eₙ) | e₁ ∈ E₁, e₂ ∈ E₂, ..., eₙ ∈ Eₙ}

- 用**菱形(Diamond)** 表示
- 可以有描述性属性(descriptive attributes)，如导师关系中的`date`
- 示例: 学生44553 导师 教师22222 → `(44553, 22222) ∈ advisor`

### Roles (角色)
- 联系集中的实体集可以不需要互不相同
- 每个实体在联系中扮演一个"角色"
- 例: `prereq`关系中，course同时扮演`course_id`和`prereq_id`两个角色

### Degree of Relationship Set (联系的度)
- **Binary (二元联系)**: 涉及两个实体集（最常见）
- **Ternary (三元联系)**: 涉及三个实体集
- 大多数联系是二元的，三元及以上很少见

---

## 3. COMPLEX ATTRIBUTES (复杂属性类型)

### Attribute Classification (属性分类)

| 类型 (Type) | English | 说明 | 举例 |
|------|---------|------|------|
| **简单属性** | Simple | 不可再分 | `ID`, `salary` |
| **复合属性** | Composite | 可分解为子部分 | `name` → (first_name, last_name) |
| **单值属性** | Single-valued | 每个实体只有一个值 | `date_of_birth` |
| **多值属性** | Multivalued | 每个实体可以有多个值 | `phone_numbers` |
| **派生属性** | Derived | 可由其他属性计算得出 | `age` (由 date_of_birth 计算) |

### E-R图中的表示:
- **复合属性**: 子属性缩进显示
- **多值属性**: 用花括号 `{ }` 表示
- **派生属性**: 用虚线椭圆表示

### Domain (域)
- 每个属性的**允许取值集合**

---

## 4. MAPPING CARDINALITIES (映射基数)

### 对于二元联系集 — 四种类型

| 类型 | E-R符号 | 含义 | 举例 |
|------|---------|------|------|
| **一对一 (1:1)** | A→→B | A中每个实体最多关联B中一个实体，反之亦然 | 一个学生只有一个导师，一个导师只指导一个学生 |
| **一对多 (1:N)** | A→—B | A关联0+个B；B中每个实体最多关联1个A | 一个导师指导多个学生，每个学生只有一个导师 |
| **多对一 (N:1)** | A—→B | A中每个实体最多关联1个B；B关联0+个A | 每个学生只有一个导师，一个导师指导多个学生 |
| **多对多 (M:N)** | A——B | A关联0+个B；B关联0+个A | 一个学生有多个导师，一个导师指导多个学生 |

**E-R图符号**:
- **有向线段 (→)** : "一"侧
- **无向线段 (—)** : "多"侧

### Participation Constraints (参与约束)

| 类型 | E-R图 | 含义 |
|------|-------|------|
| **全部参与 (Total)** | 双线 ═ | 实体集中每个实体都参与至少一个联系 |
| **部分参与 (Partial)** | 单线 — | 有些实体可以不参与任何联系 |

**示例**: student在advisor联系中是全部参与（每个学生必须有导师），instructor是部分参与（有些教师可以不指导学生）

### 替代表示法: min..max 基数
- 格式: `l..h`，l是最小值，h是最大值
- **min=1** → 全部参与
- **max=1** → 最多参与一个联系
- **max=\*** → 无限制
- 例: instructor可以指导 **0..\*** 个学生；学生必须有 **1..1** 个导师

### 三元联系上的基数约束
- **最多一个箭头**可以从三元联系中引出
- 多个箭头的含义是模糊的，因此**被禁止**

---

## 5. PRIMARY KEY (主码)

### Key Concepts (码的概念层次)

| 术语 | English | 定义 |
|------|---------|------|
| **超码** | Superkey | 能够唯一标识元组的一个或多个属性的集合 |
| **候选码** | Candidate Key | 没有任何真子集是超码的超码（即最小超码） |
| **主码** | Primary Key | 数据库设计者选定的候选码 |

### 联系集的主码选择

| 映射基数 | 主码选择 |
|-----------|---------|
| **多对多 (M:N)** | 两边实体主码的并集 |
| **一对多 (1:N)** | "多"侧的实体主码 |
| **多对一 (N:1)** | "多"侧的实体主码 |
| **一对一 (1:1)** | 任一侧的主码均可 |

### Weak Entity Sets (弱实体集)

**定义**: 其存在依赖于另一个实体集（称为**标识实体集/identifying entity**或**拥有实体集/owner entity**）的实体集
- 自身没有足够的属性来形成主码
- 主码 = 标识实体的主码 + **分辨符(discriminator/部分码/partial key)**

**E-R图符号:**
| 概念 | 符号 |
|------|------|
| 弱实体集 | **双矩形** □□ |
| 分辨符 | **虚线下划线** |
| 标识联系 | **双菱形** ◇◇ |
| 强实体集 | 单矩形 □ |

**示例**: `section` (教学班) 是弱实体集
- 分辨符: `sec_id`, `semester`, `year`
- 标识实体: `course` (通过 `course_id`)
- 主码: `(course_id, sec_id, semester, year)`

### 弱实体 vs 强实体对比

| 特征 | 强实体集 | 弱实体集 |
|------|---------|---------|
| E-R符号 | 单矩形 | 双矩形 |
| 主码 | 有自己的主码 | 无独立主码 |
| 存在 | 独立存在 | 依赖标识实体 |
| 标识联系 | 普通菱形 | 双菱形 |

**为什么要用弱实体集?** 为了**显式地**表达弱实体集对其标识实体集的依赖关系。任何弱实体集都可以通过添加属性转化为强实体集，但这样会失去清晰的依赖关系表示。

---

## 6. REMOVING REDUNDANT ATTRIBUTES (去除冗余属性)

假设 `student` 实体集有属性 `dept_name`，且存在 `student` 与 `department` 之间的联系 `stud_dept`，那么 `student` 中的 `dept_name` 就是**冗余的**，应该移除。

**但是**: 转换回关系表时，某些情况下属性需要重新引入（如弱实体集的标识实体主码）。

---

## 7. REDUCING ER TO RELATIONAL SCHEMAS (ER图转关系模式)

这是最重要的考点之一！以下是完整的转换规则：

### 规则汇总

| 情况 | 转换方法 | 示例 |
|------|---------|------|
| **强实体集** | 同名的schema，同名的属性 | `student(ID, name, tot_cred)` |
| **弱实体集** | 包含分辨符 + 标识实体主码 | `section(course_id, sec_id, sem, year)` |
| **复合属性** | **展平**: 每个子属性单独列出 | `name(first_name, last_name)` → 两个独立属性 |
| **多值属性** | **单独建表**: (E的主码, M) | `inst_phone(ID, phone_number)` |
| **多对多联系集** | **单独建表**: 两边主码+描述性属性 | `advisor(s_id, i_id)` |
| **一对多/多对一联系** (多侧全部参与) | 在"多"侧表**添加外码列** | instructor表加 dept_name 外码 |
| **一对一联系** | 在任一侧添加外码列 | |
| **弱实体标识联系** | 冗余（弱实体表已含所需属性） | |

### 多值属性的处理（重点）

每个多值属性 M 用一个**单独的模式 EM** 表示：
- EM 的属性 = E的主码 + M
- 每个多值映射到 EM 的一个元组
- 例: instructor 有 phone_number (多值) → `inst_phone(ID, phone_number)`，每个电话号码单独一行

### 复合属性的展平
```
原始: name(first_name, middle_initial, last_name)
转换后: first_name, middle_initial, last_name (三个独立属性)
如果有歧义，加前缀: name_first_name
```

---

## 8. EXTENDED E-R FEATURES (扩展ER特性)

### Specialization (特化) — 自顶向下
- 在实体集内部指定**子分组**，这些子分组与其他实体有区别
- 低层实体集继承高层实体集的**所有**属性和联系参与（**属性继承/Attribute Inheritance**）
- 用标有 **ISA** 的**三角形**表示

### Generalization (泛化) — 自底向上
- 将共享相同特征的多个实体集合并为高层实体集
- 特化和泛化是**简单的逆操作**

### 约束类型

**完整性约束 (Completeness Constraint)** — 是否必须属于某个低层实体集：

| 类型 | 含义 | 示例 |
|------|------|------|
| **全部 (Total)** | 每个高层实体 MUST 属于至少一个低层实体集 | 所有 person 必须是 employee 或 student |
| **部分 (Partial)** (默认) | 某些高层实体可以不特化 | 有些 person 既不是 employee 也不是 student |

**不相交约束 (Disjointness Constraint)**:

| 类型 | 含义 | 示例 |
|------|------|------|
| **不相交 (Disjoint)** | 实体 CANNOT 属于多个低层实体集 | instructor 和 secretary 不相交 |
| **可重叠 (Overlapping)** | 实体 CAN 属于多个低层实体集 | 一个人可以同时是 employee 和 student |

### 特化转关系模式的两种方法

**方法1 (规范化)** — 推荐：
- 为高层实体建表 + 为每个低层实体建表
- 低层表只包含主码（引用高层表）+ 本地属性
- **缺点**: 查一个完整实体需要访问两张表

**方法2 (非规范化)**：
- 只为每个低层实体建表，包含所有继承+本地属性
- **缺点**: 可重叠特化时出现冗余

### Aggregation (聚合)

将**联系集视为抽象实体**，允许联系之间的联系。

**使用场景**: 消除多个联系重叠产生的冗余。

**示例**: 评估学生在项目中受导师指导的表现
- 三元联系 `proj_guide` (student, instructor, project)
- 二元联系 `eval_for` (聚合的 proj_guide, evaluation)

**转关系模式**:
- 聚合实体的schema = 聚合联系的主码 + 关联实体的主码 + 描述性属性
- 被聚合的原始联系的关系模式变为冗余

---

## 9. DESIGN ISSUES (设计问题总结)

### 常见设计决策

| 决策 | 指导原则 |
|------|---------|
| **属性 vs 实体** | 需要存储额外信息时用**实体** |
| **实体 vs 联系** | 描述**动作**时用联系集 |
| **二元 vs 非二元** | 非二元联系可以更清晰地表示多个实体参与同一联系，但可以转为二元 |
| **强实体 vs 弱实体** | 需要显式表达依赖关系时用弱实体 |
| **是否用特化/泛化** | 增加模块化 |
| **是否用聚合** | 将聚合体视为单一单元处理 |

### ER图作为图的性质
- **不连通**: 存在完全不相关的实体集组 → 可能对应独立数据库
- **有环**: 某些实体集之间以至少两种不同方式关联

---

## 10. ALTERNATIVE NOTATIONS (替代表示法)

### UML vs ER 关键差异
- 二元联系在UML中用连接线表示（无菱形）
- **基数约束标注位置相反**
- UML中泛化用空心三角箭头

### Other Notations (其他表示法)
- **Chen 表示法**: 传统E-R图
- **IDE1FX (Crow's feet/乌鸦脚)**: 用"乌鸦脚"符号表示"多"的一侧

---

## 11. COMPLETE EXERCISE SOLUTIONS (课后习题详细解答)

### 习题 6.1 — 汽车保险公司ER图

**题目**: 为一汽车保险公司设计ER图。客户每人有一辆或多辆车，每辆车关联零次或多次事故，每份保险单承保一辆或多辆车，关联一笔或多笔保费支付。

**解答概要**:
- `premium_payment` 是**弱实体**（依赖于 policy）
- `participated` 是 car 和 accident 之间的 M:N 联系
- car 在 participated 中是**部分参与**（有从未出事的车）
- accident 在 participated 中也是**部分参与**（有未确认涉及车辆的报案）

**生成的关系模式**:
```
customer(customer_id, name, address)
car(license_no, model, customer_id)              -- FK: customer_id → customer
accident(report_id, date, place)
participated(license_no, report_id)              -- FK: both
policy(policy_id)
covers(license_no, policy_id)                    -- FK: both
premium_payment(policy_id, payment_no, due_date, amount, received_no)  -- 弱实体
```

### 习题 6.2 — 学生/课程/考试

**三元联系方法**: `exam_marks(student_id, course_id, sec_id, semester, year, exam_id, marks)`

**二元联系方法**: 将三元拆分为多个二元联系，但可能导致信息丢失。

### 习题 6.5 — ER图作为图的分析

**a. 图不连通**: 存在彼此完全不相关的实体集 → 每个连通分量对应企业中完全独立的部分 → 可能对应独立的数据库。

**b. 图有环**: 环上的实体集之间至少以两种不同方式相关联 → 存在多条路径。

### 习题 6.9 — 一对一联系的约束执行

在 `advisor(s_ID, i_ID)` 中，除了将 `s_ID` 设为主码外，还应将 `i_ID` 声明为 **UNIQUE**（超键），以确保一对一约束。

### 习题 6.10 — 使用NOT NULL强制执行全部参与

对于多对一联系R（A为"多"侧，B为"一"侧）：
- 将R的模式与A的合并
- 将引用B主码的外码属性设为 **NOT NULL**
- 这样确保A中的每个实体都在R中有对应条目

### 习题 6.11 — M:N中的全部参与约束

**为什么FK约束不够?**
- SQL外键只能引用主码或UNIQUE约束声明的超码
- M:N联系的关系表中，每个实体主码可能出现多次（不是唯一的）
- 因此无法用SQL外键约束来强制全部参与

**解决方案**: 使用 CHECK 约束或 ASSERTION（设置为延迟检查/DEFERRED）

### 习题 6.12 — 多重继承中的属性冲突

如果实体B同时继承X和Y，且两者都有属性`name`：
- B继承了两者的所有属性
- 在B中可以通过**限定名** `X.name` 或 `Y.name` 来区分

### 习题 6.13 — 跟踪时态变化(Temporal Changes)

为每个实体和联系添加 **valid_time** 属性：
- 随时间变化的属性建模为多值复合属性（每个值带valid_time）
- 转换后，原实体表只含主码，多值属性各自成表

### 习题 6.14 — 主码/候选码/超码的区别

| 术语 | English | 定义 |
|------|---------|------|
| 超码 | Superkey | 能够唯一标识元组的属性集 |
| 候选码 | Candidate Key | 最小超码（无真子集是超码） |
| 主码 | Primary Key | 设计者选定的候选码 |

### 习题 6.17 — 弱实体vs强实体

弱实体集的存在依赖于标识实体集；没有自己的主码；用标识实体的主码+分辨符来唯一标识。

### 习题 6.18 — 两个实体集中出现相同属性

三种子情况的处理：
1. **X是A的主码，不是B的**: 在B中对X创建外键引用A
2. **X是两者的主码**: 将A和B合并为一个实体集C
3. **X不是任何一方的主码**: 创建新实体集C，只含X；A和B中的X成为引用C的外键

### 习题 6.19 — 为什么需要弱实体集

为了**显式地表明**弱实体集对标识实体集的依赖性。虽然总是可以通过添加属性将任何弱实体集变为强实体集，但这样做会失去明确的依赖关系表示。

### 习题 6.20 — 为ER图生成关系模式

**a. 汽车保险公司(6.1)** — 见上文
**b. 学生/课程/考试(6.2)** — 三元方法产生 `exam_marks` 表
**c. 体育队(6.3)**:
```
match(match_id, date, stadium, opponent, own_score, opp_score)
player(player_id, name, age)
played(match_id, player_id, score)
```
**d. 医院(6.15)**:
```
patient(patient_id, name, phone_number, ...)
medical_doctor(doctor_id, name, ...)
tests_and_examinations(test_id, test_type, test_result, ...)
patient_tests(patient_id, doctor_id, test_id)  -- 三元联系
```

### 习题 6.22 — 汽车公司数据库

三元联系 `sale(VIN, customer_id, dealer_id, amount, timestamp)` 表示一次交易。

注: car 实体有 `dealer_id` 外键（哪个经销商有库存），当售出时 sale 记录捕获三方实体。

### 习题 6.23 — 包裹快递公司

使用**特化(total + disjoint)**和**聚合**:
- `location` 是泛化层，子类型: truck, plane, airport, warehouse
- `location_of_package(package_id, location_id, start_timestamp, end_timestamp)` 跟踪包裹位置历史

### 习题 6.24 — 航空公司

简化的关系模式:
```
customer(customer_id, name, phone_number, address)
flights(flight_id, src, dest, timestamp_src, timestamp_dest)
reservation(customer_id, flight_id)
```

### 习题 6.25 — 三元转二元的两种方案对比

- **图6.29b方案**: 用人工实体E + 三个二元联系，更灵活
- **图6.29c方案**: 三个二元联系 R_AB, R_BC, R_AC，更简单但丢失三元分组信息

### 习题 6.26 — 机动车辆特化层次

属性放在**尽可能低**的层次：
```
          vehicle (VIN, price, color — 所有车共有的)
        /    |    |    \
motorcycle  car   van   bus (各自特定的属性)
```

### 习题 6.27 — 不相交 vs 可重叠

- **不相交(Disjoint)**: 实体**不能**属于多个低层实体集
- **可重叠(Overlapping)**: 实体**可以**属于多个低层实体集

### 习题 6.28 — 全部 vs 部分

- **全部(Total)**: 每个高层实体必须属于某个低层实体集
- **部分(Partial)**: 某些高层实体可以不特化

---

## 12. PRACTICE EXAM (练习题 — 来自参考试卷)

### 供应链管理ER设计（Supply Chain ER Design）

**三个实体集**:
- **Product(商品)**: ProductID, ProductName, Specifications, UnitPrice
- **Store(商店)**: StoreNumber, StoreName, City
- **Supplier(供应商)**: SupplierNumber, SupplierName, City

**联系**:
- **Supply** (M:N, Supplier↔Product): 每个供应商提供多种商品，每种商品可来自不同供应商，按月提供
- **Sales** (M:N, Store↔Product): 每个商店出售多种商品，每种商品在多个商店销售，按月制定采购计划

**要求**:
1. 画ER图，创建关系模式，指出主码和外码
2. SQL定义Product表及完整性约束
3. SQL查询: 找出商店名称及针对"tissue"商品的月度采购计划，要求该商品供应商在Shanghai
4. 画查询树

---

## 13. KEY TERMS CHECKLIST (关键术语检查清单)

- [ ] 实体/实体集 (Entity/Entity Set)
- [ ] 弱实体集/强实体集 (Weak/Strong Entity Set)
- [ ] 联系/联系集 (Relationship/Relationship Set)
- [ ] 标识联系 (Identifying Relationship)
- [ ] 属性类型: 简单/复合/单值/多值/派生 (Simple/Composite/Single-valued/Multivalued/Derived)
- [ ] 映射基数: 1:1, 1:N, N:1, M:N (Mapping Cardinalities)
- [ ] 全部参与 vs 部分参与 (Total vs Partial Participation)
- [ ] 主码/候选码/超码/分辨符 (Primary Key/Candidate Key/Superkey/Discriminator)
- [ ] 特化(自顶向下) / 泛化(自底向上) (Specialization/Generalization)
- [ ] ISA联系 / 属性继承 (ISA/Attribute Inheritance)
- [ ] 完整性约束: 全部/部分 (Completeness Constraint: Total/Partial)
- [ ] 不相交/可重叠 (Disjoint/Overlapping)
- [ ] 聚合 (Aggregation)
- [ ] ER转关系模式的7条规则 (ER-to-Relational Reduction Rules)
- [ ] 复合属性展平 / 多值属性单独建表
- [ ] 二元 vs 非二元联系 (Binary vs Non-Binary)
- [ ] Chen表示法 / Crow's Feet / UML

---

## 14. ER DIAGRAM SYMBOL SUMMARY (ER图符号速查表)

| 概念 (Concept) | 符号 (Symbol) |
|---------|--------|
| 实体集 Entity Set | 矩形 Rectangle □ |
| 弱实体集 Weak Entity Set | 双矩形 Double Rectangle □□ |
| 联系集 Relationship Set | 菱形 Diamond ◇ |
| 标识联系 Identifying Relationship | 双菱形 Double Diamond ◇◇ |
| 属性 Attribute | 椭圆 Oval |
| 主码属性 Key Attribute | 下划线 Underlined |
| 分辨符 Discriminator | 虚线下划线 Dashed underline |
| 派生属性 Derived Attribute | 虚线椭圆 Dashed oval |
| 多值属性 Multivalued | 双椭圆或{花括号} |
| 全部参与 Total Participation | 双线 Double line ═ |
| 基数"一" Cardinality One | 有向线 → |
| 基数"多" Cardinality Many | 无向线 — |
| ISA (特化) Specialization | 三角形标ISA |
| 聚合 Aggregation | 虚线边界包围聚合部分 |

---

> **复习提示**: 第6章的核心是**画ER图**和**ER图转关系模式**。务必熟练掌握这两种技能，考试中必有一道ER设计大题。
