#-*- coding: gbk -*-




'''��дexcel����ļ� '''

import xlrd
import xlwt

# ��ȡ�������
def read_excel(path):
    data  = xlrd.open_workbook(path)          # ��excel����ȡ�ļ�
    tables = data.sheets()                    # ��ȡ���й�����
    names = data.sheet_names()                # ��ȡ���й���������
    
    table  = tables[0]                        # ��ȡ����һ��������
    #table = data.sheets()[0]                 
    #table = data.sheet_by_index(0)           # ͨ������ֱ����ȡһ��������
    #table = data.sheet_by_name(names[0])     # ͨ���ֱ�����ֱ����ȡһ��������
    #table = data.sheet_by_name(u'Sheet1')

    num_row, num_col=table.nrows, table.ncols # ��ȡ�����������������    

    data_row = table.row_values(0)            # ��ȡ�������һ�е�ֵ����������
    data_col = table.col_values(0)            # ��ȡ�������һ�е�ֵ����������

    value = table.cell(0,0).value             # ��ȡ�������һ���ֵ

# д��������
def write_excel(path):
    excel = xlwt.workbook(encoding='ascii')    # �������
    sheet = excel.add_sheet('my excel')        # ����������
    sheet.write(0,0,label='row 0,col 0,value') # ��Ԫ��д������
    excel.save(path + "ExcelDemo.xls")         # ������



import xlwings as xlw

xlwings���߼��ṹ:
excel��Ŀ(�ļ���)  --> excel�ļ�  --> sheet������  --> range����

xlwings  --> app1  --> book1_1  --> sheet1_1_1  --> range1_1_1_1
             app2      book1_2      sheet1_1_2      range1_1_1_2
             app3      book1_3      sheet1_1_3      range1_1_1_3
             ...       ...          ...             ...

def rw_excel_xlw(path=[]):
    if path is None:  path = []

    # �򿪱��  
    app = xlw.App(visible=True, add_book=False) # visible���Ƿ�ɼ���add_book�Ƿ��½����
    wb = app.books.open(path[0])                # �򿪱��1
    wb2 = app.books.open(path[1])    
    wb3 = app.books.add()                       # ���һ���µı��
    wb3.save('./myexcel.xlsx')                  # �����½��ı��
    wb3.close()
    
    # �������ⲿ����
    wb.sheets.add("sheet2")                     # ���1���½�������sheet2��
    wb.sheets["sheet2"].delete()                # ɾ�����1�й�����sheet2��
    print(wb.sheets[1].name)                    # �鿴��ǰ�����
    wb.sheets[1].name = "new sheet1"            # ��������������
    wb.sheets[1].clear()                        # ��յ�ǰ������
    print(wb.sheets.active)                     # ��ӡ��ǰ��Ծ�Ĺ�����
    wb.sheets["new sheet1"].activate()          # ����ָ���Ĺ�����

    # ��ȡ����������
    sht = wb.sheets["new sheet1"]               # ��ȡָ��������
    rng_a1 = sht.range("A1")                    # ��ȡ��������A1��Ԫ��
    rng_a1_value = sht.range("A1").value        # ��ȡ��������A1��Ԫ���ֵ
    rng_a2 = sht["A2"]
    rng_c1 = sht[0, 2]                          # ��ȡ��������C1��Ԫ��(��1����3)
    rng_a1_b3 = sht["A1:B3"]                    # ��ȡ��������A1:B3����ĵ�Ԫ��
    rng_a2_b4 = sht.range("A2:B4").value        # ��ȡ��������A2:B4����ĵ�Ԫ���ֵ

    # ��������д������
    sht.range("A1").value = "A1"                # �������е�Ԫ��A1д������(A1 = "A1")
    sht.range("A2").value = [1,2]               # �ӹ������е�Ԫ��A2��ʼ��������д������(A2=1, B2=2)
    sht.range("A3").options(transpose=True).value = [3,4,5] # �ӹ������е�Ԫ��A3��ʼ��������д������(A3=3, A4=4, A5=5)
    sht.range("A4").options(expand="table").value = [[1,2], [3,4]] # �ӹ�����Ԫ��A4��չд��2*2��������(A4=1,B4=2, A5=3, B5=4)
    #sht.range("A4:B5").value = [[1,2], [3,4]]
    
    # ������
    wb.save()                                   # ������1
    wb.close()                                  # �رձ��1
    #app.books[0].close()                       # �رձ��1

    # ����ļ�����
    print(app.books.active)                     # ��ӡ��ǰ��Ծ�ı��
    print(app.books)                            # ��ӡ��ǰ�����б��
    app.quit()                                  # �ر�app






'''��д�������ļ� '''

import struct
import numpy as np

a = np.arange(3*4, dtype=np.int64).reshape((3,4))     # ����һ��3*4����������64λ��
with open("sample_struct.dat", "wb") as f:            # �Զ�����д��ķ�ʽ�����ļ�
    for row in range(3):
        for col in range(4):
            sn = struct.pack('i', a[row][col])        # ���������л���i��ʾ������f��ʾ������
            f.write(sn)


b = np.zeros((3, 4), dtype=np.int64)                  # ����һ��3*4�����������64λ��
with open("sample_struct.dat", "rb") as f:            # �Զ����ƶ�ȡ�ķ�ʽ���ļ�
    for row in range(3):
        for col in range(4):
            sn = f.read(4)                            # һ�ζ�ȡ4�ֽ�
            b[row][col], = struct.unpack('i', sn)     #  b[row][col], = xxx������


