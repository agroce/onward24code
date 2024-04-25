This repository contains code and (simple) instructions for "following along" with the ideas
presented in the dialogue.

The easiest way to work with this code is to use a docker image with DeepState, CBMC (an old version, admittedly), and UniversalMutator already installed:

```
docker pull agroce/onward24_docker
docker run -it agroce/onward24_docker
cd onward24code
```

The image already has mutants generated for `binsearch.c` in the `mutants` directory.  You can generate them yourself:

```
mutate binsearch.c --cmd "clang -c binsearch.c" --mutantDir mutants
```

This invokes the "bugginator" to produce over 80 likely-buggy versions of `binsearch.c`.  The `--cmd` option tells the tool to check that the mutant is valid C code by trying to compile it.

To check the code using cbmc, just type:

```
cbmc binsearch_cbmc.c binsearch.c --unwind 12 --bounds-check --pointer-check
```

If you alter 'MAX_SIZE` you'll need to alter the unwinding depth, too.

Using DeepState is a bit more complex.  First you have to build the various test executables.  The included Makefile does this for you, however.  So, to run one test (the "default test") of binary search, you just type:
