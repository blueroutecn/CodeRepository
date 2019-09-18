'''
from pyecharts import Bar
Bar
        .add(name, x_axis, y_axis, **kwargs)
 => name(str),图例名称；x_axis & y_axis([]),x、y坐标值；
 => xaxis_interval,   x轴标题间距；xaxis_rotate, x轴标题旋转角度；y轴同理；
 => xaxis_line_color, x轴颜色；xaxis_line_width, x轴线条宽度；xaxis_label_textcolor, x轴标签颜色。

 => is_stack, 是否堆叠柱状图；bar_category,柱状图间隙 ；
 => make_point["max", "min"], 标记特征点；make_line["average"], 标记特征线。

 => is_convert,是否交换xy轴。

 => is_datazoom_show, 是否使用数据缩放效果；
 => datazoom_type="inside"，缩放特效类型“inside”；datazoom_range=[20,50]，展示范围；
 => is_datazoom_extra_show, 是否增加额外的数据缩放效果；
 => datazoom_extra_type="slider"，额外的zoom控制条类型。

 => is_more_utils, 是否打开所有工具；is_toolbox_show, 是否展示工具盒。

'''


''' sample 1 —— draw html chart table（普通柱状图）.'''
# from pyecharts import Bar
#
# bar = Bar("First Chart Table", "pyecharts realizaton")  # 设置主标题和副标题。
# bar.use_theme("light")
# bar.add("cloth", ["shirts", "pants", "jacks", "shoes", "hats", "gloves"], [5, 20, 36, 10, 75, 90],
#         is_more_utils=True)                            # 图例名称，横坐标，纵坐标，是否需要更多工具。
# # bar.print_echarts_options()
# bar.render()                                           # 在执行目录下生成html图表文件。


''' sample 2 —— draw stacked figure（堆叠柱状图,链式调用）.'''
# from pyecharts import Bar
#
# CLOTHES = ["shirts", "pants", "jacks", "shoes", "hats", "gloves"]
# clothes_1 = [5, 20, 36, 10, 75, 90]
# clothes_2 = [10, 25, 8, 60, 20, 80]
# (Bar("柱状图数据堆叠示例"))\
#         .add("商家1", CLOTHES, clothes_1, is_stack=True, is_more_utils=True, is_datazoom_show=True)\
#         .add("商家2", CLOTHES, clothes_2, is_stack=True, is_more_utils=True, is_toolbox_show=False)\
#         .render()                                      # Bar中use_theme,add,render函数均支持链式调用。


''' sample 3 —— draw many charts use one engine（同一个绘图引擎绘制多个图表）.'''
# from pyecharts import Bar, Line
# from pyecharts.engine import create_default_environment
#
# bar = Bar("First Chart Table", "pyecharts implement")
# bar.add("clothes", ["shirts", "pants", "jacks", "shoes", "hats", "gloves"], [5, 20, 36, 10, 75, 90])
#
# line = Line("First Chart Table", "pyecharts implement")
# line.add("clothes", ["shirts", "pants", "jacks", "shoes", "hats", "gloves"], [5, 20, 36, 10, 75, 90])
#
# env = create_default_environment("html")
# env.render_chart_to_file(bar, path='bar.html')
# env.render_chart_to_file(line, path='line.html')


''' sample 4 —— draw respectively figure（绘制分列图）.'''
# from pyecharts import Bar
#
# attr = ["{}月".format(i)for i in range(1, 13)]
# v1 = [1, 2, 3, 4, 12, 46, 23, 14, 8, 4, 2, 1]
# v2 = [(i+y) for i, y in zip(v1, range(12))]
#
# bar = Bar("柱状示意图")
# bar.add("蒸发量", attr, v1, mark_line=["average"], mark_point=["max", "min"])  # make_line绘制线条
# bar.add("降雨量", attr, v2, mark_line=["average"], mark_point=["max", "min"])  # make_point绘制点图
# bar.render()

