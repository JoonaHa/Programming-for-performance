import subprocess
for i in range(0, 100):
    #link = "https://www.cs.helsinki.fi/u/puglisi/PFP2020/assignment1/F" + str(i)
    #subprocess.run(["wget" , link])
    subprocess.run(["./sorted.out", "F" + str(i)])
