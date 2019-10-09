
class demo(object):
    def __init__(self):
        self.__txt = "this is demo"
        self._num = 20
        self.value = 10.5
        
    def __display_txt(self):
        print(self.__txt)

    def _display_num(self):
        print(self._num)

    def display_value(self):
        print(self.value)


class subdemo(demo):
    def __init__(self):
        super(subdemo, self).__init__()

    def display(self):
        # super(subdemo, self).__display_txt()  # 调用父类方法
        super(subdemo, self)._display_num()
        super(subdemo, self).display_value()
        
        # print(super(subdemo, self).__txt)   # 调用父类变量
        # print(super(subdemo, self)._num)
        # print(super(subdemo, self).value)
        
        # print(self.__txt)   # 调用父类变量
        print(self._num)
        print(self.value)
        
        print(self._demo__txt)   # 调用父类变量
        # print(self._demo__num)
        # print(self._demo__value)

'''
Demo = demo()
print(Demo.__txt)
Demo.__display_txt()
print(Demo._num)
Demo._display_num()
print(Demo.value)
Demo.display_value()
'''


subDemo = subdemo()
subDemo.display()
