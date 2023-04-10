# Lepton
Lepton is an educational set of quantum simulators designed to run on small devices.

Lepton's simulators are designed for small devices, with two key objectives.
Firstly, it aims to be didactic by providing complete access to all mathematical routines and simulation steps in its easy-to-read source code.
Secondly, it is optimized to run efficiently on small devices, including 8-bit computers. Keeping a small device as a baseline can help ensure that the code remains simple and efficient.

Although it is not intended to replace existing quantum simulation libraries, such as Qiskit or Pennylane, Lepton offers a unique approach to quantum simulation that is both accessible and efficient.

## Repository folders

* [**lepton**](/lepton): The source code of Lepton.

    * [**lib**](lepton/lib): Lepton library.

    * [**examples**](lepton/examples): Examples of how to program using the Lepton library.

    * [**simulators**](lepton/simulators): Fully implemented quantum simulators using the Lepton library.

    * [**tools**](lepton/tools): Auxiliary tools.

* [**make**](/make): Scripts for building Lepton.

* [**docs**](/docs): Documentation for both users and developers.

## How to build Lepton

Lepton requires Linux to be built. It should work on macOs too, but that hasn't been tested. If you are on Windows 10 or 11 you can use [WSL](https://learn.microsoft.com/en-us/windows/wsl/).

To build Lepton you'll need:

* `make` and `gcc`. On Debian-based systems you can just `apt-get install build-essential`.
* [SDCC](http://sdcc.sourceforge.net/) **v4.0.0 or newer**. On Debian-based systems you can just `apt-get install sdcc`.
* [z88dk](https://z88dk.org/) **v2.2 or newer**. On Linux systems you can visit https://github.com/z88dk/z88dk/wiki/installation#linux--unix.

There are two makefiles that will take care of building Lepton for different targets. Once the tools are in place you can just `cd` to the appropriate directory and run `make`:

* `make/linux`: builds all binaries for Linux using `gcc` and copies them to the `bin/linux` directory.
* `make/cpm`: builds all binaries for CP/M (z80) using `zcc` (z88dk) and copies them to the `bin/cpm` directory.

## Authors
Lepton's first version was developed in a collaboration between [Quantum Data Science & AI lab](https://quantumpark.yonsei.ac.kr/) (also known as q-DNA) at Yonsei University and [Centro de Informática](https://portal.cin.ufpe.br) at UFPE.

If you are doing research using Lepton, please cite our project.
We use a [CITATION.cff](https://citation-file-format.github.io/) file, so you can easily copy the citation information from the repository landing page.

## License
Lepton is **free** and **open source**, released under the Apache License, Version 2.0.