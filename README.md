# Lepton
Lepton is an educational collection of quantum simulators designed to run on small devices.

Lepton's simulators are designed for small devices, with two key objectives.
Firstly, it aims to be didactic by providing complete access to all mathematical routines and simulation steps in its easy-to-read source code.
Secondly, it is optimized to run efficiently on small devices, including 8-bit computers. Keeping a small device as a baseline can help ensure that the code remains simple and efficient.

Although it is not intended to replace existing quantum simulation libraries, such as Qiskit or Pennylane, Lepton offers a unique approach to quantum simulation that is both accessible and efficient.

## Importance of quantum computing simulators
Quantum computing simulators play a critical role in the development and implementation of practical quantum computing applications. These software programs simulate the behavior of quantum systems, providing researchers and engineers with a controlled environment to experiment with and validate quantum algorithms and circuits.

One of the key advantages of simulators is that they enable researchers to study the behavior of limited-sized quantum systems with high precision. This is particularly useful in the validation of quantum algorithms and protocols, where it is necessary to test and verify the behavior of complex quantum circuits in various scenarios and conditions. By enabling researchers to quickly iterate on their ideas and validate them efficiently, simulators accelerate the development of practical quantum computing applications.

Simulators are also valuable for training and educating students and researchers in quantum computing. They provide a virtual environment where students and researchers can experiment and learn practical skills without the need for physical and expensive resources. As such, simulators are essential tools for building the next generation of quantum computing experts.

However, it is important to note that quantum computing simulators have limitations and cannot completely replicate the behavior of real quantum systems. As such, it is crucial to validate the results obtained using simulators by conducting experiments on real quantum hardware. This is necessary to ensure the reliability and accuracy of the final results. The combination of simulators and real quantum systems will likely be used in tandem in the future, allowing researchers to efficiently validate and optimize quantum circuits while leveraging the unique capabilities of real quantum systems.

## Repository folders
* [**lepton**](/lepton): The source code of Lepton.

    * [**lib**](lepton/lib): Lepton library.

    * [**examples**](lepton/examples): Examples of how to program using the Lepton library.

    * [**simulators**](lepton/simulators): Fully implemented quantum simulators using the Lepton library.

    * [**tools**](lepton/tools): Auxiliary tools.

* [**make**](/make): Scripts for building Lepton.

* [**docs**](/docs): Documentation for both users and developers.

## How to build Lepton
Lepton requires Linux to be built, although it may also work on macOS (though this hasn't been tested). For those using Windows 10 or 11, [WSL](https://learn.microsoft.com/en-us/windows/wsl/) can be used as an alternative.

To build Lepton, you will need the following tools:

* `make` and `gcc`. On Debian-based systems, you can install them with `apt-get install build-essential`.
* [SDCC](http://sdcc.sourceforge.net/) **v4.0.0 or newer**. On Debian-based systems, you can install it with `apt-get install sdcc`.
* [z88dk](https://z88dk.org/) **v2.2 or newer**. To install it on Linux systems, please follow the instructions provided in https://github.com/z88dk/z88dk/wiki/installation#linux--unix.

To build Lepton, you can use one of the two provided makefiles, depending on your target system. Once you have installed the required tools, simply navigate to the relevant directory and run `make`. Here are the makefiles available:

* `make/linux`: builds all binaries for Linux using `gcc` and copies them to the `bin/linux` directory.
* `make/cpm`: builds all binaries for CP/M (z80) using `zcc` (z88dk) and copies them to the `bin/cpm` directory.

## Authors
Lepton's first version was developed in a collaboration between [Quantum Data Science & AI lab](https://quantumpark.yonsei.ac.kr/) (also known as q-DNA) at Yonsei University and [Centro de Informática](https://portal.cin.ufpe.br) at UFPE.

If you are doing research using Lepton, please cite our project.
We use a [CITATION.cff](https://citation-file-format.github.io/) file, so you can easily copy the citation information from the repository landing page.

## License
Lepton is **free** and **open source**, released under the Apache License, Version 2.0.