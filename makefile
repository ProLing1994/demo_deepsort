PROJECT_DIR:=$(shell pwd)
MAKE_SUB_DIR+= ${PROJECT_DIR}/deploy
RELEASE_ROOT_DIR:= ${PROJECT_DIR}/release

define MAKE_SUB_PROJECT
	@for sub_dir in ${MAKE_SUB_DIR}; do \
		cd $$sub_dir; \
		mkdir -p ${RELEASE_ROOT_DIR}/$$(basename $$sub_dir); \
		make $1 PREFIX=${RELEASE_ROOT_DIR}/$$(basename $$sub_dir); \
	done
endef

.PHONY:all
all:
	$(call MAKE_SUB_PROJECT, all)

.PHONY:clean
clean:
	$(call MAKE_SUB_PROJECT, clean)