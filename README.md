# Advent of code 2021

Advent of code 2021 solutions by Jon Vuri

### Build the project

    cmake --build ./build

For Visual Studio, give the build configuration (Release, RelWithDeb, Debug, etc) like the following:

    cmake --build ./build -- /p:configuration=Release


### Running the project

After building the project, run the solver executable (run it with no arguments to see usage):

```shell
./build/src/solver 1 src/01/part1/input.txt
```


### Running the tests

After building the project, run the doctest runner executable:

```shell
./build/unit_tests
```
