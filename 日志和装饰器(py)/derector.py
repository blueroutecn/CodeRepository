#-*- coding: gbk -*-


'''װ���� '''
# 1.�����޸ı�װ�εĺ�����Դ����
# 2.�����޸ı�װ�εĺ����ĵ��÷�ʽ
# 3.����1��2������¸�����������



import time
# ����һ����װ�κ����޲Ρ��޷���
def timer1(func):
    def decoration():          # ��Ƕ����
        start = time.time()
        func()                 # ��װ�κ���
        stop = time.time()
        print("test1 running time: %f"%(stop-start))
    return decoration          # ���ص���Ƕ�����滻�˱�װ�κ���time���ڲ��ı�time�Ĵ���͵���ͬʱ������time�Ĺ���

@timer1                        # �﷨�ǣ���ʶװ����
def test1():
    time.sleep(2)
    print("test1 is runing")
test1()


# ����������װ�κ����вΡ��޷���
def timer2(func):
    def decoration(*param,**kwargs):    # װ�����в���������*param����Ԫ�������**kwargs�����ֵ������
        start = time.time()
        func(*param, **kwargs)          # ��װ�β��������в����Ĳ������
        stop = time.time()
        print("test2 running time: %f"%(stop-start))
    return decoration

@timer2
def test2(*param):
    time.sleep(2)
    print("test2 is runing")
test2()


# ����������װ�κ����вΡ��з���
def timer3(func):
    def decoration(*param, **kwargs):
        start = time.time()
        res = func(*param, **kwargs)    # ��װ�κ����з���ֵ
        stop = time.time()
        print("test3 running time: %f"%(stop-start))
        return res                      # װ��������ֵ������ԭ��������һ�£�
    return decoration

@timer3
def test3(*param):
    time.sleep(2)
    print("test3 is running")
    return "test3 run successfully"
print(test3())


# �����ģ�װ�����в�����һ��װ�����޸Ķ����������Ҫһ���жϲ��� =�� ��һ�㺯��Ƕ�ף�
def timer4(name):                         # ���һ�㺯��Ƕ���Խ����б����
    def time_4(func):                     # �ڲ�������װ������װ
        def decoration(*param, **kwargs): # װ����
            if name == "test_1" or name == "test_2":
                start = time.time()
                func(*param, **kwargs)
                stop = time.time()
                print("%s running time: %f"%(name, stop-start))
            elif name == "test_3":
                start = time.time()
                res = func(*param, **kwargs)
                stop = time.time()
                print("%s running time: %f"%(name, stop-start))
                return res               # �з���ֵ�ı�װ�κ�����װ����Ҫ�����䷵��ֵ
        return decoration                # ����װ����
    return time_4                        # ����װ����ѡ����

@timer4(name='test_1')                   # ��������װ����
def test_1():
    time.sleep(2)
    print("test_1 is runing")

@timer4(name='test_2')
def test_2(*param):
    time.sleep(2)
    print("test_2 is runing")

@timer4(name='test_3')
def test_3(*param):
    time.sleep(2)
    print("test_3 is running")
    return "test_3 run successfully"

test_1()
test_2()
print(test_3())




