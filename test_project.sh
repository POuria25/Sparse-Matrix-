#!/bin/bash

# Define the list of test cases
TEST_CASES=(
  "huge.txt huge.b.txt huge.sol.txt"
  "medium.txt medium.b.txt medium.sol.txt"
  "small.txt small.b.txt small.sol.txt"
  "tiny.txt tiny.b.txt tiny.sol.txt"
)

# File paths for logs
VALGRIND_LOG="valgrind_log.txt"
CPPCHECK_LOG="cppcheck_report.txt"

# Clear logs
echo "Valgrind Log - $(date)" > "$VALGRIND_LOG"
echo "Cppcheck Report - $(date)" > "$CPPCHECK_LOG"

# Cppcheck commands
echo "Running cppcheck..." >> "$CPPCHECK_LOG"
echo "Running cppcheck with --enable=all and --check-config..." >> "$CPPCHECK_LOG"
cppcheck --enable=all --check-config --inconclusive --verbose -I. --suppress=missingIncludeSystem *.c >> "$CPPCHECK_LOG" 2>&1

echo -e "\n\nRunning cppcheck with --enable=warning,style,performance,portability,information..." >> "$CPPCHECK_LOG"
cppcheck --enable=warning,style,performance,portability,information --inconclusive --verbose -I. --suppress=missingIncludeSystem *.c >> "$CPPCHECK_LOG" 2>&1

echo -e "\n\nRunning cppcheck with --enable=unusedFunction and --check-config..." >> "$CPPCHECK_LOG"
cppcheck --enable=unusedFunction --check-config --inconclusive --verbose -I. --suppress=missingIncludeSystem *.c >> "$CPPCHECK_LOG" 2>&1

echo -e "\n\nRunning cppcheck with --enable=performance..." >> "$CPPCHECK_LOG"
cppcheck --enable=performance --inconclusive --verbose -I. --suppress=missingIncludeSystem *.c >> "$CPPCHECK_LOG" 2>&1

echo -e "\n\nRunning cppcheck with --enable=all and --force..." >> "$CPPCHECK_LOG"
cppcheck --enable=all --inconclusive --verbose --force -I. --suppress=missingIncludeSystem *.c >> "$CPPCHECK_LOG" 2>&1

echo -e "\n\nRunning cppcheck with --enable=style..." >> "$CPPCHECK_LOG"
cppcheck --enable=style --inconclusive --verbose -I. --suppress=missingIncludeSystem *.c >> "$CPPCHECK_LOG" 2>&1

echo -e "\n\nRunning cppcheck with --enable=all and --suppress=missingIncludeSystem..." >> "$CPPCHECK_LOG"
cppcheck --enable=all --inconclusive --verbose --suppress=missingIncludeSystem -I. *.c >> "$CPPCHECK_LOG" 2>&1

echo -e "\n\nRunning cppcheck with --enable=style and --suppress=missingIncludeSystem..." >> "$CPPCHECK_LOG"
cppcheck --enable=style --inconclusive --verbose --suppress=missingIncludeSystem -I. *.c >> "$CPPCHECK_LOG" 2>&1

echo "Cppcheck analysis completed." >> "$CPPCHECK_LOG"

# Iterate over each test case
TEST_NUMBER=1
for TEST in "${TEST_CASES[@]}"; do
  # Split the test case into input files
  INPUT_MATRIX=$(echo $TEST | cut -d' ' -f1)
  INPUT_RHS=$(echo $TEST | cut -d' ' -f2)
  EXPECTED_SOLUTION=$(echo $TEST | cut -d' ' -f3)
  OUTPUT_SOLUTION="output_${INPUT_MATRIX%.txt}.mtx" # Output file name based on input matrix

  # Check if the required files exist
  if [ ! -f "$INPUT_MATRIX" ] || [ ! -f "$INPUT_RHS" ] || [ ! -f "$EXPECTED_SOLUTION" ]; then
    echo "Test $TEST_NUMBER - [$INPUT_MATRIX, $INPUT_RHS, $EXPECTED_SOLUTION] Failed (Missing input files)" >> "$VALGRIND_LOG"
    ((TEST_NUMBER++))
    continue
  fi

  # Run the program with valgrind and save the output
  echo "Test $TEST_NUMBER - [$INPUT_MATRIX, $INPUT_RHS]" >> "$VALGRIND_LOG"
  valgrind --track-origins=yes --leak-check=full ./projet "$INPUT_MATRIX" "$INPUT_RHS" 1e-10 > "$OUTPUT_SOLUTION" 2>> "$VALGRIND_LOG"

  if [ $? -ne 0 ]; then
    echo "Test $TEST_NUMBER - [$INPUT_MATRIX, $INPUT_RHS] Failed (Execution error)" >> "$VALGRIND_LOG"
    ((TEST_NUMBER++))
    continue
  fi

  # Compare the computed solution with the expected solution
  DIFF=$(diff -q "$OUTPUT_SOLUTION" "$EXPECTED_SOLUTION")

  if [ -z "$DIFF" ]; then
    echo "Test $TEST_NUMBER - [$INPUT_MATRIX, $INPUT_RHS] Passed" >> "$VALGRIND_LOG"
  else
    echo "Test $TEST_NUMBER - [$INPUT_MATRIX, $INPUT_RHS] Failed (Mismatch)" >> "$VALGRIND_LOG"
  fi

  ((TEST_NUMBER++))
done

echo "All tests completed. See $VALGRIND_LOG for detailed results and $CPPCHECK_LOG for Cppcheck analysis."
