------------------------------------------------------------  
bug 1 (error occuring somewhere after calling parse)

stack trace (lldb)
```
* thread #1, queue = 'com.apple.main-thread', stop reason = signal SIGABRT  
    * frame #0: 0x0000000186434724 libsystem_kernel.dylib`__pthread_kill + 8  
    frame #1: 0x000000018646bc28 libsystem_pthread.dylib`pthread_kill + 288  
    frame #2: 0x0000000186379ae8 libsystem_c.dylib`abort + 180  
    frame #3: 0x000000018629ae28 libsystem_malloc.dylib`malloc_vreport + 908  
    frame #4: 0x000000018629e6e8 libsystem_malloc.dylib`malloc_report + 64  
    frame #5: 0x000000018628f0d4 libsystem_malloc.dylib`realloc + 328  
    frame #6: 0x0000000100002e08 chasm`scanLiterals(lexer=0x0000600000c04000, token=0x000000016fdff130) at templator_parlex.c:38:31  
    frame #7: 0x000000010000320c chasm`parse(lexer=0x0000600000c04000, parser=0x0000600000204100) at templator_parlex.c:107:17  
    frame #8: 0x0000000100002c40 chasm`prepare_input at chasm.c:34:5  
    frame #9: 0x0000000100002c88 chasm`main at chasm.c:44:12  
    frame #10: 0x0000000186113f28 dyld`start + 2236
```

scanLiterals might be causing the problem somehow

it seems the error is being caused by reallocation of token->value  
token->value wasn't malloc'ed when the transition from using tokenizer elements began  
------------------------------------------------------------ (resolved, see #95531b4)  

------------------------------------------------------------  
bug 3 (segmentation fault occuring somewhere)

output (lldb)
```
    * thread #1, queue = 'com.apple.main-thread', stop reason = EXC_BAD_ACCESS (code=1, address=0x60006c6d7468)  
        frame #0: 0x0000000100003670 chasm`freeParser(parser=0x0000600000201220) at templator_parlex.c:165:18  
      162 	        int childSize = parser->nodes[i].childSize;  
      163
      164 	        for (int j = 0; j < attrSize; j++) {
      -> 165 	        free(parser->nodes[i].attributes[j]);  
      166 	            free(parser->nodes[i].value[j]);
      167 	        }
      168
      Target 0: (chasm) stopped.
````
the loop is (hypothetically) freeing each char* from attributes (char**)  

the loop bounds were incorrectly set, but upon fixing this a SIGABRT occurs:  
```
* thread #1, queue = 'com.apple.main-thread', stop reason = signal SIGABRT  
    * frame #0: 0x0000000186434724 libsystem_kernel.dylib`__pthread_kill + 8  
    frame #1: 0x000000018646bc28 libsystem_pthread.dylib`pthread_kill + 288  
    frame #2: 0x0000000186379ae8 libsystem_c.dylib`abort + 180  
    frame #3: 0x000000018629ae28 libsystem_malloc.dylib`malloc_vreport + 908  
    frame #4: 0x000000018629e6e8 libsystem_malloc.dylib`malloc_report + 64  
    frame #5: 0x00000001862aaf20 libsystem_malloc.dylib`find_zone_and_free + 308  
    frame #6: 0x00000001000037b0 chasm`freeNode(node=0x0000600000008010) at templator_parlex.c:176:5  
    frame #7: 0x000000010000383c chasm`freeParser(parser=0x0000600000208040) at templator_parlex.c:186:9  
    frame #8: 0x0000000100002ce8 chasm`prepare_input at chasm.c:37:5  
    frame #9: 0x0000000100002d20 chasm`main at chasm.c:44:12  
    frame #10: 0x0000000186113f28 dyld`start + 2236
```
error was originating due to free'ing node->attributes, probably because each element of attributes was already free'd, but another SIGABRT remains  
```
* thread #1, queue = 'com.apple.main-thread', stop reason = signal SIGABRT  
    * frame #0: 0x0000000186434724 libsystem_kernel.dylib`__pthread_kill + 8  
    frame #1: 0x000000018646bc28 libsystem_pthread.dylib`pthread_kill + 288  
    frame #2: 0x0000000186379ae8 libsystem_c.dylib`abort + 180  
    frame #3: 0x000000018629ae28 libsystem_malloc.dylib`malloc_vreport + 908  
    frame #4: 0x000000018629e6e8 libsystem_malloc.dylib`malloc_report + 64  
    frame #5: 0x00000001862aaf20 libsystem_malloc.dylib`find_zone_and_free + 308  
    frame #6: 0x0000000100003830 chasm`freeParser(parser=0x0000600000201220) at templator_parlex.c:190:5  
    frame #7: 0x0000000100002cc8 chasm`prepare_input at chasm.c:37:5  
    frame #8: 0x0000000100002d00 chasm`main at chasm.c:44:12  
    frame #9: 0x0000000186113f28 dyld`start + 2236
```
this time it occured in freeParser, probably because parser->nodes was free'd. each individual node had been free'd within nodes, so there was no need to free this.  
suspicion was proven right. bug resolved #3d98d3e  
------------------------------------------------------------ (resolved, see #3d98d3e)  
## (24/07/23) `parse` in `templator_parlex.c` has been renamed to generateNodeTree to better match its purpose  
------------------------------------------------------------
bug 4 (parseTreeToHTML not iterating through node sequence correctly)
the cause of this bug is pretty obvious, as the node sequence is a tree-like structure
rather than a one-dimensional list. since there is always garaunteed to be only one root node
(html), iterating through each "node" results in only 1 iteration