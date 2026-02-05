
---

# SimpleSH v1.0 🚀

**A High-Performance POSIX-Compliant Unix Shell**

SimpleSH is a systems-programming project developed on **Ubuntu 22.04 LTS** using the **C language**. It functions as a lightweight, robust command-line interpreter that manages hardware resources, process synchronization, and inter-process communication via the Linux Kernel.

---

## 🛠️ Project Overview

SimpleSH follows the **REPL (Read-Eval-Print Loop)** architecture. It provides a secure environment where every external command is executed in an isolated child process, ensuring that the parent shell remains stable even if an application crashes.

### **Core Capabilities:**

* **Process Isolation:** Implements a strict `fork-exec-wait` lifecycle to manage system resources.
* **Stream Redirection:** Full support for `<` (Input), `>` (Output Overwrite), and `>>` (Output Append).
* **Inter-Process Communication:** Advanced piping logic (`|`) using kernel-level buffers for command chaining.
* **Built-in Logic:** Native handling for shell-level commands like `cd` and `exit` that require parent-process execution.

---

## 🏗️ System Architecture & Logic

### **1. Process Management**

The shell utilizes the POSIX API to manage processes. When a command is entered, the shell duplicates itself using `fork()`. The child process loads the new binary via `execvp()`, while the parent process uses `waitpid()` to synchronize execution and prevent "Zombie Processes."

### **2. I/O Redirection**

By manipulating the **File Descriptor Table**, SimpleSH can reroute standard data streams. Using `dup2()` and specific file access flags (`O_TRUNC`, `O_APPEND`), the shell can divert output from the terminal to physical files or read input directly from files.

### **3. Inter-Process Communication (IPC)**

SimpleSH implements piping by creating a unidirectional data channel in the kernel. This allows the `STDOUT` of one process to be piped directly into the `STDIN` of another, enabling high-speed data transfer between independent programs.

---

## 💻 Skills Demonstrated

* **Systems Programming:** Low-level memory management, buffer handling, and pointer arithmetic in C.
* **Operating Systems:** Mastery of the POSIX API, Process Lifecycle, and File Descriptors.
* **Architecture:** REPL design pattern, Lexical Analysis, and Tokenization.
* **Performance:** Latency optimization (<20ms), resource leak prevention, and robust error handling.

---

## 📖 User Manual

### **Setup & Installation**

```bash
# Compile the shell with warnings enabled
gcc -Wall myshell.c -o myshell

# Run the shell
./myshell

```

### **Command Examples**

| Feature | Syntax | Example |
| --- | --- | --- |
| **List Files** | `ls -la` | List all files in detail |
| **Change Directory** | `cd [path]` | `cd /home/user/Documents` |
| **Save Output** | `[cmd] > [file]` | `hostname > output.txt` |
| **Append Data** | `[cmd] >> [file]` | `date >> output.txt` |
| **Pipe Commands** | `[cmd] | [cmd]` | `ls | grep ".c"` |
| **Exit** | `exit` | Terminate shell session |

---

