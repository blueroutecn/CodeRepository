#-*- coding: gbk -*-




'''读写excel表格文件 '''

import xlrd
import xlwt

# 读取表格数据
def read_excel(path):
    data  = xlrd.open_workbook(path)          # 打开excel，读取文件
    tables = data.sheets()                    # 获取所有工作表
    names = data.sheet_names()                # 获取所有工作表名称
    
    table  = tables[0]                        # 提取其中一个工作表
    #table = data.sheets()[0]                 
    #table = data.sheet_by_index(0)           # 通过索引直接提取一个工作表
    #table = data.sheet_by_name(names[0])     # 通过分表名称直接提取一个工作表
    #table = data.sheet_by_name(u'Sheet1')

    num_row, num_col=table.nrows, table.ncols # 获取工作表的行数和列数    

    data_row = table.row_values(0)            # 获取工作表第一行的值，返回数组
    data_col = table.col_values(0)            # 获取工作表第一列的值，返回数组

    value = table.cell(0,0).value             # 获取工作表第一格的值

# 写入表格数据
def write_excel(path):
    excel = xlwt.workbook(encoding='ascii')    # 创建表格
    sheet = excel.add_sheet('my excel')        # 创建工作表
    sheet.write(0,0,label='row 0,col 0,value') # 向单元格写入内容
    excel.save(path + "ExcelDemo.xls")         # 保存表格



import xlwings as xlw

xlwings的逻辑结构:
excel项目(文件夹)  --> excel文件  --> sheet工作表  --> range内容

xlwings  --> app1  --> book1_1  --> sheet1_1_1  --> range1_1_1_1
             app2      book1_2      sheet1_1_2      range1_1_1_2
             app3      book1_3      sheet1_1_3      range1_1_1_3
             ...       ...          ...             ...

def rw_excel_xlw(path=[]):
    if path is None:  path = []

    # 打开表格  
    app = xlw.App(visible=True, add_book=False) # visible表是否可见，add_book是否新建表格
    wb = app.books.open(path[0])                # 打开表格1
    wb2 = app.books.open(path[1])    
    wb3 = app.books.add()                       # 添加一个新的表格
    wb3.save('./myexcel.xlsx')                  # 保存新建的表格
    wb3.close()
    
    # 工作表外部操作
    wb.sheets.add("sheet2")                     # 表格1中新建工作表“sheet2”
    wb.sheets["sheet2"].delete()                # 删除表格1中工作表“sheet2”
    print(wb.sheets[1].name)                    # 查看当前表格名
    wb.sheets[1].name = "new sheet1"            # 重命名工作表名
    wb.sheets[1].clear()                        # 清空当前工作表
    print(wb.sheets.active)                     # 打印当前活跃的工作表
    wb.sheets["new sheet1"].activate()          # 激活指定的工作表

    # 读取工作表内容
    sht = wb.sheets["new sheet1"]               # 获取指定工作表
    rng_a1 = sht.range("A1")                    # 获取工作表中A1单元格
    rng_a1_value = sht.range("A1").value        # 获取工作表中A1单元格的值
    rng_a2 = sht["A2"]
    rng_c1 = sht[0, 2]                          # 获取工作表中C1单元格(行1、列3)
    rng_a1_b3 = sht["A1:B3"]                    # 获取工作表中A1:B3区域的单元格
    rng_a2_b4 = sht.range("A2:B4").value        # 获取工作表中A2:B4区域的单元格的值

    # 向工作表中写入内容
    sht.range("A1").value = "A1"                # 向工作表中单元格A1写入数据(A1 = "A1")
    sht.range("A2").value = [1,2]               # 从工作表中单元格A2开始按行向右写入数据(A2=1, B2=2)
    sht.range("A3").options(transpose=True).value = [3,4,5] # 从工作表中单元格A3开始按列向下写入数据(A3=3, A4=4, A5=5)
    sht.range("A4").options(expand="table").value = [[1,2], [3,4]] # 从工作表单元格A4拓展写入2*2矩阵数据(A4=1,B4=2, A5=3, B5=4)
    #sht.range("A4:B5").value = [[1,2], [3,4]]
    
    # 保存表格
    wb.save()                                   # 保存表格1
    wb.close()                                  # 关闭表格1
    #app.books[0].close()                       # 关闭表格1

    # 表格文件操作
    print(app.books.active)                     # 打印当前活跃的表格
    print(app.books)                            # 打印当前的所有表格
    app.quit()                                  # 关闭app






'''读写二进制文件 '''

import struct
import numpy as np

a = np.arange(3*4, dtype=np.int64).reshape((3,4))     # 生成一个3*4的整数矩阵（64位）
with open("sample_struct.dat", "wb") as f:            # 以二进制写入的方式创建文件
    for row in range(3):
        for col in range(4):
            sn = struct.pack('i', a[row][col])        # 二进制序列化，i表示整数，f表示浮点数
            f.write(sn)


b = np.zeros((3, 4), dtype=np.int64)                  # 生成一个3*4的整数零矩阵（64位）
with open("sample_struct.dat", "rb") as f:            # 以二进制读取的方式打开文件
    for row in range(3):
        for col in range(4):
            sn = f.read(4)                            # 一次读取4字节
            b[row][col], = struct.unpack('i', sn)     #  b[row][col], = xxx，神奇


