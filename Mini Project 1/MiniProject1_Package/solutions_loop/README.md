Loop-based solution

Build:

```bash
cd MiniProject1_Package/solutions_loop
make
```

Run:

```bash
./calculator_loop
```

Quick tests (non-interactive):

```bash
printf "x = 5\n" | ./calculator_loop
printf "x = 5\ny = x + 3\n" | ./calculator_loop
printf "x = cc + 1\n" | ./calculator_loop  # undefined variable -> should print EXIT 1
```

Notes:
- This is the original loop-style parser/evaluator that builds an AST with tail recursion replaced by loops in `term_tail`/`expr_tail`.
- It currently evaluates expressions numerically (not emitting assembly). Use as a reference for converting to codegen.
