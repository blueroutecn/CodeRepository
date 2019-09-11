
import random
import pygame

# 全局定义
# global definition
SCREEN_X = 800
SCREEN_Y = 600


# 贪吃蛇类
# 点以25为单位
class Snake(object):

    # 初始化各种需要的属性（开始时默认向右/身体块x5）
    def __init__(self):
        self.direction = pygame.K_RIGHT
        self.body = []
        for x in range(5):
            self.addnode()

    # 无论何时都在前段增加蛇块
    def addnode(self):
        left, top = (0, 0)     # 采用元组的多元赋值
        if self.body:
            left, top = (self.body[0].left, self.body[0].top)

        # Rect函数的参数left.top,width,height
        node = pygame.Rect(left, top, 25, 25)

        if self.direction == pygame.K_LEFT:
            node.left -= 25
        elif self.direction == pygame.K_RIGHT:
            node.left += 25
        elif self.direction == pygame.K_UP:
            node.top -= 25
        elif self.direction == pygame.K_DOWN:
            node.top += 25

        self.body.insert(0, node)

    # 删除最后一个块
    def delnode(self):
            self.body.pop()

    # 死亡判断
    def isdead(self):
        # 撞墙
        if self.body[0].x not in range(SCREEN_X):
            return True
        if self.body[0].y not in range(SCREEN_Y):
            return True
        # 撞自己
        if self.body[0] in self.body[1:]:
            return True

        return False

    # 移动
    def move(self):
        self.addnode()
        self.delnode()

    # 改变方向，但是左右上下不能被逆向改变
    def changedirection(self, curkey):
        LR = [pygame.K_LEFT, pygame.K_RIGHT]
        UD = [pygame.K_UP, pygame.K_DOWN]
        if curkey in LR+UD:
            if (curkey in LR) and (self.direction in LR):
                return
            if (curkey in UD) and (self.direction in UD):
                return
            self.direction = curkey


# 食物类
# 方法：放置/移除
# 点以25为单位
class Food(object):
    def __init__(self):
        self.rect = pygame.Rect(-25, 0, 25, 25)

    def remove(self):
        self.rect.x = -25

    def set(self):
        if self.rect.x == -25:
            allpos_x, allpos_y = [], []
            for pos in range(25, SCREEN_X - 25, 25):
                allpos_x.append(pos)
            for pos in range(25, SCREEN_Y - 25, 25):
                allpos_y.append(pos)
            self.rect.left = random.choice(allpos_x)
            self.rect.top = random.choice(allpos_y)
            print(self.rect)


def showtext(screen, pos, text, color, font_bold=False, font_size=60,
             font_italic=False):
    # 获取系统字体，设置字体大小。
    cur_font = pygame.font.SysFont('宋体', font_size)

    # 设置是否加粗属性。
    cur_font.set_bold(font_bold)

    # 设置是否斜体属性。
    cur_font.set_italic(font_italic)

    # 设置文字内容。
    text_fmt = cur_font.render(text, 1, color)

    # 绘制文字。
    screen.blit(text_fmt, pos)


def display():
    print('it is here.')


