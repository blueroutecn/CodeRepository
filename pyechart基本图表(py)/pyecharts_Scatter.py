'''
from pyecharts import Scatter
Scatter
            .add(name, x_axis, y_axis, extra_data=None, symbol_size=10, **param)
=> name(str), 图例名称；
=> x_axis/y_axis(list), x/y 坐标轴数据，可以是单值也可以是列表；

=> extra_data(list[int],default=None), 第三维度数据，x轴为第一维度，y轴为第二维度；
=> extra_name(str)，额外的数据项的名称，可以为每一个点指定一个名称；
=> symbol_size(int,default=10), 标记图形大小；

            .draw(path, color=None)
=> path(str), 待转换图片的地址；
=> color(tuple) ,转换时要排除的颜色。
'''

from pyecharts import Scatter



'''sample 1 —— draw scatter chart.(绘制普通散点图)'''
# v1 = [10, 20, 30, 40, 50, 60]
# v2 = [10, 20, 30, 40, 50, 60]
#
# scatter = Scatter("散点图示例")
# scatter.add("data1", v1, v2)
# scatter.add("data2", v1[::-1], v2)           # v1[::-1]，将数据v1全部倒置。
# scatter.render()


'''sample 2 —— draw scatter chart with visualmap tool.'''
''' 启用visualmap组件，通过颜色映射散点数值。'''
# v1 = [10, 20, 30, 40, 50, 60]
# v2 = [10, 20, 30, 40, 50, 60]
#
# scatter = Scatter("散点图示例")
# scatter.add("data1", v1, v2)
# scatter.add("data2", v1[::-1], v2, is_visualmap=True)      # 启用visualmap组件。
# scatter.render()


'''sample 3 —— draw scatter chart with visualmap tool.'''
''' 启用visualmap组件，通过图形大小映射散点数值。'''
# v1 = [10, 20, 30, 40, 50, 60]
# v2 = [10, 20, 30, 40, 50, 60]
#
# scatter = Scatter("散点图示例")
# scatter.add("data1", v1, v2)
# scatter.add("data2", v1[::-1], v2, is_visualmap=True,       # 启用visualmap组件。
#             visual_type="size", visual_range_size=[10, 60]) # visual_range_size是指图形大小范围
# scatter.render()


'''sample 4 —— draw scatter chart with visualmap tool.'''
''' 启用visualmap组件，将横坐标修改为类目轴。'''
# v1 = [10, 20, 30, 40, 50, 60]
# v2 = [10, 20, 30, 40, 50, 60]
#
# scatter = Scatter("散点图示例")
# scatter.add("data1", ["a", "b", "c", "d", "e", "f"], v2)
# scatter.add("data2", ["a", "b", "c", "d", "e", "f"], v2[::-1], xaxis_type="category") # 修改横轴为类目轴。
# scatter.render()


'''sample 5 —— draw scatter chart from a picture.'''
''' 启用draw（），绘制图片。'''
# scatter = Scatter("散点图示例")
# v1, v2 = scatter.draw("./picture.png")  # 将图片中非白色区域转化为散点并返回两个数组。
# scatter.add("pyecharts", v1, v2, is_random=True) # 转化后的颜色随机。
# scatter.render()

