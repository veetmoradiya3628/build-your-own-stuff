
#### Scanning
- Scanning or Lexing (Lexical analysis)
- token

#### Parsing
- This is where our syntax gets a grammer - the ability to compose larger expressions and statements out of smaller parts.
- A parser takes the flat sequence of tokens and builds a tree structure that mirros the nested nature of the grammar. parse tree or abstract syntax tree
- parser job also includes reporting syntax errors

#### Static Analysis
- binding / resolution
- scope
- attributes on the syntax tree
- symbol table 
- Intermediate representation (IR)

#### Optimization
- Same semantics but optimized version of code

#### Code generation
- This step is converting user code into a form that the machine can actually run.
- virtual machine code - (p-code) - bytecode

#### Virtual machine
- A software layer that executes intermediate code (bytecode) for a specific language.
- creating a portable, platform-independent environment, like Java's JVM or Python's VM, allowing code to run anywhere by interpreting instructions and simulating hardware, providing abstraction and portability.

- Runtime
- Transpilers
- Just in time compilation

- Compiling
    - is an implementation technique that involves translating a source language to some other - usually lower-level form. when you generate bytecode or machine code, you are compiling. when you transpile to another high-level language you are compiling too.
- interpreter
    - we mean it takes in source code and executes it immediately. it runs programs from source.
    

#### Additional learnings :- 

- control flow graph
- static single-assignment
- continuation-passing style
- three-address code
- Code optimization techniques 
    - constant folding
    - constant propagation
    - common sub expression elimination
    - loop invariant code motion
    - global value numbering
    - strength reduction
    - scalar replacement of aggregates
    - dead code elimination
    - loop unrolling
- single-pass compilers
- Tree-walk interpreters
- syntax-directed translation
- transpiler - source to source compiler
