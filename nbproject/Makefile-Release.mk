#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/511e446a/ArbreAbstrait.o \
	${OBJECTDIR}/_ext/511e446a/Interpreteur.o \
	${OBJECTDIR}/_ext/511e446a/Lecteur.o \
	${OBJECTDIR}/_ext/511e446a/Symbole.o \
	${OBJECTDIR}/_ext/511e446a/SymboleValue.o \
	${OBJECTDIR}/_ext/511e446a/TableSymboles.o \
	${OBJECTDIR}/_ext/511e446a/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/m3105-tp5

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/m3105-tp5: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/m3105-tp5 ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/511e446a/ArbreAbstrait.o: ../tp5/ArbreAbstrait.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/511e446a
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/511e446a/ArbreAbstrait.o ../tp5/ArbreAbstrait.cpp

${OBJECTDIR}/_ext/511e446a/Interpreteur.o: ../tp5/Interpreteur.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/511e446a
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/511e446a/Interpreteur.o ../tp5/Interpreteur.cpp

${OBJECTDIR}/_ext/511e446a/Lecteur.o: ../tp5/Lecteur.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/511e446a
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/511e446a/Lecteur.o ../tp5/Lecteur.cpp

${OBJECTDIR}/_ext/511e446a/Symbole.o: ../tp5/Symbole.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/511e446a
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/511e446a/Symbole.o ../tp5/Symbole.cpp

${OBJECTDIR}/_ext/511e446a/SymboleValue.o: ../tp5/SymboleValue.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/511e446a
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/511e446a/SymboleValue.o ../tp5/SymboleValue.cpp

${OBJECTDIR}/_ext/511e446a/TableSymboles.o: ../tp5/TableSymboles.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/511e446a
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/511e446a/TableSymboles.o ../tp5/TableSymboles.cpp

${OBJECTDIR}/_ext/511e446a/main.o: ../tp5/main.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/511e446a
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/511e446a/main.o ../tp5/main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
