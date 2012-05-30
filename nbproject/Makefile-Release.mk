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
CND_PLATFORM=GNU-Linux-x86
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/Connection.o \
	${OBJECTDIR}/ApplicationManager.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/Parser/XMLSymbol.o \
	${OBJECTDIR}/Parser/XPath.o \
	${OBJECTDIR}/PacketHandler/PacketSIP.o \
	${OBJECTDIR}/PacketHandler/PacketRTP.o \
	${OBJECTDIR}/Parser/XMLParser.o \
	${OBJECTDIR}/Parser/Symbol.o \
	${OBJECTDIR}/PacketHandler/SDP.o \
	${OBJECTDIR}/Parser/XPathNode.o \
	${OBJECTDIR}/PacketHandler/Packet.o \
	${OBJECTDIR}/Parser/SIPParser.o


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/steganoproxy

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/steganoproxy: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/steganoproxy ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/Connection.o: Connection.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/Connection.o Connection.cpp

${OBJECTDIR}/ApplicationManager.o: ApplicationManager.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/ApplicationManager.o ApplicationManager.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/Parser/XMLSymbol.o: Parser/XMLSymbol.cpp 
	${MKDIR} -p ${OBJECTDIR}/Parser
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/Parser/XMLSymbol.o Parser/XMLSymbol.cpp

${OBJECTDIR}/Parser/XPath.o: Parser/XPath.cpp 
	${MKDIR} -p ${OBJECTDIR}/Parser
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/Parser/XPath.o Parser/XPath.cpp

${OBJECTDIR}/PacketHandler/PacketSIP.o: PacketHandler/PacketSIP.cpp 
	${MKDIR} -p ${OBJECTDIR}/PacketHandler
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/PacketHandler/PacketSIP.o PacketHandler/PacketSIP.cpp

${OBJECTDIR}/PacketHandler/PacketRTP.o: PacketHandler/PacketRTP.cpp 
	${MKDIR} -p ${OBJECTDIR}/PacketHandler
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/PacketHandler/PacketRTP.o PacketHandler/PacketRTP.cpp

${OBJECTDIR}/Parser/XMLParser.o: Parser/XMLParser.cpp 
	${MKDIR} -p ${OBJECTDIR}/Parser
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/Parser/XMLParser.o Parser/XMLParser.cpp

${OBJECTDIR}/Parser/Symbol.o: Parser/Symbol.cpp 
	${MKDIR} -p ${OBJECTDIR}/Parser
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/Parser/Symbol.o Parser/Symbol.cpp

${OBJECTDIR}/PacketHandler/SDP.o: PacketHandler/SDP.cpp 
	${MKDIR} -p ${OBJECTDIR}/PacketHandler
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/PacketHandler/SDP.o PacketHandler/SDP.cpp

${OBJECTDIR}/Parser/XPathNode.o: Parser/XPathNode.cpp 
	${MKDIR} -p ${OBJECTDIR}/Parser
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/Parser/XPathNode.o Parser/XPathNode.cpp

${OBJECTDIR}/PacketHandler/Packet.o: PacketHandler/Packet.cpp 
	${MKDIR} -p ${OBJECTDIR}/PacketHandler
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/PacketHandler/Packet.o PacketHandler/Packet.cpp

${OBJECTDIR}/Parser/SIPParser.o: Parser/SIPParser.cpp 
	${MKDIR} -p ${OBJECTDIR}/Parser
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/Parser/SIPParser.o Parser/SIPParser.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/steganoproxy

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
