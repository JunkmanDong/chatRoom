############################################
# 文  件：Makefile
# 
# 日  期：2019年 12月 13日
#
# 作  者：魏 赛 东
#
# Q Q 号：3235459847
############################################
exe_name=app

MYCC=gcc
MYCXX=g++
CCXX=$(MYCXX)

STANDARD:=-std=c++11
#STANDARD:=-mfloat-abi=softfp

SRCDIR:=
SRCDIR+=./
SRCDIR+=source/
SRCDIR+=source/clientTdef/

INCDIR:= 
INCDIR+=include/
INCDIR:=$(addprefix -I, $(INCDIR))

CLIBS := 
#CLIBS := $(addprefix -l, $(CLIBS))

CLIBSDIR := 
#CLIBSDIR := $(addprefix -L, $(CLIBSDIR))

LIBS_FLAGS :=  $(CLIBS) $(CLIBSDIR)

BUILD=build/

CCXX_PARAM:=

CC_PARAM:=-c -g -Wall 
CXX_PARAM:=-c -g -Wall 

SRC:=$(foreach i, \
	$(SRCDIR), \
	$(wildcard $(addprefix $(i)*, .cpp) ) \
	)
SRC+=$(foreach i, \
	$(SRCDIR), \
	$(wildcard $(addprefix $(i)*, .c) ) \
	)

OBJ:=$(addsuffix .o, $(basename $(SRC)) )

DEPS:=$(foreach i, $(OBJ:.o=.d), $(addprefix $(BUILD), $(i)))

vpath %.o $(BUILD)

%.o:%.cpp
	@echo [$(MYCXX)]---">"[$<]
	@$(MYCXX) $(CXX_PARAM) $< $(LIBS_FLAGS) $(STANDARD) -o $@ -MMD -MF $(BUILD)$*.d -MP $(INCDIR) ; \
	mv $@ $(BUILD)$@

%.o:%.c
	@echo [$(MYCC)]---">"[$<]
	@$(MYCC) $(CC_PARAM) $< $(LIBS_FLAGS) $(STANDARD) -o $@ -MMD -MF $(BUILD)$*.d -MP $(INCDIR) ; \
	mv $@ $(BUILD)$@

%/:
	@mkdir -p $@

.PHONY:all clean 
all:$(addprefix $(BUILD), $(SRCDIR)) $(exe_name)

$(exe_name):$(OBJ)
	@echo ---------------------------[$(CCXX)]---">"[$@]---------------------------
	@echo " "$(foreach i, $(addprefix $(BUILD), $(OBJ)), $(i)"\r\n ")
	@$(CCXX) $(CCXX_PARAM) $(addprefix $(BUILD), $(OBJ)) $(LIBS_FLAGS) $(STANDARD) -o $@ 

-include $(DEPS)

clean:
	rm -f $(exe_name)
	rm -rf $(BUILD)



