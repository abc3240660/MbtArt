#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/MbtArt.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/MbtArt.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS
SUB_IMAGE_ADDRESS_COMMAND=--image-address $(SUB_IMAGE_ADDRESS)
else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=009_System.c 000_Main.c 001_Tick_10ms.c 005_BNO055.c 007_Uart.c 006_Gpio.c 002_CLRC663.c 004_LB1938.c 012_CLRC663_NFC.c 019_ADC0.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/009_System.o ${OBJECTDIR}/000_Main.o ${OBJECTDIR}/001_Tick_10ms.o ${OBJECTDIR}/005_BNO055.o ${OBJECTDIR}/007_Uart.o ${OBJECTDIR}/006_Gpio.o ${OBJECTDIR}/002_CLRC663.o ${OBJECTDIR}/004_LB1938.o ${OBJECTDIR}/012_CLRC663_NFC.o ${OBJECTDIR}/019_ADC0.o
POSSIBLE_DEPFILES=${OBJECTDIR}/009_System.o.d ${OBJECTDIR}/000_Main.o.d ${OBJECTDIR}/001_Tick_10ms.o.d ${OBJECTDIR}/005_BNO055.o.d ${OBJECTDIR}/007_Uart.o.d ${OBJECTDIR}/006_Gpio.o.d ${OBJECTDIR}/002_CLRC663.o.d ${OBJECTDIR}/004_LB1938.o.d ${OBJECTDIR}/012_CLRC663_NFC.o.d ${OBJECTDIR}/019_ADC0.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/009_System.o ${OBJECTDIR}/000_Main.o ${OBJECTDIR}/001_Tick_10ms.o ${OBJECTDIR}/005_BNO055.o ${OBJECTDIR}/007_Uart.o ${OBJECTDIR}/006_Gpio.o ${OBJECTDIR}/002_CLRC663.o ${OBJECTDIR}/004_LB1938.o ${OBJECTDIR}/012_CLRC663_NFC.o ${OBJECTDIR}/019_ADC0.o

# Source Files
SOURCEFILES=009_System.c 000_Main.c 001_Tick_10ms.c 005_BNO055.c 007_Uart.c 006_Gpio.c 002_CLRC663.c 004_LB1938.c 012_CLRC663_NFC.c 019_ADC0.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/MbtArt.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=24FJ1024GA606
MP_LINKER_FILE_OPTION=,--script=p24FJ1024GA606.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/009_System.o: 009_System.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/009_System.o.d 
	@${RM} ${OBJECTDIR}/009_System.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  009_System.c  -o ${OBJECTDIR}/009_System.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/009_System.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -O0 -I"bsp/exp16/default" -I"system_config/exp16/default" -I"." -I".." -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/009_System.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/000_Main.o: 000_Main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/000_Main.o.d 
	@${RM} ${OBJECTDIR}/000_Main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  000_Main.c  -o ${OBJECTDIR}/000_Main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/000_Main.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -O0 -I"bsp/exp16/default" -I"system_config/exp16/default" -I"." -I".." -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/000_Main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/001_Tick_10ms.o: 001_Tick_10ms.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/001_Tick_10ms.o.d 
	@${RM} ${OBJECTDIR}/001_Tick_10ms.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  001_Tick_10ms.c  -o ${OBJECTDIR}/001_Tick_10ms.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/001_Tick_10ms.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -O0 -I"bsp/exp16/default" -I"system_config/exp16/default" -I"." -I".." -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/001_Tick_10ms.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/005_BNO055.o: 005_BNO055.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/005_BNO055.o.d 
	@${RM} ${OBJECTDIR}/005_BNO055.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  005_BNO055.c  -o ${OBJECTDIR}/005_BNO055.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/005_BNO055.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -O0 -I"bsp/exp16/default" -I"system_config/exp16/default" -I"." -I".." -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/005_BNO055.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/007_Uart.o: 007_Uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/007_Uart.o.d 
	@${RM} ${OBJECTDIR}/007_Uart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  007_Uart.c  -o ${OBJECTDIR}/007_Uart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/007_Uart.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -O0 -I"bsp/exp16/default" -I"system_config/exp16/default" -I"." -I".." -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/007_Uart.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/006_Gpio.o: 006_Gpio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/006_Gpio.o.d 
	@${RM} ${OBJECTDIR}/006_Gpio.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  006_Gpio.c  -o ${OBJECTDIR}/006_Gpio.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/006_Gpio.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -O0 -I"bsp/exp16/default" -I"system_config/exp16/default" -I"." -I".." -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/006_Gpio.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/002_CLRC663.o: 002_CLRC663.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/002_CLRC663.o.d 
	@${RM} ${OBJECTDIR}/002_CLRC663.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  002_CLRC663.c  -o ${OBJECTDIR}/002_CLRC663.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/002_CLRC663.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -O0 -I"bsp/exp16/default" -I"system_config/exp16/default" -I"." -I".." -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/002_CLRC663.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/004_LB1938.o: 004_LB1938.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/004_LB1938.o.d 
	@${RM} ${OBJECTDIR}/004_LB1938.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  004_LB1938.c  -o ${OBJECTDIR}/004_LB1938.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/004_LB1938.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -O0 -I"bsp/exp16/default" -I"system_config/exp16/default" -I"." -I".." -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/004_LB1938.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/012_CLRC663_NFC.o: 012_CLRC663_NFC.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/012_CLRC663_NFC.o.d 
	@${RM} ${OBJECTDIR}/012_CLRC663_NFC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  012_CLRC663_NFC.c  -o ${OBJECTDIR}/012_CLRC663_NFC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/012_CLRC663_NFC.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -O0 -I"bsp/exp16/default" -I"system_config/exp16/default" -I"." -I".." -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/012_CLRC663_NFC.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/019_ADC0.o: 019_ADC0.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/019_ADC0.o.d 
	@${RM} ${OBJECTDIR}/019_ADC0.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  019_ADC0.c  -o ${OBJECTDIR}/019_ADC0.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/019_ADC0.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -O0 -I"bsp/exp16/default" -I"system_config/exp16/default" -I"." -I".." -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/019_ADC0.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/009_System.o: 009_System.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/009_System.o.d 
	@${RM} ${OBJECTDIR}/009_System.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  009_System.c  -o ${OBJECTDIR}/009_System.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/009_System.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -O0 -I"bsp/exp16/default" -I"system_config/exp16/default" -I"." -I".." -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/009_System.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/000_Main.o: 000_Main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/000_Main.o.d 
	@${RM} ${OBJECTDIR}/000_Main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  000_Main.c  -o ${OBJECTDIR}/000_Main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/000_Main.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -O0 -I"bsp/exp16/default" -I"system_config/exp16/default" -I"." -I".." -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/000_Main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/001_Tick_10ms.o: 001_Tick_10ms.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/001_Tick_10ms.o.d 
	@${RM} ${OBJECTDIR}/001_Tick_10ms.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  001_Tick_10ms.c  -o ${OBJECTDIR}/001_Tick_10ms.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/001_Tick_10ms.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -O0 -I"bsp/exp16/default" -I"system_config/exp16/default" -I"." -I".." -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/001_Tick_10ms.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/005_BNO055.o: 005_BNO055.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/005_BNO055.o.d 
	@${RM} ${OBJECTDIR}/005_BNO055.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  005_BNO055.c  -o ${OBJECTDIR}/005_BNO055.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/005_BNO055.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -O0 -I"bsp/exp16/default" -I"system_config/exp16/default" -I"." -I".." -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/005_BNO055.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/007_Uart.o: 007_Uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/007_Uart.o.d 
	@${RM} ${OBJECTDIR}/007_Uart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  007_Uart.c  -o ${OBJECTDIR}/007_Uart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/007_Uart.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -O0 -I"bsp/exp16/default" -I"system_config/exp16/default" -I"." -I".." -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/007_Uart.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/006_Gpio.o: 006_Gpio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/006_Gpio.o.d 
	@${RM} ${OBJECTDIR}/006_Gpio.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  006_Gpio.c  -o ${OBJECTDIR}/006_Gpio.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/006_Gpio.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -O0 -I"bsp/exp16/default" -I"system_config/exp16/default" -I"." -I".." -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/006_Gpio.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/002_CLRC663.o: 002_CLRC663.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/002_CLRC663.o.d 
	@${RM} ${OBJECTDIR}/002_CLRC663.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  002_CLRC663.c  -o ${OBJECTDIR}/002_CLRC663.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/002_CLRC663.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -O0 -I"bsp/exp16/default" -I"system_config/exp16/default" -I"." -I".." -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/002_CLRC663.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/004_LB1938.o: 004_LB1938.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/004_LB1938.o.d 
	@${RM} ${OBJECTDIR}/004_LB1938.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  004_LB1938.c  -o ${OBJECTDIR}/004_LB1938.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/004_LB1938.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -O0 -I"bsp/exp16/default" -I"system_config/exp16/default" -I"." -I".." -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/004_LB1938.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/012_CLRC663_NFC.o: 012_CLRC663_NFC.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/012_CLRC663_NFC.o.d 
	@${RM} ${OBJECTDIR}/012_CLRC663_NFC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  012_CLRC663_NFC.c  -o ${OBJECTDIR}/012_CLRC663_NFC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/012_CLRC663_NFC.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -O0 -I"bsp/exp16/default" -I"system_config/exp16/default" -I"." -I".." -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/012_CLRC663_NFC.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/019_ADC0.o: 019_ADC0.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/019_ADC0.o.d 
	@${RM} ${OBJECTDIR}/019_ADC0.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  019_ADC0.c  -o ${OBJECTDIR}/019_ADC0.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/019_ADC0.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -O0 -I"bsp/exp16/default" -I"system_config/exp16/default" -I"." -I".." -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/019_ADC0.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/MbtArt.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/MbtArt.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)   -mreserve=data@0x800:0x81B -mreserve=data@0x81C:0x81D -mreserve=data@0x81E:0x81F -mreserve=data@0x820:0x821 -mreserve=data@0x822:0x823 -mreserve=data@0x824:0x827 -mreserve=data@0x82A:0x84F   -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,--defsym=__MPLAB_DEBUGGER_REAL_ICE=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST) 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/MbtArt.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/MbtArt.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST) 
	${MP_CC_DIR}\\xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/MbtArt.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf  
	
	@echo Normalizing hex file
	@"C:/Program Files (x86)/Microchip/MPLABX/v5.20/mplab_platform/platform/../mplab_ide/modules/../../bin/hexmate" --edf="C:/Program Files (x86)/Microchip/MPLABX/v5.20/mplab_platform/platform/../mplab_ide/modules/../../dat/en_msgs.txt" dist/${CND_CONF}/${IMAGE_TYPE}/MbtArt.${IMAGE_TYPE}.hex -odist/${CND_CONF}/${IMAGE_TYPE}/MbtArt.${IMAGE_TYPE}.hex

endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
