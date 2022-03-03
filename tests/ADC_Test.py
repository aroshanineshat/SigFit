# Date: March 2nd
# Author: Arash Roshanineshat

from joblib import Parallel, delayed
import matplotlib.pyplot as plt
import SigFit
import math
import random
import os

def GenerateNoisySine(Freq, SamplingFreq, NumberOfPoints):
    output = []
    sample_number = range(0, NumberOfPoints)
    d = 1.0 / SamplingFreq
    for i in sample_number:
        sinePoint = 100 * math.sin(2*math.pi*Freq*d*i)
        sinePoint = sinePoint + (random.random()-0.5)*100
        output.append(sinePoint)
    return sample_number, output

def run_test(iter_, F_Inp):

    Fs    = 2000 #Hz
    Count = 1000

    x, y = GenerateNoisySine(F_Inp, Fs, Count)

    A, B, C = SigFit.FitThreeParam(y, F_Inp, Fs)
    FittedSine = []
    for i in x:
        omega = 2*math.pi*F_Inp*i/Fs 
        S = A*math.cos(omega) + B * math.sin(omega) + C
        FittedSine.append(S)
    
    plt.figure()
    plt.plot(x, y, label="ADC output")
    plt.plot(x, FittedSine, label="Fitted Sine Wave", color="r", linewidth=3)
    plt.grid()
    plt.ylim([-160, 160])
    #plt.legend()
    plt.title("Analog to Digital Converter Performance")
    plt.xlabel("Time Samples")
    plt.ylabel("Amp")
    plt.savefig("./plots/iter{}.png".format(iter_), dpi=600)

if __name__ == "__main__":
    if not os.path.isdir('./plots'):
        os.mkdir('./plots')
    iterations_ = range(0, 150)
    Parallel(n_jobs = 8)(delayed(run_test)(iterations_[i], 5) for i in iterations_)
