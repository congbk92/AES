
"openssl aes-128-ecb -nosalt -K 79656c6c6f77207375626d6172696e65 -in auto_testcase.py -out cipher.txt"
"openssl aes-128-ecb -d -nosalt -K 79656c6c6f77207375626d6172696e65 -in cipher.txt -out plain.txt"

"./../build/AES dec_ecb 128 79656c6c6f77207375626d6172696e65  cipher.txt plain.txt"

import os
import subprocess
import hashlib

KEY128 = "79656c6c6f77207375626d6172696e65"
KEY192 = "79656c6c6f77207375626d6172696e650000000000000000"
KEY256 = "79656c6c6f77207375626d6172696e6579656c6c6f77207375626d6172696e65"

def getCommand(isOpenssl, isEnc, cipher, keyLength, key, inFileName, outFileName):
    if isOpenssl:
        openssl_cmd = f"aes-{keyLength}-{cipher}"
        if not isEnc:
            openssl_cmd += " -d"
        cmd = f"openssl {openssl_cmd} -nosalt -K {key} -in {inFileName} -out {outFileName}"
    else:
        dirname = os.path.dirname(os.path.realpath(__file__))
        exe_dirname = os.path.join(dirname, "..", "build", "AES")
        enc_type = "enc" if isEnc else "dec"
        cmd = f"{exe_dirname} {enc_type}_{cipher} {keyLength} {key} {inFileName} {outFileName}"
    return cmd


def getListFile(file_type):
    dirname = os.path.dirname(os.path.realpath(__file__))
    files = [f for f in os.listdir(dirname) if os.path.isfile(f)]
    result = list()
    for f in files:
        if f == __file__:
            continue
        ext = f.split(".")[-1]
        if file_type == "plain" and ext not in ["openssl_enc", "openssl_dec", "aes_enc", "aes_dec"]:
            result.append(f)
        elif file_type ==  ext:
            result.append(f)
    return result

def md5(fname):
    hash_md5 = hashlib.md5()
    with open(fname, "rb") as f:
        for chunk in iter(lambda: f.read(4096), b""):
            hash_md5.update(chunk)
    return hash_md5.hexdigest()

def run():
    plain_files = getListFile("plain")
    for f in plain_files:
        cmd = getCommand(True, True, "ecb", "256", KEY256, f, f + ".openssl_enc")
        result = subprocess.check_output(cmd, shell = True)
        cmd = getCommand(False, True, "ecb", "256", KEY256, f, f + ".aes_enc")
        result = subprocess.check_output(cmd, shell = True)
        if md5(f + ".openssl_enc") != md5(f + ".aes_enc"):
            print("False")


if __name__ == "__main__":
    run()
    try:
        #test_ans()
        pass
    except AssertionError as e:
        raise
    #print(getListTestFile())
    print("Done")