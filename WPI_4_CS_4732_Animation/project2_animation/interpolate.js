// Linear interpolated between p1 and p2.
function lerp(p0, p1, p2, p3, u) {
	return [
		p1[0]*(1-u) + p2[0]*u,
		p1[1]*(1-u) + p2[1]*u,
		p1[2]*(1-u) + p2[2]*u,
		p1[3]*(1-u) + p2[3]*u,
	];
}

var p;
function bSpline(p0,p1,p2,p3,u) {

	var us = [u*u*u, u*u, u, 1];
	var pP1 = [3*(p1[0]-p0[0]), 3*(p1[1]-p0[1]), 3*(p1[2]-p0[2]), 1];
	var pP2 = [3*(p3[0]-p2[0]), 3*(p3[1]-p2[1]), 3*(p3[2]-p2[2]), 1];

	var ps = [
		p1[0],p1[1],p1[2],1,//p0[3],
		pP1[0],pP1[1],pP1[2],1,
		pP2[0],pP2[1],pP2[2],1,
		p2[0],p2[1],p2[2],1,//p1[3],
	];


	p = ps;
	var m = [
		-1, 3, -3, 1,
		3, -6, 3, 0,
		-3, 3, 0, 0,
		1, 0, 0, 0
	];

	var mat = [];

	mat4.scalar.multiply(mat, m, ps);
	//mat4.scale(mat, mat, [1/6,1/6,1/6]);
	var vec = [];
	//console.log(ps, m, vec4.dot(us, [mat[0], mat[1], mat[3], mat[4]]));

	// return [
	// 	vec4.dot(us, [mat[0], mat[4], mat[8], mat[12]]),
	// 	vec4.dot(us, [mat[1], mat[5], mat[9], mat[13]]),
	// 	vec4.dot(us, [mat[2], mat[6], mat[10], mat[14]]),
	// 	vec4.dot(us, [mat[3], mat[7], mat[11], mat[15]])
	// 	];

	return [
		vec4.dot(us, [mat[0], mat[1], mat[2], mat[3]]),
		vec4.dot(us, [mat[4], mat[5], mat[6], mat[7]]),
		vec4.dot(us, [mat[8], mat[9], mat[10], mat[11]]),
		vec4.dot(us, [mat[12], mat[1], mat[14], mat[15]])
		];
/*
	var a = [];
	var b = [];
	vec4.subtract(a, p1, p0);
	vec4.scale(a, a, 1/3);
	vec4.subtract(b, p2, p3);
	vec4.scale(b, b, 1/3);
*/
}

// Finds a particular location between pstart and pend
function catmullRomSpline(plast,pstart,pend,pnext, u) {
		// The matrix method was painful... (multiplying by a row vector)
		// so I'm not using it
		var u2 = u*u, u3 = u*u*u;

		var pos = [0,0,0], vec = [];
		vec3.scale(vec, plast, -0.5*u+u2-0.5*u3);
		vec3.add(pos, pos, vec);
		vec3.scale(vec, pstart, 1+ 0 -2.5*u2 + 1.5*u3);
		vec3.add(pos,pos,vec);
		vec3.scale(vec, pend, 0+0.5*u+2*u2-1.5*u3);
		vec3.add(pos,pos,vec);
		vec3.scale(vec, pnext, 0+0-0.5*u2 + 0.5*u3);
		vec3.add(pos,pos,vec);
		return pos;
}

function getLocation(splines, time, interpolationFunction) {
	if(!interpolationFunction) {
		interpolationFunction=catmullRomSpline;
	}

	// Select the correct spline: (loops back to the beginning)
	var i=0;
	while(splines[i+1] && splines[i % splines.length].time < time) {
		time = time - splines[i%splines.length].time;
		i= i+1;
	}
	spline = splines[i%splines.length];

	//Parameter u goes from 0 to spline.points.length
	var u = (spline.points.length * time/spline.time) % spline.points.length;

	// Find the local control points:
	var segment = Math.floor(u);
	u = u % 1;
	var p0 = (spline.points[segment-1] || spline.points[segment]).position,
		p1 = spline.points[segment].position,
		p2 = spline.points[segment+1] ? spline.points[segment+1].position : p1,
		p3 = spline.points[segment+2] ? spline.points[segment+2].position : p2;
	return interpolationFunction(p0,p1,p2,p3,u);
}

function rotateLerp(a,b,c,d,u) {
	var vec = catmullRomSpline(a,b,c,d,u);
	var mat = [];
	mat4.identity(mat);
	mat4.rotateX(mat, mat, glMatrix.toRadian(vec[0]));
	mat4.rotateY(mat, mat, glMatrix.toRadian(vec[0]));
	mat4.rotateZ(mat, mat, glMatrix.toRadian(vec[0]));
	return mat;
}

function quaternionFlatCamullRom(a,b,c,d,u) {
	var r = glMatrix.toRadian;
	var p1 = Quaternion.fromEuler(r(a[0]), r(a[1]), r(a[2])).toArray(),
		p2 = Quaternion.fromEuler(r(b[0]), r(b[1]), r(b[2])).toArray(),
		p3 = Quaternion.fromEuler(r(c[0]), r(c[1]), r(c[2])).toArray(),
		p4 = Quaternion.fromEuler(r(d[0]), r(d[1]), r(d[2])).toArray();
	var vec = catmullRomSpline(p1, p2, p3, p4,u);
	var mat = [];
	return (new Quaternion(vec[0], vec[1], vec[2], vec[3])).toMatrix()
}

function quaternionLerp(a,b,c,d,u) {
	var r = glMatrix.toRadian;
	var p1 = Quaternion.fromEuler(r(a[0]), r(a[1]), r(a[2])).toArray(),
		p2 = Quaternion.fromEuler(r(b[0]), r(b[1]), r(b[2])).toArray(),
		p3 = Quaternion.fromEuler(r(c[0]), r(c[1]), r(c[2])).toArray(),
		p4 = Quaternion.fromEuler(r(d[0]), r(d[1]), r(d[2])).toArray();
	var vec = lerp(p1, p2, p3, p4,u);
	return (new Quaternion(vec[0], vec[1], vec[2], vec[3])).toMatrix()
}

function quaternionSlerp(a,b,c,d,u) {
	var r = glMatrix.toRadian;
	var p1 = Quaternion.fromEuler(r(a[0]), r(a[1]), r(a[2])).toArray(),
		p2 = Quaternion.fromEuler(r(b[0]), r(b[1]), r(b[2])).toArray(),
		p3 = Quaternion.fromEuler(r(c[0]), r(c[1]), r(c[2])).toArray(),
		p4 = Quaternion.fromEuler(r(d[0]), r(d[1]), r(d[2])).toArray();

	var theta = vec4.dot(p2,p3);
	var p2Influence = Math.sin((1-u) * theta)/Math.sin(theta);
	var p3Influence = Math.sin(u * theta)/Math.sin(theta);

	return (new Quaternion(
		p2[0]*p2Influence + p3[0]*p3Influence,
		p2[1]*p2Influence + p3[1]*p3Influence,
		p2[2]*p2Influence + p3[2]*p3Influence,
		p2[3]*p2Influence + p3[3]*p3Influence)).toMatrix()	
}

function getRotation(splines, time, interpolationFunction) {
	if(!interpolationFunction) {
		interpolationFunction=quaternionSlerp;
	}

	// Select the correct spline: (loops back to the beginning)
	var i=0;
	while(splines[i+1] && splines[i % splines.length].time < time) {
		time = time - splines[i%splines.length].time;
		i= i+1;
	}
	spline = splines[i%splines.length];

	//Parameter u goes from 0 to spline.points.length
	var u = (spline.points.length * time/spline.time) % spline.points.length;

	// Find the local control points:
	var segment = Math.floor(u);
	u = u % 1;
	var p0 = (spline.points[segment-1] || spline.points[segment]).rotation,
		p1 = spline.points[segment].rotation,
		p2 = spline.points[segment+1] ? spline.points[segment+1].rotation : p1,
		p3 = spline.points[segment+2] ? spline.points[segment+2].rotation : p2;
	return interpolationFunction(p0,p1,p2,p3,u);
}
