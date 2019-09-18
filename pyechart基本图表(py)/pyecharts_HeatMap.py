'''
from pyecharts import HeatMap
Scatter
            .add(*args, **param)
=> name(str), 图例名称；
=> x_axis/y_axis(str), x/y 坐标轴数据，需要为类目轴（即不能为数值）；
=> data(list[int],default=None), 第三维度数据，x轴为第一维度，y轴为第二维度；

=> is_calendar_heatmap=True,
=> calendar_date_range(str/list), 日历热力图的日期;
=> calendar_cell_size(list), 日历每格框的大小，可设置单值或数组;第一个元素是宽,第二个元素是高.
'''


from pyecharts import HeatMap
import random


'''sample 1 —— draw heatmap chart.(绘制热力图)'''
# x_axis = [
#     "12a", "1a", "2a", "3a", "4a", "5a", "6a", "7a", "8a", "9a", "10a", "11a",
#     "12p", "1p", "2p", "3p", "4p", "5p", "6p", "7p", "8p", "9p", "10p", "11p"]
# y_axis = [
#     "Saturday", "Friday", "Thursday", "Wednesday", "Tuesday", "Monday", "Sunday"]
#
# data = [[i, j, random.randint(0, 50)] for i in range(24) for j in range(7)]
# heatmap = HeatMap()
# heatmap.add(
#     "热力图直角坐标系", x_axis, y_axis, data,
#     is_visualmap=True, visual_text_color="#000", visual_orient="horizontal")
# heatmap.render()


'''sample 2 —— draw calendar heatmap chart.(绘制日历热力图)'''
import datetime

begin = datetime.date(2018, 1, 1)
end = datetime.date(2018, 12, 31)

data = [
    [str(begin + datetime.timedelta(days=i)), random.randint(1000, 25000)]
    for i in range((end - begin).days + 1)]


heatmap = HeatMap("日历热力图示例", "2018 年微信运动", width=1100)
heatmap.add("", data,  is_calendar_heatmap=True,    visual_text_color="#000",
            visual_range_text=["", ""],    visual_range=[1000, 25000],
            calendar_cell_size=["auto", 30],    is_visualmap=True,
            calendar_date_range="2018",    visual_orient="horizontal",
            visual_pos="center",    visual_top="80%",    is_piecewise=True,)
heatmap.render()

