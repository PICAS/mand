## Compile (specified) Lua scripts and install them to $lua_functions

# define Lua scripts to install here:

functions = fncStartup.lua fncAdminPasswd.lua \
	    fncPreVersionCheck.lua fncPostVersionCheck.lua

targets = $(functions:.lua=.out)

luafunctions_DATA = $(targets)

CLEANFILES = $(targets)

.lua.out :
	@LUAC@ @LUACFLAGS@ -o $@ $^

install-data-hook:
	@RENAME@ 's/\.out/.lua/' $(DESTDIR)$(luafunctionsdir)/*.out

