#-*- coding: gbk -*-



'''��־'''

import logging

logger = logging.getLogger("logTest")   # ������־����logTestΪ����
logger.setLevel(logging.INFO)           # ������־���ĵȼ�(NOTSET, DEBUG, INFO, WARNING, ERROR, CRITICAL)

handler_file = logging.FileHandler("log_logTest.log", "a+", encoding="utf-8")  # �����ļ�������
handler_stream = logging.StreamHandler()   # ��������̨������
handler_file.setLevel(logging.INFO)        # ���������ļ��������ĵȼ�
handler_stream.setLevel(logging.ERROR)     # �������ÿ���̨�������ĵȼ�

formatter = logging.Formatter(fmt='%(asctime)s :%(message)s', datefmt='%Y-%m-%d  %H:%M:%S')
handler_file.setFormatter(formatter)       # ���ô����������ʽ
handler_stream.setFormatter(formatter)

logger.addHandler(handler_file)            # ��������ע�ᵽ��־��
logger.addHandler(handler_stream)

logger.debug("logger debug level")         # д��debug�������־��Ϣ

logger.info("logger info level")           # д��info�������־��Ϣ
logger.warning("logger warning level")
logger.error("logger error level")
logger.critical("logger critical level")


