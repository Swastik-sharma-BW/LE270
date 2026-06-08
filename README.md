# le270
This is eigencom based SoC as an alternative for mc661

# Build Steps
Note: This SDK can only be compiled on Linux based system

1. Install WSL(for windows)
2. Install python version >3.6
3. Run python3 launch.py and select 'LE270-CN-1D6W7-00-OPEN'
4. Output binary file: fibocom_opensrcSDK_12007_7000\gccout\ec7xx_0h00_ec718um\LE270-CN-1D6W7-00-OPEN\fibo_soft\fibo_soft.binpkg
5. For flashing use this tool: LE270-Files\LE270-Files\Fibocom_LE_Series_Multi_Upgrade_Tool_V4.1.12 (1)\Fibocom_LE_Series_Multi_Upgrade_Tool_V4.1.12\Tool\MulDownloader.exe
6. Connect cable to USB in EVK, press BOOT_USB and reboot the board, it will go to download mode
7. Select LE270-Files\Fibocom_LE_Series_Multi_Upgrade_Tool_V4.1.12 (1)\Fibocom_LE_Series_Multi_Upgrade_Tool_V4.1.12\Tool\config_pkg_product_usb.ini as Conf file
8. Select fibo_soft.binpkg as the Package file
9. For logger use this tool: LE270-Files\Fibocom_LE Series_Windows_EPAT_V1.1.0.3\Fibocom_LESeries_Windows_EPAT_V1.1.0.3\EPAT_V1.3.291.627\EPAT_V1.3.291.627\Bin\EPAT.exe
10. Use comdb.txt file as Database state 
11. Select last port and baudrate as 3000000 and click 'start' logs should appear
