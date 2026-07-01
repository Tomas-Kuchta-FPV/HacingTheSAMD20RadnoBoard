> For education purpuses only
> There is no real hacking! Only Observing and ReverseEngineering a PCB I bought.

So I wanted to do a bit of reverse ingeneering that's why this repo exist.  
![RE](/REtest.jpg)  
It was pretty fun and you can read more about the journey in JOURNAL.md  

It was a great learning experience and I've sharpened my skils or Revese engineering, which is even needed for profesional work as an engineer as you need to quicly understand a given probelem.  

About the board:  
This board was found on some flee market so I don't think you would be able to replicate it unforunately, it would be better to RE some more widely availible board - maybe one day :). Althought this repo might help you learning RE! and understanding my thought process.   

The journal might be unclear as I'm not a native english speaker and I didn't feel like withing anything lol.  


## Reverse Engineering
RE can be found in `ReverseEngineering.md`

## Building on linux (Specifically Fedora 43 KDE)

### Using MPLAB IDE

1. Install [JTAGprobe](https://github.com/lonehog/JTAGprobe) on yout raspberry pi pico
1. Open MPLAB
2. Open the project.x
2. Install OpenOCD with `dnf install openocd`
3. Wire it up as decribed in ReverseEngineering.md
4. Hit compile. The little hammer or ctr+shift+b take not on the output `.elf`/`.hex` file
5. Flash! refer to *TOOLs Usage.md* Basicaly it's `openocd -f interface/cmsis-dap.cfg -f target/at91samdXX.cfg -c "program {link to binary} verify reset exit"`
6. And that's it

### With VS code and MPLAB extension - depreciated

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
Random board from flee market.
### SW
[JTAGprobe](https://github.com/lonehog/JTAGprobe)
[MPLAB into VS code](https://www.microchip.com/en-us/tools-resources/develop/mplab-tools-vs-code)
[OpenOCD](https://openocd.org/)
