'''
from pyecharts import Tree
Scatter
            .add(name,                # 系列名称（str）;
            data,                     # 数据项目（list）;
            tree_layout="orthogonal", # 树图布局（str），正交（orthogonal）/径向（radial）;
            tree_symbol="emptyCircle",# 标记图形（str），in/arrow/none;
            tree_symbol_size=7,       # 标记图形大小（str），[20,10]表示高20宽10;
            tree_orient="LR",         # 树图正交布局的方向，LR/RL/TB/BT;
            tree_top="12%",           # 树图组件距容器顶的距离，12%、20;
            tree_left="12%",
            tree_bottom="12%",
            tree_right="12%",
            tree_collapse_interval=0,  # 折叠节点间隔，当节点过多以解决显示过杂的问题;
            tree_label_position="left",# 标签位置;
            tree_label_vertical_align="middle", # 父节点文字垂直对齐方式;
            tree_label_align="right",  # 父节点文字水平对齐方式;
            tree_label_text_size=12,   # 父节点文字大小;
            tree_label_rotate=0,       # 父节点标签旋转角度;
            tree_leaves_position="right",       # 距离图形元素的距离;
            tree_leaves_vertical_align="middle",# 叶节点文字垂直对齐方式;
            tree_leaves_align="right", # 叶节点文字水平对齐方式;
            tree_leaves_text_size=12,  # 叶节点文字大小;
            tree_leaves_rotate=0,      # 叶节点标签旋转角度;
             **kwargs)


=> data数据结构，树形组织;
=> tree_symbol标记图形，emptyCircle/circle/rect/roundRect/triangle/diamond/pin/arrow/none;
'''


from pyecharts import Tree


''' 假如有树图
     |----B     |----E----|----I----|----Z
     |          |
     |----C-----|----F         |----J
A----|                         |----L----|----S
     |----D-----|----G----|----|----K    |----X
     |----R     |----H----|----|----N----|----Y
     |----W-----|----U         |----M
     |----Q-----|----OI        |----O----|----V                    
                               |----P
                    
'''
data = [
    {
        "name": "A",
        "children": [
            {
                "name": "B",
                "children": []
            },
            {
                "name": "C",
                "children": [
                    {
                        "name": "F",
                        "children": []
                    },
                    {
                        "name": "E",
                        "children": [
                            {
                                "name": "I",
                                "children": [
                                    {
                                        "name": "Z",
                                        "children": []
                                    }
                                ]
                            }
                        ]
                    }
                ]
            },
            {
                "name": "D",
                "children": [
                    {
                        "name": "G",
                        "children": [
                            {
                                "name": "K",
                                "children": []
                            },
                            {
                                "name": "J",
                                "children": []
                            },
                            {
                                "name": "L",
                                "children": [
                                    {
                                        "name": "S",
                                        "children": []
                                    },
                                    {
                                        "name": "X",
                                        "children": []
                                    }
                                ]
                            }
                        ]
                    },
                    {
                        "name": "H",
                        "children": [
                            {
                                "name": "N",
                                "children": [
                                    {
                                        "name": "Y",
                                        "children": []
                                    }
                                ]
                            },
                            {
                                "name": "M",
                                "children": []
                            },
                            {
                                "name": "O",
                                "children": [
                                    {
                                        "name": "V",
                                        "children": []
                                    }
                                ]
                            },
                            {
                                "name": "P",
                                "children": []
                            }
                        ]
                    }
                ]
            },
            {
                "name": "R",
                "children": []
            },
            {
                "name": "W",
                "children": [
                    {
                        "name": "U",
                        "children": []
                    }
                ]
            },
            {
                "name": "Q",
                "children": [
                    {
                        "name": "OI",
                        "children": []
                    }
                ]
            }
        ]
    }
]



'''sample 1 —— draw tree chart.(绘制树图)'''
# tree = Tree("树图示例")
# tree.add("", data)
# tree.render()


'''sample 2 —— draw tree chart with tree_collapse_interval.(调节节点间隔绘制树图)'''
tree = Tree("treechart")
tree.add("", data, tree_collapse_interval=2, tree_orient="TB", tree_layout="radial")
tree.render()
