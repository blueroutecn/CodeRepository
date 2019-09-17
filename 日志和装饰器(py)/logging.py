#-*- coding: gbk -*-



'''日志'''

import logging

logger = logging.getLogger("logTest")   # 常见日志器，logTest为名称
logger.setLevel(logging.INFO)           # 设置日志器的等级(NOTSET, DEBUG, INFO, WARNING, ERROR, CRITICAL)

handler_file = logging.FileHandler("log_logTest.log", "a+", encoding="utf-8")  # 创建文件处理器
handler_stream = logging.StreamHandler()   # 创建控制台处理器
handler_file.setLevel(logging.INFO)        # 单独设置文件处理器的等级
handler_stream.setLevel(logging.ERROR)     # 单独设置控制台处理器的等级

formatter = logging.Formatter(fmt='%(asctime)s :%(message)s', datefmt='%Y-%m-%d  %H:%M:%S')
handler_file.setFormatter(formatter)       # 设置处理器输出格式
handler_stream.setFormatter(formatter)

logger.addHandler(handler_file)            # 将处理器注册到日志器
logger.addHandler(handler_stream)

logger.debug("logger debug level")         # 写入debug级别的日志信息

logger.info("logger info level")           # 写入info级别的日志信息
logger.warning("logger warning level")
logger.error("logger error level")
logger.critical("logger critical level")


