function mothsim(firing_fn)
%MOTHSIM A virtual moth flight simulator.
%  MOTHSIM(FIRING_FUNCTION) simulates moth behavior. FIRING_FUNCTION should
%  be a function handle to a function taking a 256x256 grayscale image as
%  input, and returning a 4x1 boolean vector (vector of true or false
%  values) indicating which motor neurons should fire. The layout of the
%  vector should be as follows:
%
%    1 - Pitch up
%    2 - Pitch down
%    3 - Yaw right
%    4 - Yaw left
%
%  For example, the vector [true; false; false; true] will cause the moth
%  to rotate up and to the left. If multiple conflicting values are present
%  (e.g., both up and down motor neurons should fire), no rotation happens.
%
%  The simulation will run for 10000 steps. To stop it sooner, use Ctrl+C.

  pos = randn(1, 3);
  pos = (1 + 2 * rand) * pos / norm(pos);
  mothframe = setup_mothframe;
  moth_model = [0 1/2 0; 1/2 -1/2 1/4; 0 -1/2 0;
                -1/2 -1/2 1/4; 0 1/2 0; 0 -1/2 0] * 0.1;
  resolution = 256;
  noiselevel = 0.1;
  [viewrays, mask] = setup_viewrays(resolution);

  arrow_xs = [[-1 1 0]' [-1 1 0]' [1 1 1.5]' [-1 -1 -1.5]'];
  arrow_ys = [[1 1 1.5]' [-1 -1 -1.5]' [-1 1 0]' [-1 1 0]'];
  
  lightrad = 0.2;
  speed = 0.1;
  turnrate = pi/32;
  
  maxsteps = 10000;
  fire = false(4, 1);
  pos_hist = zeros(maxsteps, 3);
  img_handle = subplot(1, 2, 1);
  path_handle = subplot(1, 2, 2);
  firing_handle = axes('Position', [0.45 0.8 0.1 0.1]);  
  axis equal;
  axis off;
  for step = 1:maxsteps
    pos_hist(step,:) = pos;
    image = render_image;
    imshow(image, 'Parent', img_handle);
    set(gcf, 'CurrentAxes', path_handle);
    plot3(pos_hist(1:step,1), pos_hist(1:step,2), pos_hist(1:step,3));
    flapping_model = bsxfun(@times, [1 1 cos(step)], moth_model);
    xformed_model = bsxfun(@plus, pos', mothframe * flapping_model');
    hold on;
    plot3(xformed_model(1,:), xformed_model(2,:), xformed_model(3,:));
    scatter3(0, 0, 0, 'r', 'filled');
    hold off;
    axis equal;
    update_state(image);
    patch(arrow_xs, arrow_ys, shiftdim([fire, fire, 1 - fire], -1), ...
          'Parent', firing_handle);
    pause(0.05);
  end
  
  function update_state(image)
    fire = firing_fn(image);
    fire = double(fire(:));
    if any(size(fire)~= [4, 1])
      error('The motor neuron activation vector is not 4x1.');
    elseif any(abs(fire) > 1)
      error('The motor neuron activation vector has out of range values.');
    end
    
    theta = (fire(1) - fire(2)) * turnrate;
    mothframe = axis_angle_rotation(mothframe(:,1), theta) * mothframe;
    theta = (fire(4) - fire(3)) * turnrate;
    mothframe = axis_angle_rotation(mothframe(:,3), theta) * mothframe;
    
    desired_right = cross(mothframe(:,2), [0 0 1]');
    desired_right = desired_right / norm(desired_right);
    desired_up = cross(desired_right, mothframe(:,2));
    theta = atan2(mothframe(:,3)' * desired_right, ...
                  mothframe(:,3)' * desired_up) / 2;
    mothframe = axis_angle_rotation(mothframe(:,2), -theta) * mothframe;
    mothframe = bsxfun(@rdivide, mothframe, sqrt(sum(mothframe.^2)));
    pos = pos + speed * mothframe(:,2)';
  end
  
  function mtx = axis_angle_rotation(axis, angle)
    mtx = eye(3) * cos(angle) + (1 - cos(angle)) * axis(:) * axis(:)';
    ucross = [0 -axis(3) axis(2); axis(3) 0 -axis(1); -axis(2) axis(1) 0];
    mtx = mtx + sin(angle) * ucross;
  end

  function image = render_image
    image = rand(resolution, resolution) * noiselevel;
    candledir = 0 - pos;
    candledir = candledir / norm(candledir);
    
    c_dir = mothframe' * candledir(:);
    c_dist = norm(pos);
    v_dot_c = sum(bsxfun(@times, viewrays, shiftdim(c_dir, -2)), 3);
    viewray_candle_dists = sqrt(c_dist.^2 * (1 - v_dot_c.^2));
    viewray_candle_dists(v_dot_c < 0) = c_dist;
    image = image + exp(-max(viewray_candle_dists.^2 - lightrad, 0));
    image(image > 1) = 1;
    image(image < 0) = 0;
    image(~mask) = 0;
  end

  function mothframe = setup_mothframe
    mothdir = randn(1, 3);
    mothdir = mothdir / norm(mothdir);
    mothright = cross(mothdir, [0 0 1]);
    mothright = mothright / norm(mothright);
    mothup = cross(mothright, mothdir);
    mothframe = [mothright; mothdir; mothup]';
  end

  function [viewrays, mask] = setup_viewrays(resolution)
    coords = linspace(-1, 1, resolution);
    [gridxs, gridzs] = meshgrid(coords, -coords);
    p = 2;
    discriminant = abs(gridxs).^p + abs(gridzs).^p;
    ys = real((1 - discriminant).^(1/p));
    mask = discriminant <= 1;
    norms = sqrt(gridxs.^2 + ys.^2 + gridzs.^2);
    xs = gridxs ./ norms;
    ys = ys ./ norms;
    zs = gridzs ./ norms;
    viewrays = cat(3, 2 * ys .* xs, 2 * ys .* ys - 1, 2 * ys .* zs);
    viewrays = bsxfun(@times, viewrays, mask);
  end
end
