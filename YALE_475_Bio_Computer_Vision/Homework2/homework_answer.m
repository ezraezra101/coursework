%% Problem 1:
%% Problem 1a:
% With both an injected current of 0nA and 1nA, the neuron never fires,
% unlike 10nA, where it fires rapidly. At 0nA, the neuron decreases from 
% its resting potential until it reaches a limit (-70mV), whereas the 1nA
% is enough current to maintain -60mV.

membrane = -60;

ie = 0;
nsim = nsimulator(1);
nsim = nsim.set_mpotential(nsim, 1, membrane);
nsim = nsim.set_Ie(nsim, 1, ie);
nsim.animate_integration(nsim, 1, 0.2, 50, [1]);
%%
ie = 1;
nsim = nsimulator(1);
nsim = nsim.set_mpotential(nsim, 1, membrane);
nsim = nsim.set_Ie(nsim, 1, ie);
nsim.animate_integration(nsim, 2, 0.2, 50, [1]);
%%
ie = 10;
nsim = nsimulator(1);
nsim = nsim.set_mpotential(nsim, 1, membrane);
nsim = nsim.set_Ie(nsim, 1, ie);
nsim.animate_integration(nsim, 3, 0.2, 50, [1]);

%% Problem 1b:
% The behavior of the neuron is remarkably similar regardless of initial
% membrane potential (except that at 0mV, it immediately spikes and is set
% to -80mV): 1nA approaches -60mV, 10nA spikes rapidly, and 0nA approaches
% -70mV.

membrane = -80;

ie = 0;
nsim = nsimulator(1);
nsim = nsim.set_mpotential(nsim, 1, membrane);
nsim = nsim.set_Ie(nsim, 1, ie);
nsim.animate_integration(nsim, 1, 0.2, 50, [1]);
%%
ie = 1;
nsim = nsimulator(1);
nsim = nsim.set_mpotential(nsim, 1, membrane);
nsim = nsim.set_Ie(nsim, 1, ie);
nsim.animate_integration(nsim, 2, 0.2, 50, [1]);
%%
ie = 10;
nsim = nsimulator(1);
nsim = nsim.set_mpotential(nsim, 1, membrane);
nsim = nsim.set_Ie(nsim, 1, ie);
nsim.animate_integration(nsim, 3, 0.2, 50, [1]);


%%
membrane = 0;

ie = 0;
nsim = nsimulator(1);
nsim = nsim.set_mpotential(nsim, 1, membrane);
nsim = nsim.set_Ie(nsim, 1, ie);
nsim.animate_integration(nsim, 1, 0.2, 50, [1]);
%%
ie = 1;
nsim = nsimulator(1);
nsim = nsim.set_mpotential(nsim, 1, membrane);
nsim = nsim.set_Ie(nsim, 1, ie);
nsim.animate_integration(nsim, 2, 0.2, 50, [1]);
%%
ie = 10;
nsim = nsimulator(1);
nsim = nsim.set_mpotential(nsim, 1, membrane);
nsim = nsim.set_Ie(nsim, 1, ie);
nsim.animate_integration(nsim, 3, 0.2, 50, [1]);


%% Problem 1c
% Neuron 1 causes a (temporary) dip in neuron 2's potential by roughly 1mV each time it
% fires. Notably, this means that with 1nA as neuron 2's input, it never
% reaches -60mV, which it otherwise would.

ie2 = 0;
nsim = nsimulator(2);
nsim = nsim.set_mpotential(nsim, 1, -60);
nsim = nsim.set_Ie(nsim, 1, 2.5);
nsim = nsim.set_mpotential(nsim, 2, -70);
nsim = nsim.set_Ie(nsim, 2, ie2);
nsim = nsim.make_inhibitory_connection(nsim, 1, 2); % 1 effects 2
nsim.animate_integration(nsim, 3, 0.1, 50, [1, 2]);

%%

ie2 = 1;
nsim = nsimulator(2);
nsim = nsim.set_mpotential(nsim, 1, -60);
nsim = nsim.set_Ie(nsim, 1, 2.5);
nsim = nsim.set_mpotential(nsim, 2, -70);
nsim = nsim.set_Ie(nsim, 2, ie2);
nsim = nsim.make_inhibitory_connection(nsim, 1, 2); % 1 effects 2
nsim.animate_integration(nsim, 3, 0.1, 50, [1, 2]);

%%

ie2 = 10;
nsim = nsimulator(2);
nsim = nsim.set_mpotential(nsim, 1, -60);
nsim = nsim.set_Ie(nsim, 1, 2.5);
nsim = nsim.set_mpotential(nsim, 2, -70);
nsim = nsim.set_Ie(nsim, 2, ie2);
nsim = nsim.make_inhibitory_connection(nsim, 1, 2); % 1 effects 2
nsim.animate_integration(nsim, 3, 0.1, 50, [1, 2]);


%% Problem 1d
% Neuron 1 causes a temporary rise in neuron 2's potential by roughly 1mV each time it
% fires. Notably, when the injected current to neuron 2 is 1nA, the neuron
% spikes when it otherwise wouldn't.

ie2 = 0;
nsim = nsimulator(2);
nsim = nsim.set_mpotential(nsim, 1, -60);
nsim = nsim.set_Ie(nsim, 1, 2.5);
nsim = nsim.set_mpotential(nsim, 2, -70);
nsim = nsim.set_Ie(nsim, 2, ie2);
nsim = nsim.make_excitatory_connection(nsim, 1, 2); % 1 effects 2
nsim.animate_integration(nsim, 3, 0.1, 50, [1, 2]);

%%

ie2 = 1;
nsim = nsimulator(2);
nsim = nsim.set_mpotential(nsim, 1, -60);
nsim = nsim.set_Ie(nsim, 1, 2.5);
nsim = nsim.set_mpotential(nsim, 2, -70);
nsim = nsim.set_Ie(nsim, 2, ie2);
nsim = nsim.make_excitatory_connection(nsim, 1, 2); % 1 effects 2
nsim.animate_integration(nsim, 3, 0.1, 50, [1, 2]);

%%

ie2 = 10;
nsim = nsimulator(2);
nsim = nsim.set_mpotential(nsim, 1, -60);
nsim = nsim.set_Ie(nsim, 1, 2.5);
nsim = nsim.set_mpotential(nsim, 2, -70);
nsim = nsim.set_Ie(nsim, 2, ie2);
nsim = nsim.make_excitatory_connection(nsim, 1, 2); % 1 effects 2
nsim.animate_integration(nsim, 3, 0.1, 50, [1, 2]);



%% Problem 1e
% The inhibitory connection makes the spikes align, but the excitatory
% connection appears make the spikes alternate.
nsim = nsimulator(2);
nsim = nsim.set_mpotential(nsim, 1, -60);
nsim = nsim.set_Ie(nsim, 1, 2.5);
%nsim = nsim.set_mpotential(nsim, 2, -60);
nsim = nsim.set_Ie(nsim, 2, 2.5);
nsim = nsim.make_excitatory_connection(nsim, 1, 2); % 1 effects 2
nsim = nsim.make_excitatory_connection(nsim, 2, 1); % 1 effects 2
nsim = nsim.simulate_timeinterval(nsim, 1, 0.1);
nsim.plot_membranepotentials(nsim, 1, [1,2]);

%%
nsim = nsimulator(2);
nsim = nsim.set_mpotential(nsim, 1, -60);
nsim = nsim.set_Ie(nsim, 1, 2.5);
nsim = nsim.set_mpotential(nsim, 2, -60);
nsim = nsim.set_Ie(nsim, 2, 2.5);
nsim = nsim.make_inhibitory_connection(nsim, 1, 2); % 1 effects 2
nsim = nsim.make_inhibitory_connection(nsim, 2, 1); % 1 effects 2
nsim = nsim.simulate_timeinterval(nsim, 1, 0.1);
nsim.plot_membranepotentials(nsim, 1, [1,2]);





%% Problem 2
%% Problem 2a
I = eye(5);
I * [1,2,3,4,5].'

%% Problem 2b
% This doubles the output...
I2 = (2*I); % creates a diagonal matrix with 2s as its values.
I2 * [1,2,3,4,5].'

%% Problem 2c
% This performs a simple convolution related to box blur.
A = [1,1,0,0,0;
    1,1,1,0,0;
    0,1,1,1,0;
    0,0,1,1,1;
    0,0,0,1,1;
    ];
A * [1,2,3,4,5].'

%% Problem 2d

I11 = eye(11);
A = eye(10) - 0.2*I11(1:10, 2:11) - 0.2*I11(2:11, 1:10)

% A is a matrix that looks like [...;...0 0 -0.2, 1, 0.2 0 0 ...;...] with the 1 on
% the diagonal.

A2d = A;
%% Problem 2e
% Like we discussed in class, this highlights changes in color as well as
% adding a little noise around the borders.

x = [10, 10, 10, 10, 10, 20, 20, 20, 20, 20].';

result = A2d * x;
plot(horzcat(x, result));
legend('Original', 'Result');
figure()
colormap 'gray';
subplot(2,1, 1);
imagesc((x(2:9)).');
title('Original');
subplot(2,1, 2);
imagesc((result(2:9)).');
title('Result');

%% Problem 2f
% The neural network performs a blur and 'brightens' the vector.
% It convolves with kernel: [0.3, 1, 0.3]
I11 = eye(11);
A = eye(10) + I11(1:10, 2:11)/3 + I11(2:11, 1:10)/3

x = [10, 10, 10, 10, 10, 20, 20, 20, 20, 20].';
x2 = [10, 8, 10, 8, 10, 8, 10, 8, 10, 8].';

result = A * x;
result2 = A * x2;
plot(vertcat(result.', result2.').');
legend('A*transpose([10,10...20,20])', 'A*transpose([10, 8, 10, 8...])');
figure()
colormap 'gray';
subplot(2,1, 1);
imagesc((result(2:9)).');
title('A*transpose([10,10...20,20])');
subplot(2,1, 2);
imagesc((result2(2:9)).');
title('A*transpose([10, 8, 10, 8...])');

A2f = A;
%% Problem 2g
% Applying both the transformation in 2d and 2f is simple:
% A2f * (A2d * x) = (A2f * A2d) * x
% If we think of them as real convolutions and ignore borders, the new kernel is:
% [-.0666, .1333, .86666, .1333, -.0666] (kernel1 * kernel2)
% This a sharpen (A2d) and then a blur (A2f) operation.
% Interestingly, the average value of the vector shouldn't change
% (again, ignoring the edges of the vector)

x = 10 * ones(10, [1]);
A = A2f * A2d;

result = A * x; % Boring... results in [10, 10.666, 10... 10, 10.666, 10]
plot(result);
% result = A * x2 % is more interesting
%% Problem 3
% Please see `mothcontrol.m`.