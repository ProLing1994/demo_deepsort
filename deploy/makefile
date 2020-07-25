PROJECT_DIR=$(shell pwd)
MAKE_SUB_DIR=${PROJECT_DIR}

define MAKE_SUB_PROJECT
	@for subdir in $(MAKE_SUB_DIR); do \
		echo $$subdir; \
		cd $$subdir; \
		make -f makefile_lib $1; \
		make -f makefile_demo $1; \
		make -f makefile_demo_test $1; \
	done
endef

.PHONY:all
all:
	$(call MAKE_SUB_PROJECT,all)

.PHONY:clean
clean:
	$(call MAKE_SUB_PROJECT,clean)
