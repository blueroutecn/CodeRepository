import unittest
from run_routines import *


# 测试贵仁模型前处理工具集。
class TestPreProcessing(unittest.TestCase):
    def setUp(self):
        self.__in1d = r"E:\代码仓库\贵仁模型小工具(py)\案例\Projects\AllProject\IN1D"
        self.__in2d = r"E:\代码仓库\贵仁模型小工具(py)\案例\Projects\AllProject\IN2D"
        self.__outFolder = r"E:\代码仓库\贵仁模型小工具(py)\案例\result"

    # 测试HD数据转三维散点数据的功能。
    # （一个测试对应一个实例的一个功能，方便定位问题所在。）
    def test_get_HD_data(self):
        demo = pre_Processing(self.__in1d, self.__in2d, self.__outFolder)
        ID, HD = demo.get_HD_data()
        self.assertEqual(len(ID), len(HD))

    def test_out_HD_file(self):
        demo = pre_Processing(self.__in1d, self.__in2d, self.__outFolder)
        ID, HD = demo.get_HD_data()
        ID.append("blank")
        with self.assertRaises(ValueError):
            demo.out_HD_file(ID, HD)



if __name__ == "__main___":
    unittest.main()








