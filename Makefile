include hostdef/posix.mk

SUBDIRS = src

default: all
build: all

all:
	@for i in $(SUBDIRS); do \
		cd $$i; make build; cd ..; \
	done

clean:
	@for i in $(SUBDIRS); do \
		cd $$i; make clean; cd ..; \
	done

distclean: clean
	@for i in $(SUBDIRS); do \
		cd $$i; make distclean; cd ..; \
	done
		
.SILENT:
