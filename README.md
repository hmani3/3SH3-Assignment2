# Custom UNIX Shell

## Team Members and Contributions
This project was completed collaboratively, with the workload divided into two primary phases

* **Heeman:** Developed the first half of the assignment. This included implementing the main shell execution loop, handling input tokenization via `strtok`, managing concurrent background process execution (the `&` operator), and writing the core UNIX process management logic utilizing `fork()`, `execvp()`, and `wait()`.
* **Jason:** Developed the second half of the assignment. This focused on state management, specifically designing a `CommandHistory` struct using a circular buffer to efficiently store previous commands. Jason also implemented the built-in `history` command display logic and the `!!` recent-command execution feature.

## AI Usage Disclosure
Artificial Intelligence was utilized only as a reference tool during this project. **Only the specific areas listed below utilized AI assistance; all core program logic, architectural design, and final code implementation were written entirely independently by the team members.**

AI usage was strictly limited to:
1. **Troubleshooting Early Development:** AI was used to help diagnose and explain C-specific syntax errors, pointer type mismatches, and memory bound issues during the initial setup phases of the program.
2. **C Library Documentation:** AI functioned as a reference manual to clarify the exact behavior, argument typing, and edge cases of standard C library functions (e.g., `strcmp`, `strcpy`, `memset`) and UNIX system calls.
3. **README Generation:** The structural formatting and text of this specific `README.md` document were drafted with AI assistance.
