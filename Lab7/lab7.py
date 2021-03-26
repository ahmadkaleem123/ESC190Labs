energies = [[24,      22,      30,      15,      18,      19],
            [12,      23,      15,      23,      10,     15],
            [11,      13,      22,      13,      21,      14],
            [13,      15,      17,      28,      19,      21],
            [17,      17,      7,       27,      20,      19]]




def dp_energy(energy):
    h =  len(energy)
    w = len(energy[0])
    dp = [0] * h * w
    for i in range(w):
        dp[i] = energy[0][i]
    for j in range(1, h):
        for x in range(w):
            dp[j*w + x] =   dp[(j-1)*w + x] + energy[j][x]
            if(x-1 >= 0):
                if(dp[j*w + x] >  dp[(j-1)*w + x-1] + energy[j][x]):
                    dp[j*w + x] =  dp[(j-1)*w + x-1] + energy[j][x]
            if(x+1 < w):
                if(dp[j*w + x] >  dp[(j-1)*w + x+1] + energy[j][x]):
                    dp[j*w + x] =  dp[(j-1)*w + x+1] + energy[j][x]
    return min(dp[(h-1)*w:])
            
        

print(dp_energy(energies))