#!/bin/bash

echo "=== Philosophers Monitoring System Test ==="
echo

echo "Test 1: Normal simulation (should run for a few cycles)"
echo "Command: ./philo 3 800 200 200"
timeout 3s ./philo 3 800 200 200
echo -e "\n"

echo "Test 2: Death detection (philosopher should die)"
echo "Command: ./philo 2 300 200 200"
./philo 2 300 200 200
echo -e "\n"

echo "Test 3: Completion detection (all philosophers eat 2 times)"
echo "Command: ./philo 3 800 200 200 2"
./philo 3 800 200 200 2
echo -e "\n"

echo "Test 4: Single philosopher (should die - can't eat alone)"
echo "Command: ./philo 1 400 200 200"
./philo 1 400 200 200
echo -e "\n"

echo "=== All tests completed ==="