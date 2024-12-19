CC = gcc
PROG = projet

BUILD_DIR := ./build

# Do not remove any flags here (you may add -g for debugging)
CFLAGS := -g3 --pedantic -Wall -pedantic -std=c99 -lm
LDFLAGS := -g -lm

# Automatically detect all .c files in the current directory
SRC_FILES := $(wildcard *.c)

OBJS := $(patsubst %.c, $(BUILD_DIR)/%.o, $(notdir $(SRC_FILES)))

$(PROG): $(OBJS)
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(BUILD_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

valgrind:
	valgrind --track-origins=yes --leak-check=full ./$(PROG)

gdb:
	gdb ./$(PROG)

cppcheck:
	echo "Running cppcheck with --enable=all and --check-config..." >> cppcheck_report.txt
	cppcheck --enable=all --check-config --inconclusive --verbose -I. --suppress=missingIncludeSystem $(SRC_FILES) >> cppcheck_report.txt 2>&1
	echo "\n\nRunning cppcheck with --enable=warning,style,performance,portability,information..." >> cppcheck_report.txt
	cppcheck --enable=warning,style,performance,portability,information --inconclusive --verbose -I. --suppress=missingIncludeSystem $(SRC_FILES) >> cppcheck_report.txt 2>&1
	echo "\n\nRunning cppcheck with --enable=unusedFunction and --check-config..." >> cppcheck_report.txt
	cppcheck --enable=unusedFunction --check-config --inconclusive --verbose -I. --suppress=missingIncludeSystem $(SRC_FILES) >> cppcheck_report.txt 2>&1
	echo "\n\nRunning cppcheck with --enable=performance..." >> cppcheck_report.txt
	cppcheck --enable=performance --inconclusive --verbose -I. --suppress=missingIncludeSystem $(SRC_FILES) >> cppcheck_report.txt 2>&1
	echo "\n\nRunning cppcheck with --enable=all and --force..." >> cppcheck_report.txt
	cppcheck --enable=all --inconclusive --verbose --force -I. --suppress=missingIncludeSystem $(SRC_FILES) >> cppcheck_report.txt 2>&1
	echo "\n\nRunning cppcheck with --enable=style..." >> cppcheck_report.txt
	cppcheck --enable=style --inconclusive --verbose -I. --suppress=missingIncludeSystem $(SRC_FILES) >> cppcheck_report.txt 2>&1
	echo "\n\nRunning cppcheck with --enable=all and --suppress=missingIncludeSystem..." >> cppcheck_report.txt
	cppcheck --enable=all --inconclusive --verbose --suppress=missingIncludeSystem -I. $(SRC_FILES) >> cppcheck_report.txt 2>&1
	echo "\n\nRunning cppcheck with --enable=style and --suppress=missingIncludeSystem..." >> cppcheck_report.txt
	cppcheck --enable=style --inconclusive --verbose --suppress=missingIncludeSystem -I. $(SRC_FILES) >> cppcheck_report.txt 2>&1



cppcheck_common_errors:
	cppcheck --enable=warning,style,performance,portability,information --inconclusive --verbose -I. --suppress=missingIncludeSystem --output-file=cppcheck_report.txt $(SRC_FILES)

cppcheck_memory_leaks:
	cppcheck --enable=unusedFunction --check-config --inconclusive --verbose -I. --suppress=missingIncludeSystem --output-file=cppcheck_report.txt $(SRC_FILES)

cppcheck_performance:
	cppcheck --enable=performance --inconclusive --verbose -I. --suppress=missingIncludeSystem --output-file=cppcheck_report.txt $(SRC_FILES)

cppcheck_security:
	cppcheck --enable=all --inconclusive --verbose --force -I. --suppress=missingIncludeSystem --output-file=cppcheck_report.txt $(SRC_FILES)

cppcheck_code_style:
	cppcheck --enable=style --inconclusive --verbose -I. --suppress=missingIncludeSystem --output-file=cppcheck_report.txt $(SRC_FILES)

cppcheck_suppress:
	cppcheck --enable=all --inconclusive --verbose --suppress=missingIncludeSystem -I. --output-file=cppcheck_report.txt $(SRC_FILES)

cppcheck_const:
	cppcheck --enable=style --inconclusive --verbose --suppress=missingIncludeSystem -I. --output-file=cppcheck_report.txt $(SRC_FILES)

memory_sanitizer:
	clang -fsanitize=memory -fno-omit-frame-pointer -g -o $(PROG) $(SRC_FILES) $(CFLAGS) $(LDFLAGS)

tar:
	tar -czvf S-192865.tar.gz *

.PHONY: clean

clean:
	rm -rf $(BUILD_DIR) $(PROG) *.o cppcheck_report.txt
