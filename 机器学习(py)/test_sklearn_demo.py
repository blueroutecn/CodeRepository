import unittest
from demo1 import *


class TestDataPreProcessing(unittest.TestCase):
    def setUp(self):
        self.__demo = DataPreProcessing(2, 3)

    def test_standard_scaler(self):
        standard_x = self.__demo.standard_scaler()
        if standard_x.all():
            print("standard_scaler() passed.")
        else:
            raise RuntimeError("standard_scaler() failed.")


class TestSplitTrainTest(unittest.TestCase):
    def setUp(self):
        self.__demo = SplitTrainTest()

    def test_split_train_test(self):
        x, y = self.__demo.get_data(10, 5, 2)
        x_train, x_test, y_train, y_test = self.__demo.split_train_test(x, y)


class TestSupervisedLearnEstimator(unittest.TestCase):
    def setUp(self):
        self.__demo = SupervisedLearnEstimator()

    def test_get_linear_model(self):
        lr = self.__demo.get_linear_model()


class TestUnSupervisedLearnEstimator(unittest.TestCase):
    def setUp(self):
        self.__demo = UnSupervisedLearnEstimator()
        self.__ncluster = 3

    def test_get_kmean_model(self):
        kmean = self.__demo.get_kmeans_model(self.__ncluster)


class TestTrainSupervisedLearnEstimator(unittest.TestCase):
    def setUp(self):
        self.__demo = TrainSupervisedLearnEstimator(10, 5, 2)

    def test_train_linear_model(self):
        lr = self.__demo.train_linear_model()


class TestTrainUnSupervisedLearnEstimator(unittest.TestCase):
    def setUp(self):
        self.__demo = TrainUnSupervisedLearnEstimator(10, 5, 2)
        self.__ncluster = 3

    def test_train_kmean_model(self):
        kmean = self.__demo.train_kmean_model(self.__ncluster)


class TestTestEstimator(unittest.TestCase):
    def setUp(self):
        self.__demo0 = TestEstimator()
        self.__demo1 = TrainSupervisedLearnEstimator(10, 5, 2)
        self.__demo2 = TrainUnSupervisedLearnEstimator(10, 5, 2)

    def test_test_estimator_linear(self):
        lr = self.__demo1.train_linear_model()
        x_test, y_test = self.__demo1.return_test_data()
        y_pred = self.__demo0.test_estimator_model(lr, x_test)

    def test_test_estimator_svc(self):
        svc = self.__demo1.train_svc_model()
        x_test, y_test = self.__demo1.return_test_data()
        y_pred = self.__demo0.test_estimator_model(svc, x_test)

    def test_test_estimator_kmean(self):
        kmean = self.__demo2.train_kmean_model(3)
        x_test, y_test = self.__demo2.return_test_data()
        y_pred = self.__demo0.test_estimator_model(kmean, x_test)


class TestEvaluateEstimator(unittest.TestCase):
    def setUp(self):
        self.__demo0 = EvaluateEstimator()
        self.__demo1 = TrainSupervisedLearnEstimator(10, 5, 2)
        self.__demo2 = TrainUnSupervisedLearnEstimator(10, 5, 2)
        self.__demo3 = TestEstimator()

    def test_evaluate_classifiermodel_by_accuracyscore(self):
        svc = self.__demo1.train_svc_model()
        x_test, y_test = self.__demo1.return_test_data()
        y_pred = self.__demo3.test_estimator_model(svc, x_test)
        score = self.__demo0.evaluate_classifiermodel_by_accuracyscore(y_test, y_pred)

    def test_evaluate_regressmodel_by_squareerror(self):
        lr = self.__demo1.train_linear_model()
        x_test, y_test = self.__demo1.return_test_data()
        y_pred = self.__demo3.test_estimator_model(lr, x_test)
        error = self.__demo0.evaluate_regressmodel_by_squareerror(y_test, y_pred)

    def test_evaluate_clustermodel_by_randscore(self):
        kmean = self.__demo2.train_kmean_model(3)
        x_test, y_test = self.__demo2.return_test_data()
        y_pred = self.__demo3.test_estimator_model(kmean, x_test)
        score = self.__demo0.evaluate_clustermodel_by_randscore(y_test, y_pred)


class TestRegressModelDemo(unittest.TestCase):
    def setUp(self):
        self.__demo = RegressModelDemo()

    def test_preview_data(self):
        self.__demo.preview_data_describe()
        self.__demo.preview_data_keys()
        self.__demo.preview_data_feature()
        self.__demo.preview_data_featurenames()

    def test_split_train_test(self):
        self.__demo.split_train_test()
        len_xtest, len_xtrain = len(self.__demo.x_test), len(self.__demo.x_train)
        len_ytest, len_ytrain = len(self.__demo.y_test), len(self.__demo.y_train)
        self.assertEqual(len_xtest, len_ytest)
        self.assertEqual(len_xtrain, len_ytrain)
        self.assertEqual(len_xtest, len_ytest)
        if len_xtest/(len_xtest+len_xtrain) - 0.15 > 1e-2:
            self.assertRaises(ValueError)

    def test_create_randomforestmodel(self):
        self.__demo.create_randomforestregress_model()

    def test_evaluate_model_and_show_result(self):
        self.__demo.split_train_test()
        self.__demo.create_randomforestregress_model()
        self.__demo.train_model()
        self.__demo.test_model()
        self.__demo.evaluate_model()
        start, stop = -1, 10
        with self.assertRaises(ValueError):
            self.__demo.show_estimate_result(start, stop)
        start, stop = 0, 1e5
        with self.assertRaises(ValueError):
            self.__demo.show_estimate_result(start, stop)
        start, stop = 0, 10
        self.__demo.show_estimate_result(start, stop)






if __name__ == "__main__":
    unittest.main()
