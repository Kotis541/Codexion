*This project has been created as part of the 42 curriculum by vokotera*

# Description

Codexion is an advanced multithreaded simulation, exploring the complexities of process synchronization, shared memory management, and deadlock prevention. 

Inspired by the classic "Dining Philosophers" problem, this project simulates a high-pressure environment where multiple **Coders** must share limited hardware resources (**Dongles**) to execute their tasks. 

## Core Concept

A team of Coders sits around a circular table. Between each Coder is a single Dongle. To successfully compile their code, a Coder requires **two Dongles** (the one on their left and the one on their right). 

Each Coder loops through three specific states:
1. **Compiling:** Requires holding both Dongles.
2. **Debugging:** Requires no Dongles.
3. **Refactoring:** Requires no Dongles.

If a Coder goes too long without compiling, they "burn out" (die), and the simulation immediately halts.

## Blocking cases handled

This project addresses several critical concurrency issues to guarantee a stable simulation:
* **Deadlock prevention & Coffman’s conditions:** Coders follow an asymmetric odd/even locking sequence (even coders take the left dongle first, odd coders take the right dongle first). This breaks the circular wait condition.
* **Starvation prevention:** The implementation of a custom Scheduler utilizing a Min-Heap dynamically resolves starvation. Under EDF, coders closest to burnout are prioritized in the queue.
* **Cooldown handling:** After a dongle is released, a forced cooldown delay is handled using `pthread_cond_wait` with targeted `usleep`, effectively preventing immediate re-acquisition without CPU-heavy busy waiting.
* **Precise burnout detection:** A dedicated monitor thread continuously evaluates each coder's `last_compile_time`. Upon detecting a burnout, it locks the system and halts the simulation within milliseconds.
* **Log serialization:** The central logging mechanism is protected by a global mutex. It is locked the exact millisecond a death is detected, preventing any "ghost" outputs or interleaved text in the terminal.

## Thread synchronization mechanisms

The core of the simulation relies on POSIX threading primitives to coordinate access to shared resources efficiently:
* **`pthread_mutex_t`:** 
  * **Dongles:** Each dongle object contains its own `lock` mutex to protect its state (`is_taken`, `cooldown_end`) and its priority queue (`heap_data`).
  * **Monitor State:** A `time_mutex` inside each coder protects the `last_compile_time` and `compiles_done` variables. This strictly prevents **race conditions** when the independent monitor thread reads these values while the coder thread is simultaneously updating them.
  * **Logging & Global State:** A `log_mutex` ensures thread-safe console output, and a `dead_mutex` coordinates the `is_dead` flag globally across all threads.
* **`pthread_cond_t`:** 
  * Each dongle has a dedicated condition variable (`cond`). When a coder needs a dongle that is currently taken (or in cooldown), they enter a `pthread_cond_wait` state to preserve CPU resources.
  * Upon releasing a dongle, `pthread_cond_broadcast` is signaled to wake up all threads waiting for that specific dongle, allowing the scheduler to evaluate the heap and grant access to the thread with the highest priority.

## Schedulers

Codexion implements two distinct scheduling algorithms to manage the waiting queue for dongles:

* **FIFO (First-In, First-Out):** A strictly fair, ticket-based queue. Coders are granted access to dongles exactly in the order they requested them.
* **EDF (Earliest Deadline First):** A dynamic, priority-based urgent queue. The waiting room is constantly evaluated, and priority is explicitly given to the Coder who is closest to their burnout time, regardless of when they joined the queue.

## Instructions

The project utilizes standard POSIX threads and is compiled via `make`. It strictly adheres to standard C practices without relying on external libraries.

### Build
```bash
make
```

### Execution
```bash
./codexion [coders] [burnout] [compile] [debug] [refactor] [max_compiles] [cooldown] [scheduler]
```
- coders: Number of coders (and number of dongles).

- burnout: Time (in ms) a coder can survive without compiling.

- compile: Time (in ms) it takes to compile (requires 2 dongles).

- debug: Time (in ms) it takes to debug.

- refactor: Time (in ms) it takes to refactor.

- max_compiles: Number of times each coder must compile before the simulation ends successfully.

- cooldown: Forced waiting time (in ms) between actions.

- scheduler: The algorithm to use (fifo or edf).

### Resources

[youtube - intro to Threads](https://www.youtube.com/watch?v=d9s_d28yJq0&list=PLfqABt5AS4FmuQf70psXrsMLEDQXNkLq2)

[Multithreading in C](https://www.geeksforgeeks.org/c/multithreading-in-c/)

Other sources on internet ..

 - AI Usage: AI Assistant (Gemini) was utilized to help refactor the C codebase. It also provided architectural suggestions for safely implementing the Min-Heap structure for the scheduler, and assisted in debugging.

 - Not used for copy-paste without understanding.