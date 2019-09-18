'''
from pyecharts import Scatter3D
Scatter
            .add(name, data, grid3d_opacity=1, **param)
=> name(str), 图例名称；
=> data([][]), 每一行是一个数据项，每一列是一个维度；
=> grid3d_opacity(default=1), 点的透明度，默认为1（完全不透明）；
'''

from pyecharts import Scatter3D


'''sample 1 —— draw scatter3D chart.(绘制普通3D散点图)'''
# import random
# data = [
#     [random.randint(0, 100),
#      random.randint(0, 100),
#      random.randint(0, 100)] for _ in range(80)]
# range_color = [
#     '#313695', '#4575b4', '#74add1', '#abd9e9', '#e0f3f8', '#ffffbf',
#     '#fee090', '#fdae61', '#f46d43', '#d73027', '#a50026']
#
# scatter3d = Scatter3D("3d散点图", width=1200, height=600)
# scatter3d.add("", data, isvisualmap=True, visual_range_color=range_color)
# scatter3d.render()



