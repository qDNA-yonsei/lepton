# Simulators

## State vector
Quantum state vector simulators are a type of quantum simulator that computes the state of a quantum system in terms of its wavefunction or state vector. They work by applying quantum gates and measurements to an initial state, which is usually a product state where all qubits are in the state |0⟩, and then computing the resulting state vector.

The state vector represents the probability amplitudes for each possible outcome of a measurement. For example, if a single qubit is in the state |ψ⟩ = α|0⟩ + β|1⟩, the state vector would be a 2-dimensional column vector with the first entry α and the second entry β. The probabilities of measuring 0 or 1 are given by the squared magnitudes of the amplitudes |α|^2 and |β|^2, respectively.

Quantum state vector simulators are useful for studying the behavior of quantum circuits and algorithms because they allow for exact calculations of the state vector. However, the size of the state vector grows exponentially with the number of qubits, which limits the size of the circuits that can be simulated. In practice, state vector simulators are typically used to simulate small circuits with up to around 30 qubits.

Several quantum state vector simulators are available, including [IBM's Qiskit Aer](https://qiskit.org/ecosystem/aer/stubs/qiskit_aer.AerSimulator.html), [Google's Cirq](https://quantumai.google/cirq/simulate), and [Microsoft's Q# quantum simulators](https://learn.microsoft.com/en-us/azure/quantum/machines/full-state-simulator).

[![State vector simulator running on an 8-bit computer](https://i.ytimg.com/vi/2BQv1TSv4n4/hqdefault.jpg)](https://youtu.be/2BQv1TSv4n4 "State vector simulator running on an 8-bit computer")

File:
[statevec.c](statevec.c)

Implemented gates:
U, P, RX, RY, RZ, ID, X, Y, Z, H, CX, CY, CZ, CCX

To be implemented:
U1, U2, U3, R, S, SDG, SX, T, TDG, SWAP, CSX, CP, CU1, CU2, CU3, RXX, RYY, RZZ, RZX, CSWAP, MCX, MCY, MCZ, MCSX, MCP, MCU1, MCU2, MCU3, MCRX, MCRY, MCRZ, MCR, MCSWAP, UNITARY, DIAGONAL