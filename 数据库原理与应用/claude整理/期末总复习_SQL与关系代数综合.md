# 数据库原理 期末总复习：SQL与关系代数综合

> **覆盖章节**: Ch2 (关系模型/关系代数) + Ch3 (SQL基础) + Ch4 (中级SQL) + Ch5 (高级SQL)  
> **考试权重**: ⭐⭐⭐⭐⭐ 最高 (考点分布："SQL：必须")  
> **历年真题**: 2006A/B/C Q2/Q3/Q4, 2020 Q1/Q2/Q3/Q6/Q8

---

## 一、考试数据库模式（University Database Schema）

**所有SQL题目都基于这些表，必须烂熟于心！**

```
instructor(ID, name, dept_name, salary)
department(dept_name, building, budget)
student(ID, name, dept_name, tot_cred)
takes(ID, course_id, sec_id, semester, year, grade)
course(course_id, title, dept_name, credits)
section(course_id, sec_id, semester, year, building, room_no, time_slot_id)
teaches(ID, course_id, sec_id, semester, year)
advisor(s_ID, i_ID)
classroom(building, room_no, capacity)
time_slot(time_slot_id, day, start_time, end_time)
prereq(course_id, prereq_id)
```

**重要关联**：
- instructor.ID → teaches.ID
- student.ID → takes.ID, advisor.s_ID
- course.course_id → section.course_id, prereq.course_id, teaches.course_id
- takes 和 section 通过 (course_id, sec_id, semester, year) 关联

---

## 二、关系代数 (Relational Algebra) — Ch2

### 2.1 六大基本操作

| 操作 | 符号 | SQL等价 | 一元/二元 | 去重？ |
|------|------|---------|:---:|:---:|
| 选择 | σ_p(r) | WHERE p | 一元 | 否 |
| 投影 | Π_A(r) | SELECT DISTINCT A | 一元 | **是** |
| 并 | r ∪ s | UNION | 二元 | **是** |
| 差 | r − s | EXCEPT | 二元 | — |
| 笛卡尔积 | r × s | FROM r, s | 二元 | 否 |
| 重命名 | ρ_x(E) | AS x | 一元 | — |

### 2.2 关键注意点

**关系代数 vs SQL 的去重行为**：
- 关系代数：投影(Π)和并(∪)自动去重（因为是集合）
- SQL：SELECT不去重，UNION去重

**并/交/差的前提**：两个关系必须**相容(Compatible)**——
- 列数相同
- 对应列的域(数据类型)相同

### 2.3 派生操作

| 操作 | 定义 | 用途 |
|------|------|------|
| **交** r ∩ s | r − (r − s) | A中且B中也有的 |
| **自然连接** r ⋈ s | Π(σ(r × s)) | 同名等值连接，去掉重复列 |
| **θ连接** r ⋈_θ s | σ_θ(r × s) | 任意条件的连接 |
| **除** r ÷ s | 复杂的反向查询 | "满足所有..." |

### 2.4 历年关系代数真题

**2006A-Q2**：找出在Perryridge分行有贷款但没有账户的客户姓名。
```
Π_customer_name(σ_branch_name='Perryridge'(borrower ⋈ loan))
  −
Π_customer_name(σ_branch_name='Perryridge'(depositor ⋈ account))
```

**2006C-Q2（除法的经典应用）**：找出在Brooklyn分行的**所有**支行都有账户的客户。
```
Π_customer_name, branch_name(depositor ⋈ account) 
  ÷ 
Π_branch_name(σ_branch_city='Brooklyn'(branch))
```

**2020-Q8**：关系代数表达式（含/不含聚集函数版本）。

### 2.5 元组关系演算 (Tuple Relational Calculus) [了解]

2006年A/B/C每年第三题都考了元组关系演算，形式如：

```
{t | ∃ s ∈ loan (t[loan_number] = s[loan_number] 
     ∧ s[branch_name] = "Perryridge"
     ∧ ∀ u ∈ depositor ( ... ))}
```

**关键符号**：
- `∃` (存在量词): there exists
- `∀` (全称量词): for all
- `∧` (合取): and
- `∨` (析取): or
- `¬` (否定): not
- `⇒` (蕴含): implies

---

## 三、SQL 万能解题框架

### 3.1 基础查询模板

```sql
SELECT [DISTINCT] <属性/表达式/聚集函数>
FROM <表名> [AS <别名>] [[NATURAL] JOIN ...]
WHERE <行过滤条件>
GROUP BY <分组属性>
HAVING <分组过滤条件>
ORDER BY <排序属性> [ASC|DESC]
```

### 3.2 🔴 SQL执行逻辑顺序（必须理解！）

```
1. FROM        — 确定数据来源，做连接/笛卡尔积
2. WHERE       — 过滤行（不能用聚集函数！）
3. GROUP BY    — 分组
4. HAVING      — 过滤组（可以用聚集函数）
5. SELECT      — 选列/计算表达式
6. ORDER BY    — 排序
7. LIMIT       — 限制行数
```

### 3.3 各类SQL模板

#### 模板1：带分组的聚集查询
```sql
SELECT dept_name, COUNT(*) AS num_instructors, AVG(salary) AS avg_sal
FROM instructor
GROUP BY dept_name
HAVING AVG(salary) > 42000;
```

#### 模板2：嵌套子查询 — IN/NOT IN
```sql
-- 查出教过至少一门课程的教师
SELECT DISTINCT name FROM instructor
WHERE ID IN (SELECT ID FROM teaches);
```

#### 模板3：嵌套子查询 — EXISTS/NOT EXISTS（关联子查询）
```sql
-- 查出没教过任何课的教师
SELECT name FROM instructor I
WHERE NOT EXISTS (
    SELECT * FROM teaches T WHERE T.ID = I.ID
);
```

#### 模板4："所有"的翻译 — 双重否定
```sql
-- 查出选了Biology系所有课程的学生（经典全称命题！）
SELECT S.name FROM student S
WHERE NOT EXISTS (
    -- Biology系的所有课程中...
    (SELECT course_id FROM course WHERE dept_name = 'Biology')
    EXCEPT
    -- ...去掉该学生选过的
    (SELECT T.course_id FROM takes T WHERE T.ID = S.ID)
);

-- 等价写法
SELECT S.name FROM student S
WHERE NOT EXISTS (
    SELECT C.course_id FROM course C
    WHERE C.dept_name = 'Biology'
    AND NOT EXISTS (
        SELECT * FROM takes T
        WHERE T.course_id = C.course_id AND T.ID = S.ID
    )
);
```
**核心技巧**：全称命题 "对所有X都..." = `NOT EXISTS (X AND NOT EXISTS (...))`

#### 模板5：集合比较 — SOME/ALL
```sql
-- 查出工资高于Biology系至少一位教师的教师
SELECT name FROM instructor
WHERE salary > SOME (SELECT salary FROM instructor WHERE dept_name = 'Biology');

-- 查出工资高于Biology系所有教师的教师
SELECT name FROM instructor
WHERE salary > ALL (SELECT salary FROM instructor WHERE dept_name = 'Biology');
```

**记忆窍门**：
- `= SOME` ⇔ `IN`
- `<> ALL` ⇔ `NOT IN`
- `> ALL` = 大于最大值
- `> SOME` = 大于最小值

#### 模板6：WITH子句（定义临时表）
```sql
WITH dept_avg(dept_name, avg_sal) AS (
    SELECT dept_name, AVG(salary) FROM instructor GROUP BY dept_name
)
SELECT D.dept_name, D.budget
FROM department D, dept_avg A
WHERE D.dept_name = A.dept_name AND A.avg_sal > 50000;
```

#### 模板7：FROM子句中的子查询
```sql
-- 查出平均工资超过42000的系
SELECT dept_name, avg_sal
FROM (
    SELECT dept_name, AVG(salary) AS avg_sal
    FROM instructor
    GROUP BY dept_name
) AS dept_avg
WHERE avg_sal > 42000;
```

---

## 四、JOIN 全类型汇总

### 4.1 JOIN语法对比

```sql
-- 1. 隐式JOIN（旧式，不推荐）
SELECT name, title
FROM student, takes, course
WHERE student.ID = takes.ID AND takes.course_id = course.course_id;

-- 2. 自然连接（自动用同名属性连接）
SELECT name, title
FROM student NATURAL JOIN takes NATURAL JOIN course;
-- 危险！如果student和course都有dept_name，会意外多一个连接条件

-- 3. JOIN ... USING（指定连接属性名）
SELECT name, title
FROM (student NATURAL JOIN takes) JOIN course USING (course_id);

-- 4. JOIN ... ON（最明确，推荐）
SELECT name, title
FROM student
JOIN takes ON student.ID = takes.ID
JOIN course ON takes.course_id = course.course_id;
```

### 4.2 外连接

```sql
-- LEFT JOIN：保留左表全部行
-- 查所有学生(包括没选课的)
SELECT S.name, T.course_id
FROM student S LEFT JOIN takes T ON S.ID = T.ID;

-- 外连接中的ON vs WHERE（高频易错点）
SELECT * FROM student LEFT JOIN takes ON student.ID = takes.ID
WHERE takes.year = 2018;
-- 上面这个WHERE会把NULL行过滤掉，效果等同INNER JOIN！

-- 正确做法：把条件放在ON里
SELECT * FROM student LEFT JOIN takes 
ON student.ID = takes.ID AND takes.year = 2018;
-- 只在连接匹配时要求year=2018，没选课的学生仍然保留
```

---

## 五、NULL值处理全集

```sql
-- 比较NULL
WHERE grade IS NULL;        -- 正确 ✓
WHERE grade IS NOT NULL;    -- 正确 ✓
WHERE grade = NULL;         -- 错误 ✗ (永远返回unknown)

-- 算术运算
SELECT salary + NULL FROM instructor;  -- 结果全是NULL

-- 聚集函数与NULL
COUNT(*)       -- 计数所有行（含NULL）
COUNT(grade)   -- 计数grade非NULL的行
AVG(salary)    -- 忽略NULL记录
SUM(salary)    -- 忽略NULL记录

-- NULL在HAVING和WHERE中的三值逻辑
-- 只有结果为true的行才满足条件，unknown的行被排除
```

---

## 六、视图与权限

### 视图关键点

```sql
CREATE VIEW faculty AS
SELECT ID, name, dept_name FROM instructor;

-- 视图可更新条件（全部满足）：
-- 1. FROM只有一个基本表
-- 2. SELECT只有属性名（无表达式/聚集/DISTINCT）
-- 3. 没有GROUP BY或HAVING
-- 4. 没有子查询
```

**如果视图包含聚集/DISTINCT/多表：不可更新，但可查询。**

### 授权关键语法

```sql
GRANT SELECT ON department TO Amit;
GRANT SELECT ON department TO PUBLIC;
GRANT SELECT ON department TO Amit WITH GRANT OPTION;

REVOKE SELECT ON department FROM Amit CASCADE;   -- 级联回收
REVOKE SELECT ON department FROM Amit RESTRICT;  -- 不级联
REVOKE GRANT OPTION FOR SELECT ON department FROM Amit; -- 只回收再授权

CREATE ROLE instructor_role;
GRANT SELECT ON takes TO instructor_role;
GRANT instructor_role TO Amit;  -- 角色授权给用户
```

---

## 七、函数与过程 [重点]

```sql
-- 函数（返回单个值）
CREATE FUNCTION dept_count(dname VARCHAR(20))
RETURNS INTEGER
BEGIN
    DECLARE d_count INTEGER;
    SELECT COUNT(*) INTO d_count
    FROM instructor
    WHERE instructor.dept_name = dname;
    RETURN d_count;
END;

-- 表函数（返回表）
CREATE FUNCTION instructor_of(dname VARCHAR(20))
RETURNS TABLE (...)
RETURN TABLE (
    SELECT ID, name, dept_name, salary
    FROM instructor WHERE dept_name = dname
);
-- 调用：SELECT * FROM TABLE(instructor_of('CS'));

-- 过程（用CALL调用）
CREATE PROCEDURE dept_count_proc(
    IN dname VARCHAR(20), 
    OUT d_count INTEGER
)
BEGIN
    SELECT COUNT(*) INTO d_count
    FROM instructor WHERE dept_name = dname;
END;
-- 调用：CALL dept_count_proc('CS', @count);

-- 函数 vs 过程的区别：
-- 函数：有返回值，在表达式中使用（如WHERE子句中）
-- 过程：无返回值/通过OUT参数返回，用CALL显式调用
```

---

## 八、窗口函数与OLAP

```sql
-- 排名
SELECT ID, 
    RANK() OVER (ORDER BY GPA DESC) AS rank,          -- 1,1,3
    DENSE_RANK() OVER (ORDER BY GPA DESC) AS d_rank  -- 1,1,2
FROM student_grades;

-- 分区排名（系内排名）
SELECT ID, dept_name,
    RANK() OVER (PARTITION BY dept_name ORDER BY GPA DESC) AS dept_rank
FROM student_grades;

-- 窗口聚合
SELECT date, value,
    SUM(value) OVER (ORDER BY date ROWS BETWEEN 1 PRECEDING AND 1 FOLLOWING) AS moving_avg_3day
FROM sales;

-- CUBE/ROLLUP
GROUP BY CUBE(a, b, c)    -- 生成2^3=8种分组
GROUP BY ROLLUP(a, b, c)  -- 生成3+1=4种分组
```

---

## 九、触发器

```sql
-- BEFORE触发器：修改新数据
CREATE TRIGGER setnull_before_update
BEFORE UPDATE OF takes
REFERENCING NEW ROW AS nrow
FOR EACH ROW
WHEN (nrow.grade = ' ')
BEGIN ATOMIC
    SET nrow.grade = NULL;
END;

-- AFTER触发器：维护其他表
CREATE TRIGGER credits_earned
AFTER UPDATE OF takes ON (grade)
REFERENCING NEW ROW AS nrow
REFERENCING OLD ROW AS orow
FOR EACH ROW
WHEN (nrow.grade <> 'F' AND nrow.grade IS NOT NULL
      AND (orow.grade = 'F' OR orow.grade IS NULL))
BEGIN ATOMIC
    UPDATE student SET tot_cred = tot_cred +
        (SELECT credits FROM course WHERE course.course_id = nrow.course_id)
    WHERE student.ID = nrow.ID;
END;
```

**触发器核心概念**：
- BEFORE：可以修改即将写入的新数据
- AFTER：用于维护关联表（如更新学分、审计日志）
- FOR EACH ROW vs FOR EACH STATEMENT：行级 vs 语句级
- REFERENCING OLD ROW / NEW ROW：修改前后的值

---

## 十、SQL易错TOP10

1. `WHERE grade = NULL` → 永远是unknown，必须用 `IS NULL`
2. SELECT中在聚集函数外的属性必须出现在GROUP BY中
3. 外连接后用WHERE过滤NULL列 → 等价于内连接
4. 自然连接遇到同名异义属性 → 产生意外过滤
5. `= SOME` 等价于 `IN`，`<> ALL` 等价于 `NOT IN`
6. UNION去重，UNION ALL保留重复
7. RANK有空缺(1,1,3)，DENSE_RANK无空缺(1,1,2)
8. Delete vs Drop: Delete只删数据，Drop删表结构和数据
9. 视图有聚集/DISTINCT时不能更新
10. CUBE(2^n)和ROLLUP(n+1)分组数不同

---

## 十一、历年SQL真题精练

### 2020-Q1/Q2/Q3 典型SQL题

```sql
-- Q: 找出每个系工资最高的教师姓名和工资
SELECT name, salary
FROM instructor I1
WHERE salary = (
    SELECT MAX(salary) FROM instructor I2 
    WHERE I2.dept_name = I1.dept_name
);

-- Q: 用WITH重写
WITH dept_max(dept_name, max_sal) AS (
    SELECT dept_name, MAX(salary) FROM instructor GROUP BY dept_name
)
SELECT I.name, I.salary
FROM instructor I, dept_max M
WHERE I.dept_name = M.dept_name AND I.salary = M.max_sal;
```

### 经典SQL难题集

```sql
-- 1. 查出没有教过2008年任何课的教师
SELECT name FROM instructor I
WHERE NOT EXISTS (
    SELECT * FROM teaches T
    WHERE T.ID = I.ID AND T.year = 2008
);

-- 2. 查出至少教过Comp.Sci系所有课程的教师（全称命题）
SELECT I.name FROM instructor I
WHERE NOT EXISTS (
    (SELECT course_id FROM course WHERE dept_name = 'Comp.Sci')
    EXCEPT
    (SELECT T.course_id FROM teaches T WHERE T.ID = I.ID)
);

-- 3. 查出工资比本系平均工资高的教师
SELECT name, salary FROM instructor I1
WHERE salary > (
    SELECT AVG(salary) FROM instructor I2
    WHERE I2.dept_name = I1.dept_name
);

-- 4. 查出每个系学生人数超过500的系
SELECT dept_name, COUNT(*) FROM student
GROUP BY dept_name HAVING COUNT(*) > 500;

-- 5. 更新所有工资低于平均值的教师涨5%
UPDATE instructor SET salary = salary * 1.05
WHERE salary < (SELECT AVG(salary) FROM instructor);
```

---

## 十二、SQL防御性编程 [必考]

### SQL注入防御

```java
// 错误做法 ✗
String query = "SELECT * FROM users WHERE name = '" + userName + "'";
Statement stmt = conn.createStatement();
ResultSet rs = stmt.executeQuery(query);

// 正确做法 ✓
String query = "SELECT * FROM users WHERE name = ?";
PreparedStatement pstmt = conn.prepareStatement(query);
pstmt.setString(1, userName);
ResultSet rs = pstmt.executeQuery();
```

**为什么PreparedStatement安全？** 参数值永远不会被解释为SQL的一部分。用户名 `' OR '1'='1' --` 只会被当作字面字符串匹配，不会闭合引号注入代码。
