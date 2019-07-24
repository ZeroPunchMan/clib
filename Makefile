
CurDir := $(shell pwd)
SrcDir := $(CurDir)/src/
IncDir := $(CurDir)/inc/

Headers := $(shell ls ./inc)
Sources := $(shell ls ./src)
Ojects := $(shell for src in $(Sources); do echo -e "$${src/%.c/.o} \c"; done;)
LibName := clib.a

HeadPath := $(shell for src in $(shell ls ./inc); do echo -e $(IncDir)/$$src; done;)
SrcPath := $(shell for src in $(shell ls ./src); do echo -e $(SrcDir)/$$src; done;)
#ClibObj := $(shell for src in $(SrcPath); do echo -e "$${src/%.c/.o} \c"; done;)

OutputDir := ./build/debug/

QueTestName := queue_test
QueTestPath := $(CurDir)/testcase/$(QueTestName).c
QueTestObj := $(OutputDir)/$(QueTestName).o

DBG ?= 1

test:
	@echo $(Ojects)
# @echo $(SrcPath)
# @echo $(HeadPath)

all:
	@echo "all"

genslib: $(HeadPath) $(SrcPath)
	@echo gen slib
	@mkdir -vp $(OutputDir)
	@for src in $(Sources); do\
		gcc -std=c99 -I $(IncDir) -c $(SrcDir)/$$src -o $(OutputDir)/$${src/%.c/.o};\
	done  
	@rm -f $(OutputDir)/*.a
	@for obj in $(Ojects); do\
		ar -r $(OutputDir)/$(LibName) $(OutputDir)/$${obj}; \
	done

testcase: FORCE genslib queuetest
	

queuetest:
	@echo start queue test
	@gcc -std=c99 -I $(IncDir) -c $(QueTestPath) -o  $(OutputDir)/$(QueTestName).o
	@gcc -std=c99 -I $(IncDir) $(OutputDir)/$(LibName) $(OutputDir)/$(QueTestName).o -o $(OutputDir)/$(QueTestName).exe
	@$(OutputDir)/$(QueTestName).exe


.PHONY: FORCE
FORCE:

.PHONY: clean
clean:
	@rm -rf ./build/*

