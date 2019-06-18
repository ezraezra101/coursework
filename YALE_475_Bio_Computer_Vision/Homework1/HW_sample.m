%% Sample for HW submission
% This is a sample for how to submit your homework.

%% Section 1: code and comments
% Use "%% " to start a new section. Don't forget the space after "%%"!
% Use "% " to start your comments. 
x = 1;
x+1

%% Section 2: plots and images
% Use "%%" to show the results section by section. Use "figure" to start a new
% figure window. The default colormap for "imagesc" is jet, while for "imshow" it is grayscale.
% If you see a grayscale image after using "imagesc", it means you've plotted on the same figure
% window of the previous "imshow" command, which is wrong.

im = imread('cameraman.tif'); % <-- use "semicolon" to avoid showing the whole matrix!
imshow(im)
%separate figures by starting a new section below, without any title:
%%
figure;
imagesc(im)
axis image
%%
% Also, it is a good idea to close all the open images at the end of your
% file. To do this, start a new section and use "close all":
%%
close all

%% Section 3: "Publish" and more
% Try to "publish" your m-file. An html file is generated inside the folder
% "html". Open it, check the formatting and read the contents. Make sure that you have
% answered all the questions and that the document is nice and neat (e.g., do not show a
% 500-by-500 matrix!, figures are next to their corresponding answers, etc.).
%
% * Name your write-up m-file "homework_answer.m". (btw, this is how we create bullets!)
% * Second bullet
% * Third bullet
%some code here

%% Section 4: Submission
% Zip all relevant files and upload them to Canvas. Do NOT
% include the html folder that you used to test your write-up. The teaching staff will
% generate it themselves when grading.

