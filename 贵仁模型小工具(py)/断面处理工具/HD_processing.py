import os
import logging
import time


''' 贵仁水力模型(1.2.6), 前处理工具 '''


# 调整水力模型河道断面。
class pre_Processing():
    print(time.strftime("%Y-%m-%d %H:%M:%S"), "  贵仁水力模型小工具用起来👆\n")  # 类代码，第一次触发执行。
    logging_file = "grms_log.txt"  # 类字段，任何类实例访问的都是同样的字段，但修改后的字段将绑定在实例。

    @classmethod  # 类方法，不能访问类的self成员；通过cls，可以访问类成员(用于封装接口)；通过类名调用。
    def method_for_learn(cls):
        print("classmethod，类方法。")
        print("classmethod, cls访问类成员：%s。", cls.logging_file)

    @staticmethod  # 类静态方法，不能访问类的self成员，形同类外的普通函数。
    def method_for_learn_2():
        print("staticmethod，类的静态方法。")

    @property  # 类的成员属性（读），可在返回结果前进行逻辑控制并屏蔽内部字段命名。
    def property_for_learn(self):
        print("property, 类的公共成员属性(getter)。")
        print("property，调用方式 pre_Processing.property_for_learn。")
        return self.logging_file

    @property_for_learn.setter  # 类的成员属性（写），对传入的数据进行逻辑处理再赋值类成员。
    def property_for_learn(self, value):
        print("property, 类的公共成员属性（setter，与getter匹配）。")
        print("property, 调用方式 pre_Processing.property_for_learn =‘logger.txt’。")
        self.logging_file = value.lower()

    def __init__(self, in1d_folder, in2d_folder, result_folder):
        self.__IN1D = in1d_folder
        self.__IN2D = in2d_folder
        self.__outFolder = result_folder
        self.__log = self.__init_log()

    def __init_log(self):
        '''
        初始化一个日志器，保存处理信息。
        :return: None
        '''
        log = logging.getLogger()  # 注意：若未设置日志名称，则所有getLogger()返回的都是同一个日志器。
        log.setLevel(logging.INFO)  # 设置日志等级，后面低于该等级的日志信息将不会被输出。

        handler_file = logging.FileHandler(self.logging_file, "a+", encoding='utf-8')
        handler_file.setLevel(logging.WARNING)

        handler_stream = logging.StreamHandler()
        handler_stream.setLevel(logging.ERROR)

        _format = logging.Formatter(fmt="%(asctime)s, %(funcName)s, %(lineno)d, %(name)s: %(message)s")
        handler_file.setFormatter(_format)
        handler_stream.setFormatter(_format)

        log.addHandler(handler_file)
        log.addHandler(handler_stream)
        return log

    @staticmethod
    def get_scatter_ele(ele_file):
        '''
        提取三维散点的高程，并返回ID列表和高程列表。
        （场景是，采用dem散点数据和CrossSectionLocation数据，插值河道中点高程，之后再整体调整河道高程）
        :param ele_file: 三维散点文件
        :return: ID(河道断面id)，ele(散点（断面中点）高程)
        '''
        ID, ele = [], []
        with open(ele_file, "r") as f:
            for line in f.readlines():
                data = line.strip().split()
                ID.append(data[0])
                ele.append(float(data[3]))
        return ID, ele

    def get_HD_data(self):
        '''
        获取HD数据,并返回ID列表（带河段分割标志）和断面列表。
        :return: ID(河道id), HD(河道断面形状)
        '''
        ID, HD = [], []
        HD_file = self.__IN1D + "\\Topo\\HD.inp"
        f = open(HD_file, 'r')
        data = f.readlines()
        f.close()

        index, count = 1, len(data)
        while index < count:
            line = data[index].strip().split()
            if len(line) == 0:  # 剔除空行。
                index += 1
                continue
            elif line[0][0] == '[':  # 剔除河段首两行。
                index += 2
                ID.append("----")    # 作为不同河段的标志
                HD.append([])        # 与ID保持一致位置索引
                continue

            ID.append(line[1])  # 当前断面ID。
            station_index, station_pair_count, stations = 1, int(line[0]), []
            while station_index <= station_pair_count:  # 从当前断面的站点开始。
                station_pair = data[index+station_index].strip().split()
                stations.append([float(station_pair[0]), float(station_pair[1])])
                station_index += 1
            HD.append(stations)  # 当前断面的断面数据（不包括标题）。
            index += (station_pair_count + 1)

        self.__log.warning("HD read is done (for learn).\n")
        return ID, HD

    def modify_HD_with_ele(self, ele_file):
        '''
        通过断面中点高程数据调整河道模型的断面高程，并返回完整格式的HD数据。
        :param ele_file: 断面中点高程文件（由get_scatter_ele(ele_file)获得）。
        :return: ID_hd(河道断面id)，HD(完整的HD.inp数据)。
        '''
        ID_ele, ele = pre_Processing.get_scatter_ele(ele_file)
        ID_hd, HD = self.get_HD_data()

        for i, e in zip(ID_ele, ele):
            try:
                index = ID_hd.index(i)  # 断面的ID应是唯一。
            except ValueError as ve:
                self.__log.error("ValueError")
                print(ve)
                continue
            hd, hd_count = HD[index], len(HD[index])
            mid_index = int(hd_count/2)
            mid_ele = hd[mid_index][1]

            ele_delta = e - mid_ele
            for pair in hd:
                pair[1] += ele_delta
            HD[index] = hd
        return ID_hd, HD

    def average_cross_bank_ele(self, HD):
        '''
        平均同一断面上的左右河岸高程。
        :param HD: 河道断面形状数据。
        :return: None。
        '''
        index, count = 0, len(HD)
        while index < count:
            if HD[index][0] == '-':
                index += 1
                continue
            cross = HD[index]
            left_bank_ele, right_bank_ele = cross[0][1], cross[-1][1]
            average_ele = 0.5*(left_bank_ele+right_bank_ele)
            cross[0][1], cross[-1][1] = average_ele, average_ele
            HD[index] = cross
            index += 1

    def average_adjacentCross_bank_ele(self, ID, HD):
        '''
        平均各河段内相邻断面的河岸高程（使河岸平滑，不跨汊点平均）。
        :param ID: 河道断面id。
        :param HD: 河道断面形状。
        :return: None.
        '''
        reaches = self.__get_reach(ID)
        for reach in reaches:
            count, index = reach[0], reach[1]
            while index < reach[1] + count - 2:  # 当前河段的倒数第二个断面。
                cross1, cross2 = HD[index], HD[index+1]
                av_cross1_bank = 0.5*(cross1[0][1] + cross1[-1][1])
                av_cross2_bank = 0.5*(cross2[0][1] + cross2[-1][1])
                av_bank = 0.5*(av_cross1_bank + av_cross2_bank)
                cross1[0][1], cross1[-1][1] = av_bank, av_bank
                cross2[0][1], cross2[-1][1] = av_bank, av_bank
                HD[index], HD[index + 1] = cross1, cross2
                index += 1

    def average_adjacentCross_bottom_ele(self, ID, HD):
        '''
        平均各河段内相邻断面的河底高程（使底坡平滑，不跨汊点平均）。
        :param ID: 河道断面id。
        :param HD: 河道断面形状。
        :return: None。
        '''
        reaches = self.__get_reach(ID)
        for reach in reaches:
            count, index = reach[0], reach[1]
            while index < reach[1] + count - 2:  # 当前河段的倒数第二个断面。
                cross1, cross2 = HD[index], HD[index+1]
                cross1_ele = [pair[1] for pair in cross1]
                cross2_ele = [pair[1] for pair in cross2]
                bottom1, bottom2 = min(cross1_ele), min(cross2_ele)
                av_bottom = 0.5*(bottom1+bottom2)
                bottom_index1, bottom_index2 = cross1_ele.index(bottom1), cross2_ele.index(bottom2)
                HD[index][bottom_index1][1], HD[index+1][bottom_index2][1] = av_bottom, av_bottom
                index += 1

    def __get_reach(self, ID):
        '''
         提取各段断面数量及各河段标志的索引下标，并返回列表。
         （场景是，当要从HD数据中按河段提取断面数据，ID与HD结构相同。）
        :param ID: 河道断面id。
        :return: reach(河段断面个数，在HD.inp文件中的索引)。
        '''
        reach, index, index_last = [], 0, 0
        reach_index = [num for num, data in enumerate(ID) if data[0] == '-']
        reach.append(reach_index[1]-reach_index[0]-1)
        index = 1
        while index < len(reach_index)-1:
            reach.append(reach_index[index+1] - reach_index[index]-1)
            index += 1
        reach.append(len(ID)-reach_index[-1]-1)
        return reach

    def out_HD_file(self, ID, HD):
        '''
        输出河道数据到文件中.
        :param ID: 河道断面id。
        :param HD: 河道断面形状。
        :return: None.
        '''
        if len(ID) != len(HD):
            raise ValueError("len(HD) != len(ID)")

        reach = self.__get_reach(ID)
        cross_index, cross_count = 0, len(ID) - len(reach)

        HD_file = self.__outFolder + "\\HD.inp"
        f = open(HD_file, 'w')
        f.write(str(cross_count) + "\t" + str(len(reach)) + "\n")

        reach_iter = iter(reach)  # 获取一个迭代器。
        while cross_index < len(ID):
            if ID[cross_index][0] == "-":
                try:
                    reach_count = next(reach_iter)  # next()，返回当前数据后，reach_iter指向下一个数据。
                except StopIteration as si:
                    print(si)
                    break
                else:
                    f.write("\n\n[reach]	*******************	\n")
                    f.write(str(reach_count) + "\n")
                    cross_index += 1
                    continue

            stations = HD[cross_index]
            f.write(str(len(stations)) + "\t\t" + ID[cross_index] + "\n")
            for s in stations:
                f.write(str(s[0]) + "\t" + str(s[1]) + "\n")
            cross_index += 1
        f.close()

    def get_cross_min_ele(self, ID, HD):
        '''
        获取HD中各断面最小高程，并返回列表。
        （场景是，绘制河道河底的纵剖线，分析底坡变化。）
        :param ID: 河道断面id。
        :param HD: 河道断面形状。
        :return: id_min_ele(河道断面id和断面最小高程)。
        '''
        id_min_ele = []
        for i, h in zip(ID, HD):
            if i[0] == "-": continue
            h_ele = [pair[1] for pair in h]
            id_min_ele.append([i, min(h_ele)])
        return id_min_ele

    def raise_reach_ele(self, parts_id, parts_dh, ID, HD):
        '''
        分段整体抬升河道断面高程（每一段应该在一个河段内）。
        （场景是，校正水位时发现河道高程明显偏低，需要均匀抬升相关河道。）
        :param parts_id:  要提升的各段河道的首末断面id。
        :param parts_dh:  要提升的各段河道的首末断面上移量。
        :param ID: 河道断面id。
        :param HD: 河道形状数据。
        :return: None。
        '''
        for id_pair, h_pair in zip(parts_id, parts_dh):
            start_id, stop_id = str(id_pair[0]), str(id_pair[1])
            start_h, stop_h = h_pair[0], h_pair[1]
            start_index, stop_index = ID.index(start_id), ID.index(stop_id)
            ratio = (stop_h - start_h) / (stop_index - start_index)

            index = start_index
            while index <= stop_index:
                delta_h = start_h + ratio * (index - start_index)
                n, cross = 0, HD[index]
                while n < len(cross):
                    cross[n][1] += delta_h
                    n += 1
                HD[index] = cross
                index += 1

    def raise_bank_ele(self, parts_id, parts_dh, ID, HD):
        '''
        分段抬升河道的岸点高程。
        :param parts_id:  要提升的各段河道的首末断面id。
        :param parts_dh: 要提升的各段河道的首末断面上移量。
        :param ID: 河道断面id。
        :param HD: 河道形状数据。
        :return: None。
        '''
        for id_pair, h_pair in zip(parts_id, parts_dh):
            start_id, stop_id = str(id_pair[0]), str(id_pair[1])
            start_h, stop_h = h_pair[0], h_pair[1]
            start_index, stop_index = ID.index(start_id), ID.index(stop_id)
            ratio = (stop_h - start_h) / (stop_index - start_index)

            index = start_index
            while index <= stop_index:
                delta_h = start_h + ratio * (index - start_index)
                HD[index][0][1] += delta_h
                HD[index][-1][1] += delta_h
                index += 1

    def raise_cross_ele_without_bank(self,parts_id, parts_dh, ID, HD):
        '''
        整体抬升河道断面，但不包括河岸（当河岸比中间断面点低时，取中间点高程）。
        '''
        for id_pair, h_pair in zip(parts_id, parts_dh):
            start_id, stop_id = str(id_pair[0]), str(id_pair[1])
            start_h, stop_h = h_pair[0], h_pair[1]
            start_index, stop_index = ID.index(start_id), ID.index(stop_id)
            ratio = (stop_h - start_h) / (stop_index - start_index)

            index = start_index
            while index <= stop_index:
                delta_h = start_h + ratio * (index - start_index)
                n, cross = 1, HD[index]
                while n < len(cross)-1:
                    cross[n][1] += delta_h
                    n += 1
                cross_ele = [cross_station[1] for cross_station in cross]
                max_cross_ele = max(cross_ele)
                cross[0][1] = max_cross_ele if cross[0][1] < max_cross_ele else cross[0][1]
                cross[-1][1] = max_cross_ele if cross[-1][1] < max_cross_ele else cross[-1][1]
                HD[index] = cross
                index += 1


    # 输出IN2D的bcTimeSeries文件。
    #def write_in2d_bcTimeSeries(self):
    #    pass
    #
    # 把断面转成带有高程值的三维散点，并导出到文件。
    # （场景是，将散点导入sms调整断面形状；采用默认路径及命名）
    #def turn_HD_to_scatters(self):
    #    pass
    #
    # 把处理后的断面三维散点转为断面，并导出到文件（不覆盖原始HD.inp）。
    #def turn_scatters_to_HD(self, scatters_file=""):
    #    pass
    #
    # 获取河道岸点三维坐标。
    # （场景是，在一二维侧向耦合模型中获取贴岸坐标点以生成相关耦合文件。）
    #def get_bank_scatter(self):
    #    pass


''' 贵仁水力模型(1.2.6), 后处理工具 '''


# 提取二维模型输出模型结果文件，绘制二维淹没面积和淹没水深变化曲线。
class ExtractFloodAreaProcess():

    def __init__(self, folder):  # folder是二维模型的MESH文件夹。
        self.folder = folder
        self.__node = []  # 存储网格节点的三维坐标。
        self.__ele = []   # 存储网格单元的面积。
        self.__area = []  # 存储时刻下淹没面积和最大淹没水深。

    # 读取MESH的node、bed文件。
    def read_node_bed(self):
        fl = open(self.folder+"\\node", "r")
        xy_data = fl.readlines()[1:-1]  # 网格节点的xy坐标。
        fl.close()

        fl = open(self.folder+"\\bed", "r")
        z_data = fl.readlines()  # 网格节点的z坐标。
        fl.close()

        for xy_line, z_line in zip(xy_data, z_data):
            xy_arr = xy_line.strip().split()
            x, y = float(xy_arr[1]), float(xy_arr[2])
            z_arr = z_line.strip().split()
            z = float(z_arr[0])
            self.__node.append([x, y, z])   # 默认节点ID是从1开始连续的。

    # 读取MESH的ele文件。
    def read_ele(self):
        fl = open(self.folder+"\\ele", "r")
        data = fl.readlines()[1:-1]  # 单元ID及对应顶点的ID。
        fl.close()

        for line in data:
            arr = line.strip().split()
            node1, node2, node3 = int(arr[1]), int(arr[2]), int(arr[3])
            self.__ele.append([node1, node2, node3])   # 默认单元ID是从1开始连续的。
        self.__compute_ele_area()  # 调用类内函数，计算单元面积。

    # 根据单元的顶点，计算单元的面积。
    def __compute_ele_area(self):
        index, count = 0, len(self.__ele)
        while index < count:
            # 单元的ID是从1开始的，但是存储是从0开始的， ID = index + 1
            node1, node2, node3 = self.__ele[index]
            # node是点的ID(从1开始), 存储是从0开始的， ID = node -1
            n1, n2, n3 = self.__node[node1-1], self.__node[node2-1], self.__node[node3-1]
            x3x1 = n3[0] - n1[0]  # x3-x1, 下同。
            y3y1 = n3[1] - n1[1]
            x2x1 = n2[0] - n1[0]
            y2y1 = n2[1] - n1[1]
            edge1 = pow(pow(x2x1, 2) + pow(y2y1, 2), 0.5)
            edge2 = pow(pow(x3x1, 2) + pow(y3y1, 2), 0.5)
            temp = abs(x3x1*x2x1 + y3y1*y2y1)
            cos_angle = temp/(edge1*edge2)
            sin_angle = pow(1-pow(cos_angle, 2), 0.5)
            area = 0.5*edge1*edge2*sin_angle  # 计算单元面积。
            # av_z = (n1[2] + n2[2] + n3[2])/3.0   # 单元平均高度，作为判断水深依据
            self.__ele[index] = area
            index += 1

    # 计算各时刻洪水淹没面积以及最大淹没水深，默认单位m（unit参数可选m/km）。
    # result_folder为二维结果文件夹，h_level是最小淹没水深（m，小于改值不计入淹没面积）。
    def compute_flood_process(self, result_folder, h_level, unit="m"):
        for pathes, folders, files in os.walk(result_folder):
            for file in files:
                fname, ftype = os.path.splitext(file)
                if ftype != ".csv": continue  # 二维输出结果的文件类型。
                elif fname[0] == "N": continue  # N开头的文件是节点数据文件，这里统计的是单元数据文件（C开头）。

                fl = open(result_folder + "\\" + file, "r")
                data = fl.readlines()
                fl.close()

                time_line_arr = data[1].strip().split()
                time = float(time_line_arr[1])  # 当前时刻。
                area, maxH = 0, 0  # 淹没面积和最大淹没水深。
                for line in data[2:-1]:
                    arr = line.strip().split()
                    ele_ID = int(arr[0].split(',')[0])
                    ele_h = float(arr[6].split(',')[0])
                    if ele_h > h_level:
                        maxH = max(maxH, ele_h)
                        area += self.__ele[ele_ID-1]
                if unit in ["KM", "km", "Km"]:  area /= 1.0e6  # 判断面积单位。
                self.__area.append([time, area, maxH])
        self.out_flood_process(result_folder)  # 输出计算结果到OUT2D文件夹中。

    # 输出计算结果。
    def out_flood_process(self, outfolder):
        fl = open(outfolder + "\\floodprocess.txt", "w")
        for data in self.__area:
            fl.write(str(data[0]) + "\t" + str(data[1]) + "\t" + str(data[2]) + "\n")


# folder = "C:\\Users\gr\Desktop\MESH"
# result_folder = "F:\抚州项目(验收)\抚河模型\model_v1.2.6\AllProject\OUT2D\Res0000"
# h_level = 1.0e-3
# unit = "km"
# test = ExtractFloodAreaProcess(folder)
# test.read_node_bed()
# test.read_ele()
# test.compute_flood_process(result_folder, h_level)




























