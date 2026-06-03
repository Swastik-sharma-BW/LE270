#!/bin/bash
echo $PATH
clear

#fibo_soft EC718UM OPENCPU LE230-CN-1S6AW5-99
echo "project-chiptype-opencpu-target :" $*
#echo "upper chiptype: $(echo "$2" | tr '[:lower:]' '[:upper:]')"
#echo "lower chiptype: $(echo "$2" | tr '[:upper:]' '[:lower:]')"

#FORMAT CHIPTYE INFOMATION
export CHIP_TYPE=$(echo "$2" | tr '[:upper:]' '[:lower:]')
export PKG_PRODUCT="$2"_PRD
export CHIP="${CHIP_TYPE:2}"
export PKG_MAPDEF=pkg_"$CHIP"_mapdef

export PROJECT_NAME=at_command
export BOARD_NAME=ec7xx_0h00
export CHIP_NAME=ec7xx
export CORE_NAME=ap
export EUTRAN_MODE=cat_mode
export BUILD_OPTION=merge
export UNILOG=false
export TOOLCHAIN_NAME=GCC
export CUST=common
export SDKREL=false
export ROM_ENABLE=false
export LESS_LOG=false
export GCCLIB_PATH="$('pwd')/tools/gcc-arm-none-eabi"
export BUILD_ENV=linux
export COMDBLIB_PATH="./prebuild/PLAT/lib/gcc/$CHIP_TYPE/oc"
export FLASH_LD_DIR_OR="$('pwd')/device/target/board/ec7xx_0h00/flash_ld/ec_original_718_flash_ld"
export pkg_cmd=" "
export libdir="oc"

if [ "$1" == "fibo_soft" ]; then
export OPENCPU=true
fi

if [ "$2" == "EC718S" ]; then
export LTO_ENABLE=true
export GCF_ENABLE=false
export BUILD_HEADBIN=false
# 15A000 1384KB
export MAX_APP_SIZE=1417216
export LESS_LOG=true
export COMDBLIB_PATH="./prebuild/PLAT/lib/gcc/$CHIP_TYPE/oc_lesslog"
fi


if [ "$2" == "EC718U" ] || [ "$2" == "EC718UM" ]; then
export IMS_ENABLE=false
export AUDIO_ENABLE=true
export LTO_ENABLE=false
# 0x278000  2528KB
export MAX_APP_SIZE=6488064
export COMDBLIB_PATH="./prebuild/PLAT/lib/gcc/$CHIP_TYPE/ims"
export CPBIN_SUBPATH=audio
fi


if [ "$2" == "EC718P" ] || [ "$2" == "EC718PM" ]; then
export OPENCPU=true
export LTO_ENABLE=false
# 0x278000  2528KB
export MAX_APP_SIZE=2588672
export COMDBLIB_PATH="./prebuild/PLAT/lib/gcc/$CHIP_TYPE/full"
fi

if [ "$2" == "EC716S" ]; then
export LTO_ENABLE=true
export GCF_ENABLE=false
export BUILD_HEADBIN=false
# 15A000 1384KB
export MAX_APP_SIZE=1462272
export LESS_LOG=true
export COMDBLIB_PATH="./prebuild/PLAT/lib/gcc/$CHIP_TYPE/oc_lesslog"
fi

if [ "$2" == "EC716E" ]; then
export LTO_ENABLE=true
export GCF_ENABLE=false
export BUILD_HEADBIN=false
export COMDBLIB_PATH="./prebuild/PLAT/lib/gcc/$CHIP_TYPE/ram"
export RAM_ENBALE=true
export libdir="ram"
# 0x278000  2528KB
export MAX_APP_SIZE=2637824
fi

#fibocom opencpu modify OPENCPU true
if [ "$3" == "OPENCPU" ]; then
export FIBOCOM_PKG_OPENSDK=true
export OPENCPU=true
fi

if [ -d "prebuild/FW/lib/gcc/$CHIP_TYPE/oc" ];then
export CPBIN_SUBPATH=oc
elif [ -d "prebuild/FW/lib/gcc/$CHIP_TYPE/ram" ];then
export CPBIN_SUBPATH=ram
else
export CPBIN_SUBPATH=""
fi

if [ "$2" == "EC718U" ] || [ "$2" == "EC718UM" ]; then
export CPBIN_SUBPATH=audio
fi

export OUTPUT_NAME=$BOARD_NAME"_"$CHIP_TYPE
export JOBNUMBER=j8
export CUR_PATH=$('pwd')

echo $0 version $(date "+%Y%m%d")
echo GCCLIB_PATH: $GCCLIB_PATH
echo Chip Type: $CHIP_TYPE


if [ ! -e "gccout" ]; then
mkdir gccout
fi

TARGET_DIR="$('pwd')/project/$BOARD_NAME/ap/apps/bootloader/GCC/target.inc"
export KT_MACRO_EXIST=$(grep -wR "CONFIG_CUST_KR_FEATURE"  $TARGET_DIR | awk '{print $3}')
if [ -e  "$CUR_PATH/fibocom/fibo-framework/build/tools/gen.py" ]; then 
if [[ "$KT_MACRO_EXIST"  == y* ]]; then
python3 $CUR_PATH/fibocom/fibo-framework/build/tools/gen.py  $CUR_PATH/fibocom/fibo-framework/PAL/src/ec718/gtset/fibo_gtset_kt.json $CUR_PATH/gccout/gtset_json.h
else
python3 $CUR_PATH/fibocom/fibo-framework/build/tools/gen.py  $CUR_PATH/fibocom/fibo-framework/PAL/src/ec718/gtset/fibo_gtset.json $CUR_PATH/gccout/gtset_json.h
fi
python3 $CUR_PATH/tools/testcase_mk.py $CUR_PATH/fibocom/testcase/ $CUR_PATH/gccout/testcase_header.h

echo "exe gen.py ..."
fi
echo CUR_PATH:$CUR_PATH

#touch -d $(date "+%Y%m%d%H%M%S") ./fibocom/fibo-framework/module/ffw_verno/verno.c

echo $1 $PARAMETERS
PARAMETERS=$1
if [ -z $PARAMETERS ];then
echo no input paramter, use default build setting
echo default Board    is: $BOARD_NAME
echo default Chip     is: $CHIP_NAME
echo default ChipType is: $CHIP_TYPE
echo default Project  is: $PROJECT_NAME
echo default core     is: $CORE_NAME
echo default option   is: $BUILD_OPTION
fi

if [ -n  "$(echo $PARAMETERS|grep 'ec718_0h00')" ]
then
	export BOARD_NAME=ec718_0h00
	export CHIP_NAME=ec7xx
	export OUTPUT_NAME=ec718_0h00_$CHIP_TYPE
fi

if [ -n  "$(echo $PARAMETERS|grep 'ap')" ]
then
   export CORE_NAME=ap
   export BUILD_OPTION=no_merge
fi

if [ -n  "$(echo $PARAMETERS|grep 'at_command')" ]
then
  export PROJECT_NAME=at_command
fi

if [ -n  "$(echo $PARAMETERS|grep 'fibo_soft')" ]
then
  export PROJECT_NAME=fibo_soft
  export MANUFACTURER=FIBOCOM
  export LSPD=true
  #export LITE=true
fi


if [ -n  "$(echo $PARAMETERS|grep 'bootloader')" ]
then
  export PROJECT_NAME=bootloader
  export BUILD_OPTION=no_merge
fi

if [ -n  "$(echo $PARAMETERS|grep 'merge')" ]
then
  export BUILD_OPTION=merge
fi

if [ -n  "$(echo $PARAMETERS|grep 'lspd_only')" ]
then
  export LSPD=true
fi

if [ -n  "$(echo $PARAMETERS|grep 'lspd_mode')" ]
then
  export LSPD=true
fi

if [ -n  "$(echo $PARAMETERS|grep 'lite_mode')" ]
then
  export LITE=true
fi

if [ -n  "$(echo $PARAMETERS|grep 'open_cpu')" ]
then
  export LSPD=true
  export LITE=true
fi

if [ -n "$(echo $PARAMETERS|grep 'verbose')" ]
then
  export VERBOSE=1
else
  export VERBOSE=0
fi

if [ -n "$(echo $LITE|grep 'true')" ]
then 
  export COMDBLIB_PATH="./prebuild/PLAT/lib/gcc/lite"
fi

if [ -n  "$(echo $param|grep -w 'rom')" ]
		then
		  export ROM_ENABLE=true
		  export COMDBLIB_PATH="./prebuild/PLAT/lib/gcc/$CHIP_TYPE/rom"
		  export CPBIN_SUBPATH=oc
		fi
		
		if [ -n  "$(echo $param|grep -w 'lesslog')" ]
		then
			if	[ "$ROM_ENABLE" == "true" ]
			then
			  export LESS_LOG=true
			  export COMDBLIB_PATH="./prebuild/PLAT/lib/gcc/$CHIP_TYPE/rom_lesslog"
			  export CPBIN_SUBPATH=rom_lesslog
			 fi
fi

#Lucien add for secboot
if [ -n "$(echo $@|grep 'secboot')" ]
then
  export BUILD_HEADBIN=true
else
  export BUILD_HEADBIN=false
fi

#add for audio amr
if [ -n "$(echo $@|grep 'audio_amr')" ]
then
	export COMDBLIB_PATH="./prebuild/PLAT/lib/gcc/$CHIP_TYPE/oc"
	export IMS_ENABLE=false
	export AUDIO_ENABLE=true
	export CPBIN_SUBPATH=audio
else
   echo "audio amr close!"	
fi

#add for gcf
if [ -n "$(echo $@|grep 'gcf_enable')" ]
then
	export COMDBLIB_PATH="./prebuild/PLAT/lib/gcc/$CHIP_TYPE/gcf"
	export GCF_ENABLE=true
	export CPBIN_SUBPATH=wifi
else
   echo "gcf disable!"
fi

export IF_IMG_LFS_LNA=$LF_START
export IF_IMG_LFS_SIZE=$LF_SIZE

if [ -f '$COMDBLIB_PATH/comdblib.txt' ]
then
	echo cp '$COMDBLIB_PATH/comdblib.txt' './tools/'
#cp $COMDBLIB_PATH/comdblib.txt ./tools/
fi

echo curr Board   is: $BOARD_NAME
echo curr Chip    is: $CHIP_NAME
echo curr Type    is: $CHIP_TYPE
echo curr Project is: $PROJECT_NAME
echo curr core    is: $CORE_NAME
echo curr option  is: $BUILD_OPTION
echo curr headbin is: $BUILD_HEADBIN
echo curr log     is: $LESS_LOG
echo curr dbpath  is: $COMDBLIB_PATH
echo curr cpbin   is: $CPBIN_SUBPATH
#set -x
#exit 1
TARGET_DIR="$('pwd')/project/$BOARD_NAME/ap/apps/bootloader/GCC/target.inc"
export BOOT2_EXIST=$(grep -wR "CONFIG_FFW_TWO_BOOTLOADER" $TARGET_DIR | awk '{print $3}')
export TTS_RES_EXIST=$(grep -wR "CONFIG_FFW_TTS_ADD_RES"  $TARGET_DIR | awk '{print $3}')
export IMS_EXIST=$(grep -wR "CONFIG_FIBO_FEATURE_IMS_ENABLE"  $TARGET_DIR | awk '{print $3}')
export ISMS_EXIST=$(grep -wR "CONFIG_FIBO_FEATURE_ISMS_ENABLE"  $TARGET_DIR | awk '{print $3}')
export MID_EXIST=$(grep -wR "CONFIG_FIBO_EC716S_MID_ENABLE"  $TARGET_DIR | awk '{print $3}')
export DEVCFG_EXIST=$(grep -wR "CONFIG_FIBO_DEVCFG"  $TARGET_DIR | awk '{print $3}')
export ROMLESSLOG_EXIST=$(grep -wR "CONFIG_FIBO_FEATURE_ROM_LESSLOG"  $TARGET_DIR | awk '{print $3}')
echo  Default TTS RES not EXIST, now check config $TTS_RES_EXIST
echo "prj = $4 "
echo Default BOOTLOADER2 not EXIST, now check config $BOOT2_EXIST 


if [[ "$BOOT2_EXIST"  == y* ]]; then
# add bootloader2 build option
export pkg_cmd="-input  ./gccout/$OUTPUT_NAME/ap/bootloader/ap_bootloader_part2.bin -addrname PKGFLX_BL2_PKGIMG_LNA -flashsize PKGFLX_BL2_PKGIMG_LIMIT_SIZE"
export ecs_cmd="BL2IMAGE=./gccout/$OUTPUT_NAME/ap/bootloader/ap_bootloader_part2.bin"
echo BOOTLOADER2 EXIST!!!
export FLASH_LD_DIR_SR="$('pwd')/device/target/board/ec7xx_0h00/flash_ld/ec718p_flash_ld_boot2"
# 0x26A000  2472KB
export MAX_APP_SIZE=2531328  
fi

if [[ "$TTS_RES_EXIST"  == y* ]]; then
# add tts build option
echo TTS RES EXIST
TTS_ORG_DIR="./fibocom/fibo-framework/PAL/src/ec718/lib/tts/ivTTS_CE+PE_Templet.16K.irf"
export pkg_cmd=" -input  $TTS_ORG_DIR -addrname PKGFLX_FIBO_TTS_RES_LNA -flashsize PKGFLX_FIBO_TTS_RES_SIZE"
export FLASH_LD_DIR_SR="$('pwd')/device/target/board/ec7xx_0h00/flash_ld/ec718p_flash_ld_tts"
echo TTS RES EXIST!!!
echo $pkg_cmd
echo $FLASH_LD_DIR_SR
fi

#add res2 for customer
presetres=`grep -wR CONFIG_PRESET_RES $TARGET_DIR |awk -F'=' '{print $2}' | tr -d '"' | tr -d '\r\n'`
echo presetres: $presetres
echo cmd $pkg_cmd
if [ -n "$presetres" ]; then
	echo presetfile is not empty. presetres is $presetres
	export pkg_cmd+=" -input $presetres -addrname PKGFLX_FIBO_CUST_RES_LNA -flashsize PKGFLX_FIBO_CUST_RES_SIZE "
else
	echo "presetfile is empty!!!"
fi
echo pkg_cmd : $pkg_cmd

#add devcfg partition: f_def_devcfg.bin
if [[ "$DEVCFG_EXIST"  == y* ]]; then
	DEF_DEVCFG_BIN="./gccout/$OUTPUT_NAME/ap/$PROJECT_NAME/devcfg/f_def_devcfg.bin"
	export pkg_cmd+=" -input  $DEF_DEVCFG_BIN -addrname PKGFLX_FIBO_RES_DEVCFG_DEF_LNA -flashsize PKGFLX_FIBO_RES_DEVCFG_DEF_SIZE"
	DEF_DEVCFG_BIN="./gccout/$OUTPUT_NAME/ap/$PROJECT_NAME/devcfg/f_cust_devcfg.bin"
	export pkg_cmd+=" -input  $DEF_DEVCFG_BIN -addrname PKGFLX_FIBO_RES_DEVCFG_CUST_LNA -flashsize PKGFLX_FIBO_RES_DEVCFG_CUST_SIZE"
	echo $pkg_cmd
fi

if [ -n "$(echo $@|grep 'inr_presets_fs')" ]; then
export pkg_cmd+=" -input ./feedback_fs_data.bin -addrname PKGFLXLFS_LNA -flashsize PKGFLXLFS_SIZE"
fi

if [ -n "$(echo $@|grep 'ext_presets_fs')" ]; then
export pkg_cmd+=" -input ./ex_feedback_fs_data.bin -addrname EF_IMG_LFS_LNA -flashsize EF_IMG_LFS_SIZE"
fi

if [ -n "$(echo $@|grep 'poweron_boot')" ]; then
AUTO_BOOT_CFG_DIR=$CHIP_TYPE"_boot"
export pkg_cmd+=" -input ./tools/pkgdata/$AUTO_BOOT_CFG_DIR/xpk_PrMgrCfg_json.bin -addrname XPKGDPRM_VIRTIMG_MERGE_LNA -flashsize XPKG_VIRTIMG_LOAD_SIZE \
				-input ./tools/pkgdata/$AUTO_BOOT_CFG_DIR/xpk_format_json.bin -addrname XPKGDCMN_VIRTIMG_MERGE_LNA -flashsize XPKG_VIRTIMG_LOAD_SIZE \
				-input ./tools/pkgdata/$AUTO_BOOT_CFG_DIR/config_xpk_uart_baseini.bin -addrname XPKGDCMN_VIRTIMG_MERGE_LNA -flashsize XPKG_VIRTIMG_LOAD_SIZE \
				-input ./tools/pkgdata/$AUTO_BOOT_CFG_DIR/config_xpk_usb_baseini.bin -addrname XPKGDCMN_VIRTIMG_MERGE_LNA -flashsize XPKG_VIRTIMG_LOAD_SIZE "
echo pkg_cmd : $pkg_cmd
fi

echo BOOT2_EXIST: $BOOT2_EXIST  TTS_RES_EXIST :$TTS_RES_EXIST

echo opencpu:$OPENCPU  $2
echo lesslog:$LESS_LOG  
echo ims:$IMS_EXIST  
echo isms:$ISMS_EXIST
echo mid:$MID_EXIST
# EC718P/EC718E/EC716E/EC718E OPENCPU used true; EC716S/EC718S used lesslog, Volte and IMS SMD are requirements that need to be opened
if [[  "$OPENCPU"  == true* ]]; then
libdir="oc"
export COMDBLIB_PATH="./prebuild/PLAT/lib/gcc/$CHIP_TYPE/oc"
if [ "$2" == "EC718U" ]; then
export COMDBLIB_PATH="./prebuild/PLAT/lib/gcc/$CHIP_TYPE/ims"
libdir="ims"
fi
if [ "$2" == "EC716E" ]; then
export COMDBLIB_PATH="./prebuild/PLAT/lib/gcc/$CHIP_TYPE/ram"
libdir="ram"
fi
else
libdir="full"
fi

# used for ec718s or ec716s
if [[ "$LESS_LOG"  == true* ]]; then
export COMDBLIB_PATH="./prebuild/PLAT/lib/gcc/$CHIP_TYPE/oc_lesslog"
libdir="oc_lesslog"
if [[ "$ROMLESSLOG_EXIST"  == y* ]]; then
export ROM_ENABLE=true
export COMDBLIB_PATH="./prebuild/PLAT/lib/gcc/$CHIP_TYPE/rom_lesslog"
libdir="rom_lesslog"
export CPBIN_SUBPATH=rom_lesslog
fi
fi



#add for gcf
if [ -n "$(echo $@|grep 'gcf_enable')" ]
then
libdir="gcf"
fi

if [[ "$IMS_EXIST"  == y* ]]; then
libdir="ims"
export COMDBLIB_PATH="./prebuild/PLAT/lib/gcc/$CHIP_TYPE/ims"
export IMS_ENABLE=true
export OPENCPU=true
export AUDIO_ENABLE=true
export CPBIN_SUBPATH=audio
fi

if [[ "$ISMS_EXIST"  == y* ]]; then
libdir="isms"
export COMDBLIB_PATH="./prebuild/PLAT/lib/gcc/$CHIP_TYPE/isms"
export IMS_SMSONLY_ENABLE=true
export OPENCPU=true
fi

if [[ "$MID_EXIST"  == y* ]]; then
libdir="mid"
export COMDBLIB_PATH="./prebuild/PLAT/lib/gcc/$CHIP_TYPE/mid"
export MID_ENABLE=true
export CPBIN_SUBPATH=wifi
fi
echo "libdir :" $libdir
# select lib dir END 

#set +x
echo curr dbpath  is: $COMDBLIB_PATH
#exit 1
if [ -n "$(echo $PARAMETERS|grep 'help')" ]
then
echo "=============================================================================================="
echo "                                                                                              "
echo "                            >>  HOW TO BUILD A PROJECT  <<                                    "
echo "                                                                                              "
echo "=============================================================================================="
echo "                                                                                              "
echo "  @FORMAT : Gccbuild.bat [<board>[-<core>[-<project>[-<option>]]]]                            "
echo "  @CAUTION: All parameters (<board>/<core>/<project>/<option>) are optional,                  "
echo "            and the default settings of the project will take effect if it occurs!            "
echo "            For <board>, only 'ec718_0h00' is supportted, and it should be omitted !          "
echo "                                                                                              "
echo "**********************************************************************************************"
echo "  <options>:                                                                                  "
echo "   >> Basic                                                                                   "
echo "     merge        compile bl, cp and ap images and merge them into a package.  [default]      "
echo "     clean        clean the output files for a specific board and project.                    "
echo "     clall        clean all output files for every board and project.                         "
echo "     list         list all supported boards and supported projects for every board            "
echo "     help         display this help message.                                                  "
echo "                                                                                              "
echo "   >> Advanced                                                                                "
echo "     lspd_only    old style option, please refer to 'lspd_mode'.                              "
echo "     lspd_mode    build an image to meet user's more ram demands for some project,            "
echo "                  at a cost of uplink/downlink data rate loss and a few features.             "
echo "     lite_mode    build an image to meet user's more rom demands for some project,            "
echo "                  at a cost of a lot of features.                                             "
echo "     open_cpu     build an image to meet user's more ram & rom demands for some project,      "
echo "                  at a cost of uplink/downlink data rate loss and a lot of features.          "
echo "                                                                                              "
echo "      rom            mini + remove rndis/socket           AP lib: rom, CP image��ooc           "
echo "      rom-lesslog    rom + log reducing  AP lib: rom_lesslog, CP image��orom_lesslog           "
echo "**********************************************************************************************"
echo "  Examples:                                                                                   "
echo "----------------------- For Internal Full Source Build----------------------------------------"
echo "     > Gccbuild.bat                                                                           "
echo "          build bootloader, ap images and merge them into a package.                      "
echo "     > Gccbuild.bat ap-at_command                                                             "
echo "          build ap at_command project only.                                                   "
echo "     > Gccbuild.bat ap-bootloader                                                             "
echo "          build bootloader project only.                                                      "
echo "     > Gccbuild.bat ap-driver_example-clean                                                   "
echo "          clean all output files for ap driver_example project.                               "
echo "                                                                                              "
echo "----------------------- For Customer's SDK Build----------------------------------------------"
echo "     > Gccbuild.bat                                                                           "
echo "          build bootloader,ap images, use prebuilt cp image, and merge them into a package.   "
echo "     > Gccbuild.bat project_name      e.g. project_name = at_command/driver_example           "
echo "          build xxx(project_name) project                                                     "
echo "                                                                                              "
echo "     @NOTE: SDK build always generates a package(project_name.bin + bootloader.bin + cp.bin). "
echo "                                                                                              "
echo "----------------------- For Universal Build---------------------------------------------------"
echo "     > Gccbuild.bat clean                                                                     "
echo "          clean all output files for the default project.                                     "
echo "     > Gccbuild.bat clall                                                                     "
echo "          clean all output files for every board and project.                                 "
echo "     > Gccbuild.bat list                                                                      "
echo "          list all supported boards and supported projects for every board.                   "
echo "                                                                                              "
echo "     > Gccbuild.bat lspd_mode                                                                 "
echo "          build 'lspd mode' images of the default project, and merge them into a package.     "
echo "     > Gccbuild.bat lite_mode                                                                 "
echo "          build 'lite mode' images of the default project, and merge them into a package.     "
echo "     > Gccbuild.bat open_cpu                                                                  "
echo "          build 'open cpu' mode images of the default project, and merge them into a package. "
echo "                                                                                              "
echo "**********************************************************************************************"
fi
end()
{
echo "--end--"
exit 1
}

python3 opencpu.py api $CHIP_TYPE

if [ -n "$(echo $PARAMETERS|grep 'clall')" ]
then
	if [ -e "gccout/info.txt" ]; then
		CHIP_TYPE=$(sed -n '1p' ./gccout/info.txt | awk '{print tolower($1)}')
		PROGRAMME=$(sed -n '1p' ./gccout/info.txt | awk '{print $2}')
		PROJECT_NAME=fibo_soft
		echo chiptype: $CHIP_TYPE
		echo programme: $PROGRAMME
		cp ./target/$PROGRAMME/target.inc  ./project/ec7xx_0h00/ap/apps/fibo_soft/GCC/
		make -$JOBNUMBER  clean-gccall TYPE=$CHIP_TYPE TARGET=$BOARD_NAME PROJECT=$PROJECT_NAME CORE=$CORE_NAME
		rm -rf ./project/ec7xx_0h00/ap/apps/fibo_soft/GCC/target.inc	
		echo "$PROGRAMME clean all done ok..."		
	fi
	
    rm -rf gccout
	echo "Default clean all done ok..."
	end	
fi

if [ -n "$(echo $PARAMETERS|grep 'clean')" ]
then
	make -$JOBNUMBER  clean-gcc TYPE=$CHIP_TYPE TARGET=$BOARD_NAME PROJECT=$PROJECT_NAME CORE=$CORE_NAME
	echo "clean done ok..."
	end
fi

completeHandle()
{
appsize=`ls -l ./gccout/$OUTPUT_NAME/ap/$PROJECT_NAME/ap_${PROJECT_NAME}.bin|awk '{print $5}'`
realappsize=`ls -l ./gccout/$OUTPUT_NAME/ap/$PROJECT_NAME/ap_${PROJECT_NAME}_unZip.bin|awk '{print $5}'`
endtime=$(date "+%Y/%m/%d %H:%M:%S")
echo "Start time:" $starttime
echo "End time:" $endtime
if [ -f ./gccout/$OUTPUT_NAME/ap/$PROJECT_NAME/ap_$PROJECT_NAME.bin ] && [ -f ./gccout/$OUTPUT_NAME/ap/bootloader/ap_bootloader.bin ]
then
	echo  build realapp size unzip realapp size $realappsize , loade zip app size: $appsize , Max size  $MAX_APP_SIZE
	echo "#######################################################################"
	echo "##                                                                   ##"
	echo "##                 ########     ###     ######   ######              ##"
	echo "##                 ##     ##   ## ##   ##    ## ##    ##             ##"
	echo "##                 ##     ##  ##   ##  ##       ##                   ##"
	echo "##                 ########  ##     ##  ######   ######              ##"
	echo "##                 ##        #########       ##       ##             ##"
	echo "##                 ##        ##     ## ##    ## ##    ##             ##"
	echo "##                 ##        ##     ##  ######   ######              ##"
	echo "##                                                                   ##"
	echo "#######################################################################"
	echo build successfully
else
	echo build app file size $appsize ,Max size  $MAX_APP_SIZE
	failHandle
fi
}

failHandle()
{
echo fail
echo "#######################################################################"
echo "##                                                                   ##"
echo "##                    ########    ###     ####  ##                   ##"
echo "##                    ##         ## ##     ##   ##                   ##"
echo "##                    ##        ##   ##    ##   ##                   ##"
echo "##                    ######   ##     ##   ##   ##                   ##"
echo "##                    ##       #########   ##   ##                   ##"
echo "##                    ##       ##     ##   ##   ##                   ##"
echo "##                    ##       ##     ##  ####  ########             ##"
echo "##                                                                   ##"
echo "#######################################################################"
end
}

copy_config()
{
	# copy mem config
	##echo $FLASH_LD_DIR_SR
	##cp -rf $FLASH_LD_DIR_SR/pkg_$CHIP"_mapdef.h"  ./device/target/board/ec7xx_0h00/common/pkginc/
	##cp -rf $FLASH_LD_DIR_SR/mem_map_$CHIP.h       ./device/target/board/ec7xx_0h00/common/inc/
	

	# change tools name
	#cp -rf ./tools/scripts/linux/Makefile.rules ./tools/scripts/Makefile.rules 
	#grep -w "LogPrePass.exe" -wrl ./middleware/developed/* | xargs sed -i 's/LogPrePass.exe/LogPrePass/g'
	#grep -w "gperf.exe -L" -wrl ./middleware/developed/* | xargs sed -i 's/gperf.exe -L/gperf -L/g'
	
	# copy log db
    if [ -f "${COMDBLIB_PATH}/comdblib.txt" ]; then
        cp $COMDBLIB_PATH/comdblib.txt  ./tools/
    else
        # 检查文件是否存在
        if [ -f "${COMDBLIB_PATH}/comdb_ap_lib.txt" ]; then
            ./tools/LogPrePass -I "${COMDBLIB_PATH}/comdb_ap_lib.txt" \
                               -I "./prebuild/FW/lib/gcc/${CHIP_TYPE}/${CPBIN_SUBPATH}/comdb_cp.txt" \
                               -h "./tools/debug_log_no_use.h" -d "./tools/comdblib.txt"

            # 删除临时文件
            rm -f "./tools/debug_log_no_use.h"
        fi
    fi
}

restore_config()
{
	# restore mem config
	#echo $FLASH_LD_DIR_SR
	#cp -rf $FLASH_LD_DIR_OR/pkg_$CHIP"_mapdef.h"  ./device/target/board/ec7xx_0h00/common/pkginc/
	#cp -rf $FLASH_LD_DIR_OR/mem_map_$CHIP.h       ./device/target/board/ec7xx_0h00/common/inc/
	
	# change tools name
	#grep -w "LogPrePass" -wrl ./middleware/developed/* | xargs sed -i 's/LogPrePass/LogPrePass.exe/g'
	#grep -w "gperf -L" -wrl ./middleware/developed/* | xargs sed -i 's/gperf -L/gperf.exe -L/g'
	
	# restore log db
	#cp -rf ./tools/scripts/win/Makefile.rules ./tools/scripts/Makefile.rules
	rm -rf  ./tools/comdblib.txt  ./tools/comdb_cust_lib.txt
	rm -rf  -rf ./tools/tmp/ 
}

starttime=$(date "+%Y/%m/%d %H:%M:%S")
echo "Start time:" $starttime

if [ -n "$(echo $BUILD_OPTION | grep 'merge')" ]
then
	if [ -f '.\tools\comdblib.txt' ]
	then
		failHandle
	else
	
	copy_config
	# build bootloader
	make -$JOBNUMBER gccall TYPE=$CHIP_TYPE TARGET=$BOARD_NAME V=$VERBOSE EUTRAN_MODE=$EUTRAN_MODE PROJECT=bootloader CORE=$CORE_NAME SDK_REL=$SDKREL BUILD_UNILOG=false BUILD_CUST=$CUST MANUFACTURER=$MANUFACTURER BUILD_ENV=$BUILD_ENV | tee ./gccout/$OUTPUT_NAME/$CORE_NAME/outbuildlog.txt	
	# build log pp file
	echo $PROJECT_NAME 
	make -$JOBNUMBER  build-unilog TYPE=$CHIP_TYPE TARGET=$BOARD_NAME PROJECT=$PROJECT_NAME CORE=$CORE_NAME BUILD_UNILOG=true BUILD_CUST=$CUST SDK=true LSPD=$LSPD MANUFACTURER=$MANUFACTURER  BUILD_ENV=$BUILD_ENV 
	# copy log file
	cp ./gccout/$OUTPUT_NAME/ap/$PROJECT_NAME/debug_log_ap.h ./middleware/developed/debug/inc/
	if [ -e "./gccout/$OUTPUT_NAME/ap/$PROJECT_NAME/dbversion.h" ]; then
		cp -rf ./gccout/$OUTPUT_NAME/ap/$PROJECT_NAME/dbversion.h  ./middleware/developed/debug/inc/
	fi
	# build ap

	make -$JOBNUMBER gccall TYPE=$CHIP_TYPE TARGET=$BOARD_NAME V=$VERBOSE EUTRAN_MODE=$EUTRAN_MODE PROJECT=$PROJECT_NAME CORE=ap BUILD_UNILOG=true BUILD_CUST=$CUST SDK=true MANUFACTURER=$MANUFACTURER BUILD_ENV=$BUILD_ENV | tee ./gccout/$OUTPUT_NAME/ap/outbuildlog.txt
	
	if [ -n "$(echo $BUILD_HEADBIN|grep 'true')" ]
	then
	  ./tools/ecsecure APIMAGE=./gccout/$OUTPUT_NAME/ap/$PROJECT_NAME/ap_$PROJECT_NAME.bin \
	                 CPIMAGE=./prebuild/FW/lib/gcc/$CHIP_TYPE/$CPBIN_SUBPATH/cp-demo-flash.bin \
									 BLIMAGE=./gccout/$OUTPUT_NAME/ap/bootloader/ap_bootloader.bin \
									 $ecs_cmd \
									 BLHASH=1 SYSHASH=1 \
									 HEAD1=./gccout/$OUTPUT_NAME/ap/$PROJECT_NAME/bl_sec_header.bin \
									 HEAD2=./gccout/$OUTPUT_NAME/ap/$PROJECT_NAME/sys_sec_header.bin \
									 BLPEM=./project/$BOARD_NAME/ap/apps/bootloader/code/main/pub_key_bl.pem \
									 SYSPEM=./project/$BOARD_NAME/ap/apps/bootloader/code/main/pub_key_sys.pem \
									 ADRBASE=AP_FLASH_XIP_ADDR \
									 APADR=AP_FLASH_LOAD_ADDR \
									 CFGDEF=./gccout/$OUTPUT_NAME/ap/$PROJECT_NAME/mem_map.txt
		# link pkg
		./tools/fcelf -M  -input ./gccout/$OUTPUT_NAME/ap/bootloader/ap_bootloader.bin -addrname  BL_PKGIMG_LNA -flashsize BOOTLOADER_PKGIMG_LIMIT_SIZE \
											-input ./gccout/$OUTPUT_NAME/ap/$PROJECT_NAME/ap_$PROJECT_NAME.bin -addrname  AP_PKGIMG_LNA -flashsize AP_PKGIMG_LIMIT_SIZE \
											-input ./prebuild/FW/lib/gcc/$CHIP_TYPE/$CPBIN_SUBPATH/cp-demo-flash.bin -addrname CP_PKGIMG_LNA -flashsize CP_PKGIMG_LIMIT_SIZE \
										  -input ./gccout/$OUTPUT_NAME/ap/$PROJECT_NAME/bl_sec_header.bin -addrname  XPKGDBLSH_VIRTIMG_MERGE_LNA  -flashsize XPKG_VIRTIMG_LOAD_SIZE \
										  -input ./gccout/$OUTPUT_NAME/ap/$PROJECT_NAME/sys_sec_header.bin -addrname  XPKGDSYSH_VIRTIMG_MERGE_LNA  -flashsize XPKG_VIRTIMG_LOAD_SIZE \
										  $pkg_cmd  \
											-input ./tools/pkgdata/$CHIP_TYPE/xpk_PrMgrCfg_json.bin -addrname XPKGDPRM_VIRTIMG_MERGE_LNA -flashsize XPKG_VIRTIMG_LOAD_SIZE \
											-input ./tools/pkgdata/$CHIP_TYPE/xpk_format_json.bin -addrname XPKGDCMN_VIRTIMG_MERGE_LNA -flashsize XPKG_VIRTIMG_LOAD_SIZE \
											-input ./tools/pkgdata/$CHIP_TYPE/config_xpk_uart_baseini.bin -addrname XPKGDCMN_VIRTIMG_MERGE_LNA -flashsize XPKG_VIRTIMG_LOAD_SIZE \
											-input ./tools/pkgdata/$CHIP_TYPE/config_xpk_usb_baseini.bin -addrname XPKGDCMN_VIRTIMG_MERGE_LNA -flashsize XPKG_VIRTIMG_LOAD_SIZE \
											-pkgmode 1 \
											-banoldtool 1 \
											-productname $PKG_PRODUCT"_SEC" \
										  -def ./gccout/$OUTPUT_NAME/ap/$PROJECT_NAME/mem_map.txt \
											-outfile ./gccout/$OUTPUT_NAME/ap/$PROJECT_NAME/$PROJECT_NAME.binpkg
	else
		# link pkg
                ./tools/fcelf -M  -input ./gccout/$OUTPUT_NAME/ap/bootloader/ap_bootloader.bin -addrname  BL_PKGIMG_LNA -flashsize BOOTLOADER_PKGIMG_LIMIT_SIZE \
                                        -input ./gccout/$OUTPUT_NAME/ap/$PROJECT_NAME/ap_$PROJECT_NAME.bin -addrname  AP_PKGIMG_LNA -flashsize AP_PKGIMG_LIMIT_SIZE \
                                        -input ./prebuild/FW/lib/gcc/$CHIP_TYPE/$CPBIN_SUBPATH/cp-demo-flash.bin -addrname CP_PKGIMG_LNA -flashsize CP_PKGIMG_LIMIT_SIZE \
                                        $pkg_cmd \
                                        -pkgmode 1 \
                                        -banoldtool 1 \
                                        -productname $PKG_PRODUCT \
                                        -def ./gccout/$OUTPUT_NAME/ap/$PROJECT_NAME/mem_map.txt \
                                        -outfile ./gccout/$OUTPUT_NAME/ap/$PROJECT_NAME/$PROJECT_NAME.binpkg
	fi

	fi
	
	if [ "$FIBOCOM_PKG_OPENSDK" == "true" ]; then
		if [ -e "opensdk.py" ]; then
		    #python3 opensdk.py fibo_soft ec71xx LE270-CN-1D5W-00-OPEN './prebuild/PLAT/lib/gcc/$CHIP_TYPE/oc'
			python3 opensdk.py $PROJECT_NAME $CHIP_TYPE $4 $libdir
			#./opensrc.sh
		fi
	fi
	
	if [ -f './tools/UpdateDBPattern.txt' ]
	then
		cat './tools/UpdateDBPattern.txt'
	fi
	#mv ./device/target/board/ec7xx_0h00/ap/gcc/sectionInfo_$CHIP_TYPE.json.back  ./device/target/board/ec7xx_0h00/ap/gcc/sectionInfo_$CHIP_TYPE.json

	restore_config
	completeHandle
fi

buildimage()
{
echo "buildimage"
echo > .failed.tmp
make -j$JOBNUMBER gccall TARGET=$BOARD_NAME V=$VERBOSE EUTRAN_MODE=$EUTRAN_MODE PROJECT=$PROJECT_NAME CORE=$CORE_NAME BUILD_UNILOG=$UNILOG BUILD_CUST=$CUST LSPD=$LSPD  MANUFACTURER=$MANUFACTURER  BUILD_ENV=$BUILD_ENV | rm .failed.tmp | tee '.\gccout\$BOARD_NAME\$CORE_NAME\outbuildlog.txt'

echo "buildimage over"

if [ -f .failed.tmp ]
then
   rm .failed.tmp
   failHandle
fi

}

if [ -n  "$(echo $PROJECT_NAME|grep 'bootloader')" ]
then
	buildimage
fi

if [ -n  "$(echo $PROJECT_NAME|grep 'driver_example')" ]
then
	buildimage
fi


