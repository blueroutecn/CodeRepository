import sqlite3 as sql

'''
在使用SQLite前，我们先要搞清楚几个概念：

表是数据库中存放关系数据的集合，一个数据库里面通常都包含多个表。表和表之间通过外键关联。

要操作关系数据库，首先需要连接到数据库，一个数据库连接称为Connection；

连接到数据库后，需要打开游标，称之为Cursor，通过Cursor执行SQL语句，然后，获得执行结果。

Python定义了一套操作数据库的API接口，任何数据库要连接到Python，只需要提供符合Python标准的数据库驱动即可。


使用Python的DB-API时，只要搞清楚Connection和Cursor对象，打开后一定记得关闭，否则资源会泄漏。

使用Cursor对象执行insert，update，delete语句时，执行结果由rowcount返回影响的行数，就可以拿到执行结果。

使用Cursor对象执行select语句时，通过featchall()拿到结果集（一个list），每个元素都是一个tuple，对应一行记录。

如果SQL语句带有参数，那么需要把参数按照位置传递给execute()方法，有几个?占位符就必须对应几个参数。
'''


'''
1	sqlite3.connect(database [,timeout ,other optional arguments])
该 API 打开一个到 SQLite 数据库文件 database 的链接。
可以使用 ":memory:" 在 RAM 中打开一个到 database 的数据库连接，而不是在磁盘上打开。如果数据库成功打开，返回一个连接对象。

当一个数据库被多个连接访问，且其中一个修改了数据库，此时 SQLite 数据库被锁定，直到事务提交。
timeout 参数表示连接等待锁定的持续时间，直到发生异常断开连接。timeout 参数默认是 5.0（5 秒）。

如果给定的数据库名称 filename 不存在，则该调用将创建一个数据库。
如果不想在当前目录中创建数据库，那么可以指定带有路径的文件名，这样就能在任意地方创建数据库。

2	connection.cursor([cursorClass])
该例程创建一个 cursor，将在 Python 数据库编程中用到。

3	cursor.execute(sql [, optional parameters])
该例程执行一个 SQL 语句。该 SQL 语句可以被参数化（即用占位符代替 SQL 文本）。

4	connection.execute(sql [, optional parameters])
该例程是上面执行的由光标（cursor）对象提供的方法的快捷方式，
它通过调用光标（cursor）方法创建了一个中间的光标对象，然后通过给定的参数调用光标的 execute 方法。

5	cursor.executemany(sql, seq_of_parameters)
该例程对 seq_of_parameters 中的所有参数或映射执行一个 SQL 命令。

6	connection.executemany(sql[, parameters])
该例程是一个由调用光标（cursor）方法创建的中间的光标对象的快捷方式，然后通过给定的参数调用光标的 executemany 方法。

7	cursor.executescript(sql_script)
该例程一旦接收到脚本，会执行多个 SQL 语句。它首先执行 COMMIT 语句，然后执行作为参数传入的 SQL 脚本。

8	connection.executescript(sql_script)
该例程是一个由调用光标（cursor）方法创建的中间的光标对象的快捷方式，然后通过给定的参数调用光标的 executescript 方法。

9	connection.total_changes()
该例程返回自数据库连接打开以来被修改、插入或删除的数据库总行数。

10	connection.commit()
该方法提交当前的事务。如果未调用该方法，那么自上一次调用 commit() 以来所做的任何动作对其他数据库连接来说是不可见的。

11	connection.rollback()
该方法回滚自上一次调用 commit() 以来对数据库所做的更改。

12	connection.close()
该方法关闭数据库连接。请注意，这不会自动调用 commit()。
如果之前未调用 commit() 方法，就直接关闭数据库连接，所做的所有更改将全部丢失！

13	cursor.fetchone()
该方法获取查询结果集中的下一行，返回一个单一的序列，当没有更多可用的数据时，则返回 None。

14	cursor.fetchmany([size=cursor.arraysize])
该方法获取查询结果集中的下一行组，返回一个列表。当没有更多的可用的行时，则返回一个空的列表。
该方法尝试获取由 size 参数指定的尽可能多的行。

15	cursor.fetchall()
该例程获取查询结果集中所有（剩余）的行，返回一个列表。当没有可用的行时，则返回一个空的列表。
'''


'''
数据库语法详解
SQL
structured query language 结构化查询语言。专门对数据库进行查找、增加、修改、删除、统计的操作语言。
CURD 增删查改 create update retrieve delete。
书写风格，关键字大小写都行，建议大写。表名大小写都行，但是在一些数据库中不区分大小写，建议小写。

（重要）基本语法。
1. 查找
例句："SELECT * FROM student;" 查询所有
SELECT 字段1,字段2,字段3,...  FROM 表名;      python中返回值形如[(1, 502班, 小明, 男), (), ()]。
字段比较多时简写为  SELECT * FROM 表名;       由于数据库执行时会把*转换为字段再执行，性能极微小下降。
SELECT * FROM 表名 WHERE 字段1 = 过滤值,字典2=过滤值 ;       where限定条件查找。
2. 添加
例句："INSERT INTO student (id name) VALUES (1,"小红");"
INSERT INTO 表名 字段1,字段2,...   VALUES (1, "小明", "男");
简写 INSERT INTO 表名 VALUES (1, "小明", "男");
3. 修改
例句："UPDATE 表名 SET name="wode" WHERE id=1;"
UPDATE 表名 SET 字段1=新值,字段2=新值 WHERE 字段1 = 过滤值;
注意没有where条件限定行的话将会更新整张表。
4. 删除
例句："DELETE FROM student;"
DELETE FROM 表名;                 注意会删除整张表。
DELETE FROM 表名 WHERE 字段1 = 过滤值;       限定条件删除某些行。
5. 创建表
CREATE TABLE 表名 {
    字段类型 字段名 其它关键字（主键 备注），
    INT   id   PRIMARY KEY,
    VARCHAR(20) username ，
}

数据库字段类型
数据库创建表示要事先告诉计算机硬盘，每张表的每个字段存什么类型、将要存储的内容大还是小。长度过小，内容值存不进去，长度太大，浪费硬盘空间。我们需要选择合适的类型和长度。
各数据库字段类型关键字基本一致。mysql oracle postgresql常用字段类型如下：
1. 整数
整数：常用INT   INTEGER   占4个字节，2**32，可以表示常用范围整数。
（不常用）TINYINT（1字节）  SMALLINT（2字节） MEDIUMINT（3字节）
BIGINT（8字节） 适用身份证号、VIP号码比较长的编号。
2. 浮点数
常用 FLOAT(4字节) 单精度小数 。 即使是单精度，范围也不小。
DOUBLE(8字节) 双精度小数。
场景 金钱计算，轨道计算。
3. 字符串
CHAR  char(10) 可以存储长度（字节长度）不超过10的字符串。例如"hello"。但由于长度按照字节判断，存unicode编码的中文只能存3个。
常用  VARCHAR 0-65535字节，variable char 可变字符串。VARCHAR(5)  可以存储5个中文或5个英文字母。场景 用户名、家庭住址。
TEXT TINYTEXT  medium  longtext ， 场景 大文本存储，书籍文章、用户反馈。
BLOB medium  longblob ，二进制文件， 场景 图片、视频。但一般不在数据库中存储图片和视频
因为会增加数据库的计算压力和带宽传输压力和备份还原的难度和用户信息静态资源耦合到一起，解决方案是 图片视频存到普通文件目录下，数据库中存储文件路径。
4. 日期
DATE    日期， 形如"2018-11-08"
常用 DATETIME  日期时间， "2018-11-08 16:52:30"  "2018-11-08 16:52:30.123" "2018-11-08 16:52:30 GTM+8"
常用 TIMESTAMP 时间戳, 1541667270  1541667270.7252207   1541667270725

sqlite的字段比较简化：
INTEGER     整数
REAL        浮点数
TEXT        字符串
NULL        什么都不存
'''


# 打开或创建数据库，获取游标
conn = sql.connect("DBteset.db")
c = conn.cursor()
print("open database successfully.")

# 创建表
# c.execute(
#     "                             \
#     CREATE TABLE COMPANY(         \
#     ID INT PRIMARY KEY  NOT NULL, \
#     NAME TEXT NOT NULL,           \
#     AGE INT NOT NULL,             \
#     ADDRESS CHAR(50),             \
#     SALARY REAL );                \
#     "
# )
# conn.commit()
# print("table created successfully.")

# 插入数据
# c.execute(
#     "                                                     \
#     INSERT INTO COMPANY (ID, NAME, AGE, ADDRESS, SALARY)  \
#     VALUES (1, 'Paul', 32, 'California', 20000.0);        \
#     "
# )
# c.execute(
#     "  INSERT INTO COMPANY "
#     "  VALUES(2,'Allen',25,'Texas',15000.0)  "   # 这样写有隐患，有的语句无法执行。
# )
# conn.commit()
# print("records created successfully.")

# 选择操作
# cursor = c.execute( "SELECT  ID, NAME, ADDRESS, SALARY   FROM COMPANY" )
# for row in cursor:
#     line = "ID=%d, NAME=%s, ADDRESS=%s, SALARY=%f\n" % (row[0], row[1], row[2], row[3])
#     print(line)
# print("operation done successfully.")

# 更新操作
# c.execute(
#     "UPDATE COMPANY         \
#      SET SALARY = 250000.0  \
#      WHERE ID = 1"
# )
# conn.commit()
# print("total number of rows updated: ", conn.total_changes)

# 删除操作
# c.execute(
#     ""
#     "DELETE FROM COMPANY  \
#      WHERE ID = 2;        \
#     "
# )
# conn.commit()
# print("total number of rows deleted: ", conn.total_changes)
#
# cursor = c.execute(
#     "                                \
#     SELECT ID, NAME, ADDRESS, SALARY \
#     FROM COMPANY                     \
#     "
# )
# for row in cursor:
#     line = "ID=%d, NAME=%s, ADDRESS=%s, SALARY=%f\n" % (row[0], row[1], row[2], row[3])
#     print(line)


conn.close()











