BUILD:=./build.sh
CLEAN:=./clean.sh
ISO:=./iso.sh

all:
	@$(BUILD)

.PHONEY: clean all install

clean:
	@$(CLEAN)
	
install:
	@$(CLEAN)
	@$(ISO)