SUBDIRS=$(shell ls -d */)

define make_subdir
	@for subdir in $(SUBDIRS); do \
		(cd $$subdir && make $1) \
	done;
endef

all:
	$(call make_subdir)


clean:
	$(call make_subdir,clean)
