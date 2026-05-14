> For education purpuses only
> There is no real hacking! Only Observing and ReverseEngineering a PCB I bought.
## Building on linux (Specifically Fedora 43 KDE)

1. Setup Microchips [MPLAB into VS code](https://www.microchip.com/en-us/tools-resources/develop/mplab-tools-vs-code)
2. Open or Create a new project
3. Install [JTAGprobe](https://github.com/lonehog/JTAGprobe) on yout raspberry pi pico
4. Install OpenOCD with `dnf install openocd`
5. Wire it up as decribed in ReverseEngineering.md TODO
6. Hit compile. The little hammer or ctr+shift+b
7. Flash! refer to *TOOLs Usage.md* Basicaly it's `openocd -f interface/cmsis-dap.cfg -f target/at91samdXX.cfg -c "program out/{ProjectName}/default.elf verify reset exit"`

Output:
```shell
tomas@Debugie:~$ openocd -f interface/cmsis-dap.cfg -f target/at91samdXX.cfg -c "program /home/user/MPLABProjects/HacingTheSAMD20RadnoBoard/out/HacingTheSAMD20RadnoBoard/default.elf verify reset exit"
Open On-Chip Debugger 0.12.0
Licensed under GNU GPL v2
For bug reports, read
        http://openocd.org/doc/doxygen/bugs.html
Info : auto-selecting first available session transport "swd". To override use 'transport select <transport>'.
Info : Using CMSIS-DAPv2 interface with VID:PID=0x2e8a:0x000c, serial=4250305031363914
Info : CMSIS-DAP: SWD supported
Info : CMSIS-DAP: JTAG supported
Info : CMSIS-DAP: Atomic commands supported
Info : CMSIS-DAP: Test domain timer supported
Info : CMSIS-DAP: FW Version = 2.0.0
Info : CMSIS-DAP: Interface Initialised (SWD)
Info : SWCLK/TCK = 0 SWDIO/TMS = 0 TDI = 0 TDO = 0 nTRST = 0 nRESET = 0
Info : CMSIS-DAP: Interface ready
Info : clock speed 400 kHz
Info : SWD DPIDR 0x0bc11477
Info : [at91samd.cpu] Cortex-M0+ r0p1 processor detected
Info : [at91samd.cpu] target has 4 breakpoints, 2 watchpoints
Info : starting gdb server for at91samd.cpu on 3333
Info : Listening on port 3333 for gdb connections
[at91samd.cpu] halted due to debug-request, current mode: Thread 
xPSR: 0x21000000 pc: 0x0000019c msp: 0x200007b8
** Programming Started **
Info : SAMD MCU: SAMD20G18A (256KB Flash, 32KB RAM)
** Programming Finished **
** Verify Started **
** Verified OK **
** Resetting Target **
shutdown command invoked
```

> BTW I haven't had any succes with [PICO-DIRTYJTAG](https://github.com/phdussud/pico-dirtyJtag)

## Used tools
### HW
Raspberry pi pico (rp2040) (Also a rpi2 can be used)
Jumper wires
### SW
[JTAGprobe](https://github.com/lonehog/JTAGprobe)
[MPLAB into VS code](https://www.microchip.com/en-us/tools-resources/develop/mplab-tools-vs-code)
[OpenOCD](https://openocd.org/)
