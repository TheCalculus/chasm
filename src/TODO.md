# FEATURES  
status | description | identifier  

- [ ] replace current DOM / nodetree system with `nodemode` (see [this](https://github.com/TheCalculus/nodemode))
- [ ] implement webassembly support for client-side execution of c code  
- [ ] make server multithreaded and more robust to support more than one user securely (or use a library to do this)  
- [ ] make it possible for chasm to work with any server (it just needs to be told when and where changes occur)
- [ ] remove reliance on tokenizer elements (Token) in templator_parlex.c  
- [ ] write parser `nodetree to HTML` converter
- [ ] implement reactivity and functionality to update only nodes that are directly effected by a change
- [ ] remove hash function from tokenizer in `templator_parlex.c`

# BUGS
status | title | identifier  

- [x] error occuring somewhere after calling parse (chasm.c), looking at stack trace (1)  
    ``` 
        chasm(30344,0x1e1445e00) malloc: *** error for object 0x1e144e5a0: pointer being realloc'd was not allocated  
        chasm(30344,0x1e1445e00) malloc: *** set a breakpoint in malloc_error_break to debug  
        build_run.sh: line 15: 30344 Abort trap: 6           ./chasm  
    ```

- [x] segmentation fault occuring potentially after resizing node->attributes (3)  
    ```
        node->attributes (char**) resized to 10  
        'li' == 'li' (end tag found)  
        node->attributes (char**) resized to 10  
        Token* resized to 125  
        'li' == 'li' (end tag found)  
        node->attributes (char**) resized to 15  
        build_run.sh: line 14: 31411 Segmentation fault: 11  ./chasm  
    ```
- [x] go through code analysis and fix potential bugs, perhaps implement optimizations (UPDATE: code analysis was identifying completely nonexistant / trivial issues).  
- [ ] parseTreeToHTML not iterating through node sequence correctly (4)
    read #4 in BUGS.md
- [ ] bug where iterateTokens reads values incorrectly and tokens as (null) but only within a few iterations (2)  
    resolve iterateTokens bug where tokens are not being correctly outputted.   
    probably an issue related to memory.  
