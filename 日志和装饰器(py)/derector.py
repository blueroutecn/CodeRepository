#-*- coding: gbk -*-


'''装饰器 '''
# 1.不能修改被装饰的函数的源代码
# 2.不能修改被装饰的函数的调用方式
# 3.满足1、2的情况下给程序增添功能



import time
# 场景一：被装饰函数无参、无返回
def timer1(func):
    def decoration():          # 内嵌函数
        start = time.time()
        func()                 # 被装饰函数
        stop = time.time()
        print("test1 running time: %f"%(stop-start))
    return decoration          # 返回的内嵌函数替换了被装饰函数time，在不改变time的代码和调用同时增加了time的功能

@timer1                        # 语法糖，标识装饰器
def test1():
    time.sleep(2)
    print("test1 is runing")
test1()


# 场景二：被装饰函数有参、无返回
def timer2(func):
    def decoration(*param,**kwargs):    # 装饰器有不定参数（*param不定元组参数，**kwargs不定字典参数）
        start = time.time()
        func(*param, **kwargs)          # 被装饰参数可以有不定的参数组合
        stop = time.time()
        print("test2 running time: %f"%(stop-start))
    return decoration

@timer2
def test2(*param):
    time.sleep(2)
    print("test2 is runing")
test2()


# 场景三：被装饰函数有参、有返回
def timer3(func):
    def decoration(*param, **kwargs):
        start = time.time()
        res = func(*param, **kwargs)    # 被装饰函数有返回值
        stop = time.time()
        print("test3 running time: %f"%(stop-start))
        return res                      # 装饰器返回值（保持原函数调用一致）
    return decoration

@timer3
def test3(*param):
    time.sleep(2)
    print("test3 is running")
    return "test3 run successfully"
print(test3())


# 场景四：装饰器有参数（一个装饰器修改多个函数，需要一个判断参数 =》 多一层函数嵌套）
def timer4(name):                         # 多加一层函数嵌套以接受判别参数
    def time_4(func):                     # 内部真正的装饰器封装
        def decoration(*param, **kwargs): # 装饰器
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
                return res               # 有返回值的被装饰函数的装饰器要返回其返回值
        return decoration                # 返回装饰器
    return time_4                        # 返回装饰器选择器

@timer4(name='test_1')                   # 带参数的装饰器
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




