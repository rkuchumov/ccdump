CC = g++ -std=c++11
ALL_FLAGS = -c
CFLAGS = -Wall -Wextra -O4 -Os

TARGET = ccdump
OBJS_DIR = build

INCLUDE = -I./include
SOURCES = $(wildcard ./src/*.cpp)
OBJECTS = $(SOURCES:./src/%.cpp=./$(OBJS_DIR)/%.o)
HEADERS = $(wildcard ./include/*.h)

.PHONY: all
all: objs_dir  $(TARGET)

.PHONY: objs_dir
objs_dir:
	mkdir -p $(OBJS_DIR)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

$(OBJS_DIR)/%.o: src/%.cpp
	$(CC) $(ALL_FLAGS) $(VARS) $(CFLAGS) $(INCLUDE) $< -o $@

.PHONY: clean
clean:
	rm -rfv $(TARGET) $(OBJS_DIR) 2>/dev/null || true

.PHONY: dep
dep: $(OBJS_DIR)/.depend

$(OBJS_DIR)/.depend: $(SOURCES)
	mkdir -p $(OBJS_DIR)
	$(CC) $(ALL_FLAGS) $(CFLAGS) $(INCLUDE) -MM $(SOURCES) | sed -e  's/.*:/$(OBJS_DIR)\/&/' > $(OBJS_DIR)/.depend

-include $(OBJS_DIR)/.depend
