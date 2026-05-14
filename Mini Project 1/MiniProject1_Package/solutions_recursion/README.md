Recursion-based solution

Build:

```bash
cd MiniProject1_Package/solutions_recursion
make
```

Run:

```bash
./calculator_recursion
```

Quick tests (non-interactive):

```bash
printf "x = 5\n" | ./calculator_recursion
printf "x = 5\ny = x + 3\n" | ./calculator_recursion
printf "x = cc + 1\n" | ./calculator_recursion  # undefined variable -> should print EXIT 1
```

Notes:
- This version uses recursive descent with recursive helpers for `term`/`expr` etc.
- It evaluates expressions numerically and prints results; useful as a basis for adding code generation.
