CONTIKI_PROJECT = attom-gateway

all: $(CONTIKI_PROJECT)

DEFINES += PROJECT_CONF_H=\"project-conf.h\"
CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"

WITH_IP64 = 1
CONTIKI_WITH_IPV6 = 1

CONTIKI=../..
include $(CONTIKI)/Makefile.include
