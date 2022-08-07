# README

Two commands available `install` and `uninstall`. 
- Install command will install the registry keys with user supplied command. Any missing registry keys required will be created, e.g. `shell`
- Uninstall will delete the Registry values from the Fodhelper bypass registry key, however does not delete the key itself to avoid causing OS issues when cleaning up

## install

`uacbypassfodhelper install "powershell.exe -c calc.exe"`

`fodhelper` needs to be started manually, this BOF just adds the registry keys required for the bypass

## uninstall

`uacbypassfodhelper uninstall`
