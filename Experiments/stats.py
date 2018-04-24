import math

def mean(ls):
    return (1.0*sum(ls))/len(ls)

def variance(ls, mean):
    sum = 0.0
    for el in ls:
        t = el - mean
        sum += t*t
    return sum / (len(ls)-1)

def printStats(filename, ls):
    meanVal = mean(ls)
    sigma2 = variance(ls, meanVal)
    sigma = math.sqrt(sigma2)
    print("Results for list {}:\n\tmean: {}\n\tvariance: {}\n\tstandard deviation: {}".format(filename,meanVal,sigma2,sigma))

def extract_values(filename):
    results = []
    with open(filename, 'r') as infile:
        line = infile.readline()
        while line:
            s = line.strip(' ').strip('\n')
            if(len(s) > 0):
                val = int(s)
                results.append(val)
            line = infile.readline()
    return results

filenames = ["6_raw.txt", "12_raw.txt", "18_raw.txt"]

for f in filenames:
    ls = extract_values(f)
    printStats(f, ls)

