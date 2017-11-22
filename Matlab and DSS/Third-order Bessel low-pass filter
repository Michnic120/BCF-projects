clc; clear all; close all;

Fp = 1400;
fgc = 450;
Tp = 1/Fp;
wgc = 2*pi*fgc;
w = 0:0.01:4;


L =  1;
M = [1 1.73 1];
H = freqs(L,M,w);
figure(1); plot(w,abs(H)); grid on;

wga = 0.79;  %spadek 3dB
A = wga * cot(wgc*Tp/2);
Lz = [1 2 1];
Mz=[(A^2+1.73*A+1) (2-2*A^2) (A^2-1.73*A+1)];
[Hz,f] = freqz(Lz, Mz,1000,Fp);
figure(2); plot(f ,abs(Hz)); grid on;


L1 =  1;
M1 = [1 2.43 2.47 1];
H1 = freqs(L1,M1,w);
figure(3); plot(w,abs(H1)); grid on;

wga1 = 0.71;  %spadek 3dB
A1 = wga1 * cot(wgc*Tp/2);
Lz1 = [1 3 3 1];
Mz1 = [A1^3+2.43*A1^2+2.47*A1+1   -3*A1^3-2.43*A1^2+2.47*A1+3  3*A1^3-2.43*A1^2-2.47*A1+3   -A1^3+2.43*A1^2-2.47*A1+1];

[Hz1,f1] = freqz(Lz1, Mz1,1000,Fp);
figure(4); plot(f1 ,abs(Hz1),'r','LineWidth', 2); grid on; hold on;
plot(f ,abs(Hz)); grid on;

%%

t = 0:1/Fp:0.1;
syg = sin(2*pi*50*t);

figure(8); 
plot(t,syg,'g','LineWidth', 2);
grid on; hold on;

syg1 = sin(2*pi*50*t) +sin(2*pi*150*t)+sin(2*pi*250*t)...
      +sin(2*pi*350*t)+sin(2*pi*450*t)+sin(2*pi*550*t);
plot(t,syg1,'r','LineWidth', 2);

%%widmo
y1 = filter(Lz,Mz, syg1);
f1= 50; N1 = (Fp/f1); I = N1*floor (length(syg1)/N1); w_ff = abs(fft(syg1,I)/(I/2))
ff = 0:(Fp/I):((Fp/2)-1);
figure(9); stem(ff,w_ff(1:(I/2))); grid on;


y2=filter(Lz, Mz, syg1);
w_ff2=abs(fft(y2,I)/(I/2));
figure(10); stem(ff, w_ff2(1:(I/2)), 'r'); grid on; hold on

y3=filter(Lz1, Mz1, syg);
w_ff3=abs(fft(y3,I)/(I/2));
stem(ff, w_ff3(1:(I/2)), 'b'); grid on;

%nalozyc na siebie II i III rzad
figure(11); dstep(Lz, Mz)
figure(12); impulse(Lz, Mz)
