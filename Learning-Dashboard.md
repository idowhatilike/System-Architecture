# Systems Engineering — Learning Dashboard

Command center for tracking deliberate practice, mental model verification, and curriculum progression across the 18-month systems engineering roadmap.

---

## Mission

Build an uncompromising foundation in systems engineering through first-principles implementation, adversarial testing, and systematic debugging. Transition from consuming high-level abstractions to mastering the underlying physics of software: memory layouts, processor caches, operating system kernels, concurrency primitives, and durable network state.

---

## Current Position

| Parameter | Current Value |
| :--- | :--- |
| **Current Phase** | Phase 1: Toolchain, Memory, and Modern C++ Foundations |
| **Current Month** | Month 1 of 18 (Execution Window: Weeks 1–4) |
| **Current Topic** | Virtual memory layout, pointer arithmetic, memory alignment, and arena allocators |
| **Current Core Question** | How does user-space dynamic allocation interact with kernel page allocation (`brk`/`mmap`), and what is the exact cache and TLB cost of allocator fragmentation? |

---

## This Week

**Cadence:** Week 01 — Focus: Memory Layout, Alignment, and Arena Allocation

### Weekly Goals
- Map the ELF process address space into an empirical mental model via `/proc/[pid]/maps` and binary inspection tools.
- Implement a contiguous-memory Arena Allocator in Modern C++ (C++20) supporting arbitrary type alignment.
- Verify zero memory leaks and validate alignment guarantees under AddressSanitizer.

### Weekly Tasks
- [ ] Inspect an ELF binary layout using `readelf -S`, `objdump -d`, and `nm` to track text, data, bss, and rodata boundaries.
- [ ] Write a minimal program that inspects its own virtual memory mapping by parsing `/proc/self/maps`.
- [ ] Implement `ArenaAllocator` with bump-pointer allocation and RAII-based scope resets.
- [ ] Implement `align_forward(uintptr_t ptr, size_t alignment)` handling power-of-two alignment calculations.
- [ ] Stress-test the allocator with heterogeneous struct allocations and instrument with AddressSanitizer.

### Weekly Deliverables
- `experiments/memory-layout/inspect_elf.cpp`: Working binary layout introspection script.
- `projects/memory-allocator/arena_allocator.hpp`: Header-only, zero-overhead arena allocator.
- `projects/memory-allocator/tests/test_arena.cpp`: Unit test suite verifying alignment, boundary checks, and reset safety.
- Benchmark report comparing `std::malloc` vs. `ArenaAllocator` allocation latency for 100,000 small objects.

---

## Active Learning Resources

### Primary Texts
- **Computer Systems: A Programmer's Perspective (CS:APP3e)** — Bryant & O'Hallaron  
  *Active Chapters:* Chapter 1 (Tour), Chapter 3 (Machine-Level Representation), Chapter 9 (Virtual Memory).
- **The Linux Programming Interface (TLPI)** — Michael Kerrisk  
  *Active Chapters:* Chapter 6 (Processes), Chapter 7 (Memory Allocation), Chapter 49 & 50 (Memory Mapping).
- **Effective Modern C++** — Scott Meyers  
  *Active Items:* Items 1–4 (Deducing Types), Items 7–10 (Modern C++ Idioms), Items 23–25 (Move Semantics & Rvalue References).

### Documentation & Standards
- [POSIX.1-2017 Specification](https://pubs.opengroup.org/onlinepubs/9699919799/) — Memory management, system calls (`brk`, `sbrk`, `mmap`, `munmap`).
- [System V Application Binary Interface](https://gitlab.com/x86-psABIs/x86-64-ABI) — AMD64 Architecture Processor Supplement (Stack frame, register usage, alignment).
- [cppreference.com](https://en.cppreference.com/w/) — `std::align`, `std::byte`, value categories, memory models.

### Tooling Manuals
- GNU Debugger (GDB) Documentation (Memory examination `x/`, hardware watchpoints `watch`, layout registers `layout reg`).
- AddressSanitizer (ASan) & LeakSanitizer (LSan) Compiler Flags & Runtime Flags Manual.

---

## Understanding Tracker

### What Do I Understand?
- **Virtual Address Space vs. Physical Memory:** The OS presents each process with an isolated 64-bit address space. Addresses do not refer directly to physical RAM but to page table entries mapped by the MMU.
- **Data Alignment Mechanics:** CPUs load data most efficiently when word addresses are multiples of data sizes. Misaligned loads either cause hardware penalties or require multiple memory cycles and bit shifts.
- **Stack Frame Mechanics:** Return addresses, saved base pointers, and local variables are managed via `RSP`/`RBP` registers with predictable continuous push/pop overhead.

### What Can I Explain?
- Why standard `malloc()` prepends or stores chunk metadata, causing memory overhead for small allocations.
- The difference between internal fragmentation (unused space inside allocated block due to alignment/padding) and external fragmentation (free memory broken into disjoint chunks too small to satisfy requests).
- Why stack allocations are virtually free (pointer arithmetic on `RSP`) while heap allocations incur search, lock, and OS syscall overhead.

### What Can I Build?
- A bump allocator that claims a large contiguous buffer (via `mmap` or static array) and satisfies allocation requests in $O(1)$ time by advancing an offset.
- Bitwise alignment functions using two's complement arithmetic (`(ptr + align - 1) & ~(align - 1)`).
- Binary inspection tests that verify the memory layout of custom structs containing mixed-type members.

### What Still Confuses Me?
- **Kernel Lazy Page Allocation:** How many microseconds elapse between user-space calling `mmap(MAP_ANONYMOUS)` and the actual physical page allocation occurring upon the first write via minor page fault?
- **TLB Shootdowns:** What is the exact performance penalty of multi-core TLB invalidation when unmapping large memory segments in concurrent environments?
- **Allocator Metadata Protection:** How production allocators (e.g., jemalloc, mimalloc) guard metadata against buffer overflows without excessive performance cost.

---

## Debugging Journal

| Date | Problem | Hypothesis | Investigation | Result | Lesson Learned |
| :--- | :--- | :--- | :--- | :--- | :--- |
| 2026-10-08 | Segmentation fault during custom arena reset in stress test. | Pointer arithmetic overflow caused reset logic to write outside allocated region. | Ran executable under GDB; inspected `RSP`, `RBP`, and target memory address with `x/gx`. Used AddressSanitizer (`-fsanitize=address`). | Offset calculation was casting `char*` to `size_t` before adding alignment padding, leading to an underflow on boundary wrap. | Never perform pointer arithmetic on raw `void*` or unsigned integers without explicit boundary assertions. Use `std::byte*` and validate against `buffer_start + capacity`. |
| *YYYY-MM-DD* | *Upcoming* | *Pending* | *Pending* | *Pending* | *Pending* |

---

## Learning Evidence

### Notes Written
- [ ] `docs/memory-layout-amd64.md` — ELF layout, virtual address spaces, and kernel boundary mapping.
- [ ] `docs/alignment-and-padding.md` — Data alignment constraints, cache line boundaries, and struct packing rules.

### Experiments Completed
- [ ] `experiments/memory-layout/` — Empirical observation of stack growth downward vs. heap growth upward.
- [ ] `experiments/struct-padding/` — Measuring struct sizes across reordered member declarations.

### Projects Completed
- [ ] `projects/memory-allocator/` — Production-grade Arena Allocator with alignment support and unit test coverage.

### Labs & Benchmarks Completed
- [ ] `benchmarks/allocator-latency/` — Benchmarking allocation latency: `std::malloc` vs. `ArenaAllocator` across 1,000 to 1,000,000 iterations.

---

## Open Questions

1. **Memory:** When an application calls `free()`, under what exact threshold does `glibc` release memory back to the kernel via `brk()`/`sbrk()` versus keeping it in user-space bins for future reuse?
2. **Hardware:** What is the exact cycle count penalty of a 64-byte unaligned memory access that crosses a cache line boundary on modern x86_64 architectures?
3. **OS Interface:** How does the operating system schedule and handle minor page faults on huge pages (2MB/1GB) compared to standard 4KB pages?
4. **Toolchain:** How do link-time optimization (LTO) passes reorganize code layout across translation units to maximize instruction cache hit rates?

---

## Weekly Review

**Review Period:** Week 01 Retrospective (Scheduled: End of Week)

### What Went Well?
- *To be completed at week conclusion: Record specific systems concepts mastered and benchmarks completed ahead of schedule.*

### What Failed?
- *To be completed at week conclusion: Document experiments that yielded unexpected behavior, invalidated hypotheses, or took significantly longer than anticipated.*

### What Should Change Next Week?
- *To be completed at week conclusion: Technical workflow adjustments, deeper profiling instrumentation, or revisions to test harness design.*

---

## Phase Exit Gate Progress

**Phase 1 Gate Criteria: Toolchain, Memory, and Modern C++ Foundations**

```
Phase 1 Exit Readiness: [=====>-----------------------------] 16% Completed
```

| Evaluation Dimension | Standard Required for Phase Exit | Current Status | Evidence / Verification |
| :--- | :--- | :---: | :--- |
| **1. Knowledge** | Can explain without references: ELF segments, dynamic vs. static linking, virtual memory translation, page tables, cache hierarchies, struct layout, and C++ value categories. | In Progress | Drafted notes on virtual memory; pending verification against System V ABI documentation. |
| **2. Implementation** | Implement clean, reproducible C++20 allocators (Arena, Pool, Slab) with custom allocator traits conforming to modern standards. | In Progress | Arena allocator prototype underway in `projects/memory-allocator/`. |
| **3. Break** | Subject allocators to adversarial stress tests: deliberate misalignments, out-of-memory boundaries, zero-sized requests, and buffer overruns. | Pending | Stress test harness design drafted; fuzz testing queued. |
| **4. Debugging** | Diagnose memory corruptions, stack smashings, and use-after-free conditions autonomously using GDB, Valgrind, and Sanitizers within 15 minutes. | In Progress | First incident logged in Debugging Journal; ASan workflows integrated into CMake. |
| **5. Reasoning** | Formulate empirical, falsifiable hypotheses prior to running benchmarks; explain measured throughput/latency via hardware mechanics. | Pending | Microbenchmarks queued for Week 03. |
| **6. Transfer** | Apply custom memory allocation techniques to a non-trivial domain (e.g., parsing an AST or network packet buffer) and measure performance gains. | Pending | Scheduled for Month 3 capstone project. |
