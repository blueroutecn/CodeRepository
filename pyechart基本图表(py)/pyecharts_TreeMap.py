'''
from pyecharts import TreeMap
Scatter
            .add(name, data, treemap_leaft_depth, treemap_drilldown_icon, treemap_visible_min,)
=> name(str), 图例名称；
=> data(list),矩形树图的数据项是 一棵树，每个节点包括value, name（可选）, children（也是树，可选）；
=> treemap_leaft_depth(int), leafDepth 表示『展示几层』，层次更深的节点则被隐藏起来；
=> treemap_drilldown_icon(str), 当节点可以下钻时的提示符。只能是字符。默认为 '▶'；
=> treemap_visible_min(int), 如果某个节点的矩形的面积，小于这个数值，这个节点就不显示；

'''


from pyecharts import TreeMap

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
        "value": 180,
        "children": [
            {
                "name": "B",
                "value": 5,
            },
            {
                "name": "C",
                "value": 25,
                "children": [
                    {
                        "name": "F",
                        "value": 20,
                    },
                    {
                        "name": "E",
                        "value": 12,
                        "children": [
                            {
                                "name": "I",
                                "value": 8,
                                "children": [
                                    {
                                        "name": "Z",
                                        "value": 5,
                                    }
                                ]
                            }
                        ]
                    }
                ]
            },
            {
                "name": "D",
                "value": 45,
                "children": [
                    {
                        "name": "G",
                        "value": 35,
                        "children": [
                            {
                                "name": "K",
                                "value": 10,
                            },
                            {
                                "name": "J",
                                "value": 20
                            },
                            {
                                "name": "L",
                                "value": 12,
                                "children": [
                                    {
                                        "name": "S",
                                        "value": 6,
                                    },
                                    {
                                        "name": "X",
                                        "value": 6,
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
                "value": 5,
            },
            {
                "name": "W",
                "value": 10,
                "children": [
                    {
                        "name": "U",
                        "value": 6,
                    }
                ]
            },
            {
                "name": "Q",
                "value": 10,
                "children": [
                    {
                        "name": "OI",
                        "value": 4,
                    }
                ]
            }
        ]
    }
]



'''sample 1 —— draw treemap chart.(绘制矩形树图)'''
# treemap = TreeMap("矩形树图", width=1200, height=600)
# treemap.add("演示数据", data, is_label_show=True, label_pos="inside")
# treemap.render()




