# CCI SystemC Example

This project provides a practical example of how to integrate and use the **Configuration, Control, and Inspection (CCI)** library within a **SystemC** environment.

## Prerequisites

Before building this project, ensure that both **SystemC** and **CCI** are built and installed in the same directory.
Please refer to the installation manual of [SystemC][2] and [CCI][3] on how to install the libraries.

## Building the Project

To build the example application, follow these steps:

1. Set the `CMAKE_PREFIX_PATH` environment variable to the installation directory of SystemC and CCI (if they are not installed as system libraries).
2. Run the following commands from the root directory of this project:

```sh
CMAKE_PREFIX_PATH=<PATH_TO_SYSTEMC> cmake -B BUILD .
cmake --build BUILD
```

## Running the Example
After a successful build, the compiled example application will be located at:
```sh
BUILD/cci-example
```

## Resources

* [SystemC homepage][1]
* [SystemC Installation Manual][2]
* [CCI Installation Manual][3]

[1]: https://systemc.org
[2]: https://github.com/accellera-official/systemc/blob/main/INSTALL.md
[3]: https://github.com/accellera-official/cci/blob/main/INSTALL.md
