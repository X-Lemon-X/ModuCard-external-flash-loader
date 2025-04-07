# ModuCard-external-flash-loader
STM32H750  external memory loader W25Q128




## Debugging using St-link
1. Install OpenOCD
```bash
sudo apt install openocd stlink-tools 

```
2. Install arm-none-eabi toolchain  [linik](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads)
3. Add the toolchain to your PATH
4. You will probably need to install
```bash
sudo apt-get install libncurses5
```
5. Connect the ST-Link to your computer and the STM32H750 board
6. Add configuration to ```launch.json``` file in the .vscode directory
```json
  {
    "cwd": "${workspaceFolder}",
    "executable": "${workspaceFolder}/build/firmware.elf",
    "name": "Debug launch OpenOCD",
    "request": "launch",
    "type": "cortex-debug",
    "servertype": "openocd",
    "configFiles": [
      "${workspaceFolder}/stlink.cfg",
      "${workspaceFolder}/stm32h7x.cfg"
    ],
    "runToEntryPoint": "main",
    "showDevDebugOutput": "both",
    "svdFile": "${workspaceFolder}/STM32H750.svd",
    // "serialNumber": "xyz", // serial number of the ST-Link if you have multiple ST-Link devices connected
    "breakAfterReset": false
  },
  {
    "cwd": "${workspaceFolder}",
    "executable": "${workspaceFolder}/build/firmware.elf",
    "name": "Debug attach OpenOCD",
    "request": "attach",
    "type": "cortex-debug",
    "servertype": "openocd",
    "configFiles": [
      "${workspaceFolder}/stlink.cfg",
      "${workspaceFolder}/stm32h7x.cfg"
    ],
    "runToEntryPoint": "main",
    "showDevDebugOutput": "both",
    "svdFile": "${workspaceFolder}/STM32H750.svd",
    // "serialNumber": "xyz", // serial number of the ST-Link if you have multiple ST-Link devices connected
    "breakAfterReset": false
  }
```