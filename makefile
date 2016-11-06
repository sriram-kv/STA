SOURCE=./src/*.cpp
MYPROGRAM=parser
MYINCLUDES=./include/*.h
CC=g++ 
#------------------------------------------------------------------------------

.PHONY: clean

all: clean $(MYPROGRAM)
$(MYPROGRAM): $(SOURCE)
	@echo "             ____ _____  _               "
	@echo "            / ___|_   _|/ \ //////////   "
	@echo "            \___ \ | | / _ \             "
	@echo "             ___) || |/ ___ \            "
	@echo " ///////////|____/ |_/_/   \_\           "
	@echo "                                         "
	@echo " Author : Sriram Kuppahalli Venkatesha"
	@echo " Email  : kuppa010@umn.edu"
	@echo ""
	@echo "Compiling..."
	@$(CC)-std=c++11 $(SOURCE) -I$(MYINCLUDES) -o$(MYPROGRAM) -O3
	@echo "Finished"
	@echo ""
	@echo "==============================================================================="
	@echo "1. ./$(MYPROGRAM) read_ckt  <netlist>          - to get the fanin/ fanout details"
	@echo "2. ./$(MYPROGRAM) read_nldm delays <nldm file> - to print cell delay LUTs"
	@echo "3. ./$(MYPROGRAM) read_nldm slews  <nldm file> - to print cell slew LUTs"
	@echo "4. ./$(MYPROGRAM) <netlist>                    - to print slack and critical path*"
	@echo "5. ./$(MYPROGRAM)                              - display usage"
	@echo "          * Requires "liblist.txt" in the current running directory"
	@echo "==============================================================================="
	@echo ""
clean:
	@echo ""
	@echo "Removing old Binaries and previous result files"
	@rm -f $(MYPROGRAM)
	@rm -f ckt_*.txt *_LUT.txt

