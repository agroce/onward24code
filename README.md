This repository contains code and (simple) instructions for "following along" with the ideas
presented in the dialogue.

The easiest way to work with this code is to use a docker image with DeepState, CBMC (an old version, admittedly), and UniversalMutator already installed:

```
docker pull agroce/onward24_docker
docker run -it agroce/onward24_docker
cd ~/onward24code
```

The image already has mutants generated for `binsearch.c` in the `mutants` directory.  You can generate them yourself:

```
mutate binsearch.c --cmd "clang -c binsearch.c" --mutantDir mutants
```

This invokes the "bugginator" to produce over 80 likely-buggy versions of `binsearch.c`.  The `--cmd` option tells the tool to check that the mutant is valid C code by trying to compile it.

To check the code using cbmc, just type:

```
cbmc binsearch_cbmc.c binsearch.c --unwind 12 --bounds-check --pointer-check --unwinding-assertions
```

If you alter 'MAX_SIZE` you'll need to alter the unwinding depth, too.

Using DeepState is a bit more complex.  First you have to build the various test executables.  The included Makefile does this for you, however.  So, to run one test (the "default test") of binary search, you just type:

```
make
./test_binsearch
```

You should see something like:

```
TRACE: Running: Run_Bentley from binsearch_deepstate.cpp(10)
TRACE: binsearch_deepstate.cpp(16): SIZE = 1
TRACE: binsearch_deepstate.cpp(20): a[0] = 0
TRACE: binsearch_deepstate.cpp(25): Sorting...
TRACE: binsearch_deepstate.cpp(28): a[0] = 0
TRACE: binsearch_deepstate.cpp(35): k = 0
TRACE: binsearch_deepstate.cpp(36): present = 1
TRACE: binsearch_deepstate.cpp(39): r = 0
TRACE: Passed: Run_Bentley
```

To use DeepState's built-in (very dumb, not coverage-driven) fuzzer:

```
./test_binsearch --fuzz --timeout 30
```

You'll see something like:

```
INFO: Starting fuzzing
WARNING: No seed provided; using 1714058690
WARNING: No test specified, defaulting to first test defined (Run_Bentley)
INFO: Done fuzzing! Ran 1391568 tests (46385 tests/second) with 0 failed/1391568 passed/0 abandoned tests
```

Finally, to fuzz the binary search using AFL++, a very good coverage-driven mutation-based fuzzer:

```
deepstate-afl ./fuzz_binsearch -o fuzzing_output --fuzzer_out
```

Stop the fuzzer with Ctrl-C.

Finally, how do you check the mutants?

For cbmc, it's easy:

```
analyze_mutants binsearch.c "cbmc binsearch_cbmc.c binsearch.c --unwind 12 --bounds-check --pointer-check --unwinding-assertions" --timeout 600 --verbose --mutantDir mutants
```

This will take some time!

To look at the mutants not detected (thus possible holes in the specification or harness):

```
show_mutants unkilled.txt --mutantDir mutants
```

The same approach will work with DeepState:

```
analyze_mutants binsearch.c "make clean; make test_binsearch; ./test_binsearch --fuzz --timeout 15 --abort_on_fail" --timeout 20 --verbose --mutantDir mutants
```

Both CBMC and DeepState should detect more than 80% of the mutants as
faulty, suggesting we have a fairly strong specification.  DeepState's
results will vary, due to the nature of random value generation.  CBMC
should consistently detect almost 93% of  the mutants: when it is
possible, proof can be more powerful than testing.  The undetected
mutants in some cases are genuinely equivalent.  The one other case
converts binary search into a very strange linear search with
additional requirements, suggesting the omission of performance
testing is a serious issue with our approach.

The (undocumented, at least for now) code in the `advanced` directory
shows a start on trying to verify binary search, including
performance, for unbounded array sizes (up to index type size in any
case), thus covering the Bloch case also.

**ADDENDUM**

You can also try binary-based mutation using [MuttFuzz](https://github.com/agroce/muttfuzz).  One nice thing with this approach is that the problem of equivalent mutants --
mutants that change source code but not meaningful program semantics -- is much less of an issue when mutants all involve changing
binary-level jumps in a program.  Change a reachale jump instruction and you probably have a seriously different problem!

To play with this:

```
pip install muttfuzz --upgrade
muttfuzz "rm -rf fuzz; deepstate-afl ./fuzz_binsearch -o fuzz --timeout 30; ./fuzz_binsearch --input_test_files_dir fuzz/the_fuzzer/crashes/ --abort_on_fail && ./fuzz_binsearch --input_test_files_dir fuzz/the_fuzzer/queue/ --abort_on_fail && ./fuzz_binsearch --input_test_files_dir fuzz/the_fuzzer/hangs/ --abort_on_fail" ./fuzz_binsearch --score --time_per_mutant 80 --source_only_mutate binsearch --avoid_repeats --stop_on_repeat
```

The upgrade is there because muttfuzz is more of a work-in-progress than the other tools, so grabbing the latest before using it is a good idea.  This will actually run through all the binary-level jump mutants of the binsearch code.  If you are curious about exploring the mutants more, create a directory and add `--save_mutants <dir>` to the muttfuzz arguments.
