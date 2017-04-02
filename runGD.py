import os
import subprocess as sub
import threading


class RunCmd(threading.Thread):
    def __init__(self, cmd, file, timeout):
        threading.Thread.__init__(self)
        self.cmd = cmd
        self.file = file
        self.timeout = timeout

    def run(self):
        f = open(self.file, "w")
        self.p = sub.Popen(self.cmd, stdout=f)
        self.p.wait()

    def Run(self):
        self.start()
        self.join(self.timeout)

        print "."
        if self.is_alive():
            self.p.terminate()      #use self.p.kill() if process needs a kill -9
            self.join()
            print "\n"


if __name__ == '__main__':
    os.system('g++ -std=c++11 solver_bab.cpp -o solver_bab')
    os.system('mkdir output')

    print "Corriendo CHRISTOFIDES"
    os.system('mkdir output/CHRISTOFIDES/')
    for i in range(1, 9):
        print "Corriendo P0" + str(i)
        RunCmd(["./solver_bab", "instanciasPRPP/CHRISTOFIDES/P0" + str(i) + "NoRPP"], "P0" + str(i) + ".txt", 60).Run()
        os.system('mv P0' + str(i) + '.txt output/CHRISTOFIDES/')

    for i in range(10, 20):
        print "Corriendo P" + str(i)
        RunCmd(["./solver_bab", "instanciasPRPP/CHRISTOFIDES/P" + str(i) + "NoRPP"], "P" + str(i) + ".txt", 60).Run()
        os.system('mv P' + str(i) + '.txt output/CHRISTOFIDES/')

    print "Corriendo DEGREE"
    os.system('mkdir output/DEGREE/')
    for i in range(0, 14):
        print "Corriendo D" + str(i)
        RunCmd(["./solver_bab", "instanciasPRPP/DEGREE/D" + str(i) + "NoRPP", " > D0" + str(i) + ".txt"], 60).Run()
        os.system('mv D' + str(i) + '.txt output/DEGREE/')
