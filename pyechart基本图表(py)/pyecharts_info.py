
'''  ***************************************************************************  '''
'''  Chart General Options(图表通用配置) '''
'''  http://pyecharts.org/#/zh-cn/prepare '''
'''  ***************************************************************************  '''


'''  ***************************************************************************  '''
'''******  Chart Initialization(图表初始化，Bar/Bar3D/...)  ******'''
'''  ***************************************************************************  '''
'''
=> title(str,default=""),    主标题；subtitle(str,default=""),    副标题；
=> width(int,default=800px), 画布宽度；height(int,default=400px), 画布高度；

=> title_pos(str/int,default="left"), 标题距左侧距离,auto/left/right/center或整数；
=> title_top(str/int,default="top") , 标题距顶部距离,top/middle/bottom或整数；

=> title_color(str,default='#000'), 主标题颜色；subtitle_color(str,default='#aaa'), 副标题颜色；
=> title_text_size(int,default=18), 主标题字号；subtitle_text_size(int,default=12), 副标题字号；

=> background_color(str,default='#fff'), 画布背景颜色；
=> page_title(str,default='Echarts'), 指定生成的html文件中<title>标签的值；
=> extra_html_text_label(list),  额外的html文本标签（<p>, list[0]为文本内容，list[1]为字体风格样式）；

=> render(str,default='canvas'),    指定渲染方式，svg/canvas, 3D图只能使用canvas；
=> is_animation(bool,default=True), 是否开启动画。
'''


'''  ***************************************************************************  '''
'''******  Chart Options(图表配置, add())  ******'''
'''  ***************************************************************************  '''
'''
=> xyAxis：平面直角坐标系中的 x、y 轴。(Line、Bar、Scatter、EffectScatter、Kline)
=> dataZoom：dataZoom 组件用于区域缩放。(Line、Bar、Scatter、EffectScatter、Kline、Boxplot)
=> legend：图例组件, 展现了不同系列的标记(symbol)，颜色和名字。可以通过点击图例控制哪些系列不显示。
=> label：图形上的文本标签，可用于说明图形的一些数据信息，比如值，名称等。
=> lineStyle：带线图形的线的风格选项(Line、Polar、Radar、Graph、Parallel)
=> grid3D：3D笛卡尔坐标系组配置项，适用于 3D 图形。（Bar3D, Line3D, Scatter3D, Surface3D)
=> axis3D：3D笛卡尔坐标系 X，Y，Z轴配置项，适用于3D图形。（Bar3D, Line3D, Scatter3D, Surface3D)
=> visualMap：是视觉映射组件，用于进行『视觉编码』，也就是将数据映射到视觉元素（视觉通道）
=> markLine-markPoint：图形标记组件，用于标记指定的特殊数据，有标记线和标记点两种。（Bar、Line、Kline）
=> tooltip：提示框组件，用于移动或点击鼠标时弹出数据内容
=> toolbox：右侧实用工具箱
'''
'''
xyAxis, 平面直角坐标系中x、y轴（Line/Bar/Scatter/EffectScatter/Kline）。
***********************************************************************************************
=> x_axis(list), x轴数据项；
=> xaxis_pos(str), x轴位置，top/bottom；
=> is_xaxis_show(bool),    是否显示x轴；
=> is_splitline_show(bool,default=True), 是否显示x、y轴网格线；
=> xaxis_margin(int,default=8),   x轴刻度标签与轴线间距离；
=> xaxis_min/xaxis_max(int/float),x轴刻度最小/最大值，默认为自适应
=> xaxis_name(str), x轴名称；
=> xaxis_name_size(int,default=14), x轴名称字号；
=> xaxis_label_textsize(int,default=12), x轴标签字号；
=> xaxis_label_textcolor(str,default='#000'), x轴标签颜色；
=> xaxis_line_color(str,default=None), x轴轴线颜色；
=> xaxis_line_width(int,default=1), x轴轴线宽带；
=> xaxis_name_gap(int,default=25) , x轴名称与轴线间距离；
=> xaxis_formatter(str), x轴标签格式；
=> xaxis_name_pos(str), x轴名称位置，start/middle/end  ；
=> xaxis_type(str), x轴类型，value适合连续数据，category适合离散的类目数据，log适合对数数据；
=> is_convert(bool), 是否交换x轴和y轴；
=> xaxis_rotate(int,default=0), x轴标签旋转角度，-90~90；
=> is_xaxis_inverse/is_xaxis_inverse(bool,default=False), 是否反向x/y轴坐标；
=> is_xaxis_boundarygap(bool,default=True),x轴两边是否留白；
=> is_xaxislabel_align(bool,default=False), x轴刻度线和标签是否对齐；
=> xaxis_interval(int),x轴标签显示间隔，默认标签不重叠，为0强制显示所有标签，为1间隔1个显示...；

=> y轴同理。
'''
'''
dataZoom, 用于区域缩放，从而能自由关注细节的数据信息，或者概览数据整体，或者去除离群点的影响。
***********************************************************************************************
默认的dataZoom控制条：
=> is_datazoom_show(bool,default='False'), 是否使用datazoom组件；
=> datazoom_type(str,default='slider'), datazoom缩放类型，slider/inside/both；
=> datazoom_range(list,default=[50, 100]), datazoom缩放范围；
=> datazoom_orient(str,default='horizontal'), 组件在直角坐标系中的方向，horizontal/vertical；
=> datazoom_xaxis_index/datazoom_yaxis_index(int/list), datazoom组件控制的x/y轴索引；
额外的datazoom控制条：
=> is_datazoom_extra_show(bool,default='False'), 是否使用额外的datazoom组件；
=> datazoom_extra_type(str,default='slider'), 额外datazoom组件类型, slider/inside/both；
=> datazoom_extra_range(list,default=[50, 100]), datazoom缩放的范围；
=> datazoom_extra_orient(str,default='vertical'),组件在直角坐标系中的方向，默认在y轴方向；
=> datazoom_extra_xaxis_index/datazoom_extra_yaxis_index(int/list), 额外组件控制的x、y轴索引。
'''
'''
legend, 图例组件展现了不同系列的标记、颜色和名字；可以通过点击图例控制哪些系列不显示。
***********************************************************************************************
=> is_legend_show(bool,default=True), 是否显示顶端图例；
=> legend_orient(str,default='horizontal'), 图例列表的布局朝向， horizontal/vertical；
=> legend_pos(str,default='center'), 图例组件离容器左侧的距离，left/center/right，或60%等；
=> legend_top(str,default='top'), 图例组件离容器上侧的距离，top/center/bottom，或60%等；
=> legend_selectedmode(str/bool,default='multiple'), 图例控制是否改变，single/multiple, 或False；
=> legend_text_size(int), 图例名称字体大小；
=> legend_text_color(str), 图例名称字体颜色；
'''
'''
label, 图形上的文本标签, 用于说明图形的数据信息。
***********************************************************************************************
=> is_label_show(bool,default=False), 是否显示文本标签；
=> is_label_show(bool,default=True), 是否高亮显示文本标签；
=> label_pos(str,default='top'), 标签位置，top/left/right/bottom/inside/outside；
=> label_emphasis_pos(str,default='top'),高亮标签位置，top/left/right/bottom/inside/outside；
=> label_text_color(str,default='#000'), 标签字体颜色；
=> label_emphasis_textcolor(str,default='#fff'), 高亮标签字体颜色；
=> label_text_size(int,default=12), 标签字体大小；
=> label_emphasis_textsize(int,default=12), 高亮标签字体大小；
=> is_random(bool,default=False), 是否随机排列颜色列表；
=> label_color(list), 自定义标签颜色，全局颜色列表，所有图表的颜色都在这里调整；
=> label_formatter(str), 模板变量有 {a}{b}{c}{d}{e}，分别表示系列名，数据名，数据值等。
'''
'''
lineStyle, 带线图形的线的风格选项（Line、Polar、Radar、Graph、Parallel）。
***********************************************************************************************
=> line_width(int,default=1), 线的宽度；
=> line_opacity(float,default=1), 线的透明度，0为完全透明，1为完全不透明；
=> line_curve(float,default=0), 线的弯曲程度，0为完全不弯曲，1为最弯曲；
=> line_type(str,default='solid'), 线的类型，solid/dashed/dotted；
=> line_color(str), 线的颜色。
'''
'''
grid3D, 适用于 3D 图形（Bar3D, Line3D, Scatter3D, Surface3D)。
***********************************************************************************************
=> grid3d_width(int,default=100), 三维直角坐标系组件在三维场景中的宽度；
=> grid3d_height(int,default=100), 三维直角坐标系组件组件在三维场景中的高度；
=> grid3d_depth(int,default=100), 三维直角坐标系组件在三维场景中的深度；
=> is_grid3d_rotate(bool,default=False), 是否开启自旋转效果；
=> grid3d_rotate_speed(int,default=10), 自旋转速度；
=> grid3d_rotate_sensitivity(int,default=1), 旋转操作的敏感度，值越大越敏感，值为0无法旋转。
'''
'''
axis3D, 3D笛卡尔坐标系X，Y，Z轴配置项，适用于3D图形（Bar3D, Line3D, Scatter3D, Surface3D)。
***********************************************************************************************
3D x轴：
=> xaxis3d_name(str,default=''), x轴名称；
=> xaxis3d_name_size(int,default=10), x轴名称字体大小；
=> xaxis3d_name_gap(int,default=20), x轴名称与轴线间距；
=> xaxis3d_min/xaxis3d_max(int/float), x轴刻度最小/最大值；
=> xaxis3d_interval(int), x轴刻度标签的显示间隔；
=> xaxis3d_margin(int,default=0), x轴刻度标签与轴线间距离；
3D y轴：
=> yaxis3d_name(str,default=''), y轴名称；
=> yaxis3d_name_size(int,default=16), y轴名称字体大小；
=> yaxis3d_name_gap(int,default=25), y轴名称与轴线间距；
=> yaxis3d_min/yaxis3d_max(int/float), y轴刻度最小/最大值；
=> yaxis3d_interval(int), y轴刻度标签的显示间隔；
=> yaxis3d_margin(int,default=8), y轴刻度标签与轴线间距离；
3D z轴：
=> 与y轴相同。
'''
'''
visualMap, 视觉映射组件，就是将数据映射到视觉元素（视觉通道）。
***********************************************************************************************
=> is_visualmap(bool), 是否使用视觉映射组件；
=> visual_type(str,default='color'), 视觉映射方式，color通过颜色映射数值，size通过数值点大小；
=> visual_range(list,default=[0,100]), 视觉映射组件允许的最大值和最小值；
=> visual_text_color(list), 两端文本的颜色；
=> visual_range_text(list,default=['low','hight']), 两端文本；
=> visual_range_color(list,default=['#50a3ba','#eac763','#d94e5d']), 过度颜色；
=> visual_range_size(list,default=[20,50]), 数值映射的范围；
=> visual_orient(str,default='vertical'), 视觉映射组件条的方向，vertical/horizontal；
=> visual_pos(str/int,default='left'), 组件条距离左侧的距离，right/center/left, 或整数百分数；
=> visual_top(str/int,default='top'), 组件条距离顶部的位置，top/center/bottom, 或整数百分数；
=> visual_split_number(int,default=5), 分段型中分割的段数；
=> visual_dimension(int), 指定用数据的『哪个维度』，映射到视觉元素上；
=> is_calculable(bool,default=True), 是否显示拖拽的手柄；
=> is_piecewise(bool,default=False), 是否将组件转换为分段型（默认为连续型）；
=> pieces(list), 自定义『分段式视觉映射组件（visualMapPiecewise）』的每一段的范围；
'''
'''
tooltip, 提示框组件，用于移动或点击鼠标时弹出数据内容。
***********************************************************************************************
=> tooltip_trigger(str,default='item'), 触发类型, item/axis/none；
=> tooltip_trigger_on(srt,default='mousemove|click'), 触发条件，mousemove/click/none；
=> tooltip_axispointer_type(str,default='line'), 指示器类型，line/shadow/cross；
=> tooltip_formatter(str), 模版变量；
=> tooltip_text_color(str,default='#fff'), 提示框字体颜色；
=> tooltip_font_size(int,default=14), 提示框字体大小；
=> tooltip_background_color(str,default='rgba(50,50,50,0.7)'), 提示框浮层的背景颜色；
=> tooltip_border_color(str,default='#333'), 提示框浮层的边框颜色；
=> tooltip_border_width(int/float,default=0), 提示框浮层的边框宽。
'''
'''
markLine-markPoint, 图形标记组件，用标记线和标记点标记指定的特殊数据Bar、Line、Kline）。
***********************************************************************************************
=> mark_point(list), 标记点，min/max/average，支持自定义点[{"coord":[a1, b1],"name":"first markpoint"}]；
=> mark_point_raw(list), 原生格式的 markPoint 数据；
=> mark_point_symbol(str,default='pin'), 标记点图形，circle/rect/roundRect/triangle/diamond/pin/arrow；
=> mark_point_symbolsize(int,default=50), 标记点图形大小；
=> mark_point_textcolor(str,default='#fff'), 标记点字体颜色；
=> mark_line(list), 标记线，min/max/average；
=> mark_line_raw(list), 原生格式的 markLine 数据；
=> mark_line_symbolsize(int,default=15), 标记线图形大小；
=> mark_line_valuedim(list), 标记线指定在哪个维度上指定最大值最小值；
=> mark_line_coords([[]])，标记线指定起点和终点坐标；
=> mark_point_valuedim(list), 标记线指定在哪个维度上指定最大值和最小值。
'''
'''
toolbox, 右侧实用工具箱。
***********************************************************************************************
=> is_toolbox_show(bool,default=True), 指定是否显示右侧实用工具箱；
=> is_more_utils(bool), 指定是否提供更多实用工具按钮。
'''