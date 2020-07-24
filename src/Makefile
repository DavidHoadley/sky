# Makefile for sun and moon position code

# It is intended that make be run from wihin the directory containing the
# source files (rather than say, its parent directory), mainly so that assertion
# error messages within C code that use the __FILE__ string contain the file's
# name without any path string attached.
#    Object and executable files are put in separate directories. These
# directories are created within the parent directory of this directory, if they
# do not already exist.
#    This version of the makefile assumes that your compiler has command-line
# options to create dependency files automatically (which are specified in the
# variable $(DEPFLAGS). If not, you may need to drop back to a quick and dirty
# solution, and create a variable C_HDRS := $(wildcard *.h), and replace the
# dependency for .c files with $(C_OBJS): $(BLD_DIR)/%.o: %.c $(C_HDRS)
# (i.e. making each .c file dependent on every .h file).

# where to put object (.o), dependency (.d), executable and documentation files
BLD_DIR := ../build
##DEP_DIR := $(BLD_DIR)
DEP_DIR := ../dep
BIN_DIR := ../bin
DOC_DIR := ../docs

# Get lists of the required object and dependency files
C_OBJS := $(patsubst %.c,$(BLD_DIR)/%.o,$(wildcard *.c))
C_OBJS := $(filter-out $(BLD_DIR)/version.o, $(C_OBJS))
P_OBJS := $(patsubst %.cpp,$(BLD_DIR)/%.o,$(wildcard *.cpp))
C_DEPS := $(patsubst %.c,$(DEP_DIR)/%.d,$(wildcard *.c))
P_DEPS := $(patsubst %.cpp,$(DEP_DIR)/%.d,$(wildcard *.cpp))

# Commands
##DOXYGEN := doxygen
DOXYGEN := /Applications/Doxygen.app/Contents/Resources/doxygen
MKDIR_P := mkdir -p
RM_R := rm -r

#echo suspend
ifeq ("$(VERBOSE)","1")
NO_ECHO :=
else
NO_ECHO := @
endif

#---------------------
# Extract a version number from git and cut it into three components which can
# then be passed as command-line -Ddefine options to the program when compiling
#   (assumes most recent git tag is of form Vx.yy or vx.yy or Vx-yy or vx-yy)
GIT_VER := $(shell git describe --tags --long --dirty=-1)

# Strip off the leading V or v, separate the numbers and strip off leading zeros
# (to make sure they are not interpreted as octal numbers). (Up to 3 of them)
GIT_VER := $(patsubst V%,%,$(patsubst v%,%,$(GIT_VER)))
GIT_VERSION := $(subst -, ,$(subst ., ,$(GIT_VER)))
GIT_VERSION := $(patsubst 000%,-%,$(GIT_VERSION))
GIT_VERSION := $(patsubst 00%,-%,$(GIT_VERSION))
GIT_VERSION := $(patsubst 0%,-%,$(GIT_VERSION))
GIT_VERSION := $(subst - , 0 , $(GIT_VERSION))
GIT_VERSION := $(patsubst -%,%,$(GIT_VERSION))

APP_REV_MAJ := $(word 1, $(GIT_VERSION))
APP_REV_MIN := $(word 2, $(GIT_VERSION))
APP_REV_PATCH := $(word 3, $(GIT_VERSION))
ifeq ($(words $(GIT_VERSION)), 5)
APP_REV_DIRTY := $(word 5, $(GIT_VERSION))
else
APP_REV_DIRTY := 0
endif

# Force recompile of the file which uses these version numbers, every time
##DUMMY:=$(shell touch version.c)
#---------------------


CFLAGS_VERS := -DAPP_REV_MAJ="$(APP_REV_MAJ)"
CFLAGS_VERS += -DAPP_REV_MIN=\"$(APP_REV_MIN)\"
CFLAGS_VERS += -DAPP_REV_PATCH=\"$(APP_REV_PATCH)\"
CFLAGS_VERS += -DAPP_REV_DIRTY=\"$(APP_REV_DIRTY)\"

CFLAGS_WARN := -Wall -Wextra
CFLAGS_WARN += -Wbad-function-cast
CFLAGS_WARN += -Wcast-align
CFLAGS_WARN += -Wcast-qual
CFLAGS_WARN += -Wcomment
CFLAGS_WARN += -Wconversion
CFLAGS_WARN += -Wdisabled-optimization
CFLAGS_WARN += -Wfloat-equal
CFLAGS_WARN += -Wformat-nonliteral
CFLAGS_WARN += -Wformat-security
CFLAGS_WARN += -Winit-self
CFLAGS_WARN += -Winline
CFLAGS_WARN += -Wmissing-declarations
CFLAGS_WARN += -Wmissing-format-attribute
CFLAGS_WARN += -Wmissing-include-dirs
CFLAGS_WARN += -Wmissing-prototypes
CFLAGS_WARN += -Wold-style-definition
CFLAGS_WARN += -Wpointer-arith
CFLAGS_WARN += -Wredundant-decls
CFLAGS_WARN += -Wshadow
CFLAGS_WARN += -Wsign-conversion
CFLAGS_WARN += -Wstrict-prototypes
CFLAGS_WARN += -Wswitch-default
CFLAGS_WARN += -Wundef
CFLAGS_WARN += -Wunused-macros
CFLAGS_WARN += -Wswitch-enum
CFLAGS_WARN += -Wwrite-strings

CXXFLAGS := -Wctor-dtor-privacy
CXXFLAGS += -Woverloaded-virtual
CXXFLAGS += -Wsign-promo -Wstrict-null-sentinel

LFLAGS := -g

DEPFLAGS = -MT $@ -MMD -MP -MF $(DEP_DIR)/$*.d
COMPILE.c = $(CC) $(DEPFLAGS) $(CFLAGS_WARN) -c

DOXYFLAGS := DOXYGEN_APP_VER=$(APP_REV_MAJ).$(APP_REV_MIN).$(APP_REV_PATCH)-$(APP_REV_DIRTY)



all:	$(BIN_DIR)/sun1     doc


##$(BLD_DIR)/version.o: version.c general.h version.h
##	@echo "Compiling" $<
##	$(NO_ECHO)gcc -c $(CFLAGS_WARN) $(CFLAGS_VERS) -DSW_REV_MAJ=$(APP_REV_MAJ) -DSW_REV_MIN=$(APP_REV_MIN) -DSW_REV_BLD=$(APP_REV_PATCH)  $< -o $@

$(C_OBJS): $(BLD_DIR)/%.o: %.c $(DEP_DIR)/%.d | $(DEP_DIR) $(BLD_DIR)
	@echo "Compiling" $<
	$(NO_ECHO)$(COMPILE.c) $(INCLUDE_DIRS) $< -o $@

$(P_OBJS): $(BLD_DIR)/%.o: %.cpp | $(DEP_DIR) $(BLD_DIR)
	@echo "Compiling" $<
	$(NO_ECHO)g++ -c $(FLAGS) $(CXXFLAGS) $(INCLUDE_DIRS) $< -o $@

$(BIN_DIR)/sun1:   $(P_OBJS) $(C_OBJS) $(BLD_DIR)/version.o | $(BIN_DIR)
	@echo "Linking" $@
	$(NO_ECHO)gcc $(LFLAGS) -o $(BIN_DIR)/sun1 $(P_OBJS) $(C_OBJS) $(L_OPTS)

doc:  $(DOC_DIR)/html/index.html

$(DOC_DIR)/html/index.html: $(C_OBJS) | $(DOC_DIR)
	@echo "Building documentation"
	$(NO_ECHO)cd .. && $(DOXYFLAGS) $(DOXYGEN) Doxyfile

$(BIN_DIR):
	@echo "Creating directory" $@
	$(NO_ECHO)$(MKDIR_P) $(BIN_DIR)

$(BLD_DIR):
	@echo "Creating directory" $@
	$(NO_ECHO)$(MKDIR_P) $(BLD_DIR)

$(DEP_DIR):
	@echo "Creating directory" $@
	$(NO_ECHO)$(MKDIR_P) $(DEP_DIR)

$(DOC_DIR):
	@echo "Creating directory" $@
	$(NO_ECHO)$(MKDIR_P) $(DOC_DIR)

$(C_DEPS):  | $(DEP_DIR)


clean:
	@echo "Removing object, intermediate and executable files"
	$(NO_ECHO)$(RM_R) $(BLD_DIR) $(DEP_DIR) $(BIN_DIR)

include $(wildcard $(C_DEPS))

 
