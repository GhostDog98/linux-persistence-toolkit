TARGET_NAME = persist
SOURCES = main.cpp
OBJECTS = $(SOURCES:.cpp=.o)
CFLAGS = -g -Wall -Wextra -O3 -std=c++20
LDFLAGS = -lm

$(TARGET_NAME): $(OBJECTS)
	$(CXX) $(CFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET_NAME)
