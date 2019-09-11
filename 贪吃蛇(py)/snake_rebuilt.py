import pygame
import random
import sys

SCREEN_X = 800
SCREEN_Y = 600


class Snake(object):
    def __init__(self):
        self.direction = pygame.K_RIGHT
        self.body = []
        for x in range(5):
            self.addnode()

    def addnode(self):
        left, top = (0, 0)
        if self.body:
            left, top = (self.body[0].left, self.body[0].top)

        node = pygame.Rect(left, top, 25, 25)
        if self.direction == pygame.K_RIGHT:
            node.left += 25
        elif self.direction == pygame.K_LEFT:
            node.left -= 25
        elif self.direction == pygame.K_UP:
            node.top -= 25
        elif self.direction == pygame.K_DOWN:
            node.top += 25

        self.body.insert(0, node)
        # self.body.append(),错误：会导致addnode和delnode相互抵消，无前进。

    def delnode(self):
        self.body.pop()

    def move(self):
        self.addnode()
        self.delnode()

    def isdead(self):
        if self.body[0].left not in range(SCREEN_X):
            return True
        if self.body[0].top not in range(SCREEN_Y):
            return True
        if self.body[0] in self.body[1:]:
            return True
        return False

    def changedirection(self, curkey):
        LR = (pygame.K_RIGHT, pygame.K_LEFT)
        UD = (pygame.K_UP, pygame.K_DOWN)
        if (curkey in LR) and (self.direction in LR): return
        if (curkey in UD) and (self.direction in UD): return
        self.direction = curkey


class Food(object):
    def __init__(self):
        self.rect = pygame.Rect(-25, 0, 25, 25)

    def remove(self):
        self.rect.x = -25

    def set(self):
        if self.rect.x == -25:
            allpos_x, allpos_y = [], []
            for pos in range(25, SCREEN_X-25, 25):
                allpos_x.append(pos)
            for pos in range(25, SCREEN_Y-25, 25):
                allpos_y.append(pos)
            self.rect.x = random.choice(allpos_x)
            self.rect.y = random.choice(allpos_y)


def showtext(screen, pos, text, color, font_bold=False, font_size=60,
             font_italic=False):
    cur_font = pygame.font.SysFont('宋体', font_size)
    cur_font.set_bold(font_bold)
    cur_font.set_italic(font_italic)

    text_fmt = cur_font.render(text, 1, color)
    screen.blit(text_fmt, pos)


def main():
    print('this is snake-rebuilt1 routine.')
    pygame.init()
    screen_size = (SCREEN_X, SCREEN_Y)
    screen = pygame.display.set_mode(screen_size)
    pygame.display.set_caption('Snake')
    clock = pygame.time.Clock()

    scores = 0
    isdead = False

    snake = Snake()
    food = Food()

    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                sys.exit()
            if event.type == pygame.KEYDOWN:
                snake.changedirection(event.key)
                if event.key == pygame.K_SPACE and isdead:
                    return main()
        screen.fill((255, 255, 255))

        if not isdead:
            scores += 1
            snake.move()

        for rect in snake.body:
            pygame.draw.rect(screen, (0, 255, 0), rect, 0)

        isdead = snake.isdead()
        if isdead:
            showtext(screen, (100, 200), 'YOU DEAD !', (227, 29, 18), False, 100)
            showtext(screen, (120, 260), 'press space to try again...', (0, 0, 22), False, 30)

        if food.rect == snake.body[0]:
            scores += 50
            food.remove()
            snake.addnode()

        food.set()
        pygame.draw.rect(screen, (136, 0, 21), food.rect, 0)
        showtext(screen, (50, 500), 'Scores: '+str(scores), (223, 223, 223))

        pygame.display.update()
        clock.tick(10)


if __name__ == '__main__':
    main()