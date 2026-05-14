# Minimal Reference Solution - Assembly Calculator

A simple, memorizable compiler that converts arithmetic expressions into assembly code.

## Key Concepts

### Lexer (lex.c/h)
- Recognizes: integers, identifiers, operators (+, -, *, /, &, |, ^, ++, --, +=, -=, =, (, ))
- Multi-char operators: ++, --, +=, -= are recognized before single chars
- Identifiers: start with letter or underscore, can contain digits and underscores
- Invalid: identifiers starting with digit produce UNKNOWN token

### Parser (parser.c/h)
- **Symbol Table**: Tracks variable names, values, addresses, and defined status
  - Built-ins: x, y, z at addresses 0, 4, 8 (already defined)
  - New variables: added when first appearing on LHS of assignment
  
- **Grammar** (recursive descent, left-associative):
  ```
  assign_expr := ID ASSIGN assign_expr | ID ADDSUB_ASSIGN assign_expr | or_expr
  or_expr := xor_expr (OR xor_expr)*
  xor_expr := and_expr (XOR and_expr)*
  and_expr := addsub_expr (AND addsub_expr)*
  addsub_expr := muldiv_expr ((+|-) muldiv_expr)*
  muldiv_expr := unary_expr ((* |/) unary_expr)*
  unary_expr := (+ |-) unary_expr | factor
  factor := INT | ID | INCDEC ID | LPAREN assign_expr RPAREN
  ```
  
- **Semantic Checks**:
  - LHS of = or += must be ID
  - Variable must be defined before use (except on LHS of first assignment)
  - INCDEC operand must be ID
  - Divide by constant zero → ERROR

### Code Generator (codeGen.c/h)
- **Register Allocation**:
  - r0 = x, r1 = y, r2 = z (reserved, mapped to memory [0], [4], [8])
  - r3-r7 = temporaries (simple stack-based allocator)
  
- **Assembly Emission**:
  - Each AST node generates code that leaves result in a register
  - Binary ops: genCode(left) → r1, genCode(right) → r2, emit OP r1, r2
  - Assignments: genCode(rhs) → rX, emit MOV target, rX
  
- **Program Structure**:
  ```
  MOV r0, [0]     // Load x from memory
  MOV r1, [4]     // Load y from memory
  MOV r2, [8]     // Load z from memory
  ... generated code ...
  EXIT 0          // Normal exit
  ```

## Building

```bash
cd /path/to/mini_solution_simple
make
```

## Running

```bash
./calculator
```

Type expressions followed by newline:
```
>> x = 5
MOV r0, [0]
...
EXIT 0

>> y = x + 3
...
```

## Example Input/Output

**Input**: `x = 5 + 3`

**Output**:
```
MOV r0, [0]
MOV r1, [4]
MOV r2, [8]
MOV r3, 5
MOV r4, 3
ADD r3, r4
MOV r0, r3
EXIT 0

Total clock cycles: 100
```

## Error Handling

Errors produce:
```
EXIT 1
```

Examples:
- `x = undefined_var` → ERROR (variable not defined)
- `x = y / (2 - 2)` → ERROR (divide by constant zero)
- `5 = x` → ERROR (lvalue required)

## Simplifications vs Full Solution

- Register allocation is simple (no optimization)
- Memory loads counted as 10cc (simplification; spec says 200cc)
- No advanced optimizations (load/store elimination, etc.)

## Key Files to Memorize

1. **lex.c**: Multi-char operator handling (++, --, +=, -=)
2. **parser.c**: Full grammar with semantic checks
3. **codeGen.c**: Register allocation loop (`allocReg`, `freeReg`)
