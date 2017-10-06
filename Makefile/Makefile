BOARD= f4
PROJECT_DIR=
PROJECT_NAME=
DOC_DIR= /home/$(USER)/Documents/docHAL

rouge = \033[0;31m
vert = \033[0;32m
bleu = \033[0;34m
fin = \033[0m
vert_gras = \033[0;1;4;32m
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
	@echo "${rouge}+--------------------[ Compilation ]--------------------+ ${fin}"
	cd $(PROJECT_DIR) ;\
	make all
	cd $(ori);\

	@echo "${rouge}+----------------[ Compilation terminée ]---------------+ ${fin}"

flash:
	@echo "${rouge}+---------------------[ Transfert ]---------------------+ ${fin}"
	openocd -f openocd/$(BOARD).cfg -c "program $(PROJECT_DIR)/build/$(PROJECT_NAME).bin exit 0x08000000 reset"
	@echo "${rouge}+----------------[ Transfert terminé ]------------------+ ${fin}"

debug:
	@echo "${rouge}+-----------------------[ Debug ]-----------------------+ ${fin}"
	arm-none-eabi-gdb $(PROJECT_DIR)/build/$(PROJECT_NAME).elf --command=openocd/attach_$(BOARD).gdb
	@echo "${rouge}+-------------------[ Debug terminé ]-------------------+ ${fin}"

doc:
	xreader $(DOC_DIR)/docHAL-$(BOARD).pdf &

clean:
	cd $(PROJECT_DIR) ;\
	make clean
	cd $(ORI);\
	rm -f openocd/openocd.log
