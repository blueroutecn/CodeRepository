from snake import *
import sys


def main():
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
            pygame.draw.rect(screen, (0, 225, 0), rect, 0)

        isdead = snake.isdead()
        if isdead:
            showtext(screen, (100, 200), 'YOU DEAD !', (227, 29, 18), False, 100)
            showtext(screen, (120, 260), 'press space to try again......', (0, 0, 22), False, 30)

        if food.rect == snake.body[0]:
            scores += 50
            food.remove()
            snake.addnode()

        food.set()
        pygame.draw.rect(screen, (136, 0, 21), food.rect, 0)
        showtext(screen, (50, 500), 'Scores: ' + str(scores), (223, 223, 223))

        pygame.display.update()
        clock.tick(10)


if __name__ == '__main__':
    main()








