import os, sys
# g++ 08_NEAT/main.cpp -o 08_NEAT/main.exe -I"C:\MinGW\include" -L"C:\MinGW\lib" -lfreeglut -lopengl32 && 08_NEAT\main.exe
if __name__ == "__main__":
    projectId = sys.argv[1]
    folders = os.listdir()
    ids = list(map(lambda x: str(x)[:2], folders))
    indexOfFolder = ids.index(projectId)
    print(os.system(f"g++ {folders[indexOfFolder]}/main.cpp -o {folders[indexOfFolder]}/main.exe -I\"C:\\MinGW\\include\" -L\"C:\\MinGW\\lib\" -lfreeglut -lopengl32 && {folders[indexOfFolder]}\\main.exe"))