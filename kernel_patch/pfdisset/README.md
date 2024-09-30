# pfdisset

This is a simple wrapper program that can be used to run another program with prefetching permanently disabled. The tool works similar to `taskset`. It sets the `prefetch_disable` flag and then starts the target application.

## Compiling

Use CMake to build the program:

```
cmake -Bbuild .
make -C build
```

The compiled binary will be located at `build/pfdisset`.

## Usage (example)

To call the target program `./my_binary` with the command line flag `--myflag=1` and prefetching disabled, run:

```
build/pfdisset -d ./my_binary --myflag=1
```