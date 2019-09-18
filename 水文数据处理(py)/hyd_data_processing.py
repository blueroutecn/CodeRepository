import xlwings as xlw


''' 处理水文基本站的原始降雨数据（格式1）'''
class Process_Basic_Station():
    sheet_name = "2006-2014降水量摘录表"
    data_cols = {"station": "A", "year": "B", "m/d": "C", "start_hour": "D", "end_hour": "E", "rain": "F"}

    def __init__(self, data_file, out_folder):  # data_file是水文数据表格，out_folder是输出结果文件的文件夹。
        self.data_file = data_file
        self.out_folder = out_folder
        self.app = xlw.App(visible=False, add_book=False)
        self.excel = self.app.books.open(self.data_file)
        self.sheet = self.excel.sheets[self.sheet_name]
        self.nrows = self.sheet.used_range.last_cell.row       # 获取数据列长度
        self.ncols = self.sheet.used_range.last_cell.column    # 获取数据行宽度

    # 获取各个站点的数据长度并返回。
    def get_station_Len(self):
        col = self.data_cols["station"]
        data = self.sheet.range(col+"2:"+col+str(self.nrows)).value
        station_length = []

        index, index_start, id1, id2 = 0, 0, data[0], data[0]
        while index < len(data)-1:    # len(data)是长度，len(data)-1是最后一个数据
            index += 1                # index最大只能到倒数第二个数据
            id2 = data[index]
            if id2 is None:  break    # 默认空格表示该列数据结束，否则index始终在增加，导致最后长度错误
            if id1 != id2:
                station_length.append((str(id1), index-index_start))
                index_start = index
            id1 = id2
        station_length.append((str(id1), index-index_start+1))
        return station_length

    # 获取各个站点的每一年、每一月、每一天的数据长度并返回。
    def get_year_month_day_Len(self):
        station_len = self.get_station_Len()  # 调用类内函数，获取每个站点的数据长度。

        year_col = self.data_cols["year"]
        year_data = self.sheet.range(year_col+"2:"+year_col+str(self.nrows)).value
        md_col = self.data_cols["m/d"]
        md_data = self.sheet.range(md_col+"2:"+md_col+str(self.nrows)).value

        year_len, month_len, day_len = [], [], []
        index = 0
        for station in station_len:
            s_id, s_len = station[0], station[1]
            year1, year2 = year_data[index], year_data[index]
            md1, md2 = md_data[index], md_data[index]
            year_start, month_start, day_start = index, index, index
            data_range = index + s_len

            while index < data_range - 1:
                year1 = year2
                md1 = md2

                index += 1
                year2 = year_data[index]
                md2 = md_data[index]
                month1, day1 = str(md1)[:-4], str(md1)[-4:-2]
                month2, day2 = str(md2)[:-4], str(md2)[-4:-2]

                if year2 != year1:
                    year_len.append((int(year1), index - year_start))
                    year_start = index
                    month_len.append((int(month1), index - month_start))
                    month_start = index
                    day_len.append((int(day1), index - day_start))
                    day_start = index
                    continue

                if month2 != month1:
                    month_len.append((int(month1), index - month_start))
                    month_start = index
                    day_len.append((int(day1), index - day_start))
                    day_start = index
                    continue

                if day2 != day1:
                    day_len.append((int(day1), index - day_start))
                    day_start = index
                    continue

            year_len.append((int(year2), index-year_start+1))
            month_len.append((int(str(md2)[:-4]), index-month_start+1))
            day_len.append((int(str(md2)[-4:-2]), index-day_start+1))
            index += 1
        return (year_len, month_len, day_len)

    # 获取所有的起始时间和终止时间数据并返回（不区分站点）。
    def get_start_end_time(self):
        start_col = self.data_cols["start_hour"]
        start_data = self.sheet.range(start_col+"2:"+start_col+str(self.nrows)).value
        end_col = self.data_cols["end_hour"]
        end_data = self.sheet.range(end_col+"2:"+end_col+str(self.nrows)).value
        start_time, end_time = [], []

        for start, end in zip(start_data, end_data):
            start_time.append(int(start/100))       # 不区分半小时，20:00, 20:30, 20:59都是一样的
            end_time.append(int(end/100))
            # s_hour, e_hour = start/100.0, end/100.0
            # start_hour = int(s_hour)+1 if s_hour-int(s_hour) > 0.3 else int(s_hour)
            # end_hour = int(e_hour)+1 if e_hour-int(e_hour) > 0.3 else int(e_hour)
            # start_time.append(start_hour)
            # end_time.append(end_hour)
        return (start_time, end_time)

    # 获取所有的降雨数据并返回（不区分站点）。
    def get_rain(self):
        col = self.data_cols["rain"]
        rain = self.sheet.range(col+"2:"+col+str(self.nrows)).value
        return rain

    # 重整并填充一天的数据并返回（1h间隔，一天0-23h，不区分站点）。
    def __reset_day_timeseries(self, start_time, end_time, rainfall):
        timeseries = [0.0 for y in range(24)]
        for start, end, value in zip(start_time, end_time, rainfall):
            time_len = end - start
            if end > 23: end = 23                    # 注意
            if time_len == 1 or time_len == 0:       # 有可能出现8：00~8：00
                timeseries[end] = value
                continue
            if time_len < 0:
                time_len = end + 24 - start          # 但是下一天开始的几个小时的降雨量无法获取

            average_rain = round(value/time_len, 3)  # 运算结果保留3位小数
            count = 1
            while count <= time_len and (start+count) < 24:  # 这里注意
                timeseries[start + count] = average_rain
                count += 1
        return timeseries

    # 封装水文数据处理流程，按站点分别生成文件输出。
    def write_pcp_file(self):
        station_len = self.get_station_Len()
        year_len, month_len, day_len = self.get_year_month_day_Len()
        start_time, end_time = self.get_start_end_time()
        rainfall = self.get_rain()

        index = 0
        year_index, month_index, day_index = 0, 0, 0
        for station in station_len:
            s_id, s_len = station[0], station[1]
            if len(s_id) > 8:
                s_file = self.out_folder + "\\pcp_" + s_id[:-2] + ".txt"
            else:
                s_file = self.out_folder + "\\pcp_" + s_id + ".txt"
            fl = open(s_file, "w")            # 每一个站点一个文件

            year_range = 0
            for year in year_len[year_index:]:
                year_index += 1               # 年份的索引已经指向下一年
                y_id, y_len = year[0], year[1]
                year_range += y_len
                if year_range > s_len:        # 说明当前已经到了下一个站点的第一个年份了
                    year_index -= 1           # year_index已经指向下一站点第二年了，修正
                    break

                month_range = 0
                for month in month_len[month_index:]:
                    month_index += 1
                    m_id, m_len = month[0], month[1]
                    month_range += m_len
                    if month_range > y_len:
                        month_index -= 1
                        break

                    day_range, day_before = 0, day_len[day_index][0]

                    for day in day_len[day_index:]:
                        day_index += 1
                        d_id, d_len = day[0], day[1]
                        day_range += d_len
                        if day_range > m_len:
                            day_index -= 1
                            break

                        d_dis = d_id - day_before
                        if d_dis > 1:
                            n = 1
                            while n < d_dis:         # 只处理了两个日期之间的日子
                                date = str(y_id)+"/"+str(m_id)+"/"+str(day_before+n)
                                for hour in range(24):
                                    fl.write("\t"+date+"\t"+str(hour)+":00:00\t0.0\n")
                                n += 1
                        day_before = d_id

                        day_timeseries = \
                            self.__reset_day_timeseries(start_time[index: index+d_len],
                                                        end_time[index:index+d_len],
                                                        rainfall[index:index+d_len])

                        date = str(y_id)+"/"+str(m_id)+"/"+str(d_id)
                        for hour, value in zip(range(24), day_timeseries):
                            fl.write("\t"+date+"\t"+str(hour)+":00:00\t"+str(value)+"\n")
                        index += d_len
            fl.close()


# path = r"F:\抚州项目\项目建模数据\2 降雨流量数据处理\0、原始数据\1降雨洪水原始数据04-16.xlsx"  # r"xxx",r强制转为字符串。
# result = r"C:\Users\gr\Desktop\fuhe_hydro"
# basic = Process_Basic_Station(path, result)
# basic.get_year_month_day_Len()
# basic.write_pcp_file()


''' 处理水文非基本站的原始降雨数据（格式2）'''
class Process_nonBasic_Station():
    sheet_name = "2010-2016非基本库降水量摘录表"
    data_cols = {"station": "A", "y/m/d_start": "B", "y/m/d_end": "C", "rainfall": "D"}
    __day_list, __month_list, __year_list, __station_list = [], [], [], []

    def __init__(self, data_file, out_folder):  # data_file是水文数据表格，out_folder是输出结果文件的文件夹。
        self.data_file = data_file
        self.out_folder = out_folder
        self.app = xlw.App(visible=False, add_book=False)
        self.excel = self.app.books.open(self.data_file)
        self.sheet = self.excel.sheets[self.sheet_name]
        self.nrows = self.sheet.used_range.last_cell.row       # 获取数据列长度
        self.ncols = self.sheet.used_range.last_cell.column    # 获取数据行宽度

    def __del__(self):  # 类析构函数。
        self.excel.close()
        self.app.quit()

    # 获取各个站点的数据长度并返回。
    def get_station_Len_List(self):
        col = self.data_cols["station"]
        data = self.sheet.range(col+"2:"+col+str(self.nrows)).value
        station_length, station_list = [], []
        index, index_start, id1, id2 = 0, 0, data[0], data[0]
        while index < len(data)-1:    # len(data)是长度，len(data)-1是最后一个数据
            index += 1                # index最大只能到倒数第二个数据
            id2 = data[index]
            if id2 is None:  break    # 默认空格表示该列数据结束，否则index始终在增加，导致最后长度错误
            if id1 != id2:
                station_length.append((str(id1), index-index_start))
                station_list.append(str(id1))
                index_start = index
            id1 = id2
        station_length.append((str(id1), index-index_start+1))
        station_list.append(id1)
        return (station_length, station_list)

    # 获取各个站点的每一年、每一月、每一天的数据长度并返回。
    def get_day_month_year_Len_List(self):
        station_len, station_list = self.get_station_Len_List()

        col = self.data_cols["y/m/d_start"]
        data = self.sheet.range(col+"2:"+col+str(self.nrows)).value
        year_len, month_len, day_len = [], [], []
        year_list, month_list, day_list = [], [], []

        index = 0
        for station in station_len:
            s_id, s_len = station[0], station[1]

            data1, data2 = data[index], data[index]
            day_index_start, month_index_start, year_index_start = index, index, index
            data_range = index + s_len

            while index < data_range-1:
                index += 1
                data2 = data[index]
                data1_arr, data2_arr = str(data1).split(), str(data2).split()
                data1_date, data2_date = data1_arr[0].split("-"), data2_arr[0].split("-")
                if data1_date[0] != data2_date[0]:       # 年变了都变了，2015/6/5->2016/6/5, data1_arr[0][:-4]
                    year_len.append((int(data1_date[0]), index-year_index_start))
                    year_list.append(int(data1_date[0]))
                    year_index_start = index
                    month_len.append((int(data1_date[1]), index-month_index_start))
                    month_list.append(int(data1_date[1]))
                    month_index_start = index
                    day_len.append((int(data1_date[2]), index-day_index_start))
                    day_list.append(int(data1_date[2]))
                    day_index_start = index
                    data1 = data2
                    continue

                if data1_date[1] != data2_date[1]:       # 月变了日变了，6/5->1/5, data1_arr[0][:-2]
                    month_len.append((int(data1_date[1]), index-month_index_start))
                    month_list.append(int(data1_date[1]))
                    month_index_start = index
                    day_len.append((int(data1_date[2]), index-day_index_start))
                    day_list.append(int(data1_date[2]))
                    day_index_start = index
                    data1 = data2
                    continue

                if data1_date[2] != data2_date[2]:       # 日变了, data1_arr[0] != data2_arr[0]
                    day_len.append((int(data1_date[2]), index-day_index_start))
                    day_list.append(int(data1_date[2]))
                    day_index_start = index
                    data1 = data2
                    continue

            data1_arr = str(data1).split()
            data1_date = data1_arr[0].split("-")

            year_len.append((int(data1_date[0]), index-year_index_start+1))
            month_len.append((int(data1_date[1]), index-month_index_start+1))
            day_len.append((int(data1_date[2]), index-day_index_start+1))

            year_list.append(int(data1_date[0]))
            month_list.append(int(data1_date[1]))
            day_list.append(int(data1_date[2]))
            index += 1            # 这里加1进入下一段
        return (day_len, month_len, year_len, day_list, month_list, year_list)

    # 获取所有的起始时间数据并返回（不区分站点）。
    def get_start_time(self):
        col = self.data_cols["y/m/d_start"]
        data = self.sheet.range(col+"2:"+col+str(self.nrows)).value
        start_time = []
        for it in data:
            if it is None:  break      # 默认空格代表该列数据结束
            data_arr = str(it).split()
            data_time_arr = data_arr[1].split(":")
            start_time.append(int(data_time_arr[0]))
        return start_time

    # 获取所有的终止时间数据并返回（不区分站点）。
    def get_end_time(self):
        col = self.data_cols["y/m/d_end"]
        data = self.sheet.range(col+"2:"+col+str(self.nrows)).value
        end_time = []
        for it in data:
            if it is None:  break      # 默认空格代表该列数据结束
            data_arr = str(it).split()
            data_time_arr = data_arr[1].split(":")
            end_time.append(int(data_time_arr[0]))
        return end_time

    # 获取所有的降雨数据并返回（不区分站点）。
    def get_rainfall(self):
        col = self.data_cols["rainfall"]
        rainfall = self.sheet.range(col+"2:"+col+str(self.nrows)).value
        return rainfall

    # 重整并填充一天的数据并返回（1h间隔，一天0-23h，不区分站点）。
    def __reset_day_timeseries(self, start_time, end_time, rainfall):
        timeseries = [0.0 for y in range(24)]
        for start, end, value in zip(start_time, end_time, rainfall):
            time_len = end - start
            if time_len == 1 or time_len == 0:       # 有可能出现8：00~8：00
                timeseries[end] = value
                continue
            if time_len < 0:
                time_len = end + 24 - start          # 但是下一天开始的几个小时的降雨量无法获取

            average_rain = round(value/time_len, 3)  # 运算结果保留3位小数
            count = 1
            while count <= time_len and (start+count) < 24:  # 这里注意
                timeseries[start + count] = average_rain
                count += 1
        return timeseries

    # 封装水文数据处理流程，按站点分别生成文件输出。
    def write_pcp_file(self):
        station_len, station_list = self.get_station_Len_List()
        day_len, month_len, year_len, self.__day_list, self.__month_list, self.__year_list =\
            self.get_day_month_year_Len_List()
        start_time = self.get_start_time()
        end_time = self.get_end_time()
        rainfall = self.get_rainfall()

        index = 0
        year_index, month_index, day_index = 0, 0, 0
        for station in station_len:
            s_id, s_len = station[0], station[1]
            if len(s_id) > 8:
                s_file = self.out_folder + "\\pcp_" + s_id[:-2] + ".txt"
            else:
                s_file = self.out_folder + "\\pcp_" + s_id + ".txt"
            fl = open(s_file, "w")

            year_range = 0
            for year in year_len[year_index:]:
                year_index += 1               # 年份的索引已经指向下一年
                y_id, y_len = year[0], year[1]
                year_range += y_len
                if year_range > s_len:        # 说明当前已经到了下一个站点的第一个年份了
                    year_index -= 1           # year_index已经指向下一站点第二年了，修正
                    break

                month_range = 0
                for month in month_len[month_index:]:
                    month_index += 1
                    m_id, m_len = month[0], month[1]
                    month_range += m_len
                    if month_range > y_len:
                        month_index -= 1
                        break

                    day_range, day_before = 0, day_len[day_index][0]

                    for day in day_len[day_index:]:
                        day_index += 1
                        d_id, d_len = day[0], day[1]
                        day_range += d_len
                        if day_range > m_len:
                            day_index -= 1
                            break

                        d_dis = d_id - day_before
                        if d_dis > 1:
                            n = 1
                            while n < d_dis:         # 只处理了两个日期之间的日子
                                date = str(y_id)+"/"+str(m_id)+"/"+str(day_before+n)
                                for hour in range(24):
                                    fl.write("\t"+date+"\t"+str(hour)+":00:00\t0.0\n")
                                n += 1
                        day_before = d_id

                        day_timeseries = \
                            self.__reset_day_timeseries(start_time[index: index+d_len],
                                                        end_time[index:index+d_len],
                                                        rainfall[index:index+d_len])
                        date = str(y_id)+"/"+str(m_id)+"/"+str(d_id)
                        for hour, value in zip(range(24), day_timeseries):
                            fl.write("\t"+date+"\t"+str(hour)+":00:00\t"+str(value)+"\n")
                        index += d_len
            fl.close()


# path = r"F:\抚州项目(验收)\项目建模数据\\2 降雨流量数据处理\0、原始数据\1降雨洪水原始数据04-16.xlsx"
# result = r"C:\Users\gr\Desktop\fuhe_hydro2"
# non_basic = Process_nonBasic_Station(path, result)
# non_basic.write_pcp_file()


'''处理水文站流量数据（格式3）'''
class Process_DischargeData():
    path, sheet_id, station_cord = "", "", 0

    def __init__(self, path, sheet, station):  # path是数据表格文件，sheet是表格中目标页， station是水文站的站码。
        self.path, self.sheet_id, self.station_cord = path, sheet, station
        app = xlw.App(visible=False, add_book=False)
        excel = app.books.open(path)
        self.__sheet = excel.sheets[self.sheet_id]
        self.all_day_timeseries = []  # 存储所有天的序列（不区分年月）。
        self.__data_year = []         # 存储年的数据列。
        self.__data_monthday = []     # 存储月日的数据列。
        self.__data_hour = []         # 存储时的数据列。
        self.__data_q = []            # 存储流量数据列。

        # 获取页内最大行数并返回。
    def get_sheet_nrows(self):
        return self.__sheet.used_range.last_cell.row

    # 获取年、月日、时、流量数据列并返回列长度。
    def extract_station_data(self):
        nrows = self.get_sheet_nrows()  # 获取行数。
        station_start, station_end, row = nrows, 0, 2
        for cell in self.__sheet.range("A2:A" + str(nrows)).value:
            if int(cell) == self.station_cord:
                station_start = min(station_start, row)
            if int(cell) == self.station_cord:
                station_end = max(station_end, row)
            row += 1

        self.__data_year = self.__sheet.range("B" + str(station_start) + ":B" + str(station_end)).value
        self.__data_monthday = self.__sheet.range("C" + str(station_start) + ":C" + str(station_end)).value
        self.__data_hour = self.__sheet.range("D" + str(station_start) + ":D" + str(station_end)).value
        self.__data_q = self.__sheet.range("F" + str(station_start) + ":F" + str(station_end)).value
        return station_end-station_start+1

    # 重整所有天的时间序列数据。
    def reset_station_formatter(self, day_len):
        all_day_timeseries = self.__get_day_timeseries_q(day_len)
        all_day_timeseries = self.__insert_miss_day(all_day_timeseries)
        self.all_day_timeseries = all_day_timeseries

    # 将所有天的序列匹配到年月并输出到文件。
    def write_qobs_file(self, result_folder):
        fl = open(result_folder+"\\qobs_"+str(self.station_cord)+".txt", "w")
        day_len = self.__get_day_length()
        self.reset_station_formatter(day_len)

        index, data_count = 0, len(day_len)
        if len(self.all_day_timeseries) != data_count:
            print("exception")
            exit(1)

        year1, year2 = 0, self.__data_year[0]
        date1, date2 = 0, day_len[0][0]
        while index < data_count-1:
            date1 = date2
            year1 = year2
            index += 1
            date2 = day_len[index][0]
            year2 = self.__data_year[day_len[index][1]]

            date = str(year1)[:-2]+"/"+str(date1)[:-4]+"/"+str(date1)[-4:-2]+"\t"
            for i, value in zip(range(24), self.all_day_timeseries[index-1][1]):
                fl.write(date+str(i)+":00:00\t"+str(value)+"\n")

            if year2 == year1 and str(date2)[:-4] == str(date1)[:-4]:
                day2, day1 = int(str(date2)[-4:-2]), int(str(date1)[-4:-2])
                day_dis = day2 - day1
                if day_dis > 1:
                    n = 1
                    q_day1 = self.all_day_timeseries[index-1][1][-1]
                    q_day2 = self.all_day_timeseries[index][1][0]
                    q_step = (q_day2 - q_day1)/(24.0*(day_dis-1))     # 时间间隔
                    while n < day_dis:
                        date = str(year1)[:-2] + "/" + str(date1)[:-4] + "/" + str(day1+n) + "\t"
                        n += 1
                        for i in range(24):
                            fl.write(date+str(i)+":00:00\t"+str(q_day1+(n-1)*24*q_step+i*q_step) +"\n")

        date = str(year2)[:-2]+"/"+str(date2)[:-4]+"/"+str(date2)[-4:-2]+"\t"
        for i, value in zip(range(24), self.all_day_timeseries[index][1]):
            fl.write(date + str(i) + ":00:00\t" + str(value) + "\n")
        fl.close()

    # 获取年、月、日的列表并返回。
    def get_data_list(self):
        year_list, month_list = [], []
        index, count = 0, len(self.__data_year)
        year1, year2, month1, month2 = self.__data_year[0], 0, str(self.__data_monthday[0]), ""
        while index < count-2:
            index += 1
            year2 = self.__data_year[index]
            if year2 != year1:
                year_list.append(int(year1))
            year1 = year2

            month2 = str(self.__data_monthday[index])
            if month1[:-4] != month2[:-4]:
                month_list.append(int(month1[:-4]))
            month1 = month2

        day_list = [day[0] for day in self.all_day_timeseries]
        return (year_list, month_list, day_list)

    # 向已经重整过的天时间序列中插入缺失的天的数据。
    def __insert_miss_day(self, all_day_timeseries):
        new_all_day_timeseries = []
        index, count = 0, len(all_day_timeseries)
        while index < count-1:
            day1, day2 = all_day_timeseries[index], all_day_timeseries[index+1]
            new_all_day_timeseries.append(day1)
            day_dis = day2[0] - day1[0]
            if day_dis > 1:
                add = 1
                q = (day1[1][-1] + day2[1][0])/2
                while add < day_dis:
                    timeseries = [q for i in range(24)]
                    new_all_day_timeseries.append((day1[0]+add, timeseries))
                    add += 1
            index += 1
        return new_all_day_timeseries

    # 获取已有的每一天的原始数据长度并返回。
    def __get_day_length(self):
        day_len = []

        index, day_start, count = 0, 0, len(self.__data_monthday)
        date1, date2 = str(self.__data_monthday[0]), str(self.__data_monthday[0])
        while index < count-1:
            date1 = date2
            index += 1

            date2 = str(self.__data_monthday[index])
            if date1 != date2:
                day_len.append((date1, day_start, index - day_start))
                day_start = index

        day_len.append((date2, day_start, index - day_start+1))
        return day_len

    # 获取已有的每一天的流量数据序列并返回。
    def __get_day_timeseries_q(self, day_len):
        all_day_timeseries, timeseries = [], []
        index, count = 0, len(day_len)

        while index < count:
            day = day_len[index]
            hours, q = self.__data_hour[day[1]:day[1]+day[2]], self.__data_q[day[1]:day[1]+day[2]]

            if index > 0:  q_before = ((all_day_timeseries[-1])[1])[-1]  # 前面的日期已经被处理
            else:          q_before = -1

            if index < count-1:
                day_after = day_len[index+1]    # 后面的日期尚未处理
                hour_after, q_after = self.__data_hour[day_after[1]], self.__data_q[day_after[1]]
            else:
                hour_after, q_after = 0, -1

            timeseries = self.__reset_timeseries(hours, q, q_before, hour_after, q_after)

            all_day_timeseries.append((day[0], timeseries))
            index += 1
        return all_day_timeseries

    # 重整填充每一天的流量数据序列。
    def __reset_timeseries(self, day_hour, day_q, q_before, hour_after, q_after):
        timeseries = [0.0 for x in range(24)]
        existQ_hours = []
        for hour, q in zip(day_hour, day_q):
            h = int(hour / 100)
            timeseries[h] = q
            existQ_hours.append(h)           # 不能用timeseries.index(q)，q会有重复

        if q_before < 0 < existQ_hours[0]:  # q_before如果为负，说明没有前一天数据
            hour0 = existQ_hours[0]
            for i in range(hour0):
                timeseries[i] = timeseries[hour0]
        elif q_before > 0 and existQ_hours[0] > 0:
            hour0 = existQ_hours[0]
            q0 = timeseries[hour0]
            hour_dis = hour0 + 1         # 默认前一个是整理好的时间序列，到当前第一个有效时刻hour0
            q_step = round((q0 - q_before)/hour_dis, 3)
            for i in range(hour_dis-1):  # 只需要处理第一个有效时刻之前的时刻
                timeseries[i] = q_before + q_step*i

        if q_after < 0 and existQ_hours[-1] < 23:   # q_after如果为负，说明没有下一天数据
            hour1 = existQ_hours[-1]
            for i in range(hour1, 23):
                timeseries[i] = timeseries[hour1]
        elif q_after > 0 and existQ_hours[-1] < 23:
            hour1 = existQ_hours[-1]
            q1 = timeseries[hour1]
            hour_dis = hour_after + 24 - hour1
            q_step = round((q_after - q1)/hour_dis, 3)
            for i in range(hour1+1, 24):   # 只需要处理最后一个有效时刻hour1之后的时刻
                timeseries[i] = timeseries[i-1] + q_step

        index, count = 0, len(existQ_hours)
        while index < count-1:
            hour0, hour1 = existQ_hours[index], existQ_hours[index+1]
            hour_dis = hour1 - hour0     # 这里不用加1
            if hour_dis <= 1:
                index += 1
                continue
            q0, q1 = timeseries[hour0], timeseries[hour1]
            q_step = round((q1 - q0)/hour_dis, 3)
            for i in range(hour0+1, hour1):
                timeseries[i] = timeseries[i-1] + q_step
            index += 1

        return timeseries


# path = r"F:\抚州项目(验收)\项目建模数据\\2 降雨流量数据处理\0、原始数据\1降雨洪水原始数据04-16.xlsx"
# result_file = "C:\\Users\\gr\\Desktop\\fuhe_hydro3"
# sheet_id = "2006-2014洪水水文要素摘录表"
# station = 62405400                  # 当前流域流量站的站码
# discharge = Process_DischargeData(path, sheet_id, station)
# print("here1")
# discharge.extract_station_data()
# print("here2")
# #discharge.reset_station_formatter()
# print("here3")
# discharge.write_qobs_file(result_file)
# print("here4")


''' 遍历并提取水库的流量数据表格（格式4） '''
def read_reservoir_data(data_folder, result_folder, station):
    prefix = "廖坊坝上站洪水要素摘录统计表"
    year_range = (2010, 2014)

    result = open(result_folder+"\\qobs_"+str(station)+".txt", "w")

    for year in range(10, 15):
        excel_path = data_folder + "\\" + prefix + str(year) + ".xlsx"
        app = xlw.App(visible=False, add_book=False)
        excel = app.books.open(excel_path)
        sheet = excel.sheets["Sheet1"]

        nrows = sheet.used_range.last_cell.row
        date = sheet.range("A4:A"+str(nrows-5)).value
        discharge = sheet.range("C4:C"+str(nrows-5)).value

        for d, q in zip(date, discharge):
            d_arr = d.split()
            date_arr = d_arr[0].split("-")
            month, day = int(date_arr[1]), int(date_arr[2])   # 将2010/01/02改为2010/1/2
            result.write(date_arr[0]+"/"+str(month)+"/"+str(day)+"\t")
            result.write(d_arr[1]+":00:00\t"+str(q)+"\n")
    result.close()


# data_folder = "F:\抚州项目(验收)\项目建模数据\水库调研数据\廖坊"
# result_folder = "C:\\Users\\gr\\Desktop\\fuhe_hydro3"
# station = 62430700
# read_reservoir_data(data_folder, result_folder, station)


''' 遍历并提取水库的流量数据表格（格式5） '''
def read_reservoir_data_2(data_path, result_folder, station):
    result = open(result_folder+"\\qobs_"+str(station)+".txt", "w")

    app = xlw.App(visible=False, add_book=False)
    excel = app.books.open(data_path)
    sheet = excel.sheets["Sheet1"]
    nrows = sheet.used_range.last_cell.row

    date = sheet.range("B2:B"+str(nrows)).value         # date读取的是datetime格式，无法split（）
    discharge = sheet.range("D2:D"+str(nrows)).value
    count = nrows - 1

    year, month, day, hour = date[0].year, date[0].month, date[0].day, date[0].hour  # datetime数据类型。
    date_formatter = str(year)+"/"+str(month)+"/"+str(day)+"\t"+str(hour)+":00:00\t"
    result.write(date_formatter + str(discharge[0]) + "\n")

    index = 1
    while index < count:
        if date[index] == date[index-1]:
            index += 1
            continue

        year, month, day, hour = date[index].year, date[index].month, date[index].day, date[index].hour
        date_formatter = str(year) + "/" + str(month) + "/" + str(day) + "\t" + str(hour) + ":00:00\t"
        result.write(date_formatter + str(discharge[index]) + "\n")
        index += 1
    result.close()


# data_path = "F:\抚州项目(验收)\项目建模数据\水库调研数据\洪门\时段流量\整理\坝上.xlsx"
# result_folder = "C:\\Users\\gr\\Desktop\\fuhe_hydro3"
# station = 61800012
# read_reservoir_data_2(data_path, result_folder, station)


''' 从流量站的已处理文件提取洪水场次 （格式6）'''
def add_year_filtrate(year_range):            # 带有参数的装饰器,限定选择的年份区间，减少计算量。
    def handler(class_method):                # 装饰器封装
        def wrapper(self, *args, **kwargs):   # 装饰器
            class_method(self, *args, **kwargs)
            self.get_year_range(year_range[0], year_range[1])
            self.year_filtrate()              # 不能调用类的私有方法self.__method()
        return wrapper                        # 返回装饰器， 其实现将替代掉原函数class_method，但不改变原函数的接口
    return handler                            # 返回封装外壳

class Select_Flood_Events():
    def __init__(self, flood_folder, station):  # flood_folder是包含流量站数据文件的文件夹，根据默认命名规则找文件。
        self.folder, self.station = flood_folder, station
        self.start_y, self.end_y = 0, 0    # 洪水场次的年份区间。
        self.q, self.hour, self.date = [], [], []  # 分别存储流量、小时、日期数据。
        self.read_q_file(flood_folder, station)
        self.floods = []   # 存储洪水场次数据。

    #  获取日期、小时、流量数据。
    def read_q_file(self, folder, station):
        fl = open(folder+"\\qobs_"+str(station)+".txt", "r")
        for line in fl.readlines():
            line_arr = line.strip().split()
            self.date.append(line_arr[0])
            self.hour.append(line_arr[1])
            self.q.append(float(line_arr[2]))
        fl.close()

    # 获取所有数据中的平均流量值和最大流量值并返回，供选取场次参考。
    def get_averageAndmax_q(self):
        average_q, max_q = 0, 0
        count = len(self.q)
        for q in self.q:
            average_q += q
            max_q = max(max_q, q)
        average_q /= count
        return (average_q, max_q)

    # 在给定年份中粗率挑选符合流量值要求的流量场次。
    @add_year_filtrate([2010, 2018])
    def select_flood_date_range(self, flood_level, extend_hours, extend_days):
        count, index = len(self.q), 0
        while index < count - 1:
            if self.q[index] >= flood_level:
                if index < extend_hours + 24*extend_days:
                    self.floods.append((0, index + extend_hours))
                elif extend_hours+24*extend_days <= index < count - extend_hours:
                    self.floods.append((index - extend_hours - 24*extend_days, index + extend_hours))
                elif index >= count - extend_hours - 1:
                    self.floods.append((index - extend_hours - 24*extend_days, count - 1))
                index += 2*extend_hours
                continue
            index += 1

    # 输入洪水场次的年份区间。
    def get_year_range(self, start_year, end_year):
        self.start_y = start_year
        self.end_y = end_year

    # 根据输入的年份区间，筛选粗选的流量场次（其实加了装饰器之后已经务必要）。
    def year_filtrate(self):
        index_keep = []
        for i, date in zip(range(len(self.floods)), self.floods):
            start_date, end_date = self.date[date[0]].split('/'), self.date[date[1]].split('/')
            start_year, end_year = int(start_date[0]), int(end_date[0])
            if start_year >= self.start_y and end_year <= self.end_y:
                index_keep.append(i)

        floods_keep = [self.floods[i] for i in index_keep]
        self.floods = floods_keep

    # 输出洪水场次结果，采用默认命名规则。
    def write_floods_file(self, result_folder=""):
        if result_folder == "": result_folder = self.folder
        fl = open(result_folder+"\\floods_"+str(self.station)+".txt", "w")
        print("洪水场次数量：%d" % len(self.floods))
        fl.write("*************  total floods num: %d\n\n" % len(self.floods))
        for flood in self.floods:
            start, end = flood[0], flood[1]
            for index in range(start, end+1):
                fl.write("\t"+self.date[index]+"\t"+self.hour[index]+"\t"+str(self.q[index])+"\n")
            fl.write("\n\n")
        fl.close()


# result_file = "C:\\Users\\gr\\Desktop\\fuhe_hydro3"
# station = 61800012
# flood = Select_Flood_Events(result_file, station)
# averageQ, maxQ = flood.get_averageAndmax_q()
# #flood_level = maxQ*0.1
# flood_level = averageQ*0.01
# # extend_hours 必须可以使各flood_range的开始日期不会重复，否则检索会有异常。
# # extend_hours 应当考虑到流量站的当前流量可能是来自远处雨量站之前的降雨，而非当前的降雨（应当包含）。
# extend_hours = 72
# extend_days = 3
# flood.select_flood_date_range(flood_level, extend_hours, extend_days)
# flood.write_floods_file()


''' 从流量站选出的流量场次中筛选基本站非基本站都有数据的洪水场次 '''
class Select_Shared_Floods():
    def __init__(self, folder, station):  # folder是包含流量站流量场次的文件夹，station是流量站站码。
        self.folder, self.station = folder, station
        self.floods_range = []  # 存储流量站的洪水场次。
        self.read_flood_file()
        self.shared_flood = {}  # 存储所有站点共享的洪水场次。
        self.shared_flood_len = []  # 存储所有站点共享的洪水场次的长度（当前应是一样的）。
        self.weighted_pcp = []  # 存储各个站点的加权系数。

    # 获取流量站的各个场次。
    def read_flood_file(self):
        fl = open(self.folder + "\\floods_" + str(self.station) + ".txt", "r")
        data = fl.readlines()
        fl.close()
        index, count = 2, len(data)   # 数据从第三行开始
        while index < count - 1:
            flood_start = data[index].strip().split()
            start_index = index
            while len(data[index].strip().split()) != 0 and index < count - 1:
                index += 1
            flood_end = data[index - 1].strip().split()
            end_index = index - 1
            while len(data[index].strip().split()) == 0 and index < count - 1:
                index += 1
            self.floods_range.append((flood_start, flood_end, end_index-start_index+1))

    # 根据给定的年份区间，筛选流量站的洪水场次。
    def select_years_range(self, start_year, end_year):
        if start_year > end_year: exit("start_year equal to end_year")
        selected_flood_range = []
        for flood in self.floods_range:
            flood_start_date = flood[0][0].split("/")
            flood_end_date = flood[1][0].split("/")

            if int(flood_start_date[0]) < start_year:
                continue  # 这里不考虑调整场次范围（长度较短，且只粗率到年）
            if int(flood_end_date[0]) > end_year:
                continue
            selected_flood_range.append(flood)
        self.floods_range = selected_flood_range

    # 分别获取每个站点中包含的流量站洪水场次。
    def find__flood_events(self, station_folder, station_list):
        exception_end_cord = {}
        exception_start_cord = {}

        for cord in station_list:
            exception_end_cord[cord] = 0
            exception_start_cord[cord] = 0

            data_file = station_folder+"\\pcp_"+str(cord)+".txt"
            fl = open(data_file, "r")
            data = fl.readlines()
            fl.close()

            current_floods = []
            index, count = 0, len(self.floods_range)
            while index < count:
                flood_start, flood_end = self.floods_range[index][0], self.floods_range[index][1]
                start_index, end_index, flood_len = len(data), 0, self.floods_range[index][2]
                for date in data:
                    arr = date.strip().split()
                    if arr[0] == flood_start[0] and arr[1] == flood_start[1]:
                        start_index = min(data.index(date), start_index)
                    elif arr[0] == flood_end[0] and arr[1] == flood_end[1]:
                        end_index = max(data.index(date), end_index)
                    # else:         # 如果时间序列在一个月内，判断当前月份中可能存在的时间序列片段（与其他函数不匹配）
                    #     year, month, day = arr[0].split("/")         # split()返回列表，但这样似乎使用没有异常
                    #     year_start, month_start, day_start = flood_start[0].split("/")
                    #     year_end, month_end, day_end = flood_end[0].split("/")
                    #     if year == year_start and month == month_start and int(day) >= int(day_start):
                    #         start_index = min(data.index(date), start_index)
                    #     elif year == year_end and month == month_end and int(day) <= int(day_end):
                    #         end_index = max(data.index(date), end_index)

                if start_index >= end_index:
                    if start_index < len(data):
                        exception_end_cord[cord] = (exception_end_cord[cord] + 1)
                        # print("\n****** station: %d, data exception at <end, %s %s> ********\n"
                        #       % (cord, flood_end[0], flood_end[1]))
                    elif end_index > 0:
                        exception_start_cord[cord] = (exception_start_cord[cord] + 1)
                        # print("\n****** station: %d, data exception at <start, %s %s> ********\n"
                        #       % (cord, flood_start[0], flood_start[1]))
                    index += 1
                    continue
                else:
                    end_index = end_index+1 if end_index < len(data)-1 else end_index
                    if (end_index - start_index) != flood_len:    # 为避免日期之间有遗漏
                        index += 1
                        continue

                    current_floods.append(data[start_index:end_index])
                    index += 1
            self.shared_flood[cord] = current_floods

    # 从各个站点包含的洪水场次中筛选共同拥有的洪水场次。
    def select_shared_flood(self):
        start_date_q = [start[0][0] for start in self.floods_range]

        for keys, datas in self.shared_flood.items():
            start_date_current = [(date[0].strip().split())[0] for date in datas]
            shared_range = [start for start in start_date_q if start in start_date_current]
            start_date_q = shared_range
        self.floods_range = [flood for flood in self.floods_range if flood[0][0] in start_date_q]
        print("洪水场次数量：%d" % len(self.floods_range))

        for key, data in self.shared_flood.items():
            start_date_current = [(date[0].strip().split())[0] for date in data]
            shared_range_index = [start_date_current.index(start) for start in start_date_current if start in start_date_q]
            self.shared_flood[key] = [data[index] for index in shared_range_index]

        key = list(self.shared_flood.keys())[0]
        self.shared_flood_len = [len(data) for data in self.shared_flood[key]]

    # 从文件中获取所有站点共享的洪水场次（已经筛选过的洪水场次，调整权重后重新计算使用。）
    def get_shared_flood_range_fromFile(self, path):
        flood_file = open(path, "r")
        data = flood_file.readlines()
        flood_file.close()

        self.floods_range.clear()
        index, count = 0, len(data)
        while index < count - 1:
            flood_start = data[index].strip().split()
            while len(data[index].strip().split()) != 0 and index < count-1:
                index += 1
            flood_end = data[index-1].strip().split()
            while len(data[index].strip().split()) == 0 and index < count-1:
                index += 1
            self.floods_range.append((flood_start, flood_end))

    # 将共享的洪水场次输出。
    def write_shared_flood(self, area_id):
        fl_result = open(self.folder + "\\floods_" + str(self.station) + "_shared.txt", "w")
        fl_result.write("****************** total floods num: %d\n\n" % len(self.floods_range))

        fl_orign = open(self.folder + "\\floods_" + str(self.station) + ".txt", "r")
        data = fl_orign.readlines()
        fl_orign.close()

        index, count = 2, len(data)
        flood_index, flood_count = 0, len(self.floods_range)
        while index < count-1 and flood_index < flood_count:
            arr = data[index].strip().split()
            if len(arr) == 0:
                index += 1
                continue
            if arr[0] == self.floods_range[flood_index][0][0]:
                while len(data[index].strip().split()) > 0 and index < count-1:
                    fl_result.write(str(area_id) + "" + data[index])
                    index += 1
                fl_result.write("\n\n")
                flood_index += 1
            else:
                while len(data[index].strip().split()) > 0 and index < count-1:
                    index += 1
            index += 1
        fl_result.close()

    # 将共享的洪水场次输出（版本2）。
    def write_shared_flood_v2(self, area_id):
        fl_result = open(self.folder + "\\floods_" + str(self.station) + "_shared.txt", "w")
        fl_result.write("****************** total floods num: %d\n\n" % len(self.floods_range))

        fl_orign = open(self.folder + "\\qobs_" + str(self.station) + ".txt", "r")
        data = fl_orign.readlines()
        fl_orign.close()

        flood_index, flood_count = 0, len(self.floods_range)
        while flood_index < flood_count:
            flood_start, flood_end = self.floods_range[flood_index][0], self.floods_range[flood_index][1]
            date1 = flood_start[0].split('/')
            date2 = flood_end[0].split('/')

            if int(date1[2]) > 9:
                flood_start_date = flood_start[0]
            else:
                flood_start_date = date1[0] + '/' + date1[1] + '/0' + str(int(date1[2]))
            if int(date2[2]) > 9:
                flood_end_date = flood_end[0]
            else:
                flood_end_date = date2[0] + '/' + date2[1] + '/0' + str(int(date2[2]))

            index, count = 0, len(data)
            while index < count:
                arr = data[index].strip().split()
                if arr[0] == flood_start_date and arr[1] == flood_start[1]:
                    while not (arr[0] == flood_end_date and arr[1] == flood_end[1]):
                        fl_result.write(str(area_id) + "\t" + data[index])
                        index += 1
                        arr = data[index].strip().split()
                    fl_result.write(str(area_id) + "\t" + data[index] + "\n\n")
                    flood_index += 1
                    break
                index += 1
            if index == count:
                print("flood event %s can't found in qobs file" % self.flood_range[flood_index])
                flood_index += 1
        fl_result.close()

    # 输出加权后的区域降雨场次。
    def write_inflow(self, area_id, inflow_cord):
        fl_result = open(self.folder + "\\inflow_" + str(self.station) + ".txt", "w")
        fl_result.write("****************** total floods num: %d\n\n" % len(self.floods_range))

        fl_orign = open(self.folder + "\\qobs_" + str(inflow_cord) + ".txt", "r")
        data = fl_orign.readlines()
        fl_orign.close()

        flood_index, flood_count = 0, len(self.floods_range)
        while flood_index < flood_count:
            flood_start, flood_end = self.floods_range[flood_index][0], self.floods_range[flood_index][1]

            index, count = 0, len(data)
            while index < count:
                arr = data[index].strip().split()
                if arr[0] == flood_start[0] and arr[1] == flood_start[1]:
                    while not (arr[0] == flood_end[0] and arr[1] == flood_end[1]):
                        fl_result.write(str(area_id) + "\t" + data[index])
                        index += 1
                        arr = data[index].strip().split()
                    fl_result.write(str(area_id) + "\t" + data[index] + "\n\n")
                    flood_index += 1
                    break
                index += 1
            if index == count :
                print("flood_event %s can't be found in inflow file." % self.floods_range[flood_index])
                flood_index += 1
        fl_result.close()

    # 加权处理区域内各站点的降雨场次。
    def generate_weighted_pcp(self, weight, area_id, result_file):
        fixed_order = list(self.shared_flood.keys())

        index, floods_count = 0, len(self.floods_range)
        while index < floods_count:
            current_flood_len = self.shared_flood_len[index]
            current_weighted_flood = [0.0 for x in range(current_flood_len)]
            for key in fixed_order:
                weight_coe = weight[key]
                flood = self.shared_flood[key][index]        # 各站点同一场次洪水
                if len(flood) != current_flood_len:
                    print(len(flood)-current_flood_len)
                    break
                for data in flood:
                    index_data = flood.index(data)
                    arr = data.strip().split()
                    rain = round(float(arr[2])*weight_coe, 3)
                    current_weighted_flood[index_data] += rain
            self.weighted_pcp.append(current_weighted_flood)
            index += 1

        fl = open(result_file, "w")
        index = 0
        fl.write("****************** total floods num: %d\n\n" % len(self.weighted_pcp))
        for flood in self.shared_flood[fixed_order[0]]:
            for data, rain in zip(flood, self.weighted_pcp[index]):
                date = data.strip().split()
                fl.write(str(area_id) + "\t" + date[0]+"\t"+date[1]+"\t"+str(rain)+"\n")
            fl.write("\n\n")
            index += 1
        fl.close()
        self.weighted_pcp.clear()

# 获取区域雨量站点字典
def get_station_id_area_coe(excel_path, area_name, out_path):
    app = xlw.App(visible=False, add_book=False)
    excel = app.books.open(excel_path)
    sheet = excel.sheets[area_name]

    station_name = sheet.range("B2:B39").value  # 现在需要手动输入。
    station_cord = sheet.range("A2:A39").value
    used_station_name = sheet.range("B68:P68").value
    used_station_coe = sheet.range("B70:P70").value

    outfile = open(out_path + "\\temp.txt", "w")
    used_station_cord_list = []
    for name in used_station_name:
        index = station_name.index(name)
        cord = int(station_cord[index])
        used_station_cord_list.append(cord)
        outfile.write("\"%s\":%d,  " % (name, cord))

    outfile.write("\n\n")
    for cord, coe in zip(used_station_cord_list, used_station_coe):
        outfile.write(str(cord) + ":" + str(coe) + ",  ")


# excel_path = "F:\抚州项目(验收)\抚河模型\抚州新安江水文模型\子流域雨量站.xlsx"
# out_path = "C:\\Users\\gr\\Desktop"
# area_name = "洪门"
# get_station_id_area_coe(excel_path, area_name, out_path)  # 获得雨量站字典后再手动复制到下面的station_id.

# folder = "C:\\Users\\gr\\Desktop\\fuhe_hydro3"
# station = 61800012
# floods = Select_Shared_Floods(folder, station)
# print("here1")
# # file_path = "F:\抚州项目(验收)\抚河模型\抚州新安江水文模型\\9李家渡上游段\李家渡前段_预热版\\李家渡前段_flood_events.txt"
# # floods.get_shared_flood_range_fromFile(file_path)
# start_year, end_year = 2010, 2018
# floods.select_years_range(start_year, end_year)
# print("here2")
# nonbasicStation_id = {  "河东":62427160,  "黎川":62403200,  "洵口":62427500,
#                       "樟溪":62426950,  "云屯洲":62426620,  "东堡":62427120,  "光辉":62426740,
#                       "岭下":62425750,  "胡家排":62427360,  "三源":62427320,  "华联":62427520,
#                       "荷源":62427540,  "塘坊":62425820,  "石陂":62425840}#
# folder_nonbasicStation = "C:\\Users\\gr\\Desktop\\fuhe_hydro2"
# floods.find__flood_events(folder_nonbasicStation, list(nonbasicStation_id.values()))
# print("here3")
# basicStation_id = {"石璜":62428500}#
# folder_basicStation = "C:\\Users\\gr\\Desktop\\fuhe_hydro"
# floods.find__flood_events(folder_basicStation, list(basicStation_id.values()))
# print("here4")
# area_id = 140
# floods.select_shared_flood()
# floods.write_shared_flood(area_id)
# # floods.write_shared_flood_v2(area_id)
# # inflow_cord = 61800012
# # floods.write_inflow(area_id, inflow_cord)
# print("here5")
# result_folder = "C:\\Users\\gr\\Desktop\\fuhe_hydro3\\rain_" + str(station) + ".txt"
# weights = {62428500:0.1271128739573496,  62427160:0.101014272162222,  62403200:0.0475686611665322,
#            62427500:0.0267977275113812,  62426950:0.102321320633231,  62426620:0.098312812110113409,
#            62427120:0.05322631226291139,  62426740:0.0587015041809035,  62425750:0.0486841920640099,
#            62427360:0.09265017457632120,  62427320:0.0232239805918511,  62427520:0.115736981699318,
#            62427540:0.0366719533060902,  62425820:0.0592549698642108,  62425840:0.0385705775570474}
# floods.generate_weighted_pcp(weights, area_id, result_folder)
# print("here6")


import re
import os


''' 依据选取洪水场次和加权处理的降雨场次生成新安江模型文件 '''
class Write_Standard_XAJ():
    def __init__(self, data_folder, out_folder, cord):  # 输入洪水场次和降雨场次文件夹，结果文件夹，流量站站码。
        self.data_folder = data_folder
        self.cord = cord
        self.out_folder = out_folder
        self.events = []  # 存储各场洪水场次起止时间。
        self.__read_events()  # 存储读取的洪水场次。
        self.__creat_event_folder()

    # 获取各场洪水的起止时间。
    def __read_events(self):
        rain_file = self.data_folder + "\\rain_" + str(self.cord) + ".txt"
        fl = open(rain_file, "r")
        fl_data = fl.readlines()
        fl.close()

        index, count = 2, len(fl_data)
        while index < count-1:
            event_start = fl_data[index].strip().split()
            event_start = event_start[1:3]
            while len(fl_data[index].strip().split()) > 0 and index < count-1:
                index += 1
            event_end = fl_data[index-1].strip().split()
            event_end = event_end[1:3]
            while len(fl_data[index].strip().split()) == 0 and index < count-1:
                index += 1
            self.events.append([event_start, event_end])

    # 创建新安江模型各场次的文件夹。
    def __creat_event_folder(self):
        for event in self.events:
            event_date = event[0][0]
            event_date = re.sub("/", "-", event_date)      # 正则表达式替换字符
            folder = self.out_folder + "\\" + event_date
            if not os.path.exists(folder):
                os.mkdir(self.out_folder + "\\" + event_date)

    # 生成qobs流量文件。
    def write_qobs_file(self):
        qobs_file = self.data_folder + "\\floods_" + str(self.cord) + "_shared.txt"
        fl = open(qobs_file, "r")
        fl_data = fl.readlines()
        fl.close()

        index, count = 2, len(fl_data)
        while index < count:
            line = fl_data[index].strip().split()

            if len(line) > 0:
                event_date = re.sub("/", "-", line[1])
                qobs = self.out_folder + "\\" + event_date + "\\qobs.txt"
                fl = open(qobs, "w")
                while len(line) > 0 and index < count:
                    fl.write(fl_data[index])
                    index += 1
                    line = fl_data[index].strip().split()
            index += 1

    # 生成inflow入流文件。
    def write_inflow_file(self):
        inflow_file = self.data_folder + "\\inflow_" + str(self.cord) + ".txt"
        fl = open(inflow_file, "r")
        fl_data = fl.readlines()
        fl.close()

        index, count = 2, len(fl_data)
        while index < count:
            line = fl_data[index].strip().split()

            if len(line) > 0:
                event_date = re.sub("/", "-", line[1])
                inflow = self.out_folder + "\\" + event_date + "\\inflow.txt"
                fl = open(inflow, "w")
                while len(line) > 0 and index < count:
                    fl.write(fl_data[index])
                    index += 1
                    line = fl_data[index].strip().split()
            index += 1

    # 生成pcp降雨文件。
    def write_pcp_file(self):
        rain_file = self.data_folder + "\\rain_" + str(self.cord) + ".txt"
        fl = open(rain_file, "r")
        fl_data = fl.readlines()
        fl.close()

        index, count = 2, len(fl_data)
        while index < count:
            line = fl_data[index].strip().split()

            if len(line) > 0:
                event_date = re.sub("/", "-", line[1])
                rain = self.out_folder + "\\" + event_date + "\\pcp.txt"
                fl = open(rain, "w")
                while len(line) > 0 and index < count:
                    fl.write(fl_data[index])
                    index += 1
                    line = fl_data[index].strip().split()
            index += 1

    # 生成task配置文件。
    def write_task_file(self):
        example_task = self.out_folder + "\\example_task.txt"
        fl = open(example_task, "r")
        fl_data = fl.readlines()
        fl.close()

        for event in self.events:
            event_start_date, event_start_hour = event[0][0], event[0][1]
            event_end_date, event_end_hour = event[1][0], event[1][1]
            event_start_date = re.sub("/", "-", event_start_date)
            event_end_date = re.sub("/", "-", event_end_date)
            folder = self.out_folder + "\\" + event_start_date + "\\task.txt"
            fl = open(folder, "w")

            index, count = 0, len(fl_data)
            while index < count:
                if fl_data[index] == fl_data[2]:
                    fl.write(fl_data[index])
                    fl.write("START_DATETIME       %sT%s\n"%(event_start_date, event_start_hour))
                    fl.write("END_DATETIME         %sT%s\n" % (event_end_date, event_end_hour))
                    index += 3
                fl.write(fl_data[index])
                index += 1
            fl.close()

    # 生产floods场次选择文件。
    def write_floods_file(self):
        floods = self.out_folder + "\\floods.txt"
        fl = open(floods, "w")
        for event in self.events:
            event_date = event[0][0]
            event_date = re.sub("/", "-", event_date)
            fl.write(event_date + "\n")


# data_folder = "C:\\Users\\gr\\Desktop\\fuhe_hydro3"
# out_folder = "C:\\Users\\gr\\Desktop\\xaj"
# cord = 61800012
# xaj = Write_Standard_XAJ(data_folder, out_folder, cord)
# xaj.write_qobs_file()
# xaj.write_pcp_file()
# # xaj.write_inflow_file()
# xaj.write_task_file()
# xaj.write_floods_file()


# 删除各场次中的result.txt文件。
def del_resultFile_inFolder(folder_path):
    for pathes, folders, files in os.walk(folder_path):
        for folder in folders:
            child_folder_path = folder_path+"\\"+folder
            for child_pathes, child_folders, child_files in os.walk(child_folder_path):
                for file in child_files:
                    fname, ftype = os.path.splitext(file)
                    if fname == "result":
                        os.remove(child_folder_path+"\\"+file)


# folder_path = "F:\抚州项目(验收)\抚河模型\抚州新安江水文模型\9李家渡上游段\李家渡前段_预热版\李家渡前段"
# del_resultFile_inFolder(folder_path)


''' 处理雨量站数据（格式7） '''
class process_rainstation_data():
    big_month = [1, 3, 5, 7, 8, 10, 12]  # 大月31天。
    small_month = [4, 6, 9, 11]  # 小月30天。
    super_small_month = [2]  # 2月28天。
    year, stationCord, stationID = 2016, 0, ""

    def __init__(self, excel_path, txt_path):
        self.__excel_path = excel_path
        self.__txt_path = txt_path
        self.__rainfall_tseries = []  # 存储降雨时间序列数据。
        self.__data = []  # 存储表格中各页中各组数据。
        self.__read_excel()

    # 读取表格中各页中各组的数据。
    def __read_excel(self):
        app = xlw.App(visible=False)
        excel = app.books.open(self.__excel_path)
        # sheet = excel.sheets["Sheet1"]
        sheet = excel.sheets["1996"]

        ID = sheet.range("A1").value.split()
        # self.stationID = ID[1][:-7]
        self.stationID = ID[1]
        self.year = int(sheet.range("C2").value)
        self.stationCord = sheet.range("F2").value

        page1 = sheet.range("A5:E39").value  # 现在还需要手动配置。
        page2 = sheet.range("F5:J39").value
        page3 = sheet.range("K5:O39").value
        page4 = sheet.range("P5:T39").value

        page5 = sheet.range("A59:E98").value
        page6 = sheet.range("F59:J98").value
        page7 = sheet.range("K59:O98").value
        page8 = sheet.range("P59:T98").value

        page9 = sheet.range("A113:E128").value
        page10 = sheet.range("F113:J128").value
        page11 = sheet.range("K113:O128").value
        page12 = sheet.range("P113:T128").value
        self.__data = [page1, page2, page3, page4, page5, page6, page7, page8, page9, page10, page11, page12]

    # 获取已有的降雨时间序列（年月日时匹配）。
    def __get_rainfall_timeseries(self):
        month, day = 0, 0
        for page in self.__data:
            for line in page:
                if line[2] is None: continue
                if line[0] is not None: month = int(line[0])
                if line[1] is not None: day = int(line[1])
                arr2, arr3 = str(line[2]).split(':'), str(line[3]).split(':')
                h_start, h_end = float(arr2[0]), float(arr3[0])
                rainfall = float(line[4])
                self.__rainfall_tseries.append([self.year, month, day, int(h_start), int(h_end), rainfall])

    # 填满每天的时间序列（1~24h）。
    def __complete_hour(self):
        new_hour_rain = []
        index, index_last, count = 0, 0, len(self.__rainfall_tseries)
        while index < count-1:
            day = self.__rainfall_tseries[index][2]
            if day != self.__rainfall_tseries[index+1][2]:  # 当前日的时间序列
                month = self.__rainfall_tseries[index][1]
                rain_slice = self.__rainfall_tseries[index_last:index+1]
                index_last = index + 1
                new_rain = []

                start_h = rain_slice[0][3]
                num = 1
                while num <= start_h:  # 当前日的开始时间
                    new_rain.append([self.year, month, day, num, 0])
                    num += 1

                for slices in rain_slice:  # 中间时间
                    start_h, end_h, rain = slices[3], slices[4], slices[5]
                    if end_h < start_h:
                        end_h = 24 + end_h
                    if end_h-start_h > 1:
                        dis, step = end_h - start_h, rain/(end_h - start_h)
                        num = 1
                        while num <= dis and (start_h+num <= 24):
                            new_rain.append([self.year, month, day, start_h+num, step])
                            num += 1
                        continue
                    else:
                        new_rain.append([self.year, month, day, end_h, rain])

                start_h, end_h, rain = rain_slice[-1][3], rain_slice[-1][4], rain_slice[-1][5]
                if start_h < end_h < 24:  # 末尾时间
                    dis, step = 24 - end_h, 0
                    num = 1
                    while num <= dis:
                        new_rain.append([self.year, month, day, end_h + num, step])
                        num += 1
                # if end_h < start_h and day == (self.__rainfall_tseries[index + 1][2] - 1): # 末尾时间
                #     dis, step = end_h, rain / (end_h+24-start_h)
                #     num = 1
                #     while num <= dis:
                #         new_rain.append([self.year, month, day + 1, num, step])
                #         num += 1

                exist_hour = [rain[3] for rain in new_rain]
                miss_hour = [hour for hour in range(1, 25) if hour not in exist_hour]
                for h in range(1, 25):  # 填充当日时间序列中缺失的时间
                    if h in miss_hour:
                        new_hour_rain.append([self.year, month, day, h, 0])
                        continue
                    num = exist_hour.index(h)
                    new_hour_rain.append(new_rain[num])
            index += 1
        return new_hour_rain

    # 填满每月的天序列（分大小月，2月28天）。
    def __complete_day(self, new_hour_rain):
        new_day_rain = []
        index, index_last, count = 0, 0, len(new_hour_rain)

        while index < count:
            month = new_hour_rain[index][1]
            if index == count-1 and index - index_last > 0:
                rain_slice = new_hour_rain[index_last:]
            elif month != new_hour_rain[index+1][1]:
                rain_slice = new_hour_rain[index_last:index+1]
                index_last = index+1
            else:
                index += 1
                continue

            exist_day = [rain[2] for rain in rain_slice]  # 每一天长度为24
            if month in self.big_month:
                month_len = 31
            elif month in self.small_month:
                month_len = 30
            else:
                month_len = 28
            miss_day = [day for day in range(1, month_len + 1) if day not in exist_day]
            for day in range(1, month_len + 1):
                if day in miss_day:
                    for h in range(1, 25):
                        new_day_rain.append([self.year, month, day, h, 0])
                    continue
                num = exist_day.index(day)
                for h in range(24):
                    new_day_rain.append(rain_slice[num + h])
            # if month != new_hour_rain[index+1][1]:
            #     rain_slice = new_hour_rain[index_last:index+1]
            #     index_last = index+1
            #
            #     exist_day = [rain[2] for rain in rain_slice] # 每一天长度为24
            #     if month in self.big_month:  month_len = 31
            #     elif month in self.small_month:  month_len = 30
            #     else:  month_len = 28
            #     miss_day = [day for day in range(1, month_len+1) if day not in exist_day]
            #     for day in range(1, month_len+1):
            #         if day in miss_day:
            #             for h in range(1, 25):
            #                 new_day_rain.append([self.year, month, day, h, 0])
            #             continue
            #         num = exist_day.index(day)
            #         for h in range(24):
            #             new_day_rain.append(rain_slice[num+h])
            index += 1
        return new_day_rain

    # 填补已有时间序列的缺失并返回新的时间序列。
    def complete_rainfall_timeseries(self):
        self.__get_rainfall_timeseries()
        new_hour_rain = self.__complete_hour()
        new_day_rain = self.__complete_day(new_hour_rain)
        self.__rainfall_tseries = new_day_rain
        return new_day_rain

    # 输出时间序列。
    def write_rainfall_tseries(self):
        if os.path.isfile(self.__txt_path):
            outfile = self.__txt_path
        elif os.path.isdir(self.__txt_path):
            outfile = self.__txt_path + "\\" + self.stationID + "降雨站.txt"
        else:
            outfile = self.stationID + "降雨站.txt"
        fl = open(outfile, "w")
        fl.write("站点名称/站点站码：\t“" + self.stationID + "”:" + self.stationCord + ",\n\n")
        for data in self.__rainfall_tseries:
            line = "%d/%d/%d\t%d:00:00\t%f\n" % (data[0], data[1], data[2], data[3], data[4])
            fl.write(line)
        fl.close()


# excel_path = "C:\\Users\gr\Desktop\湖北澴水河水文模型项目\\雨量站\\1996年\\1996年应山站降水量摘录表.xls"
# txt_path = "C:\\Users\gr\Desktop"
# rs = process_rainstation_data(excel_path, txt_path)
# rs.complete_rainfall_timeseries()
# rs.write_rainfall_tseries()


''' 处理水文站数据（格式8，细节不同、逻辑同上） '''
class process_hydstation_data():
    big_month = [1, 3, 5, 7, 8, 10, 12]
    small_month = [4, 6, 9, 11]
    super_small_month = [2]
    year, stationCord, stationID = 2016, 0, ""

    def __init__(self, excel_path, txt_path):
        self.__excel_path = excel_path
        self.__txt_path = txt_path
        self.__data, self.__timeseries = [], []
        self.__read_excel()

    def __read_excel(self):
        app = xlw.App(visible=False)
        excel = app.books.open(self.__excel_path)
        sheet = excel.sheets["Sheet1"]

        ID = sheet.range("A1").value.split()
        self.stationID = ID[1]
        self.year = int(sheet.range("C2").value)
        self.stationCord = sheet.range("F2").value

        page1 = sheet.range("A5:D54").value
        page2 = sheet.range("E5:H54").value
        page3 = sheet.range("I5:L54").value
        page4 = sheet.range("M5:P54").value
        page5 = sheet.range("Q5:T54").value

        page6 = sheet.range("A59:D108").value
        page7 = sheet.range("E59:H108").value
        page8 = sheet.range("I59:L108").value
        page9 = sheet.range("M59:P108").value
        page10 = sheet.range("Q59:T108").value

        page11 = sheet.range("A113:D162").value
        page12 = sheet.range("E113:H162").value
        page13 = sheet.range("I113:L162").value
        page14 = sheet.range("M113:P162").value
        page15 = sheet.range("Q113:T162").value

        page16 = sheet.range("A167:D216").value
        page17 = sheet.range("E167:H216").value
        page18 = sheet.range("I167:L216").value
        page19 = sheet.range("M167:P216").value
        page20 = sheet.range("Q167:T216").value
        self.__data = [page1, page2, page3, page4,
                       page5, page6, page7, page8,
                       page9, page10, page11, page12,
                       page13, page14, page15, page16,
                       page17, page18, page19, page20,
                       ]

    def __get_rainfall_timeseries(self):
        month, day = 0, 0
        wl_last = ""
        pat = re.compile(r"\d+")  # 对于文本空格，匹配不到数字
        for page in self.__data:
            for line in page:
                if line[2] is None or pat.findall(str(line[2])) == []: continue
                if line[0] is not None and pat.findall(str(line[0])) != []: month = int(line[0])   # 更新月
                if line[1] is not None and pat.findall(str(line[1])) != []: day = int(line[1])  # 更新日
                arr2 = str(line[2]).split(':')
                hour = int(float(arr2[0]))
                if len(arr2) > 1:
                    minute = float(arr2[1])
                    hour = hour+1 if minute >= 30 else hour

                arr3 = str(line[3]).split('.')
                if len(arr3) > 1:   wl_last = arr3[0]  # 水位的特殊记录方式
                wl = float(wl_last + "." + arr3[0])
                #q = float(line[4])
                q = 0

                if len(self.__timeseries)>0 and hour == self.__timeseries[-1][3] \
                        and day == self.__timeseries[-1][2]: # 同一天同一时刻
                    self.__timeseries[-1][4] = (self.__timeseries[-1][4] + wl)/2
                    self.__timeseries[-1][5] = (self.__timeseries[-1][5] + q) / 2
                else:
                    self.__timeseries.append([self.year, month, day, hour, wl, q])

    def __complete_hour(self):
        new_hour_rain = []
        index, index_last, count = 0, 0, len(self.__timeseries)

        while index < count-1:
            day = self.__timeseries[index][2]
            if day != self.__timeseries[index+1][2]:  # 当前日的时间序列
                month = self.__timeseries[index][1]
                rain_slice = self.__timeseries[index_last:index+1]
                index_last = index + 1

                exist_hour = [rain[3] for rain in rain_slice]
                miss_hour = [hour for hour in range(1, 25) if hour not in exist_hour]
                for h in range(1, 25):  # 填充当日时间序列中缺失的时间
                    if h in miss_hour:
                        num = [exist_hour.index(hour) for hour in range(1, h) if hour in exist_hour]
                        if len(num) > 0:  # 向前查找
                            wl, q = rain_slice[num[-1]][-2], rain_slice[num[-1]][-1]
                            new_hour_rain.append([self.year, month, day, h, wl, q])
                        else:  # 向后查找
                            num = [exist_hour.index(hour) for hour in range(h+1, 25) if hour in exist_hour]
                            wl, q = rain_slice[num[0]][-2], rain_slice[num[0]][-1]
                            new_hour_rain.append([self.year, month, day, h, wl, q])
                        continue
                    num = exist_hour.index(h)
                    new_hour_rain.append(rain_slice[num])
            index += 1
        return new_hour_rain

    def __complete_day(self, new_hour_rain):
        new_day_rain = []
        index, index_last, count = 0, 0, len(new_hour_rain)

        while index < count:
            month = new_hour_rain[index][1]
            if index == count-1 and index - index_last > 0:
                rain_slice = new_hour_rain[index_last:]    # 不是new_hour_rain[index_last:index]
            elif month != new_hour_rain[index+1][1]:
                rain_slice = new_hour_rain[index_last:index+1]
                index_last = index+1
            else:
                index += 1
                continue

            exist_day = [rain[2] for rain in rain_slice]  # 当前月的日长度，每一天长度为24
            if month in self.big_month:   month_len = 31
            elif month in self.small_month:   month_len = 30
            else:  month_len = 28
            miss_day = [day for day in range(1, month_len + 1) if day not in exist_day]
            for day in range(1, month_len + 1):
                if day in miss_day:
                    num = [exist_day.index(day) for day in range(1, day) if day in exist_day]
                    if len(num) > 0:  # 向前查找
                        wl, q = rain_slice[num[-1] + 23][-2], rain_slice[num[-1] + 23][-1]
                    else:  # 向后查找
                        num = [exist_day.index(day) for day in range(day + 1, month_len + 1) if day in exist_day]
                        wl, q = rain_slice[num[0] + 23][-2], rain_slice[num[0] + 23][-1]
                    for h in range(1, 25):  # 补全每天时间
                        new_day_rain.append([self.year, month, day, h, wl, q])
                    continue
                num = exist_day.index(day)
                for h in range(24):
                    new_day_rain.append(rain_slice[num + h])
            index += 1
        return new_day_rain

    def complete_hyd_timeseries(self):
        self.__get_rainfall_timeseries()
        new_hour_rain = self.__complete_hour()
        new_day_rain = self.__complete_day(new_hour_rain)
        self.__timeseries = new_day_rain
        return new_day_rain

    def write_hyd_tseries(self):
        if os.path.isfile(self.__txt_path):
            outfile = self.__txt_path
        elif os.path.isdir(self.__txt_path):
            outfile = self.__txt_path + "\\" + self.stationID + "水文站.txt"
        else:
            outfile = self.stationID + "水文站.txt"
        fl = open(outfile, "w")
        fl.write("站点名称/站点站码：\t“" + self.stationID + "”:" + self.stationCord + ",\n\n")
        for data in self.__timeseries:
            line = "%d/%d/%d\t%d:00:00\t%f\t\n" % (data[0], data[1], data[2], data[3], data[4])
            fl.write(line)
        fl.close()


# excel_path = "C:\\Users\gr\Desktop\湖北澴水河水文模型项目\\水文站\\1996年\\1996年花园站洪水水文要素摘录表.xls"
# txt_path = "C:\\Users\gr\Desktop"
# rs = process_hydstation_data(excel_path, txt_path)
# rs.complete_hyd_timeseries()
# rs.write_hyd_tseries()


''' 将时间序列时间由1-24改为0-23 '''
def rewrite_station_file(folder_path):
    for pathes, folders, files in os.walk(folder_path):
        for file in files:
            fname, ftype = os.path.splitext(file)
            if ftype == ".txt":
                fl = open(folder_path + "\\" + file, "r")
                data = fl.readlines()
                fl.close()
                fl = open(folder_path + "\\" + file, "w")
                fl.write(data[0])
                fl.write("\n")
                for line in data[2:]:
                    arr = line.strip().split()
                    time_arr = arr[1].split(':')
                    new_line = "%s\t%d:00:00\t%s\t\n" % (arr[0], int(time_arr[0]) - 1, arr[2])
                    fl.write(new_line)
                fl.close()


# excel_path = "C:\\Users\gr\Desktop\湖北澴水河水文模型项目\澴河水文资料\雨量站_processed\\1996"
# rewrite_station_file(excel_path)




