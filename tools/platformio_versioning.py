import sys
import os

# Optional: Define Path to your local verison of https://github.com/JackGruber/auto_buildnumber
# If it is not located at a position same folder as `platformio_versioning.py`
autoversion = 'C:\\git\\meine\\auto_buildnumber'

try:
    import versioning
    autver_ok = True
except:
    if os.path.exists(autoversion):
        sys.path.insert(0, autoversion)
        try:
            import versioning
            autver_ok = True
        except:
            autver_ok = False
    else:
        autver_ok = False

if autver_ok == True:
    versioning.UpdateVersionFile("include/version.h", "DEFINEHEADER", False, "include/version_build.h")
