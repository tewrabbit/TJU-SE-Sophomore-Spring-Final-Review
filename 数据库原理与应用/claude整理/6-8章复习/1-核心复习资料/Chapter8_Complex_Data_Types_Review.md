# Chapter 8: Complex Data Types (复杂数据类型)
# 第八章 完整复习资料 — 中英双语版

---

## 1. SEMI-STRUCTURED DATA (半结构化数据)

### 为什么需要半结构化数据？

关系模型要求**原子数据类型**，但对于许多应用来说过于严格:
- 模式频繁变化的应用
- 数据交换（应用间或前后端之间）
- Web服务（JSON/XML与JavaScript/移动端交互）

### 半结构化数据模型的特征

| 特征 | English | 说明 |
|------|---------|------|
| **灵活模式** | Flexible Schema | 可以随时添加新属性 |
| **宽列表示** | Wide Column Representation | 每个元组可以有不同的属性集 |
| **稀疏列表示** | Sparse Column Representation | 固定但很大的属性集，每行只存一部分 |
| **多值数据类型** | Multivalued Data Types | 集合(Sets)、多重集(Multisets) |
| **键值映射** | Key-Value Map | {(key, value)} 对，如 {(brand, Apple), (ID, MacBook Air)} |
| **数组** | Arrays | 广泛用于科学和监控应用 |

### 多值属性与非第一范式 (NFNF)

- 集合示例: `{basketball, La Liga, cooking, anime, jazz}`
- 映射操作: `put(key, value)`, `get(key)`, `delete(key)`
- 数组优于(time, value)对: `[5, 8, 9, 11]` vs `{(1,5), (2,8), (3,9), (4,11)}`
- **数组数据库**: Oracle GeoRaster, PostGIS, SciDB

---

## 2. JSON (JavaScript Object Notation)

### JSON基础

**JSON**是目前最广泛使用的数据交换格式。

**数据类型**: integer, real, string
**对象(Objects)**: 键值映射 — `{attribute_name: value}` 对的集合
**数组(Arrays)**: 也是键值映射（偏移量→值）

### JSON示例

```json
{
    "ID": "22222",
    "name": {
        "firstname": "Albert",
        "lastname": "Einstein"
    },
    "deptname": "Physics",
    "children": [
        {"firstname": "Hans", "lastname": "Einstein"},
        {"firstname": "Eduard", "lastname": "Einstein"}
    ]
}
```

**关键特征**: 嵌套结构，对象和数组可以递归嵌套。

### JSON与SQL

现代数据库的SQL扩展支持:
1. **JSON类型**: 存储JSON数据
2. **路径表达式**: 从JSON对象中提取数据
   - 如 `V->ID` 或 `v.ID`
3. **从关系数据生成JSON**:
   ```sql
   json_build_object('ID', 12345, 'name', 'Einstein')
   ```
4. **聚合生成JSON集合**: PostgreSQL的 `json_agg`
5. **压缩存储**: BSON (Binary JSON) 用于高效存储

---

## 3. XML (Extensible Markup Language)

### XML基础

XML使用**标签(tags)**标记文本，使数据**自我描述(self-documenting)**。

### XML示例

```xml
<course>
    <course_id> CS-101 </course_id>
    <title> Intro. to Computer Science </title>
    <dept_name> Comp. Sci. </dept_name>
    <credits> 4 </credits>
</course>
```

### 更复杂的XML示例 (嵌套订单)

```xml
<purchase_order>
    <identifier> P-101 </identifier>
    <purchaser>
        <name> Cray Z. Coyote </name>
        <address> Route 66, Mesa Flats, Arizona 86047, USA </address>
    </purchaser>
    <supplier> ... </supplier>
    <itemlist>
        <item>
            <identifier> RS1 </identifier>
            <description> Atom powered rocket sled </description>
            <quantity> 2 </quantity>
            <price> 199.95 </price>
        </item>
    </itemlist>
</purchase_order>
```

### XML查询
- **XQuery**: 专门查询嵌套XML结构的语言（目前不广泛使用）
- **SQL扩展**: 存储XML、生成XML、提取XML数据、路径表达式

---

## 4. RDF AND SPARQL (知识表示与查询)

### RDF: Resource Description Format (资源描述格式)

RDF将事实表示为**三元组(Triples)**:
```
(主语 Subject, 谓语 Predicate, 宾语 Object)
```

**示例**:
```
(NBA-2019, winner, Raptors)
(Washington-DC, capital-of, USA)
(Washington-DC, population, 6,200,000)
```

### RDF建模方式

两种模式:
1. **(ID, attribute-name, value)** — 对象属性
2. **(ID₁, relationship-name, ID₂)** — 对象间关系

RDF类似ER模型但**模式灵活**，具有**天然的图表示**。

### RDF如何表示n元联系？

两种方法:

**方法1 (来自6.9.4节)**: 创建人工实体，链接到n个实体
```
(Barack Obama, president-of, USA, 2008-2016)
→ (e1, person, Barack Obama)
  (e1, country, USA)
  (e1, president-from, 2008)
  (e1, president-till, 2016)
```

**方法2**: 使用四元组(Quads)，带上下文实体(context entity)
```
(Barack Obama, president-of, USA, c1)
(c1, president-from, 2008)
(c1, president-till, 2016)
```

### RDF知识图谱
- DBPedia, Yago, Freebase, WikiData
- **Linked Open Data** 项目旨在连接不同知识图谱

### SPARQL: RDF查询语言

**三元组模式**:
```
?cid title "Intro. to Computer Science"
?sid course ?cid
```

**SPARQL查询示例**:
```sparql
SELECT ?name
WHERE {
    ?cid title "Intro. to Computer Science" .
    ?sid course ?cid .
    ?id takes ?sid .
    ?id name ?name .
}
```

**SPARQL特点**:
- 聚合(Aggregation)
- 可选连接(Optional joins) — 类似外连接
- 子查询(Subqueries)
- **路径上的传递闭包(Transitive closure on paths)**

**SPARQL与SQL的关键差异**: SPARQL中三元组模式的**谓词可以是变量**，匹配任何关系名或属性名。

---

## 5. OBJECT ORIENTATION (面向对象特性)

### 对象-关系数据模型

提供更丰富的类型系统，包括复杂数据类型和面向对象特性。

### 三种集成方法

| 方法 | English | 说明 |
|------|---------|------|
| **对象-关系数据库** | Object-Relational DB | 在关系数据库基础上添加OO特性 |
| **对象-关系映射** | ORM | 编程语言对象 ↔ 数据库元组自动转换 |
| **面向对象数据库** | Object-Oriented DB | 原生支持OO数据和直接编程访问 |

### 用户定义类型 (User-Defined Types)

```sql
CREATE TYPE Person (
    ID varchar(20) PRIMARY KEY,
    name varchar(20),
    address varchar(20)
) REF FROM(ID);

CREATE TABLE people OF Person;
```

### 表类型 (Table Types)

```sql
CREATE TYPE interest AS TABLE (
    topic varchar(20),
    degree_of_interest int
);

CREATE TABLE users (
    ID varchar(20),
    name varchar(20),
    interests interest
);
```

### 类型和表继承 (Type and Table Inheritance)

```sql
-- 类型继承 (SQL标准)
CREATE TYPE Student UNDER Person (degree varchar(20));
CREATE TYPE Teacher UNDER Person (salary integer);

-- 表继承 (PostgreSQL语法)
CREATE TABLE students (degree varchar(20)) INHERITS (people);
CREATE TABLE teachers (salary integer) INHERITS (people);

-- 另一种语法 (Oracle)
CREATE TABLE students OF Student UNDER people;
CREATE TABLE teachers OF Teacher UNDER people;
```

**两种继承模式**: 类型继承和表继承是相关但不同的概念。

### 引用类型 (Reference Types)

```sql
CREATE TYPE Person (
    ID varchar(20) PRIMARY KEY,
    name varchar(20),
    address varchar(20)
) REF FROM(ID);

CREATE TABLE people OF Person;

CREATE TYPE Department (
    dept_name varchar(20),
    head REF(Person) SCOPE people
);

CREATE TABLE departments OF Department;

-- 插入
INSERT INTO departments VALUES ('CS', '12345');

-- 使用系统生成的引用
SELECT head->name, head->address FROM departments;
```

**REF类型的使用**: 通过 `->` 运算符进行路径表达式导航。

### 对象-关系映射 (ORM)

ORM系统提供:
- 编程语言对象与数据库元组之间的**映射规范**
- 创建对象时**自动创建**数据库元组
- 更新/删除对象时**自动**更新/删除数据库元组
- **检索接口**: 查询数据库，从元组创建对象

**流行ORM框架**: Hibernate (Java), Django ORM (Python)

---

## 6. TEXTUAL DATA AND INFORMATION RETRIEVAL (文本数据与信息检索)

### 关键词查询

**基本模型**: 给定查询关键词，检索包含所有关键词的文档。
**高级模型**: 对文档进行**相关性排序(Relevance Ranking)**。

### TF-IDF (词频-逆文档频率)

**这是本章的计算重点!**

**TF(d, t) — 词频**:
> 衡量词项t对文档d的相关性
```
TF(d, t) = log(1 + n(d,t) / n(d))
```
其中:
- n(d,t) = 词项t在文档d中出现的次数
- n(d) = 文档d中词项的总数

**IDF(t) — 逆文档频率**:
> 衡量词项t在整个文档集中的区分能力
```
IDF(t) = 1 / n(t)
```
其中 n(t) = 包含词项t的文档数量

**相关性得分**:
```
r(d, Q) = Σ(t∈Q) TF(d, t) × IDF(t)
```

**其他TF-IDF变体**: 考虑词语**邻近度(proximity)**、忽略**停用词(stop words)**等。

### PageRank (页面排名算法)

**核心思想**: 
- 被很多页面链接的页面应有更高的PageRank
- 被高PageRank页面链接的页面应有更高的PageRank

**随机游走模型 (Random Walk Model)**:
```
P[j] = δ/N + (1−δ) × Σ(i=1到N) T[i,j] × P[i]
```
其中:
- N = 总页面数
- δ = 阻尼因子，通常设为 **0.15**
- T[i,j] = 从页面i点击链接到页面j的概率 (如果所有链接平等，T[i,j] = 1/Nᵢ)

**迭代解法**:
1. 初始化所有 P[i] = 1/N
2. 每轮使用上述公式更新 P
3. 当变化很小时停止（或达到约30轮迭代上限）

### 检索效果度量

| 度量 | English | 定义 | 公式 |
|------|---------|------|------|
| **查准率** | Precision | 返回结果中真正相关的比例 | 相关返回数/总返回数 |
| **查全率** | Recall | 所有相关结果中被返回的比例 | 返回相关数/总相关数 |
| **Precision@k** | P@k | 前k个结果中的查准率 | |

---

## 7. SPATIAL DATA (空间数据)

### 空间数据库

存储与**空间位置**相关的信息，支持高效的存储、索引和查询。

### 两类空间数据

| 类型 | English | 内容 | 坐标系 |
|------|---------|------|--------|
| **地理数据** | Geographic Data | 道路地图、土地利用、地形、政治边界 | (Latitude, Longitude, Elevation) |
| **几何数据** | Geometric Data | 建筑设计、飞机设计、集成电路布局 | 2D/3D欧氏空间 (X,Y,Z) |

### GIS (地理信息系统)
专门为存储地理数据设计的特殊用途数据库。

---

### 几何信息的表示

**基本几何体**:

| 几何体 | 表示方法 |
|--------|---------|
| **线段 (Line Segment)** | 端点坐标 |
| **折线 (Polyline/Linestring)** | 端点坐标序列 |
| **曲线 (Curve)** | 分段为线段序列（或圆弧序列） |
| **多边形 (Polygon)** | 顶点列表（按顺序）；也可三角剖分 |
| **多面体 (Polyhedron)** | 分割为四面体；或列出面（多边形）+ 哪侧在内部 |

### PostGIS几何类型

```
LINESTRING(1 1, 2 3, 4 4)
POLYGON((1 1, 2 3, 4 4, 1 1))
```

**类型转换**: `ST_GeometryFromText()`, `ST_GeographyFromText()`
**操作**: `ST_Union()`, `ST_Intersection()`, ...

### 设计数据库 (Design Databases)

复杂对象由简单对象通过**并(union)、交(intersection)、差(difference)**操作构造:
- 2D: 点、线、三角形、矩形、多边形
- 3D: 球体、圆柱体、立方体的并交差
- **线框模型(Wireframe models)**: 用简单对象集合表示3D表面

**空间完整性约束(Spatial Integrity Constraints)**: 管道不应相交、电线不应太近等。

### 地理数据格式

| 格式 | English | 说明 | 示例 |
|------|---------|------|------|
| **栅格数据** | Raster Data | 位图/像素图，2D+ | 卫星云图(每像素=云量)；可加温度维度、时间维度 |
| **矢量数据** | Vector Data | 基本几何对象 | 道路=线条、河流=曲线/多边形、湖泊=多边形 |

---

### 空间查询类型

| 查询类型 | English | 说明 | PostGIS函数 |
|---------|---------|------|-------------|
| **区域查询** | Region Queries | 在指定区域内部分或全部的对象 | `ST_Contains()`, `ST_Overlaps()` |
| **邻近查询** | Nearness Queries | 在指定位置附近的对象 | `ST_Distance()` |
| **最近邻查询** | Nearest Neighbor | 找到距离最近的对象 | `ST_Distance()` + MIN |
| **空间图查询** | Spatial Graph Queries | 基于空间图的信息 | 最短路径(道路网) |
| **空间连接** | Spatial Join | 以位置为连接属性 | |
| **区域运算** | Region Operations | 计算区域的交/并 | `ST_Union()`, `ST_Intersection()` |

---

## 8. COMPLETE EXERCISE SOLUTIONS (课后习题全解)

### 8.1 — 四种格式表示学生信息

**题目**: 用JSON、XML、RDF三元组、RDF图四种格式表示学生Shankar及其选课信息。

**JSON**: 嵌套对象，takes数组中每项嵌套course对象
**XML**: `<student>` 根元素，嵌套`<takes>` → `<take>` → `<course>`
**RDF三元组**: 为每个实体创建标识符，通过三元组建模所有关系
**RDF图**: 图形化表示(见 8.1d.jpg)

### 8.2 — SPARQL查询

**(a) 查找所有名叫Zhang的学生所修课程的名称**:
```sparql
SELECT ?title WHERE {
    ?student_id name "Zhang" .
    ?student_id takes ?section_id .
    ?section_id sec_course ?course_id .
    ?course_id title ?title .
}
```

**(b) 名叫Zhang的学生所修、由名叫Srinivasan的教师授课的课程**:
```sparql
SELECT ?title WHERE {
    ?student_id instance-of student .
    ?student_id name "Zhang" .
    ?instructor_id instance-of instructor .
    ?instructor_id name "Srinivasan" .
    ?student_id takes ?sec .
    ?instructor_id teaches ?sec .
    ?sec sec_course ?course_id .
    ?course_id title ?title .
}
```

**(c) 查找名为Srinivasan的教师的所有属性(无需枚举属性名)**:
```sparql
SELECT ?attribute_name ?attribute_value WHERE {
    ?instructor_id instance-of instructor .
    ?instructor_id name "Srinivasan" .
    ?instructor_id ?attribute_name ?attribute_value .
}
```
**关键**: SPARQL谓词可以是变量，这点SQL做不到！

### 8.3 — 车辆租赁SQL模式(带继承)

车辆: VIN, 车牌号, 制造商, 型号, 购买日期, 颜色
子类型: 卡车(载货容量), 跑车(马力, 租车年龄要求), 厢式货车(乘客数), 越野车(离地间隙, 驱动类型)

```sql
CREATE TYPE Vehicle (vehicle_id INTEGER, license_number CHAR(15), ... );
CREATE TABLE vehicle OF TYPE Vehicle;
CREATE TABLE truck (cargo_capacity INTEGER) UNDER vehicle;
CREATE TABLE sportsCar (horsepower INTEGER, renter_age_requirement INTEGER) UNDER vehicle;
CREATE TABLE van (num_passengers INTEGER) UNDER vehicle;
CREATE TABLE offRoadVehicle (ground_clearance REAL, driveTrain DriveTrainType) UNDER vehicle;
```

PostgreSQL: 使用 `INHERITS (vehicle)` 替代 `UNDER vehicle`

### 8.4 — 带多重集的SQL模式

员工有 ChildrenSet (multiset) 和 SkillSet (multiset)

```sql
CREATE TYPE children AS TABLE (name VARCHAR(30), birthday DATE);
CREATE TYPE exams AS TABLE (year INTEGER, city VARCHAR(30));
CREATE TYPE skills AS TABLE (type VARCHAR(30), examset exams);
CREATE TABLE emp (ename VARCHAR(30), childrenset children, skillset skills);
```

### 8.5 — 带用户定义类型的教师表

```sql
CREATE TYPE nameType AS (first_name VARCHAR(15), middle_initial CHAR, last_name VARCHAR(15));
CREATE TYPE streetType AS (street_name VARCHAR(15), street_number VARCHAR(4), apartment_number VARCHAR(7));
CREATE TYPE addressType AS (street streetType, city VARCHAR(15), state VARCHAR(15), zip_code CHAR(6));

CREATE TABLE instructor (
    id INTEGER PRIMARY KEY,
    name nameType,
    address addressType,
    phone_number VARCHAR(12)[],   -- 10元素数组
    date_of_birth DATE
);
```

### 8.6 — 引用类型与对象查询

**(a) 模式定义** (使用REF):
```sql
CREATE TYPE Employee (person_name VARCHAR(30), street VARCHAR(15), city VARCHAR(15));
CREATE TYPE Company (company_name VARCHAR(15), city VARCHAR(15));
CREATE TABLE employee OF Employee;
CREATE TABLE company OF Company;
CREATE TYPE Works (person REF(Employee) SCOPE employee, comp REF(Company) SCOPE company, salary INT);
CREATE TABLE works OF Works;
```

**(b)(i) 员工最多的公司**:
```sql
SELECT comp->name FROM works GROUP BY comp
HAVING COUNT(person) >= ALL (SELECT COUNT(person) FROM works GROUP BY comp);
```

**(b)(ii) 工资总额最小的公司**:
```sql
SELECT comp->name FROM works GROUP BY comp
HAVING SUM(salary) <= ALL (SELECT SUM(salary) FROM works GROUP BY comp);
```

**(b)(iii) 平均工资高于First Bank Corporation的公司**:
```sql
SELECT comp->name FROM works GROUP BY comp
HAVING AVG(salary) > (SELECT AVG(salary) FROM works WHERE comp->company_name = 'First Bank Corporation');
```

### 8.7 — TF-IDF相关性计算

使用本章各练习题作为"文档"，用"SQL relation"查询计算TF-IDF相关性。
- 不含任一关键词的题目相关性为0
- 图像参考 Solution_8.7.png

### 8.8 — PageRank的SQL实现

将转移矩阵表示为关系 `jump_probability_matrix(T REAL[][])`，保存所有网页的 `page_rank`。
一次迭代: `PR(j) = 0.15/N + 0.85 * Σᵢ(T[i][j] * PR(i))`

4页示例: PageRank从[0.25, 0.25, 0.25, 0.25]变为[0.482, 0.353, 0.469, 0.784]

### 8.9 — PostGIS空间查询

**(a) 找出位置在Packard 101教室内的学生 (区域查询)**:
```sql
SELECT name FROM student
WHERE ST_Contains(
    student.location,
    (SELECT location FROM classroom WHERE building='Packard' AND room_number='101')
);
```

**(b) 距离Packard 101 100米以内的所有教室 (邻近查询)**:
```sql
SELECT building, room_number FROM classroom
WHERE ST_Distance(location,
    (SELECT location FROM classroom WHERE building='Packard' AND room_number='101')) <= 100;
```

**(c) 地理上最接近学号12345的学生 (最近邻查询)**:
```sql
WITH distance_bn_12345(id, name, dis) AS (
    SELECT id, name, ST_Distance(location, (SELECT location FROM student WHERE id='12345'))
    FROM student WHERE id <> '12345'
)
SELECT id, name FROM distance_bn_12345
WHERE dis = (SELECT MIN(dis) FROM distance_bn_12345);
```

**(d) 相距小于200米的所有学生对 (空间连接)**:
```sql
SELECT s1.id AS id1, s1.name AS name1, s2.id AS id2, s2.name AS name2
FROM student AS s1 CROSS JOIN student AS s2
WHERE s1.id < s2.id AND ST_Distance(s1.location, s2.location) < 200;
```

### 8.10 — 1NF/4NF重新设计 (练习8.4的规范化)

将嵌套结构转换为多个扁平表:
- `Emp(emp_id, ename)`
- `Children(child_id, name, birthday)`
- `EmpChild(emp_id, child_id)` — 多对多
- `Skills(type)` — 主码: type
- `Exams(year, city)` — 主码: (year, city)
- `SkillExams(type, year, city)` — 连接表
- `SkillExamsEmp(emp_id, type, year, city)` — 员工拥有技能考试

### 8.11 — 继承的3NF替代方案

`tpeople(ID, name, address, degree, salary)` + 约束: degree和salary至少一个为NULL

FD: `{ID} → {name, address, degree, salary}`

### 8.12 — 人员特化 (E-R图中的重叠/不相交特化)

**(a) PostgreSQL模式**:
```sql
CREATE TABLE person (id INTEGER PRIMARY KEY, name VARCHAR(50), address VARCHAR(50));
CREATE TABLE employee (salary NUMERIC(12,2)) INHERITS (person);
CREATE TABLE student (tot_credits INTEGER) INHERITS (person);
CREATE TABLE employee_and_student () INHERITS (employee, student);  -- 多重继承(重叠)
CREATE TABLE instructor (rank INTEGER) INHERITS (employee);
CREATE TABLE secretary (hours_per_week INTEGER) INHERITS (employee);  -- 不相交
```

**(b) 不是秘书的所有人**: 用 `EXCEPT` 排除secretary

**(c) 既不是雇员也不是学生的人**: `SELECT name FROM ONLY person;` (ONLY排除子表)

**(d) 既是雇员又是学生**: **可以**。向 `employee_and_student` 插入即可（重叠特化已规划好）

### 8.13 — 元组上的TF-IDF

在数据库关键词搜索中，将每个元组视为"文档"，TF-IDF同样适用。

### 8.14 — Web环对PageRank的影响

加入Web环后，大多数网站的PageRank通常会**提高**（环形结构中的入站链接增加了）。

### 8.15 — Google广告选择

使用**数据聚类(Data Clustering)**算法对网页进行聚类，如[体育、汽车、服装]，根据网页所属聚类展示相关广告。

---

## 9. KEY CONCEPTS SUMMARY (核心概念总结)

### 数据模型对比

| 模型 | 特点 | 查询语言 |
|------|------|---------|
| **JSON** | 嵌套对象、数组，最广泛的数据交换格式 | SQL路径表达式 (`V->ID`) |
| **XML** | 标签标记、自我描述、层次结构 | XQuery, SQL/XML |
| **RDF** | 三元组(主语,谓语,宾语)、知识图谱 | SPARQL |

### SPARQL vs SQL关键差异

| 特性 | SQL | SPARQL |
|------|-----|--------|
| 数据模型 | 表(关系) | 三元组(图) |
| 模式 | 预定义固定模式 | 灵活/无预定义模式 |
| 谓词是否可为变量 | ❌ (列名固定) | ✅ (谓词变量匹配任何属性) |
| 路径查询 | 需要递归CTE | 内置传递闭包 |

### 面向对象数据库核心概念

| 概念 | SQL语法 |
|------|---------|
| 用户定义类型 | `CREATE TYPE ... AS (...)` |
| 类型继承 | `CREATE TYPE ... UNDER ...` |
| 表继承 | `CREATE TABLE ... INHERITS (...)` 或 `UNDER ...` |
| 引用类型 | `REF(Type) SCOPE table` |
| 路径表达式 | `ref->attribute` |
| 多重继承 | PostgreSQL允许多个父表 |

### TF-IDF vs PageRank

| 特性 | TF-IDF | PageRank |
|------|--------|----------|
| 用途 | 文档与查询的相关性 | 页面重要性/流行度 |
| 计算基础 | 词频 + 逆文档频率 | 超链接结构 |
| 文档相关 | ✅ 每个查询不同 | ❌ 与查询无关 |
| 公式 | Σ TF×IDF | 随机游走稳态分布 |

### PostGIS空间查询速查

| 函数 | 功能 |
|------|------|
| `ST_Contains(A, B)` | B在A内部? |
| `ST_Overlaps(A, B)` | A和B重叠? |
| `ST_Distance(A, B)` | A到B的距离 |
| `ST_Union(A, B)` | A和B的并集 |
| `ST_Intersection(A, B)` | A和B的交集 |
| `ST_GeometryFromText()` | 文本转几何类型 |
| `ST_GeographyFromText()` | 文本转地理类型 |

---

## 10. QUICK REVIEW CHECKLIST (快速复习清单)

- [ ] 半结构化数据的特征(灵活模式、宽列、稀疏列、多值)
- [ ] JSON: 对象(键值映射) vs 数组(偏移→值)
- [ ] JSON SQL扩展: 路径表达式、json_build_object、json_agg
- [ ] XML: 标签、自我描述、层次结构、XQuery
- [ ] RDF: 三元组(主语,谓语,宾语)、知识图谱表示
- [ ] RDF表示n元联系: 人工实体法 vs 四元组法
- [ ] SPARQL: 三元组模式、谓词可以是变量
- [ ] 用户定义类型: CREATE TYPE ... AS/UNDER
- [ ] 表类型: CREATE TYPE ... AS TABLE
- [ ] 继承: UNDER (标准) vs INHERITS (PostgreSQL)
- [ ] 引用类型: REF(Type) SCOPE table
- [ ] 路径表达式: ref->attribute
- [ ] ORM: Hibernate(Java), Django(Python)
- [ ] TF(d,t) = log(1 + n(d,t)/n(d))
- [ ] IDF(t) = 1/n(t)
- [ ] r(d,Q) = Σ TF × IDF
- [ ] PageRank: P[j] = δ/N + (1-δ)Σ(T[i,j]×P[i]), δ=0.15
- [ ] 查准率(Precision) vs 查全率(Recall)
- [ ] 栅格数据 vs 矢量数据
- [ ] PostGIS: 点、线、多边形、ST_Contains、ST_Distance
- [ ] 空间查询: 区域、邻近、最近邻、空间连接、空间图路径

---

> **复习提示**: 第8章相对第6、7章来说考试比重较小，但**JSON/SQL嵌套查询**、**TF-IDF计算**、**PostGIS空间查询**、**SPARQL语法**和**RDF三元组表示**是常见的考点。尤其注意SPARQL中谓词可以是变量这一SQL做不到的特性，以及TF-IDF的具体计算公式。
