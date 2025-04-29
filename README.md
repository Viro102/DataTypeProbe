# DataTypeProbe: Abstract Data Type Test Library

DataTypeProbe is a C++23 project that provides:

1. A collection of C++ concepts defining interfaces for common Abstract Data Types (ADTs) like List, Queue, Stack, Priority Queue, and Table.
2. A testing framework built using Catch2 to verify if custom data structure implementations conform to these ADT concepts.

The primary goal is to allow developers to test their own ADT implementations against a predefined set of interface requirements and behaviors.

## Directory Structure

```plain
.
├── CMakeLists.txt
├── LICENSE
├── README.md
├── include
│   ├── concepts
│   │   ├── generic_concepts.hpp
│   │   ├── list_concepts.hpp
│   │   ├── priority_queue_concepts.hpp
│   │   ├── queue_concepts.hpp
│   │   ├── stack_concepts.hpp
│   │   └── table_concepts.hpp
│   ├── list_tests.hpp
│   ├── priority_queue_tests.hpp
│   ├── queue_tests.hpp
│   ├── stack_tests.hpp
│   ├── table_tests.hpp
│   └── utility.hpp
└── tests
    ├── CMakeLists.txt
    └── example.cpp
```

## Dependencies

* **CMake:** Version 3.25 or higher.
* **C++ Compiler:** Must support C++23 (Clang recommended).
* **Catch2:** Version 3.8.1 (automatically downloaded via FetchContent in CMake).

## Building the Project

Use the standard CMake workflow:

1. **Create a build directory:**

    ```bash
    mkdir build
    cd build
    ```

2. **Configure the project:**

    ```bash
    cmake ..
    ```

3. **Build the project (including tests):**

    ```bash
    cmake --build .
    # or use make, ninja, etc. depending on your generator
    ```

This will build the library and the example test executable `example_usage` inside the `build/tests` directory.

## Usage Example

To test your own data structure implementation against one of the ADT interfaces:

1. **Include your data structure definition** in `tests/example.cpp` or include its header.
2. **Define the `TESTS_LIST` macro** in `tests/example.cpp` as a `std::tuple` containing the type(s) you want to test. You can include standard library types for comparison.

    ```cpp
    // Example for testing a custom vector and std::vector against List concepts
    #include "path/to/your/custom_vector.hpp" // Include your implementation
    #include <vector>
    #include <tuple>

    using MyTypes = std::tuple<custom_vec<int>, std::vector<int>>;
    #define TESTS_LIST MyTypes
    ```

3. **Include the corresponding test header** file for the ADT you want to test.

    ```cpp
    // Example: Include tests for the List ADT
    #include <list_tests.hpp>
    ```

4. **Build the project** using CMake as described in the "Building the Project" section.
5. **Run the tests** by executing the compiled binary (in this example [tests/](./tests/CMakeLists.txt)):

    ```shell
    ./build/tests/example_usage <optional-args>
    ```

The test output will indicate whether your implementation passes the tests defined for the specific ADT concepts.

## Optional command line arguments

You can pass arguments directly to the test executable to control test execution. Here are some common options:

* **List tests:** See available tests, tags, etc.
  * `--list-tests` or `-l`: List test case names.
  * `--list-tags` or `-t`: List tags used in tests.
* **Run specific tests:**
  * `[test name pattern]`: Run tests whose names match the pattern (wildcards `*` allowed).
  * `[tag pattern]`: Run tests matching the tag(s). Tags are enclosed in `[]`, e.g., `[list]` or `[queue][basic]`.
* **Control output:**
  * `--reporter <name>` or `-r <name>`: Change the output format (e.g., `compact`, `console`, `junit`).
  * `--success` or `-s`: Include successful assertions in the output.
  * `--break` or `-b`: Break into the debugger on test failure.
* **Combine options:** You can combine patterns, e.g., `./tests/adt_tests [list][access]` runs only tests tagged with both `[list]` and `[access]`.

**For a complete list of command-line options, refer to the official Catch2 documentation:**
<https://github.com/catchorg/Catch2/blob/devel/docs/command-line.md>
