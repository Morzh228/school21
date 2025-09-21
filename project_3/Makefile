# Makefile for a flat directory structure

# Компилятор и флаги
CC = gcc
# CFLAGS для строгой проверки и стандарта C11
CFLAGS = -Wall -Wextra -Werror -std=c11
# LDFLAGS для подключения математической библиотеки (-lm)
LDFLAGS = -lm

# Имя исполняемого файла
EXECUTABLE = graph

# Исходные .c файлы. Можно задать вручную или через wildcard.
# Вариант с wildcard (автоматически найдет все .c файлы):
SRCS = $(wildcard *.c)
# Вариант вручную (более надежный, если в папке есть другие .c файлы):
# SRCS = graph.c s21_parser.c s21_calculator.c s21_stack.c

# Объектные файлы (.o) генерируются из исходных (.c)
OBJS = $(SRCS:.c=.o)

# .PHONY указывает, что эти цели не являются файлами
.PHONY: all clean re check format

# Цель по умолчанию - собрать исполняемый файл
all: $(EXECUTABLE)

# Правило для сборки исполняемого файла из объектных файлов
$(EXECUTABLE): $(OBJS)
	@echo "Linking object files..."
	$(CC) $(CFLAGS) $(OBJS) -o $(EXECUTABLE) $(LDFLAGS)
	@echo "Build successful! Executable is at ./$(EXECUTABLE)"

# Общее правило для компиляции объектных файлов (.o) из исходных (.c)
# Для каждого .c файла будет вызвана эта команда
%.o: %.c
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Очистка скомпилированных файлов
clean:
	@echo "Cleaning up..."
	rm -f $(OBJS) $(EXECUTABLE)

# Полная пересборка
rebuild: clean all

# Инструменты для проверки качества кода
check:
	cppcheck --enable=all --suppress=missingIncludeSystem *.c
	clang-format -n --style=Google *.c *.h

# Автоформатирование кода
format:
	clang-format -i --style=Google *.c *.h