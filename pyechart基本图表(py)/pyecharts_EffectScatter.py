'''
from pyecharts import EffectScatter
EffectScatter
            .add(name, x_axis, y_axis, symbol_size=10, **param)
=> name(str), 图例名称；
=> x_axis/y_axis(list), x/y 坐标轴数据，可以是单值也可以是列表；

=> symbol_size(int,default=10), 标记图形大小；
=> symbol(str), 标记图形类型，pin/rect/roundRect/diamond/arrow/triangle；
=> effect_scale(float,default=2.5), 波纹最大缩放比例；
=> effect_period(float,default=4), 动画持续时间；
=> effect_brushtype(str,default=stroke), 波纹绘制方式，stroke/fill。
'''


'''sample 1 —— draw effected scatter chart.(绘制带有涟漪效果的散点图)'''
# from pyecharts import EffectScatter
#
# v1 = [10, 20, 30, 40, 50, 60]
# v2 = [25, 20, 15, 10, 60, 33]
# es = EffectScatter("动态散点图示例")
# es.add("effectScatter", v1, v2)
# es.render()

''' sample 2 —— draw various styles effected scatter charts.(绘制各种效果的涟漪图)'''
# from pyecharts import EffectScatter
#
# es = EffectScatter("动态散点图各种图形示例")
# es.add("", [10], [10], symbol_size=20, effect_scale=3.5, effect_period=3, symbol="pin")
# es.add("", [20], [20], symbol_size=12, effect_scale=4.5, effect_period=4, symbol="rect")
# es.add("", [30], [30], symbol_size=20, effect_scale=2.5, effect_period=5, symbol="roundRect")
# es.add("", [40], [40], symbol_size=10, effect_scale=6.5, effect_brushtype="stroke", symbol="diamond")
# es.add("", [50], [50], symbol_size=16, effect_scale=3.0, effect_period=3, symbol="arrow")
# es.add("", [60], [60], symbol_size=10, effect_scale=4.5, effect_period=3, symbol="triangle")
# es.render()

