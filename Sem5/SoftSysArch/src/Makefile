# Компилятор и флаги
CXX = g++
CXXFLAGS = -std=c++17 -pthread -Wall -Wextra -O2
TARGET = aikido_simulation

# Исходные файлы
SRCS = main.cpp Bid.cpp Buffer.cpp Source.cpp Device.cpp EventCalendar.cpp SimulationModel.cpp
OBJS = $(SRCS:.cpp=.o)
HEADERS = Bid.hpp Buffer.hpp Source.hpp Device.hpp EventCalendar.hpp SimulationModel.hpp

# Правило по умолчанию
all: $(TARGET)

# Сборка исполняемого файла
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Компиляция объектных файлов
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Очистка
clean:
	rm -f $(OBJS) $(TARGET)

# Пересборка
rebuild: clean all

# Запуск
run: $(TARGET)
	./$(TARGET)

# Отладочная сборка
debug: CXXFLAGS += -g -DDEBUG
debug: rebuild

.PHONY: all clean rebuild run debug
