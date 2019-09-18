import xlwings as xlw


''' ����ˮ�Ļ���վ��ԭʼ�������ݣ���ʽ1��'''
class Process_Basic_Station():
    sheet_name = "2006-2014��ˮ��ժ¼��"
    data_cols = {"station": "A", "year": "B", "m/d": "C", "start_hour": "D", "end_hour": "E", "rain": "F"}

    def __init__(self, data_file, out_folder):  # data_file��ˮ�����ݱ��out_folder���������ļ����ļ��С�
        self.data_file = data_file
        self.out_folder = out_folder
        self.app = xlw.App(visible=False, add_book=False)
        self.excel = self.app.books.open(self.data_file)
        self.sheet = self.excel.sheets[self.sheet_name]
        self.nrows = self.sheet.used_range.last_cell.row       # ��ȡ�����г���
        self.ncols = self.sheet.used_range.last_cell.column    # ��ȡ�����п��

    # ��ȡ����վ������ݳ��Ȳ����ء�
    def get_station_Len(self):
        col = self.data_cols["station"]
        data = self.sheet.range(col+"2:"+col+str(self.nrows)).value
        station_length = []

        index, index_start, id1, id2 = 0, 0, data[0], data[0]
        while index < len(data)-1:    # len(data)�ǳ��ȣ�len(data)-1�����һ������
            index += 1                # index���ֻ�ܵ������ڶ�������
            id2 = data[index]
            if id2 is None:  break    # Ĭ�Ͽո��ʾ�������ݽ���������indexʼ�������ӣ�������󳤶ȴ���
            if id1 != id2:
                station_length.append((str(id1), index-index_start))
                index_start = index
            id1 = id2
        station_length.append((str(id1), index-index_start+1))
        return station_length

    # ��ȡ����վ���ÿһ�ꡢÿһ�¡�ÿһ������ݳ��Ȳ����ء�
    def get_year_month_day_Len(self):
        station_len = self.get_station_Len()  # �������ں�������ȡÿ��վ������ݳ��ȡ�

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

    # ��ȡ���е���ʼʱ�����ֹʱ�����ݲ����أ�������վ�㣩��
    def get_start_end_time(self):
        start_col = self.data_cols["start_hour"]
        start_data = self.sheet.range(start_col+"2:"+start_col+str(self.nrows)).value
        end_col = self.data_cols["end_hour"]
        end_data = self.sheet.range(end_col+"2:"+end_col+str(self.nrows)).value
        start_time, end_time = [], []

        for start, end in zip(start_data, end_data):
            start_time.append(int(start/100))       # �����ְ�Сʱ��20:00, 20:30, 20:59����һ����
            end_time.append(int(end/100))
            # s_hour, e_hour = start/100.0, end/100.0
            # start_hour = int(s_hour)+1 if s_hour-int(s_hour) > 0.3 else int(s_hour)
            # end_hour = int(e_hour)+1 if e_hour-int(e_hour) > 0.3 else int(e_hour)
            # start_time.append(start_hour)
            # end_time.append(end_hour)
        return (start_time, end_time)

    # ��ȡ���еĽ������ݲ����أ�������վ�㣩��
    def get_rain(self):
        col = self.data_cols["rain"]
        rain = self.sheet.range(col+"2:"+col+str(self.nrows)).value
        return rain

    # ���������һ������ݲ����أ�1h�����һ��0-23h��������վ�㣩��
    def __reset_day_timeseries(self, start_time, end_time, rainfall):
        timeseries = [0.0 for y in range(24)]
        for start, end, value in zip(start_time, end_time, rainfall):
            time_len = end - start
            if end > 23: end = 23                    # ע��
            if time_len == 1 or time_len == 0:       # �п��ܳ���8��00~8��00
                timeseries[end] = value
                continue
            if time_len < 0:
                time_len = end + 24 - start          # ������һ�쿪ʼ�ļ���Сʱ�Ľ������޷���ȡ

            average_rain = round(value/time_len, 3)  # ����������3λС��
            count = 1
            while count <= time_len and (start+count) < 24:  # ����ע��
                timeseries[start + count] = average_rain
                count += 1
        return timeseries

    # ��װˮ�����ݴ������̣���վ��ֱ������ļ������
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
            fl = open(s_file, "w")            # ÿһ��վ��һ���ļ�

            year_range = 0
            for year in year_len[year_index:]:
                year_index += 1               # ��ݵ������Ѿ�ָ����һ��
                y_id, y_len = year[0], year[1]
                year_range += y_len
                if year_range > s_len:        # ˵����ǰ�Ѿ�������һ��վ��ĵ�һ�������
                    year_index -= 1           # year_index�Ѿ�ָ����һվ��ڶ����ˣ�����
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
                            while n < d_dis:         # ֻ��������������֮�������
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


# path = r"F:\������Ŀ\��Ŀ��ģ����\2 �����������ݴ���\0��ԭʼ����\1�����ˮԭʼ����04-16.xlsx"  # r"xxx",rǿ��תΪ�ַ�����
# result = r"C:\Users\gr\Desktop\fuhe_hydro"
# basic = Process_Basic_Station(path, result)
# basic.get_year_month_day_Len()
# basic.write_pcp_file()


''' ����ˮ�ķǻ���վ��ԭʼ�������ݣ���ʽ2��'''
class Process_nonBasic_Station():
    sheet_name = "2010-2016�ǻ����⽵ˮ��ժ¼��"
    data_cols = {"station": "A", "y/m/d_start": "B", "y/m/d_end": "C", "rainfall": "D"}
    __day_list, __month_list, __year_list, __station_list = [], [], [], []

    def __init__(self, data_file, out_folder):  # data_file��ˮ�����ݱ��out_folder���������ļ����ļ��С�
        self.data_file = data_file
        self.out_folder = out_folder
        self.app = xlw.App(visible=False, add_book=False)
        self.excel = self.app.books.open(self.data_file)
        self.sheet = self.excel.sheets[self.sheet_name]
        self.nrows = self.sheet.used_range.last_cell.row       # ��ȡ�����г���
        self.ncols = self.sheet.used_range.last_cell.column    # ��ȡ�����п��

    def __del__(self):  # ������������
        self.excel.close()
        self.app.quit()

    # ��ȡ����վ������ݳ��Ȳ����ء�
    def get_station_Len_List(self):
        col = self.data_cols["station"]
        data = self.sheet.range(col+"2:"+col+str(self.nrows)).value
        station_length, station_list = [], []
        index, index_start, id1, id2 = 0, 0, data[0], data[0]
        while index < len(data)-1:    # len(data)�ǳ��ȣ�len(data)-1�����һ������
            index += 1                # index���ֻ�ܵ������ڶ�������
            id2 = data[index]
            if id2 is None:  break    # Ĭ�Ͽո��ʾ�������ݽ���������indexʼ�������ӣ�������󳤶ȴ���
            if id1 != id2:
                station_length.append((str(id1), index-index_start))
                station_list.append(str(id1))
                index_start = index
            id1 = id2
        station_length.append((str(id1), index-index_start+1))
        station_list.append(id1)
        return (station_length, station_list)

    # ��ȡ����վ���ÿһ�ꡢÿһ�¡�ÿһ������ݳ��Ȳ����ء�
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
                if data1_date[0] != data2_date[0]:       # ����˶����ˣ�2015/6/5->2016/6/5, data1_arr[0][:-4]
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

                if data1_date[1] != data2_date[1]:       # �±����ձ��ˣ�6/5->1/5, data1_arr[0][:-2]
                    month_len.append((int(data1_date[1]), index-month_index_start))
                    month_list.append(int(data1_date[1]))
                    month_index_start = index
                    day_len.append((int(data1_date[2]), index-day_index_start))
                    day_list.append(int(data1_date[2]))
                    day_index_start = index
                    data1 = data2
                    continue

                if data1_date[2] != data2_date[2]:       # �ձ���, data1_arr[0] != data2_arr[0]
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
            index += 1            # �����1������һ��
        return (day_len, month_len, year_len, day_list, month_list, year_list)

    # ��ȡ���е���ʼʱ�����ݲ����أ�������վ�㣩��
    def get_start_time(self):
        col = self.data_cols["y/m/d_start"]
        data = self.sheet.range(col+"2:"+col+str(self.nrows)).value
        start_time = []
        for it in data:
            if it is None:  break      # Ĭ�Ͽո����������ݽ���
            data_arr = str(it).split()
            data_time_arr = data_arr[1].split(":")
            start_time.append(int(data_time_arr[0]))
        return start_time

    # ��ȡ���е���ֹʱ�����ݲ����أ�������վ�㣩��
    def get_end_time(self):
        col = self.data_cols["y/m/d_end"]
        data = self.sheet.range(col+"2:"+col+str(self.nrows)).value
        end_time = []
        for it in data:
            if it is None:  break      # Ĭ�Ͽո����������ݽ���
            data_arr = str(it).split()
            data_time_arr = data_arr[1].split(":")
            end_time.append(int(data_time_arr[0]))
        return end_time

    # ��ȡ���еĽ������ݲ����أ�������վ�㣩��
    def get_rainfall(self):
        col = self.data_cols["rainfall"]
        rainfall = self.sheet.range(col+"2:"+col+str(self.nrows)).value
        return rainfall

    # ���������һ������ݲ����أ�1h�����һ��0-23h��������վ�㣩��
    def __reset_day_timeseries(self, start_time, end_time, rainfall):
        timeseries = [0.0 for y in range(24)]
        for start, end, value in zip(start_time, end_time, rainfall):
            time_len = end - start
            if time_len == 1 or time_len == 0:       # �п��ܳ���8��00~8��00
                timeseries[end] = value
                continue
            if time_len < 0:
                time_len = end + 24 - start          # ������һ�쿪ʼ�ļ���Сʱ�Ľ������޷���ȡ

            average_rain = round(value/time_len, 3)  # ����������3λС��
            count = 1
            while count <= time_len and (start+count) < 24:  # ����ע��
                timeseries[start + count] = average_rain
                count += 1
        return timeseries

    # ��װˮ�����ݴ������̣���վ��ֱ������ļ������
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
                year_index += 1               # ��ݵ������Ѿ�ָ����һ��
                y_id, y_len = year[0], year[1]
                year_range += y_len
                if year_range > s_len:        # ˵����ǰ�Ѿ�������һ��վ��ĵ�һ�������
                    year_index -= 1           # year_index�Ѿ�ָ����һվ��ڶ����ˣ�����
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
                            while n < d_dis:         # ֻ��������������֮�������
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


# path = r"F:\������Ŀ(����)\��Ŀ��ģ����\\2 �����������ݴ���\0��ԭʼ����\1�����ˮԭʼ����04-16.xlsx"
# result = r"C:\Users\gr\Desktop\fuhe_hydro2"
# non_basic = Process_nonBasic_Station(path, result)
# non_basic.write_pcp_file()


'''����ˮ��վ�������ݣ���ʽ3��'''
class Process_DischargeData():
    path, sheet_id, station_cord = "", "", 0

    def __init__(self, path, sheet, station):  # path�����ݱ���ļ���sheet�Ǳ����Ŀ��ҳ�� station��ˮ��վ��վ�롣
        self.path, self.sheet_id, self.station_cord = path, sheet, station
        app = xlw.App(visible=False, add_book=False)
        excel = app.books.open(path)
        self.__sheet = excel.sheets[self.sheet_id]
        self.all_day_timeseries = []  # �洢����������У����������£���
        self.__data_year = []         # �洢��������С�
        self.__data_monthday = []     # �洢���յ������С�
        self.__data_hour = []         # �洢ʱ�������С�
        self.__data_q = []            # �洢���������С�

        # ��ȡҳ��������������ء�
    def get_sheet_nrows(self):
        return self.__sheet.used_range.last_cell.row

    # ��ȡ�ꡢ���ա�ʱ�����������в������г��ȡ�
    def extract_station_data(self):
        nrows = self.get_sheet_nrows()  # ��ȡ������
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

    # �����������ʱ���������ݡ�
    def reset_station_formatter(self, day_len):
        all_day_timeseries = self.__get_day_timeseries_q(day_len)
        all_day_timeseries = self.__insert_miss_day(all_day_timeseries)
        self.all_day_timeseries = all_day_timeseries

    # �������������ƥ�䵽���²�������ļ���
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
                    q_step = (q_day2 - q_day1)/(24.0*(day_dis-1))     # ʱ����
                    while n < day_dis:
                        date = str(year1)[:-2] + "/" + str(date1)[:-4] + "/" + str(day1+n) + "\t"
                        n += 1
                        for i in range(24):
                            fl.write(date+str(i)+":00:00\t"+str(q_day1+(n-1)*24*q_step+i*q_step) +"\n")

        date = str(year2)[:-2]+"/"+str(date2)[:-4]+"/"+str(date2)[-4:-2]+"\t"
        for i, value in zip(range(24), self.all_day_timeseries[index][1]):
            fl.write(date + str(i) + ":00:00\t" + str(value) + "\n")
        fl.close()

    # ��ȡ�ꡢ�¡��յ��б����ء�
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

    # ���Ѿ�����������ʱ�������в���ȱʧ��������ݡ�
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

    # ��ȡ���е�ÿһ���ԭʼ���ݳ��Ȳ����ء�
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

    # ��ȡ���е�ÿһ��������������в����ء�
    def __get_day_timeseries_q(self, day_len):
        all_day_timeseries, timeseries = [], []
        index, count = 0, len(day_len)

        while index < count:
            day = day_len[index]
            hours, q = self.__data_hour[day[1]:day[1]+day[2]], self.__data_q[day[1]:day[1]+day[2]]

            if index > 0:  q_before = ((all_day_timeseries[-1])[1])[-1]  # ǰ��������Ѿ�������
            else:          q_before = -1

            if index < count-1:
                day_after = day_len[index+1]    # �����������δ����
                hour_after, q_after = self.__data_hour[day_after[1]], self.__data_q[day_after[1]]
            else:
                hour_after, q_after = 0, -1

            timeseries = self.__reset_timeseries(hours, q, q_before, hour_after, q_after)

            all_day_timeseries.append((day[0], timeseries))
            index += 1
        return all_day_timeseries

    # �������ÿһ��������������С�
    def __reset_timeseries(self, day_hour, day_q, q_before, hour_after, q_after):
        timeseries = [0.0 for x in range(24)]
        existQ_hours = []
        for hour, q in zip(day_hour, day_q):
            h = int(hour / 100)
            timeseries[h] = q
            existQ_hours.append(h)           # ������timeseries.index(q)��q�����ظ�

        if q_before < 0 < existQ_hours[0]:  # q_before���Ϊ����˵��û��ǰһ������
            hour0 = existQ_hours[0]
            for i in range(hour0):
                timeseries[i] = timeseries[hour0]
        elif q_before > 0 and existQ_hours[0] > 0:
            hour0 = existQ_hours[0]
            q0 = timeseries[hour0]
            hour_dis = hour0 + 1         # Ĭ��ǰһ��������õ�ʱ�����У�����ǰ��һ����Чʱ��hour0
            q_step = round((q0 - q_before)/hour_dis, 3)
            for i in range(hour_dis-1):  # ֻ��Ҫ�����һ����Чʱ��֮ǰ��ʱ��
                timeseries[i] = q_before + q_step*i

        if q_after < 0 and existQ_hours[-1] < 23:   # q_after���Ϊ����˵��û����һ������
            hour1 = existQ_hours[-1]
            for i in range(hour1, 23):
                timeseries[i] = timeseries[hour1]
        elif q_after > 0 and existQ_hours[-1] < 23:
            hour1 = existQ_hours[-1]
            q1 = timeseries[hour1]
            hour_dis = hour_after + 24 - hour1
            q_step = round((q_after - q1)/hour_dis, 3)
            for i in range(hour1+1, 24):   # ֻ��Ҫ�������һ����Чʱ��hour1֮���ʱ��
                timeseries[i] = timeseries[i-1] + q_step

        index, count = 0, len(existQ_hours)
        while index < count-1:
            hour0, hour1 = existQ_hours[index], existQ_hours[index+1]
            hour_dis = hour1 - hour0     # ���ﲻ�ü�1
            if hour_dis <= 1:
                index += 1
                continue
            q0, q1 = timeseries[hour0], timeseries[hour1]
            q_step = round((q1 - q0)/hour_dis, 3)
            for i in range(hour0+1, hour1):
                timeseries[i] = timeseries[i-1] + q_step
            index += 1

        return timeseries


# path = r"F:\������Ŀ(����)\��Ŀ��ģ����\\2 �����������ݴ���\0��ԭʼ����\1�����ˮԭʼ����04-16.xlsx"
# result_file = "C:\\Users\\gr\\Desktop\\fuhe_hydro3"
# sheet_id = "2006-2014��ˮˮ��Ҫ��ժ¼��"
# station = 62405400                  # ��ǰ��������վ��վ��
# discharge = Process_DischargeData(path, sheet_id, station)
# print("here1")
# discharge.extract_station_data()
# print("here2")
# #discharge.reset_station_formatter()
# print("here3")
# discharge.write_qobs_file(result_file)
# print("here4")


''' ��������ȡˮ����������ݱ�񣨸�ʽ4�� '''
def read_reservoir_data(data_folder, result_folder, station):
    prefix = "�η�����վ��ˮҪ��ժ¼ͳ�Ʊ�"
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
            month, day = int(date_arr[1]), int(date_arr[2])   # ��2010/01/02��Ϊ2010/1/2
            result.write(date_arr[0]+"/"+str(month)+"/"+str(day)+"\t")
            result.write(d_arr[1]+":00:00\t"+str(q)+"\n")
    result.close()


# data_folder = "F:\������Ŀ(����)\��Ŀ��ģ����\ˮ���������\�η�"
# result_folder = "C:\\Users\\gr\\Desktop\\fuhe_hydro3"
# station = 62430700
# read_reservoir_data(data_folder, result_folder, station)


''' ��������ȡˮ����������ݱ�񣨸�ʽ5�� '''
def read_reservoir_data_2(data_path, result_folder, station):
    result = open(result_folder+"\\qobs_"+str(station)+".txt", "w")

    app = xlw.App(visible=False, add_book=False)
    excel = app.books.open(data_path)
    sheet = excel.sheets["Sheet1"]
    nrows = sheet.used_range.last_cell.row

    date = sheet.range("B2:B"+str(nrows)).value         # date��ȡ����datetime��ʽ���޷�split����
    discharge = sheet.range("D2:D"+str(nrows)).value
    count = nrows - 1

    year, month, day, hour = date[0].year, date[0].month, date[0].day, date[0].hour  # datetime�������͡�
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


# data_path = "F:\������Ŀ(����)\��Ŀ��ģ����\ˮ���������\����\ʱ������\����\����.xlsx"
# result_folder = "C:\\Users\\gr\\Desktop\\fuhe_hydro3"
# station = 61800012
# read_reservoir_data_2(data_path, result_folder, station)


''' ������վ���Ѵ����ļ���ȡ��ˮ���� ����ʽ6��'''
def add_year_filtrate(year_range):            # ���в�����װ����,�޶�ѡ���������䣬���ټ�������
    def handler(class_method):                # װ������װ
        def wrapper(self, *args, **kwargs):   # װ����
            class_method(self, *args, **kwargs)
            self.get_year_range(year_range[0], year_range[1])
            self.year_filtrate()              # ���ܵ������˽�з���self.__method()
        return wrapper                        # ����װ������ ��ʵ�ֽ������ԭ����class_method�������ı�ԭ�����Ľӿ�
    return handler                            # ���ط�װ���

class Select_Flood_Events():
    def __init__(self, flood_folder, station):  # flood_folder�ǰ�������վ�����ļ����ļ��У�����Ĭ�������������ļ���
        self.folder, self.station = flood_folder, station
        self.start_y, self.end_y = 0, 0    # ��ˮ���ε�������䡣
        self.q, self.hour, self.date = [], [], []  # �ֱ�洢������Сʱ���������ݡ�
        self.read_q_file(flood_folder, station)
        self.floods = []   # �洢��ˮ�������ݡ�

    #  ��ȡ���ڡ�Сʱ���������ݡ�
    def read_q_file(self, folder, station):
        fl = open(folder+"\\qobs_"+str(station)+".txt", "r")
        for line in fl.readlines():
            line_arr = line.strip().split()
            self.date.append(line_arr[0])
            self.hour.append(line_arr[1])
            self.q.append(float(line_arr[2]))
        fl.close()

    # ��ȡ���������е�ƽ������ֵ���������ֵ�����أ���ѡȡ���βο���
    def get_averageAndmax_q(self):
        average_q, max_q = 0, 0
        count = len(self.q)
        for q in self.q:
            average_q += q
            max_q = max(max_q, q)
        average_q /= count
        return (average_q, max_q)

    # �ڸ�������д�����ѡ��������ֵҪ����������Ρ�
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

    # �����ˮ���ε�������䡣
    def get_year_range(self, start_year, end_year):
        self.start_y = start_year
        self.end_y = end_year

    # ���������������䣬ɸѡ��ѡ���������Σ���ʵ����װ����֮���Ѿ����Ҫ����
    def year_filtrate(self):
        index_keep = []
        for i, date in zip(range(len(self.floods)), self.floods):
            start_date, end_date = self.date[date[0]].split('/'), self.date[date[1]].split('/')
            start_year, end_year = int(start_date[0]), int(end_date[0])
            if start_year >= self.start_y and end_year <= self.end_y:
                index_keep.append(i)

        floods_keep = [self.floods[i] for i in index_keep]
        self.floods = floods_keep

    # �����ˮ���ν��������Ĭ����������
    def write_floods_file(self, result_folder=""):
        if result_folder == "": result_folder = self.folder
        fl = open(result_folder+"\\floods_"+str(self.station)+".txt", "w")
        print("��ˮ����������%d" % len(self.floods))
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
# # extend_hours �������ʹ��flood_range�Ŀ�ʼ���ڲ����ظ���������������쳣��
# # extend_hours Ӧ�����ǵ�����վ�ĵ�ǰ��������������Զ������վ֮ǰ�Ľ��꣬���ǵ�ǰ�Ľ��꣨Ӧ����������
# extend_hours = 72
# extend_days = 3
# flood.select_flood_date_range(flood_level, extend_hours, extend_days)
# flood.write_floods_file()


''' ������վѡ��������������ɸѡ����վ�ǻ���վ�������ݵĺ�ˮ���� '''
class Select_Shared_Floods():
    def __init__(self, folder, station):  # folder�ǰ�������վ�������ε��ļ��У�station������վվ�롣
        self.folder, self.station = folder, station
        self.floods_range = []  # �洢����վ�ĺ�ˮ���Ρ�
        self.read_flood_file()
        self.shared_flood = {}  # �洢����վ�㹲��ĺ�ˮ���Ρ�
        self.shared_flood_len = []  # �洢����վ�㹲��ĺ�ˮ���εĳ��ȣ���ǰӦ��һ���ģ���
        self.weighted_pcp = []  # �洢����վ��ļ�Ȩϵ����

    # ��ȡ����վ�ĸ������Ρ�
    def read_flood_file(self):
        fl = open(self.folder + "\\floods_" + str(self.station) + ".txt", "r")
        data = fl.readlines()
        fl.close()
        index, count = 2, len(data)   # ���ݴӵ����п�ʼ
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

    # ���ݸ�����������䣬ɸѡ����վ�ĺ�ˮ���Ρ�
    def select_years_range(self, start_year, end_year):
        if start_year > end_year: exit("start_year equal to end_year")
        selected_flood_range = []
        for flood in self.floods_range:
            flood_start_date = flood[0][0].split("/")
            flood_end_date = flood[1][0].split("/")

            if int(flood_start_date[0]) < start_year:
                continue  # ���ﲻ���ǵ������η�Χ�����Ƚ϶̣���ֻ���ʵ��꣩
            if int(flood_end_date[0]) > end_year:
                continue
            selected_flood_range.append(flood)
        self.floods_range = selected_flood_range

    # �ֱ��ȡÿ��վ���а���������վ��ˮ���Ρ�
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
                    # else:         # ���ʱ��������һ�����ڣ��жϵ�ǰ�·��п��ܴ��ڵ�ʱ������Ƭ�Σ�������������ƥ�䣩
                    #     year, month, day = arr[0].split("/")         # split()�����б��������ƺ�ʹ��û���쳣
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
                    if (end_index - start_index) != flood_len:    # Ϊ��������֮������©
                        index += 1
                        continue

                    current_floods.append(data[start_index:end_index])
                    index += 1
            self.shared_flood[cord] = current_floods

    # �Ӹ���վ������ĺ�ˮ������ɸѡ��ͬӵ�еĺ�ˮ���Ρ�
    def select_shared_flood(self):
        start_date_q = [start[0][0] for start in self.floods_range]

        for keys, datas in self.shared_flood.items():
            start_date_current = [(date[0].strip().split())[0] for date in datas]
            shared_range = [start for start in start_date_q if start in start_date_current]
            start_date_q = shared_range
        self.floods_range = [flood for flood in self.floods_range if flood[0][0] in start_date_q]
        print("��ˮ����������%d" % len(self.floods_range))

        for key, data in self.shared_flood.items():
            start_date_current = [(date[0].strip().split())[0] for date in data]
            shared_range_index = [start_date_current.index(start) for start in start_date_current if start in start_date_q]
            self.shared_flood[key] = [data[index] for index in shared_range_index]

        key = list(self.shared_flood.keys())[0]
        self.shared_flood_len = [len(data) for data in self.shared_flood[key]]

    # ���ļ��л�ȡ����վ�㹲��ĺ�ˮ���Σ��Ѿ�ɸѡ���ĺ�ˮ���Σ�����Ȩ�غ����¼���ʹ�á���
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

    # ������ĺ�ˮ���������
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

    # ������ĺ�ˮ����������汾2����
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

    # �����Ȩ��������곡�Ρ�
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

    # ��Ȩ���������ڸ�վ��Ľ��곡�Ρ�
    def generate_weighted_pcp(self, weight, area_id, result_file):
        fixed_order = list(self.shared_flood.keys())

        index, floods_count = 0, len(self.floods_range)
        while index < floods_count:
            current_flood_len = self.shared_flood_len[index]
            current_weighted_flood = [0.0 for x in range(current_flood_len)]
            for key in fixed_order:
                weight_coe = weight[key]
                flood = self.shared_flood[key][index]        # ��վ��ͬһ���κ�ˮ
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

# ��ȡ��������վ���ֵ�
def get_station_id_area_coe(excel_path, area_name, out_path):
    app = xlw.App(visible=False, add_book=False)
    excel = app.books.open(excel_path)
    sheet = excel.sheets[area_name]

    station_name = sheet.range("B2:B39").value  # ������Ҫ�ֶ����롣
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


# excel_path = "F:\������Ŀ(����)\����ģ��\�����°���ˮ��ģ��\����������վ.xlsx"
# out_path = "C:\\Users\\gr\\Desktop"
# area_name = "����"
# get_station_id_area_coe(excel_path, area_name, out_path)  # �������վ�ֵ�����ֶ����Ƶ������station_id.

# folder = "C:\\Users\\gr\\Desktop\\fuhe_hydro3"
# station = 61800012
# floods = Select_Shared_Floods(folder, station)
# print("here1")
# # file_path = "F:\������Ŀ(����)\����ģ��\�����°���ˮ��ģ��\\9��Ҷ����ζ�\��Ҷ�ǰ��_Ԥ�Ȱ�\\��Ҷ�ǰ��_flood_events.txt"
# # floods.get_shared_flood_range_fromFile(file_path)
# start_year, end_year = 2010, 2018
# floods.select_years_range(start_year, end_year)
# print("here2")
# nonbasicStation_id = {  "�Ӷ�":62427160,  "�质":62403200,  "䭿�":62427500,
#                       "��Ϫ":62426950,  "������":62426620,  "����":62427120,  "���":62426740,
#                       "����":62425750,  "������":62427360,  "��Դ":62427320,  "����":62427520,
#                       "��Դ":62427540,  "����":62425820,  "ʯ��":62425840}#
# folder_nonbasicStation = "C:\\Users\\gr\\Desktop\\fuhe_hydro2"
# floods.find__flood_events(folder_nonbasicStation, list(nonbasicStation_id.values()))
# print("here3")
# basicStation_id = {"ʯ�":62428500}#
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


''' ����ѡȡ��ˮ���κͼ�Ȩ����Ľ��곡�������°���ģ���ļ� '''
class Write_Standard_XAJ():
    def __init__(self, data_folder, out_folder, cord):  # �����ˮ���κͽ��곡���ļ��У�����ļ��У�����վվ�롣
        self.data_folder = data_folder
        self.cord = cord
        self.out_folder = out_folder
        self.events = []  # �洢������ˮ������ֹʱ�䡣
        self.__read_events()  # �洢��ȡ�ĺ�ˮ���Ρ�
        self.__creat_event_folder()

    # ��ȡ������ˮ����ֹʱ�䡣
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

    # �����°���ģ�͸����ε��ļ��С�
    def __creat_event_folder(self):
        for event in self.events:
            event_date = event[0][0]
            event_date = re.sub("/", "-", event_date)      # ������ʽ�滻�ַ�
            folder = self.out_folder + "\\" + event_date
            if not os.path.exists(folder):
                os.mkdir(self.out_folder + "\\" + event_date)

    # ����qobs�����ļ���
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

    # ����inflow�����ļ���
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

    # ����pcp�����ļ���
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

    # ����task�����ļ���
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

    # ����floods����ѡ���ļ���
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


# ɾ���������е�result.txt�ļ���
def del_resultFile_inFolder(folder_path):
    for pathes, folders, files in os.walk(folder_path):
        for folder in folders:
            child_folder_path = folder_path+"\\"+folder
            for child_pathes, child_folders, child_files in os.walk(child_folder_path):
                for file in child_files:
                    fname, ftype = os.path.splitext(file)
                    if fname == "result":
                        os.remove(child_folder_path+"\\"+file)


# folder_path = "F:\������Ŀ(����)\����ģ��\�����°���ˮ��ģ��\9��Ҷ����ζ�\��Ҷ�ǰ��_Ԥ�Ȱ�\��Ҷ�ǰ��"
# del_resultFile_inFolder(folder_path)


''' ��������վ���ݣ���ʽ7�� '''
class process_rainstation_data():
    big_month = [1, 3, 5, 7, 8, 10, 12]  # ����31�졣
    small_month = [4, 6, 9, 11]  # С��30�졣
    super_small_month = [2]  # 2��28�졣
    year, stationCord, stationID = 2016, 0, ""

    def __init__(self, excel_path, txt_path):
        self.__excel_path = excel_path
        self.__txt_path = txt_path
        self.__rainfall_tseries = []  # �洢����ʱ���������ݡ�
        self.__data = []  # �洢����и�ҳ�и������ݡ�
        self.__read_excel()

    # ��ȡ����и�ҳ�и�������ݡ�
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

        page1 = sheet.range("A5:E39").value  # ���ڻ���Ҫ�ֶ����á�
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

    # ��ȡ���еĽ���ʱ�����У�������ʱƥ�䣩��
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

    # ����ÿ���ʱ�����У�1~24h����
    def __complete_hour(self):
        new_hour_rain = []
        index, index_last, count = 0, 0, len(self.__rainfall_tseries)
        while index < count-1:
            day = self.__rainfall_tseries[index][2]
            if day != self.__rainfall_tseries[index+1][2]:  # ��ǰ�յ�ʱ������
                month = self.__rainfall_tseries[index][1]
                rain_slice = self.__rainfall_tseries[index_last:index+1]
                index_last = index + 1
                new_rain = []

                start_h = rain_slice[0][3]
                num = 1
                while num <= start_h:  # ��ǰ�յĿ�ʼʱ��
                    new_rain.append([self.year, month, day, num, 0])
                    num += 1

                for slices in rain_slice:  # �м�ʱ��
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
                if start_h < end_h < 24:  # ĩβʱ��
                    dis, step = 24 - end_h, 0
                    num = 1
                    while num <= dis:
                        new_rain.append([self.year, month, day, end_h + num, step])
                        num += 1
                # if end_h < start_h and day == (self.__rainfall_tseries[index + 1][2] - 1): # ĩβʱ��
                #     dis, step = end_h, rain / (end_h+24-start_h)
                #     num = 1
                #     while num <= dis:
                #         new_rain.append([self.year, month, day + 1, num, step])
                #         num += 1

                exist_hour = [rain[3] for rain in new_rain]
                miss_hour = [hour for hour in range(1, 25) if hour not in exist_hour]
                for h in range(1, 25):  # ��䵱��ʱ��������ȱʧ��ʱ��
                    if h in miss_hour:
                        new_hour_rain.append([self.year, month, day, h, 0])
                        continue
                    num = exist_hour.index(h)
                    new_hour_rain.append(new_rain[num])
            index += 1
        return new_hour_rain

    # ����ÿ�µ������У��ִ�С�£�2��28�죩��
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

            exist_day = [rain[2] for rain in rain_slice]  # ÿһ�쳤��Ϊ24
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
            #     exist_day = [rain[2] for rain in rain_slice] # ÿһ�쳤��Ϊ24
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

    # �����ʱ�����е�ȱʧ�������µ�ʱ�����С�
    def complete_rainfall_timeseries(self):
        self.__get_rainfall_timeseries()
        new_hour_rain = self.__complete_hour()
        new_day_rain = self.__complete_day(new_hour_rain)
        self.__rainfall_tseries = new_day_rain
        return new_day_rain

    # ���ʱ�����С�
    def write_rainfall_tseries(self):
        if os.path.isfile(self.__txt_path):
            outfile = self.__txt_path
        elif os.path.isdir(self.__txt_path):
            outfile = self.__txt_path + "\\" + self.stationID + "����վ.txt"
        else:
            outfile = self.stationID + "����վ.txt"
        fl = open(outfile, "w")
        fl.write("վ������/վ��վ�룺\t��" + self.stationID + "��:" + self.stationCord + ",\n\n")
        for data in self.__rainfall_tseries:
            line = "%d/%d/%d\t%d:00:00\t%f\n" % (data[0], data[1], data[2], data[3], data[4])
            fl.write(line)
        fl.close()


# excel_path = "C:\\Users\gr\Desktop\������ˮ��ˮ��ģ����Ŀ\\����վ\\1996��\\1996��Ӧɽվ��ˮ��ժ¼��.xls"
# txt_path = "C:\\Users\gr\Desktop"
# rs = process_rainstation_data(excel_path, txt_path)
# rs.complete_rainfall_timeseries()
# rs.write_rainfall_tseries()


''' ����ˮ��վ���ݣ���ʽ8��ϸ�ڲ�ͬ���߼�ͬ�ϣ� '''
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
        pat = re.compile(r"\d+")  # �����ı��ո�ƥ�䲻������
        for page in self.__data:
            for line in page:
                if line[2] is None or pat.findall(str(line[2])) == []: continue
                if line[0] is not None and pat.findall(str(line[0])) != []: month = int(line[0])   # ������
                if line[1] is not None and pat.findall(str(line[1])) != []: day = int(line[1])  # ������
                arr2 = str(line[2]).split(':')
                hour = int(float(arr2[0]))
                if len(arr2) > 1:
                    minute = float(arr2[1])
                    hour = hour+1 if minute >= 30 else hour

                arr3 = str(line[3]).split('.')
                if len(arr3) > 1:   wl_last = arr3[0]  # ˮλ�������¼��ʽ
                wl = float(wl_last + "." + arr3[0])
                #q = float(line[4])
                q = 0

                if len(self.__timeseries)>0 and hour == self.__timeseries[-1][3] \
                        and day == self.__timeseries[-1][2]: # ͬһ��ͬһʱ��
                    self.__timeseries[-1][4] = (self.__timeseries[-1][4] + wl)/2
                    self.__timeseries[-1][5] = (self.__timeseries[-1][5] + q) / 2
                else:
                    self.__timeseries.append([self.year, month, day, hour, wl, q])

    def __complete_hour(self):
        new_hour_rain = []
        index, index_last, count = 0, 0, len(self.__timeseries)

        while index < count-1:
            day = self.__timeseries[index][2]
            if day != self.__timeseries[index+1][2]:  # ��ǰ�յ�ʱ������
                month = self.__timeseries[index][1]
                rain_slice = self.__timeseries[index_last:index+1]
                index_last = index + 1

                exist_hour = [rain[3] for rain in rain_slice]
                miss_hour = [hour for hour in range(1, 25) if hour not in exist_hour]
                for h in range(1, 25):  # ��䵱��ʱ��������ȱʧ��ʱ��
                    if h in miss_hour:
                        num = [exist_hour.index(hour) for hour in range(1, h) if hour in exist_hour]
                        if len(num) > 0:  # ��ǰ����
                            wl, q = rain_slice[num[-1]][-2], rain_slice[num[-1]][-1]
                            new_hour_rain.append([self.year, month, day, h, wl, q])
                        else:  # ������
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
                rain_slice = new_hour_rain[index_last:]    # ����new_hour_rain[index_last:index]
            elif month != new_hour_rain[index+1][1]:
                rain_slice = new_hour_rain[index_last:index+1]
                index_last = index+1
            else:
                index += 1
                continue

            exist_day = [rain[2] for rain in rain_slice]  # ��ǰ�µ��ճ��ȣ�ÿһ�쳤��Ϊ24
            if month in self.big_month:   month_len = 31
            elif month in self.small_month:   month_len = 30
            else:  month_len = 28
            miss_day = [day for day in range(1, month_len + 1) if day not in exist_day]
            for day in range(1, month_len + 1):
                if day in miss_day:
                    num = [exist_day.index(day) for day in range(1, day) if day in exist_day]
                    if len(num) > 0:  # ��ǰ����
                        wl, q = rain_slice[num[-1] + 23][-2], rain_slice[num[-1] + 23][-1]
                    else:  # ������
                        num = [exist_day.index(day) for day in range(day + 1, month_len + 1) if day in exist_day]
                        wl, q = rain_slice[num[0] + 23][-2], rain_slice[num[0] + 23][-1]
                    for h in range(1, 25):  # ��ȫÿ��ʱ��
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
            outfile = self.__txt_path + "\\" + self.stationID + "ˮ��վ.txt"
        else:
            outfile = self.stationID + "ˮ��վ.txt"
        fl = open(outfile, "w")
        fl.write("վ������/վ��վ�룺\t��" + self.stationID + "��:" + self.stationCord + ",\n\n")
        for data in self.__timeseries:
            line = "%d/%d/%d\t%d:00:00\t%f\t\n" % (data[0], data[1], data[2], data[3], data[4])
            fl.write(line)
        fl.close()


# excel_path = "C:\\Users\gr\Desktop\������ˮ��ˮ��ģ����Ŀ\\ˮ��վ\\1996��\\1996�껨԰վ��ˮˮ��Ҫ��ժ¼��.xls"
# txt_path = "C:\\Users\gr\Desktop"
# rs = process_hydstation_data(excel_path, txt_path)
# rs.complete_hyd_timeseries()
# rs.write_hyd_tseries()


''' ��ʱ������ʱ����1-24��Ϊ0-23 '''
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


# excel_path = "C:\\Users\gr\Desktop\������ˮ��ˮ��ģ����Ŀ\�׺�ˮ������\����վ_processed\\1996"
# rewrite_station_file(excel_path)




