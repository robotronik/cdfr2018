BOARD=f3
PROJECT_DIR=Asser_moteur_z
PROJECT_NAME=Asser_moteur_z
DOC_DIR= /home/$(USER)/STM32Cube/
BINPATH= /usr/bin
CUSTOM_C_SOURCES=ax_12a.c,ax_12a_hal.c,crc_16.c,encoder.c,server.c,remote_call.c,Robotronik_corp_pid.c,robotronik_protocol.c,Z_axis.c,fsm_stack.c

rouge = \033[0;31m
vert = \033[0;32m
bleu = \033[0;34m
fin = \033[0m
vert_gras = \033[0;1;32m
fin_gras = \033[0;21m

ORI=$(CURDIR)

c:
	make comp

f:
	make flash

d:
	make debug

cf:
	make comp
	make flash

cfd:
	make comp
	make flash
	make debug

comp:
	@echo -e "${vert_gras}+--------------------[ Compilation ]--------------------+ ${fin}"
	cd $(PROJECT_DIR) ;\
	make -j5 all
	cd "${ORI}" ;\

	@echo -e "${vert_gras}+----------------[ Compilation terminée ]---------------+ ${fin}"

flash:
	@echo -e "${vert_gras}+---------------------[ Transfert ]---------------------+ ${fin}"
	openocd -f openocd/st_nucleo_$(BOARD).cfg -c "program $(PROJECT_DIR)/build/$(PROJECT_NAME).bin exit 0x08000000 reset"
	@echo -e "${vert_gras}+----------------[ Transfert terminé ]------------------+ ${fin}"

debug:
	@echo -e "${vert_gras}+-----------------------[ Debug ]-----------------------+ ${fin}"
	arm-none-eabi-gdb $(PROJECT_DIR)/build/$(PROJECT_NAME).elf --command=openocd/attach_$(BOARD).gdb
	@echo -e "${vert_gras}+-------------------[ Debug terminé ]-------------------+ ${fin}"

config:
	python configure.py $(PROJECT_DIR) $(BINPATH) $(CUSTOM_C_SOURCES)

doc:
	qpdfview $(DOC_DIR)/stm32$(BOARD)_doc.pdf &

clean:
	cd $(PROJECT_DIR) ;\
	make clean
	cd "${ORI}";\
	rm -f openocd/openocd.log
