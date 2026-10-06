# Custom UNIX Shell

## Team Members and Contributions
This project was completed collaboratively, and we had split the work into 2 distinct parts. 

* **Heeman:** Developed the first half of the assignment. This meant implementing the main shell execution loop, handling input tokenization through `strtok`, then managing concurrent background process execution (the `&` operator), and finally coding the UNIX process management logic using `fork()`, `execvp()`, and `wait()`.
* **Jason:** Developed and worked on the second part of this assigment, which was primarily focused on state management, specifically designing a `CommandHistory` struct using a circular buffer which can efficiently store previous commands. He also had implemented the built-in `history` command display logic and the `!!` recent-command execution feature in the code.

## AI Usage Disclosure
Generative AI was not used to generate or implement any portion of the submitted code.
