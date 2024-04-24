Full instructions on running the code will be added shortly.

```
mutate binsearch.c --cmd "clang binsearch.c -c"
analyze_mutants binsearch.c "cbmc bentley_cbmc.c binsearch.c --unwind 12 --bounds-check --pointer-check" --timeout 80 --verbose
```