# Risc-V
As illustrated below, Lepton is operational on a Litex Risc-V SoC implemented on the Tang Nano 20k FPGA board.

| Litex SoC configuration  | Linux on Litex           |
:-------------------------:|:-------------------------:
![Litex SoC configuration](images/riscv/litex.png "Litex SoC configuration") | ![Linux on Litex](images/riscv/linux_on_litex.png "Linux on Litex")

| Bell circuit diagram     | Bell simulation          |
:-------------------------:|:-------------------------:
![Bell circuit diagram](images/riscv/drafter_bell.png "Bell circuit diagram") | ![Bell simulation](images/riscv/statevec_bell.png "Bell simulation")

| MCNOT circuit diagram    | MCNOT simulation         |
:-------------------------:|:-------------------------:
![MCNOT circuit diagram](images/riscv/drafter_mcnot.png "MCNOT circuit diagram") | ![MCNOT simulation](images/riscv/statevec_mcnot.png "MCNOT simulation")

Linux on LiteX-VexRiscv:
https://github.com/litex-hub/linux-on-litex-vexriscv

Tang Nano 20k:
https://wiki.sipeed.com/hardware/en/tang/tang-nano-20k/nano-20k.html

# ARM
Lepton is also functional on the Raspberry Pi 3 Model B Rev. 1.2, demonstrating its flexibility across different platforms.

| System configuration     | 24-qubit GHZ state       |
:-------------------------:|:-------------------------:
![System configuration](images/arm/arm_uname.png "System configuration") | ![24-qubit GHZ state](images/arm/arm_statevec_ghz24.png "24-qubit GHZ state")

| Bell circuit diagram     | Bell simulation          |
:-------------------------:|:-------------------------:
![Bell circuit diagram](images/arm/arm_drafter_bell.png "Bell circuit diagram") | ![Bell simulation](images/arm/arm_statevec_bell.png "Bell simulation")

| MCNOT circuit diagram    | MCNOT simulation         |
:-------------------------:|:-------------------------:
![MCNOT circuit diagram](images/arm/arm_drafter_mcnot.png "MCNOT circuit diagram") | ![MCNOT simulation](images/arm/arm_statevec_mcnot.png "MCNOT simulation")

Raspberry Pi 3 Model B+:
https://www.raspberrypi.com/products/raspberry-pi-3-model-b-plus

GHZ state:
https://en.wikipedia.org/wiki/Greenberger%E2%80%93Horne%E2%80%93Zeilinger_state
