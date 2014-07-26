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
CC=clang
CCC=clang++
CXX=clang++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=CLang-MacOSX
CND_DLIB_EXT=dylib
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/DAOs/PointsOfInterestDAO.o \
	${OBJECTDIR}/DAOs/RouteDAO.o \
	${OBJECTDIR}/DBConnection.o \
	${OBJECTDIR}/DBStatement.o \
	${OBJECTDIR}/DBUtil.o \
	${OBJECTDIR}/DataRequestDelegate.o \
	${OBJECTDIR}/GameCommandDelegate.o \
	${OBJECTDIR}/Models/PointOfInterest.o \
	${OBJECTDIR}/Models/Route.o \
	${OBJECTDIR}/Server.o \
	${OBJECTDIR}/ServiceLocator.o \
	${OBJECTDIR}/StringUtil.o \
	${OBJECTDIR}/SystemCommandDelegate.o \
	${OBJECTDIR}/main.o


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/gamedataserver

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/gamedataserver: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/gamedataserver ${OBJECTFILES} ${LDLIBSOPTIONS} -lsqlite3

${OBJECTDIR}/DAOs/PointsOfInterestDAO.o: DAOs/PointsOfInterestDAO.cpp 
	${MKDIR} -p ${OBJECTDIR}/DAOs
	${RM} "$@.d"
	$(COMPILE.cc) -g -IDAOs -IModels -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DAOs/PointsOfInterestDAO.o DAOs/PointsOfInterestDAO.cpp

${OBJECTDIR}/DAOs/RouteDAO.o: DAOs/RouteDAO.cpp 
	${MKDIR} -p ${OBJECTDIR}/DAOs
	${RM} "$@.d"
	$(COMPILE.cc) -g -IDAOs -IModels -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DAOs/RouteDAO.o DAOs/RouteDAO.cpp

${OBJECTDIR}/DBConnection.o: DBConnection.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -IDAOs -IModels -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DBConnection.o DBConnection.cpp

${OBJECTDIR}/DBStatement.o: DBStatement.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -IDAOs -IModels -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DBStatement.o DBStatement.cpp

${OBJECTDIR}/DBUtil.o: DBUtil.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -IDAOs -IModels -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DBUtil.o DBUtil.cpp

${OBJECTDIR}/DataRequestDelegate.o: DataRequestDelegate.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -IDAOs -IModels -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DataRequestDelegate.o DataRequestDelegate.cpp

${OBJECTDIR}/GameCommandDelegate.o: GameCommandDelegate.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -IDAOs -IModels -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/GameCommandDelegate.o GameCommandDelegate.cpp

${OBJECTDIR}/Models/PointOfInterest.o: Models/PointOfInterest.cpp 
	${MKDIR} -p ${OBJECTDIR}/Models
	${RM} "$@.d"
	$(COMPILE.cc) -g -IDAOs -IModels -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Models/PointOfInterest.o Models/PointOfInterest.cpp

${OBJECTDIR}/Models/Route.o: Models/Route.cpp 
	${MKDIR} -p ${OBJECTDIR}/Models
	${RM} "$@.d"
	$(COMPILE.cc) -g -IDAOs -IModels -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Models/Route.o Models/Route.cpp

${OBJECTDIR}/Server.o: Server.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -IDAOs -IModels -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Server.o Server.cpp

${OBJECTDIR}/ServiceLocator.o: ServiceLocator.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -IDAOs -IModels -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ServiceLocator.o ServiceLocator.cpp

${OBJECTDIR}/StringUtil.o: StringUtil.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -IDAOs -IModels -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/StringUtil.o StringUtil.cpp

${OBJECTDIR}/SystemCommandDelegate.o: SystemCommandDelegate.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -IDAOs -IModels -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SystemCommandDelegate.o SystemCommandDelegate.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -IDAOs -IModels -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/gamedataserver

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
