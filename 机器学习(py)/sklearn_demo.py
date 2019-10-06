# -*- coding=utf-8 -*-

import numpy as np
import logging as log


class Logger(object):
    def __init__(self, logger_name='', level=log.INFO):
        self.__logger = log.getLogger(logger_name)
        self.__logger.setLevel(level)

    @staticmethod
    def get_file_handler(file="logging.log", mode="a+", coding="utf-8", level=log.INFO):
        file_handler = log.FileHandler(file, mode, coding)
        file_handler.setLevel(level)
        return file_handler

    @staticmethod
    def get_stream_handler(level=log.WARN):
        stream_handler = log.StreamHandler()
        stream_handler.setLevel(level)
        return stream_handler

    @staticmethod
    def get_formatter(fmt="%(asctime)s :%(message)s", datefmt="%Y-%m-%d, %H-%M-%S"):
        formatter = log.Formatter(fmt=fmt, datefmt=datefmt)
        return formatter

    def get_default_logger(self):
        file_handler = Logger.get_file_handler()
        stream_handler = Logger.get_stream_handler()
        formatter = Logger.get_formatter()
        file_handler.setFormatter(formatter)
        stream_handler.setFormatter(formatter)
        self.__logger.addHandler(file_handler)
        self.__logger.addHandler(stream_handler)
        return self.__logger

    def get_logger_without_handler(self):
        return self.__logger


'''
监督学习也被称为归纳式学习，是机器学习的种类中最成熟也是使用最多的一种学习方式。
监督学习利用一组进行了标记的训练数据，来对陌生数据进行预测.

非监督学习则是根据无标记的数据，从数据内部寻找其隐藏的特征和结构。
训练数据中不包含任何的所需要的数据（监督学习中的标记）。 

半监督学习包含了上述监督学习和非监督学习的混合体。 

按照机器学习的性质来划分，机器学习可以划分为三类： 
 分类，用于进行类型预测； 
 回归，用于进行数值预测； 
 聚类，用于划分群体； 

分类问题属于监督学习，主要是用于解决数据种类的划分问题，依据已有的标记号分类的数据进行训练，对陌生数据进行种类识别，
比如识别广告邮件、垃圾短信，判断销量的高低等。
=》 主要的算法有回归分类、SVM 分类、贝叶斯分类，决策树等。

回归问题也属于监督学习，依据标记了数值的数据进行训练，对默认的数据进行数值预测，
例如房价预测、股价预测等。
=》 主要的算法有线性回归、逻辑回归、随机森林回归等。

聚类问题则属于非监督问题，聚类算法从大量数据之中，找到数据之间的共性和特性，对数据进行区分，
例如客户群体细分、实验分组。
=》 主要算法有 K 均值算法等。
'''


''' 数据预处理 '''
from sklearn.preprocessing import StandardScaler, Normalizer, Binarizer, LabelEncoder


class DataPreProcessing(object):
    def __init__(self, rows, cols):
        if rows*cols <= 0:
            raise ValueError("cols(%d) or rows(%d) wrong." % (cols, rows))
        self.__x = np.random.random((rows, cols))

    def standard_scaler(self):  # 数据标准化。
        scaler = StandardScaler().fit(self.__x)
        standard_x = scaler.transform(self.__x)
        return standard_x

    def normalize_scaler(self):  # 数据归一化。
        scaler = Normalizer().fit(self.__x)
        nor_x = scaler.transform(self.__x)
        return nor_x

    def binary_scaler(self):  # 数据二值化。
        scaler = Binarizer().fit(self.__x)
        bin_x = scaler.transform(self.__x)
        return bin_x

    def encod_label(self, label):  # 文本标签编码。
        encode = LabelEncoder().fit(label)
        en_label = encode.transform(encode)
        return en_label

    def show_processing_result(self):
        standard_x = self.standard_scaler()
        nor_x = self.normalize_scaler()
        bin_x = self.binary_scaler()
        print("standard: ", standard_x, end="  ; ")
        print("normalize: ", nor_x, end="  : ")
        print("binary: ", bin_x, end="\n")
        return standard_x, nor_x, bin_x


''' 分割训练数据集和测试数据集 '''
from sklearn.model_selection import train_test_split


class SplitTrainTest(object):
    def get_data(self, ranges, rows, cols):
        x = np.arange(ranges).reshape((rows, cols))
        y = list(range(rows))
        return x, y

    def split_train_test(self, data_x, data_y):  # 分割训练集和测试集。
        x_train, x_test, y_train, y_test = train_test_split(data_x, data_y)
        return x_train, x_test, y_train, y_test


''' 创建模型 '''
from sklearn.linear_model import LinearRegression
from sklearn.svm import SVC, SVR
from sklearn.tree import DecisionTreeClassifier, DecisionTreeRegressor
from sklearn.ensemble import RandomForestClassifier, RandomForestRegressor
from sklearn.naive_bayes import BernoulliNB, GaussianNB, MultinomialNB
from sklearn.cluster import KMeans


# 监督学习模型。
class SupervisedLearnEstimator(object):

    def get_linear_model(self):
        '''
        一种通过数理回归分析，来确定两个或两个以上的变量之间的关系。
        在机器学习中可 用于回归预测分析。
        :return: 线性回归模型。
        '''
        lr = LinearRegression()
        return lr

    def get_svc_model(self):
        '''
        在面对训练数据不足，数据维度过高等情况下有很好的效果，
        可以在机器学 习分类和回归分析中进行分析数据、识别模式等。
        :return: 支持向量机svc。
        '''
        svc = SVC()
        return svc

    def get_svr_model(self):
        '''
        在面对训练数据不足，数据维度过高等情况下有很好的效果，
        可以在机器学 习分类和回归分析中进行分析数据、识别模式等。
        :return: 支持向量机svr。
        '''
        svr = SVR()
        return svr

    def get_decisiontree_classify_model(self):
        '''
        根据数据特征的属性，使用树形结构创建决策模型，最终生成一颗决策树。
        解决分类问题的决策树算法为 DecisionTreeClassifier.
        :return: 决策树tree_c.
        '''
        tree_c = DecisionTreeClassifier()
        return tree_c

    def get_decisiontree_regress_model(self):
        '''
        根据数据特征的属性，使用树形结构创建决策模型，最终生成一颗决策树。
        解决回归问题的决策树算法为 DecisionTreeRegressor.
        :return: 决策树tree_r.
        '''
        tree_r = DecisionTreeRegressor()
        return tree_r

    def get_randomforest_classify_model(self):
        '''
        随机森林算法与决策树比较类似，但是是利用生成多棵树来对数据进行训练。
        解决分类问题的随机森林算法为RandomForestClassifier.
        :return: 随机森林rf_c.
        '''
        rf_c = RandomForestClassifier()
        return rf_c

    def get_randomforest_regress_model(self):
        '''
        随机森林算法与决策树比较类似，但是是利用生成多棵树来对数据进行训练。
        解决回归问题的随机森林算法为RandomForestRegressor.
        :return: 随机森林rf_r.
        '''
        rf_r = RandomForestRegressor()
        return rf_r

    def get_naviebayes_bernoulli_model(self):  # 多元伯努利贝叶斯算法。
        '''
        朴素贝叶斯算法基于贝叶斯数学原理和特征数据独立假设，广泛的用于解决各种分类问题，并且拥有较高的准确率。
        :return: 贝叶斯算法nb_b.
        '''
        nb_b = BernoulliNB()
        return nb_b

    def get_naviebayes_gaussian_model(self):  # 高斯贝叶斯算法。
        '''
        朴素贝叶斯算法基于贝叶斯数学原理和特征数据独立假设，广泛的用于解决各种分类问题，并且拥有较高的准确率。
        :return: 贝叶斯算法nb_g.
        '''
        nb_g = GaussianNB()
        return nb_g

    def get_naviebayes_multinomal_model(self):  # 多项式贝叶斯算法。
        '''
        朴素贝叶斯算法基于贝叶斯数学原理和特征数据独立假设，广泛的用于解决各种分类问题，并且拥有较高的准确率。
        :return: 贝叶斯算法nb_m.
        '''
        nb_m = MultinomialNB()
        return nb_m


# 非监督学习模型。
class UnSupervisedLearnEstimator(object):

    def get_kmeans_model(self, n_cluster=8):
        '''
        K均值算法算是通过在多组相等的方差中分离样本来对数据进行聚类。
        :param n_cluster: 聚类数量（default=8）.
        :return: K均值算法kmean.
        '''
        kmean = KMeans(n_cluster)
        return kmean


''' 训练模型 '''


# 训练监督学习模型。
class TrainSupervisedLearnEstimator(SplitTrainTest, SupervisedLearnEstimator):  # 无参多重继承。
    '''
    多重继承中的父类优先级

    # 注意下面两类D、E，都是继承A、B、C，且A类的优先级最高。但是三条__init__语句的顺序是相反的
    class D(A, B, C):
        def __init__(self, x=0, y=0, z=0):
            C.__init__(self, z) # init C
            B.__init__(self, y) # init B
            A.__init__(self, x) # init A （A最优先）

    class E(A, B, C):
        def __init__(self, x=0, y=0, z=0):
            super(E, self).__init__(x)   # init A （A最优先）  # 此句可简写成：super().__init__(x)
            super(A, self).__init__(y)   # init B
            super(B, self).__init__(z)   # init C
    '''
    '''
    继承中的_ _init_ _
    当在Python中出现继承的情况时，注意初始化函数_init_的行为:
    ********************************************************************************************

    如果子类没有定义自己的初始化函数，父类的初始化函数会被默认调用；
    但是如果要实例化子类的对象，则只能传入父类的初始化函对应的参数，否则会出错。

    如果子类定义了自己的初始化函数，而在子类中没有显示调用父类的初始化函数，则父类的属性不会被初始化。

    如果子类定义了自己的初始化函数，在子类中显示调用父类，子类和父类的属性都会被初始化。

    ********************************************************************************************
    '''
    '''
    super的使用详解
    
    super主要来调用父类方法来显式调用父类。    
    super可以防止多重继承时父类被多次初始化。 
    
    通过父类名直接访问父类的属性，当调用父类的方法是，需要将”self”显示的传递进去的方式；
    但是这会导致需要经父类名硬编码到子类中，可以用super关键字代替。    
    class Parent(object):
    Value = "Hi, Parent value"
    def fun(self):    print("This is from Parent")
    class Child(Parent):
    Value = "Hi, Child  value"
    def fun(self):    print("This is from Child")，  Parent.fun(self)   #调用父类Parent的fun函数方法    
    class Child(Parent):
    Value = "Hi, Child  value"
    def fun(self):    print("This is from Child")，  super(Child,self).fun()  #避免父类名硬编码到子类
    '''
    def __init__(self, range, rows, cols):
        super(TrainSupervisedLearnEstimator, self).__init__()  # super()调用当前类的父类的__init__()函数。

        data_x, data_y = super(TrainSupervisedLearnEstimator, self).get_data(range, rows, cols)  # 调用父类方法。
        x_train, x_test, y_train, y_test = super(TrainSupervisedLearnEstimator, self).split_train_test(data_x, data_y)
        self.x_train = x_train
        self.y_train = y_train
        self.x_test = x_test
        self.y_test = y_test

    def return_test_data(self):
        return self.x_test, self.y_test

    def train_linear_model(self):
        lr = super(TrainSupervisedLearnEstimator, self).get_linear_model()
        lr.fit(self.x_train, self.y_train)  # 监督学习模型， 参数有训练特征子集和训练目标子集。
        return lr

    def train_svc_model(self):
        svc = super(TrainSupervisedLearnEstimator, self).get_svc_model()
        svc.fit(self.x_train, self.y_train)
        return svc

    def train_svr_model(self):
        svr = super(TrainSupervisedLearnEstimator, self).get_svr_model()
        svr.fit(self.x_train, self.y_train)
        return svr

    def train_randomforest_classify_model(self):
        rf_c = super(TrainSupervisedLearnEstimator, self).get_randomforest_classify_model()
        rf_c.fit(self.x_train, self.y_train)
        return rf_c

    def train_randomforest_regress_model(self):
        rf_r = super(TrainSupervisedLearnEstimator, self).get_randomforest_regress_model()
        rf_r.fit(self.x_train, self.y_train)
        return rf_r

    def train_decisiontree_classify_model(self):
        tree_c = super(TrainSupervisedLearnEstimator, self).get_decisiontree_classify_model()
        tree_c.fit(self.x_train, self.y_train)
        return tree_c

    def train_decisiontree_regress_model(self):
        tree_r = super(TrainSupervisedLearnEstimator, self).get_decisiontree_regress_model()
        tree_r.fit(self.x_train, self.y_train)
        return tree_r

    def train_naviebayes_bernoulli_model(self):
        nb_b = super(TrainSupervisedLearnEstimator, self).get_naviebayes_bernoulli_model()
        nb_b.fit(self.x_train, self.y_train)
        return nb_b

    def train_naviebayes_gaussian_model(self):
        nb_g = super(TrainSupervisedLearnEstimator, self).get_naviebayes_gaussian_model()
        nb_g.fit(self.x_train, self.y_train)
        return nb_g

    def train_naviebayes_multinormal_model(self):
        nb_m = super(TrainSupervisedLearnEstimator, self).get_naviebayes_multinomal_model()
        nb_m.fit(self.x_train, self.y_train)
        return nb_m


# 训练非监督学习模型。
class TrainUnSupervisedLearnEstimator(SplitTrainTest, UnSupervisedLearnEstimator):
    def __init__(self, range, rows, cols):
        super(TrainUnSupervisedLearnEstimator, self).__init__()
        data_x, data_y = super(TrainUnSupervisedLearnEstimator, self).get_data(range, rows, cols)
        x_tr, x_te, y_tr, y_te = super(TrainUnSupervisedLearnEstimator, self).split_train_test(data_x, data_y)
        self.x_train = x_tr
        self.x_test = x_te
        self.y_train = y_tr
        self.y_test = y_te

    def return_test_data(self):
        return self.x_test, self.y_test

    def train_kmean_model(self, n_cluster=8):
        kmean = super(TrainUnSupervisedLearnEstimator, self).get_kmeans_model(n_cluster)
        kmean.fit(self.x_train)  # 非监督学习，不需要对数据做标记，只要一个数组。
        return kmean


''' 测试模型 '''


class TestEstimator(object):
    def __init__(self):
        pass

    def test_estimator_model(self, model, x_test):  # 动态语言天然支持多态（无类型检查）。
        y_pred = model.predict(x_test)
        return y_pred


''' 评估模型 '''
from sklearn.metrics import accuracy_score, hamming_loss, classification_report
from sklearn.metrics import mean_absolute_error, mean_squared_error, r2_score
from sklearn.metrics import adjusted_rand_score, homogeneity_score, normalized_mutual_info_score, v_measure_score


class EvaluateEstimator(Logger):
    '''
    对不同的模型有不同的评价方法。
    '''
    def __init__(self):
        super(EvaluateEstimator, self).__init__()
        self.__logger = super(EvaluateEstimator, self).get_default_logger()

    def evaluate_classifiermodel_by_accuracyscore(self, y_test, y_pred):
        '''
        精度分类评分。
        在多标记分类中，计算测试子集与预测结果的准确度。
        '''
        score = accuracy_score(y_test, y_pred)
        self.__logger.info("分类模型， 精度分类评分： %d", score)  # 培养习惯。
        return score

    def evaluate_classifiermodel_by_hammingloss(self, y_test, y_pred):
        '''
        汉明损失。
        接受测试子集和预测结果作为参数，其返回的结果值在 0 到 1 之间，
        返回值越小，损失越小，准确度越高；返回值越大，损失越大，准确度越低。
        '''
        loss = hamming_loss(y_test, y_pred)
        return loss

    def evaluate_classifiermodel_by_report(self, y_test, y_pred):
        '''
        分类报告。
        构建一个显示主要分类指标的文本报告，结果返回精确度、召回率和每一个分类的 F1分数。
        '''
        report = classification_report(y_test, y_pred)
        return report

    def evaluate_regressmodel_by_absoluteerror(self, y_test, y_pred):
        '''
        绝对平均误差。
        计算绝对平均误差的损失，返回结果为 0 到 1 之间的浮点数，最好的结果为 0.0。
        '''
        error = mean_absolute_error(y_test, y_pred)
        return error

    def evaluate_regressmodel_by_squareerror(self, y_test, y_pred):
        '''
        均方差。
        计算均方差的损失，返回结果为非负浮点数，最好的结果为 0.0。
        '''
        error = mean_squared_error(y_test, y_pred)
        self.__logger.warning("回归模型，均方差：%d", error)  # warn取消。
        return error

    def evaluate_regressmodel_by_r2score(self, y_test, y_pred):
        '''
        决定系数的回归分数。
        计算 R^2 决定系数的分数，返回结果为浮点数，最佳结果为 1.0，如果模型效果很差，结果可能为负数。
        '''
        score = r2_score(y_test, y_pred)
        return score

    def evaluate_clustermodel_by_randscore(self, y_test, y_pred):
        '''
        兰德指数分数。
        对真实聚类和预测聚类计算相似性度量，最佳值为 1.0 。
        '''
        score = adjusted_rand_score(y_test, y_pred)
        self.__logger.error("聚类问题，兰德指数分数：%d", score)  # 培养习惯。
        return score

    def evaluate_clustermodel_by_homogeneityscore(self, y_test, y_pred):
        '''
        均衡性度量。
        计算预测聚类的均匀性度量，分数在 0.0 到 1.0 之间， 最佳结果为 1.0。
        '''
        score = homogeneity_score(y_test, y_pred)
        return score

    def evaluate_clustermodel_by_normalizescore(self, y_test, y_pred):
        '''
        规范性度量。
        计算两个聚类之间的规范性，分数在 0.0 和 1.0 之间，最佳结果为 1.0。
        '''
        score = normalized_mutual_info_score(y_test, y_pred)
        return score

    def evaluate_clustermodel_by_vmeasurescore(self, y_test, y_pred):
        '''
        多度量。
        计算聚类的均匀性和规范性的调和平均值，结果分数在 0.0 和 1.0 之间，最佳结果为 1.0。
        '''
        score = v_measure_score(y_test, y_pred)
        return score


''' 机器学习问题实例 '''
from sklearn import datasets
from sklearn.metrics import explained_variance_score


# 回归问题实例
class RegressModelDemo(object):
    '''
    机器学习-回归问题-波士顿房价预测。
    '''
    def __init__(self):
        self.__logger = Logger().get_default_logger()
        self.__logger.info("\n\n机器学习-回归问题-波士顿房价预测。")
        self.__data = datasets.load_boston()  # sklearn模型自带的波士顿房价数据。
        self.x_train, self.x_test, self.y_train, self.y_test = None, None, None, None
        self.y_pred = None
        self.model = None

    def preview_data_keys(self):
        keys = self.__data.keys()  # 数据是字典类型。
        self.__logger.info(keys)
        return keys

    def preview_data_describe(self):
        describe = self.__data.DESCR  # 数据描述。
        self.__logger.info(describe)
        return describe

    def preview_data_feature(self):
        feature = self.__data.data  # 数据处理好的特征值。
        self.__logger.info(feature)
        return feature

    def preview_data_featurenames(self):
        names = self.__data.feature_names
        self.__logger.info(names)
        return names

    def split_train_test(self):
        self.x_train, self.x_test, self.y_train, self.y_test = \
            train_test_split(self.__data.data, self.__data.target, test_size=0.15)  # 测试集占15%。
        self.__logger.info("波士顿房价数据集分割完毕, test_size=0.15。")

    def create_randomforestregress_model(self):
        self.model = RandomForestRegressor(max_depth=10, n_estimators=150)  # 随机森林，设定树的最大深度和树的最大数量。
        self.__logger.info("随机森林模型创建完毕，max_depth=10, n_estimator=150.")

    def train_model(self):
        self.model.fit(self.x_train, self.y_train)
        self.__logger.warning("模型训练完毕。")

    def test_model(self):
        self.y_pred = self.model.predict(self.x_test)
        self.__logger.warning("模型测试完毕。")

    def evaluate_model(self):
        ev_score = explained_variance_score(self.y_pred, self.y_test)
        self.__logger.warning("模型评估-解释方差评分：%f", ev_score)
        r_score = r2_score(self.y_pred, self.y_test)
        self.__logger.warning("模型评估-R平方拟合优度：%f", r_score)
        return ev_score, r_score

    def show_estimate_result(self, start, stop):
        len_result = len(self.y_test)
        if start < 0 or stop >= len_result:
            self.__logger.error("查看模型预测结果失败：start(%d), stop(%d)", start, stop)
            raise ValueError
        print("波士顿房价预测：\n")
        for test, pred in zip(self.y_test[start:stop], self.y_pred[start:stop]):
            print("正确值：%f,\t=>预测值：%f\n" % (test, pred))



















