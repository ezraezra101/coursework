// Possible types of boid objects
Boid.possibleColors = ['Red', 'Green', 'Blue', 'Obstacle'];

Boid.sightThresholds = [20, 40, 11, 0];
Boid.speeds = [1,1,1, 0];
Boid.startAreaSize = 20;

// The bufferinfo for the boid objects
Boid.bufferInfo = undefined;

// The force put on p1 that prevents it from going too far offscreen.
function centeringForce(p1) {
	var dist = distance([0,0,0],p1);
	var dir = directionTo(p1, [0,0,0]);
	return [dir[0]*dist, dir[1]*dist, dir[2]*dist];
}

// Finds the distance between p1 and p2
function distance(p1, p2) {
	return Math.sqrt(
		Math.pow(p1[0]-p2[0], 2) +
		Math.pow(p1[1]-p2[1], 2) +
		Math.pow(p1[2]-p2[2], 2)
		);
}

// Returns a unit vector p1 to p2
function directionTo(p1, p2) {
	var dist = distance(p1, p2)
	if(dist == 0) {
		return [0,0,0];
	}
	return [
		(p2[0] - p1[0])/dist,
		(p2[1] - p1[1])/dist,
		(p2[2] - p1[2])/dist,
	];
}

function Boid(color) {
	if(color == "Obstacle") {
		this.move = function() {};
	}

	this.velocity = [Math.random(), Math.random(), Math.random()];
	this.position = [
		Math.random()*Boid.startAreaSize-Boid.startAreaSize/2,
		Math.random()*Boid.startAreaSize-Boid.startAreaSize/2,
		Math.random()*Boid.startAreaSize-Boid.startAreaSize/2
		];
	//this.rotation = new Quaternion(Math.random()-0.5, Math.random()-0.5, Math.random()-0.5, Math.random()-0.5).normalize()
	this.setColor(color || Boid.possibleColors[Math.floor(Math.random() * Boid.possibleColors.length)])
}

Boid.setupBuffers = function(gl) {
	var o = parseOBJFile(document.getElementById('boidModel').value)[0];
	Boid.bufferInfo = twgl.createBufferInfoFromArrays(gl, {
		position: o.position,
		indices: o.indices,
	});
}

Boid.prototype.setColor = function(color) {
	if(Boid.possibleColors.indexOf(color) == -1) {
		console.log("Attempted to set illegal color")
		return;
	}
	this.color = color;
};

// Lazy collision detection
Boid.prototype.hasCollided = function(other) {
	return distance(other.position, this.position) < 2;
}

Boid.prototype.collisionEat = function(objects) {
	var _this = this;
	objects.forEach(function(o) {
		if(!(_this.color == o.color) && _this.hasCollided(o)) {
			if(_this.color == 'Red' && o.color == 'Green') {
				_this.setColor('Green');
			} else if(_this.color == 'Blue' && o.color == 'Red') {
				_this.setColor('Red')
			} else if(_this.color == 'Green' && o.color == 'Blue') {
				_this.setColor('Blue')
			}
		}
	});
}

// Gets the attraction to any other boid
Boid.prototype.getAttraction = function(other) {
	var _this = this;
	var sightThreshold = Boid.sightThresholds[Boid.possibleColors.indexOf(this.color)];
	var speed = Boid.speeds[Boid.possibleColors.indexOf(this.color)];

	var boidRules = [
		['Red', 'Red', 0],
		['Red', 'Blue', 1],
		['Red', 'Green', -1],
		['Blue', 'Red', -1],
		['Blue', 'Blue', 0],
		['Blue', 'Green', 1],
		['Green', 'Red', 1],
		['Green', 'Blue', -1],
		['Green', 'Green', 0]
	];

	var attr = [0,0,0];

	if(other == this) {
		return attr;
	}

	if(other instanceof Boid) {
		var dist = distance(this.position, other.position)
		var dir = directionTo(this.position, other.position);
		
		if(dist < sightThreshold) {
			if (other.color == "Obstacle") {
				var vel = []; 
				vec3.scale(vel, dir, 100/distance);

			} else if(other.color == this.color) {
				var a = dist < 10 ? -4 : 1;
				var acc = [];
				vec3.scale(acc, dir, a);
				vec3.add(attr, attr, acc);

				var vel = [];
				vec3.normalize(vel, other.velocity);
				vec3.add(attr, attr, vel);
			} else {
				var rule = boidRules.find(function(r) {
					return (r[0] == _this.color) && (r[1] == other.color);
				});

				vec3.scale(dir, dir, rule[2] * speed * (Math.random()+0.5))
				vec3.add(attr, attr, dir);
			}
		}
	}
	return attr;
}

// Calculate the forces applied to this object
Boid.prototype.calculateForces = function(objects) {
	var _this = this;
	var forces = [0,0,0];
	objects.forEach(function(o) {
		vec3.add(forces, forces, _this.getAttraction(o));
	});

	var fc = centeringForce(this.position);
	vec3.add(forces, forces, fc);

	// Dampening
	var fd = [];
	vec3.scale(fd, this.velocity, 0.8);
	vec3.negate(fd, fd);
	vec3.add(forces,forces,fd);

	return forces;
}

// Move the boid object as far as it would move over the last *elapsedTime* seconds
// Only calculate this as one timestep.
Boid.prototype.move = function(elapsedTime, objects) {
	this.position = [
		this.position[0] + this.velocity[0]*elapsedTime,
		this.position[1] + this.velocity[1]*elapsedTime,
		this.position[2] + this.velocity[2]*elapsedTime
	];
	var forces = this.calculateForces(objects);
	this.velocity = [
		this.velocity[0] + forces[0]*elapsedTime,
		this.velocity[1] + forces[1]*elapsedTime,
		this.velocity[2] + forces[2]*elapsedTime
	];

	this.collisionEat(objects);
}

// Draw the boid object in gl context
Boid.prototype.draw = function(gl, viewMat) {
	if(!Boid.bufferinfo) {
		Boid.setupBuffers(gl);
	}

	var color = this.color == 'Red' ? [1,0,0,1] :
		this.color == 'Blue' ? [0,0,1,1] :
		this.color == 'Green' ? [0,1,0,1] :
		[0.1, 0.1, 0.1, 1];

	var modelMat = mat4.create();
	mat4.translate(modelMat, modelMat,this.position);
	var uniforms = {
		modelMat: modelMat,
		viewMat: viewMat,
		color: color,
	};

	twgl.setBuffersAndAttributes(gl, programInfo, Boid.bufferInfo);
	twgl.setUniforms(programInfo, uniforms);

	twgl.drawBufferInfo(gl, gl.TRIANGLES, Boid.bufferInfo);
}