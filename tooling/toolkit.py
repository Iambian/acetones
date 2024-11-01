import sys,os,ctypes,subprocess,getopt,shutil,struct,time

np  = os.path.normpath
cwd = os.getcwd()
    
TEMP_DIR     = np(cwd+"/obj/")
TEMP_PNG_DIR = np(cwd+"/obj/png")
OUTPUT_DIR   = np(cwd+"/bin")
STATUS_FILE  = np(TEMP_DIR+'/curstate')

def GETIMGPATH(fname): return np(TEMP_PNG_DIR+"/"+fname)
def GETIMGNAMES():
    global TEMP_PNG_DIR
    return sorted([f for f in os.listdir(TEMP_PNG_DIR) if os.path.isfile(os.path.join(TEMP_PNG_DIR,f))])
def ensure_dir(d):
    if not os.path.isdir(d): os.makedirs(d)
    
ensure_dir(TEMP_DIR)
ensure_dir(TEMP_PNG_DIR)
ensure_dir(OUTPUT_DIR)

ENCODER_NAMES = {   1: "1B3X-ZX7",
                    2: "2B3X-ZX7",
                    3: "2B1X-ZX7",
                    4: "1B1X-ZX7",
                    5: "4C3X-ZX7",
                    6: "4A3X-ZX7",
}
FPSEG_BY_ENCODER = {    1:30,
                        2:15,
                        3:6,
                        4:10,
                        5:15,
                        6:10,
}



# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
# Miscellaneous
def readFile(fn):
    a = []
    with open(fn,"rb") as f:
        b = f.read(1)
        while b!=b'':
            a.append(ord(b))
            b = f.read(1)
    return a
def writeFile(fn,a):
    with open(fn,"wb+") as f:
        f.write(bytearray(a))

# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
# Export data to appvar
TI_VAR_PROG_TYPE = 0x05
TI_VAR_PROTPROG_TYPE = 0x06
TI_VAR_APPVAR_TYPE = 0x15
TI_VAR_FLAG_RAM = 0x00
TI_VAR_FLAG_ARCHIVED = 0x80

def tobytes(indata:list):
    outlist = list()
    for dat in indata:
        if isinstance(dat,str):
            for s in dat:
                outlist.append(ord(s))
        elif isinstance(dat,(bytes, bytearray, list)):
            for b in dat:
                outlist.append(b)
        else:
            outlist.append(dat)
    return bytearray(outlist)

def export8xv(filebase, filedata):
    # Ensure that filedata is a string
    if isinstance(filedata,list): 
        filedata = bytearray(filedata)
    # Add size bytes to file data as per (PROT)PROG/APPVAR data structure
    dsl = len(filedata)&0xFF
    dsh = (len(filedata)>>8)&0xFF
    filedata = bytearray([dsl,dsh])+filedata
    # Construct variable header
    vsl = len(filedata)&0xFF
    vsh = (len(filedata)>>8)&0xFF
    vh  = bytearray([0x0D,0x00,vsl,vsh,TI_VAR_APPVAR_TYPE])
    vh += tobytes(filebase.ljust(8,'\x00')[:8])
    vh += bytearray([0x00,TI_VAR_FLAG_ARCHIVED,vsl,vsh])
    # Pull together variable metadata for TI8X file header
    varentry = vh + filedata
    varsizel = len(varentry)&0xFF
    varsizeh = (len(varentry)>>8)&0xFF
    varchksum = sum([i for i in varentry])
    vchkl = varchksum&0xFF
    vchkh = (varchksum>>8)&0xFF
    # Construct TI8X file header
    h  = tobytes("**TI83F*")
    h += bytearray([0x1A,0x0A,0x00])
    h += tobytes("Rawr. Gravy. Steaks. Cherries!".ljust(42)[:42])  #Always makes comments exactly 42 chars wide.
    h += bytearray([varsizel,varsizeh])
    h += varentry
    h += bytearray([vchkl,vchkh])
    # Write data out to file
    writeFile(f"{filebase}.8xv", h)
    return


    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
