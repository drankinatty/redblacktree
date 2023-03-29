# redblacktree
C Red-Black Tree Handling Both Internal and External Storage of Data

This implementation of a redblack tree written in C was adapted from MIT by Apple as part of the its opensource implementation of [sudo-73](https://opensource.apple.com/source/sudo/sudo-73/src/) and has been further refined to allow storage for the data used by the tree to either be an existing collection of objects (external storage) or this implementation will allocate storage a copy the data into the tree (internal storage). The implementation was further modified to be C89 compliant and is compliant with all language standards that follow.

**Internal and External Modes Explained**

For external storage, a pointer to the external data is assigned to the `rbnode->data` member. For internal storage, the implementation will allocate a specified number of bytes and copy the data to the `node->data pointer`. To control what storage scheme is used, if the `typesz` parameter to the `rbinsert()` function is non-zero, internal-storage is used. Passing `0` for the `typesize` parameter invokes the external-storage scheme and the pointer to the data provided is simply assigned.

To control how the tree frees, or refrains from freeing, the data, the `destroy` parameter to `rbdestory` is either `NULL` (external-storage, user responsible for freeing memory) or a function pointer is passed to a function that frees the memory as required (internal-storage). The function type for the destroy function is `void (*)(void *)`.

As a feature and convenience for freeing externally-stored data, the `rbdestroy()` function returns a pointer to the `node->data` member for the node that was removed. If the data object has allocated storage duration and is no longer required, you can simply pass the return to `free()`, e.g.

      free (rbdestroy (tree, NULL));

(**note:** the data object mush not be part of an allocated collection of objects, e.g. with an address in the middle of a larger allocated block)

**The redblack-test Program**

There is a test program provided that will exercise either internal or external storage depending on whether `EXTERNALSTRG` is defined. The test program `redblack-test.c` exercises each of the functions that make up the red-black tree implementation, filling the tree, searching, removing nodes and re-balancing as necessary. If `DEBUG` is defined, the output additionally includes the node-pointer and data member pointer addresses along with the color of each node (`red` or `black`).

By default the redblack-test program creates a 10-node tree using random numbers within a range of `0 <= ((10 * nnodes) - 1)`. It will accept the integer number of nodes (`nnodes`) to create as the first parameter. (since the argument conversion used is `atoi()`, any non-numeric value will result in an unexciting `0` node tree. The basic usage is:

    $ ./redblack [No. of Nodes (default 10)]

**Compiling**

See the top of `redblack-test.c` for compiling instructions for `gcc`. A basic compile string of the following will work fine:

    $ gcc -Wall -Wextra -pedantic -Wshadow -Werror -std=c11 -Ofast redblack.c -o redblack-test redblack-test.c

It compiles without issue on MinGW or VS (`cl.exe`).

**Feedback**

While this has been fairly well tested, if you find a problem, open an issue. If you have suggestions for improvement, author a pull-request. Good luck with your coding.
