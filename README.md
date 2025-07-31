# 🧠 Philosopher (Dining Philosophers in C)

A C implementation of the classic **Dining Philosophers** concurrency problem using **POSIX threads** and **mutexes**. This project simulates philosophers sitting at a table who alternate between eating, sleeping, and thinking --- ensuring no deadlock or starvation.

---

## 📁 Project Structure

```

├── helpers.c # Utility functions: time, error checking, argument parsing

├── init.c # Initializes philosophers, mutexes, and shared data

├── main.c # Entry point: argument validation and start-up

├── Makefile # Compilation script

├── monitor.c # Monitors philosopher death or completion

├── philo.h # Struct definitions and function prototypes

├── print.c # Thread-safe status messages

├── routine.c # Core philosopher routine: think, eat, sleep

```

---

## 📌 Problem Description

**Dining Philosophers** is a synchronization problem that illustrates challenges in avoiding deadlock and starvation in concurrent programming.

Each philosopher:

- **Thinks 🤔**

- **Picks up forks (mutexes) 🍴**

- **Eats 🍝**

- **Sleeps 😴**

They need two forks to eat. A fork is shared between adjacent philosophers.

---

## ⚙️ Compilation

Make sure you're in the `philo` directory and run:

```

make

This will create an executable called philo.

To clean the build:


make clean

To remove all built files:

make fclean

To recompile from scratch:

make re

▶️ Running the Program

./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]

Parameters

number_of_philosophers: Number of philosophers (and forks)

time_to_die: Time (in ms) before a philosopher dies if they don't start eating

time_to_eat: Time (in ms) a philosopher spends eating

time_to_sleep: Time (in ms) a philosopher spends sleeping

number_of_times_each_philosopher_must_eat (optional): If set, the simulation stops when all philosophers have eaten at least this many times

Example

./philo 5 800 200 200 3

Simulates:

5 philosophers

800ms before dying if they don't eat

200ms eating

200ms sleeping

Ends after each philosopher eats 3 times
```
🧠 Philosopher Lifecycle

Each philosopher thread follows this cycle:

Think

Take left fork (mutex)

Take right fork (mutex)

Eat

Release both forks

Sleep

The cycle continues until death or completion condition is met.

A separate monitor thread checks for:

Death due to starvation

Completion if number_of_times_each_philosopher_must_eat is specified

🧵 Concurrency Details

Each philosopher is a pthread_t

Forks are pthread_mutex_t (one per philosopher)

Print operations are synchronized with a mutex

Time is tracked with gettimeofday() or clock_gettime()

📦 Output Example

```

0 1 is thinking

1 2 is thinking

2 1 has taken a fork

3 1 has taken a fork

4 1 is eating

5 2 has taken a fork

600 3 died
```
Each line:
```
<timestamp_in_ms> <philosopher_id> <action>
```
🚫 Deadlock Prevention Strategy

To avoid deadlock:

Philosophers take forks in a defined order based on index (even/odd strategy)

Shared mutexes ensure safe access to forks and logs

🧪 Testing Tips

Try fewer philosophers (1 or 2) to see starvation quickly

Set number_of_times_each_philosopher_must_eat for finite simulations

Use tools like valgrind to detect memory leaks:
```
valgrind ./philo 5 800 200 200 3
```
