@echo off && setlocal enabledelayedexpansion
@echo %PATH% | findstr /c:"%~dp0tools/msys64/usr/bin">nul
@if %errorlevel% equ 1 set PATH=%~dp0tools/msys64/usr/bin;%PATH%
rem @set PATH=%~dp0tools/msys64/usr/bin;%PATH%
cls

#fibo_soft EC718UM OPENCPU LE230-CN-1S6AW5-99
echo "project-chiptype-opencpu-target :" %*

rem FORMAT CHIPTYE INFOMATION
set CHIP_TYPE=%2
for %%i in (a b c d e f g h i j k l m n o p q r s t u v w x y z) do call set CHIP_TYPE=%%CHIP_TYPE:%%i=%%i%%
set PKG_PRODUCT=%2_PRD
set CHIP=%CHIP_TYPE:~2%
set PKG_MAPDEF=pkg_%CHIP%_mapdef
echo **CHIP_TYPE %CHIP_TYPE%
echo **PKG_PRODUCT %PKG_PRODUCT%
echo **CHIP %CHIP%
echo **PKG_MAPDEF %PKG_MAPDEF%


set PROJECT_NAME=%1
set BOARD_NAME=ec7xx_0h00
set CHIP_NAME=ec7xx
set CORE_NAME=ap
set EUTRAN_MODE=cat_mode
set BUILD_OPTION=merge
set UNILOG=false
set TOOLCHAIN_NAME=GCC
set CUST=common
set SDKREL=false
set OPENCPU=false
set LTO_ENABLE=false
set BUILD_HEADBIN=false
set GCCLIB_PATH="C:\gcc"
set COMDBLIB_PATH=".\prebuild\PLAT\lib\gcc\%CHIP_TYPE%\full"
set FLASH_LD_DIR_OR=".\device\target\board\ec7xx_0h00\flash_ld\ec_original_718_flash_ld"
set CPBIN_SUBPATH=oc
set OUTPUT_NAME=%BOARD_NAME%_%CHIP_TYPE%
set JOBNUMBER=j8
set TTS_RES_EXIST=n
set pkg_cmd=
set ROM_ENABLE=false
set LESS_LOG=false
set libdir="oc"

if "%1" == "fibo_soft" if "%3" == "OPENCPU" (
set OPENCPU=true
)
echo OPENCPU %OPENCPU%

set "str=%2"
echo EC716S "!str:EC718S=!"
echo EC716E "!str:EC718S=!"
echo EC718S "!str:EC718S=!"
echo EC718P "!str:EC718P=!"
echo EC718PM "!str:EC718P=!"
echo EC718UM "!str:EC718UM=!"
::check Param string input

if "!str:EC716E=!" neq "%str%" (
	set LTO_ENABLE=true
	set GCF_ENABLE=false
	set BUILD_HEADBIN=false
	set RAM_ENBALE=true
	set COMDBLIB_PATH=".\prebuild\PLAT\lib\gcc\%CHIP_TYPE%\ram"
	set CPBIN_SUBPATH=ram
)

if "!str:EC716S=!" neq "%str%" (
	set LTO_ENABLE=true
	set GCF_ENABLE=false
	set BUILD_HEADBIN=false
	set LESS_LOG=true
	set COMDBLIB_PATH=".\prebuild\PLAT\lib\gcc\%CHIP_TYPE%\oc_lesslog"
	set libdir="oc_lesslog"
	set CPBIN_SUBPATH=ram
)

if "!str:EC718S=!" neq "%str%" (
	set LTO_ENABLE=true
	set GCF_ENABLE=false
	set BUILD_HEADBIN=false
	set LESS_LOG=true
	set COMDBLIB_PATH=".\prebuild\PLAT\lib\gcc\%CHIP_TYPE%\oc_lesslog"
	set libdir="oc_lesslog"
)

if "!str:EC718P=!" neq "%str%" if "!str:EC718PM=!" neq "%str%" (
	set COMDBLIB_PATH=".\prebuild\PLAT\lib\gcc\%CHIP_TYPE%\oc" 
	set CPBIN_SUBPATH=oc
)

if "!str:EC718U=!" neq "%str%" if "!str:EC718UM=!" neq "%str%" (
	set IMS_ENABLE=true
	set AUDIO_ENABLE=true
	set COMDBLIB_PATH=".\prebuild\PLAT\lib\gcc\%CHIP_TYPE%\ims"
	set CPBIN_SUBPATH=audio
	set libdir="ims"
)



echo build chiptype %CHIP_TYPE% OPENCPU %OPENCPU%  GCF_ENABLE %GCF_ENABLE%  OUTPUT_NAME %OUTPUT_NAME%
echo build.bat version 20180330
echo GCCLIB_PATH: %GCCLIB_PATH%  COMDBLIB_PATH %COMDBLIB_PATH%

set CPDIR=".\prebuild\FW\lib\gcc\%CHIP_TYPE%\oc"
set DEFDIR=%~dp0
echo CPDIR %CPDIR%

set BOOT2_EXIST=n
set TTS_RES_EXIST=n
set MID_EXIST=n

set target_dir="project\ec7xx_0h00\ap\apps\fibo_soft\GCC"
cd %target_dir%
for /f "delims=,/= tokens=1,2" %%a in (target.inc) do (
	rem echo %%a
	rem echo %%b
	echo %%a | findstr "CONFIG_FFW_TTS_ADD_RES"
	if not errorlevel 1 (	
		echo CONFIG_FFW_TTS_ADD_RES find.
		echo %%b | findstr "y"
		if not errorlevel 1 (
			echo TTS_RES_EXIST find.
			set TTS_RES_EXIST=y
		)		
	)
)

for /f "delims=,/= tokens=1,2" %%a in (target.inc) do (
	rem echo %%a
	rem echo %%b
	echo %%a | findstr "CONFIG_FFW_TWO_BOOTLOADER"
	if not errorlevel 1 (	
		echo CONFIG_FFW_TWO_BOOTLOADER find.
		echo %%b | findstr "y"
		if not errorlevel 1 (
			echo BOOT2_EXIST find.
			set BOOT2_EXIST=y
			set pkg_cmd=%pkg_cmd% -input  .\gccout\%OUTPUT_NAME%\ap\bootloader\ap_bootloader_part2.bin -addrname PKGFLX_BL2_PKGIMG_LNA -flashsize PKGFLX_BL2_PKGIMG_LIMIT_SIZE 
		)
		echo now pkg_cmd : [%pkg_cmd%]
	)
)

for /f "delims=,/= tokens=1,2" %%a in (target.inc) do (
	rem echo %%a
	rem echo %%b
	echo %%a | findstr "CONFIG_FIBO_EC716S_MID_ENABLE"
	if not errorlevel 1 (	
		echo CONFIG_FIBO_EC716S_MID_ENABLE find.
		echo %%b | findstr "y"
		if not errorlevel 1 (
			echo MID_EXIST find.
			rem set MID_EXIST=y
			set libdir="mid"
			set COMDBLIB_PATH=".\prebuild\PLAT\lib\gcc\%CHIP_TYPE%\mid"
			set MID_ENABLE=true
			set CPBIN_SUBPATH=wifi			
		)		
	)
)
set presetres=
for /F "tokens=2 delims==" %%a in ('findstr /C:"CONFIG_PRESET_RES"  target.inc') do (
set "presetres=%%a"
)

if not defined presetres (
	echo presetres is null
) else (
	rem  here presetres with "" before run stop 
	echo presetres is [%presetres%] not null
	%presetres:*"=set "presetres=%
	echo pre set resource : %presetres%
)

if not exist "gccout\" (
    mkdir gccout
)

if exist  fibocom/fibo-framework/build/tools/gen.py (
python3 fibocom/fibo-framework/build/tools/gen.py  $CUR_PATH/fibocom/fibo-framework/PAL/src/ec718/gtset/fibo_gtset.json $CUR_PATH/gccout/gtset_json.h
python3 ./tools/testcase_mk.py ./fibocom/testcase/ ./gccout/testcase_header.h
echo "exe gen.py ..."
)

set TTS_ORG_DIR=fibocom/fibo-framework/PAL/src/ec718/lib/tts/ivTTS_CE+PE_Templet.16K.irf
echo TTS_RES_EXIST:%TTS_RES_EXIST%
if %TTS_RES_EXIST%==y (
	set pkg_cmd=%pkg_cmd% -input %TTS_ORG_DIR% -addrname PKGFLX_FIBO_TTS_RES_LNA -flashsize PKGFLX_FIBO_TTS_RES_SIZE
) 
echo now pkg_cmd : [%pkg_cmd%]

echo presetres is %presetres%
if defined presetres (
	set pkg_cmd=%pkg_cmd% -input %presetres% -addrname PKGFLX_FIBO_CUST_RES_LNA -flashsize PKGFLX_FIBO_CUST_RES_SIZE 
)
echo now pkg_cmd : [%pkg_cmd%]
cd %DEFDIR%
echo CPBIN_SUBPATH %CPBIN_SUBPATH%

if not %GCCLIB_PATH% == "" (
   if not exist %GCCLIB_PATH% (
      echo ERROR: Please check GCCLIB_PATH setting, exit!!!
      goto end
   )
) else (
	echo ERROR: Please set GCCLIB_PATH firstly, exit!!!
	goto end
)

rem --------- board/project parsing begain------------------

set PARAMETERS=%1
if xx%PARAMETERS%==xx  (
  echo no input paramter, use default build setting
) else (
  echo build option  is: %PARAMETERS%
)

set parameterStr=%PARAMETERS%

:parameterSplit

for /f "tokens=1,* delims=-" %%a in ("%parameterStr%") do (
	
	set parameterStr=%%b
	
	echo %%a | findstr /r "j[0-9]*[0-9]\>" > nul
	if !errorlevel!==0 (
		set JOBNUMBER=%%a
	)

rem FIBOCOM_BASE info add ----BEGIN
	echo PARAMETERS: %PARAMETERS% | findstr "fibo_soft"
	if not errorlevel 1 (
		set PROJECT_NAME=fibo_soft
		set MANUFACTURER=FIBOCOM
	)

	echo PARAMETERS: %3 | findstr "OPENCPU"
	if not errorlevel 1 (
		set FIBOCOM_PKG_OPENSDK=true
		set OPENCPU=true
	)
	
	echo PARAMETERS: %* | findstr "audio_amr"
	if not errorlevel 1 (
		set COMDBLIB_PATH="./prebuild/PLAT/lib/gcc/%CHIP_TYPE%/oc"
		set IMS_ENABLE=false
		set AUDIO_ENABLE=true
		set CPBIN_SUBPATH=audio
	)
rem FIBOCOM_BASE info add ----END
	
	if "%%a"=="secboot" (
        set BUILD_HEADBIN=true
        for /f "tokens=1* delims=" %%a in ('type ".\project\%BOARD_NAME%\ap\apps\bootloader\GCC\Makefile"') do (
			if "%%a" equ "CONFIG_PROJ_APP_SECURITY_BOOT = n" (
				echo CONFIG_PROJ_APP_SECURITY_BOOT=y
			) else (
				echo %%a
			)
        )>>temp.txt

		type temp.txt | findstr /v ECHO >>temp1.txt
		del temp.txt
		move temp1.txt ".\project\%BOARD_NAME%\ap\apps\bootloader\GCC\Makefile" >nul
	)
)

if not "%parameterStr%"=="" goto parameterSplit
python opencpu.py api %CHIP_TYPE%

echo curr Board   is: %BOARD_NAME%
echo curr Chip    is: %CHIP_NAME%
echo curr Type    is: %CHIP_TYPE%
echo curr Project is: %PROJECT_NAME%
echo curr core    is: %CORE_NAME%
echo curr option  is: %BUILD_OPTION%
echo cp bin subpath:  %CPBIN_SUBPATH%
rem duplicate comdblib.txt from 'COMDBLIB_PATH' to '.\tools\'

set COMDBLIB_PATH=.\prebuild\PLAT\lib\gcc\%CHIP_TYPE%\oc
if exist %COMDBLIB_PATH%\comdblib.txt (
	xcopy /f /y %COMDBLIB_PATH%\comdblib.txt .\tools\
	rem xcopy /f /y .\prebuild\PLAT\lib\gcc\%CHIP_TYPE%\oc\comdblib.txt .\tools\
)

echo PARAMETERS: %PARAMETERS% | findstr "list"
if not errorlevel 1 (
   echo ---------------Supported Board------------------------------------
   for /d %%i in ("project\*") do (echo %%~nxi )

   echo ------------------------------------------------------------------

   for /d %%i in ("project\*") do (
   set CURRENT_BOARD=%%~nxi
   echo *********supported projects for board: !CURRENT_BOARD!*************
   for /d %%i in ("project\!CURRENT_BOARD!\ap\apps\*") do (echo ----%%~nxi )
   )

goto end
)

echo PARAMETERS: %PARAMETERS% | findstr "clall"
if not errorlevel 1 (
	make.exe -%JOBNUMBER%  clean-gccall TYPE=%CHIP_TYPE% TARGET=%BOARD_NAME% PROJECT=%PROJECT_NAME% CORE=%CORE_NAME% 
	echo clean all done ok...
	goto end
)

echo PARAMETERS: %PARAMETERS% | findstr "clean"
if not errorlevel 1 (
	make.exe -%JOBNUMBER%  clean-gcc TYPE=%CHIP_TYPE% TARGET=%BOARD_NAME% PROJECT=%PROJECT_NAME% CORE=%CORE_NAME%
	echo clean done ok...
	goto end
)

echo PARAMETERS: %PARAMETERS% | findstr "clobj"
if not errorlevel 1 (

	@del /s /q .\gccout\%OUTPUT_NAME%\%CORE_NAME%\%PROJECT_NAME%\*.o
	@del /s /q .\gccout\%OUTPUT_NAME%\%CORE_NAME%\%PROJECT_NAME%\*.a
	@del /s /q .\gccout\%OUTPUT_NAME%\%CORE_NAME%\%PROJECT_NAME%\*.elf
	echo clean objs done ok...
	goto end
)

set starttime=%time%
echo Start time: %date% %starttime%

if "%BUILD_OPTION%"=="merge" (
	if exist .\tools\comdblib.txt (
		echo start logprepass
		
		
	    echo > .failed.tmp
	    (make.exe -%JOBNUMBER% gccall TYPE=%CHIP_TYPE% TARGET=%BOARD_NAME% V=%VERBOSE% EUTRAN_MODE=%EUTRAN_MODE% PROJECT=bootloader CORE=%CORE_NAME% BUILD_UNILOG=false BUILD_CUST=%CUST% SDK=true  MANUFACTURER=%MANUFACTURER%  2>&1 && del .failed.tmp) | tee.exe .\gccout\%OUTPUT_NAME%\%CORE_NAME%\outbuildlog.txt

		if exist .failed.tmp (
		del .failed.tmp
		goto failHandle
		)
			
		
		
	    if exist .\tools\UpdateDBPattern.txt (
		del /q .\tools\UpdateDBPattern.txt
		)
		
		make.exe -%JOBNUMBER% build-unilog TYPE=%CHIP_TYPE% TARGET=%BOARD_NAME% PROJECT=%PROJECT_NAME% CORE=%CORE_NAME% BUILD_UNILOG=true BUILD_CUST=%CUST% SDK=true  MANUFACTURER=%MANUFACTURER% 
		
		
		rem this header file will compile with device code
		copy .\gccout\%OUTPUT_NAME%\ap\%PROJECT_NAME%\debug_log_ap.h .\middleware\developed\debug\inc\ || (goto:failHandle)
		if exist .\gccout\%OUTPUT_NAME%\ap\%PROJECT_NAME%\dbversion.h (
		copy .\gccout\%OUTPUT_NAME%\ap\%PROJECT_NAME%\dbversion.h .\middleware\developed\debug\inc\ || (goto:failHandle)
		)
		
	    echo > .failed.tmp
	   (make.exe -%JOBNUMBER% gccall TYPE=%CHIP_TYPE% TARGET=%BOARD_NAME% V=%VERBOSE% EUTRAN_MODE=%EUTRAN_MODE% PROJECT=%PROJECT_NAME% CORE=ap BUILD_UNILOG=true BUILD_CUST=%CUST% SDK=true  MANUFACTURER=%MANUFACTURER% 2>&1 && del .failed.tmp) | tee.exe .\gccout\%OUTPUT_NAME%\ap\outbuildlog.txt

	    if exist .failed.tmp (
	    del .failed.tmp
	    goto failHandle
	    )
		
		.\tools\fcelf.exe -T -bin .\gccout\%OUTPUT_NAME%\ap\%PROJECT_NAME%\ap_%PROJECT_NAME%.bin -size AP_PKGIMG_LIMIT_SIZE ^
								-bin .\prebuild\FW\lib\gcc\%CHIP_TYPE%\%CPBIN_SUBPATH%\cp-demo-flash.bin -size CP_PKGIMG_LIMIT_SIZE ^
								-bin .\gccout\%OUTPUT_NAME%\ap\bootloader\ap_bootloader.bin -size BOOTLOADER_PKGIMG_LIMIT_SIZE ^
								-h .\gccout\%OUTPUT_NAME%\ap\%PROJECT_NAME%\mem_map.txt
								
		if not errorlevel 0 (
			goto failHandle
		)
		
	    if "%BUILD_HEADBIN%"=="true" (
		    echo general headers
		    .\tools\ecsecure.exe APIMAGE=.\gccout\%OUTPUT_NAME%\ap\%PROJECT_NAME%\ap_%PROJECT_NAME%.bin ^
								 CPIMAGE=.\prebuild\FW\lib\gcc\%CHIP_TYPE%\%CPBIN_SUBPATH%\cp-demo-flash.bin ^
								 BLIMAGE=.\gccout\%OUTPUT_NAME%\ap\bootloader\ap_bootloader.bin ^
								 BLHASH=1 ^
								 SYSHASH=1 SYSBNADR=7e000^
								 HEAD1=.\gccout\%OUTPUT_NAME%\ap\%PROJECT_NAME%\bl_sec_header.bin ^
								 HEAD2=.\gccout\%OUTPUT_NAME%\ap\%PROJECT_NAME%\sys_sec_header.bin ^
								 BLPEM=.\project\%BOARD_NAME%\ap\apps\bootloader\code\main\pub_key_bl.pem ^
								 SYSPEM=.\project\%BOARD_NAME%\ap\apps\bootloader\code\main\pub_key_sys.pem

	        .\tools\fcelf.exe -M  -input .\gccout\%OUTPUT_NAME%\ap\bootloader\ap_bootloader.bin -addrname  BL_IMG_MERGE_ADDR -flashsize BOOTLOADER_FLASH_LOAD_SIZE ^
								  -input .\gccout\%OUTPUT_NAME%\ap\%PROJECT_NAME%\ap_%PROJECT_NAME%.bin -addrname  AP_IMG_MERGE_ADDR -flashsize AP_FLASH_LOAD_SIZE ^
								  -input .\prebuild\FW\lib\gcc\%CHIP_TYPE%\%CPBIN_SUBPATH%\cp-demo-flash.bin -addrname CP_IMG_MERGE_ADDR -flashsize CP_FLASH_LOAD_SIZE ^
								  -input .\gccout\%OUTPUT_NAME%\ap\%PROJECT_NAME%\bl_sec_header.bin -addrname  BLS_SEC_HAED_ADDR -flashsize BLS_FLASH_LOAD_SIZE ^
								  -input .\gccout\%OUTPUT_NAME%\ap\%PROJECT_NAME%\sys_sec_header.bin -addrname  SYS_SEC_HAED_ADDR -flashsize SYS_FLASH_LOAD_SIZE ^
								  -pkgmode 1  ^
								  -banoldtool 1 ^
								  -productname %PKG_PRODUCT% ^
								  -def .\device\target\board\%BOARD_NAME%\common\inc\%MEMMAP_NAME%.h ^
								  -outfile .\gccout\%OUTPUT_NAME%\ap\%PROJECT_NAME%\%PROJECT_NAME%.binpkg
            ) else (
	    .\tools\fcelf.exe -M  -input .\gccout\%OUTPUT_NAME%\ap\bootloader\ap_bootloader.bin -addrname BL_PKGIMG_LNA -flashsize BOOTLOADER_PKGIMG_LIMIT_SIZE  ^
		-input .\gccout\%OUTPUT_NAME%\ap\%PROJECT_NAME%\ap_%PROJECT_NAME%.bin -addrname AP_PKGIMG_LNA -flashsize AP_PKGIMG_LIMIT_SIZE ^
		-input .\prebuild\FW\lib\gcc\%CHIP_TYPE%\%CPBIN_SUBPATH%\cp-demo-flash.bin -addrname CP_PKGIMG_LNA -flashsize CP_PKGIMG_LIMIT_SIZE ^
		%pkg_cmd% ^
		-pkgmode 1  ^
		-banoldtool 1 ^
		-productname %PKG_PRODUCT% ^
		-def .\gccout\%OUTPUT_NAME%\ap\%PROJECT_NAME%\mem_map.txt ^
		-outfile .\gccout\%OUTPUT_NAME%\ap\%PROJECT_NAME%\%PROJECT_NAME%.binpkg
			)
		if not errorlevel 0 (
			goto failHandle
		)
		
		
		 if exist .\tools\UpdateDBPattern.txt (
		 type .\tools\UpdateDBPattern.txt
	     )

		 goto complete
	  	
	) else (
			
			echo > .failed.tmp
			(make.exe -%JOBNUMBER% gccall TYPE=%CHIP_TYPE% TARGET=%BOARD_NAME% V=%VERBOSE% EUTRAN_MODE=%EUTRAN_MODE% PROJECT=bootloader CORE=%CORE_NAME% SDK_REL=%SDKREL% BUILD_UNILOG=false BUILD_CUST=%CUST%  MANUFACTURER=%MANUFACTURER%  2>&1 && del .failed.tmp) | tee.exe .\gccout\%OUTPUT_NAME%\%CORE_NAME%\outbuildlog.txt

			if exist .failed.tmp (
			del .failed.tmp
			goto failHandle
			)
			
			
			
			echo start logprepass b22
			if exist .\tools\UpdateDBPattern.txt (
			del /q .\tools\UpdateDBPattern.txt
			)
			
			echo > .failed.tmp
			(make.exe -%JOBNUMBER% build-unilog TYPE=%CHIP_TYPE% TARGET=%BOARD_NAME% V=%VERBOSE% PROJECT=cp_project CORE=cp SDK_REL=%SDKREL% BUILD_UNILOG=true BUILD_CUST=%CUST%  MANUFACTURER=%MANUFACTURER%  2>&1 && del .failed.tmp) | tee.exe .\gccout\%OUTPUT_NAME%\cp\outbuildlog.txt



			if exist .failed.tmp (
			del .failed.tmp
			goto failHandle
			)
			
			rem this header file will compile with device code
			copy .\gccout\%OUTPUT_NAME%\cp\cp_project\debug_log_cp.h .\middleware\developed\debug\inc\ || (goto:failHandle)

			
		   echo > .failed.tmp
		   (make.exe -%JOBNUMBER% gccall TYPE=%CHIP_TYPE% TARGET=%BOARD_NAME% V=%VERBOSE% EUTRAN_MODE=%EUTRAN_MODE% PROJECT=cp_project CORE=cp SDK_REL=%SDKREL% BUILD_UNILOG=true BUILD_CUST=%CUST%  MANUFACTURER=%MANUFACTURER%  2>&1 && del .failed.tmp) | tee.exe .\gccout\%OUTPUT_NAME%\cp\outbuildlog.txt

           if exist .failed.tmp (
		   del .failed.tmp
		   goto failHandle
           )

		   echo > .failed.tmp
		   (make.exe -%JOBNUMBER% build-unilog TYPE=%CHIP_TYPE% TARGET=%BOARD_NAME% V=%VERBOSE% PROJECT=%PROJECT_NAME% CORE=ap SDK_REL=%SDKREL% BUILD_UNILOG=true BUILD_CUST=%CUST%  MANUFACTURER=%MANUFACTURER%  2>&1 && del .failed.tmp) | tee.exe .\gccout\%OUTPUT_NAME%\ap\outbuildlog.txt

		   if exist .failed.tmp (
		   del .failed.tmp
		   goto failHandle
			)
			
			rem this header file will compile with device code
			copy .\gccout\%OUTPUT_NAME%\ap\%PROJECT_NAME%\debug_log_ap.h .\middleware\developed\debug\inc\ || (goto:failHandle)
			if exist .\gccout\%OUTPUT_NAME%\ap\%PROJECT_NAME%\dbversion.h (
			copy .\gccout\%OUTPUT_NAME%\ap\%PROJECT_NAME%\dbversion.h .\middleware\developed\debug\inc\ || (goto:failHandle)
			)
			
		   echo > .failed.tmp
		   (make.exe -%JOBNUMBER% gccall TYPE=%CHIP_TYPE% TARGET=%BOARD_NAME% V=%VERBOSE% EUTRAN_MODE=%EUTRAN_MODE% PROJECT=%PROJECT_NAME% CORE=ap SDK_REL=%SDKREL% BUILD_UNILOG=true BUILD_CUST=%CUST%  MANUFACTURER=%MANUFACTURER%  2>&1 && del .failed.tmp) | tee.exe .\gccout\%OUTPUT_NAME%\ap\outbuildlog.txt

           if exist .failed.tmp (
		   del .failed.tmp
		   goto failHandle
          )
		  
		  .\tools\fcelf.exe -T -bin .\gccout\%OUTPUT_NAME%\ap\%PROJECT_NAME%\ap_%PROJECT_NAME%.bin -size AP_PKGIMG_LIMIT_SIZE ^
								-bin .\gccout\%OUTPUT_NAME%\cp\cp_project\cp-demo-flash.bin -size CP_PKGIMG_LIMIT_SIZE ^
								-bin .\gccout\%OUTPUT_NAME%\ap\bootloader\ap_bootloader.bin -size BOOTLOADER_PKGIMG_LIMIT_SIZE ^
								-h .\gccout\%OUTPUT_NAME%\ap\%PROJECT_NAME%\mem_map.txt
		  if not errorlevel 0 (
			goto failHandle
		  )
	    if "%BUILD_HEADBIN%"=="true" (
		    echo general headers
		    .\tools\ecsecure.exe APIMAGE=.\gccout\%OUTPUT_NAME%\ap\%PROJECT_NAME%\ap_%PROJECT_NAME%.bin ^
								 CPIMAGE=.\prebuild\FW\lib\gcc\%CHIP_TYPE%\cp-demo-flash.bin ^
								 BLIMAGE=.\gccout\%OUTPUT_NAME%\ap\bootloader\ap_bootloader.bin ^
								 BLHASH=1 ^
								 SYSHASH=1 SYSBNADR=7e000^
								 HEAD1=.\gccout\%OUTPUT_NAME%\ap\%PROJECT_NAME%\bl_sec_header.bin ^
								 HEAD2=.\gccout\%OUTPUT_NAME%\ap\%PROJECT_NAME%\sys_sec_header.bin ^
								 BLPEM=.\project\%BOARD_NAME%\ap\apps\bootloader\code\main\pub_key_bl.pem ^
								 SYSPEM=.\project\%BOARD_NAME%\ap\apps\bootloader\code\main\pub_key_sys.pem

	        .\tools\fcelf.exe -M  -input .\gccout\%OUTPUT_NAME%\ap\bootloader\ap_bootloader.bin -addrname  BL_IMG_MERGE_ADDR -flashsize BOOTLOADER_FLASH_LOAD_SIZE ^
								  -input .\gccout\%OUTPUT_NAME%\ap\%PROJECT_NAME%\ap_%PROJECT_NAME%.bin -addrname  AP_IMG_MERGE_ADDR -flashsize AP_FLASH_LOAD_SIZE ^
								  -input .\prebuild\FW\lib\gcc\%CHIP_TYPE%\cp-demo-flash.bin -addrname CP_IMG_MERGE_ADDR -flashsize CP_FLASH_LOAD_SIZE ^
								  -input .\gccout\%OUTPUT_NAME%\ap\%PROJECT_NAME%\bl_sec_header.bin -addrname  BLS_SEC_HAED_ADDR -flashsize BLS_FLASH_LOAD_SIZE ^
								  -input .\gccout\%OUTPUT_NAME%\ap\%PROJECT_NAME%\sys_sec_header.bin -addrname  SYS_SEC_HAED_ADDR -flashsize SYS_FLASH_LOAD_SIZE ^
								  -pkgmode 1  ^
								  -banoldtool 1 ^
								  -productname %PKG_PRODUCT% ^
								  -def .\device\target\board\%BOARD_NAME%\common\inc\%MEMMAP_NAME%.h ^
								  -outfile .\gccout\%OUTPUT_NAME%\ap\%PROJECT_NAME%\%PROJECT_NAME%.binpkg
            ) else (

		  .\tools\fcelf.exe -M  -input .\gccout\%OUTPUT_NAME%\ap\bootloader\ap_bootloader.bin -addrname  BL_PKGIMG_LNA -flashsize BOOTLOADER_PKGIMG_LIMIT_SIZE  ^
			-input .\gccout\%OUTPUT_NAME%\ap\%PROJECT_NAME%\ap_%PROJECT_NAME%.bin -addrname  AP_PKGIMG_LNA -flashsize AP_PKGIMG_LIMIT_SIZE ^
			-input .\gccout\%OUTPUT_NAME%\cp\cp_project\cp-demo-flash.bin -addrname CP_PKGIMG_LNA -flashsize CP_PKGIMG_LIMIT_SIZE ^
			-pkgmode 1  ^
			-banoldtool 1 ^
			-productname %PKG_PRODUCT% ^
			-def .\gccout\%OUTPUT_NAME%\ap\%PROJECT_NAME%\mem_map.txt ^
			-outfile .\gccout\%OUTPUT_NAME%\ap\%PROJECT_NAME%\%PROJECT_NAME%.binpkg
		    )
								
		  if not errorlevel 0 (
			goto failHandle
		  )
		  
		  .\tools\fcelf.exe -m  -input .\gccout\%OUTPUT_NAME%\ap\bootloader\ap_bootloader.elf -addrname BL_PKGIMG_LNA -flashsize BOOTLOADER_PKGIMG_LIMIT_SIZE  ^
			-input .\gccout\%OUTPUT_NAME%\ap\%PROJECT_NAME%\ap_%PROJECT_NAME%.elf -addrname  AP_PKGIMG_LNA -flashsize AP_PKGIMG_LIMIT_SIZE ^
			-input .\gccout\%OUTPUT_NAME%\cp\cp_project\cp-demo-flash.elf -addrname CP_PKGIMG_LNA -flashsize CP_PKGIMG_LIMIT_SIZE ^
			-pkgmode 1  ^
			-banoldtool 1 ^
			-productname %PKG_PRODUCT% ^
			-def .\gccout\%OUTPUT_NAME%\ap\%PROJECT_NAME%\mem_map.txt ^
			-outfile .\gccout\%OUTPUT_NAME%\ap\%PROJECT_NAME%\%PROJECT_NAME%.elf
			
		  if not errorlevel 0 (
			goto failHandle
		  )
		  
		  if exist .\tools\UpdateDBPattern.txt (
		  type .\tools\UpdateDBPattern.txt
		  )

		   
		  goto complete
	)
    
)


if %PROJECT_NAME%==bootloader goto :buildimage
if %PROJECT_NAME%==driver_example goto :buildimage
set UNILOG=true
echo start logprepass b2
if exist .\tools\UpdateDBPattern.txt (
del /q .\tools\UpdateDBPattern.txt
)
echo > .failed.tmp
(make.exe -%JOBNUMBER% build-unilog TYPE=%CHIP_TYPE% TARGET=%BOARD_NAME% V=%VERBOSE% PROJECT=%PROJECT_NAME% CORE=%CORE_NAME% TOOLCHAIN_NAME=%TOOLCHAIN_NAME% BUILD_UNILOG=true BUILD_CUST=%CUST%  MANUFACTURER=%MANUFACTURER% 2>&1 && del .failed.tmp) | tee.exe .\gccout\%OUTPUT_NAME%\%CORE_NAME%\outbuildlog.txt

if exist .failed.tmp (
del .failed.tmp
goto failHandle
)

rem this header file will compile with device code
copy .\gccout\%OUTPUT_NAME%\%CORE_NAME%\%PROJECT_NAME%\debug_log_%CORE_NAME%.h .\middleware\developed\debug\inc\ || (goto:failHandle)
if exist .\gccout\%OUTPUT_NAME%\%CORE_NAME%\%PROJECT_NAME%\dbversion.h (
copy .\gccout\%OUTPUT_NAME%\%CORE_NAME%\%PROJECT_NAME%\dbversion.h .\middleware\developed\debug\inc\ || (goto:failHandle)
)



echo -----build image----
:buildimage
REM goto end
echo > .failed.tmp
(make.exe -%JOBNUMBER% gccall TYPE=%CHIP_TYPE% TARGET=%BOARD_NAME% V=%VERBOSE% EUTRAN_MODE=%EUTRAN_MODE% PROJECT=%PROJECT_NAME% CORE=%CORE_NAME% BUILD_UNILOG=%UNILOG% BUILD_CUST=%CUST%  MANUFACTURER=%MANUFACTURER%  2>&1 && del .failed.tmp) | tee.exe .\gccout\%OUTPUT_NAME%\%CORE_NAME%\outbuildlog.txt

if exist .failed.tmp (
del .failed.tmp
goto failHandle
)

rem copy log database to output dir after compile successfully
REM cp .\tools\comdb_%CORE_NAME%.txt .\gccout\%OUTPUT_NAME%\%CORE_NAME%\%PROJECT_NAME%\comdb.txt

:complete

set endtime=%time%
echo .
echo End time: %date% %endtime%

set /a h1=%starttime:~0,2%
set /a m1=1%starttime:~3,2%-100
set /a s1=1%starttime:~6,2%-100
set /a h2=%endtime:~0,2%
set /a m2=1%endtime:~3,2%-100
set /a s2=1%endtime:~6,2%-100
if %h2% LSS %h1% set /a h2=%h2%+24
set /a ts1=%h1%*3600+%m1%*60+%s1%
set /a ts2=%h2%*3600+%m2%*60+%s2%
set /a ts=%ts2%-%ts1%
set /a h=%ts%/3600
set /a m=(%ts%-%h%*3600)/60
set /a s=%ts%%%60
echo Built took %h% hours %m% minutes %s% seconds

echo     #######################################################################
echo     ##                                                                   ##
echo     ##                 ########     ###     ######   ######              ##
echo     ##                 ##     ##   ## ##   ##    ## ##    ##             ##
echo     ##                 ##     ##  ##   ##  ##       ##                   ##
echo     ##                 ########  ##     ##  ######   ######              ##
echo     ##                 ##        #########       ##       ##             ##
echo     ##                 ##        ##     ## ##    ## ##    ##             ##
echo     ##                 ##        ##     ##  ######   ######              ##
echo     ##                                                                   ##
echo     #######################################################################

echo build successfully

if %UNILOG%==true (
if exist .\tools\UpdateDBPattern.txt (
type .\tools\UpdateDBPattern.txt
)
)

:end
goto:eof

:failHandle
echo fail
echo     #######################################################################
echo     ##                                                                   ##
echo     ##                    ########    ###     ####  ##                   ##
echo     ##                    ##         ## ##     ##   ##                   ##
echo     ##                    ##        ##   ##    ##   ##                   ##
echo     ##                    ######   ##     ##   ##   ##                   ##
echo     ##                    ##       #########   ##   ##                   ##
echo     ##                    ##       ##     ##   ##   ##                   ##
echo     ##                    ##       ##     ##  ####  ########             ##
echo     ##                                                                   ##
echo     #######################################################################
goto:eof

