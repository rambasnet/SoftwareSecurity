# x86 Systems, Assembly and Stack
http://www.cs.virginia.edu/~evans/cs216/guides/x86.html

## x86 Processor
- modern 80x86 (x=1, 2, 3, 4) have several registers
- in gdb, `info registers` command shows all general purpose registers
    
## Endianness
- is the order of sequence of bytes of a digital data stored in computer memory
- two types: Big-Endian and Little-Endian
- a big-endian stores the **most significant byte** of a word at the smallest memory address and the most significant byte of a word at the largest in 4-byte (32-bit) memory address
- a little-endian stores the **least significant byte** of a word at the smallest memory address and the most significant byte of a word at the largest
- x86 is Little-Endian architecture
- endianness affect mostly the numbers
- the following figure depicts how a hex representation of integer is stored in the two endianness

![endianess](./media/endianness.png)

## Registers
- variables to the CPU
- 8 32-bit general-purpose registers
- **EAX, ECX, EDX and EBX** are general-purpose registers
    - Accumulator, Counter, Data and Base respectively
    - used for a variety of purposes
    - act as variables while instructions are executing
- **ESP and EBP** are pointers
    - Stack Pointer, Base Pointer, Source Index and Destination Index, respectively
    - ESP and EBP store 32-bit addresses (point to location in memory)
    - ESP always points to the top of the stack
    - EBP acts as the base address (point of reference) for accessing local variables and parameters
- **ESI and EDI** are indices
    - technically pointers used to point to the source and destination when data is being copied

### EIP register
- Instruction Pointer register
- points to the current instruction the processor is reading
- very important pointer and will be used a lot while debugging

### EFLAGS
- EFLAGS registers contain bit flags used for comparisons and memory segmentations (stack, heap, data, bss, etc.)
- we'll ignore these as they're rarely accessed

## Assembly Language

### Intel Instruction Syntax:
```
operation <destination>, <source>
```
- destination and source values are either a register, a memory address, or a value
- the operations are usually intuitive mnemonics

- **mov**: operation will move a value from the source to the destination
    - mov ebp, esp
        - move the value from ESP to EBP
- **push**: places its operand onto the top of the stack
    - decrements ESP by 4 (stack grows towards lower address)
    - place the contents at that location
    - `push <reg>`
    - `push <mem>`
- **pop**: remove 4-byte data element from the top of the stack
    - move the top of the stack to a specified register or memory location
    - increments ESP by 4
    - `pop <reg32>`
    - `pop <mem>`

- **cal, ret**: subroutine call and return

- sub, add, cmp, jmp, and, or, xor, inc, etc.

## Calling Convention and Stack
- local variables, parameters, return address and many other values are pushed on to the stack when function is called
- let's say we've a C++ function
```c
void function(int arg1, int arg2, int arg3) {
    int variable1, variable2, variable3;
    // other code
}
```

## caller rules
```c
function(10, 20, 30);
```

1. save contents of certain registers (EAX, ECX, EDX)
    - values are pushed on to the stack if they're needed and restored upon return
- pass parameters to the subroutine (function)
    - push them on to the stack before the call
    - parameters are pushed in inverted order (i.e. last parameter first)
    - the first parameter is stored at the lowest address
        - technique allows to pass variable number of parameters
- push the return address to the top of the parameters
- use the `call` instruction to call the function at the location


## callee rules
1. push the EBP (value of the caller) on to the stack

```
push ebp
mov ebp, esp
```
2. push the local variables
- allocate local variables by pushing them on to the stack top to bottom 
- the first local variable pushed on the stack will be at -4 offset

## Note: return address of the caller is at EBP + 4
![stack calling convention](./media/stack-convention.png)
