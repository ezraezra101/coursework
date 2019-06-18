% Run this with `mothsim(mothcontrol)`
function control_function = mothcontrol

  % Any setup code goes here. In particular, this is where you
  % will create your weight matrix.

  
  % FIRST PART OF YOUR CODE HERE
  
  % Planning discussion:
  % I want approximately 4 outputs, so my final matrix should have shape 4xn
  % Really, the only parts I care much about is dividing the original image
  % into quadrants, and calculate the two quadrants in the up (or other
  % direction), and subtract it from the other.
  
  % If I reshape the image as a column vector and divide it into quadrants,
  % I get something like:
  % [q1 * 128, q3 *128, ...(*128), q2*128, q4*128, ... *128]
  
  up = repmat([ones(1,128), zeros(1,128)], 1,256); % q1 and q2
  down = repmat([zeros(1,128), ones(1,128)], 1, 256); % q3 and q4
  left = [ones(1,256*128), zeros(1, 256*128)]; % q1 and q3
  right =[zeros(1,256*128), ones(1, 256*128)]; % q3 and q4
  
  W = [up - down;down - up;right-left; left-right;];
 
  % Despite its simplicity, this works pretty well.
  
  % This is great, but if we are heading *directly* away from the light,
  % the moth will continue to fly straight.
  % I'd be inclined to add in some noise to W (which feels dubious),
  % or to build some sort of sophisticated detection method - I'm currently
  % leaning towards collecting right quarter, left quarter, and middle, and
  % seeing if middle is smaller than left + right, if it is, activate UP
  % and not DOWN... it's still a bit of a kludge, and also involves more
  % logic than a matrix multiply.
  
  % Fortunately, this is quite unlikely, so I'm not going to worry about
  % it.
  
  
  % The next line outputs a handle to firing_function below.
  control_function = @firing_function;
  % (If you don't know what function handles are, they are basically a way of
  % storing functions in variables, so that a function can be called without 
  % needing to know its name.)
  

  function firing_pattern = firing_function(image)

    % Given the image (a 2-D matrix), evaluate the network response,
    % i.e., apply the matrix you created above to the input image. 
    % Your output is given by the "firing_pattern" output variable, which should 
    % be a 4x1 boolean vector (see the help for mothsim for information about which entries correspond
    % to which directions).

    % SECOND PART OF YOUR CODE HERE
    
    % I used to multiply W by a constant (e.g. 0.01) to signify that UP
    % must have 100 pixels worth more light than DOWN, but I've discovered
    % that this isn't necessary,
    % and simply saying that if UP has 1 more pixel's light's worth than
    % DOWN, go UP.
    product = W * image(:);
    
    firing_pattern = floor(max(0, min(product, 1)));
    
  end
end